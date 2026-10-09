/*
 * Anti-Analysis Module
 *
 * Portable debugger, VM, and sandbox detection.
 * Works on Windows and Linux.
 */

#define _GNU_SOURCE
#include "jocky_rt.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#include <intrin.h>
#else
#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <errno.h>
#endif

/* ============================================================================
 * Debugger Detection
 * ============================================================================ */

bool jocky_is_debugger_present(void)
{
#ifdef _WIN32
    return IsDebuggerPresent() != 0;
#else
    /* Linux: try PTRACE_TRACEME. If it fails, we're already being traced. */
    if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1) {
        return true;
    }
    /* If we succeeded, detach ourselves so we don't interfere. */
    ptrace(PTRACE_DETACH, 0, NULL, NULL);
    return false;
#endif
}

bool jocky_is_remote_debugger(void)
{
#ifdef _WIN32
    BOOL dbg = FALSE;
    CheckRemoteDebuggerPresent(GetCurrentProcess(), &dbg);
    return dbg != 0;
#else
    /* Check /proc/self/status for TracerPid */
    FILE* f = fopen("/proc/self/status", "r");
    if (!f) return false;

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "TracerPid:", 10) == 0) {
            int pid = atoi(line + 10);
            fclose(f);
            return pid != 0;
        }
    }
    fclose(f);
    return false;
#endif
}

bool jocky_check_hardware_breakpoints(void)
{
#ifdef _WIN32
    CONTEXT ctx = {0};
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
    if (!GetThreadContext(GetCurrentThread(), &ctx))
        return false;
    return (ctx.Dr0 || ctx.Dr1 || ctx.Dr2 || ctx.Dr3);
#else
    /* On Linux, hardware breakpoints are per-task and not easily checked
     * from userspace without ptrace. We skip this on Linux. */
    return false;
#endif
}

/* ============================================================================
 * VM Detection
 * ============================================================================ */

bool jocky_is_vm(void)
{
    return false;
}

/* ============================================================================
 * Sandbox Detection
 * ============================================================================ */

bool jocky_is_sandbox(void)
{
    /* Check if sleep is accelerated (sandbox may skip sleeps) */
    if (!jocky_check_timing_api())
        return true;

#ifdef _WIN32
    /* Check common sandbox usernames */
    char username[256] = {0};
    DWORD len = sizeof(username);
    if (GetUserNameA(username, &len)) {
        static const char* sandbox_users[] = {
            "sandbox", "vmware", "virtualbox", "john doe", "test",
            "malware", "virus", "john", "admin", NULL
        };
        for (int i = 0; sandbox_users[i]; i++) {
            if (_stricmp(username, sandbox_users[i]) == 0)
                return true;
        }
    }

    /* Check if common sandbox DLLs are loaded */
    static const char* sandbox_dlls[] = {
        "sbiedll.dll",        /* Sandboxie */
        "api_log.dll",        /* Various sandboxes */
        "dir_watch.dll",      /* Various sandboxes */
        "pstorec.dll",        /* Anubis */
        "vmcheck.dll",        /* VirtualPC */
        "wpespy.dll",         /* WPE */
        NULL
    };
    for (int i = 0; sandbox_dlls[i]; i++) {
        if (GetModuleHandleA(sandbox_dlls[i]) != NULL)
            return true;
    }
#else
    /* Linux: check for common VM indicators */
    FILE* f = fopen("/sys/class/dmi/id/product_name", "r");
    if (f) {
        char name[128] = {0};
        if (fgets(name, sizeof(name), f)) {
            if (strcasestr(name, "vmware") ||
                strcasestr(name, "virtualbox") ||
                strcasestr(name, "kvm") ||
                strcasestr(name, "qemu")) {
                fclose(f);
                return true;
            }
        }
        fclose(f);
    }
#endif
    return false;
}

/* ============================================================================
 * Timing Checks
 * ============================================================================ */

bool jocky_check_timing_rdtsc(void)
{
#ifdef _WIN32
    ULONGLONG t1 = __rdtsc();
    Sleep(50);
    ULONGLONG t2 = __rdtsc();
    /* If delta is suspiciously small, sleep was skipped (sandbox) */
    return (t2 - t1) > 10000000ULL;
#else
    unsigned long long t1, t2;
    struct timespec ts = {0, 50 * 1000000}; /* 50ms */

    __asm__ __volatile__("rdtsc" : "=A"(t1));
    nanosleep(&ts, NULL);
    __asm__ __volatile__("rdtsc" : "=A"(t2));

    return (t2 - t1) > 10000000ULL;
#endif
}

bool jocky_check_timing_api(void)
{
#ifdef _WIN32
    DWORD t1 = GetTickCount();
    Sleep(500);
    DWORD t2 = GetTickCount();
    return (t2 - t1) >= 400;  /* Allow some jitter */
#else
    struct timespec t1, t2;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    struct timespec ts = {0, 500 * 1000000};
    nanosleep(&ts, NULL);
    clock_gettime(CLOCK_MONOTONIC, &t2);

    long long delta_ms = (t2.tv_sec - t1.tv_sec) * 1000LL +
                         (t2.tv_nsec - t1.tv_nsec) / 1000000LL;
    return delta_ms >= 400;
#endif
}

/* ============================================================================
 * Master Check
 * ============================================================================ */

uint32_t jocky_check_analysis_environment(void)
{
    uint32_t flags = JOCKY_ANALYSIS_CLEAN;

    if (jocky_is_debugger_present())
        flags |= JOCKY_ANALYSIS_DEBUGGER;

    if (jocky_is_remote_debugger())
        flags |= JOCKY_ANALYSIS_DEBUGGER;

    if (jocky_check_hardware_breakpoints())
        flags |= JOCKY_ANALYSIS_DEBUGGER;

    if (jocky_is_vm())
        flags |= JOCKY_ANALYSIS_VM;

    if (jocky_is_sandbox())
        flags |= JOCKY_ANALYSIS_SANDBOX;

    return flags;
}

/* ============================================================================
 * Crypto Helpers
 * ============================================================================ */

void jocky_decrypt_xor(uint8_t* data, size_t len, uint8_t key, size_t key_len)
{
    (void)key_len;
    for (size_t i = 0; i < len; i++) data[i] ^= key;
}

void jocky_decrypt_rc4(uint8_t* data, size_t len, const uint8_t* key, size_t key_len)
{
    uint8_t S[256];
    for (int i = 0; i < 256; i++) S[i] = (uint8_t)i;

    int j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + S[i] + key[i % key_len]) & 0xFF;
        uint8_t tmp = S[i]; S[i] = S[j]; S[j] = tmp;
    }

    int i = 0; j = 0;
    for (size_t n = 0; n < len; n++) {
        i = (i + 1) & 0xFF;
        j = (j + S[i]) & 0xFF;
        uint8_t tmp = S[i]; S[i] = S[j]; S[j] = tmp;
        data[n] ^= S[(S[i] + S[j]) & 0xFF];
    }
}

/* ============================================================================
 * Integrity Verification (.jtamp)
 * ============================================================================ */

#ifdef _WIN32

/* CRC32 (IEEE / zlib polynomial) – must match packer.cpp exactly. */
static uint32_t at_crc32(const uint8_t* data, size_t len)
{
    uint32_t c = 0xFFFFFFFFu;
    for (size_t i = 0; i < len; i++) {
        c ^= data[i];
        for (int j = 0; j < 8; j++)
            c = (c & 1u) ? (0xEDB88320u ^ (c >> 1)) : (c >> 1);
    }
    return c ^ 0xFFFFFFFFu;
}

/* Minimal packed PE structures – only the fields we read. */
#pragma pack(push, 1)
typedef struct { uint16_t machine; uint16_t numSections; uint32_t ts;
                 uint32_t symPtr; uint32_t numSym; uint16_t optSize;
                 uint16_t chars; } at_coff_t;
typedef struct { char name[8]; uint32_t virtSize; uint32_t virtAddr;
                 uint32_t rawSize; uint32_t rawPtr; uint32_t relPtr;
                 uint32_t lnPtr; uint16_t numRel; uint16_t numLn;
                 uint32_t chars; } at_sec_t;
#pragma pack(pop)

/* .jtamp section layout (all LE):
 *   [0..7]   "JOCKYTMP"
 *   [8..11]  flags  (0x1 = version 1 / CRC32)
 *   [12..15] XOR-folded CRC32 of all other section raw data */
static const uint8_t JTAMP_MAGIC[8] = {'J','O','C','K','Y','T','M','P'};

bool jocky_verify_integrity(void)
{
    /* All declarations at the top – avoids MSVC "goto skips init" errors. */
    char       path[MAX_PATH];
    HANDLE     hf;
    DWORD      fsize, nread;
    uint8_t*   buf;
    bool       result;
    uint32_t   peOff, secOff, stored, computed;
    at_coff_t* coff;
    at_sec_t*  secs;
    bool       found_jtamp;
    uint16_t   i;

    if (!GetModuleFileNameA(NULL, path, MAX_PATH))
        return true;

    hf = CreateFileA(path, GENERIC_READ,
                     FILE_SHARE_READ | FILE_SHARE_DELETE,
                     NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hf == INVALID_HANDLE_VALUE)
        return true;

    fsize = GetFileSize(hf, NULL);
    if (fsize == INVALID_FILE_SIZE || fsize < 0x40) { CloseHandle(hf); return true; }

    buf = (uint8_t*)VirtualAlloc(NULL, fsize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!buf) { CloseHandle(hf); return true; }

    nread = 0;
    if (!ReadFile(hf, buf, fsize, &nread, NULL) || nread != fsize) {
        CloseHandle(hf); VirtualFree(buf, 0, MEM_RELEASE); return true;
    }
    CloseHandle(hf);

    result      = true;
    stored      = 0;
    computed    = 0;
    found_jtamp = false;

    /* Validate MZ + PE signatures. */
    if (buf[0] != 'M' || buf[1] != 'Z') goto done;
    peOff = *(uint32_t*)(buf + 0x3C);
    if (peOff + 4 + sizeof(at_coff_t) > fsize) goto done;
    if (buf[peOff] != 'P' || buf[peOff + 1] != 'E') goto done;

    coff   = (at_coff_t*)(buf + peOff + 4);
    secOff = peOff + 4 + (uint32_t)sizeof(at_coff_t) + coff->optSize;
    if (secOff + (uint32_t)coff->numSections * sizeof(at_sec_t) > fsize) goto done;

    secs = (at_sec_t*)(buf + secOff);

    /* Find .jtamp and read the stored checksum. */
    for (i = 0; i < coff->numSections; i++) {
        if (memcmp(secs[i].name, JTAMP_MAGIC, 8) != 0) continue;
        /* layout: magic(8) + flags(4) + checksum(4) */
        if (secs[i].rawPtr + 12 + 4 <= fsize) {
            stored      = *(uint32_t*)(buf + secs[i].rawPtr + 12);
            found_jtamp = true;
        }
        break;
    }

    if (!found_jtamp) goto done; /* unpacked build – nothing to verify */

    /* Compute XOR-folded CRC32 over every section except .jtamp,
     * matching packer.cpp addAntiTamper() exactly. */
    for (i = 0; i < coff->numSections; i++) {
        at_sec_t* s = &secs[i];
        if (memcmp(s->name, JTAMP_MAGIC, 8) == 0) continue;
        if (s->rawPtr > 0 && s->rawSize > 0 && s->rawPtr + s->rawSize <= fsize)
            computed ^= at_crc32(buf + s->rawPtr, s->rawSize);
    }

    if (computed != stored)
        result = false;

done:
    VirtualFree(buf, 0, MEM_RELEASE);
    if (!result)
        ExitProcess(0xDEAD1337u);
    return true;
}

#else  /* non-Windows stub */

bool jocky_verify_integrity(void) { return true; }

#endif /* _WIN32 */

/* ============================================================================
 * Initialization
 * ============================================================================ */

uint32_t jocky_runtime_init(void)
{
    /* Integrity check must run first so a patched binary never reaches
     * the anti-analysis or evasion routines. */
    jocky_verify_integrity();

#ifdef _WIN32
    /* Pre-load the .jmani manifest so jocky_driver_invoke() works
     * without an extra call.  Silently a no-op if no .jmani section. */
    jocky_manifest_load();
#endif

    return jocky_check_analysis_environment();
}
