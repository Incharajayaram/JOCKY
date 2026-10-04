/**
 * Analysis Engine
 * 
 * Three functions:
 * A. Timeline Reconstruction - chronological list of events
 * B. IOC Extraction - hashes, IPs, domains, paths, registry keys
 * C. Correlation - link related events (process spawns, network, file, registry)
 */

#pragma once

#include "../forensic_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Analysis Engine API
 * ============================================================================ */

typedef struct {
    forensic_parsed_artifact_list_t* parsed_artifacts;
    forensic_timeline_t* timeline;
    forensic_ioc_list_t* iocs;
} forensic_analysis_result_t;

forensic_analysis_result_t* forensic_analysis_create(void);
void forensic_analysis_destroy(forensic_analysis_result_t* result);

/* A. Timeline Reconstruction */
int forensic_analysis_build_timeline(const forensic_parsed_artifact_list_t* parsed,
                                      forensic_timeline_t* timeline);

/* B. IOC Extraction */
int forensic_analysis_extract_iocs(const forensic_parsed_artifact_list_t* parsed,
                                    forensic_ioc_list_t* iocs);

/* C. Correlation */
typedef struct {
    char source_event[128];
    char target_event[128];
    char relationship[64];
    double confidence;
} forensic_correlation_t;

typedef struct {
    forensic_correlation_t* correlations;
    size_t count;
} forensic_correlation_graph_t;

int forensic_analysis_correlate(const forensic_timeline_t* timeline,
                                 const forensic_ioc_list_t* iocs,
                                 forensic_correlation_graph_t* graph);

void forensic_correlation_graph_destroy(forensic_correlation_graph_t* graph);

/* Full Analysis Pipeline */
int forensic_analysis_run(const forensic_parsed_artifact_list_t* parsed,
                           forensic_analysis_result_t* result);

#ifdef __cplusplus
}
#endif