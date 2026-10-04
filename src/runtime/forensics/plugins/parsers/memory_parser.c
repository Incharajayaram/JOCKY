#include "../../forensic_types.h"
/**
 * Memory Region Parser Plugin
 * 
 * Parses process memory mappings from /proc/<pid>/maps.
 */

#include "../../forensic_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ============================================================================
 * Memory Region Parser Implementation
 * ============================================================================ */

static int memory_parser_init(void* config) {
    (void)config;
    printf("[memory_parser] Initialized\n");
    return 0;
}

static forensic_parsed_artifact_t* parse_memory(const forensic_bytes_t* data, void* config) {
    (void)config;
    
    forensic_parsed_artifact_t* artifact = forensic_parsed_artifact_create("memory_region", "memory_parser");
    if (!artifact) return NULL;
    
    const char* raw = (const char*)data->data;
    size_t len = data->len;
    
    if (len == 0) return artifact;
    
    char* copy = malloc(len + 1);
    memcpy(copy, raw, len);
    copy[len] = '\0';
    
    char* line = strtok(copy, "\n");
    while (line) {
        unsigned long start, end;
        char perms[16];
        unsigned long offset;
        char dev[16];
        unsigned long inode;
        char pathname[512] = {0};
        
        if (sscanf(line, "%lx-%lx %15s %lx %15s %lu %511[^\n]",
                   &start, &end, perms, &offset, dev, &inode, pathname) >= 6) {
            char start_addr[32], end_addr[32], size_str[32];
            snprintf(start_addr, sizeof(start_addr), "0x%lx", start);
            snprintf(end_addr, sizeof(end_addr), "0x%lx", end);
            snprintf(size_str, sizeof(size_str), "%lu", end - start);
            
            char key[128];
            snprintf(key, sizeof(key), "region_%s", start_addr);
            char value[512];
            snprintf(value, sizeof(value), "end=%s perms=%s size=%s", end_addr, perms, size_str);
            if (pathname[0]) {
                strncat(value, " path=", sizeof(value) - strlen(value) - 1);
                strncat(value, pathname, sizeof(value) - strlen(value) - 1);
            }
            forensic_metadata_add(&artifact->parsed_data, key, value);
            
            // IOC: RWX regions (executable + writable = suspicious)
            if (strstr(perms, "x") && strstr(perms, "w")) {
                forensic_ioc_t* ioc = forensic_ioc_create("rwx_memory_region", start_addr,
                    "memory_parser", 0.8);
                artifact->iocs.items = realloc(artifact->iocs.items,
                    (artifact->iocs.count + 1) * sizeof(forensic_ioc_t));
                artifact->iocs.items[artifact->iocs.count++] = *ioc;
                free(ioc);
            }
            
            // IOC: Anonymous executable regions (no file backing)
            if (strstr(perms, "x") && (!pathname[0] || strstr(pathname, "[anon]"))) {
                forensic_ioc_t* ioc = forensic_ioc_create("anon_executable_region", start_addr,
                    "memory_parser", 0.7);
                artifact->iocs.items = realloc(artifact->iocs.items,
                    (artifact->iocs.count + 1) * sizeof(forensic_ioc_t));
                artifact->iocs.items[artifact->iocs.count++] = *ioc;
                free(ioc);
            }
            
            // IOC: Mapped files in memory
            if (pathname[0] && strstr(pathname, ".so")) {
                forensic_ioc_t* ioc = forensic_ioc_create("loaded_library", pathname,
                    "memory_parser", 0.5);
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
    forensic_event_t* evt = forensic_event_create("memory_parsed",
        timestamp, "memory_parser", "Memory regions parsed");
    artifact->timeline_events = realloc(artifact->timeline_events,
        (artifact->timeline_count + 1) * sizeof(forensic_event_t));
    artifact->timeline_events[artifact->timeline_count++] = *evt;
    free(evt);
    
    printf("[memory_parser] Parsed memory: %zu metadata entries\n", artifact->parsed_data.count);
    return artifact;
}

static void memory_parser_cleanup(void) {
    printf("[memory_parser] Cleanup\n");
}

/* ============================================================================
 * Plugin Definition
 * ============================================================================ */

static const char* memory_parser_capabilities[] = {
    "read_artifact",
    "parse_memory"
};

forensic_artifact_parser_plugin_t memory_parser_plugin = {
    .name = "memory_parser",
    .version = "1.0.0",
    .description = "Parses process memory regions, permissions, and loaded modules",
    .artifact_type = "memory_region",
    .init = memory_parser_init,
    .parse = parse_memory,
    .cleanup = memory_parser_cleanup,
    .required_capabilities = memory_parser_capabilities,
    .capability_count = sizeof(memory_parser_capabilities) / sizeof(memory_parser_capabilities[0]),
    .requires_sandbox = true,
    .max_cpu_time_ms = 5000,
    .max_memory_bytes = 64 * 1024 * 1024
};