#include "forensic_types.h"
/**
 * Audit Log Implementation
 */

#include "audit_log.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

/* ============================================================================
 * Hash Function
 * ============================================================================ */

static uint32_t simple_hash(const char* data, size_t len) {
    uint32_t hash = 5381;
    for (size_t i = 0; i < len; i++) {
        hash = ((hash << 5) + hash) + (unsigned char)data[i];
    }
    return hash;
}

void forensic_audit_compute_entry_hash(const forensic_audit_entry_t* entry,
                                        char* output, size_t output_size) {
    char combined[2048];
    snprintf(combined, sizeof(combined), "%lu%s%s%s%s%s%s",
             entry->index,
             entry->timestamp,
             entry->actor,
             entry->action,
             entry->input_hash,
             entry->output_hash,
             entry->prev_entry_hash);
    
    uint32_t h1 = simple_hash(combined, strlen(combined));
    uint32_t h2 = simple_hash(combined + strlen(combined)/2, strlen(combined) - strlen(combined)/2);
    snprintf(output, output_size, "%08x%08x", h1, h2);
}

/* ============================================================================
 * JSON String Extraction Helper
 * ============================================================================ */

static bool extract_json_string(const char* json, const char* key, char* output, size_t output_size) {
    // Find the key
    size_t key_len = strlen(key);
    const char* p = json;
    while ((p = strstr(p, key)) != NULL) {
        // Check if it's a JSON key (preceded by " or { or , and followed by ":")
        if (p > json) {
            char prev = *(p - 1);
            if (prev != '"' && prev != '{' && prev != ',') {
                p++;
                continue;
            }
        }
        p += key_len;
        // Find the colon
        while (*p && *p != ':') p++;
        if (!*p) continue;
        p++; // skip colon
        // Skip whitespace
        while (*p && (*p == ' ' || *p == '\t')) p++;
        // Expect opening quote
        if (*p != '"') continue;
        p++;
        // Extract value until unescaped quote
        char* out = output;
        size_t remaining = output_size - 1;
        while (*p && *p != '"' && remaining > 0) {
            if (*p == '\\' && *(p + 1)) {
                // Handle escape sequences
                p++;
                switch (*p) {
                    case '"': *out++ = '"'; break;
                    case '\\': *out++ = '\\'; break;
                    case '/': *out++ = '/'; break;
                    case 'b': *out++ = '\b'; break;
                    case 'f': *out++ = '\f'; break;
                    case 'n': *out++ = '\n'; break;
                    case 'r': *out++ = '\r'; break;
                    case 't': *out++ = '\t'; break;
                    case 'u': // Unicode - skip for now
                        p += 4;
                        continue;
                    default: *out++ = *p; break;
                }
            } else {
                *out++ = *p;
            }
            p++;
            remaining--;
        }
        *out = '\0';
        return true;
    }
    return false;
}

static bool extract_json_number(const char* json, const char* key, unsigned long* output) {
    const char* p = strstr(json, key);
    if (!p) return false;
    p += strlen(key);
    while (*p && *p != ':') p++;
    if (!*p) return false;
    p++; // skip colon
    while (*p && (*p == ' ' || *p == '\t')) p++;
    *output = strtoul(p, NULL, 10);
    return true;
}

/* ============================================================================
 * Audit Log
 * ============================================================================ */

forensic_audit_log_t* forensic_audit_log_open(const char* path) {
    if (!path) return NULL;
    
    forensic_audit_log_t* log = calloc(1, sizeof(forensic_audit_log_t));
    if (!log) return NULL;
    
    strncpy(log->path, path, sizeof(log->path) - 1);
    log->path[sizeof(log->path) - 1] = '\0';
    
    // Open in append mode
    log->file = fopen(path, "a+");
    if (!log->file) {
        free(log);
        return NULL;
    }
    
    // Read existing entries to find last hash and index
    fseek(log->file, 0, SEEK_SET);
    char line[4096];
    forensic_audit_entry_t last_entry = {0};
    bool has_entries = false;
    
    while (fgets(line, sizeof(line), log->file)) {
        // Parse JSON line - extract index and entry_hash
        unsigned long idx;
        char hash[128];
        if (extract_json_number(line, "\"index\"", &idx) &&
            extract_json_string(line, "\"entry_hash\"", hash, sizeof(hash))) {
            has_entries = true;
            last_entry.index = idx;
            strncpy(last_entry.entry_hash, hash, sizeof(last_entry.entry_hash) - 1);
            last_entry.entry_hash[sizeof(last_entry.entry_hash) - 1] = '\0';
        }
    }
    
    if (has_entries) {
        log->next_index = last_entry.index + 1;
        strncpy(log->last_hash, last_entry.entry_hash, sizeof(log->last_hash) - 1);
        log->last_hash[sizeof(log->last_hash) - 1] = '\0';
    } else {
        log->next_index = 0;
        log->last_hash[0] = '\0';
    }
    
    // Seek to end for appending
    fseek(log->file, 0, SEEK_END);
    log->initialized = true;
    
    printf("[audit_log] Opened %s (next_index=%lu)\n", path, log->next_index);
    return log;
}

void forensic_audit_log_close(forensic_audit_log_t* log) {
    if (!log) return;
    if (log->file) fclose(log->file);
    free(log);
}

int forensic_audit_log_append(forensic_audit_log_t* log,
                               const char* actor,
                               const char* action,
                               const char* input_hash,
                               const char* output_hash) {
    if (!log || !log->initialized || !actor || !action) return -1;
    
    forensic_audit_entry_t entry = {0};
    entry.index = log->next_index++;
    
    time_t now = time(NULL);
    struct tm* tm_info = gmtime(&now);
    strftime(entry.timestamp, sizeof(entry.timestamp), "%Y-%m-%dT%H:%M:%SZ", tm_info);
    
    strncpy(entry.actor, actor, sizeof(entry.actor) - 1);
    entry.actor[sizeof(entry.actor) - 1] = '\0';
    strncpy(entry.action, action, sizeof(entry.action) - 1);
    entry.action[sizeof(entry.action) - 1] = '\0';
    strncpy(entry.input_hash, input_hash ? input_hash : "", sizeof(entry.input_hash) - 1);
    entry.input_hash[sizeof(entry.input_hash) - 1] = '\0';
    strncpy(entry.output_hash, output_hash ? output_hash : "", sizeof(entry.output_hash) - 1);
    entry.output_hash[sizeof(entry.output_hash) - 1] = '\0';
    strncpy(entry.prev_entry_hash, log->last_hash, sizeof(entry.prev_entry_hash) - 1);
    entry.prev_entry_hash[sizeof(entry.prev_entry_hash) - 1] = '\0';
    
    // Compute entry hash
    forensic_audit_compute_entry_hash(&entry, entry.entry_hash, sizeof(entry.entry_hash));
    entry.entry_hash[sizeof(entry.entry_hash) - 1] = '\0';
    
    // Write as JSON line
    fprintf(log->file,
            "{\"index\":%lu,\"timestamp\":\"%s\",\"actor\":\"%s\",\"action\":\"%s\","
            "\"input_hash\":\"%s\",\"output_hash\":\"%s\","
            "\"prev_entry_hash\":\"%s\",\"entry_hash\":\"%s\"}\n",
            entry.index, entry.timestamp, entry.actor, entry.action,
            entry.input_hash, entry.output_hash,
            entry.prev_entry_hash, entry.entry_hash);
    
    fflush(log->file);
    
    // Update last hash
    strncpy(log->last_hash, entry.entry_hash, sizeof(log->last_hash) - 1);
    log->last_hash[sizeof(log->last_hash) - 1] = '\0';
    
    return 0;
}

int forensic_audit_log_verify(const forensic_audit_log_t* log) {
    if (!log || !log->initialized) return -1;
    
    FILE* f = fopen(log->path, "r");
    if (!f) return -1;
    
    char line[4096];
    forensic_audit_entry_t prev_entry = {0};
    bool first = true;
    size_t line_num = 0;
    int errors = 0;
    
    while (fgets(line, sizeof(line), f)) {
        line_num++;
        
        forensic_audit_entry_t entry = {0};
        
        // Parse JSON line using robust extraction
        extract_json_number(line, "\"index\"", &entry.index);
        extract_json_string(line, "\"timestamp\"", entry.timestamp, sizeof(entry.timestamp));
        extract_json_string(line, "\"actor\"", entry.actor, sizeof(entry.actor));
        extract_json_string(line, "\"action\"", entry.action, sizeof(entry.action));
        extract_json_string(line, "\"input_hash\"", entry.input_hash, sizeof(entry.input_hash));
        extract_json_string(line, "\"output_hash\"", entry.output_hash, sizeof(entry.output_hash));
        extract_json_string(line, "\"prev_entry_hash\"", entry.prev_entry_hash, sizeof(entry.prev_entry_hash));
        extract_json_string(line, "\"entry_hash\"", entry.entry_hash, sizeof(entry.entry_hash));
        
        // Verify prev_entry_hash matches previous entry's hash
        if (!first) {
            if (strcmp(entry.prev_entry_hash, prev_entry.entry_hash) != 0) {
                fprintf(stderr, "[audit_log] Line %zu: prev_entry_hash mismatch\n", line_num);
                errors++;
            }
        }
        
        // Verify entry_hash
        char computed_hash[128];
        forensic_audit_compute_entry_hash(&entry, computed_hash, sizeof(computed_hash));
        if (strcmp(entry.entry_hash, computed_hash) != 0) {
            fprintf(stderr, "[audit_log] Line %zu: entry_hash mismatch (stored=%s, computed=%s)\n", 
                    line_num, entry.entry_hash, computed_hash);
            errors++;
        }
        
        prev_entry = entry;
        first = false;
    }
    
    fclose(f);
    
    if (errors == 0) {
        printf("[audit_log] Verification passed: %zu entries, chain intact\n", line_num);
    } else {
        fprintf(stderr, "[audit_log] Verification FAILED: %d errors\n", errors);
    }
    
    return errors == 0 ? 0 : -1;
}

int forensic_audit_log_read_entries(const forensic_audit_log_t* log,
                                     forensic_audit_entry_t** entries,
                                     size_t* count) {
    // Would implement reading all entries
    (void)log;
    (void)entries;
    (void)count;
    return -1;
}