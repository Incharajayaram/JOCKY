# DeepZero Analysis: cpuz141.sys — Full Breakdown

---

## 🐛 The Error (Cosmetic, Not a Blocker)

Same as previous runs: `assess failed` due to Gemini free tier quota exhaustion. All critical stages (`discover`, `kernel_filter`, `decompile`, `semgrep_scanner`, `rank_by_findings`) completed successfully. The 7 HIGH findings are conclusive.

---

## 🎯 Driver Identity

| Field | Value |
|:---|:---|
| **File name** | `cpuz141.sys` |
| **SHA-256** | `ded2927f9a4e64eefd09d0caba78e94f309e3a6292841ae81d5528cab109f95d` |
| **MD5** | `db72def618cbc3c5f9aa82f091b54250` |
| **Size** | 46,400 bytes (~46 KB) |
| **Machine Type** | `AMD64` — 64-bit x86 |
| **Subsystem** | `NATIVE` — kernel-mode driver |
| **Signed** | `True` — valid signature |
| **Kernel Driver** | `True` — correct target |
| **Priority Score** | **6.0** — significantly higher than all previous drivers |

This is the **CPU-Z 1.41 kernel driver**, a well-known hardware information utility. The high priority score (6.0 vs. 3.0 for previous drivers) indicates DeepZero considers this the most exploitable candidate so far.

---

## 🔓 Attack Surface

### Device Names
```
Symbolic Link:   \DosDevices\Global\CPUZ141
Device Name:     (blank in report — likely auto-generated)
```

Two important details:
- The symbolic link uses the **`Global\` namespace**, meaning it is accessible from **any user session**, including Remote Desktop sessions. This is more permissive than a local-only link.
- The device name is blank, but that's not a problem — you open the driver via its symbolic link:
  ```cpp
  CreateFile("\\\\.\\Global\\CPUZ141", ...)
  ```

### Key Flag: `device_on_load_path = True`

The device is created **when the driver loads**. No initialisation sequence required. You can load the driver and immediately open the device. This is a major advantage over Driver B, C, and D.

### Attack Surface Numbers
```
Functions:     48
IOCTLs:        38
```

Only 48 functions, but **38 IOCTLs** — an exceptionally dense attack surface for such a small driver. Nearly every function is an IOCTL handler.

### IOCTL Codes
```
0x10000000, 0x5F5F5F5C, 0x80000005,
0x9C402400, 0x9C402404, 0x9C402408, 0x9C40240C, 0x9C402410,
0x9C402414, 0x9C402418, 0x9C402420, 0x9C402424, 0x9C402428,
0x9C40242C, 0x9C402430, 0x9C402438, 0x9C40243C, 0x9C402448,
0x9C40244C, 0x9C402450, 0x9C402454, 0x9C402458, 0x9C40245C,
0x9C402460, 0x9C402464, 0x9C402468, 0x9C40246C, 0x9C4024A8,
0x9C402500, 0x9C402504, 0x9C402508, 0x9C402510, 0x9C402514,
0x9C402518, 0x9C40251C, 0x9C402520, 0x9C402524, 0x9C402528
```

**Decoding:**
- `0x9C40` = Custom device type (typical for third-party drivers)
- `0x2400`–`0x2528` range = Hardware access operations
- The odd values (`0x10000000`, `0x5F5F5F5C`, `0x80000005`) may be internal control codes or unused stubs

The large number of IOCTLs in the `0x9C40` range suggests **many hardware read/write primitives** (PCI config space, MSRs, memory-mapped I/O).

---

## ⚠️ The 7 HIGH Severity Findings

### All 7 are `MmMapIoSpace` with User-Controlled Parameters

**Locations:** Lines 304, 403, 442, 833, 1254, 1354, 1499

**Finding text (repeated 7 times):**
```
MmMapIoSpace called with parameters derived from user input.
This can allow a user-mode attacker to map arbitrary physical memory
into kernel virtual space, bypassing all protections.
```

**What this means:**
Each of these 7 locations is a separate IOCTL handler that passes user-supplied physical address and size to `MmMapIoSpace`. Your user-mode code can request mapping of **any physical address** into kernel space.

**Why 7 instances is significant:**
- **Redundancy** — if one IOCTL has hidden validation, others likely don't
- **Variety** — different handlers may map memory with different caching flags or access modes
- **Simplicity** — you can pick the easiest handler to trigger for your exploit

Unlike the previous drivers, there are **zero `RtlCopyMemory` findings** — this driver is a pure hardware access primitive, not a buffer overflow target.

---

## 🔍 Dangerous Imports

| Import | Purpose | Risk |
|:---|:---|:---|
| **`MmMapIoSpace`** | Maps physical memory into kernel VA | 🔴 Arbitrary physical memory access |
| **`MmUnmapIoSpace`** | Cleans up mappings | 🟡 Used with the above |
| **`HalGetBusDataByOffset`** | Read PCI bus data | 🔴 PCI config space access |
| **`HalSetBusDataByOffset`** | Write PCI bus data | 🔴 PCI config space modification |
| **`KeStallExecutionProcessor`** | Busy-wait | 🟢 Standard hardware driver |
| **`KeQueryPerformanceCounter`** | High-res timer | 🟢 Standard |
| **`IoGetDeviceObjectPointer`** | Get device object by name | 🟡 Can reference other drivers |
| **`IoBuildDeviceIoControlRequest`** | Build IOCTL request | 🟡 Can send IOCTLs to other drivers |
| **`IoCancelIrp`** | Cancel I/O request | 🟢 Standard |
| **`KeBugCheckEx`** | Triggers BSOD | 🟢 Error handling |

**The critical set:** `MmMapIoSpace` + `HalGetBusDataByOffset` + `HalSetBusDataByOffset` + the `HAL.dll` import. This is the **most powerful hardware access combination** seen so far.

- `MmMapIoSpace` gives arbitrary **physical memory** mapping.
- `HalGetBusDataByOffset` / `HalSetBusDataByOffset` give direct **PCI configuration space** access.
- Together, these allow an attacker to:
  - Enumerate PCI devices
  - Modify device BARs to redirect MMIO
  - Perform DMA attacks on any device
  - Potentially bypass IOMMU/VT-d (if configured weakly)

**This is the strongest import profile of any driver analysed so far.**

### Privilege Checks: None Visible

The imported functions list contains **no privilege-checking APIs** such as `SePrivilegeCheck`, `SeAccessCheck`, `SeCaptureSubjectContext`, or `SeReleaseSubjectContext`. This suggests the driver may **not enforce any access control** beyond whatever security descriptor is set on the device object.

Combined with the **global symbolic link** (`\DosDevices\Global\CPUZ141`), this could mean **any user** (not just administrators) can open the device and send these IOCTLs. That's an extremely favourable condition for exploitation.

---

## 📊 Standalone Summary

| Aspect | Assessment |
|:---|:---|
| **Validity** | ✅ Signed, kernel-mode, AMD64 driver |
| **Attack surface** | 48 functions, **38 IOCTLs** |
| **Primary primitive** | 7× `MmMapIoSpace` user-controlled (arbitrary physical memory mapping) |
| **Secondary primitive** | PCI config space read/write via `HalGetBusDataByOffset` / `HalSetBusDataByOffset` |
| **Device creation** | ✅ On load (no init sequence required) |
| **Device security** | Global symbolic link, no obvious privilege checks |
| **Blocklist status** | **Very likely present** (CPU-Z driver is well-known in BYOVD) |
| **Overall verdict** | ★★★★★ — Strongest candidate so far, if unblocked |

**The `MmMapIoSpace` primitive, combined with PCI config access and the absence of privilege checks, makes this driver an ideal BYOVD candidate — provided it is not blocked by Microsoft.**

---

## 🛠️ Next Steps for cpuz141.sys

### 1. Verify Against the Microsoft Blocklist
Search `Microsoft_Blocked_Driver_List.pdf` for:
```
ded2927f9a4e64eefd09d0caba78e94f309e3a6292841ae81d5528cab109f95d
```
Given that CPU-Z drivers have been used in BYOVD attacks for years, this is **highly likely to be blocked**. If so, discard it.

### 2. If Unblocked, Investigate the Simplest IOCTL
Start with **line 304** (the first `MmMapIoSpace` finding). Examine:
- Which IOCTL code reaches this handler
- The input buffer layout (offset of physical address, offset of size)
- Any validation logic

### 3. Check for Privilege Gates
Examine the decompiled code for any `SePrivilegeCheck` or `SeAccessCheck` calls. If none, the driver is accessible to any user who can open the global symbolic link.

### 4. Add to Your JOCKY Manifest (Conditional)
If unblocked, add it with highest priority:

```yaml
name: "cpuz141.sys"
sha256: "ded2927f9a4e64eefd09d0caba78e94f309e3a6292841ae81d5528cab109f95d"
device_path: "\\\\.\\Global\\CPUZ141"
device_on_load: true
requires_admin: false  # No privilege checks visible
capabilities: [arb_physical_read, arb_physical_write, pci_config_rw]
priority: 1  # Highest if unblocked
```

### 5. Test in Isolated Windows VM
- Disable HVCI and blocklist
- Enable test signing
- Snapshot before loading
- Load driver, immediately open device, send the simplest `MmMapIoSpace` IOCTL
- Verify physical memory mapping works
- Revert snapshot after

---

## 🧩 Comparison: All Drivers Analyzed So Far

| Feature | Driver A (`65329dad`) | Driver B (`LHA.sys`) | Driver C (`procexp1627.sys`) | Driver D (`mtcBSv64.sys`) | Driver E (`cpuz141.sys`) |
|:---|:---|:---|:---|:---|:---|
| **Size** | 65 KB | 35 KB | 36 KB | 34 KB | 46 KB |
| **Functions** | 155 | 116 | 118 | 97 | 48 |
| **IOCTLs** | 57 | 28 | Not listed | Not listed | **38** |
| **Total HIGH findings** | 38 | 14 | 11 | 9 | 7 |
| **`MmMapIoSpace` findings** | 21 | 13 | 0 | 3 | 7 |
| **`RtlCopyMemory` findings** | 17 | 1 | 8 | 6 | 0 |
| **`KeStackAttachProcess` findings** | 0 | 0 | 3 | 0 | 0 |
| **Primary primitive** | Arbitrary physical memory mapping | Arbitrary physical memory mapping | Cross-process memory access | Arbitrary physical memory mapping | Arbitrary physical memory mapping + PCI config |
| **Device on load** | ✅ True | ❌ False | ❌ False | ❌ False | ✅ True |
| **Device security** | Unknown | Secure device | Admin checks | No privilege checks | **Global symlink, no checks** |
| **Blocklist risk** | Low (verify) | Low (verify) | **Very high** | **Very high** | **Very high** |
| **Priority score** | 3.0 | 3.0 | 2.0 | 3.0 | **6.0** |
| **Overall strength** | ★★★★★ | ★★★★☆ | ★★★★☆ (if unblocked) | ★★★★☆ (if unblocked) | ★★★★★ (if unblocked) |
| **Recommended priority** | 1 (primary) | 2 (fallback) | 3 (only if unblocked) | 4 (only if unblocked) | 1 or 2 (if unblocked) |

**Interpretation:**

- **cpuz141.sys** has the **highest priority score (6.0)** and combines multiple strengths: device created on load, no obvious privilege checks, global symbolic link, 38 IOCTLs, 7 `MmMapIoSpace` primitives, and PCI config access. **If it is not on the blocklist, it is the strongest candidate.** However, CPU-Z drivers are widely known and very likely blocked.
- **Driver A (`65329dad`)** remains the most reliable primary choice because it has the most IOCTLs (57), the most findings (38), two independent primitives (`MmMapIoSpace` + buffer overflow), and a lower blocklist risk than CPU-Z.
- **Driver B (`LHA.sys`)** is a solid fallback with a good number of `MmMapIoSpace` primitives and a secure device descriptor to investigate.
- **Driver C (`procexp1627.sys`)** provides a unique cross-process memory primitive but is likely blocked and gated by privilege checks.
- **Driver D (`mtcBSv64.sys`)** is a capable backup with no privilege checks, but likely blocked.

**Your JOCKY arsenal should prioritise Driver A, then Driver B, then cpuz141.sys if unblocked, with the others as additional fallbacks.**

---



---

## ✅ Bottom Line

**cpuz141.sys is the most powerful candidate in the corpus**, with a priority score of 6.0, device-on-load, global symlink, no privilege checks, 7 `MmMapIoSpace` primitives, and PCI config access. **But it is almost certainly on Microsoft's blocklist.** Verify the hash first. If it is blocked, discard it. If it is somehow unblocked, it becomes your top pick alongside Driver A.

Keep DeepZero running across your corpus — you are building a comprehensive, resilient BYOVD arsenal.