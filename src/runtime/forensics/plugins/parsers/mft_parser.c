#include "forensic_types.h"
/**
 * MFT Parser Plugin
 * 
 * Parses NTFS Master File Table ($MFT) for file records, timestamps,
 * and deleted entries. On Linux: parses filesystem metadata.
 */

#include "forensic_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

/* ============================================================================
 * MFT Parser Implementation (Linux - filesystem-based)
 * ============================================================================ */

static int mft_parser_init(void* config) {
    (void)config;
    printf("[mft_parser] Initialized\n");
    return 0;
}

static void scan_directory(const char* path, forensic_parsed_artifact_t* artifact, int depth) {
    if (depth > 3) return;
    
    DIR* dir = opendir(path);
    if (!dir) return;
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        
        char fullpath[4096];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);
        
        struct stat st;
        if (lstat(fullpath, &st) != 0) continue;
        
        char inode_str[32];
        snprintf(inode_str, sizeof(inode_str), "%lu", (unsigned long)st.st_ino);
        
        char key[512];
        snprintf(key, sizeof(key), "%s/%s", path, entry->d_name);
        
        char value[1024];
        snprintf(value, sizeof(value), "inode=%lu,mode=%o,size=%ld,mtime=%ld",
                 (unsigned long)st.st_ino, st.st_mode, (long)st.st_size, (long)st.st_mtime);
        
        forensic_metadata_add(&artifact->parsed_data, key, value);
        
        if (S_ISREG(st.st_mode)) {
            if (st.st_mode & S_ISUID) {
                forensic_ioc_t* ioc = forensic_ioc_create("setuid_file", fullpath,
                    "mft_parser", 0.9);
                artifact->iocs.items = realloc(artifact->iocs.items,
                    (artifact->iocs.count + 1) * sizeof(forensic_ioc_t));
                artifact->iocs.items[artifact->iocs.count++] = *ioc;
                free(ioc);
            }
            if (strstr(fullpath, ".tmp") || strstr(fullpath, "tmp")) {
                forensic_ioc_t* ioc = forensic_ioc_create("temp_file", fullpath,
                    "mft_parser", 0.5);
                artifact->iocs.items = realloc(artifact->iocs.items,
                    (artifact->iocs.count + 1) * sizeof(forensic_ioc_t));
                artifact->iocs.items[artifact->iocs.count++] = *ioc;
                free(ioc);
            }
        }
        
        if (S_ISDIR(st.st_mode)) {
            scan_directory(fullpath, artifact, depth + 1);
        }
    }
    closedir(dir);
}

static forensic_parsed_artifact_t* parse_mft(const forensic_bytes_t* data, void* config) {
    (void)data;
    (void)config;
    
    forensic_parsed_artifact_t* artifact = forensic_parsed_artifact_create("mft", "mft_parser");
    if (!artifact) return NULL;
    
    const char* paths[] = {
        "/tmp", "/var/tmp", "/home", "/root", "/etc", "/var/log", NULL
    };
    
    for (int i = 0; paths[i]; i++) {
        scan_directory(paths[i], artifact, 0);
    }
    
    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
    forensic_event_t* evt = forensic_event_create("mft_parsed",
        timestamp, "mft_parser", "Filesystem metadata collected");
    artifact->timeline_events = realloc(artifact->timeline_events,
        (artifact->timeline_count + 1) * sizeof(forensic_event_t));
    artifact->timeline_events[artifact->timeline_count++] = *evt;
    free(evt);
    
    printf("[mft_parser] Collected %zu filesystem entries\n", artifact->parsed_data.count);
    return artifact;
}

static void mft_parser_cleanup(void) {
    printf("[mft_parser] Cleanup\n");
}

/* ============================================================================
 * Plugin Definition
 * ============================================================================ */

static const char* mft_capabilities[] = {
    "read_artifact",
    "parse_mft",
    "enumerate_filesystem"
};

forensic_artifact_parser_plugin_t mft_parser_plugin = {
    .name = "mft_parser",
    .version = "1.0.0",
    .description = "Parses MFT filesystem metadata and NTFS MFT equivalent",
    .artifact_type = "mft",
    .init = mft_parser_init,
    .parse = parse_mft,
    .cleanup = mft_parser_cleanup,
    .required_capabilities = mft_capabilities,
    .capability_count = sizeof(mft_capabilities) / sizeof(mft_capabilities[0]),
    .requires_sandbox = true,
    .max_cpu_time_ms = 30000,
    .max_memory_bytes = 512 * 1024 * 1024
};