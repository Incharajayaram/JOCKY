#include "forensic_types.h"
#include "forensic_types.h"
#include "forensic_types.h"
/**
 * Analysis Engine Implementation
 */

#include "analysis.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>

/* ============================================================================
 * Helper Functions
 * ============================================================================ */

static int compare_events_by_time(const void* a, const void* b) {
    const forensic_event_t* ea = (const forensic_event_t*)a;
    const forensic_event_t* eb = (const forensic_event_t*)b;
    return strcmp(ea->timestamp ? ea->timestamp : "", eb->timestamp ? eb->timestamp : "");
}

static bool is_hash(const char* str) {
    if (!str) return false;
    size_t len = strlen(str);
    if (len == 32 || len == 40 || len == 64) {
        for (size_t i = 0; i < len; i++) {
            if (!isxdigit(str[i])) return false;
        }
        return true;
    }
    return false;
}

static bool is_ip(const char* str) {
    if (!str) return false;
    int dots = 0;
    for (const char* p = str; *p; p++) {
        if (*p == '.') dots++;
        else if (!isdigit(*p)) return false;
    }
    return dots == 3;
}

static bool is_domain(const char* str) {
    if (!str || strlen(str) > 253) return false;
    if (strchr(str, '.') == NULL) return false;
    for (const char* p = str; *p; p++) {
        if (!isalnum(*p) && *p != '.' && *p != '-') return false;
    }
    return true;
}

static bool is_file_path(const char* str) {
    if (!str) return false;
    return str[0] == '/' || (str[0] && str[1] == ':' && str[2] == '\\');
}

static bool is_registry_key(const char* str) {
    if (!str) return false;
    return strncmp(str, "HKLM\\", 5) == 0 || 
           strncmp(str, "HKCU\\", 5) == 0 ||
           strncmp(str, "HKCR\\", 5) == 0 ||
           strncmp(str, "HKU\\", 4) == 0;
}

static forensic_ioc_t* create_ioc_from_value(const char* type, const char* value,
                                              const char* source, double confidence) {
    if (!value || strlen(value) == 0) return NULL;
    return forensic_ioc_create(type, value, source, confidence);
}

static void add_ioc_if_new(forensic_ioc_list_t* iocs, forensic_ioc_t* ioc) {
    // Check for duplicates
    for (size_t i = 0; i < iocs->count; i++) {
        if (strcmp(iocs->items[i].value, ioc->value) == 0 &&
            strcmp(iocs->items[i].type, ioc->type) == 0) {
            // Update confidence if higher
            if (ioc->confidence > iocs->items[i].confidence) {
                iocs->items[i].confidence = ioc->confidence;
            }
            forensic_ioc_destroy(ioc);
            return;
        }
    }
    
    iocs->items = realloc(iocs->items, (iocs->count + 1) * sizeof(forensic_ioc_t));
    iocs->items[iocs->count++] = *ioc;
    free(ioc);
}

/* ============================================================================
 * A. Timeline Reconstruction
 * ============================================================================ */

int forensic_analysis_build_timeline(const forensic_parsed_artifact_list_t* parsed,
                                      forensic_timeline_t* timeline) {
    if (!parsed || !timeline) return -1;
    
    // Collect all timeline events from parsed artifacts
    size_t total_events = 0;
    for (size_t i = 0; i < parsed->count; i++) {
        total_events += parsed->items[i].timeline_count;
    }
    
    if (total_events == 0) return 0;
    
    timeline->items = calloc(total_events, sizeof(forensic_event_t));
    if (!timeline->items) return -1;
    
    size_t idx = 0;
    for (size_t i = 0; i < parsed->count; i++) {
        const forensic_parsed_artifact_t* art = &parsed->items[i];
        for (size_t j = 0; j < art->timeline_count; j++) {
            const forensic_event_t* src = &art->timeline_events[j];
            forensic_event_t* dst = &timeline->items[idx++];
            dst->event_type = src->event_type ? strdup(src->event_type) : NULL;
            dst->timestamp = src->timestamp ? strdup(src->timestamp) : NULL;
            dst->source_plugin = src->source_plugin ? strdup(src->source_plugin) : NULL;
            dst->description = src->description ? strdup(src->description) : NULL;
            dst->details = forensic_metadata_create(src->details.capacity);
            for (size_t k = 0; k < src->details.count; k++) {
                forensic_metadata_add(&dst->details, src->details.items[k].key, src->details.items[k].value);
            }
        }
    }
    timeline->count = idx;
    
    // Sort by timestamp
    qsort(timeline->items, timeline->count, sizeof(forensic_event_t), compare_events_by_time);
    
    printf("[analysis] Built timeline with %zu events\n", timeline->count);
    return 0;
}

/* ============================================================================
 * B. IOC Extraction
 * ============================================================================ */

int forensic_analysis_extract_iocs(const forensic_parsed_artifact_list_t* parsed,
                                    forensic_ioc_list_t* iocs) {
    if (!parsed || !iocs) return -1;
    
    iocs->items = NULL;
    iocs->count = 0;
    
    for (size_t i = 0; i < parsed->count; i++) {
        const forensic_parsed_artifact_t* art = &parsed->items[i];
        
        // Extract IOCs from parser output
        for (size_t j = 0; j < art->iocs.count; j++) {
            const forensic_ioc_t* src_ioc = &art->iocs.items[j];
            add_ioc_if_new(iocs, forensic_ioc_create(src_ioc->type, src_ioc->value,
                                                      src_ioc->source, src_ioc->confidence));
        }
        
        // Extract IOCs from parsed metadata
        for (size_t j = 0; j < art->parsed_data.count; j++) {
            const char* key = art->parsed_data.items[j].key;
            const char* value = art->parsed_data.items[j].value;
            
            if (is_hash(value)) {
                add_ioc_if_new(iocs, create_ioc_from_value("hash", value, art->parser_name, 0.9));
            } else if (is_ip(value)) {
                add_ioc_if_new(iocs, create_ioc_from_value("ip", value, art->parser_name, 0.8));
            } else if (is_domain(value)) {
                add_ioc_if_new(iocs, create_ioc_from_value("domain", value, art->parser_name, 0.7));
            } else if (is_file_path(value)) {
                add_ioc_if_new(iocs, create_ioc_from_value("file_path", value, art->parser_name, 0.8));
            } else if (is_registry_key(value)) {
                add_ioc_if_new(iocs, create_ioc_from_value("registry_key", value, art->parser_name, 0.8));
            }
        }
    }
    
    printf("[analysis] Extracted %zu IOCs\n", iocs->count);
    return 0;
}

/* ============================================================================
 * C. Correlation
 * ============================================================================ */

static bool events_related(const forensic_event_t* a, const forensic_event_t* b) {
    // Check for common elements
    if (a->details.count > 0 && b->details.count > 0) {
        for (size_t i = 0; i < a->details.count; i++) {
            for (size_t j = 0; j < b->details.count; j++) {
                if (strcmp(a->details.items[i].value, b->details.items[j].value) == 0 &&
                    strcmp(a->details.items[i].key, b->details.items[j].key) == 0) {
                    return true;
                }
            }
        }
    }
    return false;
}

int forensic_analysis_correlate(const forensic_timeline_t* timeline,
                                 const forensic_ioc_list_t* iocs,
                                 forensic_correlation_graph_t* graph) {
    if (!timeline || !graph) return -1;
    
    graph->correlations = NULL;
    graph->count = 0;
    
    // Correlate timeline events
    for (size_t i = 0; i < timeline->count; i++) {
        for (size_t j = i + 1; j < timeline->count; j++) {
            if (events_related(&timeline->items[i], &timeline->items[j])) {
                forensic_correlation_t corr = {0};
                snprintf(corr.source_event, sizeof(corr.source_event), "%s:%s",
                         timeline->items[i].source_plugin, timeline->items[i].event_type);
                snprintf(corr.target_event, sizeof(corr.target_event), "%s:%s",
                         timeline->items[j].source_plugin, timeline->items[j].event_type);
                snprintf(corr.relationship, sizeof(corr.relationship), "shared_artifact");
                corr.confidence = 0.7;
                
                graph->correlations = realloc(graph->correlations,
                    (graph->count + 1) * sizeof(forensic_correlation_t));
                graph->correlations[graph->count++] = corr;
            }
        }
    }
    
    // Correlate IOCs with timeline events
    for (size_t i = 0; i < iocs->count; i++) {
        const forensic_ioc_t* ioc = &iocs->items[i];
        for (size_t j = 0; j < timeline->count; j++) {
            const forensic_event_t* evt = &timeline->items[j];
            for (size_t k = 0; k < evt->details.count; k++) {
                if (strcmp(evt->details.items[k].value, ioc->value) == 0) {
                    forensic_correlation_t corr = {0};
                    snprintf(corr.source_event, sizeof(corr.source_event), "ioc:%s", ioc->type);
                    snprintf(corr.target_event, sizeof(corr.target_event), "%s:%s",
                             evt->source_plugin, evt->event_type);
                    snprintf(corr.relationship, sizeof(corr.relationship), "ioc_in_event");
                    corr.confidence = ioc->confidence;
                    
                    graph->correlations = realloc(graph->correlations,
                        (graph->count + 1) * sizeof(forensic_correlation_t));
                    graph->correlations[graph->count++] = corr;
                    break;
                }
            }
        }
    }
    
    printf("[analysis] Found %zu correlations\n", graph->count);
    return 0;
}

void forensic_correlation_graph_destroy(forensic_correlation_graph_t* graph) {
    if (!graph) return;
    free(graph->correlations);
    graph->correlations = NULL;
    graph->count = 0;
}

/* ============================================================================
 * Full Analysis Pipeline
 * ============================================================================ */

forensic_analysis_result_t* forensic_analysis_create(void) {
    forensic_analysis_result_t* result = calloc(1, sizeof(forensic_analysis_result_t));
    if (!result) return NULL;
    result->timeline = calloc(1, sizeof(forensic_timeline_t));
    result->iocs = calloc(1, sizeof(forensic_ioc_list_t));
    return result;
}

void forensic_analysis_destroy(forensic_analysis_result_t* result) {
    if (!result) return;
    if (result->timeline) {
        for (size_t i = 0; i < result->timeline->count; i++) {
            forensic_event_destroy(&result->timeline->items[i]);
        }
        free(result->timeline->items);
        free(result->timeline);
    }
    if (result->iocs) {
        for (size_t i = 0; i < result->iocs->count; i++) {
            forensic_ioc_destroy(&result->iocs->items[i]);
        }
        free(result->iocs->items);
        free(result->iocs);
    }
    free(result);
}

int forensic_analysis_run(const forensic_parsed_artifact_list_t* parsed,
                           forensic_analysis_result_t* result) {
    if (!parsed || !result) return -1;
    
    printf("[analysis] Starting analysis pipeline...\n");
    
    // A. Build timeline
    forensic_analysis_build_timeline(parsed, result->timeline);
    
    // B. Extract IOCs
    forensic_analysis_extract_iocs(parsed, result->iocs);
    
    // C. Correlate (store in result->parsed_artifacts temporarily)
    // We'll add a correlation field to the result later if needed
    
    printf("[analysis] Analysis pipeline complete\n");
    return 0;
}