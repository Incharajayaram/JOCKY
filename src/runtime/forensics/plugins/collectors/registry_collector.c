#include "forensic_types.h"
/**
 * Registry Collector Plugin
 * 
 * On Linux: collects systemd services, cron jobs, shell configs, 
 * startup scripts, and other persistence mechanisms.
 */

#include "forensic_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>
#include <time.h>

/* ============================================================================
 * Registry Collector Implementation (Linux - config-based)
 * ============================================================================ */

static int registry_collector_init(void* config) {
    (void)config;
    printf("[registry_collector] Initialized\n");
    return 0;
}

static forensic_artifact_t* create_registry_artifact(const char* key_path,
                                                       const char* value_name,
                                                       const char* value_data,
                                                       const char* type) {
    forensic_artifact_t* artifact = calloc(1, sizeof(forensic_artifact_t));
    if (!artifact) return NULL;
    
    artifact->plugin_name = strdup("registry_collector");
    artifact->artifact_type = strdup("registry");
    
    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
    artifact->timestamp = strdup(timestamp);
    
    char raw[1024];
    snprintf(raw, sizeof(raw), "[%s] %s = %s", key_path, value_name, value_data);
    artifact->raw = forensic_bytes_create(raw, strlen(raw));
    
    forensic_metadata_t meta = forensic_metadata_create(16);
    forensic_metadata_add(&meta, "key_path", key_path);
    forensic_metadata_add(&meta, "value_name", value_name);
    forensic_metadata_add(&meta, "value_data", value_data);
    forensic_metadata_add(&meta, "value_type", type);
    artifact->metadata = meta;
    
    return artifact;
}

static void collect_systemd_services(forensic_artifact_list_t* list) {
    const char* dirs[] = {
        "/etc/systemd/system",
        "/usr/lib/systemd/system",
        "/run/systemd/system",
        NULL
    };
    
    for (int i = 0; dirs[i]; i++) {
        DIR* dir = opendir(dirs[i]);
        if (!dir) continue;
        
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
            if (!strstr(entry->d_name, ".service")) continue;
            
            char fullpath[512];
            snprintf(fullpath, sizeof(fullpath), "%s/%s", dirs[i], entry->d_name);
            
            FILE* f = fopen(fullpath, "r");
            if (!f) continue;
            
            char line[512];
            char exec_start[512] = {0};
            while (fgets(line, sizeof(line), f)) {
                if (strncmp(line, "ExecStart=", 10) == 0) {
                    strncpy(exec_start, line + 10, sizeof(exec_start) - 1);
                    exec_start[strcspn(exec_start, "\n")] = '\0';
                    break;
                }
            }
            fclose(f);
            
            forensic_artifact_t* artifact = create_registry_artifact(
                dirs[i], entry->d_name, exec_start, "systemd_service"
            );
            if (artifact) {
                forensic_artifact_list_add(list, artifact);
            }
        }
        closedir(dir);
    }
}

static void collect_cron_jobs(forensic_artifact_list_t* list) {
    const char* cron_dirs[] = {
        "/etc/cron.d",
        "/etc/cron.daily",
        "/etc/cron.hourly",
        "/etc/cron.weekly",
        "/etc/cron.monthly",
        NULL
    };
    
    for (int i = 0; cron_dirs[i]; i++) {
        DIR* dir = opendir(cron_dirs[i]);
        if (!dir) continue;
        
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_name[0] == '.') continue;
            
            char fullpath[512];
            snprintf(fullpath, sizeof(fullpath), "%s/%s", cron_dirs[i], entry->d_name);
            
            FILE* f = fopen(fullpath, "r");
            if (!f) continue;
            
            char line[512];
            while (fgets(line, sizeof(line), f)) {
                if (line[0] == '#' || line[0] == '\n') continue;
                line[strcspn(line, "\n")] = '\0';
                
                forensic_artifact_t* artifact = create_registry_artifact(
                    cron_dirs[i], entry->d_name, line, "cron_job"
                );
                if (artifact) {
                    forensic_artifact_list_add(list, artifact);
                }
            }
            fclose(f);
        }
        closedir(dir);
    }
    
    struct passwd* pw;
    while ((pw = getpwent()) != NULL) {
        char cmd[256];
        snprintf(cmd, sizeof(cmd), "crontab -l -u %s 2>/dev/null", pw->pw_name);
        FILE* f = popen(cmd, "r");
        if (f) {
            char line[512];
            while (fgets(line, sizeof(line), f)) {
                line[strcspn(line, "\n")] = '\0';
                if (line[0] == '#' || line[0] == '\0') continue;
                
                forensic_artifact_t* artifact = create_registry_artifact(
                    "user_crontab", pw->pw_name, line, "user_cron"
                );
                if (artifact) {
                    forensic_artifact_list_add(list, artifact);
                }
            }
            pclose(f);
        }
    }
    endpwent();
}

static void collect_shell_configs(forensic_artifact_list_t* list) {
    const char* configs[] = {
        "/etc/profile", "/etc/bash.bashrc", "/etc/zsh/zshrc",
        NULL
    };
    
    for (int i = 0; configs[i]; i++) {
        struct stat st;
        if (stat(configs[i], &st) == 0) {
            forensic_artifact_t* artifact = create_registry_artifact(
                "shell_config", configs[i], "exists", "file"
            );
            if (artifact) {
                forensic_artifact_list_add(list, artifact);
            }
        }
    }
    
    struct passwd* pw;
    while ((pw = getpwent()) != NULL) {
        const char* user_configs[] = {
            ".bashrc", ".bash_profile", ".profile", ".zshrc", ".zprofile",
            NULL
        };
        for (int i = 0; user_configs[i]; i++) {
            char path[512];
            snprintf(path, sizeof(path), "%s/%s", pw->pw_dir, user_configs[i]);
            struct stat st;
            if (stat(path, &st) == 0) {
                forensic_artifact_t* artifact = create_registry_artifact(
                    "user_shell_config", user_configs[i], path, "file"
                );
                if (artifact) {
                    forensic_artifact_list_add(list, artifact);
                }
            }
        }
    }
    endpwent();
}

static void collect_init_scripts(forensic_artifact_list_t* list) {
    DIR* dir = opendir("/etc/init.d");
    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_name[0] == '.') continue;
            char path[512];
            snprintf(path, sizeof(path), "/etc/init.d/%s", entry->d_name);
            
            forensic_artifact_t* artifact = create_registry_artifact(
                "/etc/init.d", entry->d_name, path, "init_script"
            );
            if (artifact) {
                forensic_artifact_list_add(list, artifact);
            }
        }
        closedir(dir);
    }
}

static forensic_artifact_list_t* registry_collector_collect(const char* target, void* config) {
    (void)target;
    (void)config;
    
    forensic_artifact_list_t* list = forensic_artifact_list_create(128);
    if (!list) return NULL;
    
    collect_systemd_services(list);
    collect_cron_jobs(list);
    collect_shell_configs(list);
    collect_init_scripts(list);
    
    if (access("/etc/rc.local", F_OK) == 0) {
        forensic_artifact_t* artifact = create_registry_artifact(
            "/etc", "rc.local", "/etc/rc.local", "rc_local"
        );
        if (artifact) {
            forensic_artifact_list_add(list, artifact);
        }
    }
    
    printf("[registry_collector] Collected %zu registry/config artifacts\n", list->count);
    return list;
}

static void registry_collector_cleanup(void) {
    printf("[registry_collector] Cleanup\n");
}

/* ============================================================================
 * Plugin Definition
 * ============================================================================ */

static const char* registry_capabilities[] = {
    "read_system_config",
    "enumerate_services",
    "read_cron_jobs"
};

forensic_data_source_plugin_t registry_collector_plugin = {
    .name = "registry_collector",
    .version = "1.0.0",
    .description = "Collects systemd services, cron jobs, shell configs, and init scripts (Linux registry equivalent)",
    .init = registry_collector_init,
    .collect = registry_collector_collect,
    .cleanup = registry_collector_cleanup,
    .required_capabilities = registry_capabilities,
    .capability_count = sizeof(registry_capabilities) / sizeof(registry_capabilities[0])
};