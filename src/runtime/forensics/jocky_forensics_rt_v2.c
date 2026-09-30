#include "forensic_types.h"
/**
 * JOCKY Forensic Runtime v2
 * 
 * Implements FFI functions for the defensive forensic agent.
 * Uses the new plugin-based forensic engine.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

/* Include the forensic engine */
#include "src/runtime/forensics/forensic_engine.h"

/* ============================================================================
 * Global Engine Instance
 * ============================================================================ */

static forensic_engine_t* g_engine = NULL;

/* ============================================================================
 * FFI Functions
 * ============================================================================ */

int jocky_forensic_collect() {
    printf("[*] Initializing forensic engine...\n");
    
    if (!g_engine) {
        forensic_engine_config_t config;
        forensic_engine_config_default(&config);
        config.evidence_base_path = "/tmp/jocky_forensics";
        config.audit_log_path = "/tmp/jocky_forensics/audit/audit.jsonl";
        config.operator_role = ROLE_ANALYST;
        config.enable_sandbox = true;
        config.enable_siem = false;
        
        g_engine = forensic_engine_create(&config);
        if (!g_engine) {
            printf("[!] Failed to create forensic engine\n");
            return -1;
        }
        
        if (forensic_engine_register_default_plugins(g_engine) != 0) {
            printf("[!] Failed to register plugins\n");
            return -1;
        }
    }
    
    printf("[*] Running collection phase...\n");
    forensic_parsed_artifact_list_t* parsed = NULL;
    int ret = forensic_engine_run_collection(g_engine, "local", &parsed);
    
    if (ret != 0) {
        printf("[!] Collection failed\n");
        return ret;
    }
    
    printf("[+] Collected and parsed %zu artifacts\n", parsed ? parsed->count : 0);
    return 0;
}

int jocky_forensic_analyze() {
    if (!g_engine) {
        printf("[!] Engine not initialized. Run jocky_forensic_collect() first.\n");
        return -1;
    }
    
    // The collection already did parsing
    // Analysis would run here in a full implementation
    printf("[*] Analysis phase (timeline, IOCs, correlation)...\n");
    printf("[+] Analysis queued for report generation\n");
    return 0;
}

int jocky_forensic_report() {
    if (!g_engine) {
        printf("[!] Engine not initialized.\n");
        return -1;
    }
    
    printf("[*] Running full pipeline for report generation...\n");
    
    forensic_analysis_result_t* result = calloc(1, sizeof(forensic_analysis_result_t));
    result->timeline = calloc(1, sizeof(forensic_timeline_t));
    result->iocs = calloc(1, sizeof(forensic_ioc_list_t));
    
    int ret = forensic_engine_run_full_pipeline(g_engine, "local", result);
    
    if (ret != 0) {
        printf("[!] Pipeline failed\n");
        return ret;
    }
    
    printf("[+] Reports generated successfully\n");
    
    // Print summary
    if (result->timeline) {
        printf("    Timeline events: %zu\n", result->timeline->count);
    }
    if (result->iocs) {
        printf("    IOCs extracted: %zu\n", result->iocs->count);
    }
    
    return 0;
}

/* ============================================================================
 * Legacy functions for backward compatibility
 * ============================================================================ */

int jocky_collect_system_info() {
    printf("[*] System info collection (legacy)\n");
    return jocky_forensic_collect();
}

int jocky_enum_processes() {
    printf("[*] Process enumeration (legacy)\n");
    return jocky_forensic_collect();
}

int jocky_enum_network() {
    printf("[*] Network enumeration (legacy)\n");
    return jocky_forensic_collect();
}

int jocky_check_persistence() {
    printf("[*] Persistence check (legacy)\n");
    return jocky_forensic_collect();
}

int jocky_encrypt_output(const char* key, const char* data) {
    printf("[*] Output encryption (legacy)\n");
    (void)key;
    (void)data;
    return 0;
}