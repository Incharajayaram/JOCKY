/* PTRACE-based process control */

#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

int jocky_process_ptrace_attach(int pid) {
    if (pid <= 0) return -1;

    if (ptrace(PTRACE_ATTACH, pid, NULL, NULL) < 0) {
        return -1;
    }

    int status;
    if (waitpid(pid, &status, 0) < 0) {
        ptrace(PTRACE_DETACH, pid, NULL, NULL);
        return -1;
    }

    return 0;
}

int jocky_process_ptrace_detach(int pid) {
    if (pid <= 0) return -1;

    if (ptrace(PTRACE_DETACH, pid, NULL, NULL) < 0) {
        return -1;
    }

    return 0;
}

int jocky_process_get_maps(int pid) {
    /* Would read /proc/pid/maps and parse */
    return -1;
}

/* Advanced ptrace operations implemented in thread_hijack.c:
 * - jocky_thread_hijack
 * - jocky_spoof_syscall
 * - jocky_spoof_call
 */
