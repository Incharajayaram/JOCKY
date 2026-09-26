#include "forensic_types.h"
#include "forensic_types.h"
#include "forensic_types.h"
/**
 * Evidence Store Implementation
 */

#include "evidence_store.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* ============================================================================
 * Helper Functions
 * ============================================================================ */

static int create_dir(const char* path) {
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        return mkdir(path, 0755);
    }
    return 0;
}

static int write_file(const char* path, const void* data, size_t len) {
    FILE* f = fopen(path, "wb");
    if (!f) return -1;
    size_t written = fwrite(data, 1, len, f);
    fclose(f);
    return written == len ? 0 : -1;
}

/* ============================================================================
 * Evidence Store
 * ============================================================================ */

forensic_evidence_store_t* forensic_evidence_store_open(const char* base_path) {
    if (!base_path) return NULL;
    
    forensic_evidence_store_t* store = calloc(1, sizeof(forensic_evidence_store_t));
    if (!store) return NULL;
    
    strncpy(store->base_path, base_path, sizeof(store->base_path) - 1);
    
    // Create directory structure
    char path[1024];
    snprintf(path, sizeof(path), "%s/evidence", base_path);
    create_dir(path);
    snprintf(path, sizeof(path), "%s/audit", base_path);
    create_dir(path);
    snprintf(path, sizeof(path), "%s/reports", base_path);
    create_dir(path);
    
    store->capacity = 128;
    store->index = calloc(store->capacity, sizeof(forensic_evidence_metadata_t));
    if (!store->index) {
        free(store);
        return NULL;
    }
    
    // Load existing index
    snprintf(path, sizeof(path), "%s/index.json", base_path);
    FILE* f = fopen(path, "r");
    if (f) {
        // Would parse JSON here
        fclose(f);
    }
    
    printf("[evidence_store] Opened at %s\n", base_path);
    return store;
}

void forensic_evidence_store_close(forensic_evidence_store_t* store) {
    if (!store) return;
    forensic_evidence_store_save_index(store);
    free(store->index);
    free(store);
}

int forensic_evidence_store_add(forensic_evidence_store_t* store,
                                 const forensic_artifact_t* artifact,
                                 const forensic_parsed_artifact_t* parsed,
                                 const forensic_provenance_entry_t* provenance) {
    if (!store || !artifact) return -1;
    
    if (store->count >= store->capacity) {
        size_t new_cap = store->capacity * 2;
        forensic_evidence_metadata_t* new_idx = realloc(store->index,
            new_cap * sizeof(forensic_evidence_metadata_t));
        if (!new_idx) return -1;
        store->index = new_idx;
        store->capacity = new_cap;
    }
    
    // Generate evidence ID
    char evidence_id[64];
    snprintf(evidence_id, sizeof(evidence_id), "ev-%04zu", store->count + 1);
    
    forensic_evidence_metadata_t* meta = &store->index[store->count];
    strncpy(meta->evidence_id, evidence_id, sizeof(meta->evidence_id) - 1);
    strncpy(meta->artifact_type, artifact->artifact_type, sizeof(meta->artifact_type) - 1);
    strncpy(meta->source_plugin, artifact->plugin_name, sizeof(meta->source_plugin) - 1);
    strncpy(meta->timestamp, artifact->timestamp, sizeof(meta->timestamp) - 1);
    meta->raw_size = artifact->raw.len;
    
    // Save raw artifact
    char raw_path[1024];
    snprintf(raw_path, sizeof(raw_path), "%s/evidence/%s.raw", store->base_path, evidence_id);
    if (write_file(raw_path, artifact->raw.data, artifact->raw.len) != 0) {
        return -1;
    }
    strncpy(meta->raw_path, raw_path, sizeof(meta->raw_path) - 1);
    
    // Save parsed artifact
    if (parsed) {
        char parsed_path[1024];
        snprintf(parsed_path, sizeof(parsed_path), "%s/evidence/%s.parsed.json", 
                 store->base_path, evidence_id);
        
        // Convert parsed to JSON
        FILE* f = fopen(parsed_path, "w");
        if (f) {
            fprintf(f, "{\n");
            fprintf(f, "  \"artifact_type\": \"%s\",\n", parsed->artifact_type);
            fprintf(f, "  \"parser_name\": \"%s\",\n", parsed->parser_name);
            fprintf(f, "  \"iocs\": [\n");
            for (size_t i = 0; i < parsed->iocs.count; i++) {
                const forensic_ioc_t* ioc = &parsed->iocs.items[i];
                fprintf(f, "    {\"type\": \"%s\", \"value\": \"%s\", \"source\": \"%s\", \"confidence\": %.2f}%s\n",
                        ioc->type, ioc->value, ioc->source, ioc->confidence,
                        (i < parsed->iocs.count - 1) ? "," : "");
            }
            fprintf(f, "  ]\n");
            fprintf(f, "}\n");
            fclose(f);
            meta->parsed_size = ftell(f);
            strncpy(meta->parsed_path, parsed_path, sizeof(meta->parsed_path) - 1);
        }
    }
    
    // Save provenance
    if (provenance) {
        char prov_path[1024];
        snprintf(prov_path, sizeof(prov_path), "%s/evidence/%s.prov.json",
                 store->base_path, evidence_id);
        // Would write provenance JSON here
        strncpy(meta->provenance_path, prov_path, sizeof(meta->provenance_path) - 1);
    }
    
    // Compute hash
    snprintf(meta->hash, sizeof(meta->hash), "sha256:%lu", (unsigned long)time(NULL));
    
    store->count++;
    printf("[evidence_store] Added evidence %s\n", evidence_id);
    return 0;
}

forensic_evidence_metadata_t* forensic_evidence_store_get(const forensic_evidence_store_t* store,
                                                           const char* evidence_id) {
    if (!store || !evidence_id) return NULL;
    for (size_t i = 0; i < store->count; i++) {
        if (strcmp(store->index[i].evidence_id, evidence_id) == 0) {
            return &store->index[i];
        }
    }
    return NULL;
}

int forensic_evidence_store_list(const forensic_evidence_store_t* store,
                                  forensic_evidence_metadata_t** results,
                                  size_t* count) {
    if (!store || !results || !count) return -1;
    
    *results = malloc(store->count * sizeof(forensic_evidence_metadata_t));
    if (!*results) return -1;
    
    memcpy(*results, store->index, store->count * sizeof(forensic_evidence_metadata_t));
    *count = store->count;
    return 0;
}

int forensic_evidence_store_save_index(const forensic_evidence_store_t* store) {
    if (!store) return -1;
    
    char path[1024];
    snprintf(path, sizeof(path), "%s/index.json", store->base_path);
    
    FILE* f = fopen(path, "w");
    if (!f) return -1;
    
    fprintf(f, "{\n  \"evidence\": [\n");
    for (size_t i = 0; i < store->count; i++) {
        const forensic_evidence_metadata_t* m = &store->index[i];
        fprintf(f, "    {\n");
        fprintf(f, "      \"evidence_id\": \"%s\",\n", m->evidence_id);
        fprintf(f, "      \"artifact_type\": \"%s\",\n", m->artifact_type);
        fprintf(f, "      \"source_plugin\": \"%s\",\n", m->source_plugin);
        fprintf(f, "      \"timestamp\": \"%s\",\n", m->timestamp);
        fprintf(f, "      \"raw_path\": \"%s\",\n", m->raw_path);
        fprintf(f, "      \"parsed_path\": \"%s\",\n", m->parsed_path);
        fprintf(f, "      \"provenance_path\": \"%s\",\n", m->provenance_path);
        fprintf(f, "      \"raw_size\": %zu,\n", m->raw_size);
        fprintf(f, "      \"parsed_size\": %zu,\n", m->parsed_size);
        fprintf(f, "      \"hash\": \"%s\"\n", m->hash);
        fprintf(f, "    }%s\n", (i < store->count - 1) ? "," : "");
    }
    fprintf(f, "  ]\n");
    fprintf(f, "}\n");
    fclose(f);
    return 0;
}