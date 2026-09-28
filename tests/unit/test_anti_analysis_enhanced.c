/*
 * Enhanced Anti-Analysis Tests
 *
 * Tests for new VM detection, sandbox detection, and anti-disasm functions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../../src/runtime/include/jocky_rt.h"

#define TEST_ASSERT(expr, msg) \
    do { \
        if (!(expr)) { \
            printf("FAIL: %s\n", msg); \
            return false; \
        } \
        printf("PASS: %s\n", msg); \
    } while (0)

#define TEST_SUITE_START(name) \
    printf("\n=== %s ===\n", name)

#define TEST_SUITE_END(name, passed, total) \
    printf("%s: %d/%d tests passed\n", name, passed, total)

/* ============================================================================
 * VM Detection Tests
 * ============================================================================ */

static bool test_vm_detection(void)
{
    printf("\nTesting VM Detection Functions\n");
    printf("================================\n");

    bool has_vm = jocky_is_vm();
    printf("Generic VM check: %s\n", has_vm ? "VM detected" : "No VM");

    printf("Hypervisor-specific checks:\n");
    if (jocky_is_hyperv()) {
        printf("  - Hyper-V detected\n");
    }
    if (jocky_is_xen()) {
        printf("  - Xen detected\n");
    }
    if (jocky_is_kvm()) {
        printf("  - KVM detected\n");
    }
    if (jocky_is_vmware()) {
        printf("  - VMware detected\n");
    }
    if (jocky_is_virtualbox()) {
        printf("  - VirtualBox detected\n");
    }
    if (jocky_is_qemu()) {
        printf("  - QEMU detected\n");
    }

    printf("VM detection check complete\n");
    return true;
}

/* ============================================================================
 * Sandbox Detection Tests
 * ============================================================================ */

static bool test_sandbox_detection(void)
{
    printf("\nTesting Sandbox Detection Functions\n");
    printf("====================================\n");

    bool sandbox_filesystem = jocky_detect_sandbox_filesystem();
    printf("Filesystem check: %s\n", sandbox_filesystem ? "Sandbox paths found" : "No sandbox paths");

    bool analysis_processes = jocky_detect_analysis_processes();
    printf("Process list check: %s\n", analysis_processes ? "Analysis processes found" : "No analysis processes");

    bool analysis_env = jocky_detect_analysis_environment();
    printf("Environment check: %s\n", analysis_env ? "Analysis environment detected" : "No analysis environment");

    bool execution_trace = jocky_detect_execution_tracing();
    printf("Execution tracing check: %s\n", execution_trace ? "Debugger attached" : "No debugger");

    bool general_sandbox = jocky_is_sandbox();
    printf("General sandbox check: %s\n", general_sandbox ? "Sandbox detected" : "No sandbox");

    printf("Sandbox detection check complete\n");
    return true;
}

/* ============================================================================
 * Anti-Disasm Tests
 * ============================================================================ */

static bool test_anti_disasm_detection(void)
{
    printf("\nTesting Anti-Disasm Detection Functions\n");
    printf("=======================================\n");

    bool disasm_hooks = jocky_detect_disasm_hooks();
    printf("Disasm hooks check: %s\n", disasm_hooks ? "Hooks detected" : "No hooks");

    bool cfg_hooks = jocky_detect_cfg_hooks();
    printf("CFG/CET hooks check: %s\n", cfg_hooks ? "Instrumentation detected" : "No instrumentation");

    bool static_analysis = jocky_detect_static_analysis();
    printf("Static analysis check: %s\n", static_analysis ? "Instrumentation likely" : "Not instrumented");

    bool string_logging = jocky_detect_string_logging();
    printf("String logging check: %s\n", string_logging ? "API logging detected" : "No logging");

    bool frida_hooks = jocky_detect_frida_hooks();
    printf("Frida hooks check: %s\n", frida_hooks ? "Frida detected" : "No Frida");

    printf("Anti-disasm detection check complete\n");
    return true;
}

/* ============================================================================
 * Function Pointer Obfuscation Tests
 * ============================================================================ */

static bool test_function_obfuscation(void)
{
    printf("\nTesting Function Pointer Obfuscation\n");
    printf("====================================\n");

    typedef void (*test_func_t)(void);

    /* Get a legitimate function pointer */
    test_func_t original = (test_func_t)&test_function_obfuscation;
    printf("Original function pointer: %p\n", (void*)original);

    /* Obfuscate it */
    obfuscated_func_t obfuscated = jocky_obfuscate_function_ptr((obfuscated_func_t)original);
    printf("Obfuscated function pointer: %p\n", (void*)obfuscated);

    /* Verify they're different */
    if (original == (test_func_t)obfuscated) {
        printf("WARN: Obfuscation produced identical pointer\n");
    } else {
        printf("PASS: Obfuscation successfully hid pointer\n");
    }

    /* Deobfuscate and verify we get the original back */
    obfuscated_func_t deobfuscated = jocky_deobfuscate_function_ptr(obfuscated);
    printf("Deobfuscated function pointer: %p\n", (void*)deobfuscated);

    if (original == (test_func_t)deobfuscated) {
        printf("PASS: Deobfuscation recovered original pointer\n");
        return true;
    } else {
        printf("FAIL: Deobfuscation did not recover original pointer\n");
        return false;
    }
}

/* ============================================================================
 * Master Analysis Environment Check
 * ============================================================================ */

static bool test_master_check(void)
{
    printf("\nTesting Master Analysis Environment Check\n");
    printf("=========================================\n");

    uint32_t flags = jocky_check_analysis_environment();

    printf("Analysis environment flags: 0x%08x\n", flags);

    if (flags & JOCKY_ANALYSIS_DEBUGGER) {
        printf("  - DEBUGGER detected\n");
    }
    if (flags & JOCKY_ANALYSIS_VM) {
        printf("  - VM detected\n");
    }
    if (flags & JOCKY_ANALYSIS_SANDBOX) {
        printf("  - SANDBOX detected\n");
    }
    if (flags == JOCKY_ANALYSIS_CLEAN) {
        printf("  - Clean environment\n");
    }

    return true;
}

/* ============================================================================
 * Main Test Runner
 * ============================================================================ */

int main(void)
{
    printf("\n");
    printf("╔═══════════════════════════════════════════════╗\n");
    printf("║   JOCKY Enhanced Anti-Analysis Test Suite     ║\n");
    printf("╚═══════════════════════════════════════════════╝\n");

    int passed = 0;
    int total = 0;

    TEST_SUITE_START("VM Detection");
    if (test_vm_detection()) passed++;
    total++;
    TEST_SUITE_END("VM Detection", passed, total);

    TEST_SUITE_START("Sandbox Detection");
    if (test_sandbox_detection()) passed++;
    total++;
    TEST_SUITE_END("Sandbox Detection", passed, total);

    TEST_SUITE_START("Anti-Disasm Detection");
    if (test_anti_disasm_detection()) passed++;
    total++;
    TEST_SUITE_END("Anti-Disasm Detection", passed, total);

    TEST_SUITE_START("Function Obfuscation");
    if (test_function_obfuscation()) passed++;
    total++;
    TEST_SUITE_END("Function Obfuscation", passed, total);

    TEST_SUITE_START("Master Analysis Check");
    if (test_master_check()) passed++;
    total++;
    TEST_SUITE_END("Master Analysis Check", passed, total);

    printf("\n");
    printf("╔═══════════════════════════════════════════════╗\n");
    printf("║          OVERALL: %d/%d tests passed          ║\n", passed, total);
    printf("╚═══════════════════════════════════════════════╝\n");
    printf("\n");

    return (passed == total) ? 0 : 1;
}
