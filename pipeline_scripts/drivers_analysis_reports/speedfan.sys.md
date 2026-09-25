# DeepZero Analysis: speedfan.sys — Full Breakdown

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
| **File name** | `speedfan.sys` |
| **SHA-256** | `22be050955347661685a4343c51f11c7811674e030386d2264cd12ecbf544b7c` |
| **MD5** | `5f9785e7535f8f602cb294a54962c9e7` |
| **Size** | 14,104 bytes (~14 KB) — very small |
| **Machine Type** | `AMD64` — 64-bit x86 |
| **Subsystem** | `NATIVE` — kernel-mode driver |
| **Signed** | `True` — valid signature |
| **Kernel Driver** | `True` — correct target |
| **Priority Score** | **4.0** |

This is the **SpeedFan** kernel driver — device name `\Device\speedfan` and symbolic link `\DosDevices\Global\SPEEDFAN`. SpeedFan is a well-known hardware monitoring utility. It is a LOLDrivers entry and therefore very likely on Microsoft’s Vulnerable Driver Blocklist.

---

## 🔓 Attack Surface

### Device Names
```
Device Name:     \Device\speedfan
Symbolic Link:   \DosDevices\Global\SPEEDFAN
```

The symbolic link uses the **`Global\` namespace**, meaning it is accessible from any user session, including Remote Desktop sessions. From user mode:
```cpp
CreateFile("\\\\.\\Global\\SPEEDFAN", ...)
```

### Key Flag: `device_on_load_path = True`

The device is created **when the driver loads**. No initialisation sequence required — you can load the driver and immediately open the device.

### Attack Surface Numbers
```
Functions:     18
IOCTLs:        11
```

One of the smallest drivers in the corpus — only 18 functions, but **11 IOCTLs**. The driver is focused almost entirely on IOCTL dispatch.

### IOCTL Codes
```
0x9C402404, 0x9C402408, 0x9C40240C, 0x9C402410, 0x9C402414,
0x9C402418, 0x9C402420, 0x9C402430, 0x9C402434, 0x9C402438,
0x9C40243C
```

Standard `0x9C40` custom device type — typical for hardware utilities. The tight cluster of codes suggests a group of related hardware access operations.

---

## ⚠️ The 2 HIGH Severity Findings

### Both are `MmMapIoSpace` with User-Controlled Parameters

**Locations:** Lines 138, 165

**Finding text (repeated twice):**
```
MmMapIoSpace called with parameters derived from user input.
This can allow a user-mode attacker to map arbitrary physical memory
into kernel virtual space, bypassing all protections.
```

**What this means:**
The driver exposes at least one IOCTL handler that passes user-supplied physical address and size to `MmMapIoSpace`. With two separate call sites, there is redundancy. This provides **arbitrary physical memory mapping** — the strongest kernel primitive.

**No `RtlCopyMemory` or `ZwMapViewOfSection` findings.** This driver’s power is entirely in the `MmMapIoSpace` primitive.

---

## 🔍 Dangerous Imports

| Import | Purpose | Risk |
|:---|:---|:---|
| **`MmMapIoSpace`** | Maps physical memory into kernel VA | 🔴 Arbitrary physical memory access |
| **`MmUnmapIoSpace`** | Cleans up mappings | 🟡 Used with the above |
| **`PsGetVersion`** | Get OS version | 🟢 Standard |
| **`IoCreateDevice`, `IoCreateSymbolicLink`** | Device creation | 🟡 Standard driver setup |
| **`IoDeleteDevice`, `IoDeleteSymbolicLink`** | Cleanup | 🟢 Standard |
| **`KeBugCheckEx`** | Triggers BSOD | 🟢 Error handling |

**Notable:** This driver does **not** import `HalGetBusDataByOffset`/`HalSetBusDataByOffset` (no PCI config access), and does **not** import any security descriptor APIs. It is a minimal, focused driver whose only powerful capability is `MmMapIoSpace`.

**No privilege-checking APIs** are imported (no `SePrivilegeCheck`, `SeAccessCheck`). This suggests the driver may not enforce admin-only access — favourable for exploitation if the device DACL is permissive.

---

## 📊 Standalone Summary

| Aspect | Assessment |
|:---|:---|
| **Validity** | ✅ Signed, kernel-mode, AMD64 driver |
| **Attack surface** | 18 functions, 11 IOCTLs |
| **Primary primitive** | 2× `MmMapIoSpace` user-controlled (arbitrary physical memory mapping) |
| **Secondary primitive** | None (no PCI config, no MDL, no process manipulation) |
| **Device creation** | ✅ On load (no init sequence required) |
| **Device security** | No DACL APIs — relies on default; global symbolic link |
| **Blocklist status** | **Very likely present** (LOLDrivers entry) |
| **Priority score** | 4.0 |
| **Overall verdict** | ★★★★☆ — Solid mid-tier candidate if unblocked; tiny and device-on-load |

**This is a minimal, focused driver** that does exactly one thing: map physical memory. Its small size, global symbolic link, and device-on-load nature make it easy to analyse and potentially exploit. If unblocked and world-accessible, it would be a **solid addition** to your arsenal — not the strongest overall, but simple and reliable.

---

## 🛠️ Next Steps for speedfan.sys

### 1. Verify Against the Microsoft Blocklist
Search `Microsoft_Blocked_Driver_List.pdf` for:
```
22be050955347661685a4343c51f11c7811674e030386d2264cd12ecbf544b7c
```
As a LOLDrivers entry, it is **very likely blocked**. If so, discard it.

### 2. Investigate the Device DACL
Open the decompiled source:
```
/home/incharanew/JOCKY/DeepZero/work/loldrivers/drivers_out-6603aa5d/.bulk_temp/semgrep_0/22be050955347661_dispatch_ioctl.c
```

Look for the `IoCreateDevice` call. Since there are no security descriptor APIs imported, the default DACL will be applied. You can test access by trying to open `\\\\.\\Global\\SPEEDFAN` from a non-admin process in your isolated VM. If it opens, the device is accessible.

### 3. Identify the Best `MmMapIoSpace` Handler
Start with **line 138** (the first `MmMapIoSpace` finding). Analyse:
- Which IOCTL code reaches this handler
- The input buffer layout (offset of physical address, offset of size)
- Any validation logic

### 4. Add to Your JOCKY Manifest (Conditional)
If unblocked:

```yaml
name: "speedfan.sys"
sha256: "22be050955347661685a4343c51f11c7811674e030386d2264cd12ecbf544b7c"
device_path: "\\\\.\\Global\\SPEEDFAN"
device_on_load: true
requires_admin: unknown  # Default DACL to investigate
capabilities: [arb_physical_read, arb_physical_write]
priority: 4  # Solid backup if unblocked
```

### 5. Test in Isolated Windows VM
- Disable HVCI and blocklist
- Enable test signing
- Snapshot before loading
- Load driver, open device, send the simplest `MmMapIoSpace` IOCTL
- Verify physical memory mapping works
- Revert snapshot after

---

## 🧩 Where This Driver Fits

| Driver | Priority | Size | IOCTLs | Key Primitives | Device on Load | Blocklist Risk | Overall |
|:---|:---|:---|:---|:---|:---|:---|:---|
| **T (`CorsairLLAccess64`)** | 8.0 | 20 KB | 11 | 2× MmMapIoSpace, PCI config, MDL | ✅ | Very High | ★★★★★ |
| **I (`AMDRyzenMaster`)** | 8.0 | 69 KB | 31 | 4× MmMapIoSpace, PCI config, MDL | ❌ | Very High | ★★★★★ |
| **LenovoDiagnosticsDriver** | 8.0 | 40 KB | 7 | 2× MmMapIoSpace, PCI config | ❌ | Very High | ★★★★★ |
| **U (`ene.sys`)** | 7.0 | 21 KB | 3 | 2× ZwMapViewOfSection (\Device\PhysicalMemory) | ✅ | Very High | ★★★★★ |
| **M (`kEvP64`)** | 7.0 | 174 KB | 2 | 2× process attach, 1× MmMapIoSpace, process kill | ✅ | Very High | ★★★★★ |
| **E (`cpuz141`)** | 6.0 | 46 KB | 38 | 7× MmMapIoSpace, PCI config | ✅ | Very High | ★★★★★ |
| **P (`rtkiow8x64`)** | 6.0 | 47 KB | 16 | 3× MmMapIoSpace, MDL | ✅ | Very High | ★★★★★ |
| **NTIOLib** | 6.0 | 12 KB | 15 | 2× MmMapIoSpace, PCI config | ✅ | Very High | ★★★★★ |
| **J (`0eab16c7`)** | 6.0 | 22 KB | 34 | 2× MmMapIoSpace, 1× RtlCopyMemory, PCI config | ✅ | Very High | ★★★★★ |
| **New (`speedfan`)** | **4.0** | **14 KB** | **11** | **2× MmMapIoSpace** | ✅ | **Very High** | **★★★★☆** |
| **A (`65329dad`)** | 3.0 | 65 KB | 57 | 21× MmMapIoSpace, 17× RtlCopyMemory | ✅ | Low | ★★★★★ |
| **B (`LHA.sys`)** | 3.0 | 35 KB | 28 | 13× MmMapIoSpace | ❌ | Low | ★★★★☆ |

**Interpretation:**
- **speedfan.sys** is a **solid mid-tier candidate** with priority 4.0, two `MmMapIoSpace` primitives, device-on-load, and a global symbolic link. It lacks PCI config access and MDL manipulation, so it’s less powerful than the top-tier drivers.
- **Driver A** remains the strongest overall choice due to its **low blocklist risk** and multiple primitives.
- **Driver B** is a strong fallback with low blocklist risk.
- If all were unblocked, speedfan.sys would rank around priority 4–5 in your arsenal.

**Updated practical ranking (if all unblocked and accessible):**
1. Driver T (`CorsairLLAccess64`) — 8.0
2. Driver I (`AMDRyzenMaster`) — 8.0
3. Driver LenovoDiagnosticsDriver — 8.0
4. Driver U (`ene.sys`) — 7.0
5. Driver M (`kEvP64`) — 7.0
6. Driver E (`cpuz141`) — 6.0
7. Driver P (`rtkiow8x64`) — 6.0
8. Driver NTIOLib — 6.0
9. Driver J (`0eab16c7`) — 6.0
10. **Driver speedfan — 4.0**
11. Driver A (`65329dad`) — 3.0 (but low blocklist risk)
12. Driver B (`LHA.sys`) — 3.0 (but low blocklist risk)

---

## 📝 For Your Research Paper

`speedfan.sys` provides a case study of a minimal, focused hardware-access driver:

> *“A twenty-sixth driver, `speedfan.sys` (SHA-256: `22be0509...`), was analyzed. It is a signed AMD64 kernel driver of only 14 KB, identifiable as the SpeedFan hardware monitoring component (`\Device\speedfan`). It contains just 18 functions and exposes 11 IOCTLs, including two user-controlled `MmMapIoSpace` handlers that provide arbitrary physical memory mapping. It creates its device on load and exposes a global symbolic link (`\DosDevices\Global\SPEEDFAN`), making it accessible from any user session. It imports only `MmMapIoSpace` and `MmUnmapIoSpace` for hardware access, with no PCI configuration space or MDL manipulation capabilities. It imports no security descriptor APIs, relying on the default device DACL. It received a DeepZero priority score of 4.0, placing it in the mid-tier of the corpus. Its presence in the LOLDrivers database makes it highly likely to be blocked by Microsoft's Vulnerable Driver Blocklist. This driver illustrates that even extremely small hardware monitoring drivers can expose powerful physical memory mapping primitives, and that the global namespace for symbolic links can broaden the attack surface by allowing access from any session.”*

**Key insight:** A driver does not need a large attack surface to be dangerous. Two `MmMapIoSpace` handlers in a 14 KB driver provide the same core primitive as much larger drivers. The absence of PCI config access makes it less versatile, but still useful as a fallback.

---

## ✅ Bottom Line

**speedfan.sys is a solid mid-tier driver** with two `MmMapIoSpace` primitives, device-on-load, and a global symbolic link. It is almost certainly blocked (LOLDrivers entry). If unblocked and accessible, it would be a **good backup addition** to your arsenal.

**Your practical arsenal remains: Driver A (primary, low blocklist risk), Driver B (fallback, low blocklist risk), followed by the top-tier unblocked candidates (CorsairLLAccess64, AMDRyzenMaster, LenovoDiagnosticsDriver, ene.sys, kEvP64, NTIOLib, cpuz141, rtkiow8x64, etc.) and mid-tier backups like speedfan.sys.** Continue running DeepZero across your corpus — you are building a comprehensive, resilient BYOVD arsenal.