# JOCKY Runtime Library - Folder Structure Guide

## Platform-Specific Directories

### ✅ `src/runtime/windows/` - WINDOWS ONLY
**Target:** Windows PE executables, kernel, Windows APIs

**Current Files:**
- `byoud.c` - Bring Your Own Unwinding Data (CET bypass) - **WINDOWS SPECIFIC**

**When to add here:**
- Windows PE manipulation (sections, relocations, imports)
- Windows kernel features (PEB, TEB, SEH)
- Windows APIs (CreateProcess, VirtualAlloc, Registry, etc.)
- Intel CET handling
- Driver communication (IOCTL)
- Windows-specific evasion (ETW, EDR, WMI)

**Examples of what goes here:**
- Process hollowing (Windows-specific implementation)
- Token elevation
- Registry manipulation
- ETW/EDR bypass
- Driver operations

---

### ✅ `src/runtime/linux/` - LINUX ONLY
**Target:** Linux ELF executables, kernel, POSIX APIs

**Current Files:**
- `anti_analysis.c` - Debugger/tracer detection
- `compression.c` - zlib wrapper
- `crypto.c` - OpenSSL wrapper (AES, RSA, ECDH)
- `network.c` - TCP/UDP sockets
- `obfuscation.c` - Runtime code patching, hooking
- `threadpool.c` - pthread-based thread pool
- `sysinfo.c` / `sysinfo_syscall.c` - System information
- `process_syscall.c` - Process operations
- And various other syscall wrappers

**When to add here:**
- POSIX syscalls (open, read, write, mmap, etc.)
- Linux-specific detection (debuggers via /proc)
- ELF manipulation
- ptrace operations
- Linux kernel features
- Capability management (CAP_*)

**Examples of what goes here:**
- Process hollowing (ptrace-based implementation)
- Module loading (dlopen/dlsym)
- Memory mapping (mmap/mprotect)
- Signal handling
- File operations

---

## Platform-Agnostic Directories

### 📦 `src/runtime/include/` - HEADERS (both platforms)
**Purpose:** API definitions that work on both Windows and Linux

**Current Headers:**
- `jocky_threadpool.h` - Thread pool API
- `jocky_network.h` - Network sockets API
- `jocky_compression.h` - Compression API
- `jocky_crypto.h` - Cryptography API
- `jocky_anti_analysis.h` - Anti-analysis API
- `jocky_obfuscation.h` - Obfuscation API
- `jocky_byoud.h` - BYOUD (Windows PE only, but generic header)

**Guidelines:**
- Define portable API contracts
- Use `#ifdef _WIN32` / `#ifdef __linux__` for platform-specific types
- Implementations go in `windows/` or `linux/` folders
- Headers are shared between platforms

---

## Functional Categories (Reference)

These folders contain **related functional groupings** but don't dictate platform:

- `byovd/` - Driver operations (Windows focus, but cross-platform patterns)
- `cleanup/` - Anti-forensics (Windows focus)
- `compression/` - Data compression (cross-platform)
- `crypto/` - Cryptography (cross-platform)
- `evasion/` - Detection evasion (platform-specific implementations)
- `execution/` - In-memory execution (platform-specific)
- `exfil/` - Data exfiltration (cross-platform)
- `exploitation/` - Kernel/driver exploitation (platform-specific)
- `init/` - Runtime initialization (cross-platform)
- `io/` - File I/O (cross-platform)
- `memory/` - Memory operations (platform-specific)
- `network/` - Networking (cross-platform)
- `pack/` - Packing/unpacking (cross-platform)
- `registry/` - Registry access (Windows only)
- `threading/` - Threading (cross-platform)
- `util/` - Utilities (cross-platform)
- `vm/` - Virtual machine/code interpretation (cross-platform)

---

## How to Implement a Feature

### Example 1: Process Enumeration (Linux)
```c
// File: src/runtime/linux/process_enum.c
// Header: src/runtime/include/jocky_process.h

// Use POSIX APIs: /proc filesystem, procfs, getpid, etc.
// Reference: /proc/[pid]/* files
```

### Example 2: Token Elevation (Windows)
```c
// File: src/runtime/windows/token_elev.c
// Header: src/runtime/include/jocky_token.h

// Use Windows APIs: ImpersonateLoggedOnUser, AdjustTokenPrivileges, etc.
// Reference: Windows security model
```

### Example 3: Network Operations (Cross-platform)
```c
// File: src/runtime/linux/network.c  (POSIX sockets)
// File: src/runtime/windows/network.c (Winsock2)
// Header: src/runtime/include/jocky_network.h (shared API)

// Same API, different implementations per platform
```

---

## Quick Reference Table

| Feature | Windows | Linux | Header |
|---------|---------|-------|--------|
| BYOUD (CET bypass) | ✅ src/runtime/windows/ | ❌ N/A | jocky_byoud.h |
| Anti-analysis | ✅ detection | ✅ src/runtime/linux/ | jocky_anti_analysis.h |
| Crypto (AES/RSA) | ✅ via OpenSSL | ✅ src/runtime/linux/ | jocky_crypto.h |
| Compression | ✅ via zlib | ✅ src/runtime/linux/ | jocky_compression.h |
| Network | ✅ via Winsock2 | ✅ src/runtime/linux/ | jocky_network.h |
| Thread Pool | ✅ via ThreadPool API | ✅ src/runtime/linux/ | jocky_threadpool.h |
| Obfuscation | ✅ via LLVM | ✅ src/runtime/linux/ | jocky_obfuscation.h |
| Process Hollowing | ✅ src/runtime/windows/ | ✅ src/runtime/linux/ | N/A (separate APIs) |

---

## Common Mistakes to Avoid

❌ **DON'T:**
- Put Windows-specific code (Windows APIs, Registry, PE manipulation) in `src/runtime/linux/`
- Put Linux-specific code (syscalls, /proc, ptrace) in `src/runtime/windows/`
- Use platform-specific includes in header files
- Mix platform implementations in same .c file without proper #ifdef guards

✅ **DO:**
- Put Windows code in `src/runtime/windows/` with `test_*.c` files
- Put Linux code in `src/runtime/linux/` with `test_*.c` files
- Create generic headers in `src/runtime/include/` with portable APIs
- Use platform-agnostic patterns in headers
- Document which platform each implementation targets

---

## Adding New Features

### For Windows-Only Features (e.g., BYOUD):
```
src/runtime/include/jocky_byoud.h       ← API definition
src/runtime/windows/byoud.c             ← Implementation
src/runtime/windows/test_byoud.c        ← Tests
```

### For Linux-Only Features (e.g., syscall wrappers):
```
src/runtime/include/jocky_*.h           ← API definition
src/runtime/linux/*.c                   ← Implementation
src/runtime/linux/test_*.c              ← Tests
```

### For Cross-Platform Features (e.g., crypto):
```
src/runtime/include/jocky_crypto.h      ← Shared API
src/runtime/windows/crypto_win.c        ← Windows implementation
src/runtime/linux/crypto.c              ← Linux implementation
src/runtime/windows/test_crypto.c       ← Windows tests
src/runtime/linux/test_crypto.c         ← Linux tests
```

---

**Summary:** 
- 🪟 **Windows files** → `src/runtime/windows/`
- 🐧 **Linux files** → `src/runtime/linux/`
- 📋 **Shared headers** → `src/runtime/include/`
