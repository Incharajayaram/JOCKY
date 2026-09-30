/**
 * Capability Permissions Implementation
 */

#include "capabilities.h"
#include <stdbool.h>
#include <string.h>

/* ============================================================================
 * Capability Information
 * ============================================================================ */

const struct forensic_capability_info_s FORENSIC_CAPABILITIES[CAP_COUNT] = {
    {CAP_READ_PROCESS_INFO, "read_process_info", "Read process information"},
    {CAP_ENUMERATE_PROCESSES, "enumerate_processes", "Enumerate running processes"},
    {CAP_READ_FILESYSTEM, "read_filesystem", "Read filesystem"},
    {CAP_ENUMERATE_FILES, "enumerate_files", "Enumerate files"},
    {CAP_ACCESS_TEMP_DIRS, "access_temp_dirs", "Access temporary directories"},
    {CAP_READ_NETWORK_STATE, "read_network_state", "Read network state"},
    {CAP_ENUMERATE_CONNECTIONS, "enumerate_connections", "Enumerate network connections"},
    {CAP_READ_DNS_CACHE, "read_dns_cache", "Read DNS cache"},
    {CAP_READ_SYSTEM_CONFIG, "read_system_config", "Read system configuration"},
    {CAP_ENUMERATE_SERVICES, "enumerate_services", "Enumerate system services"},
    {CAP_READ_CRON_JOBS, "read_cron_jobs", "Read cron jobs"},
    {CAP_READ_PROCESS_MEMORY, "read_process_memory", "Read process memory"},
    {CAP_ENUMERATE_MEMORY_REGIONS, "enumerate_memory_regions", "Enumerate memory regions"},
    {CAP_ACCESS_PROCFS, "access_procfs", "Access /proc filesystem"},
    {CAP_READ_ARTIFACT, "read_artifact", "Read artifact data"},
    {CAP_PARSE_PE, "parse_pe", "Parse PE files"},
    {CAP_PARSE_REGISTRY, "parse_registry", "Parse registry/config"},
    {CAP_PARSE_EVTX, "parse_evtx", "Parse EVTX/logs"},
    {CAP_PARSE_PREFETCH, "parse_prefetch", "Parse prefetch/history"},
    {CAP_PARSE_MFT, "parse_mft", "Parse MFT/filesystem"},
    {CAP_PARSE_NETWORK, "parse_network", "Parse network data"},
    {CAP_WRITE_REPORT, "write_report", "Write forensic reports"},
    {CAP_NETWORK_ACCESS, "network_access", "Network access for SIEM forwarding"},
};

/* ============================================================================
 * Role Permissions
 * 
 * Operator: Can do everything including offensive operations
 * Analyst: Can only do defensive forensic operations
 * ============================================================================ */

static const forensic_capability_t OPERATOR_CAPS[] = {
    CAP_READ_PROCESS_INFO,
    CAP_ENUMERATE_PROCESSES,
    CAP_READ_FILESYSTEM,
    CAP_ENUMERATE_FILES,
    CAP_ACCESS_TEMP_DIRS,
    CAP_READ_NETWORK_STATE,
    CAP_ENUMERATE_CONNECTIONS,
    CAP_READ_DNS_CACHE,
    CAP_READ_SYSTEM_CONFIG,
    CAP_ENUMERATE_SERVICES,
    CAP_READ_CRON_JOBS,
    CAP_READ_PROCESS_MEMORY,
    CAP_ENUMERATE_MEMORY_REGIONS,
    CAP_ACCESS_PROCFS,
    CAP_READ_ARTIFACT,
    CAP_PARSE_PE,
    CAP_PARSE_REGISTRY,
    CAP_PARSE_EVTX,
    CAP_PARSE_PREFETCH,
    CAP_PARSE_MFT,
    CAP_PARSE_NETWORK,
    CAP_WRITE_REPORT,
    CAP_NETWORK_ACCESS,
};

static const forensic_capability_t ANALYST_CAPS[] = {
    CAP_READ_PROCESS_INFO,
    CAP_ENUMERATE_PROCESSES,
    CAP_READ_FILESYSTEM,
    CAP_ENUMERATE_FILES,
    CAP_ACCESS_TEMP_DIRS,
    CAP_READ_NETWORK_STATE,
    CAP_ENUMERATE_CONNECTIONS,
    CAP_READ_DNS_CACHE,
    CAP_READ_SYSTEM_CONFIG,
    CAP_ENUMERATE_SERVICES,
    CAP_READ_CRON_JOBS,
    CAP_READ_PROCESS_MEMORY,
    CAP_ENUMERATE_MEMORY_REGIONS,
    CAP_ACCESS_PROCFS,
    CAP_READ_ARTIFACT,
    CAP_PARSE_PE,
    CAP_PARSE_REGISTRY,
    CAP_PARSE_EVTX,
    CAP_PARSE_PREFETCH,
    CAP_PARSE_MFT,
    CAP_PARSE_NETWORK,
    CAP_WRITE_REPORT,
};

const forensic_role_permissions_t FORENSIC_ROLE_PERMISSIONS[ROLE_COUNT] = {
    {
        .role = ROLE_OPERATOR,
        .capabilities = {
            .capabilities = {0},
            .count = sizeof(OPERATOR_CAPS) / sizeof(OPERATOR_CAPS[0])
        }
    },
    {
        .role = ROLE_ANALYST,
        .capabilities = {
            .capabilities = {0},
            .count = sizeof(ANALYST_CAPS) / sizeof(ANALYST_CAPS[0])
        }
    }
};

/* ============================================================================
 * Initialize role permissions at runtime
 * ============================================================================ */

static void init_role_permissions(void) __attribute__((constructor));

static void init_role_permissions(void) {
    // Copy OPERATOR caps into the const struct
    forensic_capability_t* op_caps = (forensic_capability_t*)FORENSIC_ROLE_PERMISSIONS[ROLE_OPERATOR].capabilities.capabilities;
    for (size_t i = 0; i < sizeof(OPERATOR_CAPS)/sizeof(OPERATOR_CAPS[0]); i++) {
        op_caps[i] = OPERATOR_CAPS[i];
    }
    ((forensic_capability_set_t*)&FORENSIC_ROLE_PERMISSIONS[ROLE_OPERATOR].capabilities)->count = 
        sizeof(OPERATOR_CAPS)/sizeof(OPERATOR_CAPS[0]);
    
    // Copy ANALYST caps
    forensic_capability_t* an_caps = (forensic_capability_t*)FORENSIC_ROLE_PERMISSIONS[ROLE_ANALYST].capabilities.capabilities;
    for (size_t i = 0; i < sizeof(ANALYST_CAPS)/sizeof(ANALYST_CAPS[0]); i++) {
        an_caps[i] = ANALYST_CAPS[i];
    }
    ((forensic_capability_set_t*)&FORENSIC_ROLE_PERMISSIONS[ROLE_ANALYST].capabilities)->count = 
        sizeof(ANALYST_CAPS)/sizeof(ANALYST_CAPS[0]);
}

/* ============================================================================
 * Permission Check API
 * ============================================================================ */

bool forensic_permission_check_role(forensic_role_t role, forensic_capability_t cap) {
    if (role >= ROLE_COUNT) return false;
    
    const forensic_capability_set_t* set = &FORENSIC_ROLE_PERMISSIONS[role].capabilities;
    for (size_t i = 0; i < set->count; i++) {
        if (set->capabilities[i] == cap) return true;
    }
    return false;
}

bool forensic_permission_check_set(const forensic_capability_set_t* set, forensic_capability_t cap) {
    if (!set) return false;
    for (size_t i = 0; i < set->count; i++) {
        if (set->capabilities[i] == cap) return true;
    }
    return false;
}

bool forensic_permission_check_plugin(const char* plugin_name, forensic_role_t operator_role) {
    if (operator_role == ROLE_OPERATOR) return true;
    return true;
}

const char* forensic_capability_name(forensic_capability_t cap) {
    if (cap >= CAP_COUNT) return "UNKNOWN";
    return FORENSIC_CAPABILITIES[cap].name;
}

const char* forensic_role_name(forensic_role_t role) {
    switch (role) {
        case ROLE_OPERATOR: return "Operator";
        case ROLE_ANALYST: return "Analyst";
        default: return "Unknown";
    }
}

bool forensic_permission_validate_manifest(const char* plugin_name,
                                            const char** required_caps,
                                            size_t cap_count,
                                            forensic_role_t operator_role) {
    if (!required_caps || cap_count == 0) return true;
    
    const forensic_capability_set_t* role_caps = &FORENSIC_ROLE_PERMISSIONS[operator_role].capabilities;
    
    for (size_t i = 0; i < cap_count; i++) {
        forensic_capability_t cap = CAP_COUNT;
        for (size_t j = 0; j < CAP_COUNT; j++) {
            if (strcmp(FORENSIC_CAPABILITIES[j].name, required_caps[i]) == 0) {
                cap = FORENSIC_CAPABILITIES[j].capability;
                break;
            }
        }
        
        if (cap == CAP_COUNT) return false;
        if (!forensic_permission_check_set(role_caps, cap)) return false;
    }
    
    return true;
}