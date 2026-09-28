#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../src/runtime/include/jocky_ebpf.h"

static int test_count = 0;
static int test_passed = 0;

#define TEST(name) do { printf("[TEST] %s... ", name); test_count++; } while(0)
#define PASS() do { printf("PASS\n"); test_passed++; } while(0)
#define FAIL(msg) do { printf("FAIL: %s\n", msg); } while(0)

void test_ebpf_load_valid(void)
{
    TEST("eBPF: Load valid bytecode");

    uint8_t bytecode[256];
    memset(bytecode, 0xFF, sizeof(bytecode));  /* Dummy bytecode */

    JOCKY_EBPF_PROGRAM program;
    int result = jocky_ebpf_load("test_prog", bytecode, sizeof(bytecode), &program);

    if (result == 0 && program.program_fd >= 0) {
        PASS();
    } else {
        FAIL("Should load eBPF program");
    }
}

void test_ebpf_load_null_bytecode(void)
{
    TEST("eBPF: Load NULL bytecode");

    JOCKY_EBPF_PROGRAM program;
    int result = jocky_ebpf_load("test", NULL, 100, &program);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject NULL bytecode");
    }
}

void test_ebpf_load_zero_size(void)
{
    TEST("eBPF: Load zero-size bytecode");

    uint8_t bytecode[1];
    JOCKY_EBPF_PROGRAM program;
    int result = jocky_ebpf_load("test", bytecode, 0, &program);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject zero-size bytecode");
    }
}

void test_ebpf_load_null_output(void)
{
    TEST("eBPF: Load with NULL output");

    uint8_t bytecode[256];
    memset(bytecode, 0xFF, sizeof(bytecode));

    int result = jocky_ebpf_load("test", bytecode, sizeof(bytecode), NULL);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject NULL output");
    }
}

void test_ebpf_attach_valid(void)
{
    TEST("eBPF: Attach to syscall");

    uint8_t bytecode[256];
    memset(bytecode, 0xFF, sizeof(bytecode));

    JOCKY_EBPF_PROGRAM program;
    jocky_ebpf_load("test", bytecode, sizeof(bytecode), &program);

    int result = jocky_ebpf_attach(&program, "tracepoint:syscalls:sys_enter_open");

    if (result == 0 && program.attached == 1) {
        PASS();
    } else {
        FAIL("Should attach program");
    }
}

void test_ebpf_attach_null_point(void)
{
    TEST("eBPF: Attach to NULL point");

    JOCKY_EBPF_PROGRAM program;
    memset(&program, 0, sizeof(program));
    program.program_fd = 3;

    int result = jocky_ebpf_attach(&program, NULL);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject NULL attach point");
    }
}

void test_ebpf_detach_not_attached(void)
{
    TEST("eBPF: Detach not-attached program");

    JOCKY_EBPF_PROGRAM program;
    memset(&program, 0, sizeof(program));
    program.attached = 0;

    int result = jocky_ebpf_detach(&program);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject detach on non-attached");
    }
}

void test_ebpf_detach_valid(void)
{
    TEST("eBPF: Detach attached program");

    JOCKY_EBPF_PROGRAM program;
    memset(&program, 0, sizeof(program));
    program.program_fd = 3;
    program.attached = 1;

    int result = jocky_ebpf_detach(&program);

    if (result == 0 && program.attached == 0) {
        PASS();
    } else {
        FAIL("Should detach program");
    }
}

void test_ebpf_map_update(void)
{
    TEST("eBPF: Update map entry");

    JOCKY_EBPF_PROGRAM program;
    memset(&program, 0, sizeof(program));
    program.map_fd = 4;

    uint64_t key = 1;
    uint64_t value = 100;

    int result = jocky_ebpf_map_update(&program, &key, &value);

    if (result == 0) {
        PASS();
    } else {
        FAIL("Should update map");
    }
}

void test_ebpf_map_update_null_key(void)
{
    TEST("eBPF: Update map with NULL key");

    JOCKY_EBPF_PROGRAM program;
    memset(&program, 0, sizeof(program));

    uint64_t value = 100;
    int result = jocky_ebpf_map_update(&program, NULL, &value);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject NULL key");
    }
}

void test_ebpf_map_lookup(void)
{
    TEST("eBPF: Lookup map entry");

    JOCKY_EBPF_PROGRAM program;
    memset(&program, 0, sizeof(program));
    program.map_fd = 4;

    uint64_t key = 1;
    uint64_t value;

    int result = jocky_ebpf_map_lookup(&program, &key, &value);

    if (result == 0) {
        PASS();
    } else {
        FAIL("Should lookup map entry");
    }
}

void test_ebpf_query(void)
{
    TEST("eBPF: Query program status");

    JOCKY_EBPF_PROGRAM program;
    memset(&program, 0, sizeof(program));
    program.program_fd = 3;

    int result = jocky_ebpf_query(&program);

    if (result == 0) {
        PASS();
    } else {
        FAIL("Should query program");
    }
}

void test_ebpf_unload(void)
{
    TEST("eBPF: Unload program");

    JOCKY_EBPF_PROGRAM program;
    memset(&program, 0, sizeof(program));
    program.program_fd = 3;
    program.map_fd = 4;
    program.attached = 1;

    int result = jocky_ebpf_unload(&program);

    if (result == 0 && program.attached == 0) {
        PASS();
    } else {
        FAIL("Should unload program");
    }
}

int main(void)
{
    printf("=== JOCKY eBPF Loader Test Suite ===\n\n");

    test_ebpf_load_valid();
    test_ebpf_load_null_bytecode();
    test_ebpf_load_zero_size();
    test_ebpf_load_null_output();
    test_ebpf_attach_valid();
    test_ebpf_attach_null_point();
    test_ebpf_detach_not_attached();
    test_ebpf_detach_valid();
    test_ebpf_map_update();
    test_ebpf_map_update_null_key();
    test_ebpf_map_lookup();
    test_ebpf_query();
    test_ebpf_unload();

    printf("\n=== Results ===\n");
    printf("Passed: %d/%d\n", test_passed, test_count);

    return (test_passed == test_count) ? 0 : 1;
}
