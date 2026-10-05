#include "../../forensic_types.h"
/**
 * EVTX Collector Plugin
 * 
 * Collects Windows Event Log (.evtx) files from standard locations.
 * On Windows: C:\Windows\System32\winevt\Logs\*.evtx
 * On Linux: mounted Windows volumes or copied EVTX files
 */

#include "../../forensic_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _WIN32
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#endif

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

#ifndef _WIN32
static void collect_evtx_files(const char* dir_path, forensic_artifact_list_t* list) {
    DIR* dir = opendir(dir_path);
    if (!dir) return;
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type != DT_REG) continue;
        
        // Check for .evtx extension
        const char* ext = strrchr(entry->d_name, '.');
        if (!ext || strcasecmp(ext, ".evtx") != 0) continue;
        
        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dir_path, entry->d_name);
        
        struct stat st;
        if (stat(fullpath, &st) != 0) continue;
        
        // Read first 4KB to check EVTX signature
        FILE* f = fopen(fullpath, "rb");
        if (!f) continue;
        
        char header[8];
        size_t read = fread(header, 1, 8, f);
        fclose(f);
        
        // EVTX signature: "ElfFile" (0x45 0x6C 0x66 0x46 0x69 0x6C 0x65 0x00)
        bool is_evtx = (read >= 8 && 
            header[0] == 'E' && header[1] == 'l' && header[2] == 'f' &&
            header[3] == 'F' && header[4] == 'i' && header[5] == 'l' &&
            header[6] == 'e');
        
        if (!is_evtx) continue;
        
        forensic_metadata_t meta = forensic_metadata_create(16);
        forensic_metadata_add(&meta, "log_name", entry->d_name);
        forensic_metadata_add(&meta, "log_path", fullpath);
        
        char size_str[32];
        snprintf(size_str, sizeof(size_str), "%ld", st.st_size);
        forensic_metadata_add(&meta, "size", size_str);
        
        char mtime_str[32];
        strftime(mtime_str, sizeof(mtime_str), "%Y-%m-%dT%H:%M:%SZ", gmtime(&st.st_mtime));
        forensic_metadata_add(&meta, "mtime", mtime_str);
        
        forensic_artifact_t artifact = {0};
        artifact.plugin_name = strdup("evtx_collector");
        artifact.artifact_type = strdup("evtx");
        artifact.timestamp = strdup(mtime_str);
        
        // Read first chunk for raw data
        f = fopen(fullpath, "rb");
        if (f) {
            char* chunk = malloc(8192);
            size_t r = fread(chunk, 1, 8192, f);
            artifact.raw = forensic_bytes_create(chunk, r);
            free(chunk);
            fclose(f);
        }
        
        artifact.metadata = meta;
        forensic_artifact_list_add(list, &artifact);
    }
    
    closedir(dir);
}

/* ============================================================================
 * EVTX Collector Implementation
 * ============================================================================ */
#endif /* !_WIN32 - end of collect_evtx_files */

static int evtx_collector_init(void* config) {
    (void)config;
    printf("[evtx_collector] Initialized\n");
    return 0;
}

#ifndef _WIN32
static forensic_artifact_list_t* evtx_collector_collect(const char* target, void* config) {
    (void)target;
    (void)config;
    
    forensic_artifact_list_t* list = forensic_artifact_list_create(64);
    if (!list) return NULL;
    
    // Windows standard EVTX locations
    const char* win_paths[] = {
        "C:/Windows/System32/winevt/Logs",
        "C:/Windows/System32/winevt/Logs/",
        "/mnt/c/Windows/System32/winevt/Logs",
        "/media/*/Windows/System32/winevt/Logs",
        "/Volumes/*/Windows/System32/winevt/Logs",
        "./evtx_logs",
        NULL
    };
    
    for (int i = 0; win_paths[i]; i++) {
        // Handle wildcards for mounted volumes
        if (strchr(win_paths[i], '*')) {
            // Skip wildcard paths for now - would need glob
            continue;
        }
        collect_evtx_files(win_paths[i], list);
    }
    
    // Also check current directory for evtx_logs
    collect_evtx_files("./evtx_logs", list);
    
    printf("[evtx_collector] Collected %zu EVTX log files\n", list->count);
    return list;
}
#endif

static void evtx_collector_cleanup(void) {
    printf("[evtx_collector] Cleanup\n");
}

/* ============================================================================
 * Plugin Definition
 * ============================================================================ */

static const char* evtx_capabilities[] = {
    "read_filesystem",
    "enumerate_files",
    "access_temp_dirs"
};

forensic_data_source_plugin_t evtx_collector_plugin = {
    .name = "evtx_collector",
    .version = "1.0.0",
    .description = "Collects Windows Event Log (.evtx) files from standard locations",
    .init = evtx_collector_init,
#ifndef _WIN32
    .collect = evtx_collector_collect,
#endif
    .cleanup = evtx_collector_cleanup,
    .required_capabilities = evtx_capabilities,
    .capability_count = sizeof(evtx_capabilities) / sizeof(evtx_capabilities[0])
};