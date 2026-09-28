#include "../include/jocky_anti_analysis.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void test_detect_debugger() {
    printf("Test: Detect debugger (PTRACE_TRACEME)\n");

    int result = jocky_detect_debugger();
    if (result) {
        printf("DETECTED: Debugger attached\n");
    } else {
        printf("PASS (No debugger detected)\n");
    }
}

void test_detect_ida() {
    printf("Test: Detect IDA disassembler\n");

    int result = jocky_detect_ida();
    if (result) {
        printf("DETECTED: IDA found in process maps\n");
    } else {
        printf("PASS (IDA not detected)\n");
    }
}

void test_detect_valgrind() {
    printf("Test: Detect Valgrind memory tool\n");

    int result = jocky_detect_valgrind();
    if (result) {
        printf("DETECTED: Valgrind running\n");
    } else {
        printf("PASS (Valgrind not detected)\n");
    }
}

void test_detect_strace() {
    printf("Test: Detect strace syscall tracer\n");

    int result = jocky_detect_strace();
    if (result) {
        printf("DETECTED: strace tracing process\n");
    } else {
        printf("PASS (strace not detected)\n");
    }
}

void test_detect_gdbserver() {
    printf("Test: Detect gdbserver debugger\n");

    int result = jocky_detect_gdbserver();
    if (result) {
        printf("DETECTED: gdbserver listening\n");
    } else {
        printf("PASS (gdbserver not detected)\n");
    }
}

void test_check_environment() {
    printf("Test: Check for environment modifications\n");

    int result = jocky_check_environment_modified();
    if (result) {
        printf("DETECTED: Environment variables modified\n");
    } else {
        printf("PASS (Environment clean)\n");
    }
}

void test_check_memory_breakpoints() {
    printf("Test: Check for memory breakpoints\n");

    int result = jocky_check_memory_breakpoints();
    if (result) {
        printf("DETECTED: Suspicious memory patterns\n");
    } else {
        printf("PASS (No breakpoints detected)\n");
    }
}

void test_memory_protection() {
    printf("Test: Memory region protection\n");

    void* test_region = malloc(4096);
    if (!test_region) {
        printf("FAIL: Could not allocate memory\n");
        return;
    }

    if (jocky_hide_memory_region(test_region, 4096) != 0) {
        printf("FAIL: Could not hide region\n");
        free(test_region);
        return;
    }

    if (jocky_unhide_memory_region(test_region, 4096) != 0) {
        printf("FAIL: Could not unhide region\n");
        free(test_region);
        return;
    }

    free(test_region);
    printf("PASS\n");
}

void test_is_being_analyzed() {
    printf("Test: Composite analysis detection\n");

    int result = jocky_is_being_analyzed();
    if (result) {
        printf("DETECTED: Process being analyzed\n");
    } else {
        printf("PASS (No analysis detected)\n");
    }
}

void test_null_protection() {
    printf("Test: Null pointer handling\n");

    if (jocky_hide_memory_region(NULL, 1024) == 0) {
        printf("FAIL: Should reject null pointer\n");
        return;
    }

    if (jocky_unhide_memory_region(NULL, 1024) == 0) {
        printf("FAIL: Should reject null pointer\n");
        return;
    }

    printf("PASS\n");
}

int main() {
    printf("Anti-Analysis Detection Tests\n");
    printf("=============================\n\n");

    test_detect_debugger();
    test_detect_ida();
    test_detect_valgrind();
    test_detect_strace();
    test_detect_gdbserver();
    test_check_environment();
    test_check_memory_breakpoints();
    test_memory_protection();
    test_is_being_analyzed();
    test_null_protection();

    printf("\n=============================\n");
    printf("All tests completed\n");

    return 0;
}
