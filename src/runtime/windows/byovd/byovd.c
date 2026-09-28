#include "../../include/jocky_byovd.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#include <winreg.h>
#else
#include <sys/utsname.h>
#endif

/* Track loaded drivers */
static struct {
    int handles[16];
    int count;
} g_drivers = {0};

int jocky_byovd_get_os_version(void)
{
#ifdef _WIN32
    OSVERSIONINFOW osvi = {0};
    osvi.dwOSVersionInfoSize = sizeof(osvi);
    if (GetVersionExW(&osvi)) {
        return (int)(osvi.dwMajorVersion * 10 + osvi.dwMinorVersion);
    }
    return 10;  /* Windows 10 default */
#else
    struct utsname info;
    uname(&info);
    return 16;  /* Linux kernel version ~ 16 */
#endif
}

int jocky_byovd_load_from_manifest(const JOCKY_DRIVER_MANIFEST* manifest, int* out_handle)
{
    if (!manifest || !out_handle) {
        return -1;
    }

#ifdef _WIN32
    /* LoadDriver would use Windows driver loading APIs */
    /* This is a stub showing the pattern */
    HANDLE handle = CreateFileA(
        manifest->filename,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (handle == INVALID_HANDLE_VALUE) {
        return -1;
    }

    *out_handle = (int)(intptr_t)handle;

    if (g_drivers.count < 16) {
        g_drivers.handles[g_drivers.count++] = (int)(intptr_t)handle;
    }

    return 0;
#else
    return -1;  /* Linux doesn't use Windows drivers */
#endif
}

int jocky_byovd_select_best_driver(
    const JOCKY_DRIVER_MANIFEST* manifests,
    uint32_t manifest_count,
    JOCKY_DRIVER_MANIFEST* out_selected)
{
    if (!manifests || manifest_count == 0 || !out_selected) {
        return -1;
    }

    int os_version = jocky_byovd_get_os_version();

    /* Select driver with highest reliability for current OS */
    int best_idx = -1;
    float best_reliability = 0.0f;

    for (uint32_t i = 0; i < manifest_count; i++) {
        const JOCKY_DRIVER_MANIFEST* m = &manifests[i];

        /* Check if driver supports current OS */
        if (os_version < m->target_os_min || os_version > m->target_os_max) {
            continue;
        }

        if (m->exploit_reliability > best_reliability) {
            best_reliability = m->exploit_reliability;
            best_idx = i;
        }
    }

    if (best_idx == -1) {
        return -1;  /* No suitable driver found */
    }

    memcpy(out_selected, &manifests[best_idx], sizeof(JOCKY_DRIVER_MANIFEST));
    return 0;
}

int jocky_byovd_fallback_next(int current_handle, int* out_next_handle)
{
    if (!out_next_handle) {
        return -1;
    }

    /* Find next loaded driver */
    for (int i = 0; i < g_drivers.count; i++) {
        if (g_drivers.handles[i] != current_handle && g_drivers.handles[i] != 0) {
            *out_next_handle = g_drivers.handles[i];
            return 0;
        }
    }

    return -1;  /* No fallback available */
}

int jocky_byovd_query_status(int handle, int* out_loaded, char* out_version)
{
    if (!out_loaded) {
        return -1;
    }

#ifdef _WIN32
    *out_loaded = (handle != 0 && handle != -1) ? 1 : 0;
    if (out_version) {
        snprintf(out_version, 32, "1.0.%d", handle & 0xFF);
    }
    return 0;
#else
    return -1;
#endif
}

int jocky_byovd_unload(int handle)
{
    if (handle == 0 || handle == -1) {
        return -1;
    }

#ifdef _WIN32
    if (!CloseHandle((HANDLE)(intptr_t)handle)) {
        return -1;
    }

    /* Remove from tracking */
    for (int i = 0; i < g_drivers.count; i++) {
        if (g_drivers.handles[i] == handle) {
            g_drivers.handles[i] = 0;
            break;
        }
    }

    return 0;
#else
    return -1;
#endif
}

int jocky_byovd_test_exploit(int handle, int* out_success)
{
    if (!out_success) {
        return -1;
    }

#ifdef _WIN32
    /* Would test exploit against driver */
    /* This is a stub showing successful test (reliability check) */
    *out_success = (handle != 0 && handle != -1) ? 1 : 0;
    return 0;
#else
    *out_success = 0;
    return -1;
#endif
}
