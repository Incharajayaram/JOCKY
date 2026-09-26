#ifndef JOCKY_PROCESS_H
#define JOCKY_PROCESS_H

#include <stddef.h>
#include <stdint.h>

/* Process information structure */
typedef struct {
    long pid;                   /* Process ID */
    long ppid;                  /* Parent PID */
    long uid;                   /* User ID */
    long gid;                   /* Group ID */
    char name[256];             /* Process name (comm) */
    char state;                 /* Process state (R,S,D,T,W,X,Z) */
    long vm_rss;                /* RSS memory in KB */
    long vm_size;               /* Virtual memory in KB */
    long priority;              /* Priority */
    long nice;                  /* Nice value */
} jocky_process_info_t;

/* ========== PROCESS INFORMATION ========== */

/**
 * Get current process ID.
 * Returns PID (always > 0).
 */
long jocky_getpid(void);

/**
 * Get parent process ID.
 * Returns parent PID.
 */
long jocky_getppid(void);

/**
 * Get real user ID.
 * Returns UID.
 */
long jocky_getuid(void);

/**
 * Get effective user ID.
 * Returns eUID.
 */
long jocky_geteuid(void);

/**
 * Get real group ID.
 * Returns GID.
 */
long jocky_getgid(void);

/**
 * Get effective group ID.
 * Returns eGID.
 */
long jocky_getegid(void);

/**
 * Get current thread ID.
 * Returns TID.
 */
long jocky_gettid(void);

/**
 * Check if process exists.
 * Returns 1 if exists, 0 if not, -1 on error.
 */
int jocky_process_exists(long pid);

/**
 * Get process information.
 * Returns 0 on success, -1 on failure.
 */
int jocky_process_info(long pid, jocky_process_info_t* out);

/**
 * Get executable path of a process.
 * buffer must be at least 256 bytes.
 * Returns 0 on success, -1 on failure.
 */
int jocky_process_get_exe(long pid, char* buffer, size_t size);

/**
 * Get command line of process.
 * buffer must be at least 256 bytes.
 * Arguments separated by null bytes.
 * Returns 0 on success, -1 on failure.
 */
int jocky_process_get_cmdline(long pid, char* buffer, size_t size);

/**
 * Get working directory of process.
 * buffer must be at least 256 bytes.
 * Returns 0 on success, -1 on failure.
 */
int jocky_process_get_cwd(long pid, char* buffer, size_t size);

/**
 * Enumerate all processes.
 * pids: output array of PIDs
 * max_pids: size of array
 * Returns number of processes found, -1 on error.
 */
int jocky_enum_processes(long* pids, size_t max_pids);

/**
 * Send signal to process.
 * Returns 0 on success, -1 on error.
 */
int jocky_kill(long pid, int sig);

/**
 * Send signal to current process.
 * Returns 0 on success, -1 on error.
 */
int jocky_raise(int sig);

/* ========== MEMORY INFORMATION ========== */

/**
 * Get resident set size (RSS) of process in KB.
 * Returns RSS size, 0 on error.
 */
long jocky_process_get_rss(long pid);

/**
 * Get virtual memory size of process in KB.
 * Returns VM size, 0 on error.
 */
long jocky_process_get_vsize(long pid);

#endif // JOCKY_PROCESS_H
