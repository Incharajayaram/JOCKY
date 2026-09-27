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
 * Retention & Compression Policy
 * ============================================================================ */

typedef struct {
    // Time-based retention
    int max_age_days;          // Delete evidence older than this (0 = disabled)
    // Size-based retention
    size_t max_total_size_mb;  // Delete oldest when total exceeds this (0 = disabled)
    size_t max_evidence_count; // Delete oldest when count exceeds this (0 = disabled)
    // Compression
    int compress_after_days;   // Compress files older than this (0 = disabled)
    int compression_level;     // zlib compression level 1-9 (default 6)
    // Auto-cleanup
    bool auto_cleanup_on_open; // Run cleanup when opening store
    bool auto_cleanup_on_close; // Run cleanup when closing store
} forensic_retention_policy_t;

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
    bool compressed;           // Whether files are compressed
    time_t compressed_at;      // When compression happened
} forensic_evidence_metadata_t;

typedef struct {
    char base_path[512];
    forensic_evidence_metadata_t* index;
    size_t count;
    size_t capacity;
    forensic_retention_policy_t retention_policy;
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

/* ============================================================================
 * Retention & Compression API
 * ============================================================================ */

void forensic_evidence_store_default_policy(forensic_retention_policy_t* policy);

int forensic_evidence_store_set_policy(forensic_evidence_store_t* store,
                                        const forensic_retention_policy_t* policy);

int forensic_evidence_store_cleanup(forensic_evidence_store_t* store);

int forensic_evidence_store_compress_old(forensic_evidence_store_t* store);

int forensic_evidence_store_get_stats(const forensic_evidence_store_t* store,
                                       size_t* total_raw_bytes,
                                       size_t* total_compressed_bytes,
                                       size_t* evidence_count,
                                       size_t* oldest_evidence_age_days,
                                       size_t* newest_evidence_age_days);

#ifdef __cplusplus
}
#endif