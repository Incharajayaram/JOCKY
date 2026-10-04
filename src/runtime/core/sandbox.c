#define _GNU_SOURCE
#define _DEFAULT_SOURCE

#include "../include/jocky_sandbox.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <signal.h>
#include <sys/types.h>
#endif

static struct {
    uint32_t pids[256];
    int count;
} g_sandboxes = {0};

int jocky_sandbox_spawn(
    const char* executable,
    const char* args,
    uint32_t* out_pid)
{
    if (!executable || !out_pid) {
        return -1;
    }

#ifdef _WIN32
    STARTUPINFOA si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(si);

    char cmdline[1024];
    snprintf(cmdline, sizeof(cmdline), "%s %s", executable, args ? args : "");

    if (!CreateProcessA(executable, cmdline, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        return -1;
    }

    *out_pid = pi.dwProcessId;

    if (g_sandboxes.count < 256) {
        g_sandboxes.pids[g_sandboxes.count++] = pi.dwProcessId;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
#else
    pid_t pid = fork();
    if (pid == -1) {
        return -1;
    }

    if (pid == 0) {
        /* Child process */
        execl(executable, executable, args, (char*)NULL);
        exit(1);
    }

    *out_pid = (uint32_t)pid;

    if (g_sandboxes.count < 256) {
        g_sandboxes.pids[g_sandboxes.count++] = pid;
    }

    return 0;
#endif
}

int jocky_sandbox_wait(uint32_t pid, int* out_exit_code)
{
    if (!out_exit_code) {
        return -1;
    }

#ifdef _WIN32
    HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!proc) {
        return -1;
    }

    WaitForSingleObject(proc, INFINITE);

    DWORD exit_code;
    if (!GetExitCodeProcess(proc, &exit_code)) {
        CloseHandle(proc);
        return -1;
    }

    *out_exit_code = (int)exit_code;
    CloseHandle(proc);
    return 0;
#else
    int status;
    pid_t result = waitpid((pid_t)pid, &status, 0);
    if (result == -1) {
        return -1;
    }

    *out_exit_code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    return 0;
#endif
}

int jocky_sandbox_kill(uint32_t pid)
{
#ifdef _WIN32
    HANDLE proc = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (!proc) {
        return -1;
    }

    if (!TerminateProcess(proc, 1)) {
        CloseHandle(proc);
        return -1;
    }

    CloseHandle(proc);
    return 0;
#else
    if (kill((pid_t)pid, SIGKILL) == -1) {
        return -1;
    }
    return 0;
#endif
}

int jocky_sandbox_get_status(uint32_t pid, JOCKY_SANDBOX_PROCESS* out_status)
{
    if (!out_status) {
        return -1;
    }

    memset(out_status, 0, sizeof(*out_status));
    out_status->pid = pid;
    strncpy(out_status->status, "running", sizeof(out_status->status) - 1);
    out_status->memory_used = 0;  /* Would read from /proc/[pid]/status on Linux */
    out_status->cpu_time_ms = 0;

    return 0;
}

int jocky_sandbox_set_limits(
    uint32_t pid,
    uint32_t max_memory,
    uint32_t max_cpu_time_ms,
    uint32_t max_file_size)
{
#ifndef _WIN32
    struct rlimit limits;

    /* Memory limit */
    if (max_memory > 0) {
        limits.rlim_cur = max_memory;
        limits.rlim_max = max_memory;
        if (prlimit((pid_t)pid, RLIMIT_AS, &limits, NULL) == -1) {
            return -1;
        }
    }

    /* CPU time limit */
    if (max_cpu_time_ms > 0) {
        uint32_t seconds = max_cpu_time_ms / 1000;
        limits.rlim_cur = seconds;
        limits.rlim_max = seconds;
        if (prlimit((pid_t)pid, RLIMIT_CPU, &limits, NULL) == -1) {
            return -1;
        }
    }

    /* File size limit */
    if (max_file_size > 0) {
        limits.rlim_cur = max_file_size;
        limits.rlim_max = max_file_size;
        if (prlimit((pid_t)pid, RLIMIT_FSIZE, &limits, NULL) == -1) {
            return -1;
        }
    }

    return 0;
#else
    (void)pid;
    (void)max_memory;
    (void)max_cpu_time_ms;
    (void)max_file_size;
    return -1;
#endif
}

int jocky_sandbox_monitor(uint32_t pid, int* out_detected_anomaly)
{
    if (!out_detected_anomaly) {
        return -1;
    }

    *out_detected_anomaly = 0;  /* Would monitor syscalls, detect suspicious behavior */
    return 0;
}

int jocky_sandbox_export_trace(uint32_t pid, const char* filename)
{
    if (!filename) {
        return -1;
    }

    FILE* f = fopen(filename, "w");
    if (!f) {
        return -1;
    }

    fprintf(f, "sandbox_pid=%u\n", pid);
    fprintf(f, "trace_exported=1\n");

    fclose(f);
    return 0;
}
