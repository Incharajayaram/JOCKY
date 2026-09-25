# DeepZero Analysis: LenovoDiagnosticsDriver.sys — Full Breakdown

---

## 🐛 The Error / Pipeline Status

The report shows **“Needs assessment”** with 2 HIGH findings. This is not an execution error — it means DeepZero’s final AI assessment stage was either skipped or not required because the static findings were already conclusive.

**Stage status:**

| Stage | Status |
|:---|:---|
| `discover` | ✅ Completed |
| `kernel_filter` | ✅ Completed |
| `decompile` | ✅ Completed |
| `semgrep_scanner` | ✅ Completed |
| `rank_by_findings` | ✅ Completed |
| `assess` | ⚠️ Needs assessment (not run or not required) |

The 2 HIGH findings are already enough to evaluate the driver.

---

## 🎯 Driver Identity

| Field | Value |
|:---|:---|
| **File name** | `LenovoDiagnosticsDriver.sys` |
| **SHA-256** | `f05b1ee9e2f6ab704b8919d5071becbce6f9d0f9d0ba32a460c41d5272134abe` |
| **MD5** | `b941c8364308990ee4cc6eadf7214e0f` |
| **Size** | 40,472 bytes (~40 KB) |
| **Machine Type** | `AMD64` — 64-bit x86 |
| **Subsystem** | `NATIVE` — kernel-mode driver |
| **Signed** | `True` — valid signature |
| **Kernel Driver** | `True` — correct target |
| **Priority Score** | **8.0** — highest possible, matching top-tier drivers |

This is the **Lenovo Diagnostics** kernel driver — device name `\Device\LenovoDiagnosticsDriver` and symbolic link `\DosDevices\LenovoDiagnosticsDriver`. It is a legitimate hardware diagnostics component from Lenovo. It is also a known LOLDrivers entry, so it is **very likely on Microsoft’s Vulnerable Driver Blocklist**.

---

## 🔓 Attack Surface

### Device Names
```
Device Name:     \Device\LenovoDiagnosticsDriver
Symbolic Link:   \DosDevices\LenovoDiagnosticsDriver
```

Friendly, predictable name. From user mode:
```cpp
CreateFile("\\\\.\\LenovoDiagnosticsDriver", ...)
```

### Key Flag: `device_on_load_path = False`

The device is **not created when the driver first loads**. It requires an initialisation sequence — likely triggered by the Lenovo Diagnostics user-mode application. You will need to either replicate that initialisation or find an IOCTL that creates the device on demand.

### Attack Surface Numbers
```
Functions:     104
IOCTLs:        7
```

A compact driver with 104 functions and only **7 IOCTLs** — a small but focused attack surface.

### IOCTL Codes
```
0x00222000, 0x00222008, 0x0022200C, 0x00222010, 0x00222014,
0x00222018, 0x0022201C
```

Standard `METHOD_BUFFERED` IOCTLs with a custom device type. Sequential codes suggest a group of related hardware diagnostic operations.

---

## ⚠️ The 2 HIGH Severity Findings

### Both are `MmMapIoSpace` with User-Controlled Parameters

**Locations:** Lines 89, 198

**Finding text (repeated twice):**
```
MmMapIoSpace called with parameters derived from user input.
This can allow a user-mode attacker to map arbitrary physical memory
into kernel virtual space, bypassing all protections.
```

**What this means:**
The driver exposes at least one IOCTL handler that passes user-supplied physical address and size to `MmMapIoSpace`. With two separate call sites, there is redundancy. This provides **arbitrary physical memory mapping** — the strongest kernel primitive. Once you can map arbitrary physical memory, you can read/write kernel structures, steal tokens, remove EDR callbacks, and strip PPL protections.

**No `RtlCopyMemory` or `ZwMapViewOfSection` findings.** This driver’s power is entirely in the `MmMapIoSpace` primitive.

---

## 🔍 Dangerous Imports

| Import | Purpose | Risk |
|:---|:---|:---|
| **`MmMapIoSpace`** | Maps physical memory into kernel VA | 🔴 Arbitrary physical memory access |
| **`MmUnmapIoSpace`** | Cleans up mappings | 🟡 Used with the above |
| **`HalGetBusDataByOffset`** | Read PCI bus data | 🔴 PCI config space access |
| **`HalSetBusDataByOffset`** | Write PCI bus data | 🔴 PCI config space modification |
| **`ZwSetSecurityObject`** | Modify security descriptors | 🟡 DACL manipulation |
| **`RtlGetDaclSecurityDescriptor`, `RtlGetGroupSecurityDescriptor`, `RtlGetSaclSecurityDescriptor`, `RtlGetOwnerSecurityDescriptor`** | Query security descriptors | 🟡 DACL inspection |
| **`SeCaptureSecurityDescriptor`** | Capture security descriptors | 🟡 DACL handling |
| **`RtlCreateSecurityDescriptor`, `RtlSetDaclSecurityDescriptor`, `RtlAddAccessAllowedAce`** | Create and set DACL | 🟡 Device permission control |
| **`ZwOpenKey`, `ZwSetValueKey`, `ZwQueryValueKey`, `ZwCreateKey`** | Registry operations | 🟡 Persistence / config |
| **`HAL.dll`** | Hardware Abstraction Layer | 🔴 Direct hardware access |
| **`KeBugCheckEx`** | Triggers BSOD | 🟢 Error handling |

**The critical combination:** `MmMapIoSpace` + `HalGetBusDataByOffset`/`HalSetBusDataByOffset` + `HAL.dll`. This gives the driver both arbitrary physical memory mapping and PCI configuration space access — a very powerful hardware access toolkit.

**Security consideration:** The driver imports an extensive set of security descriptor APIs (`SeCaptureSecurityDescriptor`, `RtlSetDaclSecurityDescriptor`, `RtlCreateSecurityDescriptor`, `ZwSetSecurityObject`, etc.). This strongly suggests it **sets a DACL on its device object**. The DACL may restrict access to administrators or SYSTEM only, which would make exploitation require prior privilege escalation. If the DACL is misconfigured and permissive, the driver is world-accessible.

**No privilege-checking APIs** like `SePrivilegeCheck` are imported. The security descriptor APIs are about setting the device DACL, not checking caller privileges at runtime. So if the DACL is permissive, any user who can open the device can exploit it.

---

## 📊 Standalone Summary

| Aspect | Assessment |
|:---|:---|
| **Validity** | ✅ Signed, kernel-mode, AMD64 driver |
| **Attack surface** | 104 functions, 7 IOCTLs |
| **Primary primitive** | 2× `MmMapIoSpace` user-controlled (arbitrary physical memory mapping) |
| **Secondary primitive** | PCI config space read/write (`HalGetBusDataByOffset`/`HalSetBusDataByOffset`) |
| **Device creation** | ❌ Deferred (not on load) — requires init sequence |
| **Device security** | Sets DACL via security descriptor APIs — needs investigation |
| **Blocklist status** | **Very likely present** (LOLDrivers entry) |
| **Priority score** | 8.0 |
| **Overall verdict** | ★★★★★ — Exceptionally strong if unblocked and DACL permissive |

**This is a top-tier driver.** The `MmMapIoSpace` primitive, PCI config access, and priority score of 8.0 place it alongside AMDRyzenMasterDriver.sys and CorsairLLAccess64.sys as one of the strongest candidates in the corpus. The two main hurdles are:
1. **Blocklist status** — almost certainly blocked.
2. **Deferred device creation and possible restrictive DACL** — requires investigation.

If both hurdles are overcome, it would be a **primary candidate** for JOCKY.

---

## 🛠️ Next Steps for LenovoDiagnosticsDriver.sys

### 1. Verify Against the Microsoft Blocklist
Search `Microsoft_Blocked_Driver_List.pdf` for:
```
f05b1ee9e2f6ab704b8919d5071becbce6f9d0f9d0ba32a460c41d5272134abe
```
As a LOLDrivers entry, it is **very likely blocked**. If so, discard it.

### 2. Investigate the Device DACL
Open the decompiled source:
```
/home/incharanew/JOCKY/DeepZero/work/loldrivers/drivers_out-6603aa5d/.bulk_temp/semgrep_0/f05b1ee9e2f6ab70_dispatch_ioctl.c
```

Look for the `IoCreateDevice` call and the security descriptor setup. Determine:
- What SID is granted access via `RtlAddAccessAllowedAce`?
- Is the device world-accessible, admin-only, or SYSTEM-only?

### 3. Find What Triggers Device Creation
Since `device_on_load_path = False`, find the IOCTL or code path that creates `\Device\LenovoDiagnosticsDriver`. You may need to send a specific init IOCTL first.

### 4. Identify the Best `MmMapIoSpace` Handler
Start with **line 89** (the first `MmMapIoSpace` finding). Analyse:
- Which IOCTL code reaches this handler
- The input buffer layout (offset of physical address, offset of size)
- Any validation logic

### 5. Add to Your JOCKY Manifest (Conditional)
If unblocked and DACL permissive:

```yaml
name: "LenovoDiagnosticsDriver.sys"
sha256: "f05b1ee9e2f6ab704b8919d5071becbce6f9d0f9d0ba32a460c41d5272134abe"
device_path: "\\\\.\\LenovoDiagnosticsDriver"
device_on_load: false  # Requires init sequence
requires_admin: unknown  # DACL to investigate
capabilities: [arb_physical_read, arb_physical_write, pci_config_rw]
priority: 1  # If unblocked and accessible
```

### 6. Test in Isolated Windows VM
- Disable HVCI and blocklist
- Enable test signing
- Snapshot before loading
- Load driver, trigger device creation, open handle, send the simplest `MmMapIoSpace` IOCTL
- Verify physical memory mapping works
- Revert snapshot after

---

## 🧩 Comparison: Top Drivers + New Driver

| Driver | Priority | Size | IOCTLs | Key Primitives | Device on Load | Blocklist Risk | Overall |
|:---|:---|:---|:---|:---|:---|:---|:---|
| **A (`65329dad`)** | 3.0 | 65 KB | 57 | 21× MmMapIoSpace, 17× RtlCopyMemory | ✅ | Low | ★★★★★ |
| **B (`LHA.sys`)** | 3.0 | 35 KB | 28 | 13× MmMapIoSpace | ❌ | Low | ★★★★☆ |
| **T (`CorsairLLAccess64`)** | 8.0 | 20 KB | 11 | 2× MmMapIoSpace, PCI config, MDL | ✅ | Very High | ★★★★★ |
| **I (`AMDRyzenMaster`)** | 8.0 | 69 KB | 31 | 4× MmMapIoSpace, PCI config, MDL | ❌ | Very High | ★★★★★ |
| **U (`ene.sys`)** | 7.0 | 21 KB | 3 | 2× ZwMapViewOfSection (\Device\PhysicalMemory) | ✅ | Very High | ★★★★★ |
| **M (`kEvP64`)** | 7.0 | 174 KB | 2 | 2× process attach, 1× MmMapIoSpace, process kill | ✅ | Very High | ★★★★★ |
| **New (`LenovoDiagnosticsDriver`)** | **8.0** | **40 KB** | **7** | **2× MmMapIoSpace, PCI config** | ❌ | **Very High** | **★★★★★** |
| **E (`cpuz141`)** | 6.0 | 46 KB | 38 | 7× MmMapIoSpace, PCI config | ✅ | Very High | ★★★★★ |
| **P (`rtkiow8x64`)** | 6.0 | 47 KB | 16 | 3× MmMapIoSpace, MDL | ✅ | Very High | ★★★★★ |
| **Q (`AMDPowerProfiler`)** | 6.0 | 81 KB | 15 | 1× MmMapIoSpace, 1× ZwMapViewOfSection | ❌ | Very High | ★★★★★ |
| **L (`b205835b`)** | 6.0 | 55 KB | 18 | 3× MmMapIoSpace, MDL | ❌ | Very High | ★★★★★ |
| **J (`0eab16c7`)** | 6.0 | 22 KB | 34 | 2× MmMapIoSpace, 1× RtlCopyMemory, PCI config | ✅ | Very High | ★★★★★ |
| **V (`fb0dbc3b`)** | 4.0 | 39 KB | 12 | 2× MmMapIoSpace, MDL, firmware | ✅ | Very High | ★★★★☆ |

**Interpretation:**
- The **LenovoDiagnosticsDriver** is a **top-tier candidate** with priority 8.0, `MmMapIoSpace`, PCI config access, and device-on-load. It is very likely blocked and has deferred device creation + possible restrictive DACL.
- **Driver A** remains the strongest overall choice due to its **low blocklist risk**.
- **Driver B** is a strong fallback with low blocklist risk.
- If all were unblocked, the Lenovo driver would rank among the top 3–4 candidates.

**Updated practical ranking (if all unblocked and accessible):**
1. Driver T (`CorsairLLAccess64`) — 8.0
2. Driver I (`AMDRyzenMaster`) — 8.0
3. **Driver New (`LenovoDiagnosticsDriver`) — 8.0**
4. Driver U (`ene.sys`) — 7.0
5. Driver M (`kEvP64`) — 7.0
6. Driver E (`cpuz141`) — 6.0
7. Driver P (`rtkiow8x64`) — 6.0
8. Driver Q (`AMDPowerProfiler`) — 6.0
9. Driver L (`b205835b`) — 6.0
10. Driver J (`0eab16c7`) — 6.0
11. Driver A (`65329dad`) — 3.0 (but low blocklist risk)
12. Driver B (`LHA.sys`) — 3.0 (but low blocklist risk)

---

## 📝 For Your Research Paper

`LenovoDiagnosticsDriver.sys` provides a case study of a top-tier hardware diagnostics driver:

> *“A twenty-fourth driver, `LenovoDiagnosticsDriver.sys` (SHA-256: `f05b1ee9...`), was analyzed. It is a signed AMD64 kernel driver of 40 KB, identifiable as the Lenovo Diagnostics component (`\Device\LenovoDiagnosticsDriver`). It exposes 7 IOCTLs across 104 functions and contains two user-controlled `MmMapIoSpace` handlers, providing arbitrary physical memory mapping. It additionally imports `HalGetBusDataByOffset`/`HalSetBusDataByOffset` for PCI configuration space access and `HAL.dll` for direct hardware interaction. Unlike many hardware-access drivers, it imports an extensive set of security descriptor APIs (`SeCaptureSecurityDescriptor`, `RtlSetDaclSecurityDescriptor`, `RtlCreateSecurityDescriptor`, `ZwSetSecurityObject`), indicating that it sets a DACL on its device object — a security-conscious design that may restrict exploitability to administrators. It creates its device lazily rather than on load. It received a DeepZero priority score of 8.0, the highest in the corpus, indicating exceptional exploitability potential. Its presence in the LOLDrivers database makes it highly likely to be blocked by Microsoft's Vulnerable Driver Blocklist. This driver reinforces the pattern that top-tier BYOVD candidates are often hardware diagnostics utilities that expose physical memory mapping and PCI configuration access, but they also frequently implement DACLs that may limit practical exploitation to privileged users.”*

**Key insight:** A priority score of 8.0 and powerful primitives do not guarantee exploitability if the device DACL is restrictive. Always investigate the security descriptor before assuming a driver is usable from an unprivileged context.

---

## ✅ Bottom Line

**LenovoDiagnosticsDriver.sys is a top-tier driver** with a priority score of 8.0, two `MmMapIoSpace` primitives, PCI config access, and device-on-load. It is almost certainly blocked (LOLDrivers entry) and has deferred device creation plus a possible restrictive DACL. If unblocked and world-accessible, it would be a **primary candidate** for JOCKY.

**Your practical arsenal remains: Driver A (primary, low blocklist risk), Driver B (fallback, low blocklist risk), followed by the top-tier unblocked candidates (CorsairLLAccess64, AMDRyzenMaster, LenovoDiagnosticsDriver, ene.sys, kEvP64, etc.).** Continue running DeepZero across your corpus — you are building a comprehensive, resilient BYOVD arsenal.