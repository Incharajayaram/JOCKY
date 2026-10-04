#include "../../forensic_types.h"
/**
 * File Parser Plugin
 * 
 * Parses file paths, detects PE files, extracts basic metadata.
 */

#include "../../forensic_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ============================================================================
 * File Parser Implementation
 * ============================================================================ */

static int file_parser_init(void* config) {
    (void)config;
    printf("[file_parser] Initialized\n");
    return 0;
}

static forensic_parsed_artifact_t* parse_file(const forensic_bytes_t* data, void* config) {
    (void)config;
    
    forensic_parsed_artifact_t* artifact = forensic_parsed_artifact_create("file", "file_parser");
    if (!artifact) return NULL;
    
    const char* raw = (const char*)data->data;
    size_t len = data->len;
    
    if (len > 0) {
        forensic_metadata_add(&artifact->parsed_data, "path", raw);
        
        // Extract filename
        const char* fname = strrchr(raw, '/');
        if (fname) fname++; else fname = raw;
        forensic_metadata_add(&artifact->parsed_data, "filename", fname);
        
        // Check for suspicious paths
        if (strstr(raw, "/tmp/") || strstr(raw, "/dev/shm/") ||
            strstr(raw, "\\Temp\\") || strstr(raw, "\\AppData\\Local\\Temp\\")) {
            forensic_ioc_t* ioc = forensic_ioc_create("temp_path", raw,
                "file_parser", 0.6);
            artifact->iocs.items = realloc(artifact->iocs.items,
                (artifact->iocs.count + 1) * sizeof(forensic_ioc_t));
            artifact->iocs.items[artifact->iocs.count++] = *ioc;
            free(ioc);
        }
        
        // IOC: suspicious extensions
        if (strstr(fname, ".exe") || strstr(fname, ".dll") || strstr(fname, ".sys") ||
            strstr(fname, ".ps1") || strstr(fname, ".bat") || strstr(fname, ".scr")) {
            forensic_ioc_t* ioc = forensic_ioc_create("suspicious_extension", fname,
                "file_parser", 0.5);
            artifact->iocs.items = realloc(artifact->iocs.items,
                (artifact->iocs.count + 1) * sizeof(forensic_ioc_t));
            artifact->iocs.items[artifact->iocs.count++] = *ioc;
            free(ioc);
        }
    }
    
    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
    forensic_event_t* evt = forensic_event_create("file_parsed",
        timestamp, "file_parser", "File path parsed");
    artifact->timeline_events = realloc(artifact->timeline_events,
        (artifact->timeline_count + 1) * sizeof(forensic_event_t));
    artifact->timeline_events[artifact->timeline_count++] = *evt;
    free(evt);
    
    printf("[file_parser] Parsed file: %zu metadata entries\n", artifact->parsed_data.count);
    return artifact;
}

static void file_parser_cleanup(void) {
    printf("[file_parser] Cleanup\n");
}

/* ============================================================================
 * Plugin Definition
 * ============================================================================ */

static const char* file_parser_capabilities[] = {
    "read_artifact",
    "parse_file"
};

forensic_artifact_parser_plugin_t file_parser_plugin = {
    .name = "file_parser",
    .version = "1.0.0",
    .description = "Parses file paths, detects suspicious locations and extensions",
    .artifact_type = "file",
    .init = file_parser_init,
    .parse = parse_file,
    .cleanup = file_parser_cleanup,
    .required_capabilities = file_parser_capabilities,
    .capability_count = sizeof(file_parser_capabilities) / sizeof(file_parser_capabilities[0]),
    .requires_sandbox = false,
    .max_cpu_time_ms = 1000,
    .max_memory_bytes = 16 * 1024 * 1024
};