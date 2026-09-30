/**
 * HTML Output Plugin
 * 
 * Generates analyst-friendly HTML report with interactive timeline view.
 */

#include "forensic_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ============================================================================
 * HTML Output Implementation
 * ============================================================================ */

static int html_output_init(void* config) {
    (void)config;
    printf("[html_output] Initialized\n");
    return 0;
}

static void escape_html(FILE* f, const char* str) {
    if (!str) return;
    for (const char* p = str; *p; p++) {
        switch (*p) {
            case '<': fprintf(f, "<"); break;
            case '>': fprintf(f, ">"); break;
            case '&': fprintf(f, "&"); break;
            case '"': fprintf(f, "&quot;"); break;
            default: fputc(*p, f);
        }
    }
}
static void write_js_functions(FILE* f) {
    fprintf(f,
        "<script>\n"
        "// Timeline filtering and search\n"
        "function filterTimeline() {\n"
        "    const searchTerm = document.getElementById('timelineSearch').value.toLowerCase();\n"
        "    const sourceFilter = document.getElementById('sourceFilter').value;\n"
        "    const startDate = document.getElementById('startDate').value;\n"
        "    const endDate = document.getElementById('endDate').value;\n"
        "    const items = document.querySelectorAll('.timeline-item');\n"
        "    let visible = 0;\n"
        "    items.forEach(item => {\n"
        "        const text = item.textContent.toLowerCase();\n"
        "        const source = item.querySelector('.timeline-source').textContent;\n"
        "        const timeStr = item.querySelector('.timeline-time').textContent;\n"
        "        \n"
        "        let match = true;\n"
        "        if (searchTerm && !text.includes(searchTerm)) match = false;\n"
        "        if (sourceFilter && !source.includes(sourceFilter)) match = false;\n"
        "        if (startDate) {\n"
        "            const itemDate = new Date(timeStr.replace('Z', '+00:00'));\n"
        "            if (itemDate < new Date(startDate)) match = false;\n"
        "        }\n"
        "        if (endDate) {\n"
        "            const itemDate = new Date(timeStr.replace('Z', '+00:00'));\n"
        "            const end = new Date(endDate);\n"
        "            end.setHours(23, 59, 59);\n"
        "            if (itemDate > end) match = false;\n"
        "        }\n"
        "        item.style.display = match ? '' : 'none';\n"
        "        if (match) visible++;\n"
        "    });\n"
        "    document.getElementById('timelineCount').textContent = visible;\n"
        "}\n"
        "\n"
        "// IOC table filtering\n"
        "function filterIOCs() {\n"
        "    const searchTerm = document.getElementById('iocSearch').value.toLowerCase();\n"
        "    const typeFilter = document.getElementById('typeFilter').value;\n"
        "    const minConf = parseFloat(document.getElementById('minConfidence').value) || 0;\n"
        "    const rows = document.querySelectorAll('.ioc-table tbody tr');\n"
        "    let visible = 0;\n"
        "    rows.forEach(row => {\n"
        "        const text = row.textContent.toLowerCase();\n"
        "        const type = row.cells[0].textContent;\n"
        "        const conf = parseFloat(row.cells[3].textContent) / 100;\n"
        "        \n"
        "        let match = true;\n"
        "        if (searchTerm && !text.includes(searchTerm)) match = false;\n"
        "        if (typeFilter && type !== typeFilter) match = false;\n"
        "        if (conf < minConf) match = false;\n"
        "        row.style.display = match ? '' : 'none';\n"
        "        if (match) visible++;\n"
        "    });\n"
        "    document.getElementById('iocCount').textContent = visible;\n"
        "}\n"
        "\n"
        "// Timeline zoom - show/hide details\n"
        "function toggleDetails(element) {\n"
        "    const details = element.nextElementSibling;\n"
        "    if (details && details.tagName === 'UL') {\n"
        "        details.style.display = details.style.display === 'none' ? 'block' : 'none';\n"
        "        element.textContent = details.style.display === 'none' ? 'Details' : 'Hide';\n"
        "    }\n"
        "}\n"
        "\n"
        "// Sort IOC table\n"
        "let sortDirection = 1;\n"
        "let lastSortedCol = -1;\n"
        "function sortTable(colIndex) {\n"
        "    const table = document.querySelector('.ioc-table');\n"
        "    const tbody = table.querySelector('tbody');\n"
        "    const rows = Array.from(tbody.querySelectorAll('tr'));\n"
        "    \n"
        "    if (colIndex === lastSortedCol) {\n"
        "        sortDirection *= -1;\n"
        "    } else {\n"
        "        sortDirection = 1;\n"
        "        lastSortedCol = colIndex;\n"
        "    }\n"
        "    \n"
        "    rows.sort((a, b) => {\n"
        "        const aText = a.cells[colIndex].textContent.trim();\n"
        "        const bText = b.cells[colIndex].textContent.trim();\n"
        "        \n"
        "        if (colIndex === 3) { // Confidence column\n"
        "            return sortDirection * (parseFloat(bText) - parseFloat(aText));\n"
        "        }\n"
        "        return sortDirection * aText.localeCompare(bText);\n"
        "    });\n"
        "    \n"
        "    rows.forEach(row => tbody.appendChild(row));\n"
        "    \n"
        "    // Update header indicators\n"
        "    table.querySelectorAll('th').forEach((th, i) => {\n"
        "        th.classList.remove('sorted-asc', 'sorted-desc');\n"
        "        if (i === colIndex) {\n"
        "            th.classList.add(sortDirection === 1 ? 'sorted-asc' : 'sorted-desc');\n"
        "        }\n"
        "    });\n"
        "}\n"
        "\n"
        "// Timeline zoom - show/hide items outside date range\n"
        "function zoomTimeline() {\n"
        "    filterTimeline();\n"
        "}\n"
        "\n"
        "// Expand/collapse all timeline details\n"
        "function toggleAllDetails(expand) {\n"
        "    document.querySelectorAll('.timeline-item details').forEach(d => {\n"
        "        d.open = expand;\n"
        "    });\n"
        "}\n"
        "\n"
        "// Highlight timeline item on hover\n"
        "document.addEventListener('DOMContentLoaded', function() {\n"
        "    document.querySelectorAll('.timeline-item').forEach(item => {\n"
        "        item.addEventListener('mouseenter', function() {\n"
        "            this.style.backgroundColor = '#f8f9fa';\n"
        "        });\n"
        "        item.addEventListener('mouseleave', function() {\n"
        "            this.style.backgroundColor = '';\n"
        "        });\n"
        "    });\n"
        "    \n"
        "    // Initialize filters\n"
        "    const timelineSearch = document.getElementById('timelineSearch');\n"
        "    const iocSearch = document.getElementById('iocSearch');\n"
        "    if (timelineSearch) timelineSearch.addEventListener('input', filterTimeline);\n"
        "    if (iocSearch) iocSearch.addEventListener('input', filterIOCs);\n"
        "    \n"
        "    // Populate source filter dropdown\n"
        "    const sources = new Set();\n"
        "    document.querySelectorAll('.timeline-source').forEach(el => {\n"
        "        const match = el.textContent.match(/Source: ([^|]+)/);\n"
        "        if (match) sources.add(match[1].trim());\n"
        "    });\n"
        "    const sourceFilter = document.getElementById('sourceFilter');\n"
        "    if (sourceFilter) {\n"
        "        sources.forEach(s => {\n"
        "            const opt = document.createElement('option');\n"
        "            opt.value = s;\n"
        "            opt.textContent = s;\n"
        "            sourceFilter.appendChild(opt);\n"
        "        });\n"
        "    }\n"
        "    \n"
        "    // Populate type filter dropdown\n"
        "    const types = new Set();\n"
        "    document.querySelectorAll('.ioc-table tbody tr').forEach(row => {\n"
        "        types.add(row.cells[0].textContent);\n"
        "    });\n"
        "    const typeFilter = document.getElementById('typeFilter');\n"
        "    if (typeFilter) {\n"
        "        types.forEach(t => {\n"
        "            const opt = document.createElement('option');\n"
        "            opt.value = t;\n"
        "            opt.textContent = t;\n"
        "            typeFilter.appendChild(opt);\n"
        "        });\n"
        "    }\n"
        "    \n"
        "    // Make IOC table headers clickable for sorting\n"
        "    document.querySelectorAll('.ioc-table th').forEach((th, i) => {\n"
        "        th.style.cursor = 'pointer';\n"
        "        th.addEventListener('click', () => sortTable(i));\n"
        "    });\n"
        "    \n"
        "    // Add keyboard navigation for timeline\n"
        "    document.addEventListener('keydown', function(e) {\n"
        "        const focused = document.activeElement;\n"
        "        if (focused.classList.contains('timeline-item')) {\n"
        "            if (e.key === 'ArrowDown') {\n"
        "                e.preventDefault();\n"
        "                const next = focused.nextElementSibling;\n"
        "                if (next && next.classList.contains('timeline-item')) next.focus();\n"
        "            } else if (e.key === 'ArrowUp') {\n"
        "                e.preventDefault();\n"
        "                const prev = focused.previousElementSibling;\n"
        "                if (prev && prev.classList.contains('timeline-item')) prev.focus();\n"
        "            } else if (e.key === 'Enter' || e.key === ' ') {\n"
        "                e.preventDefault();\n"
        "                const details = focused.querySelector('details');\n"
        "                if (details) details.open = !details.open;\n"
        "            }\n"
        "        }\n"
        "    });\n"
        "});\n"
        "</script>\n");
}

static int html_output_generate(const forensic_timeline_t* timeline,
                                 const forensic_ioc_list_t* iocs,
                                 const forensic_metadata_t* provenance,
                                 void* config) {
    const char* output_dir = config ? (const char*)config : ".";
    char output_path[512];
    snprintf(output_path, sizeof(output_path), "%s/forensic_report.html", output_dir);
    FILE* f = fopen(output_path, "w");
    if (!f) {
        fprintf(stderr, "[html_output] Failed to open output file: %s\n", output_path);
        return -1;
    }
    
    fprintf(f, "<!DOCTYPE html>\n<html>\n<head>\n");
    fprintf(f, "<meta charset=\"UTF-8\">\n");
    fprintf(f, "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
    fprintf(f, "<title>JOCKY Forensic Report</title>\n");
    fprintf(f, "<style>\n");
    fprintf(f, "body { font-family: 'Segoe UI', Arial, sans-serif; margin: 0; padding: 20px; background: #f4f4f4; }\n");
    fprintf(f, ".container { max-width: 1400px; margin: 0 auto; background: white; padding: 30px; border-radius: 8px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }\n");
    fprintf(f, "h1 { color: #2c3e50; border-bottom: 3px solid #3498db; padding-bottom: 10px; }\n");
    fprintf(f, "h2 { color: #34495e; margin-top: 30px; display: flex; justify-content: space-between; align-items: center; }\n");
    fprintf(f, "h3 { color: #7f8c8d; }\n");
    fprintf(f, ".meta { background: #ecf0f1; padding: 15px; border-radius: 5px; margin-bottom: 20px; display: flex; flex-wrap: wrap; gap: 20px; }\n");
    fprintf(f, ".meta span { display: inline-block; }\n");
    fprintf(f, ".controls { background: #f8f9fa; padding: 15px; border-radius: 5px; margin-bottom: 20px; display: flex; flex-wrap: wrap; gap: 15px; align-items: center; }\n");
    fprintf(f, ".controls input, .controls select { padding: 8px 12px; border: 1px solid #ddd; border-radius: 4px; font-size: 14px; }\n");
    fprintf(f, ".controls label { font-weight: bold; margin-right: 8px; }\n");
    fprintf(f, ".btn { padding: 8px 16px; background: #3498db; color: white; border: none; border-radius: 4px; cursor: pointer; font-size: 14px; }\n");
    fprintf(f, ".btn:hover { background: #2980b9; }\n");
    fprintf(f, ".btn-secondary { background: #95a5a6; }\n");
    fprintf(f, ".btn-secondary:hover { background: #7f8c8d; }\n");
    fprintf(f, ".timeline { border-left: 3px solid #3498db; padding-left: 20px; margin: 20px 0; }\n");
    fprintf(f, ".timeline-item { position: relative; padding: 15px 0 15px 20px; border-bottom: 1px solid #ecf0f1; cursor: pointer; transition: background 0.2s; }\n");
    fprintf(f, ".timeline-item::before { content: ''; position: absolute; left: -26px; top: 20px; width: 12px; height: 12px; border-radius: 50%; background: #3498db; }\n");
    fprintf(f, ".timeline-item:last-child { border-bottom: none; }\n");
    fprintf(f, ".timeline-item:hover { background: #f8f9fa; }\n");
    fprintf(f, ".timeline-item:focus { outline: 2px solid #3498db; outline-offset: -2px; }\n");
    fprintf(f, ".timeline-time { font-weight: bold; color: #2c3e50; font-family: monospace; }\n");
    fprintf(f, ".timeline-source { color: #7f8c8d; font-size: 0.9em; margin-bottom: 5px; }\n");
    fprintf(f, ".timeline-desc { margin-top: 5px; }\n");
    fprintf(f, ".timeline-item details { margin-top: 10px; }\n");
    fprintf(f, ".timeline-item summary { cursor: pointer; color: #3498db; font-weight: 500; }\n");
    fprintf(f, ".timeline-item details ul { margin: 5px 0 0 20px; }\n");
    fprintf(f, ".timeline-item details li { margin: 3px 0; }\n");
    fprintf(f, ".ioc-table { width: 100%; border-collapse: collapse; margin-top: 10px; font-size: 14px; }\n");
    fprintf(f, ".ioc-table th, .ioc-table td { padding: 12px; text-align: left; border-bottom: 1px solid #ecf0f1; }\n");
    fprintf(f, ".ioc-table th { background: #3498db; color: white; cursor: pointer; user-select: none; position: sticky; top: 0; }\n");
    fprintf(f, ".ioc-table th.sorted-asc::after { content: ' \\u25b2'; }\n");
    fprintf(f, ".ioc-table th.sorted-desc::after { content: ' \\u25bc'; }\n");
    fprintf(f, ".ioc-table tr:hover { background: #f8f9fa; }\n");
    fprintf(f, ".ioc-table code { background: #f1f1f1; padding: 2px 6px; border-radius: 3px; font-family: monospace; font-size: 13px; }\n");
    fprintf(f, ".confidence-high { color: #e74c3c; font-weight: bold; }\n");
    fprintf(f, ".confidence-medium { color: #f39c12; }\n");
    fprintf(f, ".confidence-low { color: #27ae60; }\n");
    fprintf(f, ".provenance { background: #fff3cd; border: 1px solid #ffc107; padding: 15px; border-radius: 5px; margin-top: 20px; }\n");
    fprintf(f, ".provenance h3 { margin-top: 0; color: #856404; }\n");
    fprintf(f, ".counter { font-weight: bold; color: #2c3e50; }\n");
    fprintf(f, ".hidden { display: none !important; }\n");
    fprintf(f, "details summary { list-style: none; }\n");
    fprintf(f, "details summary::-webkit-details-marker { display: none; }\n");
    fprintf(f, "details summary::before { content: '\\u25b8 '; transition: transform 0.2s; display: inline-block; }\n");
    fprintf(f, "details[open] summary::before { transform: rotate(90deg); }\n");
    fprintf(f, "@media (max-width: 768px) { .container { padding: 15px; } .controls { flex-direction: column; } .ioc-table { font-size: 12px; } }\n");
    fprintf(f, "</style>\n</head>\n<body>\n");
    fprintf(f, "<div class=\"container\">\n");
    fprintf(f, "<h1>JOCKY Forensic Report</h1>\n");
    
    fprintf(f, "<div class=\"meta\">\n");
    fprintf(f, "<span><strong>Generated:</strong> %ld</span>\n", (long)time(NULL));
    fprintf(f, "<span><strong>Timeline Events:</strong> <span class=\"counter\" id=\"timelineTotal\">%zu</span> (showing: <span class=\"counter\" id=\"timelineCount\">%zu</span>)</span>\n", timeline ? timeline->count : 0, timeline ? timeline->count : 0);
    fprintf(f, "<span><strong>IOCs Found:</strong> <span class=\"counter\" id=\"iocTotal\">%zu</span> (showing: <span class=\"counter\" id=\"iocCount\">%zu</span>)</span>\n", iocs ? iocs->count : 0, iocs ? iocs->count : 0);
    fprintf(f, "</div>\n");
    
    // Controls
    fprintf(f, "<div class=\"controls\">\n");
    fprintf(f, "<div><label>Timeline Search: </label><input type=\"text\" id=\"timelineSearch\" placeholder=\"Search timeline...\" oninput=\"filterTimeline()\"></div>\n");
    fprintf(f, "<div><label>Source: </label><select id=\"sourceFilter\" onchange=\"filterTimeline()\"><option value=\"\">All Sources</option></select></div>\n");
    fprintf(f, "<div><label>From: </label><input type=\"date\" id=\"startDate\" onchange=\"filterTimeline()\"></div>\n");
    fprintf(f, "<div><label>To: </label><input type=\"date\" id=\"endDate\" onchange=\"filterTimeline()\"></div>\n");
    fprintf(f, "<button class=\"btn btn-secondary\" onclick=\"toggleAllDetails(true)\">Expand All</button>\n");
    fprintf(f, "<button class=\"btn btn-secondary\" onclick=\"toggleAllDetails(false)\">Collapse All</button>\n");
    fprintf(f, "</div>\n");
    
    fprintf(f, "<div class=\"controls\" style=\"margin-top: 30px;\">\n");
    fprintf(f, "<div><label>IOC Search: </label><input type=\"text\" id=\"iocSearch\" placeholder=\"Search IOCs...\" oninput=\"filterIOCs()\"></div>\n");
    fprintf(f, "<div><label>Type: </label><select id=\"typeFilter\" onchange=\"filterIOCs()\"><option value=\"\">All Types</option></select></div>\n");
    fprintf(f, "<div><label>Min Confidence: </label><input type=\"number\" id=\"minConfidence\" min=\"0\" max=\"1\" step=\"0.1\" value=\"0\" onchange=\"filterIOCs()\"></div>\n");
    fprintf(f, "</div>\n");
    
    if (timeline && timeline->count > 0) {
        fprintf(f, "<h2>Timeline of Events <span class=\"counter\" id=\"timelineCount\">%zu</span> / %zu</h2>\n", timeline->count, timeline->count);
        fprintf(f, "<div class=\"timeline\" tabindex=\"0\">\n");
        for (size_t i = 0; i < timeline->count; i++) {
            const forensic_event_t* evt = &timeline->items[i];
            fprintf(f, "<div class=\"timeline-item\" tabindex=\"0\" data-index=\"%zu\">\n", i);
            fprintf(f, "<div class=\"timeline-time\">%s</div>\n", evt->timestamp ? evt->timestamp : "");
            fprintf(f, "<div class=\"timeline-source\">Source: %s | Type: %s</div>\n", 
                    evt->source_plugin ? evt->source_plugin : "", evt->event_type ? evt->event_type : "");
            fprintf(f, "<div class=\"timeline-desc\">");
            escape_html(f, evt->description ? evt->description : "");
            fprintf(f, "</div>\n");
            
            if (evt->details.count > 0) {
                fprintf(f, "<details>\n");
                fprintf(f, "<summary>Details (%zu items)</summary>\n", evt->details.count);
                fprintf(f, "<ul>\n");
                for (size_t j = 0; j < evt->details.count; j++) {
                    fprintf(f, "<li><strong>");
                    escape_html(f, evt->details.items[j].key);
                    fprintf(f, ":</strong> ");
                    escape_html(f, evt->details.items[j].value);
                    fprintf(f, "</li>\n");
                }
                fprintf(f, "</ul>\n");
                fprintf(f, "</details>\n");
            }
            fprintf(f, "</div>\n");
        }
        fprintf(f, "</div>\n");
    }
    
    if (iocs && iocs->count > 0) {
        fprintf(f, "<h2>Indicators of Compromise (IOCs) <span class=\"counter\" id=\"iocCount\">%zu</span> / %zu</h2>\n", iocs->count, iocs->count);
        fprintf(f, "<table class=\"ioc-table\">\n");
        fprintf(f, "<thead><tr><th onclick=\"sortTable(0)\">Type</th><th onclick=\"sortTable(1)\">Value</th><th onclick=\"sortTable(2)\">Source</th><th onclick=\"sortTable(3)\">Confidence</th></tr></thead>\n");
        fprintf(f, "<tbody>\n");
        for (size_t i = 0; i < iocs->count; i++) {
            const forensic_ioc_t* ioc = &iocs->items[i];
            const char* conf_class = ioc->confidence >= 0.8 ? "confidence-high" : 
                                    (ioc->confidence >= 0.5 ? "confidence-medium" : "confidence-low");
            fprintf(f, "<tr>\n");
            fprintf(f, "<td>%s</td>\n", ioc->type ? ioc->type : "");
            fprintf(f, "<td><code>");
            escape_html(f, ioc->value ? ioc->value : "");
            fprintf(f, "</code></td>\n");
            fprintf(f, "<td>%s</td>\n", ioc->source ? ioc->source : "");
            fprintf(f, "<td class=\"%s\">%.0f%%</td>\n", conf_class, ioc->confidence * 100);
            fprintf(f, "</tr>\n");
        }
        fprintf(f, "</tbody></table>\n");
    }
    
    if (provenance && provenance->count > 0) {
        fprintf(f, "<div class=\"provenance\">\n");
        fprintf(f, "<h3>Provenance Chain</h3>\n");
        fprintf(f, "<ul>\n");
        for (size_t i = 0; i < provenance->count; i++) {
            fprintf(f, "<li><strong>%s:</strong> ", provenance->items[i].key);
            escape_html(f, provenance->items[i].value);
            fprintf(f, "</li>\n");
        }
        fprintf(f, "</ul></div>\n");
    }
    
    write_js_functions(f);
    
    fprintf(f, "</div>\n</body>\n</html>\n");
    fclose(f);
    
    printf("[html_output] HTML report written to %s\n", output_path);
    return 0;
}

static void html_output_cleanup(void) {
    printf("[html_output] Cleanup\n");
}

/* ============================================================================
 * Plugin Definition
 * ============================================================================ */

static const char* html_capabilities[] = {
    "write_report"
};

forensic_output_plugin_t html_output_plugin = {
    .name = "html_output",
    .version = "1.0.0",
    .description = "Generates HTML report with interactive timeline view, filtering, and search",
    .init = html_output_init,
    .generate = html_output_generate,
    .cleanup = html_output_cleanup,
    .required_capabilities = html_capabilities,
    .capability_count = sizeof(html_capabilities) / sizeof(html_capabilities[0])
};
