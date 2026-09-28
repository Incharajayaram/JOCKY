/**
 * Forensic Engine Test Program
 * 
 * Tests the defensive forensic pipeline end-to-end.
 */

#include "forensic_types.h"
#include "forensic_engine.h"
#include <stdio.h>

int main(int argc, char** argv) {
    printf("=== JOCKY Forensic Engine Test ===\n\n");
    
    forensic_engine_config_t config;
    forensic_engine_config_default(&config);
    config.evidence_base_path = "./test_forensics";
    config.audit_log_path = "./test_forensics/audit/audit.jsonl";
    config.operator_role = ROLE_ANALYST;
    config.enable_sandbox = true;
    config.enable_siem = true;
    config.siem_endpoint = "https://httpbin.org/post";
    
    forensic_engine_t* engine = forensic_engine_create(&config);
    if (!engine) {
        fprintf(stderr, "Failed to create forensic engine\n");
        return 1;
    }
    
    if (forensic_engine_register_default_plugins(engine) != 0) {
        fprintf(stderr, "Failed to register plugins\n");
        forensic_engine_destroy(engine);
        return 1;
    }
    
    forensic_analysis_result_t* result = forensic_analysis_create();
    if (!result) {
        fprintf(stderr, "Failed to create analysis result\n");
        forensic_engine_destroy(engine);
        return 1;
    }
    
    int ret = forensic_engine_run_full_pipeline(engine, "local", result);
    
    printf("\n=== Results ===\n");
    printf("Timeline events: %zu\n", result->timeline ? result->timeline->count : 0);
    printf("IOCs extracted: %zu\n", result->iocs ? result->iocs->count : 0);
    
    if (result->iocs) {
        for (size_t i = 0; i < result->iocs->count; i++) {
            const forensic_ioc_t* ioc = &result->iocs->items[i];
            printf("  IOC: %s = %s (confidence: %.2f)\n", 
                   ioc->type, ioc->value, ioc->confidence);
        }
    }
    
    printf("\n=== Audit Log Verification ===\n");
    forensic_audit_log_verify(engine->audit_log);
    
    printf("\n=== Provenance Chain Verification ===\n");
    forensic_provenance_verify_chain(engine->provenance);
    
    forensic_analysis_destroy(result);
    forensic_engine_destroy(engine);
    
    printf("\n=== Test Complete ===\n");
    return ret == 0 ? 0 : 1;
}