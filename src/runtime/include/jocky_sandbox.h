#ifndef JOCKY_SANDBOX_H
#define JOCKY_SANDBOX_H

#include <stdint.h>

/* Process sandbox and isolation for forensic containment */

typedef struct {
    uint32_t pid;
    uint32_t ppid;
    char status[32];
    uint32_t memory_used;
    uint32_t cpu_time_ms;
} JOCKY_SANDBOX_PROCESS;

/* Spawn process in isolated sandbox */
int jocky_sandbox_spawn(
    const char* executable,
    const char* args,
    uint32_t* out_pid);

/* Wait for sandboxed process */
int jocky_sandbox_wait(uint32_t pid, int* out_exit_code);

/* Kill sandboxed process */
int jocky_sandbox_kill(uint32_t pid);

/* Get sandbox process status */
int jocky_sandbox_get_status(uint32_t pid, JOCKY_SANDBOX_PROCESS* out_status);

/* Set process resource limits */
int jocky_sandbox_set_limits(
    uint32_t pid,
    uint32_t max_memory,
    uint32_t max_cpu_time_ms,
    uint32_t max_file_size);

/* Monitor process for suspicious activity */
int jocky_sandbox_monitor(uint32_t pid, int* out_detected_anomaly);

/* Export sandbox audit trail */
int jocky_sandbox_export_trace(uint32_t pid, const char* filename);

#endif
