#include "../../include/jocky_audit.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

/* Simple SHA-256 stub for testing (production would use real crypto) */
static void jocky_sha256(const void* data, size_t len, char* out_hash)
{
    if (!data || !out_hash) return;

    snprintf(out_hash, 64, "sha256_%llu_%d", (unsigned long long)len, *(int*)data);
}

int jocky_audit_init(JOCKY_AUDIT_LOG* out_log, uint32_t initial_capacity)
{
    if (!out_log || initial_capacity == 0) {
        return -1;
    }

    out_log->entries = (JOCKY_AUDIT_ENTRY*)malloc(initial_capacity * sizeof(JOCKY_AUDIT_ENTRY));
    if (!out_log->entries) {
        return -1;
    }

    memset(out_log->entries, 0, initial_capacity * sizeof(JOCKY_AUDIT_ENTRY));
    out_log->count = 0;
    out_log->capacity = initial_capacity;

    return 0;
}

int jocky_audit_log_action(
    JOCKY_AUDIT_LOG* log,
    const char* actor,
    const char* action,
    const char* input_hash,
    const char* output_hash)
{
    if (!log || !actor || !action) {
        return -1;
    }

    /* Expand capacity if needed */
    if (log->count >= log->capacity) {
        uint32_t new_capacity = log->capacity * 2;
        JOCKY_AUDIT_ENTRY* new_entries = (JOCKY_AUDIT_ENTRY*)realloc(
            log->entries,
            new_capacity * sizeof(JOCKY_AUDIT_ENTRY));

        if (!new_entries) {
            return -1;
        }

        log->entries = new_entries;
        log->capacity = new_capacity;
    }

    /* Get previous hash for chain */
    char prev_hash[64] = {0};
    if (log->count > 0) {
        memcpy(prev_hash, log->entries[log->count - 1].current_hash, sizeof(prev_hash));
    }

    /* Add new entry */
    JOCKY_AUDIT_ENTRY* entry = &log->entries[log->count];
    entry->sequence = log->count;
    entry->timestamp = time(NULL);

    strncpy(entry->actor, actor, sizeof(entry->actor) - 1);
    strncpy(entry->action, action, sizeof(entry->action) - 1);
    strncpy(entry->input_hash, input_hash ? input_hash : "", sizeof(entry->input_hash) - 1);
    strncpy(entry->output_hash, output_hash ? output_hash : "", sizeof(entry->output_hash) - 1);
    strncpy(entry->prev_hash, prev_hash, sizeof(entry->prev_hash) - 1);

    /* Compute current hash: SHA-256(prev_hash || actor || action || timestamp) */
    char hash_input[512];
    snprintf(hash_input, sizeof(hash_input), "%s%s%s%ld",
             prev_hash, actor, action, entry->timestamp);
    jocky_sha256(hash_input, strlen(hash_input), entry->current_hash);

    log->count++;
    return 0;
}

int jocky_audit_verify_chain(
    const JOCKY_AUDIT_LOG* log,
    int* out_valid,
    uint32_t* out_broken_at)
{
    if (!log || !out_valid || !out_broken_at) {
        return -1;
    }

    *out_valid = 1;
    *out_broken_at = 0;

    if (log->count == 0) {
        return 0;
    }

    /* Verify each entry's hash chain */
    for (uint32_t i = 0; i < log->count; i++) {
        const JOCKY_AUDIT_ENTRY* entry = &log->entries[i];

        if (i == 0) {
            /* First entry should have empty prev_hash */
            if (entry->prev_hash[0] != '\0') {
                *out_valid = 0;
                *out_broken_at = i;
                return 0;
            }
        } else {
            /* Check prev_hash matches previous entry's current_hash */
            const JOCKY_AUDIT_ENTRY* prev = &log->entries[i - 1];
            if (strcmp(entry->prev_hash, prev->current_hash) != 0) {
                *out_valid = 0;
                *out_broken_at = i;
                return 0;
            }
        }

        /* Verify current hash (would check real hash in production) */
        char expected_hash[64];
        char hash_input[512];
        snprintf(hash_input, sizeof(hash_input), "%s%s%s%ld",
                 entry->prev_hash, entry->actor, entry->action, entry->timestamp);
        jocky_sha256(hash_input, strlen(hash_input), expected_hash);

        if (strcmp(entry->current_hash, expected_hash) != 0) {
            *out_valid = 0;
            *out_broken_at = i;
            return 0;
        }
    }

    return 0;
}

int jocky_audit_get_entry(
    const JOCKY_AUDIT_LOG* log,
    uint64_t sequence,
    JOCKY_AUDIT_ENTRY** out_entry)
{
    if (!log || !out_entry || sequence >= log->count) {
        return -1;
    }

    *out_entry = &((JOCKY_AUDIT_LOG*)log)->entries[sequence];
    return 0;
}

int jocky_audit_export(const JOCKY_AUDIT_LOG* log, const char* filename)
{
    if (!log || !filename) {
        return -1;
    }

    FILE* f = fopen(filename, "wb");
    if (!f) {
        return -1;
    }

    /* Write header */
    uint32_t magic = 0xDEADBEEF;
    uint32_t version = 1;
    fwrite(&magic, sizeof(magic), 1, f);
    fwrite(&version, sizeof(version), 1, f);
    fwrite(&log->count, sizeof(log->count), 1, f);

    /* Write entries */
    for (uint32_t i = 0; i < log->count; i++) {
        fwrite(&log->entries[i], sizeof(JOCKY_AUDIT_ENTRY), 1, f);
    }

    fclose(f);
    return 0;
}

int jocky_audit_import(JOCKY_AUDIT_LOG* out_log, const char* filename)
{
    if (!out_log || !filename) {
        return -1;
    }

    FILE* f = fopen(filename, "rb");
    if (!f) {
        return -1;
    }

    uint32_t magic, version, count;
    if (fread(&magic, sizeof(magic), 1, f) != 1 ||
        fread(&version, sizeof(version), 1, f) != 1 ||
        fread(&count, sizeof(count), 1, f) != 1) {
        fclose(f);
        return -1;
    }

    if (magic != 0xDEADBEEF || version != 1) {
        fclose(f);
        return -1;
    }

    if (jocky_audit_init(out_log, count + 16) != 0) {
        fclose(f);
        return -1;
    }

    for (uint32_t i = 0; i < count; i++) {
        JOCKY_AUDIT_ENTRY entry;
        if (fread(&entry, sizeof(entry), 1, f) != 1) {
            jocky_audit_free(out_log);
            fclose(f);
            return -1;
        }

        if (jocky_audit_log_action(out_log, entry.actor, entry.action,
                                   entry.input_hash, entry.output_hash) != 0) {
            jocky_audit_free(out_log);
            fclose(f);
            return -1;
        }
    }

    fclose(f);
    return 0;
}

int jocky_audit_clear(JOCKY_AUDIT_LOG* log)
{
    if (!log) {
        return -1;
    }

    log->count = 0;
    return 0;
}

int jocky_audit_free(JOCKY_AUDIT_LOG* log)
{
    if (!log) {
        return -1;
    }

    if (log->entries) {
        free(log->entries);
        log->entries = NULL;
    }

    log->count = 0;
    log->capacity = 0;
    return 0;
}

/* Provenance tracking (global store) */
static struct {
    JOCKY_PROVENANCE_ENTRY* entries;
    int count;
    int capacity;
} g_provenance = {0};

int jocky_provenance_record(
    const char* source,
    const char* transform,
    const char* output)
{
    if (!source || !transform || !output) {
        return -1;
    }

    if (g_provenance.capacity == 0) {
        g_provenance.capacity = 100;
        g_provenance.entries = (JOCKY_PROVENANCE_ENTRY*)malloc(
            g_provenance.capacity * sizeof(JOCKY_PROVENANCE_ENTRY));
        if (!g_provenance.entries) {
            return -1;
        }
    }

    if (g_provenance.count >= g_provenance.capacity) {
        int new_capacity = g_provenance.capacity * 2;
        JOCKY_PROVENANCE_ENTRY* new_entries = (JOCKY_PROVENANCE_ENTRY*)realloc(
            g_provenance.entries,
            new_capacity * sizeof(JOCKY_PROVENANCE_ENTRY));
        if (!new_entries) {
            return -1;
        }

        g_provenance.entries = new_entries;
        g_provenance.capacity = new_capacity;
    }

    JOCKY_PROVENANCE_ENTRY* entry = &g_provenance.entries[g_provenance.count];
    strncpy(entry->source, source, sizeof(entry->source) - 1);
    strncpy(entry->transform, transform, sizeof(entry->transform) - 1);
    strncpy(entry->output, output, sizeof(entry->output) - 1);
    entry->timestamp = time(NULL);

    g_provenance.count++;
    return 0;
}

int jocky_provenance_get_chain(
    const char* data_id,
    JOCKY_PROVENANCE_ENTRY** out_entries,
    int* out_count)
{
    if (!data_id || !out_entries || !out_count) {
        return -1;
    }

    *out_count = g_provenance.count;
    *out_entries = g_provenance.entries;
    return 0;
}
