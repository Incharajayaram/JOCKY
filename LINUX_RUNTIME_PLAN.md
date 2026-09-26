# Linux Runtime Library Implementation Plan

**Target:** 116 → 140+ features (90% → 95%+)
**Scope:** Essential Linux runtime primitives (before Tier 2 polishing)
**Estimated Effort:** 8-12 days of focused development

---

## Priority Phases

### PHASE 1: CRITICAL FOUNDATION (3-4 days)

Essential for other modules to build on.

#### 1.1 System Information API (1 day)
**Files:**
- `src/runtime/include/jocky_sysinfo.h`
- `src/runtime/linux/sysinfo.c`
- `tests/unit/test_sysinfo.c`

**Functions:**
```c
jocky_sysinfo_cpu()          // CPU info (cores, brand, flags)
jocky_sysinfo_memory()       // Memory stats (total, free, available)
jocky_sysinfo_uptime()       // System uptime in seconds
jocky_sysinfo_hostname()     // Machine hostname
jocky_sysinfo_osversion()    // Kernel version and distro
jocky_sysinfo_arch()         // CPU architecture
```

**Impact:** Foundation for reconnaissance, process management

**Tests:** 8+ unit tests covering:
- CPU detection
- Memory calculation
- Version parsing
- Edge cases

---

#### 1.2 Process Information API (1 day)
**Files:**
- `src/runtime/include/jocky_process.h`
- `src/runtime/linux/process.c`
- `tests/unit/test_process.c`

**Functions:**
```c
jocky_enum_processes()       // List all running processes
jocky_process_info()         // Get PID, name, cmdline, memory
jocky_process_parent()       // Get parent PID
jocky_get_current_pid()      // Get own PID
jocky_get_current_tid()      // Get own thread ID
jocky_process_exists()       // Check if PID is running
jocky_get_executable_path()  // Get own executable path
```

**Impact:** Process enumeration, OPSEC checks, parent spoofing

**Tests:** 10+ unit tests covering:
- Process listing from /proc
- Parent-child relationships
- Path resolution
- Current process info

---

#### 1.3 Environment & Configuration API (0.5 day)
**Files:**
- `src/runtime/include/jocky_env.h`
- `src/runtime/linux/env.c`

**Functions:**
```c
jocky_getenv()              // Get environment variable
jocky_setenv()              // Set environment variable
jocky_unsetenv()            // Unset environment variable
jocky_getuid()              // Get UID
jocky_geteuid()             // Get effective UID
jocky_getgid()              // Get GID
jocky_getegid()             // Get effective GID
jocky_get_username()        // Get username from UID
jocky_get_home_dir()        // Get home directory
```

**Impact:** Privilege awareness, anti-analysis baseline

---

#### 1.4 File Path API (0.5 day)
**Files:**
- `src/runtime/include/jocky_path.h`
- `src/runtime/linux/path.c`

**Functions:**
```c
jocky_realpath()            // Resolve symlinks
jocky_abspath()             // Get absolute path
jocky_dirname()             // Get directory component
jocky_basename()            // Get filename component
jocky_get_cwd()             // Get working directory
jocky_get_temp_dir()        // Get /tmp or equivalent
jocky_path_exists()         // Check if path exists
jocky_is_absolute()         // Check if path is absolute
```

**Impact:** File system navigation, payload extraction paths

---

### PHASE 2: FILE SYSTEM OPERATIONS (2 days)

File attributes and directory traversal.

#### 2.1 File Attributes API (1 day)
**Files:**
- `src/runtime/include/jocky_file.h` (extend)
- `src/runtime/linux/file.c` (extend)

**Functions:**
```c
jocky_file_stat()           // Get file stats (size, permissions, times)
jocky_file_chmod()          // Change permissions
jocky_file_chown()          // Change owner (if possible)
jocky_file_mtime()          // Get modification time
jocky_file_atime()          // Get access time
jocky_file_ctime()          // Get change time
jocky_file_type()           // Get file type (regular, dir, symlink, etc.)
jocky_file_size()           // Get file size
jocky_file_is_readable()    // Check if readable
jocky_file_is_writable()    // Check if writable
```

**Impact:** Forensics evasion, timestamp manipulation

---

#### 2.2 Directory Operations API (1 day)
**Files:**
- `src/runtime/include/jocky_dir.h`
- `src/runtime/linux/dir.c`

**Functions:**
```c
jocky_mkdir()               // Create directory
jocky_rmdir()               // Remove directory
jocky_chdir()               // Change working directory
jocky_readdir()             // List directory contents
jocky_find_files()          // Find files matching pattern
jocky_walkdir()             // Recursive directory traversal
jocky_delete_tree()         // Recursively delete directory
```

**Impact:** Artifact cleanup, malware deployment

---

### PHASE 3: MEMORY MANAGEMENT (2 days)

Advanced memory operations.

#### 3.1 Memory Mapping API (1 day)
**Files:**
- `src/runtime/include/jocky_mmap.h`
- `src/runtime/linux/mmap.c`

**Functions:**
```c
jocky_mmap()                // Allocate mapped memory
jocky_munmap()              // Unmap memory
jocky_mprotect()            // Change protection
jocky_msync()               // Synchronize mappings
jocky_madvise()             // Provide mapping advice
jocky_mlockall()            // Lock pages in memory
jocky_munlockall()          // Unlock pages
jocky_get_pagesize()        // Get page size
```

**Impact:** ROP gadget allocation, shellcode hiding

---

#### 3.2 Process Memory API (1 day)
**Files:**
- `src/runtime/include/jocky_ptrace.h`
- `src/runtime/linux/ptrace.c`

**Functions:**
```c
jocky_ptrace_attach()       // Attach to process
jocky_ptrace_detach()       // Detach from process
jocky_ptrace_read()         // Read process memory
jocky_ptrace_write()        // Write process memory
jocky_ptrace_getreg()       // Read register
jocky_ptrace_setreg()       // Write register
jocky_ptrace_syscall()      // Trace syscalls
```

**Impact:** Process hollowing, code injection, DLL injection

---

### PHASE 4: SIGNALS & IPC (2-3 days)

Process control and inter-process communication.

#### 4.1 Signal Handling API (1 day)
**Files:**
- `src/runtime/include/jocky_signal.h`
- `src/runtime/linux/signal.c`

**Functions:**
```c
jocky_signal()              // Install signal handler
jocky_sigaction()           // Advanced signal handling
jocky_sigprocmask()         // Block/unblock signals
jocky_sigpending()          // Get pending signals
jocky_sigsuspend()          // Wait for signal
jocky_kill()                // Send signal to process
jocky_raise()               // Send signal to self
```

**Impact:** SIGCHLD handling, graceful cleanup, interrupt handling

---

#### 4.2 Process Control API (1 day)
**Files:**
- `src/runtime/include/jocky_exec.h`
- `src/runtime/linux/exec.c`

**Functions:**
```c
jocky_fork()                // Fork process
jocky_fork_with_flags()     // Fork with specific flags
jocky_execve()              // Execute program
jocky_execvp()              // Execute program (search PATH)
jocky_waitpid()             // Wait for process
jocky_get_exit_status()     // Get exit code
```

**Impact:** Payload staging, process launching, privilege dropping

---

#### 4.3 Capabilities API (0.5 day - Linux-specific)
**Files:**
- `src/runtime/include/jocky_cap.h`
- `src/runtime/linux/cap.c`

**Functions:**
```c
jocky_getcap()              // Get current capabilities
jocky_setcap()              // Set capabilities
jocky_drop_caps()           // Drop all unnecessary capabilities
jocky_cap_to_string()       // Convert cap to string
```

**Impact:** Least privilege principle, SUID dropping

---

#### 4.4 IPC Primitives (1 day)
**Files:**
- `src/runtime/include/jocky_ipc.h`
- `src/runtime/linux/ipc.c`

**Functions:**
```c
jocky_pipe()                // Create pipe
jocky_mkfifo()              // Create named pipe
jocky_unix_socket()         // Create Unix domain socket
jocky_socketpair()          // Create socket pair
jocky_shm_open()            // Open shared memory
jocky_shm_unlink()          // Remove shared memory
jocky_sem_open()            // Open semaphore
jocky_sem_wait()            // Wait on semaphore
jocky_sem_post()            // Signal semaphore
```

**Impact:** Multi-process coordination, data sharing

---

### PHASE 5: SYSTEM CALLS & UTILITIES (1-2 days)

Low-level syscall wrapper and utility functions.

#### 5.1 Syscall Wrapper (0.5 day)
**Files:**
- `src/runtime/include/jocky_syscall.h`
- `src/runtime/linux/syscall.c`

**Functions:**
```c
jocky_syscall()             // Generic syscall (0-6 args)
jocky_get_syscall_number()  // Get syscall number for name
```

**Impact:** Direct kernel calls, evasion of glibc hooks

---

#### 5.2 Time & Timing Functions (0.5 day)
**Files:**
- `src/runtime/include/jocky_time.h`
- `src/runtime/linux/time.c`

**Functions:**
```c
jocky_clock_gettime()       // High-resolution time
jocky_getrusage()           // Resource usage
jocky_getrlimit()           // Resource limits
jocky_setrlimit()           // Set resource limits
jocky_sleep_ms()            // Sleep milliseconds
jocky_getpid_timing()       // Timing attack detection
```

**Impact:** Anti-analysis timing checks, accurate delays

---

#### 5.3 String & Utility Functions (0.5 day)
**Functions:**
```c
jocky_strerror()            // Convert errno to string
jocky_strcpy_safe()         // Safe string copy
jocky_strdup_safe()         // Safe string duplication
jocky_parse_cmdline()       // Parse command line
jocky_hash_string()         // String hashing
```

---

## Implementation Strategy

### 1. Modular headers
- One header per logical group (sysinfo, process, etc.)
- Clean API design with error codes
- Cross-platform stubs for Windows

### 2. Comprehensive testing
- 8-12 tests per API
- Cover happy path + error cases
- Test with actual /proc files and syscalls

### 3. Code organization
```
src/runtime/
├── include/
│   ├── jocky_sysinfo.h
│   ├── jocky_process.h
│   ├── jocky_env.h
│   ├── jocky_path.h
│   ├── jocky_file.h
│   ├── jocky_dir.h
│   ├── jocky_mmap.h
│   ├── jocky_ptrace.h
│   ├── jocky_signal.h
│   ├── jocky_exec.h
│   ├── jocky_cap.h
│   ├── jocky_ipc.h
│   └── jocky_syscall.h
└── linux/
    ├── sysinfo.c
    ├── process.c
    ├── env.c
    ├── path.c
    ├── file.c
    ├── dir.c
    ├── mmap.c
    ├── ptrace.c
    ├── signal.c
    ├── exec.c
    ├── cap.c
    ├── ipc.c
    └── syscall.c
```

### 4. Build integration
- Add to CMakeLists.txt
- Compile as part of runtime library
- Link against libc + Linux-specific libs

---

## Timeline Estimate

| Phase | Days | Lines | Tests |
|-------|------|-------|-------|
| 1: Foundation | 3-4 | 1,500-2,000 | 32+ |
| 2: File System | 2 | 1,000 | 16+ |
| 3: Memory | 2 | 1,200 | 18+ |
| 4: Signals/IPC | 2-3 | 1,500 | 24+ |
| 5: Syscalls | 1-2 | 600 | 12+ |
| **TOTAL** | **10-14** | **5,800-6,300** | **102+** |

---

## Feature Completion Impact

- **Start:** 116/129 (90%)
- **After Phase 1:** 124/129 (96%)
- **After Phase 2:** 126/129 (97%)
- **After Phase 3:** 130/129+ (overflow) → Comprehensive Linux runtime
- **After Phases 4-5:** 135+/129 (bonus features)

This brings JOCKY to a fully-featured Linux malware development platform with comprehensive runtime support.

---

## Next: Start with Phase 1
Recommended to begin with **System Information API** as it has no dependencies and enables everything else.

