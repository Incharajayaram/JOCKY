/*
 * Anti-Analysis Demonstration
 *
 * Shows comprehensive usage of JOCKY's anti-debug, anti-VM, anti-sandbox,
 * and anti-disassembly capabilities.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../src/runtime/include/jocky_rt.h"

typedef struct {
    const char* name;
    bool (*check)(void);
    const char* description;
} check_item_t;

void print_check(const check_item_t* item, bool result)
{
    printf("  [%s] %-40s %s\n",
           result ? "X" : " ",
           item->name,
           item->description);
}

int main(void)
{
    printf("\n");
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║     JOCKY Anti-Analysis & Anti-Sandbox Demo            ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n\n");

    /* Run runtime initialization (includes integrity check) */
    printf("Initializing JOCKY runtime...\n");
    uint32_t threats = jocky_runtime_init();

    if (threats == JOCKY_ANALYSIS_CLEAN) {
        printf("✓ Environment appears clean\n\n");
    } else {
        printf("⚠ Threats detected: 0x%08x\n\n", threats);
    }

    /* Debugger Detection */
    printf("┌─ Debugger Detection ─────────────────────────────────┐\n");
    check_item_t debugger_checks[] = {
        {"jocky_is_debugger_present", jocky_is_debugger_present,
         "Local debugger (IsDebuggerPresent)"},
        {"jocky_is_remote_debugger", jocky_is_remote_debugger,
         "Remote debugger (CheckRemoteDebugger)"},
        {"jocky_check_hardware_breakpoints", jocky_check_hardware_breakpoints,
         "Hardware breakpoints (DR0-DR3)"},
        {"jocky_detect_execution_tracing", jocky_detect_execution_tracing,
         "Execution tracing (ptrace/debugger)"},
    };

    for (size_t i = 0; i < sizeof(debugger_checks) / sizeof(debugger_checks[0]); i++) {
        print_check(&debugger_checks[i], debugger_checks[i].check());
    }
    printf("└──────────────────────────────────────────────────────┘\n\n");

    /* VM Detection */
    printf("┌─ VM Detection ───────────────────────────────────────┐\n");
    check_item_t vm_checks[] = {
        {"jocky_is_vm", jocky_is_vm,
         "Generic hypervisor (CPUID bit 31)"},
        {"jocky_is_hyperv", jocky_is_hyperv,
         "Hyper-V detection"},
        {"jocky_is_xen", jocky_is_xen,
         "Xen hypervisor detection"},
        {"jocky_is_kvm", jocky_is_kvm,
         "KVM (Linux) detection"},
        {"jocky_is_vmware", jocky_is_vmware,
         "VMware detection (backdoor)"},
        {"jocky_is_virtualbox", jocky_is_virtualbox,
         "VirtualBox detection"},
        {"jocky_is_qemu", jocky_is_qemu,
         "QEMU detection"},
    };

    for (size_t i = 0; i < sizeof(vm_checks) / sizeof(vm_checks[0]); i++) {
        print_check(&vm_checks[i], vm_checks[i].check());
    }
    printf("└──────────────────────────────────────────────────────┘\n\n");

    /* Sandbox Detection */
    printf("┌─ Sandbox Detection ──────────────────────────────────┐\n");
    check_item_t sandbox_checks[] = {
        {"jocky_is_sandbox", jocky_is_sandbox,
         "General sandbox/timing checks"},
        {"jocky_detect_sandbox_filesystem", jocky_detect_sandbox_filesystem,
         "Sandbox filesystem markers"},
        {"jocky_detect_analysis_processes", jocky_detect_analysis_processes,
         "Analysis tools in process list"},
        {"jocky_detect_analysis_environment", jocky_detect_analysis_environment,
         "Analysis environment variables"},
    };

    for (size_t i = 0; i < sizeof(sandbox_checks) / sizeof(sandbox_checks[0]); i++) {
        print_check(&sandbox_checks[i], sandbox_checks[i].check());
    }
    printf("└──────────────────────────────────────────────────────┘\n\n");

    /* Anti-Disassembly Detection */
    printf("┌─ Anti-Disassembly / Static Analysis Detection ────────┐\n");
    check_item_t disasm_checks[] = {
        {"jocky_detect_disasm_hooks", jocky_detect_disasm_hooks,
         "Disassembler API hooks"},
        {"jocky_detect_cfg_hooks", jocky_detect_cfg_hooks,
         "CFG/CET instrumentation"},
        {"jocky_detect_static_analysis", jocky_detect_static_analysis,
         "Runtime code instrumentation"},
        {"jocky_detect_string_logging", jocky_detect_string_logging,
         "API logging/interception"},
        {"jocky_detect_frida_hooks", jocky_detect_frida_hooks,
         "Frida code injection"},
    };

    for (size_t i = 0; i < sizeof(disasm_checks) / sizeof(disasm_checks[0]); i++) {
        print_check(&disasm_checks[i], disasm_checks[i].check());
    }
    printf("└──────────────────────────────────────────────────────┘\n\n");

    /* Function Pointer Obfuscation Demo */
    printf("┌─ Function Pointer Obfuscation Demo ──────────────────┐\n");
    typedef void (*test_func_t)(void);
    test_func_t original = (test_func_t)&main;

    printf("  Original function pointer:   %p\n", (void*)original);

    obfuscated_func_t obfuscated = jocky_obfuscate_function_ptr((obfuscated_func_t)original);
    printf("  Obfuscated pointer:          %p\n", (void*)obfuscated);

    obfuscated_func_t deobfuscated = jocky_deobfuscate_function_ptr(obfuscated);
    printf("  Deobfuscated pointer:        %p\n", (void*)deobfuscated);

    if (original == (test_func_t)deobfuscated) {
        printf("  ✓ Obfuscation/deobfuscation working correctly\n");
    }
    printf("└──────────────────────────────────────────────────────┘\n\n");

    /* Summary */
    printf("┌─ Analysis Summary ───────────────────────────────────┐\n");
    uint32_t analysis = jocky_check_analysis_environment();

    printf("  Analysis flags: 0x%08x\n", analysis);
    printf("  Environment is: ");

    if (analysis == JOCKY_ANALYSIS_CLEAN) {
        printf("CLEAN ✓\n");
    } else {
        printf("HOSTILE ✗\n");
        if (analysis & JOCKY_ANALYSIS_DEBUGGER) printf("    - Debugger/tracer present\n");
        if (analysis & JOCKY_ANALYSIS_VM) printf("    - Running inside VM\n");
        if (analysis & JOCKY_ANALYSIS_SANDBOX) printf("    - Sandbox environment detected\n");
    }
    printf("└──────────────────────────────────────────────────────┘\n\n");

    return (analysis != JOCKY_ANALYSIS_CLEAN) ? 1 : 0;
}
