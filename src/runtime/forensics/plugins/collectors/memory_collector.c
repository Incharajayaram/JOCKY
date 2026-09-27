#include "forensic_types.h"
/**
 * Memory Collector Plugin
 * 
 * Collects process memory regions, loaded DLLs/shared libraries, heap info.
 * On Linux: reads /proc/<pid>/maps, /proc/<pid>/smaps, /proc/<pid>/mem
 */

#include "forensic_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

/* ============================================================================
 * Memory Collector Implementation
 * ============================================================================ */

static int memory_collector_init(void* config) {
    (void)config;
    printf("[memory_collector] Initialized\n");
    return 0;
}

static forensic_artifact_t* create_memory_artifact(pid_t pid,
                                                     const char* start_addr,
                                                     const char* end_addr,
                                                     const char* perms,
                                                     const char* path) {
    forensic_artifact_t* artifact = calloc(1, sizeof(forensic_artifact_t));
    if (!artifact) return NULL;
    
    artifact->plugin_name = strdup("memory_collector");
    artifact->artifact_type = strdup("memory_region");
    
    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
    artifact->timestamp = strdup(timestamp);
    
    char raw[1024];
    snprintf(raw, sizeof(raw), "PID %d: %s-%s %s %s",
             pid, start_addr, end_addr, perms, path ? path : "[anon]");
    artifact->raw = forensic_bytes_create(raw, strlen(raw));
    
    forensic_metadata_t meta = forensic_metadata_create(16);
    char pid_str[32];
    snprintf(pid_str, sizeof(pid_str), "%d", pid);
    forensic_metadata_add(&meta, "pid", pid_str);
    forensic_metadata_add(&meta, "start_addr", start_addr);
    forensic_metadata_add(&meta, "end_addr", end_addr);
    forensic_metadata_add(&meta, "permissions", perms);
    if (path) forensic_metadata_add(&meta, "path", path);
    
    if (strstr(perms, "x") && strstr(perms, "w")) {
        forensic_metadata_add(&meta, "suspicious", "RWX_region");
    }
    artifact->metadata = meta;
    
    return artifact;
}

static void parse_proc_maps(pid_t pid, forensic_artifact_list_t* list) {
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/maps", pid);
    
    FILE* f = fopen(path, "r");
    if (!f) return;
    
    char line[1024];
    while (fgets(line, sizeof(line), f)) {
        unsigned long start, end;
        char perms[16];
        unsigned long offset;
        char dev[16];
        unsigned long inode;
        char pathname[512] = {0};
        
        if (sscanf(line, "%lx-%lx %15s %lx %15s %lu %511[^\n]",
                   &start, &end, perms, &offset, dev, &inode, pathname) < 6) {
            continue;
        }
        
        char start_addr[32], end_addr[32];
        snprintf(start_addr, sizeof(start_addr), "%lx", start);
        snprintf(end_addr, sizeof(end_addr), "%lx", end);
        
        forensic_artifact_t* artifact = create_memory_artifact(
            pid, start_addr, end_addr, perms, pathname[0] ? pathname : NULL
        );
        if (artifact) {
            forensic_artifact_list_add(list, artifact);
        }
    }
    fclose(f);
}

static void collect_process_memory(pid_t pid, forensic_artifact_list_t* list) {
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/comm", pid);
    FILE* f = fopen(path, "r");
    if (!f) return;
    
    char comm[256];
    if (!fgets(comm, sizeof(comm), f)) {
        fclose(f);
        return;
    }
    fclose(f);
    comm[strcspn(comm, "\n")] = '\0';
    
    const char* skip[] = {
        "kthreadd", "ksoftirqd", "kworker", "migration", "rcu",
        "systemd", "init", "bash", "sh", "sshd", "login",
        NULL
    };
    bool should_skip = false;
    for (int i = 0; skip[i]; i++) {
        if (strstr(comm, skip[i])) {
            should_skip = true;
            break;
        }
    }
    if (should_skip) return;
    
    parse_proc_maps(pid, list);
}

static forensic_artifact_list_t* memory_collector_collect(const char* target, void* config) {
    (void)target;
    (void)config;
    
    forensic_artifact_list_t* list = forensic_artifact_list_create(256);
    if (!list) return NULL;
    
    DIR* dir = opendir("/proc");
    if (!dir) return list;
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type != DT_DIR) continue;
        
        char* endptr;
        long pid = strtol(entry->d_name, &endptr, 10);
        if (*endptr != '\0' || pid <= 0) continue;
        
        collect_process_memory(pid, list);
    }
    
    closedir(dir);
    printf("[memory_collector] Collected %zu memory regions\n", list->count);
    return list;
}

static void memory_collector_cleanup(void) {
    printf("[memory_collector] Cleanup\n");
}

/* ============================================================================
 * Plugin Definition
 * ============================================================================ */

static const char* memory_capabilities[] = {
    "read_process_memory",
    "enumerate_memory_regions",
    "access_procfs"
};

forensic_data_source_plugin_t memory_collector_plugin = {
    .name = "memory_collector",
    .version = "1.0.0",
    .description = "Collects process memory regions, permissions, and loaded modules from /proc",
    .init = memory_collector_init,
    .collect = memory_collector_collect,
    .cleanup = memory_collector_cleanup,
    .required_capabilities = memory_capabilities,
    .capability_count = sizeof(memory_capabilities) / sizeof(memory_capabilities[0])
};