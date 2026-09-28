#ifndef JOCKY_BLINDSIDE_H
#define JOCKY_BLINDSIDE_H

#include <stdint.h>
#include <windows.h>

/* Blindside: Hardware Breakpoint-Based ntdll Unhooking
 *
 * Uses hardware debug registers (DR0-DR3) to acquire a clean copy of
 * ntdll.dll from a suspended child process BEFORE EDR userland hooks
 * and DLL injection occurs. Bypasses disk-based ntdll restoration by
 * obtaining the image directly from memory of an unhooked child process.
 *
 * Technique: Cymulate, September 2026
 * Platform: Windows x64
 * Effort: 3 days
 *
 * Advantages:
 * - No disk reads of ntdll (EDRs monitor these)
 * - Breaks before EDR DLLs are loaded into child
 * - No user-mode hooks in clean ntdll copy
 * - Works under modern EDR/XDR platforms
 */

/* Breakpoint context and debug register configuration */
typedef struct {
    CONTEXT thread_context;
    uint64_t breakpoint_address;
    uint32_t dr_register;  /* Which DR (0-3) holds the breakpoint */
} DEBUG_BREAKPOINT_CONFIG;

typedef struct {
    HANDLE process;
    HANDLE thread;
    uint32_t pid;
    void* ntdll_base;
    size_t ntdll_size;
} CHILD_PROCESS_CONTEXT;

/* Step 1: Create a suspended child process for debug context */
int jocky_blindside_create_debug_child(
    const char* target_exe,
    CHILD_PROCESS_CONTEXT* out_ctx);

/* Step 2: Set hardware breakpoint on LdrLoadDll in child process */
int jocky_blindside_set_ldrloddll_breakpoint(
    CHILD_PROCESS_CONTEXT* child_ctx,
    DEBUG_BREAKPOINT_CONFIG* out_config);

/* Step 3: Resume child and wait for breakpoint hit */
int jocky_blindside_wait_breakpoint(
    CHILD_PROCESS_CONTEXT* child_ctx);

/* Step 4: Read clean ntdll from child's address space */
int jocky_blindside_read_clean_ntdll(
    CHILD_PROCESS_CONTEXT* child_ctx,
    uint8_t** out_ntdll_copy,
    size_t* out_size);

/* Step 5: Inject clean ntdll into parent process */
int jocky_blindside_inject_clean_ntdll(
    HANDLE parent_process,
    const uint8_t* clean_ntdll,
    size_t ntdll_size);

/* Full pipeline: create child, breakpoint, extract clean ntdll, inject to parent */
int jocky_blindside_unhook_ntdll(void);

/* Cleanup: terminate debug child and restore original ntdll mappings */
int jocky_blindside_cleanup(CHILD_PROCESS_CONTEXT* child_ctx);

/* Utility: Find ntdll base and size in process */
int jocky_blindside_find_ntdll(
    HANDLE process,
    void** out_base,
    size_t* out_size);

/* Utility: Set debug register in thread context */
int jocky_blindside_set_debug_register(
    CONTEXT* context,
    uint32_t dr_num,
    uint64_t address);

#endif /* JOCKY_BLINDSIDE_H */
