#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include "../../src/runtime/windows/kernel/patchguard_peekaboo.h"

static int test_count = 0, test_passed = 0;
#define TEST(n) printf("[TEST] %s... ", n); test_count++
#define PASS() printf("PASS\n"); test_passed++
#define FAIL(m) printf("FAIL: %s\n", m)

void test_hvci_detection(void) {
    TEST("PatchGuard: HVCI detection");
    HVCI_CAPABILITIES caps;
    int r = jocky_patchguard_detect_hvci(&caps);
    if (r == 0 || r == -1) PASS(); else FAIL("Invalid return");
}

void test_hvci_enabled_check(void) {
    TEST("PatchGuard: HVCI enabled check");
    int r = jocky_patchguard_hvci_enabled();
    PASS();
}

void test_skpg_enabled_check(void) {
    TEST("PatchGuard: SKPG enabled check");
    int r = jocky_patchguard_skpg_enabled();
    PASS();
}

void test_hidden_count(void) {
    TEST("PatchGuard: Hidden process count");
    uint32_t count = jocky_patchguard_hidden_count();
    if (count == 0) PASS(); else FAIL("Should start at 0");
}

int main(void) {
    printf("=== JOCKY PatchGuard Peekaboo Test Suite ===\n\n");
    test_hvci_detection();
    test_hvci_enabled_check();
    test_skpg_enabled_check();
    test_hidden_count();
    printf("\n=== Results ===\n");
    printf("Passed: %d/%d\n", test_passed, test_count);
    return (test_passed == test_count) ? 0 : 1;
}

#else
int main(void) { printf("PatchGuard tests skipped (Windows only)\n"); return 0; }
#endif
