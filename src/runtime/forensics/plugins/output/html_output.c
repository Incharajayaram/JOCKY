/**
 * HTML Output Plugin
 * 
 * Generates analyst-friendly HTML report with timeline view.
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
            case '"': fprintf(f, ""); break;
            case '\'': fprintf(f, "'"); break;
            default: fputc(*p, f);
        }
    }
}

static int html_output_generate(const forensic_timeline_t* timeline,
                                const forensic_ioc_list_t* iocs,
                                const forensic_metadata_t* provenance,
                                void* config) {
    const char* output_path = config ? (const char*)config : "forensic_report.html";
    FILE* f = fopen(output_path, "w");
    if (!f) {
        fprintf(stderr, "[html_output] Failed to open output file: %s\n", output_path);
        return -1;
    }
    
    fprintf(f, "<!DOCTYPE html>\n<html>\n<head>\n");
    fprintf(f, "<meta charset=\"UTF-8\">\n");
    fprintf(f, "<title>JOCKY Forensic Report</title>\n");
    fprintf(f, "<style>\n");
    fprintf(f, "body { font-family: 'Segoe UI', Arial, sans-serif; margin: 0; padding: 20px; background: #f4f4f4; }\n");
    fprintf(f, ".container { max-width: 1200px; margin: 0 auto; background: white; padding: 30px; border-radius: 8px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }\n");
    fprintf(f, "h1 { color: #2c3e50; border-bottom: 3px solid #3498db; padding-bottom: 10px; }\n");
    fprintf(f, "h2 { color: #34495e; margin-top: 30px; }\n");
    fprintf(f, "h3 { color: #7f8c8d; }\n");
    fprintf(f, ".meta { background: #ecf0f1; padding: 15px; border-radius: 5px; margin-bottom: 20px; }\n");
    fprintf(f, ".meta span { display: inline-block; margin-right: 20px; }\n");
    fprintf(f, ".timeline { border-left: 3px solid #3498db; padding-left: 20px; margin: 20px 0; }\n");
    fprintf(f, ".timeline-item { position: relative; padding: 15px 0 15px 20px; border-bottom: 1px solid #ecf0f1; }\n");
    fprintf(f, ".timeline-item::before { content: ''; position: absolute; left: -26px; top: 20px; width: 12px; height: 12px; border-radius: 50%; background: #3498db; }\n");
    fprintf(f, ".timeline-item:last-child { border-bottom: none; }\n");
    fprintf(f, ".timeline-time { font-weight: bold; color: #2c3e50; }\n");
    fprintf(f, ".timeline-source { color: #7f8c8d; font-size: 0.9em; }\n");
    fprintf(f, ".timeline-desc { margin-top: 5px; }\n");
    fprintf(f, ".ioc-table { width: 100%; border-collapse: collapse; margin-top: 10px; }\n");
    fprintf(f, ".ioc-table th, .ioc-table td { padding: 12px; text-align: left; border-bottom: 1px solid #ecf0f1; }\n");
    fprintf(f, ".ioc-table th { background: #3498db; color: white; }\n");
    fprintf(f, ".ioc-table tr:hover { background: #f8f9fa; }\n");
    fprintf(f, ".confidence-high { color: #e74c3c; font-weight: bold; }\n");
    fprintf(f, ".confidence-medium { color: #f39c12; }\n");
    fprintf(f, ".confidence-low { color: #27ae60; }\n");
    fprintf(f, ".provenance { background: #fff3cd; border: 1px solid #ffc107; padding: 15px; border-radius: 5px; margin-top: 20px; }\n");
    fprintf(f, ".provenance h3 { margin-top: 0; color: #856404; }\n");
    fprintf(f, "</style>\n</head>\n<body>\n");
    fprintf(f, "<div class=\"container\">\n");
    fprintf(f, "<h1>JOCKY Forensic Report</h1>\n");
    
    fprintf(f, "<div class=\"meta\">\n");
    fprintf(f, "<span><strong>Generated:</strong> %ld</span>\n", (long)time(NULL));
    fprintf(f, "<span><strong>Timeline Events:</strong> %zu</span>\n", timeline ? timeline->count : 0);
    fprintf(f, "<span><strong>IOCs Found:</strong> %zu</span>\n", iocs ? iocs->count : 0);
    fprintf(f, "</div>\n");
    
    if (timeline && timeline->count > 0) {
        fprintf(f, "<h2>Timeline of Events</h2>\n");
        fprintf(f, "<div class=\"timeline\">\n");
        for (size_t i = 0; i < timeline->count; i++) {
            const forensic_event_t* evt = &timeline->items[i];
            fprintf(f, "<div class=\"timeline-item\">\n");
            fprintf(f, "<div class=\"timeline-time\">%s</div>\n", evt->timestamp ? evt->timestamp : "");
            fprintf(f, "<div class=\"timeline-source\">Source: %s | Type: %s</div>\n", 
                    evt->source_plugin ? evt->source_plugin : "", evt->event_type ? evt->event_type : "");
            fprintf(f, "<div class=\"timeline-desc\">");
            escape_html(f, evt->description ? evt->description : "");
            fprintf(f, "</div>\n");
            
            if (evt->details.count > 0) {
                fprintf(f, "<details><summary>Details</summary><ul>\n");
                for (size_t j = 0; j < evt->details.count; j++) {
                    fprintf(f, "<li><strong>%s:</strong> ", evt->details.items[j].key);
                    escape_html(f, evt->details.items[j].value);
                    fprintf(f, "</li>\n");
                }
                fprintf(f, "</ul></details>\n");
            }
            fprintf(f, "</div>\n");
        }
        fprintf(f, "</div>\n");
    }
    
    if (iocs && iocs->count > 0) {
        fprintf(f, "<h2>Indicators of Compromise (IOCs)</h2>\n");
        fprintf(f, "<table class=\"ioc-table\">\n");
        fprintf(f, "<thead><tr><th>Type</th><th>Value</th><th>Source</th><th>Confidence</th></tr></thead>\n");
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
    .description = "Generates HTML report with interactive timeline view",
    .init = html_output_init,
    .generate = html_output_generate,
    .cleanup = html_output_cleanup,
    .required_capabilities = html_capabilities,
    .capability_count = sizeof(html_capabilities) / sizeof(html_capabilities[0])
};