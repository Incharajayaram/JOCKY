#include "../../forensic_types.h"
/**
 * MFT Collector Plugin
 * 
 * Collects Windows Master File Table ($MFT) from NTFS volumes.
 * On Windows: \\.\C: (requires admin/raw disk access)
 * On Linux: mounted NTFS volumes or copied $MFT files
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
static void collect_mft_files(const char* dir_path, forensic_artifact_list_t* list) {
    DIR* dir = opendir(dir_path);
    if (!dir) return;
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type != DT_REG) continue;
        
        // Check for $MFT filename (case insensitive)
        if (strcasecmp(entry->d_name, "$mft") != 0 && 
            strcasecmp(entry->d_name, "$mftmirr") != 0) continue;
        
        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dir_path, entry->d_name);
        
        struct stat st;
        if (stat(fullpath, &st) != 0) continue;
        
        // Read first 4 bytes to check MFT signature
        FILE* f = fopen(fullpath, "rb");
        if (!f) continue;
        
        char header[4];
        size_t read = fread(header, 1, 4, f);
        fclose(f);
        
        // MFT signature: "FILE" (0x46 0x49 0x4C 0x45)
        bool is_mft = (read >= 4 && 
            header[0] == 'F' && header[1] == 'I' && header[2] == 'L' && header[3] == 'E');
        
        if (!is_mft) continue;
        
        forensic_metadata_t meta = forensic_metadata_create(16);
        forensic_metadata_add(&meta, "mft_name", entry->d_name);
        forensic_metadata_add(&meta, "mft_path", fullpath);
        
        char size_str[32];
        snprintf(size_str, sizeof(size_str), "%ld", st.st_size);
        forensic_metadata_add(&meta, "size", size_str);
        
        char mtime_str[32];
        strftime(mtime_str, sizeof(mtime_str), "%Y-%m-%dT%H:%M:%SZ", gmtime(&st.st_mtime));
        forensic_metadata_add(&meta, "mtime", mtime_str);
        
        forensic_metadata_add(&meta, "volume", dir_path);
        
        forensic_artifact_t artifact = {0};
        artifact.plugin_name = strdup("mft_collector");
        artifact.artifact_type = strdup("mft");
        artifact.timestamp = strdup(mtime_str);
        
        // Read first chunk for raw data (first record = $MFT metadata)
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
 * MFT Collector Implementation
 * ============================================================================ */
#endif /* !_WIN32 */

static int mft_collector_init(void* config) {
    (void)config;
    printf("[mft_collector] Initialized\n");
    return 0;
}

#ifndef _WIN32
static forensic_artifact_list_t* mft_collector_collect(const char* target, void* config) {
    (void)target;
    (void)config;
    
    forensic_artifact_list_t* list = forensic_artifact_list_create(16);
    if (!list) return NULL;
    
    // Windows standard MFT locations (on mounted volumes)
    const char* win_paths[] = {
        "/mnt/c",
        "/mnt/d",
        "/mnt/e",
        "/media/*/C",
        "/media/*/D",
        "/Volumes/*",
        "./mft_files",
        NULL
    };
    
    for (int i = 0; win_paths[i]; i++) {
        // Handle wildcards for mounted volumes
        if (strchr(win_paths[i], '*')) {
            continue;
        }
        collect_mft_files(win_paths[i], list);
    }
    
    // Also check current directory
    collect_mft_files("./mft_files", list);
    
    printf("[mft_collector] Collected %zu MFT files\n", list->count);
    return list;
}
#endif

static void mft_collector_cleanup(void) {
    printf("[mft_collector] Cleanup\n");
}

/* ============================================================================
 * Plugin Definition
 * ============================================================================ */

static const char* mft_capabilities[] = {
    "read_filesystem",
    "enumerate_files",
    "access_temp_dirs"
};

forensic_data_source_plugin_t mft_collector_plugin = {
    .name = "mft_collector",
    .version = "1.0.0",
    .description = "Collects Windows Master File Table ($MFT) from NTFS volumes",
    .init = mft_collector_init,
#ifndef _WIN32
    .collect = mft_collector_collect,
#endif
    .cleanup = mft_collector_cleanup,
    .required_capabilities = mft_capabilities,
    .capability_count = sizeof(mft_capabilities) / sizeof(mft_capabilities[0])
};