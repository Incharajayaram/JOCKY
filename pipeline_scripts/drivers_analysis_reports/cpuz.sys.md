# DeepZero Analysis: cpuz.sys — Full Breakdown

---

## 🐛 The Error (Cosmetic, Not a Blocker)

Same as previous runs: `assess failed` due to Google Gemini free tier quota exhaustion. All critical stages completed successfully. The 34 findings (5 HIGH, 29 MEDIUM) are conclusive.

---

## 🎯 Driver Identity

| Field | Value |
|:---|:---|
| **File name** | `cpuz.sys` |
| **SHA-256** | `8c95d28270a4a314299cf50f05dcbe63033b2a555195d2ad2f678e09e00393e6` |
| **MD5** | `c2eb4539a4f6ab6edd01bdc191619975` |
| **Size** | 21,992 bytes (~22 KB) |
| **Machine Type** | **`I386` — 32-bit x86** |
| **Subsystem** | `NATIVE` — kernel-mode driver |
| **Signed** | `True` — valid signature |
| **Kernel Driver** | `True` — correct target |
| **Priority Score** | **6.0** — highest, same as cpuz141.sys |

This is an **older, 32-bit CPU-Z kernel driver** (device name `cpuz135`). The `I386` machine type is a **critical limitation**: this driver will only load on **32-bit Windows**. On 64-bit Windows (which is virtually all modern systems), it cannot be loaded at all. This makes it largely irrelevant for modern BYOVD attacks, but it remains a useful case study for the evolution of CPU-Z driver vulnerabilities.

---

## 🔓 Attack Surface

### Device Names
```
Device Name:     \Device\cpuz135
Symbolic Link:   \DosDevices\CPUZ135
```

Friendly, predictable name. From user mode:
```cpp
CreateFile("\\\\.\\CPUZ135", ...)
```

### Key Flag: `device_on_load_path = True`

The device is created **when the driver loads**. No initialisation sequence required.

### Attack Surface Numbers
```
Functions:     58
IOCTLs:        28
```

A modest number of functions and IOCTLs, but the findings show a **dense concentration of hardware primitives**.

### IOCTL Codes
```
0x5F534750, 0x5F5F5F5C,
0x9C402400, 0x9C402404, 0x9C402408, 0x9C40240C, 0x9C402410,
0x9C402414, 0x9C402418, 0x9C402420, 0x9C402424, 0x9C402428,
0x9C40242C, 0x9C402430, 0x9C402438, 0x9C40243C, 0x9C402448,
0x9C40244C, 0x9C402450, 0x9C402454, 0x9C402458, 0x9C40245C,
0x9C402460, 0x9C402464, 0x9C402468, 0x9C4024A8, 0x9C402500,
0x9C402504
```

Same `0x9C40` custom device type as cpuz141.sys. The two anomalous values (`0x5F534750`, `0x5F5F5F5C`) may be magic numbers or internal control codes.

---

## ⚠️ The 34 Findings

### HIGH Severity: 5× `MmMapIoSpace` with User-Controlled Parameters

**Locations:** Lines 255, 376, 406, 693, 954

**Finding text (repeated 5 times):**
```
MmMapIoSpace called with parameters derived from user input.
This can allow a user-mode attacker to map arbitrary physical memory
into kernel virtual space, bypassing all protections.
```

Five separate IOCTL handlers that pass user-supplied physical address and size to `MmMapIoSpace`. Provides arbitrary physical memory mapping.

### MEDIUM Severity: 29× Direct I/O Port Access with User-Controlled Port Number

**Locations:** Lines 191, 206, 230, 237, 414, 417, 424, 739, 742, 751, 788, 789, 790, 791, 793, 796, 800, 810, 816, 822, 823, 845, 846, 847, 848, 852, 860, 978, 979

**Finding text (repeated 29 times):**
```
Direct I/O port access with potentially user-controlled port number.
Can be used to interact with hardware directly.
```

**What this means:**
The driver imports `READ_PORT_*` and `WRITE_PORT_*` functions and exposes **29 IOCTL handlers** that allow the user to specify a **port number** and **read or write** to it directly. This is an **unusually rich hardware access primitive** — more granular than `MmMapIoSpace` alone.

**Why 29 is significant:**
- You can read/write arbitrary I/O ports, which control:
  - **PCI configuration space** (via `0xCF8`/`0xCFC` ports)
  - **CMOS/NVRAM** (via `0x70`/`0x71` ports)
  - **Keyboard controller** (which can be abused for DMA attacks)
  - **Any hardware device** with an I/O port interface
- Combined with `MmMapIoSpace`, this gives a **complete hardware control toolkit**.

**Note:** I/O port access is marked MEDIUM because it's hardware-dependent and less directly exploitable than `MmMapIoSpace` for kernel memory corruption. However, a skilled attacker can use port I/O to perform **DMA attacks** (e.g., via the keyboard controller or PCI BAR manipulation) that bypass IOMMU protections in some configurations.

---

## 🔍 Dangerous Imports

| Import | Purpose | Risk |
|:---|:---|:---|
| **`MmMapIoSpace`** | Maps physical memory into kernel VA | 🔴 Arbitrary physical memory access |
| **`MmUnmapIoSpace`** | Cleans up mappings | 🟡 Used with the above |
| **`HalGetBusDataByOffset`** | Read PCI bus data | 🔴 PCI config space access |
| **`HalSetBusDataByOffset`** | Write PCI bus data | 🔴 PCI config space modification |
| **`READ_PORT_UCHAR`, `READ_PORT_USHORT`, `READ_PORT_ULONG`** | Read I/O ports | 🔴 Direct hardware I/O |
| **`WRITE_PORT_UCHAR`, `WRITE_PORT_USHORT`, `WRITE_PORT_ULONG`** | Write I/O ports | 🔴 Direct hardware I/O |
| **`MmIsAddressValid`** | Check kernel address validity | 🟡 Used for safe memory access |
| **`IoGetDeviceObjectPointer`** | Get device object by name | 🟡 Can reference other drivers |
| **`IoBuildDeviceIoControlRequest`** | Build IOCTL request | 🟡 Can send IOCTLs to other drivers |
| **`KeStallExecutionProcessor`** | Busy-wait | 🟢 Standard hardware driver |
| **`KeBugCheckEx`** | Triggers BSOD | 🟢 Error handling |

**The critical combination:** `MmMapIoSpace` + `HalGetBusDataByOffset` / `HalSetBusDataByOffset` + `READ_PORT_*` / `WRITE_PORT_*`. This is the **most hardware-capable import profile** observed in any driver so far. It provides:
- Arbitrary physical memory mapping
- PCI configuration space read/write
- Direct I/O port read/write

Together, these allow:
- Full control over PCI devices (redirect BARs, enable bus mastering)
- DMA attacks on any device
- Manipulation of chipset registers
- Potential bypass of IOMMU/VT-d in weakly configured systems

**No privilege-checking APIs** are imported. Combined with the friendly symbolic link, this driver may be accessible to any user who can open the device (subject to the device's DACL, which we don't know).

---

## 📊 Standalone Summary

| Aspect | Assessment |
|:---|:---|
| **Validity** | ✅ Signed, kernel-mode driver |
| **Architecture** | **❌ 32-bit only (`I386`)** — will not load on 64-bit Windows |
| **Attack surface** | 58 functions, 28 IOCTLs |
| **Primary primitive** | 5× `MmMapIoSpace` user-controlled (arbitrary physical memory mapping) |
| **Secondary primitives** | 29× direct I/O port access with user-controlled port; PCI config space read/write |
| **Device creation** | ✅ On load (no init sequence required) |
| **Device security** | Friendly symbolic link, no privilege checks |
| **Blocklist status** | **Very likely present** (CPU-Z driver is well-known in BYOVD) |
| **Overall verdict** | Powerful primitives but crippled by 32-bit architecture; likely blocked |

**The 32-bit limitation is fatal for modern BYOVD attacks.** On any 64-bit Windows system (Windows 7 x64 and later), this driver cannot be loaded. It is only useful on legacy 32-bit systems, which are rare and largely obsolete. Even if unblocked, its practical applicability is minimal.

---

## 🛠️ Next Steps for cpuz.sys

### 1. Verify Against the Microsoft Blocklist
Search `Microsoft_Blocked_Driver_List.pdf` for:
```
8c95d28270a4a314299cf50f05dcbe63033b2a555195d2ad2f678e09e00393e6
```
Given that this is a CPU-Z driver, it is **almost certainly blocked**.

### 2. Consider the Architecture Limitation
If your target is a 64-bit Windows system (as any modern EDR test would be), **discard this driver immediately**. It cannot load. Keep it only as a historical case study for your research paper.

### 3. If You Have a 32-bit Windows VM
If you have a 32-bit Windows VM (e.g., Windows 7 x86), you could test it. But this is unlikely to be relevant to your hackathon project.

### 4. Add to Your JOCKY Manifest (Conditional)
Given the 32-bit limitation, this driver should be marked as **not recommended**:

```yaml
name: "cpuz.sys"
sha256: "8c95d28270a4a314299cf50f05dcbe63033b2a555195d2ad2f678e09e00393e6"
device_path: "\\\\.\\CPUZ135"
device_on_load: true
requires_admin: false
architecture: "I386"  # 32-bit only
capabilities: [arb_physical_read, arb_physical_write, pci_config_rw, port_io]
priority: 99  # Do not use — 32-bit only
```

---

## 🧩 Comparison: All Drivers Analyzed So Far

| Feature | Driver A (`65329dad`) | Driver B (`LHA.sys`) | Driver C (`procexp1627.sys`) | Driver D (`mtcBSv64.sys`) | Driver E (`cpuz141.sys`) | Driver F (`zam64.sys`) | Driver G (`cpuz.sys`) |
|:---|:---|:---|:---|:---|:---|:---|:---|
| **Size** | 65 KB | 35 KB | 36 KB | 34 KB | 46 KB | 200 KB | 22 KB |
| **Arch** | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 | AMD64 | **I386 (32-bit)** |
| **Functions** | 155 | 116 | 118 | 97 | 48 | 348 | 58 |
| **IOCTLs** | 57 | 28 | Not listed | Not listed | 38 | 29 | 28 |
| **Total HIGH findings** | 38 | 14 | 11 | 9 | 7 | 6 | 5 |
| **Total MED findings** | 0 | 0 | 0 | 0 | 0 | 0 | **29** |
| **`MmMapIoSpace` findings** | 21 | 13 | 0 | 3 | 7 | 0 | 5 |
| **`RtlCopyMemory` findings** | 17 | 1 | 8 | 6 | 0 | 6 | 0 |
| **Port I/O findings** | 0 | 0 | 0 | 0 | 0 | 0 | 29 |
| **Process manipulation findings** | 0 | 0 | 3 | 0 | 0 | 0 (imports present) | 0 |
| **Primary primitive** | Arbitrary physical memory mapping | Arbitrary physical memory mapping | Cross-process memory access | Arbitrary physical memory mapping | Arbitrary physical memory mapping + PCI config | Process termination, cross-process memory, file/registry | Arbitrary physical memory + port I/O |
| **Device on load** | ✅ True | ❌ False | ❌ False | ❌ False | ✅ True | ✅ True | ✅ True |
| **Privilege checks** | Unknown | Unknown | ✅ Present | Not imported | Not imported | Not imported | Not imported |
| **Blocklist risk** | Low (verify) | Low (verify) | **Very high** | **Very high** | **Very high** | **Very high** | **Very high** |
| **Priority score** | 3.0 | 3.0 | 2.0 | 3.0 | 6.0 | 2.0 | 6.0 |
| **Overall strength** | ★★★★★ | ★★★★☆ | ★★★★☆ (if unblocked) | ★★★★☆ (if unblocked) | ★★★★★ (if unblocked, 64-bit) | ★★★★☆ (if unblocked) | ★★☆☆☆ (32-bit only) |
| **Recommended priority** | 1 | 2 | 3 | 4 | 1 or 2 (if unblocked) | 3 or 4 (if unblocked) | **Do not use** |

**Interpretation:**
- **cpuz.sys** has the same priority score as cpuz141.sys (6.0) due to the rich port I/O findings, but its **32-bit architecture makes it effectively useless** for modern 64-bit Windows targets. It should be excluded from your active arsenal and retained only as a case study.
- **Driver A (`65329dad`)** remains the strongest all-round candidate.
- **Driver B (`LHA.sys`)** is a solid fallback.
- **Driver E (`cpuz141.sys`)** is the strongest 64-bit alternative if unblocked.

---


---

## ✅ Bottom Line

**cpuz.sys is a 32-bit driver with rich hardware access primitives but is unusable on modern 64-bit Windows systems.** It is also almost certainly on Microsoft's blocklist. **Discard it from your active arsenal.** Keep it only as a case study for your research paper on the evolution of CPU-Z driver vulnerabilities.

**Your strongest candidates remain Driver A (`65329dad`), Driver B (`LHA.sys`), and cpuz141.sys (if unblocked).** Continue running DeepZero across your corpus — you are building a comprehensive, resilient BYOVD arsenal.