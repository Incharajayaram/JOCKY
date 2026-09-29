#define _GNU_SOURCE
#include "jocky_rt.h"

#ifdef _WIN32
#include <windows.h>

bool jocky_self_delete(void) {
    wchar_t path[MAX_PATH];
    if (!GetModuleFileNameW(NULL, path, MAX_PATH))
        return false;

    HANDLE h = CreateFileW(path, DELETE,
                           FILE_SHARE_READ | FILE_SHARE_DELETE,
                           NULL, OPEN_EXISTING,
                           FILE_FLAG_DELETE_ON_CLOSE, NULL);
    if (h != INVALID_HANDLE_VALUE) {
        CloseHandle(h);
        return true;
    }
    return (bool)MoveFileExW(path, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
}

#else
#include <unistd.h>

bool jocky_self_delete(void) {
    char path[1024];
    ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if (len <= 0) return false;
    path[len] = '\0';
    return unlink(path) == 0;
}

#endif
