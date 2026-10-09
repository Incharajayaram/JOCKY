/*
 * Cleanup: Self-Deletion
 *
 * Deletes the running executable from disk using a three-tier fallback:
 *
 *  Tier 1 (Win10 1809+): NtSetInformationFile with FileDispositionInformationEx
 *    and FILE_DISPOSITION_FLAG_POSIX_SEMANTICS.  The file is unlinked while
 *    still mapped; the deletion takes effect immediately and the path is gone
 *    before the process exits.
 *
 *  Tier 2 (Win7+): Rename the binary to a random name under %TEMP%, open it
 *    with FILE_FLAG_DELETE_ON_CLOSE, then close the handle.  The file is
 *    deleted when the last handle (held by the OS image loader) is released,
 *    i.e. when the process terminates.
 *
 *  Tier 3 (fallback): MoveFileEx with MOVEFILE_DELAY_UNTIL_REBOOT.  The file
 *    is deleted on the next system restart.
 *
 *  Linux: readlink /proc/self/exe, then unlink.
 */

#include "jocky_rt.h"
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>

/* ── NtSetInformationFile types (not in all SDK headers) ────────────── */

typedef LONG NTSTATUS;

typedef struct {
    ULONG_PTR  Information;
    NTSTATUS   Status;
} IO_STATUS_BLOCK_LOCAL;

#define FileDispositionInformationEx  64

#define FILE_DISPOSITION_FLAG_DELETE           0x00000001UL
#define FILE_DISPOSITION_FLAG_POSIX_SEMANTICS  0x00000002UL
#define FILE_DISPOSITION_FLAG_IGNORE_READONLY  0x00000010UL

typedef struct {
    DWORD Flags;
} FILE_DISPOSITION_INFORMATION_EX;

typedef NTSTATUS (WINAPI* pfn_NtSIF)(HANDLE, IO_STATUS_BLOCK_LOCAL*, PVOID, ULONG, ULONG);

/* ── Tier 1: POSIX delete ───────────────────────────────────────────── */

static bool try_posix_delete(const wchar_t* path)
{
    pfn_NtSIF NtSIF = (pfn_NtSIF)GetProcAddress(
        GetModuleHandleA("ntdll.dll"), "NtSetInformationFile");
    if (!NtSIF) return false;

    HANDLE h = CreateFileW(path, DELETE,
                            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                            NULL, OPEN_EXISTING,
                            FILE_FLAG_OPEN_REPARSE_POINT, NULL);
    if (h == INVALID_HANDLE_VALUE) return false;

    FILE_DISPOSITION_INFORMATION_EX diex;
    diex.Flags = FILE_DISPOSITION_FLAG_DELETE
               | FILE_DISPOSITION_FLAG_POSIX_SEMANTICS
               | FILE_DISPOSITION_FLAG_IGNORE_READONLY;

    IO_STATUS_BLOCK_LOCAL iosb = {0};
    NTSTATUS st = NtSIF(h, &iosb, &diex,
                         sizeof(diex), FileDispositionInformationEx);

    CloseHandle(h);
    return st >= 0;  /* NTSTATUS success = non-negative */
}

/* ── Tier 2: rename + delete-on-close ──────────────────────────────── */

static bool try_rename_delete(const wchar_t* path)
{
    wchar_t tmp_dir[MAX_PATH], tmp_file[MAX_PATH];
    if (!GetTempPathW(MAX_PATH, tmp_dir)) return false;
    if (!GetTempFileNameW(tmp_dir, L"jk", 0, tmp_file)) return false;

    /* GetTempFileName creates the file; delete it so MoveFile can use the name */
    DeleteFileW(tmp_file);

    if (!MoveFileW(path, tmp_file)) return false;

    /* Open with delete-on-close so the OS removes it when handles drop to zero */
    HANDLE h = CreateFileW(tmp_file,
                            GENERIC_READ,
                            FILE_SHARE_READ | FILE_SHARE_DELETE,
                            NULL, OPEN_EXISTING,
                            FILE_FLAG_DELETE_ON_CLOSE, NULL);
    if (h != INVALID_HANDLE_VALUE) CloseHandle(h);  /* triggers pending delete */

    /* The file may still exist until the image-loader handle closes at exit.
     * That is acceptable — the original path is already gone. */
    return true;
}

/* ── Tier 3: schedule for next reboot ──────────────────────────────── */

static bool try_reboot_delete(const wchar_t* path)
{
    wchar_t tmp_dir[MAX_PATH], tmp_file[MAX_PATH];
    if (!GetTempPathW(MAX_PATH, tmp_dir)) return false;
    if (!GetTempFileNameW(tmp_dir, L"jk", 0, tmp_file)) return false;
    DeleteFileW(tmp_file);

    /* Move first so the original path is gone immediately */
    if (MoveFileW(path, tmp_file))
        return MoveFileExW(tmp_file, NULL, MOVEFILE_DELAY_UNTIL_REBOOT) != FALSE;

    return MoveFileExW(path, NULL, MOVEFILE_DELAY_UNTIL_REBOOT) != FALSE;
}

/* ── Tier 4: bat script that deletes the file after process exits ────── */

static void launch_bat_cleanup(const char* path_a)
{
    char tmp[MAX_PATH], bat[MAX_PATH];
    if (!GetTempPathA(MAX_PATH, tmp)) return;
    if (!GetTempFileNameA(tmp, "jkd", 0, bat)) return;

    char content[2048];
    /* ping is a reliable ~1 s/hop delay without requiring extra binaries */
    snprintf(content, sizeof(content),
        "@echo off\r\n"
        "ping 127.0.0.1 -n 4 >nul\r\n"
        ":loop\r\n"
        "del /f /q \"%s\"\r\n"
        "if exist \"%s\" (ping 127.0.0.1 -n 2 >nul & goto loop)\r\n"
        "del /f /q \"%%~f0\"\r\n",
        path_a, path_a);

    HANDLE hf = CreateFileA(bat, GENERIC_WRITE, 0, NULL,
                            CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hf == INVALID_HANDLE_VALUE) return;
    DWORD written = 0;
    WriteFile(hf, content, (DWORD)strlen(content), &written, NULL);
    CloseHandle(hf);

    SHELLEXECUTEINFOA sei = {0};
    sei.cbSize = sizeof(sei);
    sei.lpVerb = "open";
    sei.lpFile = bat;
    sei.nShow  = SW_HIDE;
    ShellExecuteExA(&sei);
}

/* ── Public API ─────────────────────────────────────────────────────── */

void jocky_self_delete(void)
{
    wchar_t exe[MAX_PATH] = {0};
    char    exe_a[MAX_PATH] = {0};
    if (!GetModuleFileNameW(NULL, exe, MAX_PATH)) return;
    WideCharToMultiByte(CP_ACP, 0, exe, -1, exe_a, MAX_PATH, NULL, NULL);

    /* Attempt in-process deletion first; also always launch the bat cleanup
     * so the file is gone within a few seconds of process exit regardless of
     * which tier succeeds or fails. */
    launch_bat_cleanup(exe_a);

    if (!try_posix_delete(exe) && !try_rename_delete(exe))
        try_reboot_delete(exe);
}

#endif /* _WIN32 */
