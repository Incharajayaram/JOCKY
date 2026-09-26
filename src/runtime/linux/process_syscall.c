#include "../include/jocky_process.h"
#include "../include/jocky_syscall.h"
#include <string.h>
#include <stdio.h>

long jocky_getpid(void) {
    return jocky_syscall0(SYS_getpid);
}

long jocky_getppid(void) {
    return jocky_syscall0(SYS_getppid);
}

long jocky_getuid(void) {
    return jocky_syscall0(SYS_getuid);
}

long jocky_geteuid(void) {
    return jocky_syscall0(SYS_geteuid);
}

long jocky_getgid(void) {
    return jocky_syscall0(SYS_getgid);
}

long jocky_getegid(void) {
    return jocky_syscall0(SYS_getegid);
}

long jocky_gettid(void) {
    return jocky_syscall0(SYS_gettid);
}

int jocky_process_exists(long pid) {
    /* Use kill with signal 0 to check if process exists */
    long result = jocky_syscall2(SYS_kill, pid, 0);
    if (result == 0) return 1;
    return 0;
}

int jocky_process_info(long pid, jocky_process_info_t* out) {
    if (!out) return -1;

    memset(out, 0, sizeof(jocky_process_info_t));
    out->pid = pid;

    /* Read /proc/[pid]/status for reliable parsing */
    char path[64];
    snprintf(path, sizeof(path), "/proc/%ld/status", pid);

    long fd = jocky_syscall2(SYS_open, (long)path, 0);
    if (fd < 0) return -1;

    char buf[2048];
    long nread = jocky_syscall3(SYS_read, fd, (long)buf, sizeof(buf) - 1);
    jocky_syscall1(SYS_close, fd);

    if (nread <= 0) return -1;
    buf[nread] = '\0';

    /* Parse /proc/[pid]/status format */
    strncpy(out->name, "unknown", sizeof(out->name) - 1);

    /* Read /proc/[pid]/status for more info */
    snprintf(path, sizeof(path), "/proc/%ld/status", pid);
    fd = jocky_syscall2(SYS_open, (long)path, 0);
    if (fd >= 0) {
        nread = jocky_syscall3(SYS_read, fd, (long)buf, sizeof(buf) - 1);
        jocky_syscall1(SYS_close, fd);

        if (nread > 0) {
            buf[nread] = '\0';

            /* Parse Uid, Gid, VmRSS, VmSize */
            char* pos = buf;
            while (*pos) {
                if (strncmp(pos, "Uid:", 4) == 0) {
                    sscanf(pos, "Uid:\t%ld", &out->uid);
                } else if (strncmp(pos, "Gid:", 4) == 0) {
                    sscanf(pos, "Gid:\t%ld", &out->gid);
                } else if (strncmp(pos, "VmRSS:", 6) == 0) {
                    sscanf(pos, "VmRSS:\t%ld", &out->vm_rss);
                } else if (strncmp(pos, "VmSize:", 7) == 0) {
                    sscanf(pos, "VmSize:\t%ld", &out->vm_size);
                }

                pos = strchr(pos, '\n');
                if (!pos) break;
                pos++;
            }
        }
    }

    return 0;
}

int jocky_process_get_exe(long pid, char* buffer, size_t size) {
    if (!buffer || size == 0) return -1;

    char path[64];
    snprintf(path, sizeof(path), "/proc/%ld/exe", pid);

    /* readlink is a syscall that follows symlinks */
    long result = jocky_syscall3(SYS_readlink, (long)path, (long)buffer, size - 1);
    if (result <= 0) return -1;

    buffer[result] = '\0';
    return 0;
}

int jocky_process_get_cmdline(long pid, char* buffer, size_t size) {
    if (!buffer || size == 0) return -1;

    char path[64];
    snprintf(path, sizeof(path), "/proc/%ld/cmdline", pid);

    long fd = jocky_syscall2(SYS_open, (long)path, 0);
    if (fd < 0) return -1;

    long nread = jocky_syscall3(SYS_read, fd, (long)buffer, size - 1);
    jocky_syscall1(SYS_close, fd);

    if (nread <= 0) return -1;

    buffer[nread] = '\0';
    return 0;
}

int jocky_process_get_cwd(long pid, char* buffer, size_t size) {
    if (!buffer || size == 0) return -1;

    char path[64];
    snprintf(path, sizeof(path), "/proc/%ld/cwd", pid);

    long result = jocky_syscall3(SYS_readlink, (long)path, (long)buffer, size - 1);
    if (result <= 0) return -1;

    buffer[result] = '\0';
    return 0;
}

int jocky_enum_processes(long* pids, size_t max_pids) {
    if (!pids || max_pids == 0) return -1;

    /* Simplified: scan from PID 1 to 32768 checking existence */
    long count = 0;
    for (long pid = 1; pid <= 32768 && count < (long)max_pids; pid++) {
        if (jocky_process_exists(pid)) {
            pids[count++] = pid;
        }
    }

    return count;
}

int jocky_kill(long pid, int sig) {
    long result = jocky_syscall2(SYS_kill, pid, sig);
    return (result == 0) ? 0 : -1;
}

int jocky_raise(int sig) {
    long tid = jocky_syscall0(SYS_gettid);
    long pid = jocky_syscall0(SYS_getpid);
    long result = jocky_syscall3(SYS_tgkill, pid, tid, sig);
    return (result == 0) ? 0 : -1;
}

long jocky_process_get_rss(long pid) {
    jocky_process_info_t info;
    if (jocky_process_info(pid, &info) != 0) return 0;
    return info.vm_rss;
}

long jocky_process_get_vsize(long pid) {
    jocky_process_info_t info;
    if (jocky_process_info(pid, &info) != 0) return 0;
    return info.vm_size;
}
