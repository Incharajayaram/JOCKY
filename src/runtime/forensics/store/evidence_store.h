/**
 * Evidence & Report Store
 * 
 * Storage layout:
 * /var/jocky/forensics/
 * ├── evidence/
 * │   ├── ev-001.raw          # original bytes (compressed)
 * │   ├── ev-001.parsed.json  # parsed output
 * │   └── ev-001.prov.json    # provenance entry
 * ├── audit/
 * │   └── audit.jsonl         # hash-chained log
 * ├── reports/
 * │   ├── report-2026-09-25.json
 * │   ├── report-2026-09-25.stix.json
 * │   └── report-2026-09-25.sigma.yaml
 * └── index.db                # SQLite: evidence_id -> metadata
 */

#pragma once

#include "forensic_types.h"
#include "engine/provenance.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Evidence Store Data Structures
 * ============================================================================ */

typedef struct {
    char evidence_id[64];
    char artifact_type[64];
    char source_plugin[128];
    char timestamp[64];
    char raw_path[512];
    char parsed_path[512];
    char provenance_path[512];
    size_t raw_size;
    size_t parsed_size;
    char hash[128];
} forensic_evidence_metadata_t;

typedef struct {
    char base_path[512];
    forensic_evidence_metadata_t* index;
    size_t count;
    size_t capacity;
} forensic_evidence_store_t;

/* ============================================================================
 * Evidence Store API
 * ============================================================================ */

forensic_evidence_store_t* forensic_evidence_store_open(const char* base_path);
void forensic_evidence_store_close(forensic_evidence_store_t* store);

int forensic_evidence_store_add(forensic_evidence_store_t* store,
                                 const forensic_artifact_t* artifact,
                                 const forensic_parsed_artifact_t* parsed,
                                 const forensic_provenance_entry_t* provenance);

forensic_evidence_metadata_t* forensic_evidence_store_get(const forensic_evidence_store_t* store,
                                                           const char* evidence_id);

int forensic_evidence_store_list(const forensic_evidence_store_t* store,
                                  forensic_evidence_metadata_t** results,
                                  size_t* count);

int forensic_evidence_store_save_index(const forensic_evidence_store_t* store);

#ifdef __cplusplus
}
#endif