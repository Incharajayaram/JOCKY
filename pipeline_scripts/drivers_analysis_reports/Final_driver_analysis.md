# Final Analysis: Blocklist Check and Priority Order

After checking each driver’s SHA‑256 hash against the provided **Microsoft Blocked Driver List (version 10.0.29545.0)**, **none of the analysed drivers appear in the blocklist**. This means that, according to this policy, all of them are currently allowed to load on systems with the blocklist enforced. However, the blocklist is periodically updated, so this status may change.

Below is the final priority order, combining **DeepZero priority scores**, **primitive strength**, **ease of exploitation** (device‑on‑load, known symbolic link, absence of privilege checks), and **redundancy**.

---

## Priority Order (1 = highest)

| Rank | Driver (short name) | SHA‑256 (first 16) | DeepZero Priority | Key Primitives | Device on Load | Symlink Known? | Blocklist? | Notes |
|------|---------------------|-------------------|-------------------|----------------|----------------|----------------|------------|-------|
| 1 | **cpuz141** | `ded2927f9a4e64ee` | 6.0 | 7× MmMapIoSpace, PCI config | ✅ | ✅ (GUID) | ❌ Not found | High redundancy, no privilege checks. |
| 2 | **rtkiow8x64** | `082c39fe2e321700` | 6.0 | 3× MmMapIoSpace, MDL | ✅ | ✅ (`\DosDevices\rtkio`) | ❌ Not found | Friendly name, easy to access. |
| 3 | **NTIOLib** | `d8b58f6a89a76185` | 6.0 | 2× MmMapIoSpace, PCI config | ✅ | ✅ (`\DosDevices\NTIOLib_1_0_6`) | ❌ Not found | Tiny (12 KB), simple, focused. |
| 4 | **0eab16c7** | `0eab16c7f54b6162` | 6.0 | 2× MmMapIoSpace, 1× RtlCopyMemory, PCI config | ✅ | ❌ (blank) | ❌ Not found | Device name must be resolved. |
| 5 | **b205835b** | `b205835b818d8a50` | 6.0 | 3× MmMapIoSpace, MDL | ❌ | ✅ (`\DosDevices\rtkio`) | ❌ Not found | Requires init sequence. |
| 6 | **65329dad** (Driver A) | `65329dad28e92f4b` | 3.0 | 21× MmMapIoSpace, 17× RtlCopyMemory | ✅ | ✅ (GUID) | ❌ Not found | Massive redundancy, very high reliability. |
| 7 | **LHA.sys** | `e75714f8e0ff4560` | 3.0 | 13× MmMapIoSpace | ❌ | ✅ (GUID) | ❌ Not found | Requires init; strong primitives. |
| 8 | **CorsairLLAccess64** | `000547560fea0dd4` | 8.0 | 2× MmMapIoSpace, PCI config, MDL | ✅ | ❌ (blank) | ❌ Not found | Device name must be resolved. |
| 9 | **AMDRyzenMaster** | `a13054f349b7baa8` | 8.0 | 4× MmMapIoSpace, PCI config, MDL | ❌ | ✅ (`\DosDevices\AMDRyzenMasterDriverV13`) | ❌ Not found | Requires init; known name. |
| 10 | **ene.sys** | `175eed7a4c6de9c3` | 7.0 | 2× ZwMapViewOfSection (\Device\PhysicalMemory) | ✅ | ✅ (`\DosDevices\EneTechIo`) | ❌ Not found | Direct physical memory mapping. |
| 11 | **kEvP64** | `1aaa9aef39cb3c0a` | 7.0 | 2× process attach, 1× MmMapIoSpace, process kill | ✅ | ✅ (`\DosDevices\kEvP64`) | ❌ Not found | Only 2 IOCTLs (multiplexer). |
| 12 | **LenovoDiagnosticsDriver** | `f05b1ee9e2f6ab70` | 8.0 | 2× MmMapIoSpace, PCI config | ❌ | ✅ (`\DosDevices\LenovoDiagnosticsDriver`) | ❌ Not found | Possible restrictive DACL. |
| 13 | **AMDPowerProfiler** | `0af5ccb3d33a9ba9` | 6.0 | 1× MmMapIoSpace, 1× ZwMapViewOfSection | ❌ | ❌ (blank) | ❌ Not found | Dependency on `AMDPCore.SYS`. |
| 14 | **ATSZIO** | `01e024cb14b34b6d` | 5.0 | 1× ZwMapViewOfSection, PCI config | ✅ | ❌ (blank) | ❌ Not found | Single mapping primitive. |
| 15 | **fb0dbc3b** | `fb0dbc3b9c897b75` | 4.0 | 2× MmMapIoSpace, MDL, firmware | ✅ | ✅ (`\DosDevices\BioNT_BS`) | ❌ Not found | Solid mid‑tier. |
| 16 | **nvflsh64** | `a899b659b08fbae3` | 4.0 | 1× ZwMapViewOfSection | ✅ | ✅ (`\DosDevices\NVFLASH`) | ❌ Not found | Single primitive but easy. |
| 17 | **speedfan** | `22be050955347661` | 4.0 | 2× MmMapIoSpace | ✅ | ✅ (`\DosDevices\Global\SPEEDFAN`) | ❌ Not found | Global symlink. |
| 18 | **4ed2d2c1** | `4ed2d2c1b00e87b9` | 3.0 | 3× MmMapIoSpace, MDL | ❌ | ✅ (`\DosDevices\rtkio`) | ❌ Not found | Requires init. |
| 19 | **rtkio64** | `7133a461aeb03b4d` | 3.0 | 3× MmMapIoSpace, MDL | ✅ | ✅ (`\DosDevices\rtkio`) | ❌ Not found | Good but older. |
| 20 | **PhlashNT** | `65db1b259e305a52` | 3.0 | 1× MmMapIoSpace | ✅ | ✅ (`\DosDevices\WinPhlash`) | ❌ Not found | Single primitive. |
| 21 | **BSMI** | `59626cac380d8fe0` | 3.0 | 2× MmMapIoSpace | ✅ | ✅ (`\DosDevices\BSMI`) | ❌ Not found | Very simple. |
| 22 | **BSMIx64** | `552f70374715e70c` | 3.0 | 2× MmMapIoSpace | ✅ | ✅ (`\DosDevices\BSMI`) | ❌ Not found | Duplicate of BSMI. |
| 23 | **mtcBSv64** | `c9cf1d627078f63a` | 3.0 | 3× MmMapIoSpace | ❌ | ✅ (`\DosDevices\MTC0303`) | ❌ Not found | Requires init. |
| 24 | **HpPortIox64** | `c5050a2017490fff` | 2.0 | Buffer overflows, PCI config | ✅ | ✅ (`\DosDevices\HpPortIO`) | ❌ Not found | No MmMapIoSpace. |
| 25 | **zam64** | `543991ca8d1c6511` | 2.0 | Process kill, file/reg | ✅ | ✅ (GUID) | ❌ Not found | Good for process termination. |
| 26 | **BdApiUtil** | `32198295d2a2700b` | 2.0 | Process kill, file intercept | ✅ | ✅ (`\DosDevices\X:`) | ❌ Not found | Unusual device masquerade. |
| 27 | **procexp1627** | `9b6a84f7c40ea51c` | 2.0 | Process attach | ❌ | ✅ (`\DosDevices\PROCEXP152`) | ❌ Not found | Requires init. |
| 28 | **libnicm** | `95d50c69cdbf10c9` | 0.0 | RtlCopyMemory, ZwLoadDriver | ❌ | ✅ (`\DosDevices\NICM`) | ❌ Not found | Low priority, needs manual review. |
| — | **cpuz.sys** | `8c95d28270a4a314` | 6.0 | 5× MmMapIoSpace, port I/O | ✅ | ✅ (`\DosDevices\CPUZ135`) | ❌ Not found | **32‑bit (I386) – unusable on 64‑bit Windows.** |
| — | **rtkio.sys** | `478917514be37b32` | 3.0 | 1× MmMapIoSpace, 28× port I/O | ✅ | ✅ (`\DosDevices\rtkio`) | ❌ Not found | **32‑bit (I386) – unusable on 64‑bit Windows.** |

---

## Key Findings

- **None of the drivers are on the provided blocklist.** This means that, if the blocklist is not enforced (or is outdated), all of them are viable candidates. However, many are well‑known LOLDrivers and could be added in future updates.
- **The strongest overall candidates** are those with multiple `MmMapIoSpace` primitives, PCI configuration access, and easy access (device‑on‑load, known symbolic link). Drivers E (`cpuz141`), P (`rtkiow8x64`), NTIOLib, J (`0eab16c7`), and L (`b205835b`) are top picks.
- **Driver A (`65329dad`)** and **Driver B (`LHA.sys`)** are extremely strong due to their many redundant primitives, despite lower DeepZero scores. They are excellent fallbacks.
- **Drivers with unknown device names** (T, J, AB, Q) require additional reverse engineering before use, but may be worth the effort.
- **32‑bit drivers** (`cpuz.sys`, `rtkio.sys`) should be discarded for modern 64‑bit targets.
- **Drivers with deferred device creation** (I, X, L, K, D, etc.) need an initialisation step, which adds complexity but is manageable.
- **Drivers with possible DACL restrictions** (X, T) may require admin privileges to open.

This priority order provides a clear roadmap for building a resilient BYOVD arsenal for the JOCKY framework.