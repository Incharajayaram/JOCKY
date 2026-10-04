#include "../../forensic_types.h"
/**
 * DNS Cache Parser Plugin
 * 
 * Parses DNS cache entries from /etc/resolv.conf or similar sources.
 */

#include "../../forensic_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ============================================================================
 * DNS Cache Parser Implementation
 * ============================================================================ */

static int dns_parser_init(void* config) {
    (void)config;
    printf("[dns_parser] Initialized\n");
    return 0;
}

static forensic_parsed_artifact_t* parse_dns(const forensic_bytes_t* data, void* config) {
    (void)config;
    
    forensic_parsed_artifact_t* artifact = forensic_parsed_artifact_create("dns_cache", "dns_parser");
    if (!artifact) return NULL;
    
    const char* raw = (const char*)data->data;
    size_t len = data->len;
    
    if (len == 0) return artifact;
    
    char* copy = malloc(len + 1);
    memcpy(copy, raw, len);
    copy[len] = '\0';
    
    char* line = strtok(copy, "\n");
    while (line) {
        // Parse nameserver entries
        if (strncmp(line, "nameserver", 10) == 0) {
            char* ns = line + 10;
            while (*ns == ' ' || *ns == '\t') ns++;
            forensic_metadata_add(&artifact->parsed_data, "nameserver", ns);
            
            // IOC: external DNS
            if (strncmp(ns, "8.8.", 4) != 0 && strncmp(ns, "1.1.", 4) != 0 &&
                strncmp(ns, "9.9.", 4) != 0 && strncmp(ns, "127.", 4) != 0) {
                forensic_ioc_t* ioc = forensic_ioc_create("dns_server", ns,
                    "dns_parser", 0.5);
                artifact->iocs.items = realloc(artifact->iocs.items,
                    (artifact->iocs.count + 1) * sizeof(forensic_ioc_t));
                artifact->iocs.items[artifact->iocs.count++] = *ioc;
                free(ioc);
            }
        }
        // Parse search domains
        else if (strncmp(line, "search", 6) == 0) {
            char* search = line + 6;
            while (*search == ' ' || *search == '\t') search++;
            forensic_metadata_add(&artifact->parsed_data, "search_domain", search);
        }
        line = strtok(NULL, "\n");
    }
    
    free(copy);
    
    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
    forensic_event_t* evt = forensic_event_create("dns_parsed",
        timestamp, "dns_parser", "DNS cache/config parsed");
    artifact->timeline_events = realloc(artifact->timeline_events,
        (artifact->timeline_count + 1) * sizeof(forensic_event_t));
    artifact->timeline_events[artifact->timeline_count++] = *evt;
    free(evt);
    
    printf("[dns_parser] Parsed DNS: %zu metadata entries\n", artifact->parsed_data.count);
    return artifact;
}

static void dns_parser_cleanup(void) {
    printf("[dns_parser] Cleanup\n");
}

/* ============================================================================
 * Plugin Definition
 * ============================================================================ */

static const char* dns_parser_capabilities[] = {
    "read_artifact",
    "parse_dns"
};

forensic_artifact_parser_plugin_t dns_parser_plugin = {
    .name = "dns_parser",
    .version = "1.0.0",
    .description = "Parses DNS resolver configuration and cache entries",
    .artifact_type = "dns_cache",
    .init = dns_parser_init,
    .parse = parse_dns,
    .cleanup = dns_parser_cleanup,
    .required_capabilities = dns_parser_capabilities,
    .capability_count = sizeof(dns_parser_capabilities) / sizeof(dns_parser_capabilities[0]),
    .requires_sandbox = false,
    .max_cpu_time_ms = 1000,
    .max_memory_bytes = 8 * 1024 * 1024
};