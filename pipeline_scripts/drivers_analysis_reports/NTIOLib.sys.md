# DeepZero Analysis: NTIOLib.sys — Full Breakdown

---

## 🐛 Pipeline Status

The report shows **“Needs assessment”** with 2 HIGH findings. There is no execution error. DeepZero completed all stages except the final AI assessment (`assess`), which was either skipped or not required because the static findings are already conclusive.

| Stage | Status |
|:---|:---|
| `discover` | ✅ Completed |
| `kernel_filter` | ✅ Completed |
| `decompile` | ✅ Completed |
| `semgrep_scanner` | ✅ Completed |
| `rank_by_findings` | ✅ Completed |
| `assess` | ⚠️ Needs assessment (not run) |

The 2 HIGH findings are sufficient to evaluate the driver.

---

## 🎯 Driver Identity

| Field | Value |
|:---|:---|
| **File name** | `NTIOLib.sys` |
| **SHA-256** | `d8b58f6a89a7618558e37afc360cd772b6731e3ba367f8d58734ecee2244a530` |
| **MD5** | `c02f70960fa934b8defa16a03d7f6556` |
| **Size** | 11,888 bytes (~12 KB) — **one of the smallest in the corpus** |
| **Machine Type** | `AMD64` — 64-bit x86 |
| **Subsystem** | `NATIVE` — kernel-mode driver |
| **Signed** | `True` — valid signature |
| **Kernel Driver** | `True` — correct target |
| **Priority Score** | **6.0** — high, matching top-tier drivers |

This is the **NTIOLib** driver — device name `\Device\NTIOLib_1_0_6` and symbolic link `\DosDevices\NTIOLib_1_0_6`. The name suggests a low-level I/O library for Windows NT. It is a LOLDrivers entry and therefore very likely on Microsoft’s Vulnerable Driver Blocklist.

---

## 🔓 Attack Surface

### Device Names
```
Device Name:     \Device\NTIOLib_1_0_6
Symbolic Link:   \DosDevices\NTIOLib_1_0_6
```

Friendly, predictable name. From user mode:
```cpp
CreateFile("\\\\.\\NTIOLib_1_0_6", ...)
```

### Key Flag: `device_on_load_path = True`

The device is created **when the driver loads**. No initialisation sequence required — you can load the driver and immediately open the device.

### Attack Surface Numbers
```
Functions:     21
IOCTLs:        15
```

A tiny driver with only 21 functions, but **15 IOCTLs** — an unusually dense attack surface for its size. The driver is focused almost entirely on IOCTL dispatch.

### IOCTL Codes
```
0xC3502000, 0xC3502084, 0xC3502088, 0xC350208C, 0xC3502090,
0xC35060CC, 0xC35060D0, 0xC35060D4, 0xC3506104, 0xC3506144,
0xC350A0C8, 0xC350A0D8, 0xC350A0DC, 0xC350A0E0, 0xC350A108
```

Standard `METHOD_BUFFERED` IOCTLs with a custom device type. The three clusters (`0x2084`, `0x60CC`, `0xA0C8`) suggest three groups of related hardware operations.

---

## ⚠️ The 2 HIGH Severity Findings

### Both are `MmMapIoSpace` with User-Controlled Parameters

**Locations:** Lines 235, 314

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
| **`IoCreateDevice`, `IoCreateSymbolicLink`** | Device creation | 🟡 Standard driver setup |
| **`IoDeleteDevice`, `IoDeleteSymbolicLink`** | Cleanup | 🟢 Standard |
| **`KeBugCheckEx`** | Triggers BSOD | 🟢 Error handling |
| **`HAL.dll`** | Hardware Abstraction Layer | 🔴 Direct hardware access |

**The critical combination:** `MmMapIoSpace` + `HalGetBusDataByOffset`/`HalSetBusDataByOffset` + `HAL.dll`. This is the same powerful hardware access toolkit seen in top-tier drivers like AMDRyzenMasterDriver.sys, CorsairLLAccess64.sys, and LenovoDiagnosticsDriver.sys.

**Security consideration:** There are **no security descriptor APIs** imported (no `SeCaptureSecurityDescriptor`, `RtlSetDaclSecurityDescriptor`, `ZwSetSecurityObject`). This suggests the driver does **not** set an explicit DACL on its device object — it relies on the default security descriptor. Default device object security typically grants full access to SYSTEM and administrators, and may grant read access to everyone. Whether the IOCTLs require write access (and thus admin) depends on the specific access flags in the IOCTL codes. The `0xC350xxxx` codes use `METHOD_BUFFERED` and likely `FILE_ANY_ACCESS`, which means they may be accessible to any user who can open the device.

**No privilege-checking APIs** like `SePrivilegeCheck` are imported. So if the device can be opened, the IOCTLs are likely accessible.

---

## 📊 Standalone Summary

| Aspect | Assessment |
|:---|:---|
| **Validity** | ✅ Signed, kernel-mode, AMD64 driver |
| **Attack surface** | 21 functions, 15 IOCTLs |
| **Primary primitive** | 2× `MmMapIoSpace` user-controlled (arbitrary physical memory mapping) |
| **Secondary primitive** | PCI config space read/write (`HalGetBusDataByOffset`/`HalSetBusDataByOffset`) |
| **Device creation** | ✅ On load (no init sequence required) |
| **Device security** | No explicit DACL APIs — relies on default; needs investigation |
| **Blocklist status** | **Very likely present** (LOLDrivers entry) |
| **Priority score** | 6.0 |
| **Overall verdict** | ★★★★★ — Exceptionally strong if unblocked; tiny, focused, device-on-load |

**This is a top-tier driver.** Despite being only 12 KB, it provides two `MmMapIoSpace` primitives, PCI config access, and device-on-load. Its priority score of 6.0 matches drivers like cpuz141.sys and rtkiow8x64.sys. The two main hurdles are:
1. **Blocklist status** — almost certainly blocked.
2. **Default DACL** — may restrict access to administrators; needs investigation.

If both hurdles are overcome, it would be a **primary candidate** for JOCKY.

---

## 🛠️ Next Steps for NTIOLib.sys

### 1. Verify Against the Microsoft Blocklist
Search `Microsoft_Blocked_Driver_List.pdf` for:
```
d8b58f6a89a7618558e37afc360cd772b6731e3ba367f8d58734ecee2244a530
```
As a LOLDrivers entry, it is **very likely blocked**. If so, discard it.

### 2. Investigate the Device DACL
Open the decompiled source:
```
/home/incharanew/JOCKY/DeepZero/work/loldrivers/drivers_out-6603aa5d/.bulk_temp/semgrep_0/d8b58f6a89a76185_dispatch_ioctl.c
```

Look for the `IoCreateDevice` call. Since there are no security descriptor APIs imported, the default DACL will be applied. You can test access by trying to open `\\\\.\\NTIOLib_1_0_6` from a non-admin process in your isolated VM. If it opens, the device is accessible.

### 3. Identify the Best `MmMapIoSpace` Handler
Start with **line 235** (the first `MmMapIoSpace` finding). Analyse:
- Which IOCTL code reaches this handler
- The input buffer layout (offset of physical address, offset of size)
- Any validation logic

### 4. Add to Your JOCKY Manifest (Conditional)
If unblocked and accessible:

```yaml
name: "NTIOLib.sys"
sha256: "d8b58f6a89a7618558e37afc360cd772b6731e3ba367f8d58734ecee2244a530"
device_path: "\\\\.\\NTIOLib_1_0_6"
device_on_load: true
requires_admin: unknown  # Default DACL to investigate
capabilities: [arb_physical_read, arb_physical_write, pci_config_rw]
priority: 2  # Strong if unblocked and accessible
```

### 5. Test in Isolated Windows VM
- Disable HVCI and blocklist
- Enable test signing
- Snapshot before loading
- Load driver, open device, send the simplest `MmMapIoSpace` IOCTL
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
| **LenovoDiagnosticsDriver** | 8.0 | 40 KB | 7 | 2× MmMapIoSpace, PCI config | ❌ | Very High | ★★★★★ |
| **New (`NTIOLib`)** | **6.0** | **12 KB** | **15** | **2× MmMapIoSpace, PCI config** | ✅ | **Very High** | **★★★★★** |
| **E (`cpuz141`)** | 6.0 | 46 KB | 38 | 7× MmMapIoSpace, PCI config | ✅ | Very High | ★★★★★ |
| **P (`rtkiow8x64`)** | 6.0 | 47 KB | 16 | 3× MmMapIoSpace, MDL | ✅ | Very High | ★★★★★ |

**Interpretation:**
- **NTIOLib.sys** is a **top-tier candidate** with priority 6.0, two `MmMapIoSpace` primitives, PCI config access, device-on-load, and no explicit DACL APIs (meaning default security may be permissive). Its tiny size (12 KB) makes it easy to analyse.
- **Driver A** remains the strongest overall choice due to its **low blocklist risk**.
- **Driver B** is a strong fallback with low blocklist risk.
- If all were unblocked, NTIOLib would rank among the top 6–8 candidates.

**Updated practical ranking (if all unblocked and accessible):**
1. Driver T (`CorsairLLAccess64`) — 8.0
2. Driver I (`AMDRyzenMaster`) — 8.0
3. Driver LenovoDiagnosticsDriver — 8.0
4. Driver U (`ene.sys`) — 7.0
5. Driver M (`kEvP64`) — 7.0
6. **Driver NTIOLib — 6.0**
7. Driver E (`cpuz141`) — 6.0
8. Driver P (`rtkiow8x64`) — 6.0
9. Driver Q (`AMDPowerProfiler`) — 6.0
10. Driver L (`b205835b`) — 6.0
11. Driver J (`0eab16c7`) — 6.0
12. Driver A (`65329dad`) — 3.0 (but low blocklist risk)
13. Driver B (`LHA.sys`) — 3.0 (but low blocklist risk)

---

## 📝 For Your Research Paper

`NTIOLib.sys` provides a case study of a minimal, focused hardware-access driver:

> *“A twenty-fifth driver, `NTIOLib.sys` (SHA-256: `d8b58f6a...`), was analyzed. It is a signed AMD64 kernel driver of only 12 KB, identifiable as a low-level I/O library (`\Device\NTIOLib_1_0_6`). It contains just 21 functions but exposes 15 IOCTLs, and includes two user-controlled `MmMapIoSpace` handlers, providing arbitrary physical memory mapping. It additionally imports `HalGetBusDataByOffset`/`HalSetBusDataByOffset` for PCI configuration space access and `HAL.dll` for direct hardware interaction. It creates its device on load and imports no security descriptor APIs, suggesting it relies on the default device DACL. It received a DeepZero priority score of 6.0, placing it among the top-tier candidates in the corpus. Its presence in the LOLDrivers database makes it highly likely to be blocked by Microsoft's Vulnerable Driver Blocklist. This driver illustrates that even extremely small drivers can expose powerful physical memory and PCI configuration primitives, and that the absence of explicit DACL APIs may indicate a more permissive default security posture. A comprehensive BYOVD arsenal should include such minimal, focused drivers as potential high-reliability candidates.”*

**Key insight:** Size is not a reliable indicator of risk. A 12 KB driver with `MmMapIoSpace` and PCI config access can be as dangerous as a 200 KB driver, and may be easier to analyse and exploit.

---

## ✅ Bottom Line

**NTIOLib.sys is a top-tier driver** with a priority score of 6.0, two `MmMapIoSpace` primitives, PCI config access, and device-on-load. It is almost certainly blocked (LOLDrivers entry) and its default DACL needs investigation. If unblocked and accessible, it would be a **strong primary or secondary candidate** for JOCKY.

**Your practical arsenal remains: Driver A (primary, low blocklist risk), Driver B (fallback, low blocklist risk), followed by the top-tier unblocked candidates (CorsairLLAccess64, AMDRyzenMaster, LenovoDiagnosticsDriver, ene.sys, kEvP64, NTIOLib, etc.).** Continue running DeepZero across your corpus — you are building a comprehensive, resilient BYOVD arsenal.