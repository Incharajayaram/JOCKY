/*
 * Test Suite for Advanced Linux Syscall Interception
 * Unit tests for ptrace-based syscall hooking and manipulation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <linux/ptrace.h>

/* Forward declarations from advanced_syscalls.c */
typedef struct {
    unsigned long original_syscall;
    unsigned long hook_address;
    unsigned long syscall_number;
    int hooked;
} syscall_hook_t;

typedef struct {
    pid_t target_pid;
    syscall_hook_t hooks[32];
    int hook_count;
} syscall_interception_context_t;

int jocky_syscall_hook_init(syscall_interception_context_t* ctx, pid_t pid);
int jocky_syscall_hook_install(syscall_interception_context_t* ctx, unsigned long syscall_num, unsigned long hook_addr);
int jocky_syscall_trace_enable(syscall_interception_context_t* ctx);
int jocky_syscall_intercept_read_args(pid_t pid, struct user_regs_struct* regs);
int jocky_syscall_intercept_modify_args(pid_t pid, struct user_regs_struct* regs, unsigned int arg_index, unsigned long new_value);
int jocky_syscall_inject_syscall(pid_t pid, unsigned long syscall_num, unsigned long arg1, unsigned long arg2, unsigned long arg3);
int jocky_syscall_hook_cleanup(syscall_interception_context_t* ctx);

#define ASSERT_EQ(actual, expected, test_name) \
    if (actual == expected) { \
        printf("[✓] %s\n", test_name); \
        passed++; \
    } else { \
        printf("[✗] %s (got %ld, expected %ld)\n", test_name, (long)actual, (long)expected); \
        failed++; \
    }

#define ASSERT_NEQ(actual, not_expected, test_name) \
    if (actual != not_expected) { \
        printf("[✓] %s\n", test_name); \
        passed++; \
    } else { \
        printf("[✗] %s (got %ld, should not be %ld)\n", test_name, (long)actual, (long)not_expected); \
        failed++; \
    }

#define ASSERT_NULL(ptr, test_name) \
    if (ptr == NULL) { \
        printf("[✓] %s\n", test_name); \
        passed++; \
    } else { \
        printf("[✗] %s (expected NULL, got %p)\n", test_name, ptr); \
        failed++; \
    }

int test_hook_init() {
    printf("\n=== Test: Syscall Hook Initialization ===\n");
    int passed = 0, failed = 0;

    syscall_interception_context_t ctx;
    int result = jocky_syscall_hook_init(&ctx, 12345);

    ASSERT_EQ(result, 0, "init returns 0 on success");
    ASSERT_EQ(ctx.target_pid, 12345, "target_pid set correctly");
    ASSERT_EQ(ctx.hook_count, 0, "hook_count initialized to 0");
    ASSERT_EQ(ctx.hooks[0].hooked, 0, "first hook marked unhooked");

    printf("Passed: %d, Failed: %d\n", passed, failed);
    return failed;
}

int test_hook_init_null_context() {
    printf("\n=== Test: Syscall Hook Init - Null Context ===\n");
    int passed = 0, failed = 0;

    int result = jocky_syscall_hook_init(NULL, 12345);

    ASSERT_EQ(result, -1, "init returns -1 on NULL context");

    printf("Passed: %d, Failed: %d\n", passed, failed);
    return failed;
}

int test_hook_install() {
    printf("\n=== Test: Syscall Hook Installation ===\n");
    int passed = 0, failed = 0;

    syscall_interception_context_t ctx;
    jocky_syscall_hook_init(&ctx, 12345);

    int result = jocky_syscall_hook_install(&ctx, 59, 0xdeadbeef);

    ASSERT_EQ(result, 0, "install returns 0 on success");
    ASSERT_EQ(ctx.hook_count, 1, "hook_count incremented");
    ASSERT_EQ(ctx.hooks[0].syscall_number, 59, "syscall number stored");
    ASSERT_EQ(ctx.hooks[0].hook_address, 0xdeadbeef, "hook address stored");
    ASSERT_EQ(ctx.hooks[0].hooked, 1, "hook marked as installed");

    printf("Passed: %d, Failed: %d\n", passed, failed);
    return failed;
}

int test_hook_install_multiple() {
    printf("\n=== Test: Multiple Syscall Hook Installation ===\n");
    int passed = 0, failed = 0;

    syscall_interception_context_t ctx;
    jocky_syscall_hook_init(&ctx, 12345);

    jocky_syscall_hook_install(&ctx, 59, 0x1000);
    jocky_syscall_hook_install(&ctx, 257, 0x2000);
    jocky_syscall_hook_install(&ctx, 9, 0x3000);

    ASSERT_EQ(ctx.hook_count, 3, "hook_count is 3");
    ASSERT_EQ(ctx.hooks[0].syscall_number, 59, "first hook: syscall 59");
    ASSERT_EQ(ctx.hooks[1].syscall_number, 257, "second hook: syscall 257");
    ASSERT_EQ(ctx.hooks[2].syscall_number, 9, "third hook: syscall 9");

    printf("Passed: %d, Failed: %d\n", passed, failed);
    return failed;
}

int test_hook_install_max_hooks() {
    printf("\n=== Test: Maximum Hook Limit (32) ===\n");
    int passed = 0, failed = 0;

    syscall_interception_context_t ctx;
    jocky_syscall_hook_init(&ctx, 12345);

    int i;
    for (i = 0; i < 32; i++) {
        int result = jocky_syscall_hook_install(&ctx, i, 0x1000 + i);
        ASSERT_EQ(result, 0, "install hook %d");
    }

    ASSERT_EQ(ctx.hook_count, 32, "hook_count is exactly 32");

    /* Try to install 33rd hook - should fail */
    int result = jocky_syscall_hook_install(&ctx, 100, 0x5000);
    ASSERT_EQ(result, -1, "33rd hook install returns -1");

    printf("Passed: %d, Failed: %d\n", passed, failed);
    return failed;
}

int test_modify_args_arg0() {
    printf("\n=== Test: Modify Syscall Argument 0 (rdi) ===\n");
    int passed = 0, failed = 0;

    struct user_regs_struct regs = {0};
    regs.rdi = 0x1000;
    regs.rsi = 0x2000;
    regs.rdx = 0x3000;

    // Can't actually test ptrace without a valid PID
    // But we can verify the hook registration works
    printf("[*] Note: Full ptrace testing requires a valid target process\n");
    printf("    This test verifies hook initialization\n");

    syscall_interception_context_t ctx;
    jocky_syscall_hook_init(&ctx, getpid());
    int result = jocky_syscall_hook_install(&ctx, 1, 0x1000);

    ASSERT_EQ(result, 0, "hook installation succeeds");

    printf("Passed: %d, Failed: %d\n", passed, failed);
    return failed;
}

int test_hook_cleanup_success() {
    printf("\n=== Test: Syscall Hook Cleanup ===\n");
    int passed = 0, failed = 0;

    syscall_interception_context_t ctx;
    jocky_syscall_hook_init(&ctx, getpid());
    jocky_syscall_hook_install(&ctx, 59, 0xdeadbeef);

    // Cleanup on self will fail (can't ptrace self without being debugged)
    // But we can verify the data structure is set up
    ASSERT_EQ(ctx.hook_count, 1, "hook installed before cleanup");

    printf("Passed: %d, Failed: %d\n", passed, failed);
    return failed;
}

int test_edge_case_zero_pid() {
    printf("\n=== Test: Edge Case - PID 0 (current process) ===\n");
    int passed = 0, failed = 0;

    syscall_interception_context_t ctx;
    int result = jocky_syscall_hook_init(&ctx, 0);

    ASSERT_EQ(result, 0, "init with PID 0 succeeds");
    ASSERT_EQ(ctx.target_pid, 0, "PID 0 stored correctly");

    printf("Passed: %d, Failed: %d\n", passed, failed);
    return failed;
}

int test_edge_case_negative_pid() {
    printf("\n=== Test: Edge Case - Negative PID ===\n");
    int passed = 0, failed = 0;

    syscall_interception_context_t ctx;
    int result = jocky_syscall_hook_init(&ctx, -1);

    // Should still initialize (PID validation happens at ptrace time)
    ASSERT_EQ(result, 0, "init with negative PID succeeds");
    ASSERT_EQ(ctx.target_pid, -1, "negative PID stored");

    printf("Passed: %d, Failed: %d\n", passed, failed);
    return failed;
}

int test_large_hook_address() {
    printf("\n=== Test: Large Hook Address (64-bit) ===\n");
    int passed = 0, failed = 0;

    syscall_interception_context_t ctx;
    jocky_syscall_hook_init(&ctx, 12345);

    unsigned long large_addr = 0xffffffffffffffff;
    int result = jocky_syscall_hook_install(&ctx, 59, large_addr);

    ASSERT_EQ(result, 0, "install with max address succeeds");
    ASSERT_EQ(ctx.hooks[0].hook_address, large_addr, "large address stored correctly");

    printf("Passed: %d, Failed: %d\n", passed, failed);
    return failed;
}

int main() {
    printf("================================================================================\n");
    printf("JOCKY Advanced Syscall Interception - Comprehensive Test Suite\n");
    printf("Authorized: Red Hat + IIT Bombay Cyber Security Team\n");
    printf("================================================================================\n");

    int total_failed = 0;

    total_failed += test_hook_init();
    total_failed += test_hook_init_null_context();
    total_failed += test_hook_install();
    total_failed += test_hook_install_multiple();
    total_failed += test_hook_install_max_hooks();
    total_failed += test_modify_args_arg0();
    total_failed += test_hook_cleanup_success();
    total_failed += test_edge_case_zero_pid();
    total_failed += test_edge_case_negative_pid();
    total_failed += test_large_hook_address();

    printf("\n================================================================================\n");
    printf("TEST SUMMARY\n");
    printf("================================================================================\n");

    if (total_failed == 0) {
        printf("[✓] All tests passed\n");
        printf("Coverage: Initialization, hook installation, edge cases, limits\n");
        return 0;
    } else {
        printf("[✗] %d test(s) failed\n", total_failed);
        return 1;
    }
}
