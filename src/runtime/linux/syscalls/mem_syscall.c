#include "../include/jocky_mem.h"
#include "../include/jocky_syscall.h"
#include "../include/jocky_file.h"
#include <string.h>
#include <stdio.h>

/* ========== MEMORY MAPPING ========== */

void* jocky_mmap(void* addr, size_t length, int prot, int flags, long fd, long offset) {
    if (length == 0) return NULL;

    long result = jocky_syscall6(SYS_mmap, (long)addr, length, prot, flags, fd, offset);
    if (result < 0) return NULL;

    return (void*)result;
}

int jocky_munmap(void* addr, size_t length) {
    if (!addr || length == 0) return -1;

    long result = jocky_syscall2(SYS_munmap, (long)addr, length);
    return (result == 0) ? 0 : -1;
}

int jocky_mprotect(void* addr, size_t length, int prot) {
    if (!addr || length == 0) return -1;

    long result = jocky_syscall3(SYS_mprotect, (long)addr, length, prot);
    return (result == 0) ? 0 : -1;
}

int jocky_msync(void* addr, size_t length) {
    if (!addr || length == 0) return -1;

    long result = jocky_syscall3(SYS_msync, (long)addr, length, 0);
    return (result == 0) ? 0 : -1;
}

long jocky_pagesize(void) {
    /* x86_64 standard page size is 4096 bytes */
    return 4096;
}

/* ========== MEMORY INTROSPECTION ========== */

int jocky_get_memmaps(void* maps, size_t max_maps) {
    if (!maps || max_maps == 0) return -1;

    /* Read /proc/self/maps */
    long fd = jocky_syscall2(SYS_open, (long)"/proc/self/maps", 0);
    if (fd < 0) return -1;

    char buffer[8192];
    long nread = jocky_syscall3(SYS_read, fd, (long)buffer, sizeof(buffer) - 1);
    jocky_syscall1(SYS_close, fd);

    if (nread <= 0) return -1;
    buffer[nread] = '\0';

    /* Parse /proc/self/maps format: addr-addr perms offset dev inode pathname */
    int count = 0;
    char* pos = buffer;

    while (pos < buffer + nread && count < (int)max_maps) {
        /* Skip to next newline */
        char* newline = strchr(pos, '\n');
        if (!newline) break;

        /* For now, just count lines as maps (simplified) */
        count++;
        pos = newline + 1;
    }

    return count;
}

uint64_t jocky_get_rss(void) {
    /* Read /proc/self/status and extract VmRSS */
    long fd = jocky_syscall2(SYS_open, (long)"/proc/self/status", 0);
    if (fd < 0) return 0;

    char buffer[4096];
    long nread = jocky_syscall3(SYS_read, fd, (long)buffer, sizeof(buffer) - 1);
    jocky_syscall1(SYS_close, fd);

    if (nread <= 0) return 0;
    buffer[nread] = '\0';

    uint64_t rss = 0;
    char* pos = buffer;

    while (*pos) {
        if (strncmp(pos, "VmRSS:", 6) == 0) {
            sscanf(pos, "VmRSS:\t%lu", &rss);
            break;
        }

        pos = strchr(pos, '\n');
        if (!pos) break;
        pos++;
    }

    return rss;
}

uint64_t jocky_get_vsize(void) {
    /* Read /proc/self/status and extract VmSize */
    long fd = jocky_syscall2(SYS_open, (long)"/proc/self/status", 0);
    if (fd < 0) return 0;

    char buffer[4096];
    long nread = jocky_syscall3(SYS_read, fd, (long)buffer, sizeof(buffer) - 1);
    jocky_syscall1(SYS_close, fd);

    if (nread <= 0) return 0;
    buffer[nread] = '\0';

    uint64_t vsize = 0;
    char* pos = buffer;

    while (*pos) {
        if (strncmp(pos, "VmSize:", 7) == 0) {
            sscanf(pos, "VmSize:\t%lu", &vsize);
            break;
        }

        pos = strchr(pos, '\n');
        if (!pos) break;
        pos++;
    }

    return vsize;
}
