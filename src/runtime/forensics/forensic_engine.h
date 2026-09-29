/**
 * JOCKY Forensic Engine
 * 
 * Main orchestrator for the defensive forensic subsystem.
 * Coordinates plugins, provenance, analysis, audit, and evidence storage.
 */

#pragma once

#include "forensic_types.h"
#include "engine/provenance.h"
#include "engine/analysis.h"
#include "audit/audit_log.h"
#include "store/evidence_store.h"
#include "control/capabilities.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Forensic Engine Configuration
 * ============================================================================ */

typedef struct {
    const char* evidence_base_path;
    const char* audit_log_path;
    forensic_role_t operator_role;
    bool enable_sandbox;
    bool enable_siem;
    const char* siem_endpoint;
} forensic_engine_config_t;

/* ============================================================================
 * Forensic Engine
 * ============================================================================ */

typedef struct {
    forensic_engine_config_t config;
    forensic_plugin_registry_t* registry;
    forensic_provenance_t* provenance;
    forensic_audit_log_t* audit_log;
    forensic_evidence_store_t* evidence_store;
    forensic_correlation_graph_t* correlations;
} forensic_engine_t;

/* ============================================================================
 * Forensic Engine API
 * ============================================================================ */

forensic_engine_t* forensic_engine_create(const forensic_engine_config_t* config);
void forensic_engine_destroy(forensic_engine_t* engine);

int forensic_engine_register_default_plugins(forensic_engine_t* engine);

int forensic_engine_run_collection(forensic_engine_t* engine,
                                    const char* target,
                                    forensic_parsed_artifact_list_t** parsed_output);

int forensic_engine_run_analysis(forensic_engine_t* engine,
                                  const forensic_parsed_artifact_list_t* parsed,
                                  forensic_analysis_result_t* result);

int forensic_engine_generate_outputs(forensic_engine_t* engine,
                                      const forensic_analysis_result_t* result);

int forensic_engine_run_full_pipeline(forensic_engine_t* engine,
                                       const char* target,
                                       forensic_analysis_result_t* result);

/* ============================================================================
 * Default Configuration
 * ============================================================================ */

void forensic_engine_config_default(forensic_engine_config_t* config);

#ifdef __cplusplus
}
#endif