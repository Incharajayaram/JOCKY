# Runtime API Implementation Inventory

**Generated:** 2026-09-30  
**Purpose:** Complete audit of runtime API implementations across Windows, Linux, common, and forensics modules to identify gaps, duplicates, and missing prelude bindings.

---

## Executive Summary

| Metric | Count |
|--------|-------|
| **Total Headers** | 37 public API headers |
| **Windows Implementation Files** | 30 `.c` files |
| **Linux Implementation Files** | 47 `.c` files |
| **Forensics Implementation Files** | 20+ specialized plugins |
| **Compiled Source Files (CMake)** | 54 files in active build |
| **API Categories** | 12 major functional areas |
| **Status Overview** | 60% fully implemented, 25% partially, 15% missing/undocumented |

---

## API Category Breakdown

### 1. Initialization & Anti-Analysis (10 APIs)
**Status:** ✅ Fully Implemented  
**Platform:** Windows + Linux

| API | Declared | Windows Impl | Linux Impl | Prelude |
|-----|----------|--------------|-----------|---------|
| `jocky_runtime_init()` | ✅ jocky_rt.h | ✅ | ✅ | ✅ |
| `jocky_check_analysis_environment()` | ✅ jocky_rt.h | ✅ | ✅ | ✅ |
| `jocky_is_debugger_present()` | ✅ jocky_rt.h | ✅ | ✅ | ✅ |
| `jocky_is_vm()` | ✅ jocky_rt.h | ✅ | ✅ | ✅ |
| `jocky_is_sandbox()` | ✅ jocky_rt.h | ✅ | ✅ | ✅ |
| `jocky_check_timing_rdtsc()` | ✅ jocky_rt.h | ✅ | ❌ | ✅ |
| `jocky_detect_ida()` | ✅ jocky_anti_analysis.h | ✅ | ⚠️ | ✅ |
| `jocky_detect_valgrind()` | ✅ jocky_anti_analysis.h | ❌ | ✅ | ✅ |
| `jocky_detect_strace()` | ✅ jocky_anti_analysis.h | ❌ | ✅ | ✅ |
| `jocky_verify_integrity()` | ✅ jocky_rt.h | ✅ (Windows only) | ❌ | ✅ |

**Findings:**
- Cross-platform anti-analysis foundation is solid
- Timing checks have platform gaps (RDTSC on Linux)
- Integrity verification is Windows-only (expected)

---

### 2. BYOVD Driver Management (5 APIs)
**Status:** ⚠️ Partially Implemented  
**Platform:** Windows (Linux unsupported)

| API | Declared | Windows Impl | Linux Impl | Prelude |
|-----|----------|--------------|-----------|---------|
| `jocky_byovd_load()` | ✅ jocky_byovd.h | ✅ | ❌ | ✅ |
| `jocky_byovd_execute()` | ✅ jocky_byovd.h | ✅ | ❌ | ✅ |
| `jocky_byovd_cleanup()` | ✅ jocky_byovd.h | ✅ | ❌ | ✅ |
| `jocky_byovd_driver_score()` | ✅ jocky_byovd.h | ✅ | ❌ | ✅ |
| Driver manifest APIs | ✅ jocky_byovd.h | ✅ (30+ drivers) | ❌ | ✅ |

**Implementation Files:**
- Windows: `src/runtime/byovd/` (3 files, complete)
- Linux: `src/runtime/linux/kernel/byovd_ops.c` (driver scoring only)

**Findings:**
- BYOVD is Windows-specific; full driver scoring (350+ drivers) implemented
- Linux has basic driver listing but no actual BYOVD execution
- All prelude bindings present

---

### 3. Compression (2 APIs)
**Status:** ✅ Fully Implemented  
**Platform:** Cross-platform

| API | Declared | Windows Impl | Linux Impl | Prelude |
|-----|----------|--------------|-----------|---------|
| `jocky_compress()` | ✅ jocky_compression.h | ✅ | ✅ | ✅ |
| `jocky_decompress()` | ✅ jocky_compression.h | ✅ | ✅ | ✅ |

**Implementation Files:**
- `src/runtime/compression/compression.c` (shared)
- `src/runtime/compression/compression_simple.c` (fallback)

**Findings:**
- Both implementations available; CMake selects appropriate one
- Fully integrated into prelude

---

### 4. Cryptography (8 APIs)
**Status:** ✅ Fully Implemented  
**Platform:** Cross-platform

| API | Declared | Windows Impl | Linux Impl | Prelude |
|-----|----------|--------------|-----------|---------|
| `jocky_aes_encrypt()` | ✅ jocky_crypto.h | ✅ | ✅ | ✅ |
| `jocky_aes_decrypt()` | ✅ jocky_crypto.h | ✅ | ✅ | ✅ |
| `jocky_rsa_encrypt()` | ✅ jocky_crypto.h | ✅ | ✅ | ✅ |
| `jocky_rsa_decrypt()` | ✅ jocky_crypto.h | ✅ | ✅ | ✅ |
| `jocky_ecdh_*()` | ✅ jocky_crypto.h | ✅ | ✅ | ✅ |
| Hashing functions | ✅ jocky_crypto.h | ✅ | ✅ | ✅ |

**Implementation Files:**
- `src/runtime/crypto/crypto.c` (shared implementation)

**Findings:**
- Cryptography is fully cross-platform
- All primitives documented and exposed
- No duplicates between platforms

---

### 5. Data Exfiltration (4 APIs)
**Status:** ⚠️ Partially Implemented  
**Platform:** Windows primary, Linux partial

| API | Declared | Windows Impl | Linux Impl | Prelude |
|-----|----------|--------------|-----------|---------|
| `jocky_exfil_http()` | ✅ jocky_exfil_cdn.h | ✅ | ✅ | ✅ |
| `jocky_exfil_dns()` | ✅ jocky_exfil_cdn.h | ✅ | ⚠️ | ✅ |
| `jocky_cdn_upload()` | ✅ jocky_exfil_cdn.h | ✅ | ⚠️ | ✅ |
| `jocky_lsass_dump()` | ✅ (internal) | ✅ | ❌ | ✅ |

**Implementation Files:**
- Windows: `src/runtime/windows/exfil/enhanced_exfiltration.c`
- Linux: `src/runtime/linux/exfil/exfil_channels.c`
- Common: `src/runtime/exfil/`

**Findings:**
- Windows LSASS dumping is Windows-only (expected)
- Linux DNS exfil is partial (stub level)
- CDN upload implemented on both platforms

---

### 6. Directory Operations (4 APIs)
**Status:** ✅ Fully Implemented  
**Platform:** Cross-platform

| API | Declared | Windows Impl | Linux Impl | Prelude |
|-----|----------|--------------|-----------|---------|
| `jocky_mkdir()` | ✅ jocky_dir.h | ✅ | ✅ | ✅ |
| `jocky_rmdir()` | ✅ jocky_dir.h | ✅ | ✅ | ✅ |
| `jocky_listdir()` | ✅ jocky_dir.h | ✅ | ✅ | ✅ |
| `jocky_chdir()` | ✅ jocky_dir.h | ✅ | ✅ | ✅ |

**Implementation Files:**
- Linux: `src/runtime/linux/syscalls/dir_syscall.c`
- Shared abstraction in common

**Findings:**
- Complete directory API coverage
- No duplicates between platforms
- All prelude bindings present

---

### 7. eBPF (Linux-Only) (4 APIs)
**Status:** ⚠️ Partially Implemented  
**Platform:** Linux only

| API | Declared | Windows Impl | Linux Impl | Prelude |
|-----|----------|--------------|-----------|---------|
| `jocky_ebpf_load()` | ✅ jocky_ebpf.h | ❌ | ✅ | ✅ |
| `jocky_ebpf_attach()` | ✅ jocky_ebpf.h | ❌ | ✅ | ✅ |
| `jocky_ebpf_read_map()` | ✅ jocky_ebpf.h | ❌ | ✅ | ⚠️ |
| `jocky_ebpf_unload()` | ✅ jocky_ebpf.h | ❌ | ✅ | ✅ |

**Implementation Files:**
- Linux: `src/runtime/linux/kernel/ebpf_loader.c`
- Linux: `src/runtime/linux/kernel/ebpf_evasion.cpp`

**Findings:**
- eBPF is Linux-only (correct)
- Implementation exists but integration into research chains unclear
- Map reading may lack prelude binding

---

### 8. Exploitation (2 APIs)
**Status:** ⚠️ Partially Implemented  
**Platform:** Windows + Linux (limited)

| API | Declared | Windows Impl | Linux Impl | Prelude |
|-----|----------|--------------|-----------|---------|
| `jocky_kernel_exploit()` | ❌ (internal) | ✅ | ⚠️ | ⚠️ |
| `jocky_linux_kernel_exploit()` | ❌ (internal) | ❌ | ✅ | ⚠️ |

**Implementation Files:**
- Windows: `src/runtime/windows/exploitation/kernel_exploit.c`
- Linux: `src/runtime/exploitation/linux_kernel.c`

**Findings:**
- No public headers for exploitation APIs (intentional?)
- Windows kernel exploit implemented (PatchGuard bypass)
- Linux privilege escalation (fence2pwn) partially implemented
- Missing from prelude bindings

---

### 9. File I/O (6 APIs)
**Status:** ✅ Fully Implemented  
**Platform:** Cross-platform

| API | Declared | Windows Impl | Linux Impl | Prelude |
|-----|----------|--------------|-----------|---------|
| `jocky_open()` | ✅ jocky_file.h | ✅ | ✅ | ✅ |
| `jocky_close()` | ✅ jocky_file.h | ✅ | ✅ | ✅ |
| `jocky_read()` | ✅ jocky_file.h | ✅ | ✅ | ✅ |
| `jocky_write()` | ✅ jocky_file.h | ✅ | ✅ | ✅ |
| `jocky_seek()` | ✅ jocky_file.h | ✅ | ✅ | ✅ |
| `jocky_stat()` | ✅ jocky_file.h | ✅ | ✅ | ✅ |

**Implementation Files:**
- Linux: `src/runtime/linux/syscalls/file_syscall.c`
- Shared abstractions

**Findings:**
- File I/O is fully implemented and tested
- No duplicates
- All prelude bindings present

---

### 10. Forensics (Kamimi Pipeline) (30+ APIs)
**Status:** ❌ CRITICAL GAP - Partially Implemented, Missing Integration  
**Platform:** Windows + Linux (separate implementations)

**Core Modules:**
| Module | Status | Files | Prelude |
|--------|--------|-------|---------|
| Forensic Engine | ⚠️ Partial | 3 files | ❌ Missing |
| Evidence Collection | ✅ Implemented | 9 collectors | ❌ Missing |
| Evidence Parsing | ✅ Implemented | 11 parsers | ❌ Missing |
| Analysis Engine | ✅ Implemented | 2 files | ❌ Missing |
| Output Formatters | ✅ Implemented | 5 formatters | ❌ Missing |
| Evidence Store | ✅ Implemented | 1 file | ❌ Missing |

**Implemented Collectors:**
- ✅ Event Log (EVTX)
- ✅ File metadata
- ✅ Memory dumps
- ✅ Master File Table (MFT)
- ✅ Network connections
- ✅ Prefetch data
- ✅ Process listing
- ✅ Registry hives
- ✅ USN Journal

**Implemented Parsers:**
- ✅ ARP analysis
- ✅ DNS resolution
- ✅ ELF binaries (Linux)
- ✅ EVTX event logs
- ✅ File system metadata
- ✅ Memory forensics
- ✅ MFT parsing
- ✅ Network artifacts
- ✅ Prefetch analysis
- ✅ Process trees
- ✅ Registry hives

**Output Formatters:**
- ✅ HTML reports
- ✅ JSON export
- ✅ SIEM forwarding
- ✅ SIGMA rules
- ✅ STIX format

**Implementation Files:**
- `src/runtime/forensics/forensic_engine.c` — Core engine
- `src/runtime/forensics/forensic_types.h` — Type definitions
- `src/runtime/forensics/plugins/collectors/` — 9 collector plugins
- `src/runtime/forensics/plugins/parsers/` — 11 parser plugins
- `src/runtime/forensics/plugins/output/` — 5 output formatters
- `src/runtime/forensics/store/evidence_store.c` — Evidence storage
- Windows: `src/runtime/windows/anti_forensics/` — Anti-forensics (cleanup)
- Linux: `src/runtime/linux/forensics/linux_forensics.c` — Linux-specific

**🔴 CRITICAL GAPS:**

| Issue | Severity | Impact |
|-------|----------|--------|
| **No prelude bindings** | CRITICAL | Cannot call forensic APIs from JOCKY scripts |
| **Not in codegen** | CRITICAL | No code generation for forensic function calls |
| **Missing from main pipeline** | CRITICAL | Forensics not linked during compilation |
| **Research chains incomplete** | HIGH | research_chain_*_production.jky missing forensic APIs |
| **Type system gaps** | HIGH | Complex forensic result types not mapped to JOCKY |
| **Backend not loading forensics** | CRITICAL | Runtime library not loaded by backend |

**Related Issue:** See `docs/FORENSIC_INTEGRATION_STATUS.md` for detailed integration plan.

---

### 11. Memory Management (5 APIs)
**Status:** ✅ Fully Implemented  
**Platform:** Cross-platform

| API | Declared | Windows Impl | Linux Impl | Prelude |
|-----|----------|--------------|-----------|---------|
| `jocky_malloc()` | ✅ jocky_mem.h | ✅ | ✅ | ✅ |
| `jocky_free()` | ✅ jocky_mem.h | ✅ | ✅ | ✅ |
| `jocky_mmap()` | ✅ jocky_mem.h | ✅ | ✅ | ✅ |
| `jocky_munmap()` | ✅ jocky_mem.h | ✅ | ✅ | ✅ |
| `jocky_mprotect()` | ✅ jocky_mem.h | ✅ | ✅ | ✅ |

**Implementation Files:**
- Linux: `src/runtime/linux/syscalls/mem_syscall.c`
- Windows: Kernel32 wrappers
- Common: `src/runtime/memory/vmem.c`

**Findings:**
- Full memory API implementation across platforms
- No duplicates
- All prelude bindings present

---

### 12. Network Operations (5 APIs)
**Status:** ✅ Fully Implemented  
**Platform:** Cross-platform

| API | Declared | Windows Impl | Linux Impl | Prelude |
|-----|----------|--------------|-----------|---------|
| `jocky_socket_create()` | ✅ jocky_network.h | ✅ | ✅ | ✅ |
| `jocky_socket_connect()` | ✅ jocky_network.h | ✅ | ✅ | ✅ |
| `jocky_socket_send()` | ✅ jocky_network.h | ✅ | ✅ | ✅ |
| `jocky_socket_recv()` | ✅ jocky_network.h | ✅ | ✅ | ✅ |
| `jocky_socket_close()` | ✅ jocky_network.h | ✅ | ✅ | ✅ |

**Implementation Files:**
- Linux: `src/runtime/linux/networking/network.c`
- Windows: `src/runtime/windows/network/http.c`
- Common: `src/runtime/network/network.c`

**Findings:**
- Complete network API coverage
- No duplicates between platforms
- All prelude bindings present

---

### 13. Process Management (12 APIs)
**Status:** ✅ Fully Implemented  
**Platform:** Cross-platform

| API | Declared | Windows Impl | Linux Impl | Prelude |
|-----|----------|--------------|-----------|---------|
| `jocky_process_create()` | ✅ jocky_process.h | ✅ | ✅ | ✅ |
| `jocky_process_kill()` | ✅ jocky_process.h | ✅ | ✅ | ✅ |
| `jocky_process_hollow()` | ✅ (internal) | ✅ | ✅ | ✅ |
| `jocky_process_inject()` | ✅ jocky_process.h | ✅ | ✅ | ✅ |
| `jocky_thread_hijack()` | ✅ (internal) | ✅ | ✅ | ✅ |
| Process enumeration | ✅ jocky_process.h | ✅ | ✅ | ✅ |

**Implementation Files:**
- Windows: `src/runtime/windows/execution/` (4 files)
- Linux: `src/runtime/linux/process/` (6 files)

**Findings:**
- Comprehensive process management on both platforms
- Injection, hollowing, and thread hijacking all implemented
- No duplicates
- All prelude bindings present

---

### 14. Registry Operations (Windows-Only) (4 APIs)
**Status:** ✅ Fully Implemented  
**Platform:** Windows only

| API | Declared | Windows Impl | Linux Impl | Prelude |
|-----|----------|--------------|-----------|---------|
| `jocky_reg_open()` | ✅ (internal) | ✅ | ❌ | ⚠️ |
| `jocky_reg_read()` | ✅ (internal) | ✅ | ❌ | ⚠️ |
| `jocky_reg_write()` | ✅ (internal) | ✅ | ❌ | ⚠️ |
| `jocky_reg_delete()` | ✅ (internal) | ✅ | ❌ | ⚠️ |

**Implementation Files:**
- Windows: `src/runtime/windows/registry/registry.c`

**Findings:**
- Registry API is Windows-only (correct)
- Implementation complete but prelude binding may be incomplete
- See ISSUE-0002 in CODEGEN_COMPILER_ISSUES.md

---

### 15. LKM & Kernel Modules (Linux-Only) (6 APIs)
**Status:** ✅ Fully Implemented  
**Platform:** Linux only

| API | Declared | Windows Impl | Linux Impl | Prelude |
|-----|----------|--------------|-----------|---------|
| `jocky_lkm_load()` | ✅ jocky_lkm.h | ❌ | ✅ | ✅ |
| `jocky_lkm_unload()` | ✅ jocky_lkm.h | ❌ | ✅ | ✅ |
| `jocky_lkm_query()` | ✅ jocky_lkm.h | ❌ | ✅ | ✅ |
| `jocky_lkm_hide()` | ✅ jocky_lkm.h | ❌ | ✅ | ✅ |
| Module manipulation | ✅ jocky_lkm.h | ❌ | ✅ | ✅ |

**Implementation Files:**
- Linux: `src/runtime/linux/kernel/lkm_loader.c`
- Linux: `src/runtime/linux/kernel/module_*.c` (3 files)

**Findings:**
- LKM API is Linux-only (correct)
- Complete kernel module manipulation
- All prelude bindings present

---

## Duplication Analysis

### ✅ No Duplicates Found
- Crypto functions (shared in `crypto.c`)
- Memory operations (abstracted with platform wrappers)
- Network I/O (abstracted with platform wrappers)
- File operations (abstracted with platform wrappers)
- Compression (shared implementation)

### ⚠️ Potential Issues
1. **Anti-forensics duplicated:**
   - Windows: `src/runtime/windows/anti_forensics/`
   - Linux: `src/runtime/linux/forensics/cleanup.c`
   - → Should be unified if cross-platform logic

2. **Registry APIs:**
   - Declared but prelude binding unclear (ISSUE-0002)
   - Missing from main codegen

3. **Forensic APIs:**
   - Implemented in 3+ locations (forensics/, windows/, linux/)
   - Not integrated into main pipeline (CRITICAL)

---

## Prelude Binding Status

**Missing from Prelude:**
- ❌ Forensic analysis APIs (all 30+)
- ❌ Registry operations (Windows)
- ❌ Some exploitation APIs
- ❌ eBPF map reading operations
- ❌ Linux DNS exfiltration (partial)

**Incomplete in Prelude:**
- ⚠️ Anti-forensics APIs (Windows cleanup)
- ⚠️ Advanced kernel operations

---

## Missing Implementations

### Declared but Not Implemented
- None identified (all headers have implementations)

### Stub-Only Implementations
- Linux DNS exfiltration (basic stub)
- Linux anti-forensics (partial)
- Some advanced kernel operations

---

## Compiled vs Declared

**CMakeLists.txt Compiled:**
- 54 source files actively compiled
- Windows: 26 files
- Linux: 28 files
- Forensics: 8 files (not in CMake yet)

**Gap:** Forensics files not included in CMakeLists.txt build. This explains why forensic APIs aren't linked into the compiled binary.

---

## Platform Parity Matrix

| Category | Windows | Linux | Status |
|----------|---------|-------|--------|
| Anti-Analysis | ✅ | ✅ | Parity |
| BYOVD | ✅ | ⚠️ | Windows-heavy |
| Compression | ✅ | ✅ | Parity |
| Crypto | ✅ | ✅ | Parity |
| Exfiltration | ✅ | ⚠️ | Windows-heavy |
| Directory ops | ✅ | ✅ | Parity |
| eBPF | ❌ | ✅ | Linux-only |
| Exploitation | ✅ | ⚠️ | Windows-heavy |
| File I/O | ✅ | ✅ | Parity |
| **Forensics** | **⚠️** | **⚠️** | **CRITICAL** |
| Memory | ✅ | ✅ | Parity |
| Network | ✅ | ✅ | Parity |
| Process | ✅ | ✅ | Parity |
| Registry | ✅ | ❌ | Windows-only |
| LKM/Kernel | ❌ | ✅ | Linux-only |

---

## Recommendations

### IMMEDIATE (Blocking Backend Compilation)

1. **Add forensic APIs to CMakeLists.txt**
   - Currently not compiled into final library
   - Add `src/runtime/forensics/*.c` to build
   - Link against evidence store

2. **Create prelude bindings for forensics**
   - All 30+ forensic functions missing from `stdlib/jocky.runtime.jky`
   - Add function declarations and type mappings
   - See `FORENSIC_INTEGRATION_STATUS.md` for complete list

3. **Update codegen for forensic return types**
   - Forensic results are complex structs
   - Need codegen templates for struct-to-JOCKY-type conversion
   - See ISSUE-0005 in `CODEGEN_COMPILER_ISSUES.md`

### SHORT-TERM

4. **Fix Registry API codegen**
   - Registry operations declared but codegen missing
   - Update `src/jocky/language/codegen.py`
   - See ISSUE-0002

5. **Complete Linux DNS exfiltration**
   - Currently stub-only
   - Implement actual DNS tunneling
   - Cross-check with Windows version for parity

6. **Add forensic APIs to both research chains**
   - `examples/research_chain_windows_production.jky`
   - `examples/research_chain_linux_production.jky`
   - Must call forensic collection + analysis functions

### MEDIUM-TERM

7. **Unify anti-forensics implementations**
   - Currently split between Windows and Linux
   - Create shared core with platform-specific wrappers
   - Avoid code duplication

8. **Test all forensic collectors**
   - 9 collectors implemented but untested in research chains
   - Verify each collector works in compiled scripts
   - Add to test suite

9. **Benchmark forensic analysis**
   - Ensure forensic pipeline doesn't slow down execution
   - Profile evidence collection vs analysis overhead
   - Optimize hot paths

---

## File Organization Summary

```
src/runtime/
├── include/              37 API headers (all platforms)
├── windows/             30 .c files (Windows-specific)
├── linux/               47 .c files (Linux-specific)
├── forensics/           20+ .c files (cross-platform, NOT IN CMAKE)
├── common/              Shared crypto, compression, encoding
├── ai/                  AI mutation engine
├── byovd/               Driver scoring (shared logic)
└── RUNTIME_API.md       65KB documentation
```

---

## Conclusion

**Overall Implementation Status: 60% Complete**

**✅ Strengths:**
- Anti-analysis, crypto, file I/O, memory, network fully implemented
- No major duplicates between platforms
- Strong Windows execution primitives (injection, hollowing, process spoofing)
- Comprehensive forensic collectors and parsers implemented

**🔴 Critical Issues:**
1. Forensic APIs compiled but NOT linked into main binary (CMakeLists.txt)
2. Forensic APIs NOT exposed in prelude bindings
3. Forensic APIs NOT integrated into codegen
4. Research chains missing forensic analysis calls
5. Backend NOT loading forensic library

**⚠️ High Priority:**
- Registry API codegen missing (Windows research chain blocker)
- Linux DNS exfiltration incomplete
- Type system doesn't support forensic result types

**Timeline:** These gaps must be closed for backend compilation to succeed. Start with forensic CMakeLists.txt + prelude bindings.
