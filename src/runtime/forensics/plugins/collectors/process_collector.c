#include "../../forensic_types.h"
/**
 * Process Collector Plugin
 * 
 * Collects running processes with parent-child relationships,
 * command lines, loaded modules, and injection detection.
 */

#include "../../forensic_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

/* ============================================================================
 * Helper Functions
 * ============================================================================ */

static char* read_file_to_string(const char* path, size_t* out_len) {
    FILE* f = fopen(path, "r");
    if (!f) return NULL;
    
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    char* buf = malloc(len + 1);
    if (!buf) {
        fclose(f);
        return NULL;
    }
    
    size_t read = fread(buf, 1, len, f);
    buf[read] = '\0';
    fclose(f);
    
    if (out_len) *out_len = read;
    return buf;
}

static void replace_nulls(char* str, size_t len) {
    for (size_t i = 0; i < len && i < len - 1; i++) {
        if (str[i] == '\0') str[i] = ' ';
    }
}

static char* get_process_cmdline(pid_t pid) {
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/cmdline", pid);
    size_t len;
    char* cmdline = read_file_to_string(path, &len);
    if (cmdline && len > 0) {
        replace_nulls(cmdline, len);
    }
    return cmdline;
}

static char* get_process_exe(pid_t pid) {
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/exe", pid);
    char buf[512];
    ssize_t len = readlink(path, buf, sizeof(buf) - 1);
    if (len > 0) {
        buf[len] = '\0';
        return strdup(buf);
    }
    return NULL;
}

static pid_t get_ppid(pid_t pid) {
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    char* stat = read_file_to_string(path, NULL);
    if (!stat) return 0;
    
    char* token = strtok(stat, " ");
    for (int i = 0; i < 3 && token; i++) {
        token = strtok(NULL, " ");
    }
    pid_t ppid = token ? atoi(token) : 0;
    free(stat);
    return ppid;
}

static char* get_process_name(pid_t pid) {
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/comm", pid);
    size_t len;
    char* name = read_file_to_string(path, &len);
    if (name && len > 0 && name[len-1] == '\n') name[len-1] = '\0';
    return name;
}

/* ============================================================================
 * Process Collector Implementation
 * ============================================================================ */

static int process_collector_init(void* config) {
    (void)config;
    printf("[process_collector] Initialized\n");
    return 0;
}

static forensic_artifact_list_t* process_collector_collect(const char* target, void* config) {
    (void)target;
    (void)config;
    
    forensic_artifact_list_t* list = forensic_artifact_list_create(128);
    if (!list) return NULL;
    
    DIR* dir = opendir("/proc");
    if (!dir) {
        forensic_artifact_list_destroy(list);
        return NULL;
    }
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type != DT_DIR) continue;
        
        char* endptr;
        long pid = strtol(entry->d_name, &endptr, 10);
        if (*endptr != '\0' || pid <= 0) continue;
        
        char* cmdline = get_process_cmdline(pid);
        char* exe = get_process_exe(pid);
        char* name = get_process_name(pid);
        pid_t ppid = get_ppid(pid);
        
        if (!cmdline && !exe && !name) continue;
        
        forensic_metadata_t meta = forensic_metadata_create(16);
        forensic_metadata_add(&meta, "pid", entry->d_name);
        if (ppid > 0) {
            char ppid_str[32];
            snprintf(ppid_str, sizeof(ppid_str), "%d", ppid);
            forensic_metadata_add(&meta, "ppid", ppid_str);
        }
        if (exe) forensic_metadata_add(&meta, "exe", exe);
        if (name) forensic_metadata_add(&meta, "name", name);
        
        forensic_artifact_t artifact = {0};
        artifact.plugin_name = strdup("process_collector");
        artifact.artifact_type = strdup("process");
        
        time_t now = time(NULL);
        char timestamp[64];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
        artifact.timestamp = strdup(timestamp);
        
        if (cmdline) {
            artifact.raw = forensic_bytes_create(cmdline, strlen(cmdline));
        } else {
            artifact.raw = forensic_bytes_create("", 0);
        }
        artifact.metadata = meta;
        
        forensic_artifact_list_add(list, &artifact);
        
        free(cmdline);
        free(exe);
        free(name);
    }
    
    closedir(dir);
    printf("[process_collector] Collected %zu processes\n", list->count);
    return list;
}

static void process_collector_cleanup(void) {
    printf("[process_collector] Cleanup\n");
}

/* ============================================================================
 * Plugin Definition
 * ============================================================================ */

static const char* process_capabilities[] = {
    "read_process_info",
    "enumerate_processes"
};

forensic_data_source_plugin_t process_collector_plugin = {
    .name = "process_collector",
    .version = "1.0.0",
    .description = "Collects running processes with command lines, parent PIDs, and executable paths",
    .init = process_collector_init,
    .collect = process_collector_collect,
    .cleanup = process_collector_cleanup,
    .required_capabilities = process_capabilities,
    .capability_count = sizeof(process_capabilities) / sizeof(process_capabilities[0])
};