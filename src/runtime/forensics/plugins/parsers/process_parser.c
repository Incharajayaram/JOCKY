#include "forensic_types.h"
/**
 * Process Parser Plugin
 * 
 * Parses process command lines, parent-child relationships, and executable paths.
 */

#include "forensic_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ============================================================================
 * Process Parser Implementation
 * ============================================================================ */

static int process_parser_init(void* config) {
    (void)config;
    printf("[process_parser] Initialized\n");
    return 0;
}

static forensic_parsed_artifact_t* parse_process(const forensic_bytes_t* data, void* config) {
    (void)config;
    
    forensic_parsed_artifact_t* artifact = forensic_parsed_artifact_create("process", "process_parser");
    if (!artifact) return NULL;
    
    const char* raw = (const char*)data->data;
    size_t len = data->len;
    
    if (len > 0) {
        forensic_metadata_add(&artifact->parsed_data, "cmdline", raw);
        
        // Extract executable name from cmdline
        const char* exe = strrchr(raw, '/');
        if (exe) exe++; else exe = raw;
        char* space = strchr(exe, ' ');
        if (space) {
            *space = '\0';
            forensic_metadata_add(&artifact->parsed_data, "exe_name", exe);
            *space = ' ';
        } else {
            forensic_metadata_add(&artifact->parsed_data, "exe_name", exe);
        }
        
        // IOC: suspicious commands
        if (strstr(raw, "nc ") || strstr(raw, "bash -i") ||
            strstr(raw, "python -c") || strstr(raw, "reverse") ||
            strstr(raw, "curl |") || strstr(raw, "wget |")) {
            forensic_ioc_t* ioc = forensic_ioc_create("suspicious_command", raw,
                "process_parser", 0.7);
            artifact->iocs.items = realloc(artifact->iocs.items,
                (artifact->iocs.count + 1) * sizeof(forensic_ioc_t));
            artifact->iocs.items[artifact->iocs.count++] = *ioc;
            free(ioc);
        }
    }
    
    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
    forensic_event_t* evt = forensic_event_create("process_parsed",
        timestamp, "process_parser", "Process command line parsed");
    artifact->timeline_events = realloc(artifact->timeline_events,
        (artifact->timeline_count + 1) * sizeof(forensic_event_t));
    artifact->timeline_events[artifact->timeline_count++] = *evt;
    free(evt);
    
    printf("[process_parser] Parsed process: %zu metadata entries\n", artifact->parsed_data.count);
    return artifact;
}

static void process_parser_cleanup(void) {
    printf("[process_parser] Cleanup\n");
}

/* ============================================================================
 * Plugin Definition
 * ============================================================================ */

static const char* process_parser_capabilities[] = {
    "read_artifact",
    "parse_process"
};

forensic_artifact_parser_plugin_t process_parser_plugin = {
    .name = "process_parser",
    .version = "1.0.0",
    .description = "Parses process command lines, executable names, and parent-child relationships",
    .artifact_type = "process",
    .init = process_parser_init,
    .parse = parse_process,
    .cleanup = process_parser_cleanup,
    .required_capabilities = process_parser_capabilities,
    .capability_count = sizeof(process_parser_capabilities) / sizeof(process_parser_capabilities[0]),
    .requires_sandbox = false,
    .max_cpu_time_ms = 1000,
    .max_memory_bytes = 16 * 1024 * 1024
};