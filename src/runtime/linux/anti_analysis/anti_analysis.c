#include "../include/jocky_anti_analysis.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ptrace.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <signal.h>
#include <setjmp.h>

static jmp_buf jump_buffer;
static int detected_signal = 0;

static void signal_handler(int sig) {
    detected_signal = sig;
    longjmp(jump_buffer, 1);
}

int jocky_detect_debugger(void) {
    if (ptrace(PTRACE_TRACEME, 0, 0, 0) < 0) {
        return 1;
    }
    return 0;
}

int jocky_detect_ida(void) {
    const char* ida_markers[] = {
        "IDA",
        "IDAPYTHON",
        "_ida",
        "__ida"
    };

    char proc_maps[256];
    snprintf(proc_maps, sizeof(proc_maps), "/proc/%d/maps", getpid());

    int fd = open(proc_maps, O_RDONLY);
    if (fd < 0) {
        return 0;
    }

    char buffer[4096];
    int bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);

    if (bytes_read <= 0) {
        return 0;
    }

    buffer[bytes_read] = '\0';

    for (int i = 0; i < 4; i++) {
        if (strstr(buffer, ida_markers[i]) != NULL) {
            return 1;
        }
    }

    return 0;
}

int jocky_detect_valgrind(void) {
    if (getenv("VALGRIND") != NULL) {
        return 1;
    }

    if (getenv("VALGRIND_OPTS") != NULL) {
        return 1;
    }

    char proc_cmdline[256];
    snprintf(proc_cmdline, sizeof(proc_cmdline), "/proc/%d/cmdline", getpid());

    int fd = open(proc_cmdline, O_RDONLY);
    if (fd < 0) {
        return 0;
    }

    char buffer[4096];
    int bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);

    if (bytes_read <= 0) {
        return 0;
    }

    if (strstr(buffer, "valgrind") != NULL) {
        return 1;
    }

    if (strstr(buffer, "memcheck") != NULL) {
        return 1;
    }

    return 0;
}

int jocky_detect_strace(void) {
    char proc_status[256];
    snprintf(proc_status, sizeof(proc_status), "/proc/%d/status", getpid());

    int fd = open(proc_status, O_RDONLY);
    if (fd < 0) {
        return 0;
    }

    char buffer[4096];
    int bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);

    if (bytes_read <= 0) {
        return 0;
    }

    buffer[bytes_read] = '\0';

    if (strstr(buffer, "TracerPid:\t0") == NULL) {
        const char* marker = "TracerPid:";
        const char* pos = strstr(buffer, marker);
        if (pos != NULL) {
            pos += strlen(marker);
            while (*pos == ' ' || *pos == '\t') pos++;
            if (*pos != '0') {
                return 1;
            }
        }
    }

    return 0;
}

int jocky_detect_gdbserver(void) {
    if (getenv("GDB") != NULL) {
        return 1;
    }

    const char* proc_net[256];
    snprintf((char*)proc_net, sizeof(proc_net), "/proc/net/tcp");

    int fd = open((const char*)proc_net, O_RDONLY);
    if (fd < 0) {
        return 0;
    }

    char buffer[16384];
    int bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);

    if (bytes_read <= 0) {
        return 0;
    }

    if (strstr(buffer, ":2357") != NULL) {
        return 1;
    }

    if (strstr(buffer, ":4444") != NULL) {
        return 1;
    }

    return 0;
}

int jocky_detect_ptrace(void) {
    pid_t pid = fork();
    if (pid < 0) {
        return 0;
    }

    if (pid == 0) {
        if (ptrace(PTRACE_ATTACH, getppid(), 0, 0) == 0) {
            exit(1);
        }
        exit(0);
    }

    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status) && WEXITSTATUS(status) == 1) {
        return 1;
    }

    return 0;
}

int jocky_check_environment_modified(void) {
    if (getenv("LD_PRELOAD") != NULL) {
        return 1;
    }

    if (getenv("DYLD_INSERT_LIBRARIES") != NULL) {
        return 1;
    }

    if (getenv("DYLD_FORCE_FLAT_NAMESPACE") != NULL) {
        return 1;
    }

    return 0;
}

int jocky_check_memory_breakpoints(void) {
    char proc_maps[256];
    snprintf(proc_maps, sizeof(proc_maps), "/proc/%d/maps", getpid());

    int fd = open(proc_maps, O_RDONLY);
    if (fd < 0) {
        return 0;
    }

    char buffer[4096];
    int bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);

    if (bytes_read <= 0) {
        return 0;
    }

    buffer[bytes_read] = '\0';

    int breakpoint_count = 0;
    const char* pos = buffer;
    while ((pos = strchr(pos, 'x')) != NULL) {
        if (strncmp(pos, "xp", 2) == 0 || strncmp(pos, "x-", 2) == 0) {
            breakpoint_count++;
        }
        pos++;
    }

    return breakpoint_count > 5 ? 1 : 0;
}

int jocky_enable_anti_debugger_traps(void) {
    signal(SIGTRAP, signal_handler);

    if (setjmp(jump_buffer) == 0) {
        asm volatile("int $3");
    }

    if (detected_signal == SIGTRAP) {
        return 1;
    }

    return 0;
}

int jocky_enable_anti_trace_traps(void) {
    pid_t tracer_pid = 0;

    char proc_status[256];
    snprintf(proc_status, sizeof(proc_status), "/proc/%d/status", getpid());

    int fd = open(proc_status, O_RDONLY);
    if (fd >= 0) {
        char buffer[4096];
        int bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        close(fd);

        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            const char* marker = "TracerPid:";
            const char* pos = strstr(buffer, marker);
            if (pos != NULL) {
                pos += strlen(marker);
                tracer_pid = atoi(pos);
            }
        }
    }

    return tracer_pid > 0 ? 1 : 0;
}

int jocky_hide_memory_region(void* addr, int size) {
    if (!addr || size <= 0) {
        return -1;
    }

    if (mprotect(addr, size, PROT_NONE) < 0) {
        return -1;
    }

    return 0;
}

int jocky_unhide_memory_region(void* addr, int size) {
    if (!addr || size <= 0) {
        return -1;
    }

    if (mprotect(addr, size, PROT_READ | PROT_WRITE | PROT_EXEC) < 0) {
        return -1;
    }

    return 0;
}

int jocky_is_being_analyzed(void) {
    if (jocky_detect_debugger()) {
        return 1;
    }

    if (jocky_detect_ida()) {
        return 1;
    }

    if (jocky_detect_valgrind()) {
        return 1;
    }

    if (jocky_detect_strace()) {
        return 1;
    }

    if (jocky_detect_gdbserver()) {
        return 1;
    }

    if (jocky_check_environment_modified()) {
        return 1;
    }

    return 0;
}
