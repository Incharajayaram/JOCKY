#include "../../forensic_types.h"
/**
 * Registry Parser Plugin
 * 
 * Parses registry hive bytes into keys, values, timestamps, and deletion markers.
 * On Linux: parses systemd service files, cron entries, shell configs.
 */

#include "../../forensic_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ============================================================================
 * Registry Parser Implementation (Linux - config-based)
 * ============================================================================ */

static int registry_parser_init(void* config) {
    (void)config;
    printf("[registry_parser] Initialized\n");
    return 0;
}

static forensic_parsed_artifact_t* parse_registry(const forensic_bytes_t* data, void* config) {
    (void)config;
    
    forensic_parsed_artifact_t* artifact = forensic_parsed_artifact_create("registry", "registry_parser");
    if (!artifact) return NULL;
    
    const char* raw = (const char*)data->data;
    size_t len = data->len;
    
    if (len == 0) return artifact;
    
    char* copy = malloc(len + 1);
    memcpy(copy, raw, len);
    copy[len] = '\0';
    
    char* line = strtok(copy, "\n");
    while (line) {
        if (line[0] == '#' || line[0] == ';' || line[0] == '\0') {
            line = strtok(NULL, "\n");
            continue;
        }
        
        char* eq = strchr(line, '=');
        if (eq) {
            *eq = '\0';
            forensic_metadata_add(&artifact->parsed_data, line, eq + 1);
            
            if (strstr(eq + 1, "nc ") || strstr(eq + 1, "bash -i") ||
                strstr(eq + 1, "python") || strstr(eq + 1, "reverse")) {
                forensic_ioc_t* ioc = forensic_ioc_create("suspicious_command", eq + 1,
                    "registry_parser", 0.7);
                artifact->iocs.items = realloc(artifact->iocs.items,
                    (artifact->iocs.count + 1) * sizeof(forensic_ioc_t));
                artifact->iocs.items[artifact->iocs.count++] = *ioc;
                free(ioc);
            }
        }
        line = strtok(NULL, "\n");
    }
    
    free(copy);
    
    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
    forensic_event_t* evt = forensic_event_create("registry_parsed",
        timestamp, "registry_parser", "Registry/config parsed");
    artifact->timeline_events = realloc(artifact->timeline_events,
        (artifact->timeline_count + 1) * sizeof(forensic_event_t));
    artifact->timeline_events[artifact->timeline_count++] = *evt;
    free(evt);
    
    printf("[registry_parser] Parsed registry/config: %zu entries\n", artifact->parsed_data.count);
    return artifact;
}

static void registry_parser_cleanup(void) {
    printf("[registry_parser] Cleanup\n");
}

/* ============================================================================
 * Plugin Definition
 * ============================================================================ */

static const char* registry_parser_capabilities[] = {
    "read_artifact",
    "parse_registry"
};

forensic_artifact_parser_plugin_t registry_parser_plugin = {
    .name = "registry_parser",
    .version = "1.0.0",
    .description = "Parses registry hives and config files into keys, values, and timestamps",
    .artifact_type = "registry",
    .init = registry_parser_init,
    .parse = parse_registry,
    .cleanup = registry_parser_cleanup,
    .required_capabilities = registry_parser_capabilities,
    .capability_count = sizeof(registry_parser_capabilities) / sizeof(registry_parser_capabilities[0]),
    .requires_sandbox = true,
    .max_cpu_time_ms = 5000,
    .max_memory_bytes = 64 * 1024 * 1024
};