/*
 * EDR Throttle Profiler - Header
 * Dynamic EDR Detection and Adaptive Payload Behavior
 */

#ifndef EDR_THROTTLE_PROFILER_H
#define EDR_THROTTLE_PROFILER_H

#include <windows.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    EDR_PROFILE_NOT_DETECTED,
    EDR_PROFILE_FREQUENT,
    EDR_PROFILE_NORMAL,
    EDR_PROFILE_THROTTLED,
    EDR_PROFILE_RARE,
    EDR_PROFILE_UNKNOWN
} EDR_PROFILE_TYPE;

typedef enum {
    EDR_MODE_STEALTH,
    EDR_MODE_NORMAL,
    EDR_MODE_AGGRESSIVE
} EDR_ADAPTIVE_MODE;

typedef struct {
    DWORD magic;
    DWORD version;
    int initialized;
    FILETIME start_time;
    EDR_PROFILE_TYPE state;
    int callback_count;
    DWORD min_callback_interval;
    DWORD max_callback_interval;
    DWORD avg_callback_interval;
    DWORD throttle_confidence;
    char* profile_cache_path;
} EDR_PROFILE_CONTEXT;

HANDLE edr_profiler_init();

void edr_profiler_shutdown(HANDLE profiler);

void edr_profiler_record_callback(HANDLE profiler);

EDR_PROFILE_TYPE edr_profiler_analyze(HANDLE profiler);

int edr_profiler_get_adaptive_mode(HANDLE profiler, EDR_ADAPTIVE_MODE* out_mode);

int edr_profiler_save_profile(HANDLE profiler, const char* profile_path);

int edr_profiler_load_profile(const char* profile_path, EDR_PROFILE_CONTEXT* out_profile);

int edr_profiler_adjust_behavior(EDR_ADAPTIVE_MODE mode);

int edr_profiler_get_syscall_delay(EDR_ADAPTIVE_MODE mode);

int edr_profiler_should_reduce_syscalls(EDR_ADAPTIVE_MODE mode);

int edr_profiler_should_batch_operations(EDR_ADAPTIVE_MODE mode);

int edr_profiler_should_use_indirect_syscalls(EDR_ADAPTIVE_MODE mode);

#ifdef __cplusplus
}
#endif

#endif /* EDR_THROTTLE_PROFILER_H */
