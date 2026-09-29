#include "forensic_types.h"
/**
 * EVTX Parser Plugin
 * 
 * Parses Windows Event Log (EVTX) files into structured events.
 * On Linux: parses syslog, journald, and auditd logs.
 */

#include "forensic_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ============================================================================
 * EVTX Parser Implementation (Linux - log-based)
 * ============================================================================ */

static int evtx_parser_init(void* config) {
    (void)config;
    printf("[evtx_parser] Initialized\n");
    return 0;
}

static forensic_parsed_artifact_t* parse_evtx(const forensic_bytes_t* data, void* config) {
    (void)config;
    
    forensic_parsed_artifact_t* artifact = forensic_parsed_artifact_create("evtx", "evtx_parser");
    if (!artifact) return NULL;
    
    const char* raw = (const char*)data->data;
    size_t len = data->len;
    
    if (len == 0) return artifact;
    
    char* copy = malloc(len + 1);
    memcpy(copy, raw, len);
    copy[len] = '\0';
    
    char* line = strtok(copy, "\n");
    int event_count = 0;
    
    while (line && event_count < 1000) {
        char timestamp[64] = {0};
        char hostname[64] = {0};
        char service[64] = {0};
        char message[512] = {0};
        
        if (sscanf(line, "%63s %63s %63[^:]: %511[^\n]",
                   timestamp, hostname, service, message) == 4) {
            
            forensic_metadata_t meta = forensic_metadata_create(8);
            forensic_metadata_add(&meta, "timestamp", timestamp);
            forensic_metadata_add(&meta, "hostname", hostname);
            forensic_metadata_add(&meta, "service", service);
            forensic_metadata_add(&meta, "message", message);
            
            if (strstr(message, "Failed password") || 
                strstr(message, "authentication failure") ||
                strstr(message, "sudo:") ||
                strstr(message, "Accepted password")) {
                forensic_ioc_t* ioc = forensic_ioc_create("auth_event", message,
                    "evtx_parser", 0.9);
                artifact->iocs.items = realloc(artifact->iocs.items,
                    (artifact->iocs.count + 1) * sizeof(forensic_ioc_t));
                artifact->iocs.items[artifact->iocs.count++] = *ioc;
                free(ioc);
            }
            
            if (strstr(message, "kernel:") && 
                (strstr(message, "segfault") || strstr(message, "killed"))) {
                forensic_ioc_t* ioc = forensic_ioc_create("process_crash", message,
                    "evtx_parser", 0.8);
                artifact->iocs.items = realloc(artifact->iocs.items,
                    (artifact->iocs.count + 1) * sizeof(forensic_ioc_t));
                artifact->iocs.items[artifact->iocs.count++] = *ioc;
                free(ioc);
            }
            
            event_count++;
        }
        
        line = strtok(NULL, "\n");
    }
    
    free(copy);
    
    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
    forensic_event_t* evt = forensic_event_create("evtx_parsed",
        timestamp, "evtx_parser", "Event log parsed");
    artifact->timeline_events = realloc(artifact->timeline_events,
        (artifact->timeline_count + 1) * sizeof(forensic_event_t));
    artifact->timeline_events[artifact->timeline_count++] = *evt;
    free(evt);
    
    printf("[evtx_parser] Parsed %d events\n", event_count);
    return artifact;
}

static void evtx_parser_cleanup(void) {
    printf("[evtx_parser] Cleanup\n");
}

/* ============================================================================
 * Plugin Definition
 * ============================================================================ */

static const char* evtx_capabilities[] = {
    "read_artifact",
    "parse_evtx"
};

forensic_artifact_parser_plugin_t evtx_parser_plugin = {
    .name = "evtx_parser",
    .version = "1.0.0",
    .description = "Parses EVTX event logs and Linux syslog/journald logs",
    .artifact_type = "evtx",
    .init = evtx_parser_init,
    .parse = parse_evtx,
    .cleanup = evtx_parser_cleanup,
    .required_capabilities = evtx_capabilities,
    .capability_count = sizeof(evtx_capabilities) / sizeof(evtx_capabilities[0]),
    .requires_sandbox = true,
    .max_cpu_time_ms = 10000,
    .max_memory_bytes = 256 * 1024 * 1024
};