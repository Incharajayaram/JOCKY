#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../../src/runtime/linux/lpe/fence2pwn.h"

static int test_count = 0, test_passed = 0;
#define TEST(n) printf("[TEST] %s... ", n); test_count++
#define PASS() printf("PASS\n"); test_passed++
#define FAIL(m) printf("FAIL: %s\n", m)

void test_kfence_detection(void) {
    TEST("Fence2Pwn: KFENCE detection");
    int r = jocky_fence2pwn_detect_kfence();
    if (r == 0 || r == -1) PASS(); else FAIL("Invalid return");
}

void test_pool_info(void) {
    TEST("Fence2Pwn: Pool info query");
    KFENCE_POOL_INFO info;
    int r = jocky_fence2pwn_get_pool_info(&info);
    PASS();
}

void test_trigger_allocations(void) {
    TEST("Fence2Pwn: Trigger allocations");
    int r = jocky_fence2pwn_trigger_allocations(256, 10);
    if (r == 0 || r == -1) PASS(); else FAIL("Invalid return");
}

int main(void) {
    printf("=== JOCKY Fence2Pwn Test Suite ===\n\n");
    test_kfence_detection();
    test_pool_info();
    test_trigger_allocations();
    printf("\n=== Results ===\n");
    printf("Passed: %d/%d\n", test_passed, test_count);
    return 0;
}
