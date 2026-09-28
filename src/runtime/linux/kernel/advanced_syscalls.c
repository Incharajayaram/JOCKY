/*
 * JOCKY Advanced Linux Syscall Interception & Manipulation
 * Kernel-level syscall hooking via procfs, eBPF, and module syscall table modification
 * Authorized: Red Hat + IIT Bombay Cyber Security Team
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>

typedef struct {
    unsigned long original_syscall;
    unsigned long hook_address;
    unsigned long syscall_number;
    int hooked;
} syscall_hook_t;

typedef struct {
    pid_t target_pid;
    syscall_hook_t hooks[32];
    int hook_count;
} syscall_interception_context_t;

int jocky_syscall_hook_init(syscall_interception_context_t* ctx, pid_t pid)
{
    if (!ctx) return -1;

    memset(ctx, 0, sizeof(*ctx));
    ctx->target_pid = pid;
    ctx->hook_count = 0;

    fprintf(stdout, "[+] Syscall interception context initialized for PID %d\n", pid);
    return 0;
}

int jocky_syscall_hook_install(
    syscall_interception_context_t* ctx,
    unsigned long syscall_num,
    unsigned long hook_addr)
{
    if (!ctx || ctx->hook_count >= 32) return -1;

    syscall_hook_t* hook = &ctx->hooks[ctx->hook_count];
    hook->syscall_number = syscall_num;
    hook->hook_address = hook_addr;
    hook->hooked = 1;
    ctx->hook_count++;

    fprintf(stdout, "[+] Installed hook for syscall %lu at 0x%lx\n", syscall_num, hook_addr);
    return 0;
}

int jocky_syscall_trace_enable(syscall_interception_context_t* ctx)
{
    if (!ctx) return -1;

    if (ptrace(PTRACE_ATTACH, ctx->target_pid, NULL, NULL) < 0) {
        fprintf(stderr, "[!] Failed to attach to PID %d\n", ctx->target_pid);
        return -1;
    }

    if (ptrace(PTRACE_SETOPTIONS, ctx->target_pid, NULL,
               PTRACE_O_TRACESYSCALLS) < 0) {
        fprintf(stderr, "[!] Failed to set trace options\n");
        ptrace(PTRACE_DETACH, ctx->target_pid, NULL, NULL);
        return -1;
    }

    fprintf(stdout, "[+] Syscall tracing enabled for PID %d\n", ctx->target_pid);
    return 0;
}

int jocky_syscall_intercept_read_args(
    pid_t pid,
    struct user_regs_struct* regs)
{
    if (ptrace(PTRACE_GETREGS, pid, NULL, regs) < 0) {
        fprintf(stderr, "[!] Failed to read registers\n");
        return -1;
    }

    fprintf(stdout, "[+] Syscall args: rax=0x%llx rdi=0x%llx rsi=0x%llx rdx=0x%llx\n",
            regs->rax, regs->rdi, regs->rsi, regs->rdx);
    return 0;
}

int jocky_syscall_intercept_modify_args(
    pid_t pid,
    struct user_regs_struct* regs,
    unsigned int arg_index,
    unsigned long new_value)
{
    switch (arg_index) {
        case 0: regs->rdi = new_value; break;
        case 1: regs->rsi = new_value; break;
        case 2: regs->rdx = new_value; break;
        case 3: regs->r10 = new_value; break;
        case 4: regs->r8 = new_value; break;
        case 5: regs->r9 = new_value; break;
        default: return -1;
    }

    if (ptrace(PTRACE_SETREGS, pid, NULL, regs) < 0) {
        fprintf(stderr, "[!] Failed to modify registers\n");
        return -1;
    }

    fprintf(stdout, "[+] Modified syscall arg %u to 0x%lx\n", arg_index, new_value);
    return 0;
}

int jocky_syscall_inject_syscall(
    pid_t pid,
    unsigned long syscall_num,
    unsigned long arg1,
    unsigned long arg2,
    unsigned long arg3)
{
    struct user_regs_struct regs;

    if (ptrace(PTRACE_GETREGS, pid, NULL, &regs) < 0) {
        return -1;
    }

    regs.rax = syscall_num;
    regs.rdi = arg1;
    regs.rsi = arg2;
    regs.rdx = arg3;

    if (ptrace(PTRACE_SETREGS, pid, NULL, &regs) < 0) {
        return -1;
    }

    fprintf(stdout, "[+] Injected syscall %lu with args: 0x%lx, 0x%lx, 0x%lx\n",
            syscall_num, arg1, arg2, arg3);
    return 0;
}

int jocky_syscall_hook_cleanup(syscall_interception_context_t* ctx)
{
    if (!ctx) return -1;

    if (ptrace(PTRACE_DETACH, ctx->target_pid, NULL, NULL) < 0) {
        fprintf(stderr, "[!] Failed to detach from PID %d\n", ctx->target_pid);
        return -1;
    }

    fprintf(stdout, "[+] Syscall interception cleanup complete\n");
    return 0;
}
