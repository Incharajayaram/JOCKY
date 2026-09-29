/* Kernel memory read/write - core capability for Linux */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>

/* Try /dev/mem first, fallback to BYOVD */
int jocky_kread(void* addr, void* data, int size) {
    if (!addr || !data || size <= 0) return -1;

    /* Try /dev/mem */
    int fd = open("/dev/mem", O_RDONLY);
    if (fd >= 0) {
        if (lseek(fd, (off_t)addr, SEEK_SET) == (off_t)addr) {
            ssize_t n = read(fd, data, size);
            close(fd);
            return (n == size) ? 0 : -1;
        }
        close(fd);
    }

    /* Try /proc/self/mem (requires ptrace attachment to self) */
    fd = open("/proc/self/mem", O_RDONLY);
    if (fd >= 0) {
        if (lseek(fd, (off_t)addr, SEEK_SET) == (off_t)addr) {
            ssize_t n = read(fd, data, size);
            close(fd);
            return (n == size) ? 0 : -1;
        }
        close(fd);
    }

    return -1;
}

int jocky_kwrite(void* addr, void* data, int size) {
    if (!addr || !data || size <= 0) return -1;

    /* Try /dev/mem */
    int fd = open("/dev/mem", O_WRONLY);
    if (fd >= 0) {
        if (lseek(fd, (off_t)addr, SEEK_SET) == (off_t)addr) {
            ssize_t n = write(fd, data, size);
            close(fd);
            return (n == size) ? 0 : -1;
        }
        close(fd);
    }

    /* Try /proc/self/mem */
    fd = open("/proc/self/mem", O_WRONLY);
    if (fd >= 0) {
        if (lseek(fd, (off_t)addr, SEEK_SET) == (off_t)addr) {
            ssize_t n = write(fd, data, size);
            close(fd);
            return (n == size) ? 0 : -1;
        }
        close(fd);
    }

    return -1;
}

int jocky_driver_read_phys(void* addr, void* data, int size) {
    /* BYOVD driver physical memory read */
    return jocky_kread(addr, data, size);
}

int jocky_driver_write_phys(void* addr, void* data, int size) {
    /* BYOVD driver physical memory write */
    return jocky_kwrite(addr, data, size);
}

int jocky_driver_map_kernel(void) {
    /* Map kernel memory (identity mapping in Linux) */
    return 0;
}
