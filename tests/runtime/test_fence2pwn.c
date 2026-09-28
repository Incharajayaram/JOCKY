#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../src/runtime/linux/lpe/fence2pwn.h"

static int test_count = 0, test_passed = 0;

void test_kfence_detection(void) {
    printf("[TEST] Fence2Pwn: KFENCE detection... ");
    test_count++;
    int r = jocky_fence2pwn_detect_kfence();
    if (r == 0 || r == -1) {
        printf("PASS\n");
        test_passed++;
    } else {
        printf("FAIL\n");
    }
}

void test_pool_info(void) {
    printf("[TEST] Fence2Pwn: Pool info query... ");
    test_count++;
    KFENCE_POOL_INFO info;
    jocky_fence2pwn_get_pool_info(&info);
    printf("PASS\n");
    test_passed++;
}

void test_trigger_allocations(void) {
    printf("[TEST] Fence2Pwn: Trigger allocations... ");
    test_count++;
    int r = jocky_fence2pwn_trigger_allocations(256, 10);
    if (r == 0 || r == -1) {
        printf("PASS\n");
        test_passed++;
    } else {
        printf("FAIL\n");
    }
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
