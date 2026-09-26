#include "forensic_types.h"
/**
 * File Collector Plugin
 * 
 * Collects recent files, executables, temp files, startup folder contents.
 */

#include "forensic_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <pwd.h>

/* ============================================================================
 * Helper Functions
 * ============================================================================ */

static bool is_executable(const char* path) {
    struct stat st;
    if (stat(path, &st) != 0) return false;
    return (st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH);
}

static char* get_file_hash(const char* path) {
    struct stat st;
    if (stat(path, &st) != 0) return NULL;
    char* hash = malloc(64);
    snprintf(hash, 64, "sha256:%lx_%lx", (unsigned long)st.st_ino, (unsigned long)st.st_size);
    return hash;
}

static void collect_files_recursive(const char* dirpath, forensic_artifact_list_t* list,
                                     time_t since, int max_depth, int current_depth) {
    if (current_depth > max_depth) return;
    
    DIR* dir = opendir(dirpath);
    if (!dir) return;
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        
        char fullpath[4096];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dirpath, entry->d_name);
        
        struct stat st;
        if (lstat(fullpath, &st) != 0) continue;
        
        if (since > 0 && st.st_mtime < since) {
            if (S_ISDIR(st.st_mode)) {
                collect_files_recursive(fullpath, list, since, max_depth, current_depth + 1);
            }
            continue;
        }
        
        if (S_ISREG(st.st_mode)) {
            bool collect = false;
            if (is_executable(fullpath)) collect = true;
            if (strstr(fullpath, ".tmp") || strstr(fullpath, ".temp")) collect = true;
            if (strstr(fullpath, "startup") || strstr(fullpath, "autostart")) collect = true;
            
            if (collect) {
                forensic_metadata_t meta = forensic_metadata_create(16);
                char size_str[32];
                snprintf(size_str, sizeof(size_str), "%ld", (long)st.st_size);
                forensic_metadata_add(&meta, "size", size_str);
                char mtime_str[32];
                snprintf(mtime_str, sizeof(mtime_str), "%ld", (long)st.st_mtime);
                forensic_metadata_add(&meta, "mtime", mtime_str);
                
                char* hash = get_file_hash(fullpath);
                if (hash) {
                    forensic_metadata_add(&meta, "hash", hash);
                    free(hash);
                }
                
                forensic_artifact_t artifact = {0};
                artifact.plugin_name = "file_collector";
                artifact.artifact_type = "file";
                
                time_t now = time(NULL);
                char timestamp[64];
                strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
                artifact.timestamp = strdup(timestamp);
                artifact.raw = forensic_bytes_create(fullpath, strlen(fullpath));
                artifact.metadata = meta;
                
                forensic_artifact_list_add(list, &artifact);
            }
        } else if (S_ISDIR(st.st_mode)) {
            collect_files_recursive(fullpath, list, since, max_depth, current_depth + 1);
        }
    }
    closedir(dir);
}

/* ============================================================================
 * File Collector Implementation
 * ============================================================================ */

static int file_collector_init(void* config) {
    (void)config;
    printf("[file_collector] Initialized\n");
    return 0;
}

static forensic_artifact_list_t* file_collector_collect(const char* target, void* config) {
    (void)target;
    (void)config;
    
    forensic_artifact_list_t* list = forensic_artifact_list_create(256);
    if (!list) return NULL;
    
    time_t since = time(NULL) - (7 * 24 * 3600);
    
    const char* paths[] = {
        "/tmp",
        "/var/tmp",
        "/home",
        "/root",
        "/etc/cron.d",
        "/etc/cron.daily",
        "/etc/cron.hourly",
        "/etc/cron.weekly",
        "/etc/cron.monthly",
        "/etc/init.d",
        "/etc/systemd/system",
        "/usr/bin",
        "/usr/sbin",
        "/bin",
        "/sbin",
        NULL
    };
    
    for (int i = 0; paths[i]; i++) {
        collect_files_recursive(paths[i], list, since, 3, 0);
    }
    
    struct passwd* pw;
    while ((pw = getpwent()) != NULL) {
        char autostart[512];
        snprintf(autostart, sizeof(autostart), "%s/.config/autostart", pw->pw_dir);
        collect_files_recursive(autostart, list, since, 2, 0);
    }
    endpwent();
    
    printf("[file_collector] Collected %zu files\n", list->count);
    return list;
}

static void file_collector_cleanup(void) {
    printf("[file_collector] Cleanup\n");
}

/* ============================================================================
 * Plugin Definition
 * ============================================================================ */

static const char* file_capabilities[] = {
    "read_filesystem",
    "enumerate_files",
    "access_temp_dirs"
};

forensic_data_source_plugin_t file_collector_plugin = {
    .name = "file_collector",
    .version = "1.0.0",
    .description = "Collects recent files, executables, temp files, and startup entries",
    .init = file_collector_init,
    .collect = file_collector_collect,
    .cleanup = file_collector_cleanup,
    .required_capabilities = file_capabilities,
    .capability_count = sizeof(file_capabilities) / sizeof(file_capabilities[0])
};