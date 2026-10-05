#include "../../forensic_types.h"
/**
 * Memory Collector Plugin
 * 
 * Collects process memory regions, loaded DLLs/shared libraries, heap info.
 * On Linux: reads /proc/<pid>/maps, /proc/<pid>/smaps, /proc/<pid>/mem
 */

#include "../../forensic_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifndef _WIN32
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#endif

/* ============================================================================
 * Memory Collector Implementation
 * ============================================================================ */

static int memory_collector_init(void* config) {
    (void)config;
    printf("[memory_collector] Initialized\n");
    return 0;
}

#ifndef _WIN32
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
    int region_count = 0;
    const int MAX_REGIONS_PER_PROCESS = 500; // Limit per process
    
    while (fgets(line, sizeof(line), f) && region_count < MAX_REGIONS_PER_PROCESS) {
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
        
        // Skip anonymous mappings without path if desired
        // (Keep for now - they can be suspicious: heap, stack, RWX)
        
        char start_addr[32], end_addr[32];
        snprintf(start_addr, sizeof(start_addr), "%lx", start);
        snprintf(end_addr, sizeof(end_addr), "%lx", end);
        
        forensic_artifact_t* artifact = create_memory_artifact(
            pid, start_addr, end_addr, perms, pathname[0] ? pathname : NULL
        );
        if (artifact) {
            forensic_artifact_list_add(list, artifact);
            region_count++;
        }
    }
    fclose(f);
}

static bool should_skip_process(const char* comm) {
    // Extended skip list for kernel and system processes
    static const char* skip_patterns[] = {
        // Kernel threads
        "kthreadd", "ksoftirqd", "kworker", "migration", "rcu",
        "kcompactd", "kswapd", "khugepaged", "kintegrityd", "kblockd",
        "kmdflush", "kdevtmpfs", "kpsmoused", "kdmflush", "kioctx",
        "kseriod", "khungtaskd", "kfreezer", "kclock", "ktimersoftd",
        "watchdog", "bioset", "crypto", "kstriped", "kmpathd", "kdmremove",
        "kvmstat", "kvmpt", "kvmio", "kvmclock", "kvm-irqfd", "kvm-vcpu",
        
        // Init/system processes
        "systemd", "systemd-journal", "systemd-udevd", "systemd-logind",
        "systemd-resolved", "systemd-timesyncd", "systemd-networkd",
        "systemd-machined", "systemd-coredump", "systemd-tmpfiles",
        "init", "runit", "runsvdir", "runsv", "svlogd", "s6-supervise",
        
        // Shells (usually not interesting for memory forensics)
        "bash", "sh", "zsh", "fish", "dash", "ash", "tcsh", "csh",
        
        // SSH/login (usually not interesting)
        "sshd", "login", "agetty", "mingetty", "systemd-logind",
        
        // Container/virtualization
        "containerd", "dockerd", "containerd-shim", "runc", "crio",
        
        // Monitoring/telemetry (usually noise)
        "telegraf", "collectd", "prometheus", "node_exporter",
        "datadog", "newrelic", "instana", "elastic",
        
        NULL
    };
    
    for (int i = 0; skip_patterns[i]; i++) {
        if (strstr(comm, skip_patterns[i])) {
            return true;
        }
    }
    return false;
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
    
    if (should_skip_process(comm)) return;
    
    // Also skip kernel threads (no cmdline)
    char cmdline_path[256];
    snprintf(cmdline_path, sizeof(cmdline_path), "/proc/%d/cmdline", pid);
    FILE* cf = fopen(cmdline_path, "r");
    if (!cf) return; // Kernel thread - no cmdline
    fclose(cf);
    
    parse_proc_maps(pid, list);
}
#endif

static forensic_artifact_list_t* memory_collector_collect(const char* target, void* config) {
    (void)target;
    (void)config;
    
    forensic_artifact_list_t* list = forensic_artifact_list_create(256);
    if (!list) return NULL;
    
#ifndef _WIN32
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
#endif
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