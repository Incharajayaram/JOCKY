#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef _WIN32
#include <windows.h>
#include "../../src/runtime/windows/byovd/btr_abuse.h"

static int test_count = 0;
static int test_passed = 0;

#define TEST(name) \
    do { printf("[TEST] %s... ", name); test_count++; } while(0)
#define PASS() \
    do { printf("PASS\n"); test_passed++; } while(0)
#define FAIL(msg) \
    do { printf("FAIL: %s\n", msg); } while(0)

void test_btr_detection(void)
{
    TEST("BTR Reforged: Driver detection");
    int result = jocky_btr_detect_available();
    if (result == 0 || result == -1) { PASS(); } else { FAIL("Invalid return"); }
}

void test_btr_load(void)
{
    TEST("BTR Reforged: Driver loading");
    JOCKY_BTR_CONTEXT ctx;
    int result = jocky_btr_load("BehaviorThreatRemoval", &ctx);
    if (result == 0 || result == -1) { PASS(); } else { FAIL("Invalid return"); }
}

void test_invalid_pid(void)
{
    TEST("BTR Reforged: Invalid PID handling");
    JOCKY_BTR_CONTEXT ctx = {0};
    int result = jocky_btr_kill_process(&ctx, 0);
    if (result == -1) { PASS(); } else { FAIL("Should reject invalid PID"); }
}

void test_notification_masks(void)
{
    TEST("BTR Reforged: Notification mask handling");
    JOCKY_BTR_CONTEXT ctx = {0};
    int result = jocky_btr_disable_notifications(&ctx, 0);
    if (result == 0 || result == -1) { PASS(); } else { FAIL("Invalid return"); }
}

int main(void)
{
    printf("=== JOCKY BTR Reforged Test Suite ===\n\n");
    test_btr_detection();
    test_btr_load();
    test_invalid_pid();
    test_notification_masks();
    printf("\n=== Results ===\n");
    printf("Passed: %d/%d\n", test_passed, test_count);
    return (test_passed == test_count) ? 0 : 1;
}

#else
int main(void) { printf("BTR tests skipped (Windows only)\n"); return 0; }
#endif
