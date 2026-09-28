#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../src/runtime/include/jocky_lkm.h"

static int test_count = 0;
static int test_passed = 0;

#define TEST(name) do { printf("[TEST] %s... ", name); test_count++; } while(0)
#define PASS() do { printf("PASS\n"); test_passed++; } while(0)
#define FAIL(msg) do { printf("FAIL: %s\n", msg); } while(0)

void test_lkm_get_syscall_table(void)
{
    TEST("LKM: Get syscall table address");

    uint64_t table_addr;
    int result = jocky_lkm_get_syscall_table(&table_addr);

    if (result == 0 && table_addr > 0) {
        PASS();
    } else {
        FAIL("Should get syscall table address");
    }
}

void test_lkm_get_syscall_table_null(void)
{
    TEST("LKM: Get syscall table (NULL out)");

    int result = jocky_lkm_get_syscall_table(NULL);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject NULL output");
    }
}

void test_lkm_query_unloaded(void)
{
    TEST("LKM: Query unloaded module");

    JOCKY_LKM module;
    memset(&module, 0, sizeof(module));
    module.loaded = 0;

    int result = jocky_lkm_query(&module);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should report unloaded module");
    }
}

void test_lkm_hook_syscall_invalid(void)
{
    TEST("LKM: Hook syscall (invalid module)");

    JOCKY_LKM module;
    memset(&module, 0, sizeof(module));
    module.loaded = 0;

    int dummy = 0;
    int result = jocky_lkm_hook_syscall(&module, 1, &dummy);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject unloaded module");
    }
}

void test_lkm_hook_syscall_null_func(void)
{
    TEST("LKM: Hook syscall (NULL function)");

    JOCKY_LKM module;
    memset(&module, 0, sizeof(module));
    module.loaded = 1;

    int result = jocky_lkm_hook_syscall(&module, 1, NULL);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject NULL hook function");
    }
}

void test_lkm_unhook_syscall(void)
{
    TEST("LKM: Unhook syscall");

    int result = jocky_lkm_unhook_syscall(1);

    if (result == 0) {
        PASS();
    } else {
        FAIL("Should unhook syscall");
    }
}

void test_lkm_load_nonexistent(void)
{
    TEST("LKM: Load nonexistent module");

    JOCKY_LKM module;
    int result = jocky_lkm_load("/nonexistent/module.ko", &module);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should fail on nonexistent file");
    }
}

void test_lkm_load_null_path(void)
{
    TEST("LKM: Load with NULL path");

    JOCKY_LKM module;
    int result = jocky_lkm_load(NULL, &module);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject NULL path");
    }
}

void test_lkm_unload_null(void)
{
    TEST("LKM: Unload NULL module");

    int result = jocky_lkm_unload(NULL);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject NULL module");
    }
}

void test_lkm_execute_invalid(void)
{
    TEST("LKM: Execute on unloaded module");

    JOCKY_LKM module;
    memset(&module, 0, sizeof(module));
    module.loaded = 0;

    int result = jocky_lkm_execute(&module, "some_func");

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject unloaded module");
    }
}

int main(void)
{
    printf("=== JOCKY LKM Loader Test Suite ===\n\n");

    test_lkm_get_syscall_table();
    test_lkm_get_syscall_table_null();
    test_lkm_query_unloaded();
    test_lkm_hook_syscall_invalid();
    test_lkm_hook_syscall_null_func();
    test_lkm_unhook_syscall();
    test_lkm_load_nonexistent();
    test_lkm_load_null_path();
    test_lkm_unload_null();
    test_lkm_execute_invalid();

    printf("\n=== Results ===\n");
    printf("Passed: %d/%d\n", test_passed, test_count);

    return (test_passed == test_count) ? 0 : 1;
}
