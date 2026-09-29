/*
 * General Win32 Interop Wrapper Implementation for JOCKY
 */

#include "jocky_win_wrapper.h"

#ifdef _WIN32
#include <windows.h>
#include <stdio.h>

int32_t jocky_win_get_process_id(void) {
    return (int32_t)GetCurrentProcessId();
}

int32_t jocky_win_get_system_info(char* out_buffer, int32_t max_len) {
    if (!out_buffer || max_len <= 0) return -1;
    SYSTEM_INFO sys_info;
    GetSystemInfo(&sys_info);
    return snprintf(out_buffer, (size_t)max_len,
                    "Processors: %u, PageSize: %u, Arch: %u",
                    sys_info.dwNumberOfProcessors,
                    sys_info.dwPageSize,
                    sys_info.wProcessorArchitecture);
}

int64_t jocky_win_get_tick_count(void) {
    return (int64_t)GetTickCount64();
}

int32_t jocky_win_get_computer_name(char* out_buffer, int32_t max_len) {
    if (!out_buffer || max_len <= 0) return -1;
    DWORD len = (DWORD)max_len;
    if (GetComputerNameA(out_buffer, &len)) {
        return (int32_t)len;
    }
    return -1;
}

int32_t jocky_win_get_memory_status(char* out_buffer, int32_t max_len) {
    if (!out_buffer || max_len <= 0) return -1;
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    if (GlobalMemoryStatusEx(&statex)) {
        return snprintf(out_buffer, (size_t)max_len,
                        "Memory Load: %u%%, TotalPhys: %llu MB",
                        statex.dwMemoryLoad,
                        (unsigned long long)(statex.ullTotalPhys / (1024 * 1024)));
    }
    return -1;
}

int32_t jocky_win_get_temp_path(char* out_buffer, int32_t max_len) {
    if (!out_buffer || max_len <= 0) return -1;
    DWORD res = GetTempPathA((DWORD)max_len, out_buffer);
    return (int32_t)res;
}

bool jocky_win_file_exists(const char* file_path) {
    if (!file_path) return false;
    DWORD attribs = GetFileAttributesA(file_path);
    return (attribs != INVALID_FILE_ATTRIBUTES && !(attribs & FILE_ATTRIBUTE_DIRECTORY));
}

int32_t jocky_win_show_msgbox(const char* title, const char* message, int32_t flags) {
    return (int32_t)MessageBoxA(NULL, message, title, (UINT)flags);
}

#else
/* Non-Windows stubs for cross-compilation fallback */
#include <stdio.h>
#include <string.h>

int32_t jocky_win_get_process_id(void) { return 1000; }
int32_t jocky_win_get_system_info(char* out_buffer, int32_t max_len) {
    if (!out_buffer || max_len <= 0) return -1;
    return snprintf(out_buffer, (size_t)max_len, "Processors: 4, PageSize: 4096, Arch: 9");
}
int64_t jocky_win_get_tick_count(void) { return 50000; }
int32_t jocky_win_get_computer_name(char* out_buffer, int32_t max_len) {
    if (!out_buffer || max_len <= 0) return -1;
    strncpy(out_buffer, "CROSS-COMPILER", max_len - 1);
    out_buffer[max_len - 1] = '\0';
    return (int32_t)strlen(out_buffer);
}
int32_t jocky_win_get_memory_status(char* out_buffer, int32_t max_len) {
    if (!out_buffer || max_len <= 0) return -1;
    return snprintf(out_buffer, (size_t)max_len, "Memory Load: 45%%, TotalPhys: 16384 MB");
}
int32_t jocky_win_get_temp_path(char* out_buffer, int32_t max_len) {
    if (!out_buffer || max_len <= 0) return -1;
    strncpy(out_buffer, "C:\\Users\\Temp\\", max_len - 1);
    out_buffer[max_len - 1] = '\0';
    return (int32_t)strlen(out_buffer);
}
bool jocky_win_file_exists(const char* file_path) { (void)file_path; return false; }
int32_t jocky_win_show_msgbox(const char* title, const char* message, int32_t flags) {
    (void)title; (void)message; (void)flags; return 1;
}
#endif
