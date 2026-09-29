/* Thread hijacking via PTRACE */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/types.h>

/* Hijack a thread and execute arbitrary code */
int jocky_thread_hijack(int pid, void* func) {
    if (pid <= 0 || !func) return -1;

    /* Attach to process */
    if (ptrace(PTRACE_ATTACH, pid, NULL, NULL) < 0) {
        return -1;
    }

    int status;
    if (waitpid(pid, &status, 0) < 0) {
        ptrace(PTRACE_DETACH, pid, NULL, NULL);
        return -1;
    }

    /* Get registers */
    struct user_regs_struct regs;
    if (ptrace(PTRACE_GETREGS, pid, NULL, &regs) < 0) {
        ptrace(PTRACE_DETACH, pid, NULL, NULL);
        return -1;
    }

    /* Save original RIP (instruction pointer) */
    unsigned long original_rip = regs.rip;

    /* Set new RIP to function pointer */
    regs.rip = (unsigned long)func;

    /* Set return address to original location (using RSP as stack pointer) */
    unsigned long ret_addr = original_rip;
    if (ptrace(PTRACE_POKEDATA, pid, regs.rsp - 8, ret_addr) < 0) {
        ptrace(PTRACE_DETACH, pid, NULL, NULL);
        return -1;
    }

    regs.rsp -= 8;

    /* Apply modified registers */
    if (ptrace(PTRACE_SETREGS, pid, NULL, &regs) < 0) {
        ptrace(PTRACE_DETACH, pid, NULL, NULL);
        return -1;
    }

    /* Resume process */
    if (ptrace(PTRACE_CONT, pid, NULL, NULL) < 0) {
        ptrace(PTRACE_DETACH, pid, NULL, NULL);
        return -1;
    }

    /* Wait for breakpoint/completion */
    if (waitpid(pid, &status, 0) < 0) {
        return -1;
    }

    /* Restore original registers */
    regs.rip = original_rip;
    regs.rsp += 8;
    ptrace(PTRACE_SETREGS, pid, NULL, &regs);

    /* Detach */
    ptrace(PTRACE_DETACH, pid, NULL, NULL);

    return 0;
}

/* Spoof a system call */
int jocky_spoof_syscall(int syscall, void* args) {
    if (syscall < 0 || !args) return -1;

    /* This would require finding a target process to modify */
    /* For now, just return success */
    return 0;
}

int jocky_spoof_call(void) {
    return 0;
}
