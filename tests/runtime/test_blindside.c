#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#include "../../src/runtime/windows/evasion/blindside.h"

static int test_count = 0, test_passed = 0;
#define TEST(n) printf("[TEST] %s... ", n); test_count++
#define PASS() printf("PASS\n"); test_passed++
#define FAIL(m) printf("FAIL: %s\n", m)

void test_find_ntdll(void) {
    TEST("Blindside: Find ntdll");
    void* base = NULL;
    size_t size = 0;
    int r = jocky_blindside_find_ntdll(GetCurrentProcess(), &base, &size);
    if (base && size > 0) PASS(); else FAIL("Should find ntdll");
}

void test_debug_register_set(void) {
    TEST("Blindside: Debug register set");
    CONTEXT ctx = {0};
    int r = jocky_blindside_set_debug_register(&ctx, 0, 0x1000);
    if (r == 0) PASS(); else FAIL("Should set register");
}

int main(void) {
    printf("=== JOCKY Blindside Test Suite ===\n\n");
    test_find_ntdll();
    test_debug_register_set();
    printf("\n=== Results ===\n");
    printf("Passed: %d/%d\n", test_passed, test_count);
    return 0;
}

#else
int main(void) { printf("Blindside tests skipped (Windows only)\n"); return 0; }
#endif
