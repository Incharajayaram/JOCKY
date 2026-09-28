#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../src/runtime/include/jocky_sandbox.h"

static int test_count = 0;
static int test_passed = 0;

#define TEST(name) do { printf("[TEST] %s... ", name); test_count++; } while(0)
#define PASS() do { printf("PASS\n"); test_passed++; } while(0)
#define FAIL(msg) do { printf("FAIL: %s\n", msg); } while(0)

void test_sandbox_spawn_invalid_exe(void)
{
    TEST("Sandbox: Spawn invalid executable");

    uint32_t pid;
    int result = jocky_sandbox_spawn("/nonexistent/binary", NULL, &pid);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should fail on nonexistent executable");
    }
}

void test_sandbox_spawn_null_exe(void)
{
    TEST("Sandbox: Spawn with NULL executable");

    uint32_t pid;
    int result = jocky_sandbox_spawn(NULL, NULL, &pid);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject NULL executable");
    }
}

void test_sandbox_spawn_null_pid(void)
{
    TEST("Sandbox: Spawn with NULL pid output");

    int result = jocky_sandbox_spawn("/bin/echo", NULL, NULL);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject NULL PID output");
    }
}

void test_sandbox_wait_invalid_pid(void)
{
    TEST("Sandbox: Wait for invalid PID");

    int exit_code;
    int result = jocky_sandbox_wait(0xDEADBEEF, &exit_code);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should fail for invalid PID");
    }
}

void test_sandbox_wait_null_exit_code(void)
{
    TEST("Sandbox: Wait with NULL exit code");

    int result = jocky_sandbox_wait(1234, NULL);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject NULL exit code");
    }
}

void test_sandbox_kill_invalid_pid(void)
{
    TEST("Sandbox: Kill invalid PID");

    int result = jocky_sandbox_kill(0xDEADBEEF);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should fail for invalid PID");
    }
}

void test_sandbox_get_status(void)
{
    TEST("Sandbox: Get process status");

    JOCKY_SANDBOX_PROCESS status;
    int result = jocky_sandbox_get_status(1234, &status);

    if (result == 0 && status.pid == 1234) {
        PASS();
    } else {
        FAIL("Should get process status");
    }
}

void test_sandbox_get_status_null(void)
{
    TEST("Sandbox: Get status with NULL output");

    int result = jocky_sandbox_get_status(1234, NULL);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject NULL output");
    }
}

void test_sandbox_set_limits_memory(void)
{
    TEST("Sandbox: Set memory limit");

    uint32_t max_memory = 1024 * 1024 * 100;  /* 100 MB */
    int result = jocky_sandbox_set_limits(1234, max_memory, 0, 0);

    #ifdef __unix__
    if (result == -1) {  /* Expected: no such process */
        PASS();
    } else {
        FAIL("Should handle memory limit");
    }
    #else
    if (result == -1) {
        PASS();
    } else {
        FAIL("Not supported on this platform");
    }
    #endif
}

void test_sandbox_set_limits_cpu(void)
{
    TEST("Sandbox: Set CPU time limit");

    uint32_t max_cpu_ms = 60000;  /* 60 seconds */
    int result = jocky_sandbox_set_limits(1234, 0, max_cpu_ms, 0);

    #ifdef __unix__
    if (result == -1) {  /* Expected: no such process */
        PASS();
    } else {
        FAIL("Should handle CPU limit");
    }
    #else
    if (result == -1) {
        PASS();
    } else {
        FAIL("Not supported on this platform");
    }
    #endif
}

void test_sandbox_monitor(void)
{
    TEST("Sandbox: Monitor process");

    int anomaly = 0;
    int result = jocky_sandbox_monitor(1234, &anomaly);

    if (result == 0) {
        PASS();
    } else {
        FAIL("Should monitor process");
    }
}

void test_sandbox_monitor_null(void)
{
    TEST("Sandbox: Monitor with NULL anomaly");

    int result = jocky_sandbox_monitor(1234, NULL);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject NULL anomaly output");
    }
}

void test_sandbox_export_trace(void)
{
    TEST("Sandbox: Export audit trace");

    const char* filename = "/tmp/jocky_sandbox_trace.txt";
    int result = jocky_sandbox_export_trace(1234, filename);

    if (result == 0) {
        PASS();
        remove(filename);
    } else {
        FAIL("Should export trace");
    }
}

void test_sandbox_export_trace_null_filename(void)
{
    TEST("Sandbox: Export trace with NULL filename");

    int result = jocky_sandbox_export_trace(1234, NULL);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject NULL filename");
    }
}

int main(void)
{
    printf("=== JOCKY Sandbox Test Suite ===\n\n");

    test_sandbox_spawn_invalid_exe();
    test_sandbox_spawn_null_exe();
    test_sandbox_spawn_null_pid();
    test_sandbox_wait_invalid_pid();
    test_sandbox_wait_null_exit_code();
    test_sandbox_kill_invalid_pid();
    test_sandbox_get_status();
    test_sandbox_get_status_null();
    test_sandbox_set_limits_memory();
    test_sandbox_set_limits_cpu();
    test_sandbox_monitor();
    test_sandbox_monitor_null();
    test_sandbox_export_trace();
    test_sandbox_export_trace_null_filename();

    printf("\n=== Results ===\n");
    printf("Passed: %d/%d\n", test_passed, test_count);

    return (test_passed == test_count) ? 0 : 1;
}
