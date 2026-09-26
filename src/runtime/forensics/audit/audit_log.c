#include "forensic_types.h"
#include "forensic_types.h"
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
 * Audit Log
 * ============================================================================ */

forensic_audit_log_t* forensic_audit_log_open(const char* path) {
    if (!path) return NULL;
    
    forensic_audit_log_t* log = calloc(1, sizeof(forensic_audit_log_t));
    if (!log) return NULL;
    
    strncpy(log->path, path, sizeof(log->path) - 1);
    
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
        // Parse JSON line
        // Simplified - in production use proper JSON parser
        if (strstr(line, "\"index\"")) {
            has_entries = true;
            // Extract index and entry_hash (simplified)
            sscanf(line, "{\"index\":%lu,\"entry_hash\":\"%127[^\"]",
                   &last_entry.index, last_entry.entry_hash);
        }
    }
    
    if (has_entries) {
        log->next_index = last_entry.index + 1;
        strncpy(log->last_hash, last_entry.entry_hash, sizeof(log->last_hash) - 1);
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
    strncpy(entry.action, action, sizeof(entry.action) - 1);
    strncpy(entry.input_hash, input_hash ? input_hash : "", sizeof(entry.input_hash) - 1);
    strncpy(entry.output_hash, output_hash ? output_hash : "", sizeof(entry.output_hash) - 1);
    strncpy(entry.prev_entry_hash, log->last_hash, sizeof(entry.prev_entry_hash) - 1);
    
    // Compute entry hash
    forensic_audit_compute_entry_hash(&entry, entry.entry_hash, sizeof(entry.entry_hash));
    
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
        
        // Simplified JSON parsing
        char* idx = strstr(line, "\"index\":");
        if (idx) entry.index = strtoul(idx + 8, NULL, 10);
        
        char* ts = strstr(line, "\"timestamp\":\"");
        if (ts) {
            ts += 13;
            char* end = strchr(ts, '"');
            if (end && (size_t)(end - ts) < sizeof(entry.timestamp)) {
                strncpy(entry.timestamp, ts, end - ts);
            }
        }
        
        char* act = strstr(line, "\"actor\":\"");
        if (act) {
            act += 9;
            char* end = strchr(act, '"');
            if (end && (size_t)(end - act) < sizeof(entry.actor)) {
                strncpy(entry.actor, act, end - act);
            }
        }
        
        char* action = strstr(line, "\"action\":\"");
        if (action) {
            action += 10;
            char* end = strchr(action, '"');
            if (end && (size_t)(end - action) < sizeof(entry.action)) {
                strncpy(entry.action, action, end - action);
            }
        }
        
        char* in_hash = strstr(line, "\"input_hash\":\"");
        if (in_hash) {
            in_hash += 14;
            char* end = strchr(in_hash, '"');
            if (end && (size_t)(end - in_hash) < sizeof(entry.input_hash)) {
                strncpy(entry.input_hash, in_hash, end - in_hash);
            }
        }
        
        char* out_hash = strstr(line, "\"output_hash\":\"");
        if (out_hash) {
            out_hash += 15;
            char* end = strchr(out_hash, '"');
            if (end && (size_t)(end - out_hash) < sizeof(entry.output_hash)) {
                strncpy(entry.output_hash, out_hash, end - out_hash);
            }
        }
        
        char* prev_hash = strstr(line, "\"prev_entry_hash\":\"");
        if (prev_hash) {
            prev_hash += 19;
            char* end = strchr(prev_hash, '"');
            if (end && (size_t)(end - prev_hash) < sizeof(entry.prev_entry_hash)) {
                strncpy(entry.prev_entry_hash, prev_hash, end - prev_hash);
            }
        }
        
        char* entry_hash = strstr(line, "\"entry_hash\":\"");
        if (entry_hash) {
            entry_hash += 14;
            char* end = strchr(entry_hash, '"');
            if (end && (size_t)(end - entry_hash) < sizeof(entry.entry_hash)) {
                strncpy(entry.entry_hash, entry_hash, end - entry_hash);
            }
        }
        
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
            fprintf(stderr, "[audit_log] Line %zu: entry_hash mismatch\n", line_num);
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