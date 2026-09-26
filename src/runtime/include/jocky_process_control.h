#ifndef JOCKY_PROCESS_CONTROL_H
#define JOCKY_PROCESS_CONTROL_H

#include <stddef.h>

/* Process control flags */
#define JOCKY_WAIT_NOHANG    1
#define JOCKY_WAIT_UNTRACED  2

/* ========== PROCESS CONTROL ========== */

/**
 * Fork current process.
 * Returns child PID in parent, 0 in child, -1 on error.
 */
long jocky_fork(void);

/**
 * Execute program with arguments.
 * argv: NULL-terminated array of arguments
 * Returns -1 on error (never returns on success).
 */
int jocky_execve(const char* path, const char* const* argv, const char* const* envp);

/**
 * Execute program with arguments (searches PATH).
 * argv: NULL-terminated array of arguments
 * Returns -1 on error (never returns on success).
 */
int jocky_execvp(const char* filename, const char* const* argv);

/**
 * Wait for child process.
 * status: Output child exit status (optional, can be NULL)
 * Returns child PID, -1 on error.
 */
long jocky_waitpid(long pid, int* status, int flags);

/**
 * Exit current process.
 * status: Exit code.
 */
void jocky_exit(int status);

/**
 * Get exit status from waitpid status value.
 */
int jocky_wifexited(int status);
int jocky_wexitstatus(int status);

#endif // JOCKY_PROCESS_CONTROL_H
