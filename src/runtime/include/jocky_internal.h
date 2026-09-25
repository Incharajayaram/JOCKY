/*
 * JOCKY Runtime — Internal Shared Helpers
 *
 * Not part of the public API.  Include only from runtime .c files, never from
 * user code.  All functions are static so each translation unit gets its own
 * copy with no link-time symbol collisions.
 */

#ifndef JOCKY_INTERNAL_H
#define JOCKY_INTERNAL_H

#ifdef _WIN32
#include <windows.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Fill buf with len cryptographically random bytes.
 * Uses advapi32!SystemFunction036 (RtlGenRandom). */
static bool gen_random(uint8_t* buf, size_t len)
{
    typedef BOOLEAN (WINAPI* pfn_t)(PVOID, ULONG);
    pfn_t fn = (pfn_t)GetProcAddress(GetModuleHandleA("advapi32.dll"),
                                      "SystemFunction036");
    return fn && fn(buf, (ULONG)len);
}

/* Enable a named token privilege for the current process.
 * Example names: SE_BACKUP_NAME, SE_RESTORE_NAME, SE_DEBUG_NAME. */
static bool enable_privilege(const wchar_t* name)
{
    HANDLE hTok;
    if (!OpenProcessToken(GetCurrentProcess(),
                          TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hTok))
        return false;

    LUID luid;
    bool ok = false;
    if (LookupPrivilegeValueW(NULL, name, &luid)) {
        TOKEN_PRIVILEGES tp;
        tp.PrivilegeCount           = 1;
        tp.Privileges[0].Luid       = luid;
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        ok = AdjustTokenPrivileges(hTok, FALSE, &tp, 0, NULL, NULL) &&
             GetLastError() == ERROR_SUCCESS;
    }
    CloseHandle(hTok);
    return ok;
}

/* Delete every file matching a wildcard glob pattern (single directory,
 * no recursion).  Strips read-only attributes before deleting. */
static void wipe_glob(const wchar_t* pattern)
{
    WIN32_FIND_DATAW fd;
    HANDLE h = FindFirstFileW(pattern, &fd);
    if (h == INVALID_HANDLE_VALUE) return;

    wchar_t dir[MAX_PATH];
    wcsncpy_s(dir, MAX_PATH, pattern, _TRUNCATE);
    wchar_t* sl = wcsrchr(dir, L'\\');
    if (!sl) { FindClose(h); return; }
    *(sl + 1) = L'\0';

    do {
        if (!wcscmp(fd.cFileName, L".") || !wcscmp(fd.cFileName, L"..")) continue;
        wchar_t full[MAX_PATH];
        _snwprintf_s(full, MAX_PATH, _TRUNCATE, L"%s%s", dir, fd.cFileName);
        SetFileAttributesW(full, FILE_ATTRIBUTE_NORMAL);
        DeleteFileW(full);
    } while (FindNextFileW(h, &fd));

    FindClose(h);
}

#endif /* _WIN32 */
#endif /* JOCKY_INTERNAL_H */
