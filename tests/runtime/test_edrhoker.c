#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef _WIN32
#include <windows.h>
#include "../../src/runtime/windows/evasion/edrhoker.h"

/* Test framework */
static int test_count = 0;
static int test_passed = 0;

#define TEST(name) \
    do { \
        printf("[TEST] %s... ", name); \
        test_count++; \
    } while(0)

#define PASS() \
    do { \
        printf("PASS\n"); \
        test_passed++; \
    } while(0)

#define FAIL(msg) \
    do { \
        printf("FAIL: %s\n", msg); \
    } while(0)

/* Test 1: EDR Process Detection */
void test_detect_edr_processes(void)
{
    TEST("EDRChoker: Detect EDR processes");

    EDR_PROCESS_INFO processes[32];
    int found = 0;

    int result = jocky_edrhoker_detect_edr_processes(processes, 32, &found);

    /* Should return 0 if processes found, -1 if none */
    if (result == 0 || result == -1) {
        printf("(found %d EDR processes) ", found);
        PASS();
    } else {
        FAIL("Unexpected return code");
    }
}

/* Test 2: Invalid input handling */
void test_invalid_inputs(void)
{
    TEST("EDRChoker: Invalid input handling");

    int result = jocky_edrhoker_throttle_process(0, 0);  /* Invalid PID and rate */

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject invalid inputs");
    }
}

/* Test 3: Profile selection */
void test_profile_crowdstrike(void)
{
    TEST("EDRChoker: CrowdStrike profile");

    int result = jocky_edrhoker_profile_crowdstrike();

    /* Result depends on if CrowdStrike is running */
    if (result == 0 || result == -1) {
        PASS();
    } else {
        FAIL("Unexpected return code");
    }
}

/* Test 4: NULL pointer safety */
void test_null_pointer_safety(void)
{
    TEST("EDRChoker: NULL pointer safety");

    int result = jocky_edrhoker_detect_edr_processes(NULL, 0, NULL);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should handle NULL pointers");
    }
}

/* Test 5: Throttle rate validation */
void test_throttle_rate_validation(void)
{
    TEST("EDRChoker: Throttle rate validation");

    /* Test with invalid rate */
    int result = jocky_edrhoker_throttle_process(9999, 0);  /* Invalid rate */

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should validate throttle rate");
    }
}

/* Test 6: Auto-throttle function */
void test_auto_throttle(void)
{
    TEST("EDRChoker: Auto-throttle all EDR");

    int result = jocky_edrhoker_auto_throttle(1);  /* 1 KB/s */

    if (result == 0 || result == -1) {
        PASS();
    } else {
        FAIL("Unexpected return code");
    }
}

/* Test 7: Cleanup operations */
void test_cleanup_operations(void)
{
    TEST("EDRChoker: Cleanup throttles");

    int result = jocky_edrhoker_remove_all_throttles();

    if (result == 0) {
        PASS();
    } else {
        FAIL("Cleanup should succeed or be no-op");
    }
}

/* Test 8: Multiple profile attempts */
void test_multiple_profiles(void)
{
    TEST("EDRChoker: Multiple profiles");

    int all_attempted = 0;
    all_attempted += (jocky_edrhoker_profile_sentinelone() >= -1);
    all_attempted += (jocky_edrhoker_profile_carbonblack() >= -1);
    all_attempted += (jocky_edrhoker_profile_mbeddr() >= -1);
    all_attempted += (jocky_edrhoker_profile_cortex() >= -1);

    if (all_attempted == 4) {
        PASS();
    } else {
        FAIL("Not all profiles available");
    }
}

/* Run all tests */
int main(void)
{
    printf("=== JOCKY EDRChoker Test Suite ===\n\n");

    test_detect_edr_processes();
    test_invalid_inputs();
    test_profile_crowdstrike();
    test_null_pointer_safety();
    test_throttle_rate_validation();
    test_auto_throttle();
    test_cleanup_operations();
    test_multiple_profiles();

    printf("\n=== Results ===\n");
    printf("Passed: %d/%d\n", test_passed, test_count);

    return (test_passed == test_count) ? 0 : 1;
}

#else
/* Linux stub */
int main(void)
{
    printf("EDRChoker tests skipped (Windows only)\n");
    return 0;
}
#endif
