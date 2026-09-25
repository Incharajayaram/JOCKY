# DeepZero Analysis: BSMIx64.sys — Full Breakdown

---

## 🐛 The Error (Cosmetic, Not a Blocker)

Same as all previous runs: `assess failed` due to Google Gemini free tier quota exhaustion. All critical stages (`discover`, `kernel_filter`, `decompile`, `semgrep_scanner`, `rank_by_findings`) completed successfully. The 2 HIGH findings are conclusive.

---

## 🎯 Driver Identity

| Field | Value |
|:---|:---|
| **File name** | `BSMIx64.sys` |
| **SHA-256** | `552f70374715e70c4ade591d65177be2539ec60f751223680dfaccb9e0be0ed9` |
| **MD5** | `444f538daa9f7b340cfd43974ed43690` |
| **Size** | 16,504 bytes (~16 KB) — very small |
| **Machine Type** | `AMD64` — 64-bit x86 |
| **Subsystem** | `NATIVE` — kernel-mode driver |
| **Signed** | `True` — valid signature |
| **Kernel Driver** | `True` — correct target |
| **Priority Score** | 3.0 |

This driver is **functionally identical to `BSMI.sys`** analysed previously. The device name (`\Device\BSMI`), symbolic link (`\DosDevices\BSMI`), function count (27), imports, imphash (`59cfec2c58e7fc283795064756d843ff`), and the two `MmMapIoSpace` findings are all the same. The only differences are:
- **File name**: `BSMIx64.sys` (vs. `BSMI.sys`)
- **Size**: 16,504 bytes vs. 17,056 bytes (slightly smaller)
- **MD5/SHA-256**: Different hashes
- **Device creation function address**: `FUN_000160c0` vs. `FUN_000170c0`

This is almost certainly a **different build** of the same driver (perhaps a slightly newer or older version). The identical imphash confirms the import table is the same, and the same function count and dispatch name (`FUN_00015130`) strongly suggest the code is nearly identical.

---

## 🔓 Attack Surface

### Device Names
```
Device Name:     \Device\BSMI
Symbolic Link:   \DosDevices\BSMI
```

Friendly, predictable name. From user mode:
```cpp
CreateFile("\\\\.\\BSMI", ...)
```

### Key Flag: `device_on_load_path = True`

The device is created **when the driver loads**. No initialisation sequence required.

### Attack Surface Numbers
```
Functions:     27
IOCTLs:        Not explicitly listed, but dispatch_ioctl.c exists
```

Only 27 functions total — extremely small. The two `MmMapIoSpace` findings confirm IOCTLs are present.

---

## ⚠️ The 2 HIGH Severity Findings

### Both are `MmMapIoSpace` with User-Controlled Parameters

**Locations:** Lines 80, 87

**Finding text (repeated twice):**
```
MmMapIoSpace called with parameters derived from user input.
This can allow a user-mode attacker to map arbitrary physical memory
into kernel virtual space, bypassing all protections.
```

**What this means:**
The driver exposes at least one IOCTL handler that passes user-supplied physical address and size to `MmMapIoSpace`. With two separate call sites, there is redundancy. This provides **arbitrary physical memory mapping** — the strongest kernel primitive.

**No `RtlCopyMemory` findings.** This driver is a pure hardware access primitive, not a buffer overflow target.

---

## 🔍 Dangerous Imports

| Import | Purpose | Risk |
|:---|:---|:---|
| **`MmMapIoSpace`** | Maps physical memory into kernel VA | 🔴 Arbitrary physical memory access |
| **`MmUnmapIoSpace`** | Cleans up mappings | 🟡 Used with the above |
| **`MmGetPhysicalAddress`** | Translates VA → physical address | 🔴 Enables physical memory attacks |
| **`IoCreateDevice`, `IoCreateSymbolicLink`** | Device creation | 🟡 Standard driver setup |
| **`IoDeleteDevice`, `IoDeleteSymbolicLink`** | Cleanup | 🟢 Standard |
| **`KeBugCheckEx`** | Triggers BSOD | 🟢 Error handling |
| **`DbgPrint`** | Kernel debug logging | 🟢 Debug output |
| **`RtlAssert`** | Assertion checking | 🟢 Debug |

**The critical trio:** `MmMapIoSpace` + `MmGetPhysicalAddress` + `MmUnmapIoSpace` — the classic hardware access primitive. This tiny driver exists solely to provide physical memory mapping.

**No privilege-checking APIs** are imported. This suggests the driver may not enforce admin-only access — favourable for exploitation if the device DACL is permissive.

---

## 📊 Standalone Summary

| Aspect | Assessment |
|:---|:---|
| **Validity** | ✅ Signed, kernel-mode, AMD64 driver |
| **Attack surface** | 27 functions, IOCTLs present (exact count not listed) |
| **Primary primitive** | 2× `MmMapIoSpace` user-controlled (arbitrary physical memory mapping) |
| **Device creation** | ✅ On load (no init sequence required) |
| **Device security** | No privilege-check APIs visible |
| **Blocklist status** | **Very likely present** (LOLDrivers entry) |
| **Priority score** | 3.0 |
| **Overall verdict** | ★★★★☆ — Strong if unblocked; tiny, simple, device-on-load |

**This is a minimal, focused driver** that does exactly one thing: map physical memory. Its small size and device-on-load nature make it easy to analyse and potentially exploit. If unblocked and world-accessible, it would be a **solid addition** to your arsenal — not the strongest overall, but simple and reliable.

---

## 🛠️ Next Steps for BSMIx64.sys

### 1. Verify Against the Microsoft Blocklist
Search `Microsoft_Blocked_Driver_List.pdf` for:
```
552f70374715e70c4ade591d65177be2539ec60f751223680dfaccb9e0be0ed9
```
As a LOLDrivers entry, it is **very likely blocked**. If so, discard it.

### 2. Investigate the Device DACL
Open the decompiled source:
```
/home/incharanew/JOCKY/DeepZero/work/loldrivers/drivers_out-6603aa5d/.bulk_temp/semgrep_0/552f70374715e70c_dispatch_ioctl.c
```

Look for the `IoCreateDevice` call and any security descriptor setup. Determine if the device is world-accessible, admin-only, or SYSTEM-only.

### 3. Identify the Best `MmMapIoSpace` Handler
Start with **line 80** (the first `MmMapIoSpace` finding). Analyse:
- Which IOCTL code reaches this handler
- The input buffer layout (offset of physical address, offset of size)
- Any validation logic

### 4. Add to Your JOCKY Manifest (Conditional)
If unblocked:

```yaml
name: "BSMIx64.sys"
sha256: "552f70374715e70c4ade591d65177be2539ec60f751223680dfaccb9e0be0ed9"
device_path: "\\\\.\\BSMI"
device_on_load: true
requires_admin: unknown  # No privilege-check APIs visible
capabilities: [arb_physical_read, arb_physical_write]
priority: 4  # Solid, but lower than larger drivers
```

### 5. Test in Isolated Windows VM
- Disable HVCI and blocklist
- Enable test signing
- Snapshot before loading
- Load driver, open device, send the simplest `MmMapIoSpace` IOCTL
- Verify physical memory mapping works
- Revert snapshot after

---

## 🧩 Comparison: All Drivers Analyzed So Far

| Feature | A (`65329dad`) | B (`LHA.sys`) | C (`procexp1627.sys`) | D (`mtcBSv64.sys`) | E (`cpuz141.sys`) | F (`zam64.sys`) | G (`cpuz.sys`) | H (`BdApiUtil.sys`) | I (`AMDRyzenMaster`) | J (`0eab16c7`) | K (`4ed2d2c1`) | L (`b205835b`) | M (`kEvP64`) | N (`libnicm`) | O (`rtkio64`) | P (`rtkiow8x64`) | Q (`AMDPowerProfiler`) | R (`BSMI`) | **S (`BSMIx64`)** |
|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|
| **Size** | 65 KB | 35 KB | 36 KB | 34 KB | 46 KB | 200 KB | 22 KB | 114 KB | 69 KB | 22 KB | 54 KB | 55 KB | 174 KB | 35 KB | 46 KB | 47 KB | 81 KB | 17 KB | 16 KB |
| **Arch** | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 | I386 | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 |
| **Functions** | 155 | 116 | 118 | 97 | 48 | 348 | 58 | 165 | 173 | 65 | 139 | 150 | 344 | 110 | 81 | 93 | 228 | 27 | 27 |
| **IOCTLs** | 57 | 28 | n/a | n/a | 38 | 29 | 28 | 12 | 31 | 34 | 20 | 18 | 2 | 6 | 18 | 16 | 15 | n/a | n/a |
| **HIGH findings** | 38 | 14 | 11 | 9 | 7 | 6 | 5 | 5 | 4 | 3 | 3 | 3 | 3 | 3 | 3 | 3 | 2 | 2 | 2 |
| **`MmMapIoSpace`** | 21 | 13 | 0 | 3 | 7 | 0 | 5 | 0 | 4 | 2 | 3 | 3 | 1 | 0 | 3 | 3 | 1 | 2 | 2 |
| **`RtlCopyMemory`** | 17 | 1 | 8 | 6 | 0 | 6 | 0 | 5 | 0 | 1 | 0 | 0 | 0 | 3 | 0 | 0 | 0 | 0 | 0 |
| **`ZwMapViewOfSection`** | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 0 | 0 |
| **Process attach** | 0 | 0 | 3 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 | 0 | 0 | 0 | 0 | 0 | 0 |
| **Process kill import** | No | No | Yes | No | No | Yes | No | Yes | No | No | No | No | Yes | No | No | No | No | No | No |
| **`ZwLoadDriver`** | No | No | No | No | No | No | No | No | No | No | No | No | No | Yes | No | No | No | No | No |
| **Device on load** | ✅ | ❌ | ❌ | ❌ | ✅ | ✅ | ✅ | ✅ | ❌ | ✅ | ❌ | ❌ | ✅ | ❌ | ✅ | ✅ | ❌ | ✅ | ✅ |
| **Blocklist risk** | Low | Low | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High |
| **Priority score** | 3.0 | 3.0 | 2.0 | 3.0 | 6.0 | 2.0 | 6.0 | 2.0 | 8.0 | 6.0 | 3.0 | 6.0 | 7.0 | 0.0 | 3.0 | 6.0 | 6.0 | 3.0 | 3.0 |
| **Overall strength** | ★★★★★ | ★★★★☆ | ★★★★☆ | ★★★★☆ | ★★★★★ | ★★★★☆ | ★★☆☆☆ | ★★★★☆ | ★★★★★ | ★★★★★ | ★★★★☆ | ★★★★★ | ★★★★★ | ★★☆☆☆ | ★★★★☆ | ★★★★★ | ★★★★★ | ★★★★☆ | ★★★★☆ |
| **Recommended priority** | 1 | 2 | 3 | 4 | 1/2 (if unblocked) | 3/4 (if unblocked) | Do not use | 3 (if unblocked) | 1/2 (if unblocked) | 2 (if unblocked) | 4 (if unblocked) | 2 (if unblocked) | 1/2 (if unblocked) | Do not use | 3 (if unblocked) | 2 (if unblocked) | 1/2 (if unblocked) | 4 (if unblocked) | 4 (if unblocked) |

**Interpretation:**

- **BSMIx64.sys** is a near-duplicate of **BSMI.sys** with a different hash and slightly smaller size. It has the same primitives, same device name, same function count. It is almost certainly blocked (LOLDrivers entry). If unblocked, it would be a solid backup.
- **Driver A** remains the strongest overall choice due to its low blocklist risk and multiple primitives.
- **Driver B** is a strong fallback.
- **BSMIx64.sys** ranks alongside BSMI.sys as a simple backup.

**For your JOCKY arsenal, the ideal ranking remains:**
1. Driver A (`65329dad`) — primary, low blocklist risk
2. Driver B (`LHA.sys`) — fallback, low blocklist risk
3. Driver M (`kEvP64`) — strongest if unblocked
4. Driver I (`AMDRyzenMaster`) — strong if unblocked
5. Driver Q (`AMDPowerProfiler`) — strong if unblocked
6. Driver E (`cpuz141.sys`) — strong if unblocked
7. Driver P (`rtkiow8x64`) — strong if unblocked
8. Driver J (`0eab16c7`) — strong if unblocked
9. Driver L (`b205835b`) — strong if unblocked
10. Driver K (`4ed2d2c1`) — solid if unblocked
11. Driver H (`BdApiUtil.sys`) — process kill if unblocked
12. Driver F (`zam64.sys`) — process kill if unblocked
13. Driver C (`procexp1627.sys`) — cross-process if unblocked
14. Driver D (`mtcBSv64.sys`) — backup if unblocked
15. Driver O (`rtkio64.sys`) — solid if unblocked
16. Driver R (`BSMI.sys`) — simple backup if unblocked
17. Driver S (`BSMIx64.sys`) — simple backup if unblocked
18. Driver G (`cpuz.sys`) — do not use (32-bit)
19. Driver N (`libnicm.sys`) — do not use unless manual reverse confirms viability

---

## 📝 For Your Research Paper

`BSMIx64.sys` provides another case study of multiple versions of the same driver:

> *“A nineteenth driver, `BSMIx64.sys` (SHA-256: `552f7037...`), was analyzed. It is functionally identical to the previously analyzed `BSMI.sys`, with the same device name (`\Device\BSMI`), symbolic link (`\DosDevices\BSMI`), function count (27), import table (identical imphash), and two user-controlled `MmMapIoSpace` handlers. The only differences are the file name, size (16 KB vs. 17 KB), and binary hashes. It received the same priority score (3.0). This illustrates that hardware vendors often ship multiple builds of the same driver, and each must be evaluated independently against the blocklist. A driver that is blocked in one build may be unblocked in another, or vice versa. A comprehensive BYOVD arsenal should include multiple versions of the same driver family to maximize resilience.”*

**Key insight:** Identical imphash and function count strongly indicate a near-identical driver. Multiple builds exist, and each must be checked separately against the blocklist. This reinforces the need for a dynamic, version-aware driver arsenal.

---

## ✅ Bottom Line

**BSMIx64.sys is a near-duplicate of BSMI.sys** with a different hash. It provides two `MmMapIoSpace` primitives, device-on-load, and no obvious privilege checks. It is almost certainly blocked (LOLDrivers entry). If unblocked and world-accessible, it would be a solid backup addition to your arsenal.

**Your practical arsenal remains: Driver A (primary), Driver B (fallback), followed by kEvP64, AMDRyzenMaster, AMDPowerProfiler, cpuz141, rtkiow8x64, and the other unblocked candidates.** Continue running DeepZero across your corpus — you are building a comprehensive, resilient BYOVD arsenal.