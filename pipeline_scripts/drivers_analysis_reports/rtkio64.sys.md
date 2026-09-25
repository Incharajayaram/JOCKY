# DeepZero Analysis: rtkio64.sys — Full Breakdown

---

## 🐛 The Error (Cosmetic, Not a Blocker)

Same as all previous runs: `assess failed` due to Google Gemini free tier quota exhaustion. All critical stages (`discover`, `kernel_filter`, `decompile`, `semgrep_scanner`, `rank_by_findings`) completed successfully. The 3 HIGH findings are conclusive.

---

## 🎯 Driver Identity

| Field | Value |
|:---|:---|
| **File name** | `rtkio64.sys` |
| **SHA-256** | `7133a461aeb03b4d69d43f3d26cd1a9e3ee01694e97a0645a3d8aa1a44c39129` |
| **MD5** | `70dcd07d38017b43f710061f37cb4a91` |
| **Size** | 45,920 bytes (~45 KB) |
| **Machine Type** | `AMD64` — 64-bit x86 |
| **Subsystem** | `NATIVE` — kernel-mode driver |
| **Signed** | `True` — valid signature |
| **Kernel Driver** | `True` — correct target |
| **Priority Score** | 3.0 |

This is another **Realtek I/O driver** — device name `\Device\rtkio` and symbolic link `\DosDevices\rtkio`. It is a LOLDrivers entry, identified by its filename. The driver is smaller than the previous Realtek rtkio variants (~45 KB vs. ~55 KB), has fewer functions (81 vs. 150), but the same number of IOCTLs (18). It imports `HAL.dll` and MDL manipulation APIs, confirming it is a hardware-access driver.

---

## 🔓 Attack Surface

### Device Names
```
Device Name:     \Device\rtkio
Symbolic Link:   \DosDevices\rtkio
```

Friendly, predictable name. From user mode:
```cpp
CreateFile("\\\\.\\rtkio", ...)
```

### Key Flag: `device_on_load_path = True`

The device is created **when the driver loads**. No initialisation sequence required — you can load the driver and immediately open the device. This is a significant advantage over the other Realtek rtkio variants that create their device lazily.

### Attack Surface Numbers
```
Functions:     81
IOCTLs:        18
```

A moderate driver with 81 functions and **18 IOCTLs** — a dense attack surface for its size.

### IOCTL Codes
```
0x80002000, 0x80002004, 0x80002008, 0x8000200C, 0x80002018,
0x8000201C, 0x80002024, 0x80002028, 0x8000202C, 0x80002030,
0x80002034, 0x813610EC, 0x813710EC, 0x816110EC, 0x816610EC,
0x816710EC, 0x816810EC, 0x816910EC
```

**Decoding:**
- `0x8000` prefix for most IOCTLs = `METHOD_BUFFERED`, `FILE_ANY_ACCESS` (world-accessible at the IOCTL level, subject to DACL)
- The `0x8136...`–`0x8169...` codes are vendor-specific for hardware access
- The cluster of `0x816x...` codes suggests a group of related hardware operations

---

## ⚠️ The 3 HIGH Severity Findings

### All 3 are `MmMapIoSpace` with User-Controlled Parameters

**Locations:** Lines 135, 565, 1020

**Finding text (repeated 3 times):**
```
MmMapIoSpace called with parameters derived from user input.
This can allow a user-mode attacker to map arbitrary physical memory
into kernel virtual space, bypassing all protections.
```

**What this means:**
Three separate IOCTL handlers pass user-supplied physical address and size to `MmMapIoSpace`. Your user-mode code can request mapping of **any physical address** into kernel space. This is the strongest kernel primitive — it allows direct physical memory access, enabling token theft, callback removal, PPL stripping, and more.

**No `RtlCopyMemory` findings.** This is a pure hardware-access driver. The three `MmMapIoSpace` instances provide redundancy in case one handler has hidden validation.

---

## 🔍 Dangerous Imports

| Import | Purpose | Risk |
|:---|:---|:---|
| **`MmMapIoSpace`** | Maps physical memory into kernel VA | 🔴 Arbitrary physical memory access |
| **`MmUnmapIoSpace`** | Cleans up mappings | 🟡 Used with the above |
| **`IoAllocateMdl`** | Allocate MDL | 🔴 DMA-like access |
| **`MmBuildMdlForNonPagedPool`** | Build MDL | 🔴 Required for MDL-based access |
| **`MmMapLockedPagesSpecifyCache`** | Map locked pages | 🔴 Kernel memory access |
| **`MmUnmapLockedPages`** | Unmap pages | 🟡 Cleanup |
| **`IoWMIRegistrationControl`, `IoWMIWriteEvent`** | WMI interaction | 🟢 System info |
| **`KeSetSystemAffinityThread`** | Set CPU affinity | 🟢 Performance tuning |
| **`KeStallExecutionProcessor`** | Busy-wait | 🟢 Hardware timing |
| **`KeBugCheckEx`** | Triggers BSOD | 🟢 Error handling |
| **`HAL.dll`** | Hardware Abstraction Layer | 🔴 Direct hardware access |

**The critical combination:** `MmMapIoSpace` + `IoAllocateMdl` + `MmMapLockedPagesSpecifyCache` + `HAL.dll`. This driver can map physical memory and manipulate MDLs for DMA-like access. The lack of `HalGetBusDataByOffset`/`HalSetBusDataByOffset` suggests it focuses on memory-mapped I/O rather than PCI config space, but the `MmMapIoSpace` primitive is still extremely powerful.

**No privilege-checking APIs** are imported (no `SePrivilegeCheck`, `SeAccessCheck`, `SeCaptureSubjectContext`). This suggests the driver may not enforce admin-only access — favourable for exploitation if the device DACL is permissive.

---

## 📊 Standalone Summary

| Aspect | Assessment |
|:---|:---|
| **Validity** | ✅ Signed, kernel-mode, AMD64 driver |
| **Attack surface** | 81 functions, 18 IOCTLs |
| **Primary primitive** | 3× `MmMapIoSpace` user-controlled (arbitrary physical memory mapping) |
| **Secondary primitive** | MDL manipulation (`IoAllocateMdl`, `MmMapLockedPagesSpecifyCache`) |
| **Device creation** | ✅ On load (no init sequence required) |
| **Device security** | No privilege-check APIs visible |
| **Blocklist status** | **Very likely present** (LOLDrivers entry) |
| **Priority score** | 3.0 |
| **Overall verdict** | ★★★★☆ — Strong if unblocked; device-on-load is a plus |

**This is a solid `MmMapIoSpace` driver** with three redundant primitives and MDL manipulation. The device-on-load is a significant advantage over the other rtkio variants. If unblocked and world-accessible, it would be a good addition to your arsenal.

---

## 🛠️ Next Steps for rtkio64.sys

### 1. Verify Against the Microsoft Blocklist
Search `Microsoft_Blocked_Driver_List.pdf` for:
```
7133a461aeb03b4d69d43f3d26cd1a9e3ee01694e97a0645a3d8aa1a44c39129
```
As a LOLDrivers entry, it is **very likely blocked**. If so, discard it.

### 2. Investigate the Device DACL
Open the decompiled source:
```
/home/incharanew/JOCKY/DeepZero/work/loldrivers/drivers_out-6603aa5d/.bulk_temp/semgrep_0/7133a461aeb03b4d_dispatch_ioctl.c
```

Look for the `IoCreateDevice` call and any security descriptor setup. Determine if the device is world-accessible, admin-only, or SYSTEM-only.

### 3. Identify the Best `MmMapIoSpace` Handler
Start with **line 135** (the first `MmMapIoSpace` finding). Analyse:
- Which IOCTL code reaches this handler
- The input buffer layout (offset of physical address, offset of size)
- Any validation logic

### 4. Add to Your JOCKY Manifest (Conditional)
If unblocked:

```yaml
name: "rtkio64.sys"
sha256: "7133a461aeb03b4d69d43f3d26cd1a9e3ee01694e97a0645a3d8aa1a44c39129"
device_path: "\\\\.\\rtkio"
device_on_load: true
requires_admin: unknown  # No privilege-check APIs visible
capabilities: [arb_physical_read, arb_physical_write, mdl_manipulation]
priority: 2  # Strong if unblocked
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

| Feature | A (`65329dad`) | B (`LHA.sys`) | C (`procexp1627.sys`) | D (`mtcBSv64.sys`) | E (`cpuz141.sys`) | F (`zam64.sys`) | G (`cpuz.sys`) | H (`BdApiUtil.sys`) | I (`AMDRyzenMaster`) | J (`0eab16c7`) | K (`4ed2d2c1`) | L (`b205835b`) | M (`kEvP64`) | N (`libnicm`) | **O (`rtkio64`)** |
|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|
| **Size** | 65 KB | 35 KB | 36 KB | 34 KB | 46 KB | 200 KB | 22 KB | 114 KB | 69 KB | 22 KB | 54 KB | 55 KB | 174 KB | 35 KB | 46 KB |
| **Arch** | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 | I386 | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 |
| **Functions** | 155 | 116 | 118 | 97 | 48 | 348 | 58 | 165 | 173 | 65 | 139 | 150 | 344 | 110 | 81 |
| **IOCTLs** | 57 | 28 | n/a | n/a | 38 | 29 | 28 | 12 | 31 | 34 | 20 | 18 | 2 | 6 | 18 |
| **HIGH findings** | 38 | 14 | 11 | 9 | 7 | 6 | 5 | 5 | 4 | 3 | 3 | 3 | 3 | 3 | 3 |
| **`MmMapIoSpace`** | 21 | 13 | 0 | 3 | 7 | 0 | 5 | 0 | 4 | 2 | 3 | 3 | 1 | 0 | 3 |
| **`RtlCopyMemory`** | 17 | 1 | 8 | 6 | 0 | 6 | 0 | 5 | 0 | 1 | 0 | 0 | 0 | 3 | 0 |
| **Process attach** | 0 | 0 | 3 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 | 0 | 0 |
| **Process kill import** | No | No | Yes | No | No | Yes | No | Yes | No | No | No | No | Yes | No | No |
| **`ZwLoadDriver`** | No | No | No | No | No | No | No | No | No | No | No | No | No | Yes | No |
| **Device on load** | ✅ | ❌ | ❌ | ❌ | ✅ | ✅ | ✅ | ✅ | ❌ | ✅ | ❌ | ❌ | ✅ | ❌ | ✅ |
| **Blocklist risk** | Low | Low | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High |
| **Priority score** | 3.0 | 3.0 | 2.0 | 3.0 | 6.0 | 2.0 | 6.0 | 2.0 | 8.0 | 6.0 | 3.0 | 6.0 | 7.0 | 0.0 | 3.0 |
| **Overall strength** | ★★★★★ | ★★★★☆ | ★★★★☆ | ★★★★☆ | ★★★★★ | ★★★★☆ | ★★☆☆☆ | ★★★★☆ | ★★★★★ | ★★★★★ | ★★★★☆ | ★★★★★ | ★★★★★ | ★★☆☆☆ | ★★★★☆ |
| **Recommended priority** | 1 | 2 | 3 | 4 | 1/2 (if unblocked) | 3/4 (if unblocked) | Do not use | 3 (if unblocked) | 1/2 (if unblocked) | 2 (if unblocked) | 4 (if unblocked) | 2 (if unblocked) | 1/2 (if unblocked) | Do not use | 3 (if unblocked) |

**Interpretation:**

- **rtkio64.sys** is a solid `MmMapIoSpace` driver with three redundant primitives and device-on-load. Its priority score of 3.0 reflects its moderate strength. It is very likely blocked.
- **Driver A** remains the strongest overall choice due to its low blocklist risk, device-on-load, high IOCTL count, and multiple primitives.
- **Driver B** is a strong fallback.
- **Drivers I, E, J, L, M** are top-tier if unblocked, but are very likely blocked.
- **rtkio64.sys** ranks behind those top-tier drivers but ahead of the weaker ones.

**For your JOCKY arsenal, the ideal ranking remains:**
1. Driver A (`65329dad`) — primary, low blocklist risk
2. Driver B (`LHA.sys`) — fallback, low blocklist risk
3. Driver M (`kEvP64`) — strongest if unblocked
4. Driver I (`AMDRyzenMaster`) — strong if unblocked
5. Driver E (`cpuz141.sys`) — strong if unblocked
6. Driver J (`0eab16c7`) — strong if unblocked
7. Driver L (`b205835b`) — strong if unblocked
8. Driver K (`4ed2d2c1`) — solid if unblocked
9. Driver H (`BdApiUtil.sys`) — process kill if unblocked
10. Driver F (`zam64.sys`) — process kill if unblocked
11. Driver C (`procexp1627.sys`) — cross-process if unblocked
12. Driver D (`mtcBSv64.sys`) — backup if unblocked
13. Driver O (`rtkio64.sys`) — solid if unblocked
14. Driver G (`cpuz.sys`) — do not use (32-bit)
15. Driver N (`libnicm.sys`) — do not use unless manual reverse confirms viability

---

## 📝 For Your Research Paper

`rtkio64.sys` provides another case study of a hardware utility driver:

> *"A fifteenth driver, `rtkio64.sys` (SHA-256: `7133a461...`), was analyzed. It is a signed AMD64 kernel driver of 46 KB, identifiable as a Realtek I/O driver (`\Device\rtkio`). It exposes 18 IOCTLs across 81 functions and contains three user-controlled `MmMapIoSpace` handlers, providing arbitrary physical memory mapping. It imports MDL manipulation APIs (`IoAllocateMdl`, `MmMapLockedPagesSpecifyCache`) and `HAL.dll`, confirming hardware access capabilities. Unlike some other Realtek rtkio variants in the corpus, it creates its device on load, simplifying exploitation. It imports no privilege-checking APIs, suggesting that the attack surface may be accessible to any user who can open the device, subject to the device DACL. Its presence in the LOLDrivers database makes it highly likely to be blocked by Microsoft's Vulnerable Driver Blocklist, reinforcing the need for a diverse arsenal. This driver illustrates the common pattern of hardware utility drivers exposing powerful physical memory primitives, and the importance of evaluating each variant independently — even within the same driver family, device creation timing and security configurations can differ significantly."*

**Key insight:** Multiple versions of the same driver family (Realtek rtkio) can differ in device creation timing, function count, and security posture. Each must be evaluated independently. A version with device-on-load is more immediately exploitable than one requiring an init sequence.

---

## ✅ Bottom Line

**rtkio64.sys is a solid `MmMapIoSpace` driver** with three redundant primitives, MDL manipulation, and device-on-load. It is almost certainly blocked (LOLDrivers entry). If unblocked and world-accessible, it would be a good addition to your arsenal.

**Your practical arsenal remains: Driver A (primary), Driver B (fallback), followed by kEvP64, AMDRyzenMaster, cpuz141, and the other unblocked candidates.** Continue running DeepZero across your corpus — you are building a comprehensive, resilient BYOVD arsenal.