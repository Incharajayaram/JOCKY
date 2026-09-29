/**
 * Audit Log (Hash-Chained)
 * 
 * Every action is logged with tamper-evident hash chain.
 * Format: JSON Lines (.jsonl) - each line is one entry.
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Audit Log Data Structures
 * ============================================================================ */

typedef struct {
    uint64_t index;
    char timestamp[64];
    char actor[128];
    char action[128];
    char input_hash[128];
    char output_hash[128];
    char prev_entry_hash[128];
    char entry_hash[128];
} forensic_audit_entry_t;

typedef struct {
    FILE* file;
    char path[512];
    uint64_t next_index;
    char last_hash[128];
    bool initialized;
} forensic_audit_log_t;

/* ============================================================================
 * Audit Log API
 * ============================================================================ */

forensic_audit_log_t* forensic_audit_log_open(const char* path);
void forensic_audit_log_close(forensic_audit_log_t* log);

int forensic_audit_log_append(forensic_audit_log_t* log,
                               const char* actor,
                               const char* action,
                               const char* input_hash,
                               const char* output_hash);

int forensic_audit_log_verify(const forensic_audit_log_t* log);

int forensic_audit_log_read_entries(const forensic_audit_log_t* log,
                                     forensic_audit_entry_t** entries,
                                     size_t* count);

/* ============================================================================
 * Hash Functions
 * ============================================================================ */

void forensic_audit_compute_entry_hash(const forensic_audit_entry_t* entry,
                                        char* output, size_t output_size);

#ifdef __cplusplus
}
#endif