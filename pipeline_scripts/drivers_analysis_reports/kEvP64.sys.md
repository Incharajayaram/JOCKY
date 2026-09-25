# DeepZero Analysis: kEvP64.sys — Full Breakdown

---

## 🐛 The Error (Cosmetic, Not a Blocker)

Same as all previous runs: `assess failed` due to Google Gemini free tier quota exhaustion. All critical stages completed successfully:

| Stage | Status |
|:---|:---|
| `discover` | ✅ Completed |
| `kernel_filter` | ✅ Completed |
| `decompile` | ✅ Completed |
| `semgrep_scanner` | ✅ Completed |
| `rank_by_findings` | ✅ Completed |
| `assess` | ❌ Failed (quota exceeded) |

The 3 HIGH findings are conclusive. The `assess` stage would only add an AI second opinion.

---

## 🎯 Driver Identity

| Field | Value |
|:---|:---|
| **File name** | `kEvP64.sys` |
| **SHA-256** | `1aaa9aef39cb3c0a854ecb4ca7d3b213458f302025e0ec5bfbdef973cca9111c` |
| **MD5** | `20125794b807116617d43f02b616e092` |
| **Size** | 177,816 bytes (~174 KB) |
| **Machine Type** | `AMD64` — 64-bit x86 |
| **Subsystem** | `NATIVE` — kernel-mode driver |
| **Signed** | `True` — valid signature |
| **Kernel Driver** | `True` — correct target |
| **Priority Score** | **7.0** — very high, second only to AMDRyzenMasterDriver.sys (8.0) |

The file name `kEvP64.sys` and device name `\Device\kEvP64` suggest this is a kernel component of a security product — possibly an antivirus, EDR, or anti-cheat driver. It imports from `FLTMGR.SYS` (file system minifilter), `HAL.dll` (hardware access), and includes process/thread callback registration APIs (`ObRegisterCallbacks`, `PsSetCreateProcessNotifyRoutine`). It is a known LOLDrivers entry and is therefore very likely on Microsoft’s Vulnerable Driver Blocklist.

---

## 🔓 Attack Surface

### Device Names
```
Device Name:     \Device\kEvP64
Symbolic Link:   \DosDevices\kEvP64
```

Friendly, predictable name. From user mode:
```cpp
CreateFile("\\\\.\\kEvP64", ...)
```

### Key Flag: `device_on_load_path = True`

The device is created **when the driver loads**. No initialisation sequence required.

### Attack Surface Numbers
```
Functions:     344
IOCTLs:        2
```

This is a striking mismatch: **344 internal functions but only 2 IOCTLs**. This strongly suggests the driver uses a **command multiplexer**: one or both IOCTLs accept a sub-command ID and data buffer, then dispatch to any of the 344 internal functions. This is a common design in security product drivers, and it means the **2 IOCTLs may expose the full range of driver capabilities** — including process memory access, process termination, file operations, and hardware access.

### IOCTL Codes
```
0x000F423F, 0x0022211C
```

- `0x000F423F` = 999,999 decimal. This is suspicious and likely a magic value or sentinel, not a standard IOCTL. It may be used as a sub-command identifier or an internal marker.
- `0x0022211C` is a typical custom IOCTL code.

Because there are only two, reverse engineering the dispatch table is much easier than with drivers exposing dozens of IOCTLs. Identify the sub-command structure and you can map all capabilities.

---

## ⚠️ The 3 HIGH Severity Findings

### Finding 1 & 3: `PsLookupProcessByProcessId` + `KeStackAttachProcess` (Lines 1307, 3311)

**Finding text (repeated twice):**
```
PsLookupProcessByProcessId followed by KeStackAttachProcess.
Classic process manipulation primitive for cross-process memory operations.
```

**What this means:**
The driver looks up a process by PID and attaches to its address space. While attached, it can **read and write memory** in the target process’s context. This is the core primitive for:
- Credential dumping (reading LSASS memory)
- EDR tampering (writing to EDR process memory)
- Bypassing PPL in some cases
- Cross-process injection

Two separate instances provide redundancy.

### Finding 2: `MmMapIoSpace` with User-Controlled Parameters (Line 2834)

**Finding text:**
```
MmMapIoSpace called with parameters derived from user input.
This can allow a user-mode attacker to map arbitrary physical memory
into kernel virtual space, bypassing all protections.
```

**What this means:**
The driver exposes a handler that passes user-supplied physical address and size to `MmMapIoSpace`. This provides **arbitrary physical memory mapping** — the strongest kernel primitive. Combined with the cross-process memory primitives, this driver can:
- Map any physical memory (including kernel structures)
- Attach to any process and read/write its memory
- Terminate processes (via `ZwTerminateProcess`, which is imported)
- Access PCI configuration space (via `HalGetBusDataByOffset`/`HalSetBusDataByOffset`)

No `RtlCopyMemory` findings — the driver’s power is in its high-level primitives, not memory-safety bugs.

---

## 🔍 Dangerous Imports and Inferred Capabilities

| Import | Purpose | Inferred Capability |
|:---|:---|:---|
| **`KeStackAttachProcess`** | Attach to another process | Cross-process memory R/W |
| **`PsLookupProcessByProcessId`** | Look up process by PID | Required for above |
| **`ZwTerminateProcess`** | Terminate a process | Kill EDR/AV processes |
| **`MmMapIoSpace`** | Map physical memory | Arbitrary physical memory access |
| **`MmProbeAndLockPages`** | Lock pages for DMA | Physical memory access |
| **`IoAllocateMdl`, `MmBuildMdlForNonPagedPool`** | MDL manipulation | DMA-like access |
| **`HalGetBusDataByOffset`, `HalSetBusDataByOffset`** | PCI config space R/W | Hardware manipulation |
| **`ObRegisterCallbacks`, `ObUnRegisterCallbacks`** | Object callbacks | Handle/process protection |
| **`PsSetCreateProcessNotifyRoutine`, `PsRemoveCreateThreadNotifyRoutine`** | Process/thread callbacks | Monitoring or unhooking |
| **`FLTMGR.SYS`** | File system minifilter | File operation interception |
| **`ProbeForRead`, `ProbeForWrite`** | Validate user buffers | Safe user-mode access |
| **`ExGetPreviousMode`** | Determine caller privilege | Possible privilege checks |
| **`SeCreateAccessState`** | Security access checks | Possible privilege gating |

**This is a security product’s kernel driver, repurposed for offense.** The combination of process attach, process termination, physical memory mapping, MDL manipulation, PCI config access, and file system minifilter means the driver can do nearly anything a kernel attacker would want.

**Privilege checks:** The imports of `ExGetPreviousMode`, `ProbeForRead`, `ProbeForWrite`, and `SeCreateAccessState` indicate the driver **does perform some privilege validation**. However, the `MmMapIoSpace` and process attach handlers may or may not be gated. Manual reverse engineering is required to determine which IOCTL/sub-command paths are accessible without admin rights.

---

## 📊 Standalone Summary

| Aspect | Assessment |
|:---|:---|
| **Validity** | ✅ Signed, kernel-mode, AMD64 driver |
| **Attack surface** | 344 functions, **2 IOCTLs** (likely command multiplexer) |
| **Primary primitives** | 2× cross-process memory access (`KeStackAttachProcess`); 1× arbitrary physical memory mapping (`MmMapIoSpace`) |
| **Secondary primitives** | Process termination (`ZwTerminateProcess`), PCI config access, MDL manipulation, file system minifilter |
| **Device creation** | ✅ On load |
| **Device security** | Some privilege checks visible (`ExGetPreviousMode`, `ProbeFor*`, `SeCreateAccessState`) |
| **Blocklist status** | **Very likely present** (LOLDrivers entry) |
| **Priority score** | 7.0 |
| **Overall verdict** | ★★★★★ — Extremely powerful if unblocked; likely blocked and may require admin |

**This is one of the strongest drivers in the corpus.** It combines cross-process memory access, process termination, arbitrary physical memory mapping, PCI config access, and file system interception. If it is not blocked and the IOCTL sub-commands are accessible without admin privileges, it would be a **top-tier primary candidate** for JOCKY.

The only obstacles are:
1. **Blocklist status** — almost certainly blocked.
2. **Privilege checks** — some operations may require admin.
3. **Command multiplexer reverse engineering** — the two IOCTLs need to be decoded to find the sub-commands that trigger each primitive.

---

## 🛠️ Next Steps for kEvP64.sys

### 1. Verify Against the Microsoft Blocklist
Search `Microsoft_Blocked_Driver_List.pdf` for:
```
1aaa9aef39cb3c0a854ecb4ca7d3b213458f302025e0ec5bfbdef973cca9111c
```
As a LOLDrivers entry, it is **very likely blocked**. If so, discard it.

### 2. Reverse the IOCTL Dispatch
Open the decompiled source:
```
/home/incharanew/JOCKY/DeepZero/work/loldrivers/drivers_out-6603aa5d/.bulk_temp/semgrep_0/1aaa9aef39cb3c0a_dispatch_ioctl.c
```

Since there are only two IOCTLs, start with the dispatch function `FUN_0002a130`. Identify how the input buffer is parsed:
- Does it contain a command ID?
- Are there sub-command handlers for process attach, process kill, physical memory map?
- What privileges are required for each sub-command?

### 3. Check Privilege Gates
Look for `ExGetPreviousMode`, `SeCreateAccessState`, and any `SeAccessCheck` calls. Determine whether the dangerous primitives are accessible without admin rights.

### 4. Add to Your JOCKY Manifest (Conditional)
If unblocked:

```yaml
name: "kEvP64.sys"
sha256: "1aaa9aef39cb3c0a854ecb4ca7d3b213458f302025e0ec5bfbdef973cca9111c"
device_path: "\\\\.\\kEvP64"
device_on_load: true
requires_admin: unknown  # Investigate
capabilities: [process_attach, process_memory_rw, process_kill, arb_physical_read, arb_physical_write, pci_config_rw, mdl_manipulation, file_system_intercept]
priority: 1  # If unblocked
```

### 5. Test in Isolated Windows VM
- Disable HVCI and blocklist
- Enable test signing
- Snapshot before loading
- Load driver, open device, send the IOCTL with sub-commands to test process attach and physical memory mapping
- Verify primitives work
- Revert snapshot after

---

## 🧩 Comparison: All Drivers Analyzed So Far

| Feature | A (`65329dad`) | B (`LHA.sys`) | C (`procexp1627.sys`) | D (`mtcBSv64.sys`) | E (`cpuz141.sys`) | F (`zam64.sys`) | G (`cpuz.sys`) | H (`BdApiUtil.sys`) | I (`AMDRyzenMaster`) | J (`0eab16c7`) | K (`4ed2d2c1`) | L (`b205835b`) | **M (`kEvP64`)** |
|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|
| **Size** | 65 KB | 35 KB | 36 KB | 34 KB | 46 KB | 200 KB | 22 KB | 114 KB | 69 KB | 22 KB | 54 KB | 55 KB | 174 KB |
| **Arch** | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 | I386 | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 |
| **Functions** | 155 | 116 | 118 | 97 | 48 | 348 | 58 | 165 | 173 | 65 | 139 | 150 | 344 |
| **IOCTLs** | 57 | 28 | n/a | n/a | 38 | 29 | 28 | 12 | 31 | 34 | 20 | 18 | **2** |
| **HIGH findings** | 38 | 14 | 11 | 9 | 7 | 6 | 5 | 5 | 4 | 3 | 3 | 3 | 3 |
| **`MmMapIoSpace`** | 21 | 13 | 0 | 3 | 7 | 0 | 5 | 0 | 4 | 2 | 3 | 3 | 1 |
| **Process attach** | 0 | 0 | 3 | 0 | 0 | 0 (imports) | 0 | 0 (imports) | 0 | 0 | 0 | 0 | **2** |
| **Process kill import** | No | No | Yes | No | No | Yes | No | Yes | No | No | No | No | **Yes** |
| **Device on load** | ✅ | ❌ | ❌ | ❌ | ✅ | ✅ | ✅ | ✅ | ❌ | ✅ | ❌ | ❌ | ✅ |
| **Blocklist risk** | Low | Low | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High | Very High | **Very High** |
| **Priority score** | 3.0 | 3.0 | 2.0 | 3.0 | 6.0 | 2.0 | 6.0 | 2.0 | 8.0 | 6.0 | 3.0 | 6.0 | **7.0** |
| **Overall strength** | ★★★★★ | ★★★★☆ | ★★★★☆ | ★★★★☆ | ★★★★★ | ★★★★☆ | ★★☆☆☆ | ★★★★☆ | ★★★★★ | ★★★★★ | ★★★★☆ | ★★★★★ | ★★★★★ |
| **Recommended priority** | 1 | 2 | 3 | 4 | 1/2 (if unblocked) | 3/4 (if unblocked) | Do not use | 3 (if unblocked) | 1/2 (if unblocked) | 2 (if unblocked) | 4 (if unblocked) | 2 (if unblocked) | **1/2 (if unblocked)** |

**Interpretation:**

- **kEvP64.sys** has a priority score of 7.0 and combines cross-process memory access (2×), arbitrary physical memory mapping (1×), and process termination. It is one of the strongest drivers in the corpus.
- **However**, it is a known LOLDrivers entry, so it is **very likely blocked**.
- If unblocked, it would rank **alongside Driver I (AMDRyzenMaster) and Driver A** as a top-tier candidate.
- The only practical hurdles are blocklist status, possible privilege checks, and reverse engineering the 2-IOCTL command multiplexer.

**For your JOCKY arsenal, the ideal ranking is:**
1. **Driver A (`65329dad`)** — primary, low blocklist risk, strong primitives
2. **Driver B (`LHA.sys`)** — fallback, low blocklist risk
3. **Driver M (`kEvP64`)** — strongest if unblocked, but very likely blocked
4. **Driver I (`AMDRyzenMaster`)** — strong if unblocked
5. **Driver E (`cpuz141.sys`)** — strong if unblocked
6. **Driver J (`0eab16c7`)** — strong if unblocked
7. **Driver L (`b205835b`)** — strong if unblocked
8. **Driver K (`4ed2d2c1`)** — solid if unblocked
9. **Driver H (`BdApiUtil.sys`)** — process kill if unblocked
10. **Driver F (`zam64.sys`)** — process kill if unblocked
11. **Driver C (`procexp1627.sys`)** — cross-process if unblocked
12. **Driver D (`mtcBSv64.sys`)** — backup if unblocked
13. **Driver G (`cpuz.sys`)** — do not use (32-bit)

---


---

## ✅ Bottom Line

**kEvP64.sys is one of the strongest drivers in the corpus**, with a priority score of 7.0, cross-process memory access, process termination, and arbitrary physical memory mapping. **It is almost certainly blocked** (LOLDrivers entry) and may have privilege checks. If unblocked and accessible, it would be a top-tier primary candidate for JOCKY.

**Your practical arsenal remains: Driver A (primary), Driver B (fallback), followed by kEvP64, AMDRyzenMaster, cpuz141, and the other unblocked candidates.** Continue running DeepZero across your corpus — you are building a comprehensive, resilient BYOVD arsenal.