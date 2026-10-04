#include "../../forensic_types.h"
/**
 * ARP Entry Parser Plugin
 * 
 * Parses ARP table entries (IP to MAC mappings).
 */

#include "../../forensic_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ============================================================================
 * ARP Parser Implementation
 * ============================================================================ */

static int arp_parser_init(void* config) {
    (void)config;
    printf("[arp_parser] Initialized\n");
    return 0;
}

static forensic_parsed_artifact_t* parse_arp(const forensic_bytes_t* data, void* config) {
    (void)config;
    
    forensic_parsed_artifact_t* artifact = forensic_parsed_artifact_create("arp_entry", "arp_parser");
    if (!artifact) return NULL;
    
    const char* raw = (const char*)data->data;
    size_t len = data->len;
    
    if (len == 0) return artifact;
    
    char* copy = malloc(len + 1);
    memcpy(copy, raw, len);
    copy[len] = '\0';
    
    char* line = strtok(copy, "\n");
    while (line) {
        // Format: IP address  HW type  Flags  HW address  Mask  Device
        char ip[64], hw[64], dev[64];
        if (sscanf(line, "%63s %*s %*s %63s %*s %63s", ip, hw, dev) == 3) {
            char key[128];
            snprintf(key, sizeof(key), "arp_%s", ip);
            char value[256];
            snprintf(value, sizeof(value), "mac=%s dev=%s", hw, dev);
            forensic_metadata_add(&artifact->parsed_data, key, value);
            
            // IOC: MAC address
            forensic_ioc_t* ioc = forensic_ioc_create("mac_address", hw,
                "arp_parser", 0.6);
            artifact->iocs.items = realloc(artifact->iocs.items,
                (artifact->iocs.count + 1) * sizeof(forensic_ioc_t));
            artifact->iocs.items[artifact->iocs.count++] = *ioc;
            free(ioc);
            
            // IOC: IP address
            ioc = forensic_ioc_create("ip_address", ip,
                "arp_parser", 0.6);
            artifact->iocs.items = realloc(artifact->iocs.items,
                (artifact->iocs.count + 1) * sizeof(forensic_ioc_t));
            artifact->iocs.items[artifact->iocs.count++] = *ioc;
            free(ioc);
        }
        line = strtok(NULL, "\n");
    }
    
    free(copy);
    
    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
    forensic_event_t* evt = forensic_event_create("arp_parsed",
        timestamp, "arp_parser", "ARP table parsed");
    artifact->timeline_events = realloc(artifact->timeline_events,
        (artifact->timeline_count + 1) * sizeof(forensic_event_t));
    artifact->timeline_events[artifact->timeline_count++] = *evt;
    free(evt);
    
    printf("[arp_parser] Parsed ARP: %zu metadata entries\n", artifact->parsed_data.count);
    return artifact;
}

static void arp_parser_cleanup(void) {
    printf("[arp_parser] Cleanup\n");
}

/* ============================================================================
 * Plugin Definition
 * ============================================================================ */

static const char* arp_parser_capabilities[] = {
    "read_artifact",
    "parse_arp"
};

forensic_artifact_parser_plugin_t arp_parser_plugin = {
    .name = "arp_parser",
    .version = "1.0.0",
    .description = "Parses ARP table entries (IP to MAC address mappings)",
    .artifact_type = "arp_entry",
    .init = arp_parser_init,
    .parse = parse_arp,
    .cleanup = arp_parser_cleanup,
    .required_capabilities = arp_parser_capabilities,
    .capability_count = sizeof(arp_parser_capabilities) / sizeof(arp_parser_capabilities[0]),
    .requires_sandbox = false,
    .max_cpu_time_ms = 1000,
    .max_memory_bytes = 8 * 1024 * 1024
};