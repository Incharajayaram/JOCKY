# Runtime API Structure & Organization

**Last Updated:** 2026-09-30  
**Purpose:** Comprehensive reference for runtime API organization to prevent duplication and guide new implementations.

---

## Directory Organization

```
src/runtime/
├── include/              # Public API headers (all platforms)
│   ├── jocky_rt.h       # Main runtime interface
│   ├── jocky_process.h
│   ├── jocky_network.h
│   ├── jocky_syscall.h
│   ├── jocky_mem.h
│   ├── jocky_audit.h
│   ├── jocky_plugin.h
│   ├── jocky_sandbox.h
│   ├── jocky_lkm.h
│   ├── jocky_dir.h
│   ├── jocky_process_control.h
│   └── jocky_internal.h
│
├── common/              # Shared implementations (both platforms)
│   ├── *.c files with cross-platform logic
│   └── No platform-specific code here
│
├── windows/             # Windows-specific implementations
│   ├── win_process.c
│   ├── win_network.c
│   ├── win_syscall.c
│   ├── win_audit.c
│   ├── win_sandbox.c
│   ├── win_lkm.c
│   ├── win_registry.c
│   ├── win_drivers.c
│   └── ... (platform-specific modules)
│
├── linux/               # Linux-specific implementations
│   ├── linux_process.c
│   ├── linux_network.c
│   ├── linux_syscall.c
│   ├── linux_audit.c
│   ├── linux_sandbox.c
│   ├── linux_lkm.c
│   ├── linux_ebpf.c
│   └── ... (platform-specific modules)
│
├── forensics/           # Forensic analysis APIs (Kamimi's pipeline)
│   ├── forensic_core.c
│   ├── forensic_*.c (various forensic modules)
│   └── ... (see FORENSIC_INTEGRATION_STATUS.md)
│
├── ai/                  # AI/ML model implementations
├── byovd/               # BYOVD chain implementations
├── compression/         # Compression utilities
├── crypto/              # Cryptographic implementations
├── exfil/               # Data exfiltration APIs
├── exploitation/        # Exploitation utilities
├── io/                  # I/O operations
├── memory/              # Memory management APIs
├── network/             # Network primitives
├── pack/                # Packing utilities
├── util/                # Utility functions
├── vm/                  # Virtual machine implementations
├── core/                # Core runtime logic
├── init/                # Initialization routines
│
├── RUNTIME_API.md       # Detailed API documentation (65KB+)
├── CMakeLists.txt       # Build configuration
└── obfuscation.c        # Obfuscation pass logic
```

---

## API Categories & Platform Assignments

### Shared APIs (in `common/`)
These should NOT be duplicated between Windows and Linux:
- Cryptographic primitives (AES, RSA, hashing)
- Generic compression logic
- Generic network utilities
- Utility functions (logging, memory helpers)
- AI/ML model inference

### Windows-Only APIs (`windows/`)
- Windows Registry operations (`jocky_registry.h`)
- Windows driver interactions (Kernel-Mode Driver Framework)
- Windows API wrappers (CreateProcess, GetModuleHandle, etc.)
- Windows-specific audit/logging (ETW integration)
- Windows Sandbox APIs

### Linux-Only APIs (`linux/`)
- Linux syscall wrappers (seccomp, ptrace)
- eBPF program management
- Linux Kernel Module (LKM) interactions
- Linux-specific audit (auditd integration)
- Linux Sandbox APIs (seccomp, namespace)

### Platform-Agnostic APIs (Core)
These implement logic that works on both platforms via platform wrappers:
- Process management (wraps Windows CreateProcess + Linux fork/execve)
- Network I/O (wraps Windows socket API + Linux BSD sockets)
- Memory management (wraps VirtualAlloc + mmap)
- System information (wraps GetSystemInfo + sysconf)

---

## Adding a New Runtime API

### 1. Determine Scope
- **If shared logic:** Implement in `common/`, create cross-platform interface in `include/`
- **If platform-specific:** Implement in `windows/` or `linux/`, expose via `include/jocky_*.h`
- **If both:** Shared core in `common/`, platform wrappers in respective dirs

### 2. Check Existing Implementations
```bash
# Search for existing function
grep -r "function_name" src/runtime --include="*.c" --include="*.h"

# Check API documentation
grep "function_name" src/runtime/RUNTIME_API.md

# Check headers for declarations
grep -r "function_name" src/runtime/include/
```

### 3. Create Implementation
- Add `.c` file to appropriate directory
- Add header declaration to `src/runtime/include/jocky_*.h`
- Update CMakeLists.txt with new source file
- Document in RUNTIME_API.md

### 4. Link into Compilation
- Update `src/jocky/language/codegen.py` if new builtin
- Update prelude (`stdlib/jocky.runtime.jky`) if user-facing
- Link in backend configuration
- Add tests in `tests/` directory

---

## Preventing Duplication

### Before Implementation
1. **Search runtime folder:** `grep -r "api_name" src/runtime/`
2. **Check RUNTIME_API.md:** Look for similar functionality
3. **Ask user:** "Should this be implemented from scratch, or does a similar API exist?"

### During Implementation
- Use existing utilities from `common/` and `util/`
- Leverage platform wrappers instead of reimplementing logic
- Follow existing naming conventions (`jocky_*`, `win_*`, `linux_*`)

### Code Review Checklist
- ✅ No duplicate implementations in `windows/` and `linux/`
- ✅ Shared logic extracted to `common/`
- ✅ Platform-specific logic only in respective directories
- ✅ All headers updated in `include/`
- ✅ CMakeLists.txt includes new files
- ✅ No copy-paste code between platforms

---

## Key Files for Reference

- **RUNTIME_API.md:** Complete API documentation (65KB, detailed)
- **CMakeLists.txt:** Build configuration showing all compiled files
- **include/jocky_rt.h:** Main runtime interface
- **Common implementation patterns:** See `src/runtime/common/` for examples

---

## Current Implementation Status

### Fully Implemented & Tested
- ✅ Process management (Windows + Linux)
- ✅ Network I/O (Windows + Linux)
- ✅ Cryptographic functions
- ✅ Memory management
- ✅ System information
- ✅ Audit logging (partial)
- ✅ Sandbox APIs (partial)
- ✅ Plugin system
- ✅ BYOVD chain
- ✅ LKM support (partial)
- ✅ eBPF support (Linux)

### Partially Integrated
- ⚠️ Forensic analysis APIs (Kamimi's pipeline) — See FORENSIC_INTEGRATION_STATUS.md
- ⚠️ AI/ML model integration

### In Development
- Research chain integration of forensics
- Full eBPF integration
- Complete sandbox API coverage

---

## Notes for Integration Work

1. **Forensic Integration:** The forensic pipeline from Kamimi exists in `src/runtime/forensics/` but is not fully integrated into compilation pipeline. See FORENSIC_INTEGRATION_STATUS.md.

2. **Research Chain Files:** The two research chain production files need forensic APIs linked:
   - `examples/research_chain_windows_production.jky`
   - `examples/research_chain_linux_production.jky`

3. **Backend Compilation:** Main pipeline must compile Windows and Linux scripts without errors. Use `./dev_launch.sh` for testing.

4. **Platform Parity:** Ensure Windows and Linux have equivalent API coverage where possible to avoid platform-specific bugs.
