#ifndef JOCKY_AUDIT_H
#define JOCKY_AUDIT_H

#include <stdint.h>
#include <time.h>

/* Audit and Provenance API - Hash-chained audit logs for accountability
 *
 * Supports dual-use: offensive operations can audit themselves for defensive review.
 * Every operation is logged with cryptographic hash chaining for tamper-evidence.
 */

typedef struct {
    uint64_t sequence;
    time_t timestamp;
    char actor[256];           /* Who performed the action */
    char action[256];          /* What action was performed */
    char input_hash[64];       /* SHA-256 of input data */
    char output_hash[64];      /* SHA-256 of output data */
    char prev_hash[64];        /* Previous entry's hash (chain) */
    char current_hash[64];     /* This entry's hash */
} JOCKY_AUDIT_ENTRY;

typedef struct {
    JOCKY_AUDIT_ENTRY* entries;
    uint32_t count;
    uint32_t capacity;
} JOCKY_AUDIT_LOG;

/* Initialize audit log */
int jocky_audit_init(JOCKY_AUDIT_LOG* out_log, uint32_t initial_capacity);

/* Record an operation in audit log */
int jocky_audit_log_action(
    JOCKY_AUDIT_LOG* log,
    const char* actor,
    const char* action,
    const char* input_hash,
    const char* output_hash);

/* Verify audit chain integrity */
int jocky_audit_verify_chain(
    const JOCKY_AUDIT_LOG* log,
    int* out_valid,
    uint32_t* out_broken_at);

/* Get audit entry by sequence number */
int jocky_audit_get_entry(
    const JOCKY_AUDIT_LOG* log,
    uint64_t sequence,
    JOCKY_AUDIT_ENTRY** out_entry);

/* Export audit log to file */
int jocky_audit_export(const JOCKY_AUDIT_LOG* log, const char* filename);

/* Import audit log from file */
int jocky_audit_import(JOCKY_AUDIT_LOG* out_log, const char* filename);

/* Clear audit log */
int jocky_audit_clear(JOCKY_AUDIT_LOG* log);

/* Free audit log resources */
int jocky_audit_free(JOCKY_AUDIT_LOG* log);

/* Provenance tracking - track data transformations */
typedef struct {
    char source[512];         /* Where data came from */
    char transform[256];      /* What transformation was applied */
    char output[512];         /* Where result went */
    uint64_t timestamp;
} JOCKY_PROVENANCE_ENTRY;

int jocky_provenance_record(
    const char* source,
    const char* transform,
    const char* output);

int jocky_provenance_get_chain(
    const char* data_id,
    JOCKY_PROVENANCE_ENTRY** out_entries,
    int* out_count);

#endif
