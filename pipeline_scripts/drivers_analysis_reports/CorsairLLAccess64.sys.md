# DeepZero Analysis: CorsairLLAccess64.sys — Full Breakdown

---

## 🐛 The Error (Cosmetic, Not a Blocker)

Same as all previous runs: `assess failed` due to Google Gemini free tier quota exhaustion. All critical stages (`discover`, `kernel_filter`, `decompile`, `semgrep_scanner`, `rank_by_findings`) completed successfully. The 2 HIGH findings are conclusive.

---

## 🎯 Driver Identity

| Field | Value |
|:---|:---|
| **File name** | `CorsairLLAccess64.sys` |
| **SHA-256** | `000547560fea0dd4b477eb28bf781ea67bf83c748945ce8923f90fdd14eb7a4b` |
| **MD5** | `803a371a78d528a44ef8777f67443b16` |
| **Size** | 20,696 bytes (~20 KB) |
| **Machine Type** | `AMD64` — 64-bit x86 |
| **Subsystem** | `NATIVE` — kernel-mode driver |
| **Signed** | `True` — valid signature |
| **Kernel Driver** | `True` — correct target |
| **Priority Score** | **8.0** — highest score, matching AMDRyzenMasterDriver.sys |

This is the **Corsair Link** kernel driver, a component of Corsair’s hardware monitoring and RGB control software. It is a known LOLDrivers entry and therefore very likely on Microsoft’s Vulnerable Driver Blocklist.

**Critical observation:** The `device_name` and `symbolic_link` fields are **blank** in the report (just `\Device\` and `\DosDevices\`). The driver creates a device (`discover.creates_device = True`) and a symbolic link (`discover.creates_symlink = True`), but their names were not captured by the decompiler. This is likely because the names are constructed dynamically (e.g., from a GUID or registry value). You must reverse the code around `IoCreateDevice` and `IoCreateSymbolicLink` to find the actual access path.

---

## 🔓 Attack Surface

### Device Names
```
Device Name:     \Device\  (name unresolved)
Symbolic Link:   \DosDevices\  (name unresolved)
```

The device name is not known. You will need to inspect the decompiled code to determine the actual name. Common patterns for Corsair drivers include fixed names like `\Device\CorsairLLAccess` or GUID-based names.

### Key Flag: `device_on_load_path = True`

The device is created **when the driver loads**. No initialisation sequence required — once loaded, the device exists.

### Attack Surface Numbers
```
Functions:     56
IOCTLs:        11
```

A compact driver with 56 functions and **11 IOCTLs**. The IOCTL surface is relatively small, but the two `MmMapIoSpace` findings and the PCI config imports suggest powerful primitives.

### IOCTL Codes
```
0x00225348, 0x00225358, 0x00225374, 0x0022537C, 0x00225388,
0x0022934C, 0x00229350, 0x00229354, 0x00229378, 0x00229380,
0x00229384
```

These are standard `METHOD_BUFFERED` IOCTLs with a custom device type. The two clusters (`0x2253xx` and `0x2293xx`) suggest two groups of related operations.

---

## ⚠️ The 2 HIGH Severity Findings

### Both are `MmMapIoSpace` with User-Controlled Parameters

**Locations:** Lines 291, 421

**Finding text (repeated twice):**
```
MmMapIoSpace called with parameters derived from user input.
This can allow a user-mode attacker to map arbitrary physical memory
into kernel virtual space, bypassing all protections.
```

**What this means:**
The driver exposes at least one IOCTL handler that passes user-supplied physical address and size to `MmMapIoSpace`. With two separate call sites, there is redundancy. This provides **arbitrary physical memory mapping** — the strongest kernel primitive.

**No `RtlCopyMemory` findings.** This is a pure hardware-access driver, not a buffer overflow target.

---

## 🔍 Dangerous Imports

| Import | Purpose | Risk |
|:---|:---|:---|
| **`MmMapIoSpace`** | Maps physical memory into kernel VA | 🔴 Arbitrary physical memory access |
| **`MmUnmapIoSpace`** | Cleans up mappings | 🟡 Used with the above |
| **`HalGetBusDataByOffset`** | Read PCI bus data | 🔴 PCI config space access |
| **`HalSetBusDataByOffset`** | Write PCI bus data | 🔴 PCI config space modification |
| **`IoAllocateMdl`** | Allocate MDL | 🔴 DMA-like access |
| **`MmBuildMdlForNonPagedPool`** | Build MDL | 🔴 Required for MDL-based access |
| **`MmMapLockedPagesSpecifyCache`** | Map locked pages | 🔴 Kernel memory access |
| **`MmUnmapLockedPages`** | Unmap pages | 🟡 Cleanup |
| **`KeBugCheckEx`** | Triggers BSOD | 🟢 Error handling |
| **`HAL.dll`** | Hardware Abstraction Layer | 🔴 Direct hardware access |

**The critical combination:** `MmMapIoSpace` + `HalGetBusDataByOffset`/`HalSetBusDataByOffset` + `IoAllocateMdl` + `MmMapLockedPagesSpecifyCache` + `HAL.dll`. This is the **same comprehensive hardware access toolkit** seen in AMDRyzenMasterDriver.sys (priority score 8.0). The driver can:
- Map arbitrary physical memory
- Read/write PCI configuration space
- Manipulate MDLs for DMA-like access

**No privilege-checking APIs** are imported (no `SePrivilegeCheck`, `SeAccessCheck`, `SeCaptureSubjectContext`). This suggests the driver may not enforce admin-only access — favourable for exploitation if the device DACL is permissive.

---

## 📊 Standalone Summary

| Aspect | Assessment |
|:---|:---|
| **Validity** | ✅ Signed, kernel-mode, AMD64 driver |
| **Attack surface** | 56 functions, 11 IOCTLs |
| **Primary primitive** | 2× `MmMapIoSpace` user-controlled (arbitrary physical memory mapping) |
| **Secondary primitives** | PCI config space read/write; MDL manipulation |
| **Device creation** | ✅ On load (no init sequence required) |
| **Device access** | ⚠️ Symbolic link name unknown — must be resolved |
| **Device security** | No privilege-check APIs visible |
| **Blocklist status** | **Very likely present** (LOLDrivers entry) |
| **Priority score** | 8.0 |
| **Overall verdict** | ★★★★★ — Exceptionally strong if unblocked; unknown device name is a minor hurdle |

**This is one of the strongest drivers in the corpus.** The combination of `MmMapIoSpace`, PCI config access, MDL manipulation, device-on-load, and a priority score of 8.0 makes it a top-tier candidate. The only open questions are:
1. **Blocklist status** — almost certainly blocked.
2. **Device name** — must be resolved from the decompiled code.

If both are favourable, it would be a **primary candidate** for JOCKY.

---

## 🛠️ Next Steps for CorsairLLAccess64.sys

### 1. Verify Against the Microsoft Blocklist
Search `Microsoft_Blocked_Driver_List.pdf` for:
```
000547560fea0dd4b477eb28bf781ea67bf83c748945ce8923f90fdd14eb7a4b
```
As a LOLDrivers entry, it is **very likely blocked**. If so, discard it.

### 2. Resolve the Device Name
Open the decompiled source:
```
/home/incharanew/JOCKY/DeepZero/work/loldrivers/drivers_out-6603aa5d/.bulk_temp/semgrep_0/000547560fea0dd4_dispatch_ioctl.c
```

Look for the `IoCreateDevice` and `IoCreateSymbolicLink` calls. The device name may be built from a string constant or from registry data. Trace how the `UNICODE_STRING` is populated before the call.

### 3. Investigate the Device DACL
Determine if the device is world-accessible, admin-only, or SYSTEM-only. The absence of DACL APIs in the imports suggests it may not set a restrictive DACL — but always confirm.

### 4. Identify the Best `MmMapIoSpace` Handler
Start with **line 291** (the first `MmMapIoSpace` finding). Analyse:
- Which IOCTL code reaches this handler
- The input buffer layout (offset of physical address, offset of size)
- Any validation logic

### 5. Add to Your JOCKY Manifest (Conditional)
If unblocked:

```yaml
name: "CorsairLLAccess64.sys"
sha256: "000547560fea0dd4b477eb28bf781ea67bf83c748945ce8923f90fdd14eb7a4b"
device_path: "TBD"  # Resolve from decompiled code
device_on_load: true
requires_admin: unknown
capabilities: [arb_physical_read, arb_physical_write, pci_config_rw, mdl_manipulation]
priority: 1  # If unblocked and device accessible
```

### 6. Test in Isolated Windows VM
- Disable HVCI and blocklist
- Enable test signing
- Snapshot before loading
- Load driver, resolve device name, open handle, send the simplest `MmMapIoSpace` IOCTL
- Verify physical memory mapping works
- Revert snapshot after

---

## 🧩 Comparison: All Drivers Analyzed So Far

| Feature | A (`65329dad`) | B (`LHA.sys`) | C (`procexp1627.sys`) | D (`mtcBSv64.sys`) | E (`cpuz141.sys`) | F (`zam64.sys`) | G (`cpuz.sys`) | H (`BdApiUtil.sys`) | I (`AMDRyzenMaster`) | J (`0eab16c7`) | K (`4ed2d2c1`) | L (`b205835b`) | M (`kEvP64`) | N (`libnicm`) | O (`rtkio64`) | P (`rtkiow8x64`) | Q (`AMDPowerProfiler`) | R (`BSMI`) | S (`BSMIx64`) | **T (`CorsairLLAccess64`)** |
|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|
| **Size** | 65 KB | 35 KB | 36 KB | 34 KB | 46 KB | 200 KB | 22 KB | 114 KB | 69 KB | 22 KB | 54 KB | 55 KB | 174 KB | 35 KB | 46 KB | 47 KB | 81 KB | 17 KB | 16 KB | 20 KB |
| **Arch** | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 | I386 | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 |
| **Functions** | 155 | 116 | 118 | 97 | 48 | 348 | 58 | 165 | 173 | 65 | 139 | 150 | 344 | 110 | 81 | 93 | 228 | 27 | 27 | 56 |
| **IOCTLs** | 57 | 28 | n/a | n/a | 38 | 29 | 28 | 12 | 31 | 34 | 20 | 18 | 2 | 6 | 18 | 16 | 15 | n/a | n/a | 11 |
| **HIGH findings** | 38 | 14 | 11 | 9 | 7 | 6 | 5 | 5 | 4 | 3 | 3 | 3 | 3 | 3 | 3 | 3 | 2 | 2 | 2 | 2 |
| **`MmMapIoSpace`** | 21 | 13 | 0 | 3 | 7 | 0 | 5 | 0 | 4 | 2 | 3 | 3 | 1 | 0 | 3 | 3 | 1 | 2 | 2 | 2 |
| **`RtlCopyMemory`** | 17 | 1 | 8 | 6 | 0 | 6 | 0 | 5 | 0 | 1 | 0 | 0 | 0 | 3 | 0 | 0 | 0 | 0 | 0 | 0 |
| **PCI config access** | No | No | No | No | Yes | No | Yes | No | Yes | Yes | No | No | No | No | No | No | No | No | No | **Yes** |
| **Device on load** | ✅ | ❌ | ❌ | ❌ | ✅ | ✅ | ✅ | ✅ | ❌ | ✅ | ❌ | ❌ | ✅ | ❌ | ✅ | ✅ | ❌ | ✅ | ✅ | ✅ |
| **Blocklist risk** | Low | Low | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High |
| **Priority score** | 3.0 | 3.0 | 2.0 | 3.0 | 6.0 | 2.0 | 6.0 | 2.0 | 8.0 | 6.0 | 3.0 | 6.0 | 7.0 | 0.0 | 3.0 | 6.0 | 6.0 | 3.0 | 3.0 | **8.0** |
| **Overall strength** | ★★★★★ | ★★★★☆ | ★★★★☆ | ★★★★☆ | ★★★★★ | ★★★★☆ | ★★☆☆☆ | ★★★★☆ | ★★★★★ | ★★★★★ | ★★★★☆ | ★★★★★ | ★★★★★ | ★★☆☆☆ | ★★★★☆ | ★★★★★ | ★★★★★ | ★★★★☆ | ★★★★☆ | ★★★★★ |
| **Recommended priority** | 1 | 2 | 3 | 4 | 1/2 (if unblocked) | 3/4 (if unblocked) | Do not use | 3 (if unblocked) | 1/2 (if unblocked) | 2 (if unblocked) | 4 (if unblocked) | 2 (if unblocked) | 1/2 (if unblocked) | Do not use | 3 (if unblocked) | 2 (if unblocked) | 1/2 (if unblocked) | 4 (if unblocked) | 4 (if unblocked) | 1/2 (if unblocked) |

**Interpretation:**

- **CorsairLLAccess64.sys** has a priority score of 8.0, matching AMDRyzenMasterDriver.sys as the highest in the corpus. It provides `MmMapIoSpace`, PCI config access, and MDL manipulation, and creates its device on load. It is very likely blocked, and its device name is unknown.
- **Driver A** remains the strongest overall choice due to its low blocklist risk and multiple primitives.
- **Driver B** is a strong fallback.
- **CorsairLLAccess64.sys** is a **top-tier candidate** if unblocked and the device name is resolved. It ranks alongside AMDRyzenMasterDriver.sys.

**For your JOCKY arsenal, the ideal ranking remains:**
1. Driver A (`65329dad`) — primary, low blocklist risk
2. Driver B (`LHA.sys`) — fallback, low blocklist risk
3. Driver T (`CorsairLLAccess64`) — strong if unblocked
4. Driver M (`kEvP64`) — strongest if unblocked
5. Driver I (`AMDRyzenMaster`) — strong if unblocked
6. Driver Q (`AMDPowerProfiler`) — strong if unblocked
7. Driver E (`cpuz141.sys`) — strong if unblocked
8. Driver P (`rtkiow8x64`) — strong if unblocked
9. Driver J (`0eab16c7`) — strong if unblocked
10. Driver L (`b205835b`) — strong if unblocked
11. Driver K (`4ed2d2c1`) — solid if unblocked
12. Driver H (`BdApiUtil.sys`) — process kill if unblocked
13. Driver F (`zam64.sys`) — process kill if unblocked
14. Driver C (`procexp1627.sys`) — cross-process if unblocked
15. Driver D (`mtcBSv64.sys`) — backup if unblocked
16. Driver O (`rtkio64.sys`) — solid if unblocked
17. Driver R (`BSMI.sys`) — simple backup if unblocked
18. Driver S (`BSMIx64.sys`) — simple backup if unblocked
19. Driver G (`cpuz.sys`) — do not use (32-bit)
20. Driver N (`libnicm.sys`) — do not use unless manual reverse confirms viability

---

## 📝 For Your Research Paper

`CorsairLLAccess64.sys` provides a case study of a high-priority hardware-access driver with an obscured device name:

> *“A twentieth driver, `CorsairLLAccess64.sys` (SHA-256: `00054756...`), was analyzed. It is a signed AMD64 kernel driver of 20 KB, identifiable as the Corsair Link kernel component. It exposes 11 IOCTLs across 56 functions and contains two user-controlled `MmMapIoSpace` handlers, providing arbitrary physical memory mapping. It additionally imports `HalGetBusDataByOffset`/`HalSetBusDataByOffset` for PCI configuration space access, `IoAllocateMdl` and `MmMapLockedPagesSpecifyCache` for MDL manipulation, and `HAL.dll` for direct hardware interaction. It creates its device on load and imports no privilege-checking APIs. It received a DeepZero priority score of 8.0 — the highest in the corpus alongside AMDRyzenMasterDriver.sys — indicating exceptional exploitability potential. However, its device name and symbolic link were not resolvable from the decompiled output, requiring manual reverse engineering. Its presence in the LOLDrivers database makes it highly likely to be blocked by Microsoft's Vulnerable Driver Blocklist. This driver reinforces the pattern that hardware utility drivers frequently expose powerful physical memory primitives, and that the highest-priority candidates are often those with the most comprehensive hardware access toolkits. The obscured device name illustrates an operational hurdle that must be overcome before exploitation.”*

**Key insight:** A high priority score does not guarantee immediate exploitability. Obscured device names, blocklist status, and dependency chains can all present hurdles. A comprehensive BYOVD arsenal should include multiple top-tier drivers to ensure that if one is blocked or inaccessible, another can be used.

---

## ✅ Bottom Line

**CorsairLLAccess64.sys is an exceptionally strong driver** with a priority score of 8.0, `MmMapIoSpace`, PCI config access, MDL manipulation, and device-on-load. It is almost certainly blocked (LOLDrivers entry) and its device name is unknown. If unblocked and the device name is resolved, it would be a **top-tier primary candidate** for JOCKY.

**Your practical arsenal remains: Driver A (primary), Driver B (fallback), followed by kEvP64, AMDRyzenMaster, CorsairLLAccess64, AMDPowerProfiler, cpuz141, rtkiow8x64, and the other unblocked candidates.** Continue running DeepZero across your corpus — you are building a comprehensive, resilient BYOVD arsenal.