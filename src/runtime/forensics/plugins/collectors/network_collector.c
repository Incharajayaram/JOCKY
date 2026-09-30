#include "forensic_types.h"
/**
 * Network Collector Plugin
 * 
 * Collects active connections, listening ports, DNS cache, ARP table.
 */

#include "forensic_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>

/* ============================================================================
 * Network Collector Implementation
 * ============================================================================ */

static int network_collector_init(void* config) {
    (void)config;
    printf("[network_collector] Initialized\n");
    return 0;
}

static forensic_artifact_t* create_network_artifact(const char* proto,
                                                     const char* local_addr,
                                                     uint16_t local_port,
                                                     const char* remote_addr,
                                                     uint16_t remote_port,
                                                     const char* state) {
    forensic_artifact_t* artifact = calloc(1, sizeof(forensic_artifact_t));
    if (!artifact) return NULL;
    
    artifact->plugin_name = "network_collector";
    artifact->artifact_type = "network_connection";
    
    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
    artifact->timestamp = strdup(timestamp);
    
    char raw[512];
    snprintf(raw, sizeof(raw), "%s %s:%d -> %s:%d [%s]",
             proto, local_addr, local_port, remote_addr, remote_port, state);
    artifact->raw = forensic_bytes_create(raw, strlen(raw));
    
    forensic_metadata_t meta = forensic_metadata_create(16);
    forensic_metadata_add(&meta, "protocol", proto);
    forensic_metadata_add(&meta, "local_addr", local_addr);
    char lport[16];
    snprintf(lport, sizeof(lport), "%d", local_port);
    forensic_metadata_add(&meta, "local_port", lport);
    forensic_metadata_add(&meta, "remote_addr", remote_addr);
    char rport[16];
    snprintf(rport, sizeof(rport), "%d", remote_port);
    forensic_metadata_add(&meta, "remote_port", rport);
    forensic_metadata_add(&meta, "state", state);
    artifact->metadata = meta;
    
    return artifact;
}

static void parse_proc_net(const char* path, const char* proto,
                            forensic_artifact_list_t* list) {
    FILE* f = fopen(path, "r");
    if (!f) return;
    
    char line[512];
    fgets(line, sizeof(line), f);
    
    while (fgets(line, sizeof(line), f)) {
        unsigned int local_addr, rem_addr;
        unsigned int local_port, rem_port;
        unsigned int state;
        int uid;
        unsigned int inode;
        
        if (sscanf(line, "%*d: %08X:%04X %08X:%04X %02X %*08X:%*08X %*02X:%*08X %*08X %d %*d %u",
                   &local_addr, &local_port, &rem_addr, &rem_port,
                   &state, &uid, &inode) < 5) continue;
        
        struct in_addr local, remote;
        local.s_addr = local_addr;
        remote.s_addr = rem_addr;
        
        const char* state_str = "UNKNOWN";
        if (strcmp(proto, "TCP") == 0) {
            switch (state) {
                case 1: state_str = "ESTABLISHED"; break;
                case 2: state_str = "SYN_SENT"; break;
                case 3: state_str = "SYN_RECV"; break;
                case 4: state_str = "FIN_WAIT1"; break;
                case 5: state_str = "FIN_WAIT2"; break;
                case 6: state_str = "TIME_WAIT"; break;
                case 7: state_str = "CLOSE"; break;
                case 8: state_str = "CLOSE_WAIT"; break;
                case 9: state_str = "LAST_ACK"; break;
                case 10: state_str = "LISTEN"; break;
                case 11: state_str = "CLOSING"; break;
            }
        } else {
            state_str = (state == 7) ? "LISTEN" : "UNKNOWN";
        }
        
        forensic_artifact_t* artifact = create_network_artifact(
            proto,
            inet_ntoa(local), ntohs((uint16_t)local_port),
            inet_ntoa(remote), ntohs((uint16_t)rem_port),
            state_str
        );
        
        if (artifact) {
            forensic_artifact_list_add(list, artifact);
            free(artifact->timestamp);
            forensic_bytes_destroy(&artifact->raw);
            forensic_metadata_destroy(&artifact->metadata);
            free(artifact);
        }
    }
    fclose(f);
}

static void collect_dns_cache(forensic_artifact_list_t* list) {
    FILE* f = popen("resolvectl statistics 2>/dev/null || cat /etc/resolv.conf 2>/dev/null", "r");
    if (!f) return;
    
    char line[512];
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "nameserver") || strstr(line, "search")) {
            forensic_metadata_t meta = forensic_metadata_create(8);
            forensic_metadata_add(&meta, "type", "dns_config");
            forensic_metadata_add(&meta, "content", line);
            
            forensic_artifact_t artifact = {0};
            artifact.plugin_name = strdup("network_collector");
            artifact.artifact_type = strdup("dns_cache");
            time_t now = time(NULL);
            char timestamp[64];
            strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
            artifact.timestamp = strdup(timestamp);
            artifact.raw = forensic_bytes_create(line, strlen(line));
            artifact.metadata = meta;
            forensic_artifact_list_add(list, &artifact);
        }
    }
    pclose(f);
}

static forensic_artifact_list_t* network_collector_collect(const char* target, void* config) {
    (void)target;
    (void)config;
    
    forensic_artifact_list_t* list = forensic_artifact_list_create(128);
    if (!list) return NULL;
    
    parse_proc_net("/proc/net/tcp", "TCP", list);
    parse_proc_net("/proc/net/udp", "UDP", list);
    parse_proc_net("/proc/net/tcp6", "TCP6", list);
    parse_proc_net("/proc/net/udp6", "UDP6", list);
    collect_dns_cache(list);
    
    FILE* f = fopen("/proc/net/arp", "r");
    if (f) {
        char line[512];
        fgets(line, sizeof(line), f);
        while (fgets(line, sizeof(line), f)) {
            char ip[64], hw[64], dev[64];
            if (sscanf(line, "%63s %*s %63s %*s %*s %63s", ip, hw, dev) == 3) {
                forensic_metadata_t meta = forensic_metadata_create(8);
                forensic_metadata_add(&meta, "type", "arp");
                forensic_metadata_add(&meta, "ip", ip);
                forensic_metadata_add(&meta, "mac", hw);
                forensic_metadata_add(&meta, "interface", dev);
                
forensic_artifact_t artifact = {0};
            artifact.plugin_name = strdup("network_collector");
            artifact.artifact_type = strdup("arp_entry");
                time_t now = time(NULL);
                char timestamp[64];
                strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
                artifact.timestamp = strdup(timestamp);
                artifact.raw = forensic_bytes_create(line, strlen(line));
                artifact.metadata = meta;
                forensic_artifact_list_add(list, &artifact);
            }
        }
        fclose(f);
    }
    
    printf("[network_collector] Collected %zu network artifacts\n", list->count);
    return list;
}

static void network_collector_cleanup(void) {
    printf("[network_collector] Cleanup\n");
}

/* ============================================================================
 * Plugin Definition
 * ============================================================================ */

static const char* network_capabilities[] = {
    "read_network_state",
    "enumerate_connections",
    "read_dns_cache"
};

forensic_data_source_plugin_t network_collector_plugin = {
    .name = "network_collector",
    .version = "1.0.0",
    .description = "Collects active connections, listening ports, DNS cache, and ARP table",
    .init = network_collector_init,
    .collect = network_collector_collect,
    .cleanup = network_collector_cleanup,
    .required_capabilities = network_capabilities,
    .capability_count = sizeof(network_capabilities) / sizeof(network_capabilities[0])
};