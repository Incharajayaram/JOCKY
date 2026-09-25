# DeepZero Analysis: PhlashNT.sys — Full Breakdown

---

## 🐛 Pipeline Status

The report shows **“Needs assessment”** with 1 HIGH finding. There is no execution error. DeepZero completed all stages except the final AI assessment (`assess`), which was either skipped or not required because the static finding is already conclusive.

| Stage | Status |
|:---|:---|
| `discover` | ✅ Completed |
| `kernel_filter` | ✅ Completed |
| `decompile` | ✅ Completed |
| `semgrep_scanner` | ✅ Completed |
| `rank_by_findings` | ✅ Completed |
| `assess` | ⚠️ Needs assessment (not run) |

The single HIGH finding is sufficient to evaluate the driver.

---

## 🎯 Driver Identity

| Field | Value |
|:---|:---|
| **File name** | `PhlashNT.sys` |
| **SHA-256** | `65db1b259e305a52042e07e111f4fa4af16542c8bacd33655f753ef642228890` |
| **MD5** | `e9e786bdba458b8b4f9e93d034f73d00` |
| **Size** | 61,496 bytes (~60 KB) |
| **Machine Type** | `AMD64` — 64-bit x86 |
| **Subsystem** | `NATIVE` — kernel-mode driver |
| **Signed** | `True` — valid signature |
| **Kernel Driver** | `True` — correct target |
| **Priority Score** | 3.0 |

This is the **WinPhlash** kernel driver — device name `\Device\WinPhlash` and symbolic link `\DosDevices\WinPhlash`. It is used for flashing BIOS/firmware on Windows. It is a LOLDrivers entry and therefore **very likely on Microsoft’s Vulnerable Driver Blocklist**.

---

## 🔓 Attack Surface

### Device Names
```
Device Name:     \Device\WinPhlash
Symbolic Link:   \DosDevices\WinPhlash
```

Friendly, predictable name. From user mode:
```cpp
CreateFile("\\\\.\\WinPhlash", ...)
```

### Key Flag: `device_on_load_path = True`

The device is created **when the driver loads**. No initialisation sequence required.

### Attack Surface Numbers
```
Functions:     130
IOCTLs:        5
```

A relatively large driver (130 functions) but only **5 IOCTLs** — a small attack surface.

### IOCTL Codes
```
0x80002000, 0x80002006, 0x8000200A, 0x8000200E, 0x80002016
```

Standard `METHOD_BUFFERED` IOCTLs with a custom device type. The small number of IOCTLs makes reverse engineering straightforward.

---

## ⚠️ The 1 HIGH Severity Finding

### `MmMapIoSpace` with User-Controlled Parameters (Line 2670)

**Finding text:**
```
MmMapIoSpace called with parameters derived from user input.
This can allow a user-mode attacker to map arbitrary physical memory
into kernel virtual space, bypassing all protections.
```

**What this means:**
The driver exposes one IOCTL handler that passes a user-supplied physical address and size to `MmMapIoSpace`. This provides **arbitrary physical memory mapping** — a strong kernel primitive. However, there is only **one call site**, so if that IOCTL is gated or validated, there is no fallback.

**No `RtlCopyMemory` or `ZwMapViewOfSection` findings.** This driver’s power is entirely in this single `MmMapIoSpace` primitive.

---

## 🔍 Dangerous Imports

| Import | Purpose | Risk |
|:---|:---|:---|
| **`MmMapIoSpace`** | Maps physical memory into kernel VA | 🔴 Arbitrary physical memory access |
| **`MmUnmapIoSpace`** | Cleans up mappings | 🟡 Used with the above |
| **`MmMapLockedPages`** | Map locked pages | 🟡 MDL-based memory access |
| **`ExAllocatePoolWithTag`, `ExFreePoolWithTag`** | Pool allocation | 🟢 Standard |
| **`RtlAssert`, `DbgPrint`** | Debug | 🟢 Debug output |
| **`IoCreateDevice`, `IoCreateSymbolicLink`** | Device creation | 🟡 Standard driver setup |
| **`IoDeleteDevice`, `IoDeleteSymbolicLink`** | Cleanup | 🟢 Standard |
| **`IofCompleteRequest`** | I/O completion | 🟢 Standard |

**Notable:** This driver **does not import `HAL.dll`** and does **not import `HalGetBusDataByOffset`/`HalSetBusDataByOffset`**. It has no PCI configuration space access. Its only powerful primitive is `MmMapIoSpace`. It also does not import any security descriptor APIs, suggesting it relies on the default device DACL.

**No privilege-checking APIs** are imported (no `SePrivilegeCheck`, `SeAccessCheck`). This suggests the driver may not enforce admin-only access — favourable for exploitation if the device DACL is permissive.

---

## 📊 Standalone Summary

| Aspect | Assessment |
|:---|:---|
| **Validity** | ✅ Signed, kernel-mode, AMD64 driver |
| **Attack surface** | 130 functions, 5 IOCTLs |
| **Primary primitive** | 1× `MmMapIoSpace` user-controlled (arbitrary physical memory mapping) |
| **Secondary primitive** | MDL mapping (`MmMapLockedPages`) |
| **Device creation** | ✅ On load (no init sequence required) |
| **Device access** | ✅ Symbolic link known: `\\.\WinPhlash` |
| **Device security** | No privilege-check APIs visible; default DACL |
| **Blocklist status** | **Very likely present** (LOLDrivers entry) |
| **Priority score** | 3.0 |
| **Overall verdict** | ★★★☆☆ — Solid mid-tier candidate if unblocked; single primitive, no PCI config |

**This is a mid-tier driver.** It provides one `MmMapIoSpace` primitive, device-on-load, and a known symbolic link. It lacks redundancy and PCI configuration space access, making it less versatile than top-tier drivers. If unblocked and accessible, it would be a **good backup** addition to your arsenal.

---

## 🛠️ Next Steps for PhlashNT.sys

### 1. Verify Against the Microsoft Blocklist
Search `Microsoft_Blocked_Driver_List.pdf` for:
```
65db1b259e305a52042e07e111f4fa4af16542c8bacd33655f753ef642228890
```
As a LOLDrivers entry, it is **very likely blocked**. If so, discard it.

### 2. Investigate the Device DACL
Open the decompiled source:
```
/home/incharanew/JOCKY/DeepZero/work/loldrivers/drivers_out-6603aa5d/.bulk_temp/semgrep_0/65db1b259e305a52_dispatch_ioctl.c
```

Look for the `IoCreateDevice` call. Since there are no security descriptor APIs imported, the default DACL will be applied. You can test access by trying to open `\\\\.\\WinPhlash` from a non-admin process in your isolated VM. If it opens, the device is accessible.

### 3. Identify the IOCTL Handler
Start with **line 2670** (the `MmMapIoSpace` finding). Analyse:
- Which IOCTL code reaches this handler (likely one of the 5 IOCTLs)
- The input buffer layout (offset of physical address, offset of size)
- Any validation logic

### 4. Add to Your JOCKY Manifest (Conditional)
If unblocked and accessible:

```yaml
name: "PhlashNT.sys"
sha256: "65db1b259e305a52042e07e111f4fa4af16542c8bacd33655f753ef642228890"
device_path: "\\\\.\\WinPhlash"
device_on_load: true
requires_admin: unknown  # Default DACL to investigate
capabilities: [arb_physical_read, arb_physical_write]
priority: 6  # Mid-tier backup if unblocked
```

### 5. Test in Isolated Windows VM
- Disable HVCI and blocklist
- Enable test signing
- Snapshot before loading
- Load driver, open `\\\\.\\WinPhlash`, send the IOCTL to map a view of physical memory
- Verify read/write to a known physical address
- Revert snapshot after

---

## 🧩 Comparison: All Drivers Analyzed So Far

| Driver | Priority | Size | Arch | IOCTLs | Key Primitives | Device on Load | Blocklist Risk | Overall |
|:---|:---|:---|:---|:---|:---|:---|:---|:---|
| **T (`CorsairLLAccess64`)** | 8.0 | 20 KB | AMD64 | 11 | 2× MmMapIoSpace, PCI config, MDL | ✅ | Very High | ★★★★★ |
| **I (`AMDRyzenMaster`)** | 8.0 | 69 KB | AMD64 | 31 | 4× MmMapIoSpace, PCI config, MDL | ❌ | Very High | ★★★★★ |
| **LenovoDiagnosticsDriver** | 8.0 | 40 KB | AMD64 | 7 | 2× MmMapIoSpace, PCI config | ❌ | Very High | ★★★★★ |
| **U (`ene.sys`)** | 7.0 | 21 KB | AMD64 | 3 | 2× ZwMapViewOfSection (\Device\PhysicalMemory) | ✅ | Very High | ★★★★★ |
| **M (`kEvP64`)** | 7.0 | 174 KB | AMD64 | 2 | 2× process attach, 1× MmMapIoSpace, process kill | ✅ | Very High | ★★★★★ |
| **E (`cpuz141`)** | 6.0 | 46 KB | AMD64 | 38 | 7× MmMapIoSpace, PCI config | ✅ | Very High | ★★★★★ |
| **P (`rtkiow8x64`)** | 6.0 | 47 KB | AMD64 | 16 | 3× MmMapIoSpace, MDL | ✅ | Very High | ★★★★★ |
| **NTIOLib** | 6.0 | 12 KB | AMD64 | 15 | 2× MmMapIoSpace, PCI config | ✅ | Very High | ★★★★★ |
| **J (`0eab16c7`)** | 6.0 | 22 KB | AMD64 | 34 | 2× MmMapIoSpace, 1× RtlCopyMemory, PCI config | ✅ | Very High | ★★★★★ |
| **ATSZIO** | 5.0 | 20 KB | AMD64 | 16 | 1× ZwMapViewOfSection (\Device\PhysicalMemory), PCI config | ✅ | Very High | ★★★★☆ |
| **nvflsh64** | 4.0 | 15 KB | AMD64 | 16 | 1× ZwMapViewOfSection (\Device\PhysicalMemory) | ✅ | Very High | ★★★★☆ |
| **speedfan** | 4.0 | 14 KB | AMD64 | 11 | 2× MmMapIoSpace | ✅ | Very High | ★★★★☆ |
| **A (`65329dad`)** | 3.0 | 65 KB | AMD64 | 57 | 21× MmMapIoSpace, 17× RtlCopyMemory | ✅ | Low | ★★★★★ |
| **B (`LHA.sys`)** | 3.0 | 35 KB | AMD64 | 28 | 13× MmMapIoSpace | ❌ | Low | ★★★★☆ |
| **New (`PhlashNT`)** | **3.0** | **60 KB** | **AMD64** | **5** | **1× MmMapIoSpace, MDL** | ✅ | **Very High** | **★★★☆☆** |
| **O (`rtkio64`)** | 3.0 | 46 KB | AMD64 | 18 | 3× MmMapIoSpace, MDL | ✅ | Very High | ★★★★☆ |
| **K (`4ed2d2c1`)** | 3.0 | 54 KB | AMD64 | 20 | 3× MmMapIoSpace, MDL | ❌ | Very High | ★★★★☆ |

**Interpretation:**
- **PhlashNT.sys** is a **mid-tier candidate** with priority 3.0, one `MmMapIoSpace` primitive, device-on-load, and a known symbolic link. It lacks redundancy and PCI config access, so it is less versatile than top-tier drivers.
- **Driver A** remains the strongest overall choice due to its **low blocklist risk** and multiple primitives.
- **Driver B** is a strong fallback with low blocklist risk.
- If all were unblocked, PhlashNT.sys would rank around priority 12–15 in your arsenal.

**Updated practical ranking (if all unblocked and accessible, 64-bit only):**
1. Driver T (`CorsairLLAccess64`) — 8.0
2. Driver I (`AMDRyzenMaster`) — 8.0
3. Driver LenovoDiagnosticsDriver — 8.0
4. Driver U (`ene.sys`) — 7.0
5. Driver M (`kEvP64`) — 7.0
6. Driver E (`cpuz141`) — 6.0
7. Driver P (`rtkiow8x64`) — 6.0
8. Driver NTIOLib — 6.0
9. Driver J (`0eab16c7`) — 6.0
10. Driver ATSZIO — 5.0
11. Driver nvflsh64 — 4.0
12. Driver speedfan — 4.0
13. **Driver PhlashNT — 3.0**
14. Driver A (`65329dad`) — 3.0 (but low blocklist risk)
15. Driver B (`LHA.sys`) — 3.0 (but low blocklist risk)

---

## 📝 For Your Research Paper

`PhlashNT.sys` provides a case study of a firmware flashing utility driver with a single physical memory primitive:

> *“A thirtieth driver, `PhlashNT.sys` (SHA-256: `65db1b25...`), was analyzed. It is a signed AMD64 kernel driver of 60 KB, identifiable as the WinPhlash BIOS flashing component (`\Device\WinPhlash`). It exposes only 5 IOCTLs across 130 functions and contains one user-controlled `MmMapIoSpace` handler, providing arbitrary physical memory mapping. It imports `MmMapIoSpace`, `MmUnmapIoSpace`, and `MmMapLockedPages` for memory access, but notably does not import `HAL.dll` or any PCI configuration space functions. It creates its device on load and exposes a known symbolic link (`\DosDevices\WinPhlash`). It imports no privilege-checking APIs, relying on the default device DACL. It received a DeepZero priority score of 3.0, placing it in the mid-tier of the corpus. Its presence in the LOLDrivers database makes it highly likely to be blocked by Microsoft's Vulnerable Driver Blocklist. This driver illustrates that a single physical memory mapping primitive can be sufficient for a BYOVD attack, but the lack of redundancy and PCI configuration access makes it less versatile than multi-primitive drivers. A comprehensive BYOVD arsenal should include such minimal drivers as fallback options, while prioritizing those with multiple primitives and lower blocklist risk.”*

**Key insight:** A single `MmMapIoSpace` call site can be enough, but it is a single point of failure. Drivers with multiple primitives are more resilient. Also, firmware flashing utilities often expose physical memory primitives but may lack broader hardware access.

---

## ✅ Bottom Line

**PhlashNT.sys is a mid-tier driver** with a priority score of 3.0, one `MmMapIoSpace` primitive, device-on-load, and a known symbolic link. It is almost certainly blocked (LOLDrivers entry). If unblocked and accessible, it would be a **backup addition** to your arsenal.

**Your practical arsenal remains: Driver A (primary, low blocklist risk), Driver B (fallback, low blocklist risk), followed by the top-tier unblocked candidates (CorsairLLAccess64, AMDRyzenMaster, LenovoDiagnosticsDriver, ene.sys, kEvP64, NTIOLib, cpuz141, rtkiow8x64, ATSZIO, etc.) and mid-tier backups like nvflsh64, speedfan, and PhlashNT.** Continue running DeepZero across your corpus — you are building a comprehensive, resilient BYOVD arsenal.