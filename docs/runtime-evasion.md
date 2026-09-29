# JOCKY Runtime Evasion Modules (Option B)

## Overview

Implemented in-memory execution and API evasion modules for the JOCKY defensive forensic framework. These modules allow the forensic agent to execute on systems with aggressive EDR/AV without being detected or blocked.

## Modules Implemented

### 1. API Unhooking (Windows) `src/runtime/evasion/unhook.c`

**Purpose:** Restore clean ntdll.dll syscalls to bypass user-mode EDR hooks.

**How it works:**
1. Get base address of currently mapped ntdll.dll
2. Open fresh ntdll.dll from `C:\Windows\System32\ntdll.dll`
3. Read entire file into memory buffer
4. Parse PE headers to find `.text` section in both copies
5. Use `NtProtectVirtualMemory` (direct syscall) to change memory protection to RWX
6. Copy clean `.text` section over hooked version
7. Restore original memory protection

**Key feature:** Uses direct syscall for `NtProtectVirtualMemory` to avoid triggering hooks during the unhooking process itself.

**API:**
```c
bool jocky_unhook_ntdll(void);
```

### 2. Process Hollowing (Windows) `src/runtime/execution/hollow.c`

**Purpose:** Replace the image of a suspended trusted process with forensic payload.

**How it works:**
1. Create target process (e.g., `notepad.exe`) in suspended state
2. Parse payload PE headers
3. Query target process PEB to get image base address
4. Unmap original image via `NtUnmapViewOfSection`
5. Allocate RWX memory at same base address
6. Write PE headers and sections
7. Update PEB ImageBaseAddress
8. Set thread context to new entry point (`Rcx`/`Eax`)
9. Resume thread

**Use case:** Execute forensic agent inside a trusted Windows process to blend with normal activity.

**API:**
```c
bool jocky_process_hollow(const wchar_t* target_path,
                          const uint8_t* payload,
                          size_t payload_size);
```

### 3. Reflective DLL Injection (Windows) `src/runtime/execution/reflective.c`

**Purpose:** Load PE/DLL entirely from memory without touching disk.

**How it works:**
1. Parse PE headers from memory buffer
2. Allocate RWX memory for the image
3. Copy headers and all sections
4. Process base relocations (fix addresses for new base)
5. Resolve imports via `LoadLibraryA`/`GetProcAddress`
6. Set proper memory protections per section (RX, RW, R)
7. Call `DllMain` with `DLL_PROCESS_ATTACH`

**Key feature:** No disk artifacts - the DLL never exists as a file.

**API:**
```c
bool jocky_reflective_inject(void* hProcess, const uint8_t* dll_data, size_t dll_size);
bool jocky_reflective_load_self(const uint8_t* dll_data, size_t dll_size, void** outModule);
```

### 4. Linux Process Injection `src/runtime/execution/linux_inject.c`

**Purpose:** Inject and execute code in running Linux processes.

**How it works:**
1. Attach to target process via `ptrace(PTRACE_ATTACH)`
2. Read current registers and save original state
3. Save original code at injection point
4. Write x86_64 trampoline shellcode that:
   - Saves all registers (push)
   - Calls function pointer (movabs + call)
   - Restores all registers (pop)
   - Returns (ret)
5. Single-step execute the trampoline
6. Restore original code and registers
7. Detach from process

**Additional features:**
- `jocky_linux_alloc_rwx()` - Allocate RWX memory via mmap
- `jocky_linux_memexec()` - Execute code directly from memory buffer

**API:**
```c
bool jocky_linux_inject_code(int target_pid, void* function_ptr, void** remote_addr);
void* jocky_linux_alloc_rwx(size_t size);
bool jocky_linux_memexec(const uint8_t* code, size_t code_size, void** entry_point);
```

## Integration with JOCKY Compiler

All modules are automatically compiled and linked when building JOCKY binaries:

```
compiler/src/pipeline.cpp:
  if (isWindowsTarget && !noRuntime) {
      sources.push_back(runtimeDir / "evasion" / "unhook.c");
      sources.push_back(runtimeDir / "evasion" / "syscalls.c");
      sources.push_back(runtimeDir / "execution" / "hollow.c");
      sources.push_back(runtimeDir / "execution" / "reflective.c");
  } else if (!isWindowsTarget && !noRuntime) {
      sources.push_back(runtimeDir / "execution" / "linux_inject.c");
  }
```

## JOCKY Script API

Runtime functions exposed via FFI declarations:

```jocky
// Windowsfi jocky_unhook_ntdll() -> i32;
ffi jocky_process_hollow(target_path: string, payload: ptr, size: i64) -> i32;
ffi jocky_reflective_load_self(dll_data: ptr, size: i64, outModule: ptr) -> i32;

// Linux
ffi jocky_linux_inject_code(target_pid: i32, function_ptr: ptr, remote_addr: ptr) -> i32;
ffi jocky_linux_alloc_rwx(size: i64) -> ptr;
ffi jocky_linux_memexec(code: ptr, code_size: i64, entry_point: ptr) -> i32;
```

## Usage Example

```jocky
// Windows: Unhook EDR then run forensic collection
fn main() -> i32 {
    if jocky_unhook_ntdll() {
        printf("EDR hooks removed\n");
    }
    
    // Now safe to run forensic operations
    jocky_collect_system_info();
    jocky_enum_processes();
    
    return 0;
}
```

```jocky
// Linux: Inject forensic code into trusted process
fn main() -> i32 {
    let target_pid: i32 = 1234; // e.g., sshd, cron
    let forensic_func: ptr = get_forensic_function();
    
    if jocky_linux_inject_code(target_pid, forensic_func, null) {
        printf("Forensic code injected into target process\n");
    }
    
    return 0;
}
```

## Testing

All modules compile successfully with the JOCKY pipeline:

```bash
./jockyc forensics_agent.jky -o agent -p paranoid
# Compiles runtime library with all evasion modules
# Links into final obfuscated binary
```

Binary includes all runtime functions and executes correctly.

## Architecture

```
JOCKY Source (.jky)
       |
       v
JOCKY Compiler (LLVM IR + Obfuscation)
       |
       v
+----------------------------------------+
|  Runtime Library                       |
|  - init/anti_analysis.c               |
|  - evasion/unhook.c    (Windows)      |
|  - evasion/syscalls.c  (Windows)      |
|  - execution/hollow.c  (Windows)      |
|  - execution/reflective.c (Windows)   |
|  - execution/linux_inject.c (Linux)   |
|  - forensics/forensics_rt.c           |
+----------------------------------------+
       |
       v
Obfuscated Binary with In-Memory Execution
```

## Next Steps (Option C - BYOVD)

1. **Vulnerable driver detection** - Scan for known BYOVD drivers (RTCore64, gdrv, dbutil)
2. **Driver loader** - Load signed vulnerable driver
3. **Kernel callback disabling** - Use driver to remove EDR kernel callbacks
4. **ETW patching** - Disable Event Tracing for Windows

## Files Modified/Created

| File | Status | Purpose |
|------|--------|---------|
| `src/runtime/evasion/unhook.c` | Enhanced | API unhooking |
| `src/runtime/execution/hollow.c` | Existing | Process hollowing |
| `src/runtime/execution/reflective.c` | New | Reflective DLL injection |
| `src/runtime/execution/linux_inject.c` | New | Linux ptrace injection |
| `src/runtime/include/jocky_rt.h` | Updated | API declarations |
| `compiler/src/pipeline.cpp` | Updated | Auto-compile new modules |
