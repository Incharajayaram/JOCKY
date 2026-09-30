/*
 * Unit Tests for EDR Throttle Profiler
 * Tests callback detection and adaptive mode selection
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <windows.h>
#include "../../src/runtime/windows/evasion/edr_throttle_profiler.h"

static void test_profiler_init()
{
    printf("Testing profiler initialization...\n");

    HANDLE profiler = edr_profiler_init();
    assert(profiler != NULL);
    printf("  Profiler initialized: OK\n");

    edr_profiler_shutdown(profiler);
    printf("  Profiler shutdown: OK\n");

    printf("  PASS\n\n");
}

static void test_no_callbacks()
{
    printf("Testing no EDR detected case...\n");

    HANDLE profiler = edr_profiler_init();
    assert(profiler != NULL);

    EDR_PROFILE_TYPE profile = edr_profiler_analyze(profiler);
    assert(profile == EDR_PROFILE_NOT_DETECTED);
    printf("  No callbacks detected: %d (expected: %d)\n", profile, EDR_PROFILE_NOT_DETECTED);

    EDR_ADAPTIVE_MODE mode;
    int result = edr_profiler_get_adaptive_mode(profiler, &mode);
    assert(result == 0);
    assert(mode == EDR_MODE_NORMAL);
    printf("  Mode with no EDR: %d (expected: %d)\n", mode, EDR_MODE_NORMAL);

    edr_profiler_shutdown(profiler);
    printf("  PASS\n\n");
}

static void test_frequent_callbacks()
{
    printf("Testing frequent callback detection...\n");

    HANDLE profiler = edr_profiler_init();
    assert(profiler != NULL);

    for (int i = 0; i < 20; i++) {
        edr_profiler_record_callback(profiler);
        Sleep(30);
    }

    EDR_PROFILE_TYPE profile = edr_profiler_analyze(profiler);
    assert(profile == EDR_PROFILE_FREQUENT || profile == EDR_PROFILE_NORMAL);
    printf("  Frequent callbacks profile: %d\n", profile);

    EDR_ADAPTIVE_MODE mode;
    int result = edr_profiler_get_adaptive_mode(profiler, &mode);
    assert(result == 0);
    assert(mode == EDR_MODE_STEALTH);
    printf("  Mode for frequent callbacks: %d (expected: %d)\n", mode, EDR_MODE_STEALTH);

    int delay = edr_profiler_get_syscall_delay(mode);
    assert(delay > 50 && delay <= 500);
    printf("  Stealth syscall delay: %dms (expected: 100-500ms)\n", delay);

    edr_profiler_shutdown(profiler);
    printf("  PASS\n\n");
}

static void test_normal_callbacks()
{
    printf("Testing normal callback detection...\n");

    HANDLE profiler = edr_profiler_init();
    assert(profiler != NULL);

    for (int i = 0; i < 15; i++) {
        edr_profiler_record_callback(profiler);
        Sleep(75);
    }

    EDR_PROFILE_TYPE profile = edr_profiler_analyze(profiler);
    printf("  Normal callbacks profile: %d\n", profile);

    EDR_ADAPTIVE_MODE mode;
    int result = edr_profiler_get_adaptive_mode(profiler, &mode);
    assert(result == 0);
    printf("  Mode for normal callbacks: %d\n", mode);

    int delay = edr_profiler_get_syscall_delay(mode);
    printf("  Syscall delay: %dms\n", delay);

    edr_profiler_shutdown(profiler);
    printf("  PASS\n\n");
}

static void test_rare_callbacks()
{
    printf("Testing rare callback detection...\n");

    HANDLE profiler = edr_profiler_init();
    assert(profiler != NULL);

    for (int i = 0; i < 5; i++) {
        edr_profiler_record_callback(profiler);
        Sleep(600);
    }

    EDR_PROFILE_TYPE profile = edr_profiler_analyze(profiler);
    printf("  Rare callbacks profile: %d\n", profile);

    EDR_ADAPTIVE_MODE mode;
    int result = edr_profiler_get_adaptive_mode(profiler, &mode);
    assert(result == 0);
    assert(mode == EDR_MODE_AGGRESSIVE);
    printf("  Mode for rare callbacks: %d (expected: %d)\n", mode, EDR_MODE_AGGRESSIVE);

    int delay = edr_profiler_get_syscall_delay(mode);
    assert(delay == 0);
    printf("  Aggressive syscall delay: %dms (expected: 0ms)\n", delay);

    edr_profiler_shutdown(profiler);
    printf("  PASS\n\n");
}

static void test_behavior_adjustment()
{
    printf("Testing behavior adjustment functions...\n");

    int should_reduce = edr_profiler_should_reduce_syscalls(EDR_MODE_STEALTH);
    assert(should_reduce == 1);
    printf("  Stealth mode reduces syscalls: %d (expected: 1)\n", should_reduce);

    should_reduce = edr_profiler_should_reduce_syscalls(EDR_MODE_NORMAL);
    assert(should_reduce == 0);
    printf("  Normal mode doesn't reduce syscalls: %d (expected: 0)\n", should_reduce);

    int should_batch = edr_profiler_should_batch_operations(EDR_MODE_STEALTH);
    assert(should_batch == 1);
    printf("  Stealth mode batches operations: %d (expected: 1)\n", should_batch);

    int use_indirect = edr_profiler_should_use_indirect_syscalls(EDR_MODE_STEALTH);
    assert(use_indirect == 1);
    printf("  Stealth mode uses indirect syscalls: %d (expected: 1)\n", use_indirect);

    use_indirect = edr_profiler_should_use_indirect_syscalls(EDR_MODE_AGGRESSIVE);
    assert(use_indirect == 0);
    printf("  Aggressive mode doesn't use indirect syscalls: %d (expected: 0)\n", use_indirect);

    printf("  PASS\n\n");
}

static void test_profile_persistence()
{
    printf("Testing profile persistence...\n");

    const char* test_path = "test_edr_profile.bin";

    HANDLE profiler = edr_profiler_init();
    assert(profiler != NULL);

    for (int i = 0; i < 20; i++) {
        edr_profiler_record_callback(profiler);
        Sleep(40);
    }

    EDR_PROFILE_TYPE profile = edr_profiler_analyze(profiler);
    printf("  Original profile type: %d\n", profile);

    int result = edr_profiler_save_profile(profiler, test_path);
    assert(result == 0);
    printf("  Profile saved: OK\n");

    EDR_PROFILE_CONTEXT loaded_ctx = {0};
    result = edr_profiler_load_profile(test_path, &loaded_ctx);
    assert(result == 0);
    assert(loaded_ctx.state == profile);
    printf("  Profile loaded: state = %d (expected: %d)\n", loaded_ctx.state, profile);

    printf("  Callback count: %d\n", loaded_ctx.callback_count);
    printf("  Min interval: %dms\n", loaded_ctx.min_callback_interval);
    printf("  Max interval: %dms\n", loaded_ctx.max_callback_interval);
    printf("  Avg interval: %dms\n", loaded_ctx.avg_callback_interval);

    remove(test_path);
    edr_profiler_shutdown(profiler);
    printf("  PASS\n\n");
}

static void test_callback_intervals()
{
    printf("Testing callback interval tracking...\n");

    HANDLE profiler = edr_profiler_init();
    assert(profiler != NULL);

    edr_profiler_record_callback(profiler);
    Sleep(50);
    edr_profiler_record_callback(profiler);
    Sleep(100);
    edr_profiler_record_callback(profiler);
    Sleep(75);
    edr_profiler_record_callback(profiler);

    EDR_PROFILE_TYPE profile = edr_profiler_analyze(profiler);
    printf("  Profile determined: %d\n", profile);

    EDR_PROFILE_CONTEXT* ctx = (EDR_PROFILE_CONTEXT*)profiler;
    printf("  Min interval: %dms (expected: ~50ms)\n", ctx->min_callback_interval);
    printf("  Max interval: %dms (expected: ~100ms)\n", ctx->max_callback_interval);
    printf("  Avg interval: %dms (expected: ~75ms)\n", ctx->avg_callback_interval);
    printf("  Callback count: %d (expected: 4)\n", ctx->callback_count);

    edr_profiler_shutdown(profiler);
    printf("  PASS\n\n");
}

static void test_edge_cases()
{
    printf("Testing edge cases...\n");

    HANDLE profiler = edr_profiler_init();
    assert(profiler != NULL);

    edr_profiler_record_callback(profiler);
    EDR_PROFILE_TYPE profile = edr_profiler_analyze(profiler);
    printf("  Single callback profile: %d\n", profile);

    edr_profiler_shutdown(NULL);
    printf("  Shutdown with NULL handle: OK\n");

    EDR_ADAPTIVE_MODE mode;
    int result = edr_profiler_get_adaptive_mode(NULL, &mode);
    assert(result == -1);
    printf("  Get mode with NULL profiler: returns error (expected)\n");

    result = edr_profiler_save_profile(NULL, "test.bin");
    assert(result == -1);
    printf("  Save with NULL profiler: returns error (expected)\n");

    result = edr_profiler_load_profile("nonexistent.bin", NULL);
    assert(result == -1);
    printf("  Load with invalid path: returns error (expected)\n");

    printf("  PASS\n\n");
}

int main()
{
    printf("=== EDR Throttle Profiler Tests ===\n\n");

    test_profiler_init();
    test_no_callbacks();
    test_frequent_callbacks();
    test_normal_callbacks();
    test_rare_callbacks();
    test_behavior_adjustment();
    test_profile_persistence();
    test_callback_intervals();
    test_edge_cases();

    printf("=== All Tests Passed ===\n");
    return 0;
}
