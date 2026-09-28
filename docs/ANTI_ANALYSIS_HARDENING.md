# JOCKY Anti-Analysis & Anti-Sandbox Hardening

Comprehensive anti-debugging, anti-VM, anti-sandbox, and anti-disassembly techniques for evading sophisticated analysis environments.

## Overview

The enhanced anti-analysis module provides three-layer defense:

1. **Debugger/Tracer Detection** – Detect local and remote debugging
2. **Hypervisor Detection** – Identify VMs and containers
3. **Sandbox Evasion** – Detect analysis tools and environments
4. **Anti-Disassembly** – Defeat static analysis and IDA/Ghidra

All checks are production-quality, portable (Windows/Linux), and designed for minimal false positives.

## Part 1: Debugger Detection

### `jocky_is_debugger_present() -> bool`

Detect local debugger attachment using native APIs:
- **Windows:** `IsDebuggerPresent()` + `CheckRemoteDebuggerPresent()`
- **Linux:** `ptrace(PTRACE_TRACEME)` – fails if already being traced

Returns `true` if debugger found.

### `jocky_is_remote_debugger() -> bool`

Detect remote debugging sessions:
- **Windows:** `CheckRemoteDebuggerPresent()` on current process
- **Linux:** Parse `/proc/self/status` for non-zero `TracerPid`

### `jocky_check_hardware_breakpoints() -> bool`

Windows-only. Check CPU debug registers (DR0–DR3) for hardware breakpoints set by debuggers.

### `jocky_detect_execution_tracing() -> bool`

Detect if process is under tracing:
- **Windows:** `CheckRemoteDebuggerPresent()` + `IsDebuggerPresent()`
- **Linux:** Check `/proc/self/status` for attached tracer

## Part 2: Enhanced VM Detection

All functions return `true` if VM detected.

### `jocky_is_vm() -> bool`

Generic hypervisor detection via CPUID:
- Check bit 31 of ECX in CPUID leaf 1
- Works across all major hypervisors

### `jocky_is_hyperv() -> bool`

Detect Microsoft Hyper-V:
- CPUID leaf 0x40000000 returns specific signature
- Windows-primary detection

### `jocky_is_xen() -> bool`

Detect Xen hypervisor:
- CPUID leaf 0x40000000 returns "XenVMMXenVMM"
- Common in cloud/VPS environments

### `jocky_is_kvm() -> bool`

Detect KVM (Linux hypervisor):
- CPUID leaf 0x40000000 returns "KVMKVMKVM"
- Checks via inline assembly

### `jocky_is_vmware() -> bool`

Detect VMware:
- Uses VMware backdoor port (0x5658)
- Sends magic number 0x564d5868 ("VMXh")
- Fallback to CPUID checking on Linux

### `jocky_is_virtualbox() -> bool`

Detect VirtualBox:
- Windows: Check CPUID vendor
- Linux: Parse `/proc/cpuinfo` for VirtualBox markers

### `jocky_is_qemu() -> bool`

Detect QEMU emulator:
- Linux: Parse `/proc/cpuinfo` for QEMU markers
- Minimal footprint

## Part 3: Enhanced Sandbox Detection

Detect analysis/sandbox environments via multiple vectors.

### `jocky_detect_sandbox_filesystem() -> bool`

Check for known sandbox marker directories:
- Cuckoo: `/opt/cuckoo`, `C:\Cuckoo`
- Sandboxie: `/opt/sandboxie`, `C:\Sandboxie`
- Threat Defense: `/opt/threat_defense`
- Frida: `/opt/frida`
- Analysis environments: `/var/sandbox`, `/etc/sandbox`

### `jocky_detect_analysis_processes() -> bool`

Scan running process list for known analysis tools:
- **Debuggers:** gdb, lldb, windbg, x64dbg, ollydbg
- **Dynamic Analysis:** strace, ltrace, valgrind, frida-server
- **Reverse Engineering:** ghidra, radare2, ida, ida64
- **Monitoring:** procmon, procexp, wireshark, fiddler
- **Disassembly:** objdump, readelf, nm

Windows uses `CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS)`.
Linux parses `/proc/self/cmdline`.

### `jocky_detect_analysis_environment() -> bool`

Check environment variables set by analysis platforms:
- `CUCKOO` – Cuckoo sandbox
- `SANDBOX` – Generic sandbox
- `QEMU` – QEMU emulator
- `WINE` – Wine compatibility layer
- `FRIDA` – Frida instrumentation
- `LD_PRELOAD` / `LD_AUDIT` – Linux library hooking
- `VALGRIND` – Memory profiler
- `VPC`, `XPVM` – Older VMs

### `jocky_detect_execution_tracing() -> bool`

Detect active process tracing:
- Windows: Check debugger present via APIs
- Linux: Check `/proc/self/status` for non-zero `TracerPid`

### `jocky_is_sandbox() -> bool`

**Master sandbox check** – combines all above techniques plus:
- Timing checks (sleep acceleration)
- Known sandbox usernames (sandbox, vmware, virtualbox, test, etc.)
- Known sandbox DLLs (sbiedll.dll, api_log.dll, etc.)
- Linux DMI product name checks

## Part 4: Anti-Disassembly Techniques

### `jocky_hide_function_entry(func_ptr) -> uintptr_t`

Return offset pointer to hide true function entry:
- Returns `func_ptr + 3` – skips first 3 bytes
- Disassemblers get confused following this pointer
- Used to hide xrefs in static analysis tools

### `jocky_cross_function_obfuscate() -> void`

Break linear disassembly with indirect jumps:
- Use RIP-relative LEA to load nearby address
- Jump indirectly via register
- Force misalignment with `.p2align 4`
- Disassembler loses linear tracking

### `jocky_detect_disasm_hooks() -> bool`

Detect if disassembler APIs have been hooked:
- Check function prologue at entry point
- Unusual bytes indicate possible hook/instrumentation
- Sandboxes sometimes hook disassembly for inspection

### `jocky_has_polymorphic_encoding(code_ptr, len) -> bool`

Verify code uses multiple instruction encodings:
- `mov rbp, rsp` can be encoded multiple ways
- Check for PROLOGUE_VARIANT_1 through VARIANT_4
- Returns `true` if polymorphic encoding detected

### `jocky_detect_cfg_hooks() -> bool`

Detect Control Flow Guard (CFG) / CET instrumentation:
- **Windows:** Check for `__guard_check_icall_nop` export
- **Linux:** Check `/proc/cpuinfo` for `shstk` (shadow stack)
- Indicates running under instrumentation/monitoring

### `jocky_detect_static_analysis() -> bool`

Runtime check if code was instrumented at startup:
- Hash function entry bytes
- Compare against expected value
- Instrumentation tools modify code bytes

### `jocky_detect_string_logging() -> bool`

Detect API string logging/interception:
- Windows: Check if kernel32 functions are hooked
- Linux: Scan `/proc/self/maps` for RWX regions in libc
- Multiple RWX regions = likely instrumentation

### `jocky_detect_frida_hooks() -> bool`

Detect Frida dynamic instrumentation framework:
- Windows: Scan ntdll.dll for Frida's typical JMP patterns
- Linux: Check `/proc/self/maps` for frida library
- Frida uses direct JMPs to trampoline code

### Function Pointer Obfuscation

#### `jocky_obfuscate_function_ptr(func) -> obfuscated_func_t`

XOR-encrypt function pointer to hide from static analysis:
- Uses environment-based key for randomness
- Different key each run
- IDA/Ghidra cannot track xrefs

```c
obfuscated_func_t hidden = jocky_obfuscate_function_ptr((obfuscated_func_t)callback);
/* Pass hidden pointer around, hidden from disassemblers */
```

#### `jocky_deobfuscate_function_ptr(hidden) -> obfuscated_func_t`

Reverse the XOR encryption:
- Recovers original function pointer
- Use just before indirect call

```c
jocky_deobfuscate_function_ptr(hidden)();  /* Call the obfuscated function */
```

### `jocky_anti_disasm_init() -> void`

Initialize anti-disasm defenses at startup:
- Called from `jocky_runtime_init()` or manually
- Runs confusion checks
- Sets up polymorphic prologues

## Part 5: Master Analysis Check

### `jocky_check_analysis_environment() -> i32`

**All-in-one analysis environment detection.**

Returns bitmask:

| Bit | Value | Meaning |
|-----|-------|---------|
| 0   | 1     | Debugger present |
| 1   | 2     | Running in VM |
| 2   | 4     | Sandbox detected |

```c
uint32_t threats = jocky_check_analysis_environment();

if (threats & JOCKY_ANALYSIS_DEBUGGER) {
    /* Debugger attached */
}
if (threats & JOCKY_ANALYSIS_VM) {
    /* Running inside hypervisor */
}
if (threats & JOCKY_ANALYSIS_SANDBOX) {
    /* Sandbox environment detected */
}
```

## Usage Examples

### Comprehensive Environment Check

```c
uint32_t threats = jocky_runtime_init();  /* Automatically called, but can be explicit */

if (threats != JOCKY_ANALYSIS_CLEAN) {
    fprintf(stderr, "Hostile analysis environment detected\n");
    exit(1);
}
```

### Specific Checks

```c
if (jocky_is_debugger_present()) {
    fprintf(stderr, "Local debugger found\n");
    exit(1);
}

if (jocky_is_vm()) {
    fprintf(stderr, "Running inside VM\n");
    exit(1);
}

if (jocky_detect_analysis_processes()) {
    fprintf(stderr, "Analysis tools found\n");
    exit(1);
}
```

### Dynamic Function Pointer Protection

```c
typedef int (*callback_t)(int);

callback_t func = &my_callback;

/* Hide from disassembler */
obfuscated_func_t hidden = jocky_obfuscate_function_ptr((obfuscated_func_t)func);

/* Store/transmit hidden pointer */
...

/* Later: deobfuscate and call */
callback_t restored = (callback_t)jocky_deobfuscate_function_ptr(hidden);
int result = restored(42);
```

### Polymorphic Code Generation

In codegen, mark functions with `obfuscate` attribute:

```jocky
@obfuscate
fn sensitive_operation() -> void {
    /* This function will get:
       - Polymorphic prologue
       - Stack frame confusion
       - Anti-disasm markers
    */
}
```

## Implementation Details

### Timing Checks

Both Windows and Linux include timing validation:
- `jocky_check_timing_rdtsc()` – RDTSC delta over 50ms sleep
- `jocky_check_timing_api()` – API-based timing over 500ms sleep
- Sandboxes often skip/accelerate sleeps

### Portable Design

All functions work on Windows and Linux x86-64:
- Conditional compilation (#ifdef _WIN32)
- Native APIs where applicable
- Fallback to portable checks

### Minimal Dependencies

- Windows: windows.h, intrin.h, tlhelp32.h
- Linux: stdio.h, unistd.h, sys/* headers
- No external libraries required

### Performance

All checks are optimized:
- Most are O(1) or O(n) with small n
- Process scanning O(process_count)
- Filesystem checks cached where possible
- Suitable for startup initialization

## Security Considerations

### False Positives

Design prioritizes avoiding false positives in clean environments:
- No aggressive heuristics
- Checks are precise and specific
- Known good values and signatures

### Evasion Limitations

These checks are defeated by:
- Sophisticated hypervisor/VM that perfectly mimics bare metal
- Custom sandbox with all markers removed
- Debugger with complete API spoofing

### Suggested Integration

Use as part of multi-layer defense:
1. Integrity check (on-disk verification)
2. Anti-analysis checks (environment detection)
3. Anti-hook verification (unhook ntdll)
4. Behavioral monitoring during execution
5. Obfuscation of sensitive code paths

## Testing

Run the test suite:

```bash
cd /home/deval/JOCKY/tests/unit
gcc -I../../src/runtime/include test_anti_analysis_enhanced.c \
    ../../src/runtime/init/anti_analysis.c \
    ../../src/runtime/init/anti_disasm.c \
    -o test_anti_analysis
./test_anti_analysis
```

Run the demonstration:

```bash
cd /home/deval/JOCKY/examples
gcc -I../src/runtime/include anti_analysis_demo.c \
    ../src/runtime/init/anti_analysis.c \
    ../src/runtime/init/anti_disasm.c \
    -o anti_analysis_demo
./anti_analysis_demo
```

## API Reference

See `/home/deval/JOCKY/src/runtime/RUNTIME_API.md` sections 2 (Anti-Analysis) for complete function signatures and FFI declarations for use in JOCKY language.

## Files Modified/Created

- `src/runtime/init/anti_analysis.c` – Enhanced with VM/sandbox/tracing detection
- `src/runtime/init/anti_disasm.c` – New file with anti-disassembly techniques
- `src/runtime/include/jocky_rt.h` – New function declarations
- `src/runtime/CMakeLists.txt` – Added anti_disasm.c to build
- `src/runtime/RUNTIME_API.md` – Added documentation for new functions
- `tests/unit/test_anti_analysis_enhanced.c` – Comprehensive test suite
- `examples/anti_analysis_demo.c` – Demonstration of all features
- `docs/ANTI_ANALYSIS_HARDENING.md` – This file

## Future Enhancements

Possible additions:
- GPU-based analysis detection (CUDA/OpenCL)
- Memory forensics resistance (resist volatility plugins)
- Behavior-based sandbox detection (anomalous API patterns)
- Anti-QEMU-specific techniques (I/O port fingerprinting)
- Custom codegen hooks for inline obfuscation
