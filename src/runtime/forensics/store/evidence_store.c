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
#include <dirent.h>
#include <zlib.h>

/* ============================================================================
 * Compression Helpers (zlib)
 * ============================================================================ */

static int compress_file(const char* input_path, const char* output_path, int level) {
    FILE* in = fopen(input_path, "rb");
    if (!in) return -1;
    
    gzFile out = gzopen(output_path, "wb");
    if (!out) {
        fclose(in);
        return -1;
    }
    
    gzsetparams(out, level, Z_DEFAULT_STRATEGY);
    
    char buffer[8192];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), in)) > 0) {
        if (gzwrite(out, buffer, bytes) != (int)bytes) {
            fclose(in);
            gzclose(out);
            return -1;
        }
    }
    
    fclose(in);
    gzclose(out);
    return 0;
}

static int decompress_file(const char* input_path, const char* output_path) {
    gzFile in = gzopen(input_path, "rb");
    if (!in) return -1;
    
    FILE* out = fopen(output_path, "wb");
    if (!out) {
        gzclose(in);
        return -1;
    }
    
    char buffer[8192];
    int bytes;
    while ((bytes = gzread(in, buffer, sizeof(buffer))) > 0) {
        if (fwrite(buffer, 1, bytes, out) != (size_t)bytes) {
            fclose(out);
            gzclose(in);
            return -1;
        }
    }
    
    fclose(out);
    gzclose(in);
    return 0;
}

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
 * Default Retention Policy
 * ============================================================================ */

void forensic_evidence_store_default_policy(forensic_retention_policy_t* policy) {
    if (!policy) return;
    policy->max_age_days = 30;           // Delete evidence older than 30 days
    policy->max_total_size_mb = 1024;    // Max 1GB total
    policy->max_evidence_count = 10000;  // Max 10k evidence items
    policy->compress_after_days = 7;     // Compress after 7 days
    policy->compression_level = 6;       // Default zlib level
    policy->auto_cleanup_on_open = false;
    policy->auto_cleanup_on_close = false;
}

/* ============================================================================
 * Policy Functions
 * ============================================================================ */

int forensic_evidence_store_set_policy(forensic_evidence_store_t* store,
                                        const forensic_retention_policy_t* policy) {
    if (!store || !policy) return -1;
    store->retention_policy = *policy;
    return 0;
}

/* ============================================================================
 * Cleanup & Compression Functions
 * ============================================================================ */

static time_t parse_timestamp(const char* timestamp_str) {
    struct tm tm = {0};
    strptime(timestamp_str, "%Y-%m-%dT%H:%M:%SZ", &tm);
    return mktime(&tm);
}

static bool should_delete_evidence(const forensic_evidence_metadata_t* meta,
                                    const forensic_retention_policy_t* policy,
                                    time_t now) {
    if (!meta || !policy) return false;
    
    time_t evidence_time = parse_timestamp(meta->timestamp);
    double age_days = difftime(now, evidence_time) / 86400.0;
    
    // Check age
    if (policy->max_age_days > 0 && age_days > policy->max_age_days) {
        return true;
    }
    return false;
}

static bool should_compress_evidence(const forensic_evidence_metadata_t* meta,
                                      const forensic_retention_policy_t* policy,
                                      time_t now) {
    if (!meta || !policy) return false;
    if (meta->compressed) return false;
    
    time_t evidence_time = parse_timestamp(meta->timestamp);
    double age_days = difftime(now, evidence_time) / 86400.0;
    
    if (policy->compress_after_days > 0 && age_days > policy->compress_after_days) {
        return true;
    }
    return false;
}

int forensic_evidence_store_compress_old(forensic_evidence_store_t* store) {
    if (!store) return -1;
    
    time_t now = time(NULL);
    int compressed = 0;
    
    for (size_t i = 0; i < store->count; i++) {
        forensic_evidence_metadata_t* meta = &store->index[i];
        
        if (should_compress_evidence(meta, &store->retention_policy, now)) {
            char compressed_raw[512];
            char compressed_parsed[512];
            
            snprintf(compressed_raw, sizeof(compressed_raw), "%s.gz", meta->raw_path);
            snprintf(compressed_parsed, sizeof(compressed_parsed), "%s.gz", meta->parsed_path);
            
            int level = store->retention_policy.compression_level;
            if (level < 1) level = 1;
            if (level > 9) level = 9;
            
            if (compress_file(meta->raw_path, compressed_raw, level) == 0) {
                unlink(meta->raw_path);
                rename(compressed_raw, meta->raw_path);
                meta->compressed = true;
                meta->compressed_at = time(NULL);
                compressed++;
            }
            
            if (compress_file(meta->parsed_path, compressed_parsed, level) == 0) {
                unlink(meta->parsed_path);
                rename(compressed_parsed, meta->parsed_path);
                compressed++;
            }
            
            if (meta->provenance_path[0]) {
                char compressed_prov[512];
                snprintf(compressed_prov, sizeof(compressed_prov), "%s.gz", meta->provenance_path);
                compress_file(meta->provenance_path, compressed_prov, store->retention_policy.compression_level);
                unlink(meta->provenance_path);
                rename(compressed_prov, meta->provenance_path);
            }
        }
    }
    
    if (compressed > 0) {
        forensic_evidence_store_save_index(store);
        printf("[evidence_store] Compressed %d old evidence files\n", compressed);
    }
    return compressed;
}

int forensic_evidence_store_cleanup(forensic_evidence_store_t* store) {
    if (!store) return -1;
    
    time_t now = time(NULL);
    int deleted = 0;
    
    // First pass: mark for deletion
    bool* to_delete = calloc(store->count, sizeof(bool));
    if (!to_delete) return -1;
    
    // Apply age-based deletion
    if (store->retention_policy.max_age_days > 0) {
        for (size_t i = 0; i < store->count; i++) {
            if (should_delete_evidence(&store->index[i], &store->retention_policy, now)) {
                to_delete[i] = true;
            }
        }
    }
    
    // Apply size-based deletion (oldest first)
    if (store->retention_policy.max_total_size_mb > 0) {
        size_t total_size = 0;
        for (size_t i = 0; i < store->count; i++) {
            total_size += store->index[i].raw_size;
        }
        
        size_t max_bytes = store->retention_policy.max_total_size_mb * 1024 * 1024;
        if (total_size > max_bytes) {
            // Sort by age (oldest first) and mark for deletion
            size_t* indices = malloc(store->count * sizeof(size_t));
            for (size_t i = 0; i < store->count; i++) indices[i] = i;
            
            // Simple bubble sort by timestamp (oldest first)
            for (size_t i = 0; i < store->count - 1; i++) {
                for (size_t j = 0; j < store->count - i - 1; j++) {
                    time_t t1 = parse_timestamp(store->index[indices[j]].timestamp);
                    time_t t2 = parse_timestamp(store->index[indices[j+1]].timestamp);
                    if (t1 > t2) {
                        size_t tmp = indices[j];
                        indices[j] = indices[j+1];
                        indices[j+1] = tmp;
                    }
                }
            }
            
            size_t freed = 0;
            for (size_t i = 0; i < store->count && total_size > max_bytes; i++) {
                size_t idx = indices[i];
                if (!to_delete[idx]) {
                    to_delete[idx] = true;
                    freed += store->index[idx].raw_size;
                    total_size -= store->index[idx].raw_size;
                }
            }
            free(indices);
        }
    }
    
    // Apply count-based deletion
    if (store->retention_policy.max_evidence_count > 0 && store->count > store->retention_policy.max_evidence_count) {
        size_t excess = store->count - store->retention_policy.max_evidence_count;
        size_t* indices = malloc(store->count * sizeof(size_t));
        for (size_t i = 0; i < store->count; i++) indices[i] = i;
        
        // Sort by timestamp (oldest first)
        for (size_t i = 0; i < store->count - 1; i++) {
            for (size_t j = 0; j < store->count - i - 1; j++) {
                time_t t1 = parse_timestamp(store->index[indices[j]].timestamp);
                time_t t2 = parse_timestamp(store->index[indices[j+1]].timestamp);
                if (t1 > t2) {
                    size_t tmp = indices[j];
                    indices[j] = indices[j+1];
                    indices[j+1] = tmp;
                }
            }
        }
        
        for (size_t i = 0; i < excess; i++) {
            to_delete[indices[i]] = true;
        }
        free(indices);
    }
    
    // Second pass: actually delete
    for (size_t i = 0; i < store->count; i++) {
        if (to_delete[i]) {
            forensic_evidence_metadata_t* meta = &store->index[i];
            unlink(meta->raw_path);
            unlink(meta->parsed_path);
            if (meta->provenance_path[0]) unlink(meta->provenance_path);
            deleted++;
        }
    }
    
    // Compact the index
    size_t write_idx = 0;
    for (size_t i = 0; i < store->count; i++) {
        if (!to_delete[i]) {
            if (write_idx != i) {
                store->index[write_idx] = store->index[i];
            }
            write_idx++;
        }
    }
    
    store->count = write_idx;
    free(to_delete);
    
    if (deleted > 0) {
        forensic_evidence_store_save_index(store);
        printf("[evidence_store] Cleaned up %d old evidence items\n", deleted);
    }
    return deleted;
}

forensic_evidence_store_t* forensic_evidence_store_open(const char* base_path) {
    if (!base_path) return NULL;
    
    forensic_evidence_store_t* store = calloc(1, sizeof(forensic_evidence_store_t));
    if (!store) return NULL;
    
    strncpy(store->base_path, base_path, sizeof(store->base_path) - 1);
    
    // Set default retention policy
    forensic_evidence_store_default_policy(&store->retention_policy);
    
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
    
    // Run cleanup on open if configured
    if (store->retention_policy.auto_cleanup_on_open) {
        forensic_evidence_store_cleanup(store);
    }
    
    printf("[evidence_store] Opened at %s\n", base_path);
    return store;
}

void forensic_evidence_store_close(forensic_evidence_store_t* store) {
    if (!store) return;
    
    // Run cleanup on close if configured
    if (store->retention_policy.auto_cleanup_on_close) {
        forensic_evidence_store_cleanup(store);
    }
    
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
    meta->compressed = false;
    meta->compressed_at = 0;
    
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

int forensic_evidence_store_get_stats(const forensic_evidence_store_t* store,
                                       size_t* total_raw_bytes,
                                       size_t* total_compressed_bytes,
                                       size_t* evidence_count,
                                       size_t* oldest_evidence_age_days,
                                       size_t* newest_evidence_age_days) {
    if (!store) return -1;
    
    if (total_raw_bytes) *total_raw_bytes = 0;
    if (total_compressed_bytes) *total_compressed_bytes = 0;
    if (evidence_count) *evidence_count = store->count;
    if (oldest_evidence_age_days) *oldest_evidence_age_days = 0;
    if (newest_evidence_age_days) *newest_evidence_age_days = 0;
    
    time_t now = time(NULL);
    time_t oldest = 0, newest = 0;
    bool first = true;
    
    for (size_t i = 0; i < store->count; i++) {
        const forensic_evidence_metadata_t* meta = &store->index[i];
        
        if (total_raw_bytes) *total_raw_bytes += meta->raw_size;
        if (total_compressed_bytes && meta->compressed) *total_compressed_bytes += meta->raw_size;
        
        time_t ts = parse_timestamp(meta->timestamp);
        if (first || ts < oldest) oldest = ts;
        if (first || ts > newest) newest = ts;
        first = false;
    }
    
    if (oldest_evidence_age_days && oldest > 0) {
        *oldest_evidence_age_days = difftime(now, oldest) / 86400.0;
    }
    if (newest_evidence_age_days && newest > 0) {
        *newest_evidence_age_days = difftime(now, newest) / 86400.0;
}
    
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