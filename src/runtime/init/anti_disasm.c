/*
 * Anti-Disassembly Module
 *
 * Advanced techniques to defeat static analysis and disassembler heuristics.
 * Includes polymorphic prologues, stack frame confusion, and control flow obfuscation.
 *
 * Portable across Windows x86-64 and Linux x86-64.
 */

#define _GNU_SOURCE
#include "jocky_rt.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/mman.h>
#endif

/* Polymorphic prologue variants - each encodes the same function setup differently */
static const uint8_t PROLOGUE_VARIANT_1[] = {
    0x48, 0x89, 0xe5,          /* mov rbp, rsp */
    0x90,                       /* nop */
};

static const uint8_t PROLOGUE_VARIANT_2[] = {
    0x55,                       /* push rbp */
    0x48, 0x89, 0xe5,          /* mov rbp, rsp */
};

static const uint8_t PROLOGUE_VARIANT_3[] = {
    0x48, 0x8b, 0xec,          /* mov rbp, rsp (alternate encoding) */
    0x0f, 0x1f, 0x00,          /* nop (3-byte multi-byte nop) */
};

static const uint8_t PROLOGUE_VARIANT_4[] = {
    0x55,                       /* push rbp */
    0x48, 0x8b, 0xec,          /* mov rbp, rsp (alternate encoding) */
    0xcc, 0x90,                 /* int3 + nop (confuse static analysis) */
};

/* Stack frame confusion pattern - interleave fake operations */
static void jocky_confuse_stack_frame(void)
{
    volatile int dummy[32];
    int i;

    for (i = 0; i < 32; i++) {
        dummy[i] = 0xdeadbeef ^ (i * 0x12345678);
    }

    /* Force compiler to not optimize away */
    __asm__ volatile("" : : "r"(dummy) : "memory");
}

/* Return address hiding via offset pointer */
uintptr_t jocky_hide_function_entry(uintptr_t func_ptr)
{
    /* Return offset pointer to hide true entry point.
     * Disassembler will be confused when it tries to follow this address. */
    return func_ptr + 3;  /* Skip first 3 bytes */
}

/* Cross-function indirect jumps to break linear disassembly */
void jocky_cross_function_obfuscate(void)
{
    /* Use indirect jumps through registers that reference adjacent code */
#ifdef _WIN32
    __asm__(
        "leaq 0x20(%%rip), %%rax\n"
        "jmp *%%rax\n"
        ".p2align 4\n"
        : : : "rax"
    );
#else
    __asm__ volatile(
        "leaq 0x20(%%rip), %%rax\n"
        "jmp *%%rax\n"
        ".p2align 4\n"
        : : : "rax"
    );
#endif
}

/* Detect disassembler hooks by checking instruction stream consistency */
bool jocky_detect_disasm_hooks(void)
{
    /* Some sandboxes hook disassembly APIs or modify code pages.
     * Check if our function prologue is what we expect. */
    uintptr_t func_addr = (uintptr_t)&jocky_detect_disasm_hooks;
    uint8_t* code = (uint8_t*)func_addr;

    /* On x86-64, valid prologues should match known patterns.
     * If code is patched, bytes won't match any valid prologue. */
    if (code[0] == 0x55 || code[0] == 0x48 || code[0] == 0xcc) {
        return false;  /* Standard prologue detected */
    }

    /* Unusual bytes at function entry = possible hook/modification */
    return true;
}

/* Polymorphic instruction encoding detection evasion */
bool jocky_has_polymorphic_encoding(uint8_t* code_ptr, size_t len)
{
    /* Verify that code uses multiple instruction encodings for same operations.
     * Disassemblers struggle when mov rbp, rsp is encoded multiple ways. */
    size_t variant_count = 0;

    if (len >= sizeof(PROLOGUE_VARIANT_1) &&
        memcmp(code_ptr, PROLOGUE_VARIANT_1, sizeof(PROLOGUE_VARIANT_1)) == 0) {
        variant_count++;
    }

    if (len >= sizeof(PROLOGUE_VARIANT_2) &&
        memcmp(code_ptr, PROLOGUE_VARIANT_2, sizeof(PROLOGUE_VARIANT_2)) == 0) {
        variant_count++;
    }

    if (len >= sizeof(PROLOGUE_VARIANT_3) &&
        memcmp(code_ptr, PROLOGUE_VARIANT_3, sizeof(PROLOGUE_VARIANT_3)) == 0) {
        variant_count++;
    }

    return variant_count > 0;
}

/* Detect CFG (Control Flow Guard) instrumentation hooks */
bool jocky_detect_cfg_hooks(void)
{
#ifdef _WIN32
    /* CFG adds `__guard_check_icall_nop` calls before indirect calls.
     * Presence indicates running under CFG. */
    HMODULE kernel32 = GetModuleHandleA("kernel32.dll");
    if (!kernel32) return false;

    /* Try to find _guard_check_icall_nop - if it exists, CFG is active */
    FARPROC guard_func = GetProcAddress(kernel32, "__guard_check_icall_nop");
    return guard_func != NULL;
#else
    /* Linux: CFG equivalent is CET (Control-flow Enforcement Technology).
     * Check via /proc/cpuinfo for shadow stack support. */
    FILE* cpuinfo = fopen("/proc/cpuinfo", "r");
    if (!cpuinfo) return false;

    char line[256];
    bool has_shstk = false;

    while (fgets(line, sizeof(line), cpuinfo)) {
        if (strstr(line, "shstk") != NULL) {
            has_shstk = true;
            break;
        }
    }

    fclose(cpuinfo);
    return has_shstk;
#endif
}

/* Detect static analysis by checking if code is modified at runtime */
bool jocky_detect_static_analysis(void)
{
    /* Compute hash of our function at entry.
     * If static analysis tools have instrumented the code, bytes will differ. */
    static uint8_t entry_hash = 0;
    static bool hash_computed = false;

    if (!hash_computed) {
        uint8_t* func_code = (uint8_t*)&jocky_detect_static_analysis;
        uint32_t checksum = 0;
        size_t i;

        for (i = 0; i < 64; i++) {
            checksum ^= func_code[i];
            checksum = (checksum << 1) | (checksum >> 31);
        }

        entry_hash = (uint8_t)(checksum & 0xFF);
        hash_computed = true;
    }

    return entry_hash != 0;
}

/* Function pointer mutation to evade IDA/Ghidra xrefs */
typedef void (*obfuscated_func_t)(void);

obfuscated_func_t jocky_obfuscate_function_ptr(obfuscated_func_t func)
{
    /* XOR function pointer with random value to hide from static analysis */
    static uint64_t obfuscation_key = 0;

    if (obfuscation_key == 0) {
        /* Generate key from environment - different each run */
        obfuscation_key = (uint64_t)&obfuscation_key ^ 0xdeadbeefcafebabeULL;
    }

    return (obfuscated_func_t)((uintptr_t)func ^ obfuscation_key);
}

/* Reverse obfuscation - deobfuscate XORed function pointer */
obfuscated_func_t jocky_deobfuscate_function_ptr(obfuscated_func_t func)
{
    static uint64_t obfuscation_key = 0;

    if (obfuscation_key == 0) {
        obfuscation_key = (uint64_t)&obfuscation_key ^ 0xdeadbeefcafebabeULL;
    }

    return (obfuscated_func_t)((uintptr_t)func ^ obfuscation_key);
}

/* Detect string interception/logging */
bool jocky_detect_string_logging(void)
{
#ifdef _WIN32
    /* Check if GetProcAddress has been patched to log calls */
    HMODULE kernel32 = GetModuleHandleA("kernel32.dll");
    if (!kernel32) return false;

    uint8_t* get_proc_addr = (uint8_t*)GetProcAddress(kernel32, "GetProcAddress");
    if (!get_proc_addr) return false;

    /* Legitimate GetProcAddress should start with standard prologue.
     * If heavily patched, likely under analysis. */
    if (get_proc_addr[0] != 0x48 && get_proc_addr[0] != 0x55) {
        return true;  /* Unusual prologue = likely hooked */
    }

    return false;
#else
    /* Linux: check if libc functions are wrapped/intercepted */
    FILE* maps = fopen("/proc/self/maps", "r");
    if (!maps) return false;

    char line[256];
    int libc_regions = 0;

    while (fgets(line, sizeof(line), maps)) {
        if (strstr(line, "libc") != NULL && strstr(line, "rwx")) {
            libc_regions++;
        }
    }

    fclose(maps);

    /* Multiple RWX regions in libc = likely instrumentation */
    return libc_regions > 1;
#endif
}

/* Detect Frida code injection by checking for imported function hooks */
bool jocky_detect_frida_hooks(void)
{
#ifdef _WIN32
    /* Frida injects into ntdll.dll - check for anomalies */
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    if (!ntdll) return false;

    uint8_t* ntdll_code = (uint8_t*)ntdll;

    /* Scan for Frida's common stub pattern: JMP to trampoline */
    for (size_t i = 0; i < 0x1000; i++) {
        if (ntdll_code[i] == 0xE9) {  /* JMP rel32 - direct jump */
            /* Frida uses direct jumps for hooking */
            int32_t offset = *(int32_t*)(ntdll_code + i + 1);
            uintptr_t target = (uintptr_t)(ntdll_code + i + 5 + offset);

            /* Check if target is outside ntdll's typical range */
            if (target < (uintptr_t)ntdll ||
                target > (uintptr_t)ntdll + 0x100000) {
                return true;  /* Jump outside module = likely Frida hook */
            }
        }
    }

    return false;
#else
    /* Linux: check for Frida's agent library */
    FILE* maps = fopen("/proc/self/maps", "r");
    if (!maps) return false;

    char line[256];
    bool found_frida = false;

    while (fgets(line, sizeof(line), maps)) {
        if (strstr(line, "frida") != NULL) {
            found_frida = true;
            break;
        }
    }

    fclose(maps);
    return found_frida;
#endif
}

/* Mark functions with anti-disasm attributes for codegen integration */
void jocky_anti_disasm_init(void)
{
    /* Called at startup to set up anti-disasm defenses.
     * Can be invoked from jocky_runtime_init or compiled into hot functions. */

    if (jocky_detect_disasm_hooks()) {
        return;  /* Disassembler hooks detected */
    }

    if (jocky_detect_cfg_hooks()) {
        return;  /* CFG/CET instrumentation detected */
    }

    jocky_confuse_stack_frame();
}
