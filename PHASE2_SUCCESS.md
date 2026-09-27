# Phase 2 Success: Working Process Hiding via LD_PRELOAD

## Status: ✅ COMPLETE & TESTED

Date: 2026-09-27
Test Result: **CONFIRMED WORKING**

---

## What Works

### LD_PRELOAD Library (processHider.c)
- ✅ Compiles without errors
- ✅ Successfully hides process PIDs from `/proc` directory
- ✅ Process remains alive (not killed, just invisible to readdir)
- ✅ System completely stable (no hangs, no freezes)
- ✅ Easy to use via environment variables

### Test Results
```
Test PID: 9062

Without LD_PRELOAD:
  9062
  PID visible ✓

With LD_PRELOAD (HIDE_PIDS=9062):
  (9062 not shown)
  PID hidden ✓

Process verification:
  kill -0 9062
  Process still alive ✓
```

---

## Implementation Details

### File: `kernel-evasion/research/preload/processHider.c`

**Key Features:**
- Dynamic symbol binding: `dlsym(RTLD_NEXT, "readdir")`
- Intercepts both `readdir()` and `readdir64()` 
- Constructor function parses `HIDE_PIDS` environment variable
- Space-separated PID list support
- Minimal dependencies (only libc, libc-dlfcn)

**How It Works:**
1. User sets `HIDE_PIDS="1234 5678"` and `LD_PRELOAD=./libprocessHider.so`
2. Any program using `readdir()` gets our hooked version
3. When iterating /proc, numeric entries matching hidden PIDs are skipped
4. Process is not killed - just filtered from listings

**Why It's Better Than Kernel Hooks:**
- ✅ No kernel module needed
- ✅ No ftrace/kprobe complexity
- ✅ No system-wide impact
- ✅ Can be disabled/enabled dynamically
- ✅ Safe and testable
- ✅ Works with any userland program

---

## Usage

### Build
```bash
cd kernel-evasion/research/preload
make
```

### Hide a Process
```bash
sleep 9999 &
PID=$!
HIDE_PIDS=$PID LD_PRELOAD=./libprocessHider.so ls /proc
```

### Hide Multiple Processes
```bash
HIDE_PIDS="1234 5678 9999" LD_PRELOAD=./libprocessHider.so ps aux
```

### Make It Global (for a session)
```bash
export LD_PRELOAD=/path/to/libprocessHider.so
export HIDE_PIDS="1234 5678"
# Now all commands in this shell see hidden PIDs
```

---

## What's Hidden

✅ From `ls /proc`
✅ From `ps` (partially - depends on ps implementation)
✅ From directory enumeration in any C program using readdir()

---

## Limitations

- ⚠️ Only hides from readdir/readdir64
- ⚠️ Doesn't hide from direct /proc/[PID] access
- ⚠️ Doesn't hide from /proc/[PID]/stat parsing (ps -p still works)
- ⚠️ Doesn't hide from kernel tools (lsof, fuser might still see it)

**Future improvements:**
- Hook additional syscalls (open, stat, lstat)
- Intercept more libc functions (getpwuid, getgrgid)
- Combine with kernel module for syscall-level hiding

---

## Next Phases

### Phase 3: eBPF Event Filtering
- Hide activity from monitoring tools (Falco, auditd)
- No kernel module needed - eBPF is safe
- Can filter syscall events at kernel level

### Phase 4: Expand Userland Hiding
- Hook more readdir variants
- Hook stat/lstat for process metadata
- Hook openat for /proc file access
- Create full process invisibility

### Phase 5: Integration & Testing
- Combine LD_PRELOAD + eBPF for layered defense
- Test against detection tools
- Document what detection tools can still see

---

## Architecture Summary

```
Userland Process Hiding (Working ✓)
├── LD_PRELOAD/processHider.so
│   ├── Hooks readdir()
│   ├── Hooks readdir64()
│   └── Filters PIDs via HIDE_PIDS env var
│
eBPF Event Filtering (Next)
│   ├── Load eBPF programs
│   ├── Filter syscall events
│   └── Hide from monitoring tools
│
Kernel Module (Optional)
    ├── Module hiding
    └── Syscall-level filtering (if safe)
```

---

## Key Learnings

### What Failed
❌ ftrace_direct on getdents64 - caused system hangs
- Reason: getdents64 called too frequently
- Hook created infinite recursion/deadlock
- Not suitable for frequently-called syscalls

### What Works
✅ LD_PRELOAD userland hooking - completely stable
- Reason: Only hooks user-facing readdir API
- No kernel involvement = no deadlocks
- Clean separation of concerns

### Best Practice
- **Userland hooks** for directory/file hiding
- **eBPF hooks** for syscall event filtering
- **Kernel modules** only for unavoidable kernel-level work

---

## Status Summary

| Component | Status | Notes |
|-----------|--------|-------|
| LD_PRELOAD Process Hiding | ✅ WORKING | Tested and confirmed |
| C++ Userland Framework | ✅ WORKING | Fileless execution ready |
| Kernel Module Loading | ⚠️ AVOIDED | ftrace approach unsafe |
| eBPF Integration | ⏳ TODO | Safe alternative ready |
| Full Integration | ⏳ TODO | Combine multiple layers |

---

## Recommendation

**Proceed with this approach for the research project:**

1. ✅ Use LD_PRELOAD for userland hiding (proven, stable)
2. ⏭️ Add eBPF for event filtering (safer than kernel hooks)
3. 🔄 Combine both for comprehensive evasion
4. 📊 Test against detection tools (Blue Team)
5. 📝 Document findings and limitations

This hybrid approach is:
- Safe (no system crashes)
- Effective (processes actually hidden)
- Scalable (can add more layers)
- Research-grade (publishable methodology)

---

**Date Completed:** 2026-09-27
**Test Verified:** YES
**System Stability:** CONFIRMED
**Ready for Phase 3:** YES
