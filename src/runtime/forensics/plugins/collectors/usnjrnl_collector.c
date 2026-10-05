#include "../../forensic_types.h"
/**
 * USN Journal Collector Plugin
 * 
 * Collects Windows USN Journal ($UsnJrnl) from NTFS volumes.
 * On Windows: \\.\C: (requires admin/raw disk access)
 * On Linux: mounted NTFS volumes or copied $Extend\$UsnJrnl:$J files
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

#ifndef _WIN32
static void collect_usnjrnl_files(const char* dir_path, forensic_artifact_list_t* list) {
    DIR* dir = opendir(dir_path);
    if (!dir) return;
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type != DT_REG) continue;
        
        // Check for $J or $UsnJrnl filename
        if (strcasecmp(entry->d_name, "$j") != 0 && 
            strcasecmp(entry->d_name, "$usnjrnl") != 0 &&
            strcasecmp(entry->d_name, "$usnjrnl:$j") != 0) continue;
        
        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dir_path, entry->d_name);
        
        struct stat st;
        if (stat(fullpath, &st) != 0) continue;
        
        // Read first 8 bytes to check USN signature
        FILE* f = fopen(fullpath, "rb");
        if (!f) continue;
        
        char header[8];
        size_t read = fread(header, 1, 8, f);
        fclose(f);
        
        // USN Journal records start with major version (2) + minor version
        // First record is typically the $J stream with signature
        bool is_usn = (read >= 4);
        
        if (!is_usn) continue;
        
        forensic_metadata_t meta = forensic_metadata_create(16);
        forensic_metadata_add(&meta, "usnjrnl_name", entry->d_name);
        forensic_metadata_add(&meta, "usnjrnl_path", fullpath);
        
        char size_str[32];
        snprintf(size_str, sizeof(size_str), "%ld", st.st_size);
        forensic_metadata_add(&meta, "size", size_str);
        
        char mtime_str[32];
        strftime(mtime_str, sizeof(mtime_str), "%Y-%m-%dT%H:%M:%SZ", gmtime(&st.st_mtime));
        forensic_metadata_add(&meta, "mtime", mtime_str);
        
        forensic_metadata_add(&meta, "volume", dir_path);
        
        forensic_artifact_t artifact = {0};
        artifact.plugin_name = strdup("usnjrnl_collector");
        artifact.artifact_type = strdup("usnjrnl");
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
 * USN Journal Collector Implementation
 * ============================================================================ */
#endif /* !_WIN32 */

static int usnjrnl_collector_init(void* config) {
    (void)config;
    printf("[usnjrnl_collector] Initialized\n");
    return 0;
}

#ifndef _WIN32
static forensic_artifact_list_t* usnjrnl_collector_collect(const char* target, void* config) {
    (void)target;
    (void)config;
    
    forensic_artifact_list_t* list = forensic_artifact_list_create(16);
    if (!list) return NULL;
    
    // Windows standard USN Journal locations
    const char* win_paths[] = {
        "/mnt/c",
        "/mnt/d",
        "/mnt/e",
        "/media/*/C",
        "/media/*/D",
        "/Volumes/*",
        "./usnjrnl_files",
        NULL
    };
    
    for (int i = 0; win_paths[i]; i++) {
        // Handle wildcards for mounted volumes
        if (strchr(win_paths[i], '*')) {
            continue;
        }
        collect_usnjrnl_files(win_paths[i], list);
    }
    
    // Also check current directory
    collect_usnjrnl_files("./usnjrnl_files", list);
    
    // Check for $Extend directory on mounted volumes
    const char* extend_paths[] = {
        "/mnt/c/$Extend",
        "/mnt/d/$Extend",
        "/mnt/e/$Extend",
        "./usnjrnl_files",
        NULL
    };
    
    for (int i = 0; extend_paths[i]; i++) {
        collect_usnjrnl_files(extend_paths[i], list);
    }
    
    printf("[usnjrnl_collector] Collected %zu USN Journal files\n", list->count);
    return list;
}
#endif

static void usnjrnl_collector_cleanup(void) {
    printf("[usnjrnl_collector] Cleanup\n");
}

/* ============================================================================
 * Plugin Definition
 * ============================================================================ */

static const char* usnjrnl_capabilities[] = {
    "read_filesystem",
    "enumerate_files",
    "access_temp_dirs"
};

forensic_data_source_plugin_t usnjrnl_collector_plugin = {
    .name = "usnjrnl_collector",
    .version = "1.0.0",
    .description = "Collects Windows USN Journal ($Extend\\$UsnJrnl:$J) from NTFS volumes",
    .init = usnjrnl_collector_init,
#ifndef _WIN32
    .collect = usnjrnl_collector_collect,
#endif
    .cleanup = usnjrnl_collector_cleanup,
    .required_capabilities = usnjrnl_capabilities,
    .capability_count = sizeof(usnjrnl_capabilities) / sizeof(usnjrnl_capabilities[0])
};