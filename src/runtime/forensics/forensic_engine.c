#include "forensic_types.h"
/**
 * Forensic Engine Implementation
 */

#include "forensic_engine.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#define mkdir(path, mode) _mkdir(path)
#endif

/* ============================================================================
 * Default Plugins Registration
 * ============================================================================ */

extern forensic_data_source_plugin_t process_collector_plugin;
extern forensic_data_source_plugin_t file_collector_plugin;
extern forensic_data_source_plugin_t network_collector_plugin;
#ifdef _WIN32
extern forensic_data_source_plugin_t registry_collector_plugin;
extern forensic_data_source_plugin_t evtx_collector_plugin;
#endif
extern forensic_data_source_plugin_t memory_collector_plugin;
extern forensic_data_source_plugin_t prefetch_collector_plugin;
extern forensic_data_source_plugin_t mft_collector_plugin;
extern forensic_data_source_plugin_t usnjrnl_collector_plugin;

#ifdef _WIN32
extern forensic_artifact_parser_plugin_t pe_parser_plugin;
#endif
extern forensic_artifact_parser_plugin_t registry_parser_plugin;
extern forensic_artifact_parser_plugin_t evtx_parser_plugin;
extern forensic_artifact_parser_plugin_t prefetch_parser_plugin;
extern forensic_artifact_parser_plugin_t mft_parser_plugin;
extern forensic_artifact_parser_plugin_t network_parser_plugin;
extern forensic_artifact_parser_plugin_t process_parser_plugin;
extern forensic_artifact_parser_plugin_t file_parser_plugin;
extern forensic_artifact_parser_plugin_t dns_parser_plugin;
extern forensic_artifact_parser_plugin_t arp_parser_plugin;
extern forensic_artifact_parser_plugin_t memory_parser_plugin;
extern forensic_artifact_parser_plugin_t elf_parser_plugin;

extern forensic_output_plugin_t json_output_plugin;
extern forensic_output_plugin_t stix_output_plugin;
extern forensic_output_plugin_t sigma_output_plugin;
extern forensic_output_plugin_t html_output_plugin;
extern forensic_output_plugin_t siem_forwarder_plugin;

/* ============================================================================
 * Forensic Engine
 * ============================================================================ */

void forensic_engine_config_default(forensic_engine_config_t* config) {
    if (!config) return;
    config->evidence_base_path = "/var/jocky/forensics";
    config->audit_log_path = "/var/jocky/forensics/audit/audit.jsonl";
    config->operator_role = ROLE_ANALYST;
    config->enable_sandbox = true;
    config->enable_siem = false;
    config->siem_endpoint = "https://siem.example.com/hec";
}

forensic_engine_t* forensic_engine_create(const forensic_engine_config_t* config) {
    forensic_engine_t* engine = calloc(1, sizeof(forensic_engine_t));
    if (!engine) return NULL;
    
    if (config) {
        engine->config = *config;
    } else {
        forensic_engine_config_default(&engine->config);
    }
    
    engine->registry = forensic_plugin_registry_create();
    engine->provenance = forensic_provenance_create();
    engine->audit_log = forensic_audit_log_open(engine->config.audit_log_path);
    engine->evidence_store = forensic_evidence_store_open(engine->config.evidence_base_path);
    engine->correlations = calloc(1, sizeof(forensic_correlation_graph_t));
    
    if (!engine->registry || !engine->provenance || !engine->audit_log || 
        !engine->evidence_store || !engine->correlations) {
        forensic_engine_destroy(engine);
        return NULL;
    }
    
    printf("[forensic_engine] Created with role: %s\n", 
           forensic_role_name(engine->config.operator_role));
    return engine;
}

void forensic_engine_destroy(forensic_engine_t* engine) {
    if (!engine) return;
    forensic_plugin_registry_destroy(engine->registry);
    forensic_provenance_destroy(engine->provenance);
    forensic_audit_log_close(engine->audit_log);
    forensic_evidence_store_close(engine->evidence_store);
    forensic_correlation_graph_destroy(engine->correlations);
    free(engine);
}

int forensic_engine_register_default_plugins(forensic_engine_t* engine) {
    if (!engine) return -1;
    
    forensic_plugin_registry_register_collector(engine->registry, &process_collector_plugin);
    forensic_plugin_registry_register_collector(engine->registry, &file_collector_plugin);
    forensic_plugin_registry_register_collector(engine->registry, &network_collector_plugin);
#ifdef _WIN32
    forensic_plugin_registry_register_collector(engine->registry, &registry_collector_plugin);
    forensic_plugin_registry_register_collector(engine->registry, &evtx_collector_plugin);
#endif
    forensic_plugin_registry_register_collector(engine->registry, &memory_collector_plugin);
    forensic_plugin_registry_register_collector(engine->registry, &prefetch_collector_plugin);
    forensic_plugin_registry_register_collector(engine->registry, &mft_collector_plugin);
    forensic_plugin_registry_register_collector(engine->registry, &usnjrnl_collector_plugin);

#ifdef _WIN32
    forensic_plugin_registry_register_parser(engine->registry, &pe_parser_plugin);
    forensic_plugin_registry_register_parser(engine->registry, &registry_parser_plugin);
    forensic_plugin_registry_register_parser(engine->registry, &evtx_parser_plugin);
#endif
    forensic_plugin_registry_register_parser(engine->registry, &prefetch_parser_plugin);
    forensic_plugin_registry_register_parser(engine->registry, &mft_parser_plugin);
    forensic_plugin_registry_register_parser(engine->registry, &network_parser_plugin);
    forensic_plugin_registry_register_parser(engine->registry, &process_parser_plugin);
    forensic_plugin_registry_register_parser(engine->registry, &file_parser_plugin);
    forensic_plugin_registry_register_parser(engine->registry, &dns_parser_plugin);
    forensic_plugin_registry_register_parser(engine->registry, &arp_parser_plugin);
    forensic_plugin_registry_register_parser(engine->registry, &memory_parser_plugin);
    forensic_plugin_registry_register_parser(engine->registry, &elf_parser_plugin);
    
    forensic_plugin_registry_register_output(engine->registry, &json_output_plugin);
    forensic_plugin_registry_register_output(engine->registry, &stix_output_plugin);
    forensic_plugin_registry_register_output(engine->registry, &sigma_output_plugin);
    forensic_plugin_registry_register_output(engine->registry, &html_output_plugin);
    if (engine->config.enable_siem) {
        forensic_plugin_registry_register_output(engine->registry, &siem_forwarder_plugin);
    }
    
    for (size_t i = 0; i < engine->registry->collector_count; i++) {
        if (engine->registry->collectors[i]->init) {
            engine->registry->collectors[i]->init(NULL);
        }
    }
    for (size_t i = 0; i < engine->registry->parser_count; i++) {
        if (engine->registry->parsers[i]->init) {
            engine->registry->parsers[i]->init(NULL);
        }
    }
    for (size_t i = 0; i < engine->registry->output_count; i++) {
        if (engine->registry->outputs[i]->init) {
            engine->registry->outputs[i]->init((void*)engine->config.siem_endpoint);
        }
    }
    
    forensic_audit_log_append(engine->audit_log, "system", "register_plugins", "", "");
    
    printf("[forensic_engine] Registered %zu collectors, %zu parsers, %zu outputs\n",
           engine->registry->collector_count,
           engine->registry->parser_count,
           engine->registry->output_count);
    return 0;
}

/* ============================================================================
 * Collection Phase
 * ============================================================================ */

static forensic_artifact_list_t* run_collector(forensic_engine_t* engine,
                                                forensic_data_source_plugin_t* collector,
                                                const char* target) {
    if (!forensic_permission_check_plugin(collector->name, engine->config.operator_role)) {
        fprintf(stderr, "[forensic_engine] Permission denied for collector: %s\n", collector->name);
        return NULL;
    }
    
    forensic_audit_log_append(engine->audit_log, "collector", collector->name, "", "");
    
    forensic_artifact_list_t* artifacts = collector->collect(target, NULL);
    
    char output_hash[128];
    snprintf(output_hash, sizeof(output_hash), "artifacts:%zu", artifacts ? artifacts->count : 0);
    forensic_audit_log_append(engine->audit_log, "collector", collector->name, "", output_hash);
    
    return artifacts;
}

int forensic_engine_run_collection(forensic_engine_t* engine,
                                    const char* target,
                                    forensic_parsed_artifact_list_t** parsed_output) {
    if (!engine || !parsed_output) return -1;
    
    printf("[forensic_engine] Starting collection phase...\n");
    
    forensic_artifact_list_t* all_artifacts = forensic_artifact_list_create(256);
    if (!all_artifacts) return -1;
    
    for (size_t i = 0; i < engine->registry->collector_count; i++) {
        forensic_data_source_plugin_t* collector = engine->registry->collectors[i];
        forensic_artifact_list_t* artifacts = run_collector(engine, collector, target);
        
        if (artifacts) {
            for (size_t j = 0; j < artifacts->count; j++) {
                forensic_artifact_list_add_deep(all_artifacts, &artifacts->items[j]);
            }
            forensic_artifact_list_destroy(artifacts);
        }
    }
    
    printf("[forensic_engine] Collected %zu total artifacts\n", all_artifacts->count);
    
    *parsed_output = calloc(1, sizeof(forensic_parsed_artifact_list_t));
    (*parsed_output)->items = calloc(all_artifacts->count, sizeof(forensic_parsed_artifact_t));
    (*parsed_output)->count = 0;
    
    for (size_t i = 0; i < all_artifacts->count; i++) {
        const forensic_artifact_t* artifact = &all_artifacts->items[i];
        
        forensic_artifact_parser_plugin_t* parser = forensic_plugin_registry_find_parser(
            engine->registry, artifact->artifact_type);
        
        if (!parser) {
            fprintf(stderr, "[forensic_engine] No parser for artifact type: %s\n", artifact->artifact_type);
            continue;
        }
        
        if (!forensic_permission_check_plugin(parser->name, engine->config.operator_role)) {
            fprintf(stderr, "[forensic_engine] Permission denied for parser: %s\n", parser->name);
            continue;
        }
        
        forensic_parsed_artifact_t* parsed = parser->parse(&artifact->raw, NULL);
        if (parsed) {
            forensic_evidence_store_add(engine->evidence_store, artifact, parsed, NULL);
            
            forensic_source_t source = {0};
            strncpy(source.evidence_id, "ev-", sizeof(source.evidence_id) - 1);
            strncpy(source.plugin_name, artifact->plugin_name, sizeof(source.plugin_name) - 1);
            strncpy(source.target, target ? target : "local", sizeof(source.target) - 1);
            strncpy(source.timestamp, artifact->timestamp, sizeof(source.timestamp) - 1);
            
            forensic_transform_t transform = {0};
            strncpy(transform.parser_name, parser->name, sizeof(transform.parser_name) - 1);
            strncpy(transform.version, parser->version, sizeof(transform.version) - 1);
            strncpy(transform.duration_ms, "0", sizeof(transform.duration_ms) - 1);
            
            forensic_output_t output = {0};
            strncpy(output.type, artifact->artifact_type, sizeof(output.type) - 1);
            strncpy(output.hash, "sha256:placeholder", sizeof(output.hash) - 1);
            strncpy(output.data_ref, "store://", sizeof(output.data_ref) - 1);
            
            forensic_provenance_record(engine->provenance, &source, &transform, &output);
            
            forensic_audit_log_append(engine->audit_log, "parser", parser->name, "raw", "parsed");
            
            (*parsed_output)->items[(*parsed_output)->count++] = *parsed;
            free(parsed);
        }
    }
    
    forensic_artifact_list_destroy(all_artifacts);
    printf("[forensic_engine] Parsed %zu artifacts\n", (*parsed_output)->count);
    return 0;
}

/* ============================================================================
 * Analysis Phase
 * ============================================================================ */

int forensic_engine_run_analysis(forensic_engine_t* engine,
                                  const forensic_parsed_artifact_list_t* parsed,
                                  forensic_analysis_result_t* result) {
    if (!engine || !parsed || !result) return -1;
    
    printf("[forensic_engine] Starting analysis phase...\n");
    
    forensic_audit_log_append(engine->audit_log, "analyzer", "analysis_start", "", "");
    
    forensic_analysis_run(parsed, result);
    forensic_analysis_correlate(result->timeline, result->iocs, engine->correlations);
    
    forensic_audit_log_append(engine->audit_log, "analyzer", "analysis_complete", "", "");
    
    return 0;
}

/* ============================================================================
 * Output Generation
 * ============================================================================ */

int forensic_engine_generate_outputs(forensic_engine_t* engine,
                                       const forensic_analysis_result_t* result) {
    if (!engine || !result) return -1;
    
    printf("[forensic_engine] Generating outputs...\n");
    
    forensic_audit_log_append(engine->audit_log, "output", "generation_start", "", "");
    
    forensic_metadata_t prov_meta = forensic_metadata_create(16);
    forensic_metadata_add(&prov_meta, "engine", "JOCKY Forensic Engine");
    
    // Create output directory path
    char output_dir[512];
    snprintf(output_dir, sizeof(output_dir), "%s/output", engine->config.evidence_base_path);
    
    // Create output directory
    mkdir(output_dir, 0755);
    
    for (size_t i = 0; i < engine->registry->output_count; i++) {
        forensic_output_plugin_t* output = engine->registry->outputs[i];
        
        if (!forensic_permission_check_plugin(output->name, engine->config.operator_role)) {
            continue;
        }
        
        forensic_audit_log_append(engine->audit_log, "output", output->name, "", "");
        
        // Pass output directory for file-based outputs, endpoint for SIEM
        if (strcmp(output->name, "siem_forwarder") == 0) {
            output->generate(result->timeline, result->iocs, &prov_meta,
                            (void*)engine->config.siem_endpoint);
        } else {
            output->generate(result->timeline, result->iocs, &prov_meta,
                            (void*)output_dir);
        }
    }
    
    forensic_metadata_destroy(&prov_meta);
    forensic_audit_log_append(engine->audit_log, "output", "generation_complete", "", "");
    
    return 0;
}

/* ============================================================================
 * Full Pipeline
 * ============================================================================ */

int forensic_engine_run_full_pipeline(forensic_engine_t* engine,
                                       const char* target,
                                       forensic_analysis_result_t* result) {
    if (!engine || !result) return -1;
    
    printf("[forensic_engine] ========== Starting Full Forensic Pipeline ==========\n");
    
    forensic_audit_log_append(engine->audit_log, "engine", "pipeline_start", "", "");
    
    if (engine->registry->collector_count == 0) {
        forensic_engine_register_default_plugins(engine);
    }
    
    forensic_parsed_artifact_list_t* parsed = NULL;
    int ret = forensic_engine_run_collection(engine, target, &parsed);
    if (ret != 0) {
        forensic_audit_log_append(engine->audit_log, "engine", "collection_failed", "", "");
        return ret;
    }
    
    ret = forensic_engine_run_analysis(engine, parsed, result);
    if (ret != 0) {
        forensic_audit_log_append(engine->audit_log, "engine", "analysis_failed", "", "");
        return ret;
    }
    
    ret = forensic_engine_generate_outputs(engine, result);
    if (ret != 0) {
        forensic_audit_log_append(engine->audit_log, "engine", "output_failed", "", "");
        return ret;
    }
    
    forensic_provenance_save(engine->provenance, "/var/jocky/forensics/provenance.json");
    
    forensic_audit_log_append(engine->audit_log, "engine", "pipeline_complete", "", "");
    
    printf("[forensic_engine] ========== Pipeline Complete ==========\n");
    return 0;
}