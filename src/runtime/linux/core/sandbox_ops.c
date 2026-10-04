/* Sandbox operations for Linux */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sched.h>

/* Fallback definitions if not available on this system */
#ifndef CLONE_NEWPID
#define CLONE_NEWPID 0x20000000
#endif
#ifndef CLONE_NEWNET
#define CLONE_NEWNET 0x40000000
#endif
#ifndef CLONE_NEWIPC
#define CLONE_NEWIPC 0x08000000
#endif

/* Spawn sandboxed process */
int sandbox_spawn(const char* exe, const char* args) {
    if (!exe) return -1;

    pid_t pid = fork();

    if (pid == 0) {
        /* Child process - set up sandbox */

        /* Unshare namespaces for isolation */
        unshare(CLONE_NEWPID | CLONE_NEWNET | CLONE_NEWIPC);

        /* Execute the process */
        char* argv[] = {(char*)exe, (char*)args, NULL};
        execvp(exe, argv);

        exit(1);
    } else if (pid > 0) {
        return pid;
    }

    return -1;
}

/* Set resource limits for sandboxed process */
int sandbox_set_limits(int pid, long max_memory, int cpu_time, long max_files) {
    struct rlimit rlim;

    if (max_memory > 0) {
        rlim.rlim_cur = max_memory;
        rlim.rlim_max = max_memory;
        prlimit(pid, RLIMIT_AS, &rlim, NULL);
    }

    if (cpu_time > 0) {
        rlim.rlim_cur = cpu_time;
        rlim.rlim_max = cpu_time;
        prlimit(pid, RLIMIT_CPU, &rlim, NULL);
    }

    if (max_files > 0) {
        rlim.rlim_cur = max_files;
        rlim.rlim_max = max_files;
        prlimit(pid, RLIMIT_NOFILE, &rlim, NULL);
    }

    return 0;
}

/* Monitor process execution */
int sandbox_monitor(int pid) {
    if (pid <= 0) return -1;

    /* Could use ptrace or /proc for monitoring */
    /* Simple implementation just checks if process exists */
    char proc_path[64];
    snprintf(proc_path, sizeof(proc_path), "/proc/%d", pid);

    return (access(proc_path, F_OK) == 0) ? 1 : 0;
}

/* Wait for sandboxed process */
int sandbox_wait(int pid) {
    if (pid <= 0) return -1;

    int status;
    if (waitpid(pid, &status, 0) == pid) {
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        }
        return -1;
    }

    return -1;
}

/* Export process trace/strace output */
int sandbox_export_trace(int pid, const char* file) {
    if (pid <= 0 || !file) return -1;

    char cmd[512];
    snprintf(cmd, sizeof(cmd), "strace -p %d -o %s 2>&1", pid, file);

    return system(cmd);
}
