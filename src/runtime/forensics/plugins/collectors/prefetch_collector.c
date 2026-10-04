#include "../../forensic_types.h"
/**
 * Prefetch Collector Plugin
 * 
 * Collects Windows Prefetch (.pf) files from standard locations.
 * On Windows: C:\Windows\Prefetch\*.pf
 * On Linux: mounted Windows volumes or copied prefetch files
 */

#include "../../forensic_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

/* ============================================================================
 * Helper Functions
 * ============================================================================ */

static void collect_prefetch_files(const char* dir_path, forensic_artifact_list_t* list) {
    DIR* dir = opendir(dir_path);
    if (!dir) return;
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type != DT_REG) continue;
        
        // Check for .pf extension
        const char* ext = strrchr(entry->d_name, '.');
        if (!ext || strcasecmp(ext, ".pf") != 0) continue;
        
        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dir_path, entry->d_name);
        
        struct stat st;
        if (stat(fullpath, &st) != 0) continue;
        
        // Read first 8 bytes to check Prefetch signature
        FILE* f = fopen(fullpath, "rb");
        if (!f) continue;
        
        char header[8];
        size_t read = fread(header, 1, 8, f);
        fclose(f);
        
        // Prefetch signature: "MAM" (0x4D 0x41 0x4D) or "SCCA" for newer formats
        bool is_prefetch = (read >= 4 && 
            ((header[0] == 'M' && header[1] == 'A' && header[2] == 'M') ||
             (header[0] == 'S' && header[1] == 'C' && header[2] == 'C' && header[3] == 'A')));
        
        if (!is_prefetch) continue;
        
        forensic_metadata_t meta = forensic_metadata_create(16);
        forensic_metadata_add(&meta, "prefetch_name", entry->d_name);
        forensic_metadata_add(&meta, "prefetch_path", fullpath);
        
        char size_str[32];
        snprintf(size_str, sizeof(size_str), "%ld", st.st_size);
        forensic_metadata_add(&meta, "size", size_str);
        
        char mtime_str[32];
        strftime(mtime_str, sizeof(mtime_str), "%Y-%m-%dT%H:%M:%SZ", gmtime(&st.st_mtime));
        forensic_metadata_add(&meta, "mtime", mtime_str);
        
        // Extract executable name from prefetch filename
        // Format: APPNAME-HASH.pf
        char* dash = strrchr(entry->d_name, '-');
        if (dash) {
            *dash = '\0';
            forensic_metadata_add(&meta, "executable", entry->d_name);
            *dash = '-';
        }
        
        forensic_artifact_t artifact = {0};
        artifact.plugin_name = strdup("prefetch_collector");
        artifact.artifact_type = strdup("prefetch");
        artifact.timestamp = strdup(mtime_str);
        
        // Read first chunk for raw data
        f = fopen(fullpath, "rb");
        if (f) {
            char* chunk = malloc(4096);
            size_t r = fread(chunk, 1, 4096, f);
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
 * Prefetch Collector Implementation
 * ============================================================================ */

static int prefetch_collector_init(void* config) {
    (void)config;
    printf("[prefetch_collector] Initialized\n");
    return 0;
}

static forensic_artifact_list_t* prefetch_collector_collect(const char* target, void* config) {
    (void)target;
    (void)config;
    
    forensic_artifact_list_t* list = forensic_artifact_list_create(64);
    if (!list) return NULL;
    
    // Windows standard Prefetch locations
    const char* win_paths[] = {
        "C:/Windows/Prefetch",
        "C:/Windows/Prefetch/",
        "/mnt/c/Windows/Prefetch",
        "/media/*/Windows/Prefetch",
        "/Volumes/*/Windows/Prefetch",
        "./prefetch_logs",
        NULL
    };
    
    for (int i = 0; win_paths[i]; i++) {
        // Handle wildcards for mounted volumes
        if (strchr(win_paths[i], '*')) {
            continue;
        }
        collect_prefetch_files(win_paths[i], list);
    }
    
    // Also check current directory
    collect_prefetch_files("./prefetch_logs", list);
    
    printf("[prefetch_collector] Collected %zu Prefetch files\n", list->count);
    return list;
}

static void prefetch_collector_cleanup(void) {
    printf("[prefetch_collector] Cleanup\n");
}

/* ============================================================================
 * Plugin Definition
 * ============================================================================ */

static const char* prefetch_capabilities[] = {
    "read_filesystem",
    "enumerate_files",
    "access_temp_dirs"
};

forensic_data_source_plugin_t prefetch_collector_plugin = {
    .name = "prefetch_collector",
    .version = "1.0.0",
    .description = "Collects Windows Prefetch (.pf) files from standard locations",
    .init = prefetch_collector_init,
    .collect = prefetch_collector_collect,
    .cleanup = prefetch_collector_cleanup,
    .required_capabilities = prefetch_capabilities,
    .capability_count = sizeof(prefetch_capabilities) / sizeof(prefetch_capabilities[0])
};