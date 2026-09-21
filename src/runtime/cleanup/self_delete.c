/*
 * Cleanup: Self-Deletion
 *
 * Deletes the running executable from disk.
 * Windows: uses MoveFileEx with MOVEFILE_DELAY_UNTIL_REBOOT
 * Linux: unlinks /proc/self/exe
 */

#include "jocky_rt.h"
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <linux/limits.h>
#endif

bool jocky_self_delete(void)
{
#ifdef _WIN32
    wchar_t exePath[MAX_PATH] = {0};
    if (!GetModuleFileNameW(NULL, exePath, MAX_PATH))
        return false;

    /* Rename to a random temp name first, then mark for deletion on reboot */
    wchar_t tempPath[MAX_PATH] = {0};
    if (!GetTempPathW(MAX_PATH, tempPath))
        return false;

    wchar_t tempFile[MAX_PATH] = {0};
    GetTempFileNameW(tempPath, L"jk", 0, tempFile);

    if (!MoveFileW(exePath, tempFile))
        return false;

    /* Schedule deletion on next reboot */
    return MoveFileExW(tempFile, NULL, MOVEFILE_DELAY_UNTIL_REBOOT) != 0;
#else
    /* Linux: read /proc/self/exe and unlink it */
    char path[PATH_MAX] = {0};
    ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if (len == -1) return false;
    path[len] = '\0';

    return unlink(path) == 0;
#endif
}
