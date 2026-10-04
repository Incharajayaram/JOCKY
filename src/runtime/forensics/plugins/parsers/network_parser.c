#include "../../forensic_types.h"
/**
 * Network Parser Plugin
 * 
 * Parses PCAP or NetFlow data into connections, protocols, endpoints.
 * On Linux: parses /proc/net/* and connection data.
 */

#include "../../forensic_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ============================================================================
 * Network Parser Implementation
 * ============================================================================ */

static int network_parser_init(void* config) {
    (void)config;
    printf("[network_parser] Initialized\n");
    return 0;
}

static forensic_parsed_artifact_t* parse_network(const forensic_bytes_t* data, void* config) {
    (void)config;
    
    forensic_parsed_artifact_t* artifact = forensic_parsed_artifact_create("network_connection", "network_parser");
    if (!artifact) return NULL;
    
    const char* raw = (const char*)data->data;
    size_t len = data->len;
    
    if (len == 0) return artifact;
    
    char* copy = malloc(len + 1);
    memcpy(copy, raw, len);
    copy[len] = '\0';
    
    char* line = strtok(copy, "\n");
    int count = 0;
    
    while (line && count < 1000) {
        char proto[16], local_addr[64], remote_addr[64], state[32];
        int local_port, remote_port;
        
        if (sscanf(line, "%15s %63[^:]:%d -> %63[^:]:%d [%31[^]]",
                   proto, local_addr, &local_port, remote_addr, &remote_port, state) == 6) {
            
            char key[256];
            snprintf(key, sizeof(key), "%s %s:%d->%s:%d", proto, local_addr, local_port, remote_addr, remote_port);
            char value[128];
            snprintf(value, sizeof(value), "state=%s", state);
            forensic_metadata_add(&artifact->parsed_data, key, value);
            
            if (strcmp(state, "ESTABLISHED") == 0) {
                if (remote_port == 4444 || remote_port == 6667 || 
                    remote_port == 8080 || remote_port == 6666) {
                    forensic_ioc_t* ioc = forensic_ioc_create("suspicious_port", remote_addr,
                        "network_parser", 0.7);
                    artifact->iocs.items = realloc(artifact->iocs.items,
                        (artifact->iocs.count + 1) * sizeof(forensic_ioc_t));
                    artifact->iocs.items[artifact->iocs.count++] = *ioc;
                    free(ioc);
                }
            }
            count++;
        }
        line = strtok(NULL, "\n");
    }
    
    free(copy);
    
    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
    forensic_event_t* evt = forensic_event_create("network_parsed",
        timestamp, "network_parser", "Network connections parsed");
    artifact->timeline_events = realloc(artifact->timeline_events,
        (artifact->timeline_count + 1) * sizeof(forensic_event_t));
    artifact->timeline_events[artifact->timeline_count++] = *evt;
    free(evt);
    
    printf("[network_parser] Parsed %d connections\n", count);
    return artifact;
}

static void network_parser_cleanup(void) {
    printf("[network_parser] Cleanup\n");
}

/* ============================================================================
 * Plugin Definition
 * ============================================================================ */

static const char* network_parser_capabilities[] = {
    "read_artifact",
    "parse_network"
};

forensic_artifact_parser_plugin_t network_parser_plugin = {
    .name = "network_parser",
    .version = "1.0.0",
    .description = "Parses network connection data into structured connections",
    .artifact_type = "network_connection",
    .init = network_parser_init,
    .parse = parse_network,
    .cleanup = network_parser_cleanup,
    .required_capabilities = network_parser_capabilities,
    .capability_count = sizeof(network_parser_capabilities) / sizeof(network_parser_capabilities[0]),
    .requires_sandbox = true,
    .max_cpu_time_ms = 5000,
    .max_memory_bytes = 64 * 1024 * 1024
};