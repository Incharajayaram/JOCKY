#include "../../forensic_types.h"
/**
 * Prefetch Parser Plugin
 * 
 * Parses Windows Prefetch files for execution history.
 * On Linux: parses shell history, recent files, desktop entries.
 */

#include "../../forensic_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ============================================================================
 * Prefetch Parser Implementation (Linux - history-based)
 * ============================================================================ */

static int prefetch_parser_init(void* config) {
    (void)config;
    printf("[prefetch_parser] Initialized\n");
    return 0;
}

static forensic_parsed_artifact_t* parse_prefetch(const forensic_bytes_t* data, void* config) {
    (void)config;
    
    forensic_parsed_artifact_t* artifact = forensic_parsed_artifact_create("prefetch", "prefetch_parser");
    if (!artifact) return NULL;
    
    const char* raw = (const char*)data->data;
    size_t len = data->len;
    
    if (len == 0) return artifact;
    
    char* copy = malloc(len + 1);
    memcpy(copy, raw, len);
    copy[len] = '\0';
    
    char* line = strtok(copy, "\n");
    int count = 0;
    
    while (line && count < 500) {
        if (line[0] != '#' && line[0] != '\0') {
            forensic_metadata_add(&artifact->parsed_data, "command", line);
            
            if (strstr(line, "curl") || strstr(line, "wget") ||
                strstr(line, "nc ") || strstr(line, "bash -i") ||
                strstr(line, "python") || strstr(line, "reverse")) {
                forensic_ioc_t* ioc = forensic_ioc_create("suspicious_command", line,
                    "prefetch_parser", 0.8);
                artifact->iocs.items = realloc(artifact->iocs.items,
                    (artifact->iocs.count + 1) * sizeof(forensic_ioc_t));
                artifact->iocs.items[artifact->iocs.count++] = *ioc;
                free(ioc);
            }
            count++;
        }
        line = strtok(NULL, "\n");
    }
    
    free(copy);
    
    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
    forensic_event_t* evt = forensic_event_create("prefetch_parsed",
        timestamp, "prefetch_parser", "Shell history parsed");
    artifact->timeline_events = realloc(artifact->timeline_events,
        (artifact->timeline_count + 1) * sizeof(forensic_event_t));
    artifact->timeline_events[artifact->timeline_count++] = *evt;
    free(evt);
    
    printf("[prefetch_parser] Parsed %d commands\n", count);
    return artifact;
}

static void prefetch_parser_cleanup(void) {
    printf("[prefetch_parser] Cleanup\n");
}

/* ============================================================================
 * Plugin Definition
 * ============================================================================ */

static const char* prefetch_capabilities[] = {
    "read_artifact",
    "parse_prefetch"
};

forensic_artifact_parser_plugin_t prefetch_parser_plugin = {
    .name = "prefetch_parser",
    .version = "1.0.0",
    .description = "Parses prefetch files and shell history for execution evidence",
    .artifact_type = "prefetch",
    .init = prefetch_parser_init,
    .parse = parse_prefetch,
    .cleanup = prefetch_parser_cleanup,
    .required_capabilities = prefetch_capabilities,
    .capability_count = sizeof(prefetch_capabilities) / sizeof(prefetch_capabilities[0]),
    .requires_sandbox = true,
    .max_cpu_time_ms = 5000,
    .max_memory_bytes = 32 * 1024 * 1024
};