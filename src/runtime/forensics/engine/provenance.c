#include "forensic_types.h"
#include "forensic_types.h"
#include "forensic_types.h"
/**
 * Provenance Engine Implementation
 */

#include "provenance.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

/* ============================================================================
 * Simple Hash Function (SHA-256 would be used in production)
 * ============================================================================ */

static uint32_t simple_hash(const char* data, size_t len) {
    uint32_t hash = 5381;
    for (size_t i = 0; i < len; i++) {
        hash = ((hash << 5) + hash) + (unsigned char)data[i];
    }
    return hash;
}

void forensic_hash_compute(const char* input, size_t len, char* output, size_t output_size) {
    // In production, use OpenSSL SHA-256
    // This is a simplified hash for demonstration
    uint32_t h1 = simple_hash(input, len);
    uint32_t h2 = simple_hash(input + len/2, len - len/2);
    snprintf(output, output_size, "%08x%08x", h1, h2);
}

void forensic_hash_chain_compute(const char* prev_hash,
                                  const forensic_provenance_entry_t* entry,
                                  char* output, size_t output_size) {
    char combined[2048];
    snprintf(combined, sizeof(combined), "%s%s%s%s%s%s%s%s%s",
             prev_hash ? prev_hash : "",
             entry->source.evidence_id,
             entry->source.plugin_name,
             entry->source.target,
             entry->source.timestamp,
             entry->transform.parser_name,
             entry->transform.version,
             entry->transform.duration_ms,
             entry->output.type,
             entry->output.hash,
             entry->output.data_ref);
    
    forensic_hash_compute(combined, strlen(combined), output, output_size);
}

/* ============================================================================
 * Provenance Engine
 * ============================================================================ */

forensic_provenance_t* forensic_provenance_create(void) {
    forensic_provenance_t* prov = calloc(1, sizeof(forensic_provenance_t));
    if (!prov) return NULL;
    
    prov->capacity = 128;
    prov->entries = calloc(prov->capacity, sizeof(forensic_provenance_entry_t));
    if (!prov->entries) {
        free(prov);
        return NULL;
    }
    prov->last_hash[0] = '\0';
    return prov;
}

void forensic_provenance_destroy(forensic_provenance_t* prov) {
    if (!prov) return;
    free(prov->entries);
    free(prov);
}

int forensic_provenance_record(forensic_provenance_t* prov,
                                const forensic_source_t* source,
                                const forensic_transform_t* transform,
                                const forensic_output_t* output) {
    if (!prov || !source || !transform || !output) return -1;
    
    if (prov->count >= prov->capacity) {
        size_t new_cap = prov->capacity * 2;
        forensic_provenance_entry_t* new_entries = realloc(prov->entries,
            new_cap * sizeof(forensic_provenance_entry_t));
        if (!new_entries) return -1;
        prov->entries = new_entries;
        prov->capacity = new_cap;
    }
    
    forensic_provenance_entry_t* entry = &prov->entries[prov->count];
    memcpy(&entry->source, source, sizeof(forensic_source_t));
    memcpy(&entry->transform, transform, sizeof(forensic_transform_t));
    memcpy(&entry->output, output, sizeof(forensic_output_t));
    
    // Compute chain hash
    forensic_hash_chain_compute(prov->last_hash[0] ? prov->last_hash : NULL,
                                 entry, entry->chain.entry_hash, sizeof(entry->chain.entry_hash));
    
    if (prov->count > 0) {
        strncpy(entry->chain.prev_hash, prov->last_hash, sizeof(entry->chain.prev_hash) - 1);
    } else {
        entry->chain.prev_hash[0] = '\0';
    }
    
    // Update last hash
    strncpy(prov->last_hash, entry->chain.entry_hash, sizeof(prov->last_hash) - 1);
    
    prov->count++;
    return 0;
}

int forensic_provenance_verify_chain(const forensic_provenance_t* prov) {
    if (!prov || prov->count == 0) return 0;
    
    char expected_prev[128] = {0};
    
    for (size_t i = 0; i < prov->count; i++) {
        const forensic_provenance_entry_t* entry = &prov->entries[i];
        
        // Verify prev_hash matches previous entry's hash
        if (i > 0 && strcmp(entry->chain.prev_hash, expected_prev) != 0) {
            fprintf(stderr, "[provenance] Chain broken at index %zu: prev_hash mismatch\n", i);
            return -1;
        }
        
        // Recompute entry hash
        char computed_hash[128];
        forensic_hash_chain_compute(i > 0 ? expected_prev : NULL,
                                     entry, computed_hash, sizeof(computed_hash));
        
        if (strcmp(entry->chain.entry_hash, computed_hash) != 0) {
            fprintf(stderr, "[provenance] Chain broken at index %zu: entry_hash mismatch\n", i);
            return -1;
        }
        
        strncpy(expected_prev, entry->chain.entry_hash, sizeof(expected_prev) - 1);
    }
    
    printf("[provenance] Chain verified: %zu entries, integrity OK\n", prov->count);
    return 0;
}

const forensic_provenance_entry_t* forensic_provenance_get(const forensic_provenance_t* prov, size_t index) {
    if (!prov || index >= prov->count) return NULL;
    return &prov->entries[index];
}

int forensic_provenance_save(const forensic_provenance_t* prov, const char* path) {
    if (!prov || !path) return -1;
    
    FILE* f = fopen(path, "w");
    if (!f) return -1;
    
    fprintf(f, "{\n");
    fprintf(f, "  \"last_hash\": \"%s\",\n", prov->last_hash);
    fprintf(f, "  \"entries\": [\n");
    
    for (size_t i = 0; i < prov->count; i++) {
        const forensic_provenance_entry_t* e = &prov->entries[i];
        fprintf(f, "    {\n");
        fprintf(f, "      \"source\": {\n");
        fprintf(f, "        \"evidence_id\": \"%s\",\n", e->source.evidence_id);
        fprintf(f, "        \"plugin_name\": \"%s\",\n", e->source.plugin_name);
        fprintf(f, "        \"target\": \"%s\",\n", e->source.target);
        fprintf(f, "        \"timestamp\": \"%s\"\n", e->source.timestamp);
        fprintf(f, "      },\n");
        fprintf(f, "      \"transform\": {\n");
        fprintf(f, "        \"parser_name\": \"%s\",\n", e->transform.parser_name);
        fprintf(f, "        \"version\": \"%s\",\n", e->transform.version);
        fprintf(f, "        \"duration_ms\": \"%s\"\n", e->transform.duration_ms);
        fprintf(f, "      },\n");
        fprintf(f, "      \"output\": {\n");
        fprintf(f, "        \"type\": \"%s\",\n", e->output.type);
        fprintf(f, "        \"hash\": \"%s\",\n", e->output.hash);
        fprintf(f, "        \"data_ref\": \"%s\"\n", e->output.data_ref);
        fprintf(f, "      },\n");
        fprintf(f, "      \"chain\": {\n");
        fprintf(f, "        \"prev_hash\": \"%s\",\n", e->chain.prev_hash);
        fprintf(f, "        \"entry_hash\": \"%s\"\n", e->chain.entry_hash);
        fprintf(f, "      }\n");
        fprintf(f, "    }%s\n", (i < prov->count - 1) ? "," : "");
    }
    
    fprintf(f, "  ]\n");
    fprintf(f, "}\n");
    fclose(f);
    return 0;
}

int forensic_provenance_load(forensic_provenance_t* prov, const char* path) {
    // JSON parsing would be needed here
    // Simplified implementation - returns error
    (void)prov;
    (void)path;
    return -1;
}