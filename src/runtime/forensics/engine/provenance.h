/**
 * Provenance Engine
 * 
 * Records source -> transform -> output chain for every piece of evidence.
 * Maintains hash chain for tamper-evidence.
 */

#pragma once

#include "../forensic_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Provenance Data Structures
 * ============================================================================ */

typedef struct {
    char evidence_id[64];
    char plugin_name[128];
    char target[256];
    char timestamp[64];
} forensic_source_t;

typedef struct {
    char parser_name[128];
    char version[32];
    char duration_ms[32];
} forensic_transform_t;

typedef struct {
    char type[64];
    char hash[128];
    char data_ref[256];
} forensic_output_t;

typedef struct {
    char prev_hash[128];
    char entry_hash[128];
} forensic_chain_t;

typedef struct {
    forensic_source_t source;
    forensic_transform_t transform;
    forensic_output_t output;
    forensic_chain_t chain;
} forensic_provenance_entry_t;

typedef struct {
    forensic_provenance_entry_t* entries;
    size_t count;
    size_t capacity;
    char last_hash[128];
} forensic_provenance_t;

/* ============================================================================
 * Provenance Engine API
 * ============================================================================ */

forensic_provenance_t* forensic_provenance_create(void);
void forensic_provenance_destroy(forensic_provenance_t* prov);

int forensic_provenance_record(forensic_provenance_t* prov,
                                const forensic_source_t* source,
                                const forensic_transform_t* transform,
                                const forensic_output_t* output);

int forensic_provenance_verify_chain(const forensic_provenance_t* prov);

const forensic_provenance_entry_t* forensic_provenance_get(const forensic_provenance_t* prov, size_t index);

int forensic_provenance_save(const forensic_provenance_t* prov, const char* path);
int forensic_provenance_load(forensic_provenance_t* prov, const char* path);

/* ============================================================================
 * Hash Chain Utilities
 * ============================================================================ */

void forensic_hash_compute(const char* input, size_t len, char* output, size_t output_size);
void forensic_hash_chain_compute(const char* prev_hash,
                                  const forensic_provenance_entry_t* entry,
                                  char* output, size_t output_size);

#ifdef __cplusplus
}
#endif