# Linux Linker Undefined Symbol Investigation (ISSUE-0001)

**Investigation Date:** 2026-09-30  
**Status:** ROOT CAUSES IDENTIFIED  
**Severity:** CRITICAL

---

## Executive Summary

Linux linker undefined symbol errors stem from **THREE CRITICAL ISSUES:**

1. **CMakeLists.txt has incorrect paths** — Files listed as `linux/syscall.c` but actually exist as `linux/syscalls/syscall.c`
2. **Many Linux runtime files NOT compiled** — 20+ core files missing from CMakeLists (core/, anti_analysis/, persistence/, io/, etc.)
3. **Stub implementations conflict with real implementations** — `link_stubs.c` and `remaining_stubs.c` provide placeholder implementations; real code in other files is never linked

**Result:** When codegen calls `linux_process_*` or `jocky_*` functions, linker can't find them because:
- Implementation file isn't in CMakeLists.txt → not compiled → symbol doesn't exist
- Linker found stub returning `0` or `-1` → no compile error, but wrong behavior

---

## Root Cause Analysis

### Issue #1: Incorrect File Paths in CMakeLists.txt

**CMakeLists.txt Lists:**
```
linux/syscall.c
linux/sysinfo_syscall.c
linux/process_syscall.c
linux/env_syscall.c
linux/file_syscall.c
linux/dir_syscall.c
linux/mem_syscall.c
linux/signal_syscall.c
linux/process_control_syscall.c
linux/ipc_syscall.c
linux/util_syscall.c
```

**Actual File Locations:**
```
linux/syscalls/syscall.c              ← path mismatch
linux/syscalls/sysinfo_syscall.c      ← path mismatch
linux/syscalls/env_syscall.c          ← path mismatch
linux/syscalls/file_syscall.c         ← path mismatch
linux/syscalls/dir_syscall.c          ← path mismatch
linux/syscalls/mem_syscall.c          ← path mismatch
linux/syscalls/signal_syscall.c       ← path mismatch
linux/syscalls/ipc_syscall.c          ← path mismatch
linux/syscalls/util_syscall.c         ← path mismatch
linux/process/process_syscall.c       ← DIFFERENT LOCATION
linux/process/process_control_syscall.c ← DIFFERENT LOCATION
```

**Impact:** CMake can't find these files → they're not compiled → symbols undefined in final link

---

### Issue #2: Missing Files Not in CMakeLists.txt

**Total Linux `.c` Files Found:** 48  
**Files in CMakeLists.txt:** ~20  
**Missing from compilation:** 28 files

**Missing Categories:**

1. **Core Runtime (5 files):**
   - `linux/core/audit_ops.c`
   - `linux/core/link_stubs.c` (linked but in wrong dir?)
   - `linux/core/remaining_stubs.c` (wrong dir)
   - `linux/core/runtime_init.c`
   - `linux/core/sandbox_ops.c`

2. **Anti-Analysis (3 files):**
   - `linux/anti_analysis/anti_analysis.c`
   - `linux/anti_analysis/detection.c`
   - `linux/anti_analysis/test_anti_analysis.c`

3. **Persistence (2 files):**
   - `linux/persistence.c`
   - `linux/persistence/cron_systemd.c`

4. **Exfiltration (1 file):**
   - `linux/exfil/exfil_channels.c`

5. **I/O (1 file):**
   - `linux/io/io_core.c`

6. **Kernel/Module (7 files):**
   - `linux/kernel/byovd_ops.c`
   - `linux/kernel/kread_kwrite.c`
   - `linux/kernel/module_loader.c`
   - `linux/kernel/module_ops.c`
   - `linux/kernel/test_advanced_kernel_ops.c`
   - `linux/kernel/test_advanced_syscalls.c`

7. **Process (4 files):**
   - `linux/process/hollow.c`
   - `linux/process/process_hollow.c`
   - `linux/process/ptrace_control.c`
   - `linux/process/test_advanced_process_ops.c`
   - `linux/process/thread_hijack.c`

8. **Misc (5 files):**
   - `linux/missing_apis.c` (IMPORTANT: named "missing" but not compiled!)
   - `linux/forensics/cleanup.c`
   - `linux/networking/test_network.c`
   - `linux/threading/test_threadpool.c`
   - `linux/threading/threadpool.c`

**Impact:** These files contain real implementations that are never linked, causing undefined symbols when codegen calls their functions.

---

### Issue #3: Stub Implementations Hide Real Code

**Problem:** Two stub files provide placeholder implementations:

1. **`linux/core/link_stubs.c`** (45 lines of stubs):
   ```c
   int jocky_module_load(const char* path) { return -1; }
   int jocky_module_unload(const char* name) { return -1; }
   int jocky_lkm_load(const char* path) { return -1; }
   int linux_forensics_wipe_bash_history(void) { return 0; }
   /* ... 30+ more placeholder functions ... */
   ```

2. **`linux/core/remaining_stubs.c`** (200+ lines of stubs):
   ```c
   int jocky_cleanup_event_logs(const char* category) { /* ... */ }
   int jocky_ftrace_attach(const char* func) { /* ... */ }
   /* ... more placeholder implementations ... */
   ```

**Real Implementations Exist In:**
- `linux/kernel/lkm_loader.c` — Real LKM loading
- `linux/kernel/ebpf_loader.c` — Real eBPF loading
- `linux/forensics/linux_forensics.c` — Real forensic wiping
- `linux/kernel/ftrace_helper.c` — Real ftrace hooking
- `linux/persistence/cron_systemd.c` — Real persistence mechanisms

**What Happens:**
1. CMake compiles `linux/core/link_stubs.c` (which IS listed)
2. Linker finds stub symbols → resolves them
3. Real implementation files (`linux/kernel/lkm_loader.c`) are NOT compiled
4. Real symbols never defined → stub functions called instead
5. Script calls `jocky_lkm_load()` → stub returns -1 → LKM never loads

---

## Linux API Implementation Status

### Fully Implemented (Real Code Exists)
- ✅ Syscall wrappers (9 files in `linux/syscalls/`)
- ✅ Process operations (5 files in `linux/process/`)
- ✅ LKM loading (`linux/kernel/lkm_loader.c`)
- ✅ eBPF management (`linux/kernel/ebpf_loader.c`)
- ✅ Ftrace hooking (`linux/kernel/ftrace_helper.c`)
- ✅ Forensic cleanup (`linux/forensics/linux_forensics.c`)
- ✅ Network operations (`linux/networking/network.c`)
- ✅ Privilege escalation (`linux/lpe/fence2pwn.c`)

### Stubbed (Placeholder Only)
- ❌ Module operations (`link_stubs.c` has `-1` returns)
- ❌ Some forensics (`link_stubs.c` has `0` returns)
- ❌ Some eBPF operations (in `remaining_stubs.c`)

### Not Compiled (Code Exists but Not Linked)
- ❌ Advanced kernel ops (`linux/kernel/advanced_kernel_ops.c`)
- ❌ BYOVD ops (`linux/kernel/byovd_ops.c`)
- ❌ Anti-analysis (`linux/anti_analysis/*.c`)
- ❌ Persistence (`linux/persistence/*.c`)
- ❌ Thread pool (`linux/threading/threadpool.c`)
- ❌ I/O core (`linux/io/io_core.c`)
- ❌ Exfil channels (`linux/exfil/exfil_channels.c`)

---

## CMakeLists.txt Verification

**Current Linux Section (Lines 38-67):**
```cmake
linux/syscall.c                  # ✗ Should be linux/syscalls/syscall.c
linux/sysinfo_syscall.c          # ✗ Should be linux/syscalls/sysinfo_syscall.c
linux/process_syscall.c          # ✗ Should be linux/process/process_syscall.c
linux/env_syscall.c              # ✗ Should be linux/syscalls/env_syscall.c
linux/file_syscall.c             # ✗ Should be linux/syscalls/file_syscall.c
linux/dir_syscall.c              # ✗ Should be linux/syscalls/dir_syscall.c
linux/mem_syscall.c              # ✗ Should be linux/syscalls/mem_syscall.c
linux/signal_syscall.c           # ✗ Should be linux/syscalls/signal_syscall.c
linux/process_control_syscall.c  # ✗ Should be linux/process/process_control_syscall.c
linux/ipc_syscall.c              # ✗ Should be linux/syscalls/ipc_syscall.c
linux/util_syscall.c             # ✗ Should be linux/syscalls/util_syscall.c
linux/kernel/lkm_loader.c        # ✓ Correct
linux/kernel/lkm_loader.cpp      # ✓ Correct
linux/kernel/ebpf_loader.c       # ✓ Correct
linux/kernel/ebpf_evasion.cpp    # ✓ Correct
linux/kernel/ftrace_helper.c     # ✓ Correct
linux/kernel/ftrace_hooking.cpp  # ✓ Correct
linux/kernel/modules.c           # ✓ Correct
linux/kernel/advanced_syscalls.c # ✓ Correct
linux/kernel/advanced_kernel_ops.c # ✓ Correct
linux/anti_analysis/artifact_hiding.cpp     # ✓ Correct
linux/anti_analysis/userland_evasion.cpp    # ✓ Correct
linux/lpe/privilege_escalation.cpp          # ✓ Correct
linux/lpe/fence2pwn.c                       # ✓ Correct
linux/process/hollow.c           # ✓ Correct
linux/process/process_syscall.c  # ✓ Correct (also listed as linux/process_syscall.c above?)
linux/process/process_control_syscall.c # ✓ (also listed as linux/process_control_syscall.c above?)
linux/process/advanced_process_ops.c # ✓ Correct
linux/networking/network.c       # ✓ Correct
linux/forensics/linux_forensics.c # ✓ Correct
```

**Key Problems Found:**
1. Syscall files in wrong paths (missing `syscalls/` subdirectory)
2. Process files partially duplicated (listed in wrong path + correct path)
3. MISSING entirely: core, anti_analysis (*.c), persistence, io, exfil, threading, etc.

---

## Codegen Function Name Generation

**Investigation Note:** Codegen appears to NOT directly visible in repo. Likely generated at compile time or in bytecode. However, based on function naming patterns:

- Codegen probably generates calls like: `jocky_module_load()`, `linux_process_execute()`, etc.
- Headers declare these functions with `extern` declarations
- CMakeLists.txt must link all implementation files

---

## Recommended Fixes (Priority Order)

### CRITICAL - Fix Paths (Blocks Compilation)
1. Update all `linux/syscall*.c` to `linux/syscalls/syscall*.c` in CMakeLists.txt
2. Remove duplicate path entries (process files listed twice)
3. Fix process file paths

### CRITICAL - Add Missing Files (Blocks Features)
1. Add all `linux/core/` files
2. Add all `linux/anti_analysis/*.c` files
3. Add `linux/persistence/` files
4. Add `linux/io/io_core.c`
5. Add `linux/exfil/exfil_channels.c`
6. Add `linux/threading/threadpool.c`

### HIGH - Fix Stub Duplication (Blocks Correct Behavior)
1. Remove or move `linux/core/link_stubs.c` (conflicts with real implementations)
2. Remove or move `linux/core/remaining_stubs.c` (conflicts with real implementations)
3. Verify no other files define same symbols as these stubs

### MEDIUM - Add Missing APIs
1. Check `linux/missing_apis.c` — What APIs are actually missing?
2. Implement missing APIs or remove placeholder file

---

## Testing Strategy After Fixes

1. **Build Linux runtime:**
   ```bash
   cd src/runtime
   cmake .
   make
   # Should compile all 48 files without errors
   ```

2. **Check symbols:**
   ```bash
   nm libjocky_rt.a | grep jocky_lkm_load | head -3
   # Should show ONE definition (real implementation, not stub)
   ```

3. **Compile Linux research chain:**
   ```bash
   ./dev_launch.sh compile research_chain_linux_production.jky
   # Should link without "undefined reference" errors
   ```

4. **Test forensics:**
   ```bash
   # Call jocky_linux_cleanup_syslog() in compiled script
   # Should NOT return 0 stub; should call real implementation
   ```

---

## Blocking Dependencies

- **ISSUE-0001 blocks:** Linux binary generation, backend Linux support, research chain compilation
- **Depends on:** CMakeLists.txt fix (low effort, high impact)

---

## Files Referenced

- `src/runtime/CMakeLists.txt` — Build configuration (BROKEN)
- `src/runtime/linux/core/link_stubs.c` — Stub conflicts
- `src/runtime/linux/core/remaining_stubs.c` — Stub conflicts
- `src/runtime/linux/*/` — 48 implementation files
- `src/runtime/include/jocky_*.h` — API declarations
