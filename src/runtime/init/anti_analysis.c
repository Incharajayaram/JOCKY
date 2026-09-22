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
    /* CPUID hypervisor bit check */
#ifdef _WIN32
    int cpuinfo[4] = {0};
    __cpuid(cpuinfo, 1);
    return (cpuinfo[2] & (1 << 31)) != 0;
#else
    unsigned int eax, ebx, ecx, edx;
    __asm__ __volatile__("cpuid"
                         : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                         : "a"(1));
    return (ecx & (1U << 31)) != 0;
#endif
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

void jocky_decrypt_xor(uint8_t* data, size_t len, uint8_t key)
{
    for (size_t i = 0; i < len; i++) {
        data[i] ^= key;
        key = (key << 1) | (key >> 7); /* rotate left */
    }
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
 * Initialization
 * ============================================================================ */

uint32_t jocky_runtime_init(void)
{
    uint32_t threats = jocky_check_analysis_environment();

    /* Optionally: if threats detected, we could exit or take evasive action.
     * For the SIH framework, we just report them and let the caller decide. */

    return threats;
}
