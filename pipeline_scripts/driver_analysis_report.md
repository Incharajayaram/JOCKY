# Driver Static Analysis Report
Generated: 2026-09-25T11:31:07.249564+00:00
Total samples: 53

## Summary by Risk Score

| Rank | File | Arch | Signed | Dangerous Imports | Score | Warnings |
|------|------|------|--------|-------------------|-------|----------|
| 1 | TmComm.sys | x64 | Yes | 18 | 36 | - |
| 2 | aswArPot.sys | x64 | Yes | 18 | 36 | - |
| 3 | viragt.sys | x86 | Yes | 16 | 34 | RWX section: INIT |
| 4 | 1cd219f58b249a2e4f86553bdd649c73785093e22c87170798dae90f193240af | x64 | Yes | 15 | 32 | RWX section: INIT |
| 5 | zam64.sys | x64 | Yes | 14 | 32 | RWX section: .hook; RWX section: INIT |
| 6 | TfSysMon.sys | x64 | Yes | 14 | 30 | RWX section: INIT |
| 7 | cpuz.sys | x86 | Yes | 14 | 30 | RWX section: INIT |
| 8 | rtkio.sys | x86 | Yes | 14 | 30 | RWX section: INIT |
| 9 | ZYArKit.sys | x64 | Yes | 13 | 28 | RWX section: INIT |
| 10 | iQVW64.SYS | x64 | Yes | 13 | 28 | RWX section: INIT |
| 11 | kEvP64.sys | x64 | Yes | 12 | 26 | RWX section: INIT |
| 12 | segwindrvx64.sys | x64 | Yes | 12 | 26 | RWX section: INIT |
| 13 | vboxdrv.sys | x64 | Yes | 12 | 26 | RWX section: INIT |
| 14 | 4ed2d2c1b00e87b926fb58b4ea43d2db35e5912975f4400aa7bd9f8c239d08b7.sys | x64 | Yes | 11 | 24 | RWX section: INIT |
| 15 | AMDPowerProfiler.sys | x64 | Yes | 12 | 24 | - |
| 16 | BdApiUtil.sys | x64 | Yes | 11 | 24 | RWX section: INIT |
| 17 | LHA.sys | x64 | Yes | 11 | 24 | RWX section: INIT |
| 18 | RootLaser.sys | x64 | Yes | 12 | 24 | - |
| 19 | AMDRyzenMasterDriver.sys | x64 | Yes | 11 | 22 | - |
| 20 | HwOs2Ec7x64.sys | x64 | Yes | 11 | 22 | - |
| 21 | ProcessCtr.sys | x64 | Yes | 11 | 22 | - |
| 22 | Truesight | x64 | Yes | 11 | 22 | - |
| 23 | UCOREW64.SYS | x64 | Yes | 10 | 22 | RWX section: INIT |
| 24 | b205835b818d8a50903cf76936fcf8160060762725bd74a523320cfbd091c038.sys | x64 | Yes | 11 | 22 | - |
| 25 | procexp1627.sys | x64 | Yes | 11 | 22 | - |
| 26 | viraglt64.sys | x64 | Yes | 10 | 22 | RWX section: INIT |
| 27 | 0eab16c7f54b61620277977f8c332737081a46bc6bbde50742b6904bdd54f502.sys | x64 | Yes | 9 | 20 | RWX section: INIT |
| 28 | LenovoDiagnosticsDriver.sys | x64 | Yes | 10 | 20 | - |
| 29 | NCHGBIOS2x64.SYS | x64 | Yes | 9 | 20 | RWX section: INIT |
| 30 | rtkio64.sys | x64 | Yes | 9 | 20 | RWX section: INIT |
| 31 | ATSZIO.sys | x64 | Yes | 8 | 18 | RWX section: INIT |
| 32 | PhlashNT.sys | x64 | Yes | 8 | 18 | RWX section: INIT |
| 33 | b16e217cdca19e00c1b68bdfb28ead53b20adeabd6edcd91542f9fbf48942877 | x64 | Yes | 8 | 18 | RWX section: INIT |
| 34 | libnicm.sys | x64 | Yes | 8 | 18 | RWX section: INIT |
| 35 | mtcBSv64.sys | x64 | Yes | 8 | 18 | RWX section: INIT |
| 36 | rtkiow10x64.sys | x64 | Yes | 9 | 18 | - |
| 37 | rtkiow8x64.sys | x64 | Yes | 9 | 18 | - |
| 38 | BSMEMx64.sys | x64 | Yes | 7 | 16 | RWX section: INIT |
| 39 | BSMI.sys | x64 | Yes | 7 | 16 | RWX section: INIT |
| 40 | BSMIx64.sys | x64 | Yes | 7 | 16 | RWX section: INIT |
| 41 | HpPortIox64.sys | x64 | Yes | 7 | 16 | RWX section: INIT |
| 42 | cpuz141.sys | x64 | Yes | 7 | 16 | RWX section: INIT |
| 43 | elbycdio.sys | x86 | Yes | 7 | 16 | RWX section: INIT |
| 44 | BS_I2c64.sys | x64 | Yes | 6 | 14 | RWX section: INIT |
| 45 | CorsairLLAccess64.sys | x64 | Yes | 7 | 14 | - |
| 46 | NTIOLib.sys | x64 | Yes | 6 | 14 | RWX section: INIT |
| 47 | ProcessMonitorDriver.sys | x64 | Yes | 7 | 14 | - |
| 48 | ene.sys | x64 | Yes | 7 | 14 | - |
| 49 | fb0dbc3b9c897b7571b94fb2203ffb1ac0facfe366b2cb1f91904ea5335018f0.sys | x64 | Yes | 7 | 14 | - |
| 50 | nvflsh64.sys | x64 | Yes | 6 | 14 | RWX section: INIT |
| 51 | speedfan.sys | x64 | Yes | 6 | 14 | RWX section: INIT |
| 52 | 206f27ae820783b7755bca89f83a0fe096dbb510018dd65b63fc80bd20c03261 | x64 | Yes | 6 | 12 | - |
| 53 | vmdrv.sys | x64 | Yes | 6 | 12 | - |

---

## TmComm.sys

- **SHA256:** `cc687fe3741bbde1dd142eac0ef59fd1d4457daee43cdde23bb162ef28d04e64`
- **Size:** 454,264 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2018-04-09T10:08:57+00:00
- **Imphash:** `N/A`
- **Score:** 36

**Dangerous Imports:**
- `MmMapLockedPagesSpecifyCache` from `ntoskrnl.exe`
- `ZwMapViewOfSection` from `ntoskrnl.exe`
- `KeStackAttachProcess` from `ntoskrnl.exe`
- `ZwAllocateVirtualMemory` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `PsGetCurrentProcessId` from `ntoskrnl.exe`
- `PsLookupProcessByProcessId` from `ntoskrnl.exe`
- `MmGetSystemRoutineAddress` from `ntoskrnl.exe`
- `ZwCreateKey` from `ntoskrnl.exe`
- `NtOpenProcess` from `ntoskrnl.exe`
- `ZwDeleteKey` from `ntoskrnl.exe`
- `ZwSetValueKey` from `ntoskrnl.exe`
- `ZwOpenProcess` from `ntoskrnl.exe`
- `ExAllocatePool` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 341,573 | 342,016 | 5.88 | R | - | X |
| .rdata | 0x55000 | 24,948 | 25,088 | 3.44 | R | - | - |
| .data | 0x5c000 | 13,308 | 5,632 | 4.68 | R | W | - |
| .pdata | 0x60000 | 15,072 | 15,360 | 5.52 | R | - | - |
| .gfids | 0x64000 | 4 | 512 | 0.06 | R | - | - |
| PAGE | 0x65000 | 6,884 | 7,168 | 6.18 | R | - | X |
| .edata | 0x67000 | 22,580 | 23,040 | 5.8 | R | - | - |
| INIT | 0x6d000 | 6,288 | 6,656 | 5.17 | R | - | X |
| .rsrc | 0x6f000 | 1,384 | 1,536 | 3.12 | R | - | - |
| .reloc | 0x70000 | 1,224 | 1,536 | 4.86 | R | - | - |

**Exports (500):**
- `??0CAutoUpdateConfigThread@@QEAA@AEBV0@@Z`
- `??0CAutoUpdateConfigThread@@QEAA@PEAU_UNICODE_STRING@@P6AX0PEAX@Z1@Z`
- `??0CBlobConfig@@QEAA@AEBV0@@Z`
- `??0CBlobConfig@@QEAA@K@Z`
- `??0CContext@@QEAA@AEBV0@@Z`
- `??0CContext@@QEAA@KP6AJPEAU_EVENT_REPORT@@PEAXPEAU_TMCE_REPORT@@PEAU_TMCE_FEEDBACK@@@Z1K@Z`
- `??0CContextList@@QEAA@AEBV0@@Z`
- `??0CContextList@@QEAA@KPEAVIMemoryAllocator@@@Z`
- `??0CDebugLog@@QEAA@AEBV0@@Z`
- `??0CDebugLog@@QEAA@PEBG@Z`
- ... and 490 more

**Interesting Strings:**
- `|$XReadu`
- `_DirectKmCallUmCommPort(): Client port not initialized.`
- `_DirectKmCallUmCommPort(): CommPort not ready.`
- `source\CommPortKm.cpp`
- `_DirectKmCallUmCommPort(): No memory, silent event.`
- `_DirectKmCallUmCommPort: pShareMemBlk is NULL!`
- `_DirectKmCallUmCommPort: ObOpenObjectByPointer() failed. status: 0x%x`
- `_DirectKmCallUmCommPort: ObOpenObjectByPointer() done, but invalid handle value.`
- `_DirectKmCallUmCommPort: ZwAllocateVirtualMemory() failed. status: 0x%x`
- `_DirectKmCallUmCommPort: ZwAllocateVirtualMemory() succeeded, but size of returned memory too small.`

**Entry Point Disassembly:**
```asm
0x18006d000: mov qword ptr [rsp + 8], rbx
0x18006d005: push rdi
0x18006d006: sub rsp, 0x20
0x18006d00a: mov rbx, rdx
0x18006d00d: mov rdi, rcx
0x18006d010: call 0x18006d02c
0x18006d015: mov rdx, rbx
0x18006d018: mov rcx, rdi
```

---

## aswArPot.sys

- **SHA256:** `4b5229b3250c8c08b98cb710d6c056144271de099a57ae09f5d2097fc41bd4f1`
- **Size:** 208,024 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2021-02-01T09:08:43+00:00
- **Imphash:** `N/A`
- **Score:** 36

**Dangerous Imports:**
- `ExAllocatePool` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `KeStackAttachProcess` from `ntoskrnl.exe`
- `PsLookupProcessByProcessId` from `ntoskrnl.exe`
- `PsGetCurrentProcessId` from `ntoskrnl.exe`
- `ZwMapViewOfSection` from `ntoskrnl.exe`
- `MmGetSystemRoutineAddress` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `MmMapLockedPagesSpecifyCache` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `ZwOpenProcess` from `ntoskrnl.exe`
- `MmMapIoSpace` from `ntoskrnl.exe`
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `KeAttachProcess` from `ntoskrnl.exe`
- `ZwSetValueKey` from `ntoskrnl.exe`
- `ZwCreateKey` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 141,506 | 141,824 | 6.38 | R | - | X |
| .rdata | 0x24000 | 15,268 | 15,360 | 5.71 | R | - | - |
| .data | 0x28000 | 154,320 | 2,560 | 2.83 | R | W | - |
| .pdata | 0x4e000 | 4,632 | 5,120 | 5.04 | R | - | - |
| PAGE | 0x50000 | 7,243 | 7,680 | 6.11 | R | - | X |
| INIT | 0x52000 | 5,084 | 5,120 | 5.34 | R | - | X |
| .rsrc | 0x54000 | 920 | 1,024 | 3.02 | R | - | - |
| .reloc | 0x55000 | 416 | 512 | 4.96 | R | - | - |

**Interesting Strings:**
- `THREADIDM`
- `EXDEVICEH`
- `PEREGKEYH`
- `ZwReadVirtualMemory`
- `ZwWriteVirtualMemory`
- `PsGetCurrentProcess`
- `NtWriteVirtualMemory`
- `PsGetThreadWin32Thread`
- `PsGetThreadTeb`
- `PsGetProcessPeb`

**Entry Point Disassembly:**
```asm
0x140052000: mov qword ptr [rsp + 8], rbx
0x140052005: push rdi
0x140052006: sub rsp, 0x20
0x14005200a: mov rbx, rdx
0x14005200d: mov rdi, rcx
0x140052010: call 0x14005202c
0x140052015: mov rdx, rbx
0x140052018: mov rcx, rdi
```

---

## viragt.sys

- **SHA256:** `e05eeb2b8c18ad2cb2d1038c043d770a0d51b96b748bc34be3e7fc6f3790ce53`
- **Size:** 91,952 bytes
- **Arch:** x86
- **Signed:** Yes
- **Timestamp:** 2013-01-23T08:38:45+00:00
- **Imphash:** `N/A`
- **Score:** 34

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `ZwSetValueKey` from `ntoskrnl.exe`
- `ZwDeleteKey` from `ntoskrnl.exe`
- `ZwOpenProcess` from `ntoskrnl.exe`
- `MmMapLockedPagesSpecifyCache` from `ntoskrnl.exe`
- `PsLookupProcessByProcessId` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `memcpy` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `ZwCreateKey` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `READ_PORT_ULONG` from `HAL.dll`
- `WRITE_PORT_UCHAR` from `HAL.dll`
- `READ_PORT_UCHAR` from `HAL.dll`
- `READ_PORT_BUFFER_UCHAR` from `HAL.dll`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x480 | 56,245 | 56,320 | 6.74 | R | - | X |
| NonPaged | 0xe080 | 1,965 | 2,048 | 6.58 | R | - | X |
| .rdata | 0xe880 | 1,492 | 1,536 | 5.0 | R | - | - |
| .data | 0xee80 | 13,900 | 13,952 | 0.05 | R | W | - |
| INIT | 0x12500 | 2,692 | 2,816 | 5.53 | R | W | X |
| .rsrc | 0x13000 | 1,072 | 1,152 | 3.14 | R | - | - |
| .reloc | 0x13480 | 3,928 | 3,968 | 6.24 | R | - | - |

**Interesting Strings:**
- `\Registry\Machine\`
- `\Registry\User\`
- `\Registry\Machine\SOFTWARE\Classes\`
- `Processo: `
- `SOFTWARE\Microsoft\Command Processor`
- `Debugger`
- `\Device\Harddisk0\DR0`
- `AntiTDL::ReadDiskSector - The IRQL is too high to process this request.`
- `AntiTdl!HandleRemoveTDL3IoCtl - Non sono riuscito ad eliminare i settori del TDL3 alla fine del volume.`
- `AntiTdl!HandleRemoveTDL3IoCtl - GetVolumePhysDisk has failed.`

**Entry Point Disassembly:**
```asm
0x22505: mov edi, edi
0x22507: push ebp
0x22508: mov ebp, esp
0x2250a: mov eax, dword ptr [0x1eed4]
0x2250f: test eax, eax
0x22511: mov ecx, 0xbb40e64e
0x22516: je 0x2251c
0x22518: cmp eax, ecx
```

---

## 1cd219f58b249a2e4f86553bdd649c73785093e22c87170798dae90f193240af

- **SHA256:** `1cd219f58b249a2e4f86553bdd649c73785093e22c87170798dae90f193240af`
- **Size:** 81,584 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2017-12-06T11:12:00+00:00
- **Imphash:** `N/A`
- **Score:** 32

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `MmGetSystemRoutineAddress` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `ZwDeleteKey` from `ntoskrnl.exe`
- `ExAllocatePool` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `ZwCreateKey` from `ntoskrnl.exe`
- `ZwSetValueKey` from `ntoskrnl.exe`
- `ZwOpenProcess` from `ntoskrnl.exe`
- `PsGetCurrentProcessId` from `ntoskrnl.exe`
- `KeStackAttachProcess` from `ntoskrnl.exe`
- `PsLookupProcessByProcessId` from `ntoskrnl.exe`
- `ZwMapViewOfSection` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 52,932 | 53,248 | 6.13 | R | - | X |
| .data | 0xe000 | 1,712 | 512 | 0.59 | R | W | - |
| .pdata | 0xf000 | 2,412 | 2,560 | 4.34 | R | - | - |
| PAGE | 0x10000 | 152 | 512 | 2.38 | R | - | X |
| INIT | 0x11000 | 3,720 | 4,096 | 5.38 | R | W | X |
| .rsrc | 0x12000 | 944 | 1,024 | 3.16 | R | - | - |
| .reloc | 0x13000 | 84 | 512 | 0.27 | R | - | - |

**Interesting Strings:**
- `NtEnumerateKey`
- `NtEnumerateValueKey`
- `NtQueryValueKey`
- `NtDeleteValueKey`
- `NtDeleteKey`
- `NtSetValueKey`
- `IoDeleteDevice`
- `IoCreateDevice`
- `SeTokenIsAdmin`
- `ZwDeleteKey`

**Entry Point Disassembly:**
```asm
0x21000: mov qword ptr [rsp + 0x10], rbx
0x21005: push rdi
0x21006: sub rsp, 0x60
0x2100a: mov rdi, rcx
0x2100d: mov ebx, 0xc0000001
0x21012: mov qword ptr [rip - 0x2df1], rcx
0x21019: call 0x188b0
0x2101e: mov r11d, dword ptr [rip - 0x2e19]
```

---

## zam64.sys

- **SHA256:** `543991ca8d1c65113dff039b85ae3f9a87f503daec30f46929fd454bc57e5a91`
- **Size:** 203,680 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2016-08-17T17:06:53+00:00
- **Imphash:** `N/A`
- **Score:** 32

**Warnings:**
- RWX section: .hook
- RWX section: INIT

**Dangerous Imports:**
- `ZwSetValueKey` from `ntoskrnl.exe`
- `MmGetSystemRoutineAddress` from `ntoskrnl.exe`
- `NtOpenProcess` from `ntoskrnl.exe`
- `ZwDeleteKey` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `MmMapLockedPagesSpecifyCache` from `ntoskrnl.exe`
- `KeStackAttachProcess` from `ntoskrnl.exe`
- `PsLookupProcessByProcessId` from `ntoskrnl.exe`
- `ZwOpenProcess` from `ntoskrnl.exe`
- `PsGetCurrentProcessId` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 137,141 | 137,216 | 6.32 | R | - | X |
| .hook | 0x23000 | 2,479 | 2,560 | 5.04 | R | W | X |
| .rdata | 0x24000 | 18,244 | 18,432 | 5.49 | R | - | - |
| .data | 0x29000 | 343,176 | 26,624 | 4.89 | R | W | - |
| .pdata | 0x7d000 | 2,256 | 2,560 | 4.69 | R | - | - |
| INIT | 0x7e000 | 4,358 | 4,608 | 5.15 | R | W | X |
| .rsrc | 0x80000 | 616 | 1,024 | 2.07 | R | - | - |
| .reloc | 0x81000 | 96 | 512 | 1.21 | R | - | - |

**Interesting Strings:**
- `Not enough memory`
- `HlpGetProcessImagePath`
- `NtOpenProcess failed!`
- `ZwQueryInformationProcess failed!`
- `Can not allocate memory for buffer`
- `ZwQueryInformationProcess failed 2nd time`
- `Current process %s`
- `HlpPrintCurrentProcessName`
- `Can not open process id %d`
- `HlpIsCriticalSystemProcess`

**Entry Point Disassembly:**
```asm
0x14007e000: mov qword ptr [rsp + 8], rbx
0x14007e005: push rdi
0x14007e006: sub rsp, 0x20
0x14007e00a: mov rbx, rdx
0x14007e00d: mov rdi, rcx
0x14007e010: call 0x14007e02c
0x14007e015: mov rdx, rbx
0x14007e018: mov rcx, rdi
```

---

## TfSysMon.sys

- **SHA256:** `1c1a4ca2cbac9fe5954763a20aeb82da9b10d028824f42fff071503dcbe15856`
- **Size:** 60,416 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2010-02-01T19:16:19+00:00
- **Imphash:** `N/A`
- **Score:** 30

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `PsLookupProcessByProcessId` from `ntoskrnl.exe`
- `MmGetSystemRoutineAddress` from `ntoskrnl.exe`
- `ExAllocatePool` from `ntoskrnl.exe`
- `PsGetCurrentProcessId` from `ntoskrnl.exe`
- `KeStackAttachProcess` from `ntoskrnl.exe`
- `ZwAllocateVirtualMemory` from `ntoskrnl.exe`
- `ZwOpenProcess` from `ntoskrnl.exe`
- `ZwSetValueKey` from `ntoskrnl.exe`
- `ZwDeleteKey` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 41,144 | 41,472 | 6.3 | R | - | X |
| .rdata | 0xc000 | 3,232 | 3,584 | 4.94 | R | - | - |
| .data | 0xd000 | 2,016 | 512 | 0.62 | R | W | - |
| .pdata | 0xe000 | 1,152 | 1,536 | 3.53 | R | - | - |
| INIT | 0xf000 | 3,268 | 3,584 | 4.83 | R | W | X |
| .rsrc | 0x10000 | 1,016 | 1,024 | 3.27 | R | - | - |
| .reloc | 0x11000 | 48 | 512 | 0.19 | R | - | - |

**Interesting Strings:**
- `NtProtectVirtualMemory`
- `ZwAllocateVirtualMemory`
- `ZwReadVirtualMemory`
- `ZwWriteVirtualMemory`
- `ZwQueryVirtualMemory`
- `ZwProtectVirtualMemory`
- `PsLookupProcessByProcessId`
- `KeUnstackDetachProcess`
- `PsCreateSystemThread`
- `ZwQueryValueKey`

**Entry Point Disassembly:**
```asm
0x1f008: mov rax, qword ptr [rip - 0x1f0f]
0x1f00f: movabs r9, 0x2b992ddfa232
0x1f019: test rax, rax
0x1f01c: je 0x1f023
0x1f01e: cmp rax, r9
0x1f021: jne 0x1f052
0x1f023: lea r8, [rip - 0x1f2a]
0x1f02a: movabs rax, 0xfffff78000000320
```

---

## cpuz.sys

- **SHA256:** `8c95d28270a4a314299cf50f05dcbe63033b2a555195d2ad2f678e09e00393e6`
- **Size:** 21,992 bytes
- **Arch:** x86
- **Signed:** Yes
- **Timestamp:** 2010-11-09T13:32:57+00:00
- **Imphash:** `N/A`
- **Score:** 30

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `IofCompleteRequest` from `ntoskrnl.exe`
- `ExFreePool` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `MmMapIoSpace` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `READ_PORT_USHORT` from `HAL.dll`
- `READ_PORT_ULONG` from `HAL.dll`
- `WRITE_PORT_UCHAR` from `HAL.dll`
- `WRITE_PORT_USHORT` from `HAL.dll`
- `WRITE_PORT_ULONG` from `HAL.dll`
- `READ_PORT_UCHAR` from `HAL.dll`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 9,712 | 9,728 | 6.2 | R | - | X |
| .rdata | 0x4000 | 768 | 1,024 | 3.66 | R | - | - |
| .data | 0x5000 | 480 | 512 | 0.32 | R | W | - |
| INIT | 0x6000 | 1,012 | 1,024 | 5.38 | R | W | X |
| .rsrc | 0x7000 | 848 | 1,024 | 2.93 | R | - | - |
| .reloc | 0x8000 | 588 | 1,024 | 3.69 | R | - | - |

**Interesting Strings:**
- `IoBuildDeviceIoControlRequest`
- `IoDeleteDevice`
- `MmUnmapIoSpace`
- `IoGetDeviceObjectPointer`
- `MmMapIoSpace`
- `IoCreateDevice`
- `KeStallExecutionProcessor`
- `WRITE_PORT_ULONG`
- `WRITE_PORT_USHORT`
- `WRITE_PORT_UCHAR`

**Entry Point Disassembly:**
```asm
0x1603e: mov edi, edi
0x16040: push ebp
0x16041: mov ebp, esp
0x16043: call 0x16005
0x16048: pop ebp
0x16049: jmp 0x12f70
0x1604e: int3 
0x1604f: int3 
```

---

## rtkio.sys

- **SHA256:** `478917514be37b32d5ccf76e4009f6f952f39f5553953544f1b0688befd95e82`
- **Size:** 17,216 bytes
- **Arch:** x86
- **Signed:** Yes
- **Timestamp:** 2010-07-02T12:59:01+00:00
- **Imphash:** `N/A`
- **Score:** 30

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `ExAllocatePool` from `ntoskrnl.exe`
- `MmMapLockedPagesSpecifyCache` from `ntoskrnl.exe`
- `MmMapIoSpace` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `WRITE_PORT_ULONG` from `HAL.dll`
- `READ_PORT_USHORT` from `HAL.dll`
- `READ_PORT_ULONG` from `HAL.dll`
- `READ_PORT_UCHAR` from `HAL.dll`
- `WRITE_PORT_UCHAR` from `HAL.dll`
- `WRITE_PORT_USHORT` from `HAL.dll`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x480 | 1,902 | 1,920 | 5.95 | R | - | X |
| .rdata | 0xc00 | 356 | 384 | 3.56 | R | - | - |
| .data | 0xd80 | 12 | 128 | 0.52 | R | W | - |
| PAGE | 0xe00 | 1,635 | 1,664 | 6.19 | R | - | X |
| INIT | 0x1480 | 1,182 | 1,280 | 5.5 | R | W | X |
| .rsrc | 0x1980 | 1,032 | 1,152 | 3.14 | R | - | - |
| .reloc | 0x1e00 | 308 | 384 | 4.34 | R | - | - |

**Interesting Strings:**
- `ERROR: unrecognized IOCTL %x`
- `KeActiveProcessors=%d`
- `!!!!!!SmiPort=0x%x SmiCommand=0x%x SmiSubCommand=0x%x!!`
- `!!IOCTL_PHYMEM_SENDSMI`
- `Call to MmMapLocked failed due to exception 0x%0x`
- `Couldn't create the device object`
- `IoDeleteDevice`
- `MmUnmapIoSpace`
- `MmUnmapLockedPages`
- `KeSetSystemAffinityThread`

**Entry Point Disassembly:**
```asm
0x1154d: mov edi, edi
0x1154f: push ebp
0x11550: mov ebp, esp
0x11552: mov eax, dword ptr [0x10d80]
0x11557: test eax, eax
0x11559: mov ecx, 0xbb40e64e
0x1155e: je 0x11564
0x11560: cmp eax, ecx
```

---

## ZYArKit.sys

- **SHA256:** `46883bc25c77678f60c1b836f4c438d87158c9af6b229f533522f635a0d5276e`
- **Size:** 93,664 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2017-01-12T05:35:25+00:00
- **Imphash:** `N/A`
- **Score:** 28

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `ZwCreateKey` from `ntoskrnl.exe`
- `ZwSetValueKey` from `ntoskrnl.exe`
- `MmGetSystemRoutineAddress` from `ntoskrnl.exe`
- `ZwOpenProcess` from `ntoskrnl.exe`
- `PsLookupProcessByProcessId` from `ntoskrnl.exe`
- `PsGetCurrentProcessId` from `ntoskrnl.exe`
- `ZwDeleteKey` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `KeStackAttachProcess` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 48,835 | 49,152 | 6.12 | R | - | X |
| .rdata | 0xd000 | 7,228 | 7,680 | 4.34 | R | - | - |
| .data | 0xf000 | 4,880 | 4,096 | 1.88 | R | W | - |
| .pdata | 0x11000 | 1,740 | 2,048 | 4.15 | R | - | - |
| PAGE | 0x12000 | 6,727 | 7,168 | 6.06 | R | - | X |
| INIT | 0x14000 | 4,170 | 4,608 | 4.92 | R | W | X |
| .rsrc | 0x16000 | 736 | 1,024 | 2.47 | R | - | - |
| .reloc | 0x17000 | 330 | 512 | 1.77 | R | - | - |

**Interesting Strings:**
- `NtDeleteValueKey`
- `NtDeleteKey`
- `NtEnumerateValueKey`
- `NtEnumerateKey`
- `NtSetValueKey`
- `NtQueryValueKey`
- `NtOpenKey`
- `NtCreateKey`
- `DeleteKeyValuePassthrough Delete Error : 0x%08x, [KEY = %S], [VALUE = %S]`
- `DeleteKeyValuePassthrough Open Error : 0x%08x, [KEY = %S], [VALUE = %S]`

**Entry Point Disassembly:**
```asm
0x24064: sub rsp, 0x28
0x24068: mov r8, rdx
0x2406b: mov r9, rcx
0x2406e: call 0x24008
0x24073: mov rdx, r8
0x24076: mov rcx, r9
0x24079: add rsp, 0x28
0x2407d: jmp 0x11088
```

---

## iQVW64.SYS

- **SHA256:** `37c637a74bf20d7630281581a8fae124200920df11ad7cd68c14c26cc12c5ec9`
- **Size:** 58,520 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2018-09-17T09:18:08+00:00
- **Imphash:** `N/A`
- **Score:** 28

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `MmGetPhysicalAddress` from `ntoskrnl.exe`
- `MmMapLockedPagesSpecifyCache` from `ntoskrnl.exe`
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `MmAllocateContiguousMemory` from `ntoskrnl.exe`
- `MmMapIoSpace` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `MmGetSystemRoutineAddress` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `ZwCreateKey` from `ntoskrnl.exe`
- `ZwSetValueKey` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 18,757 | 18,944 | 6.23 | R | - | X |
| .rdata | 0x6000 | 3,792 | 4,096 | 4.51 | R | - | - |
| .data | 0x7000 | 6,070,432 | 1,024 | 1.13 | R | W | - |
| .pdata | 0x5d2000 | 1,656 | 2,048 | 3.95 | R | - | - |
| PAGE | 0x5d3000 | 7,025 | 7,168 | 6.07 | R | - | X |
| INIT | 0x5d5000 | 2,892 | 3,072 | 5.56 | R | W | X |
| .rsrc | 0x5d6000 | 1,016 | 1,024 | 3.42 | R | - | - |
| .reloc | 0x5d7000 | 96 | 512 | 0.32 | R | - | - |

**Interesting Strings:**
- `Nal Windows Driver Unload: IoDeleteDevice NOT called: NULL DeviceObject`
- `Nal Windows DriverAddDevice: done`
- `Nal Windows DriverIoCreateDevice failed.  Status = 0x%0x`
- `Nal Windows DriverAddDevice: entered`
- `NalDeviceControl: InputBuffer was incorrect`
- `Nal Windows DriverDeviceControl: Invalid IOCTL code 0x%0x`
- `NalResolveOsiIoctl: FuctionId = %d`
- `NAL_ENABLE_DEBUG_PRINT_FUNCID: FunctionData is NULL`
- `NalResolveHwBusIoctl: FuctionId = %d`
- `NalResolveHwBusIoctl: FunctionId = %d`

**Entry Point Disassembly:**
```asm
0x5e5250: mov rax, qword ptr [rip - 0x5ce14f]
0x5e5257: movabs r9, 0x2b992ddfa232
0x5e5261: test rax, rax
0x5e5264: je 0x5e526b
0x5e5266: cmp rax, r9
0x5e5269: jne 0x5e529a
0x5e526b: lea r8, [rip - 0x5ce16a]
0x5e5272: movabs rax, 0xfffff78000000320
```

---

## kEvP64.sys

- **SHA256:** `1aaa9aef39cb3c0a854ecb4ca7d3b213458f302025e0ec5bfbdef973cca9111c`
- **Size:** 177,816 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2016-02-02T05:35:30+00:00
- **Imphash:** `N/A`
- **Score:** 26

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `PsLookupProcessByProcessId` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `MmGetSystemRoutineAddress` from `ntoskrnl.exe`
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `MmMapLockedPagesSpecifyCache` from `ntoskrnl.exe`
- `ExAllocatePool` from `ntoskrnl.exe`
- `MmMapIoSpace` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `KeStackAttachProcess` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 114,381 | 114,688 | 5.6 | R | - | X |
| .rdata | 0x1d000 | 15,032 | 15,360 | 5.01 | R | - | - |
| .data | 0x21000 | 27,776 | 26,624 | 4.88 | R | W | - |
| .pdata | 0x28000 | 2,700 | 3,072 | 4.61 | R | - | - |
| PAGE | 0x29000 | 3,341 | 3,584 | 5.33 | R | - | X |
| INIT | 0x2a000 | 4,052 | 4,096 | 5.5 | R | W | X |
| .rsrc | 0x2b000 | 792 | 1,024 | 2.65 | R | - | - |
| .reloc | 0x2c000 | 24 | 512 | 0.12 | R | - | - |

**Interesting Strings:**
- `[kEvP64]ATA READ MBR FAILED!!`
- `[kEvP64] ProcessImageFileName: ProcessImageFileName returned 0x%X.`
- `[kEvP64] Unknown IOCTL: 0x%X (%04X,%04X)`
- `WRMSR`
- `RDMSR`
- `VMREAD`
- `VMWRITE`
- `AESKEYGENASSIST`
- `VAESKEYGENASSIST`
- `PsSetCreateProcessNotifyRoutine`

**Entry Point Disassembly:**
```asm
0x3a260: sub rsp, 0x28
0x3a264: mov r8, rdx
0x3a267: mov r9, rcx
0x3a26a: call 0x3a204
0x3a26f: mov rdx, r8
0x3a272: mov rcx, r9
0x3a275: add rsp, 0x28
0x3a279: jmp 0x3a010
```

---

## segwindrvx64.sys

- **SHA256:** `65329dad28e92f4bcc64de15c552b6ef424494028b18875b7dba840053bc0cdd`
- **Size:** 65,224 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2015-04-01T09:39:09+00:00
- **Imphash:** `N/A`
- **Score:** 26

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `MmMapLockedPagesSpecifyCache` from `ntoskrnl.exe`
- `MmMapIoSpace` from `ntoskrnl.exe`
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `MmAllocateContiguousMemorySpecifyCache` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `MmGetPhysicalAddress` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `MmGetSystemRoutineAddress` from `ntoskrnl.exe`
- `ExAllocatePool` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 14,224 | 14,336 | 6.37 | R | - | X |
| page | 0x5000 | 31,660 | 31,744 | 6.44 | R | - | X |
| init | 0xd000 | 2,272 | 2,560 | 5.75 | R | - | X |
| .rdata | 0xe000 | 2,220 | 2,560 | 4.24 | R | - | - |
| .data | 0xf000 | 1,992 | 512 | 0.36 | R | W | - |
| .pdata | 0x10000 | 960 | 1,024 | 4.1 | R | - | - |
| INIT | 0x11000 | 1,066 | 1,536 | 4.23 | R | W | X |
| .rsrc | 0x12000 | 1,048 | 1,536 | 2.5 | R | - | - |
| .reloc | 0x13000 | 20 | 512 | 0.21 | R | - | - |

**Interesting Strings:**
- `ERROR: Get SMICommandPort fail!`
- `Init SMICommandPort: %X`
- `===> READ_PCI32`
- `<=== READ_PCI32`
- `===> READ_PORT32`
- `<=== READ_PORT32`
- `===> READ_MEM_BUFFER`
- `<=== READ_MEM_BUFFER`
- `===> WRITE_MEM_BUFFER`
- `<=== WRITE_MEM_BUFFER`

**Entry Point Disassembly:**
```asm
0x140011000: mov qword ptr [rsp + 8], rbx
0x140011005: push rdi
0x140011006: sub rsp, 0x20
0x14001100a: mov rbx, rdx
0x14001100d: mov rdi, rcx
0x140011010: call 0x14001102c
0x140011015: mov rdx, rbx
0x140011018: mov rcx, rdi
```

---

## vboxdrv.sys

- **SHA256:** `cf3a7d4285d65bf8688215407bce1b51d7c6b22497f09021f0fce31cbeb78986`
- **Size:** 68,288 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2008-05-31T02:18:53+00:00
- **Imphash:** `N/A`
- **Score:** 26

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `IofCompleteRequest` from `ntoskrnl.exe`
- `MmGetSystemRoutineAddress` from `ntoskrnl.exe`
- `MmMapLockedPagesSpecifyCache` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `MmGetPhysicalAddress` from `ntoskrnl.exe`
- `MmAllocateContiguousMemory` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `PsGetCurrentProcessId` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `MmMapIoSpace` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x320 | 34,158 | 34,176 | 6.2 | R | - | X |
| .rdata | 0x88a0 | 10,840 | 10,848 | 5.6 | R | - | - |
| .data | 0xb300 | 7,424 | 7,424 | 1.97 | R | W | - |
| .pdata | 0xd000 | 3,312 | 3,328 | 4.66 | R | - | - |
| .edata | 0xdd00 | 2,674 | 2,688 | 5.33 | R | - | - |
| INIT | 0xe780 | 1,592 | 1,600 | 4.86 | R | W | X |
| .reloc | 0xedc0 | 316 | 320 | 3.71 | R | - | - |

**Exports (96):**
- `AssertMsg1`
- `RTAssertDoBreakpoint`
- `RTErrConvertFromNtStatus`
- `RTLogDefaultInstance`
- `RTLogLogger`
- `RTLogLoggerEx`
- `RTLogLoggerExV`
- `RTLogPrintf`
- `RTLogPrintfV`
- `RTLogRelDefaultInstance`
- ... and 86 more

**Interesting Strings:**
- `VBoxDrvLinuxIOCtl: too much output! %#x > %#x; uCmd=%#x!`
- `vboxdrv: Bad ioctl request header; cbIn=%#lx cbOut=%#lx fFlags=%#lx`
- `SUP_IOCTL_PAGE_ALLOC: Invalid input/output sizes. cbIn=%ld expected %ld. cbOut=%ld expected %ld.`
- `SUP_IOCTL_PAGE_ALLOC: %s`
- `pReq->Hdr.cbIn <= SUP_IOCTL_PAGE_ALLOC_SIZE_IN`
- `SUP_IOCTL_SET_VM_FOR_FAST: pVMR0=%p!`
- `SUP_IOCTL_LOW_ALLOC: Invalid input/output sizes. cbIn=%ld expected %ld. cbOut=%ld expected %ld.`
- `SUP_IOCTL_LOW_ALLOC: %s`
- `pReq->Hdr.cbIn <= SUP_IOCTL_LOW_ALLOC_SIZE_IN`
- `SUP_IOCTL_CALL_VMMR0: %s`

**Entry Point Disassembly:**
```asm
0x140000d40: mov qword ptr [rsp + 8], rbx
0x140000d45: mov qword ptr [rsp + 0x10], rsi
0x140000d4a: push rdi
0x140000d4b: sub rsp, 0x60
0x140000d4f: mov rsi, rcx
0x140000d52: lea rdx, [rip + 0x7de7]
0x140000d59: lea rcx, [rsp + 0x40]
0x140000d5e: call qword ptr [rip + 0x7b6c]
```

---

## 4ed2d2c1b00e87b926fb58b4ea43d2db35e5912975f4400aa7bd9f8c239d08b7.sys

- **SHA256:** `4ed2d2c1b00e87b926fb58b4ea43d2db35e5912975f4400aa7bd9f8c239d08b7`
- **Size:** 54,960 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2020-07-09T09:29:01+00:00
- **Imphash:** `N/A`
- **Score:** 24

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `MmMapIoSpace` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `MmGetSystemRoutineAddress` from `ntoskrnl.exe`
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `MmMapLockedPagesSpecifyCache` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `ZwCreateKey` from `ntoskrnl.exe`
- `ZwSetValueKey` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 16,923 | 17,408 | 6.29 | R | - | X |
| .rdata | 0x6000 | 2,348 | 2,560 | 4.5 | R | - | - |
| .data | 0x7000 | 1,024 | 1,024 | 1.15 | R | W | - |
| .pdata | 0x8000 | 936 | 1,024 | 4.03 | R | - | - |
| PAGE | 0x9000 | 8,084 | 8,192 | 6.22 | R | - | X |
| INIT | 0xb000 | 3,200 | 3,584 | 5.43 | R | W | X |
| .rsrc | 0xc000 | 1,144 | 1,536 | 2.62 | R | - | - |
| .reloc | 0xd000 | 108 | 512 | 0.35 | R | - | - |

**Interesting Strings:**
- `IOCTL_PHYMEM_MAP`
- `IOCTL_PHYMEM_UNMAP`
- `IOCTL_PHYMEM_GETPORT`
- `IOCTL_PHYMEM_SETPORT`
- `IOCTL_PHYMEM_SENDSMI`
- `IOCTL_ENUM_RTKNIC`
- `IOCTL_PHYMEM_GETPCIULONG`
- `IOCTL_PHYMEM_GETEEPROM`
- `IOCTL_PHYMEM_SETEEEPROM`
- `IOCTL_PHYMEM_GETCHANNEL`

**Entry Point Disassembly:**
```asm
0x1b4bc: sub rsp, 0x28
0x1b4c0: mov r8, rdx
0x1b4c3: mov r9, rcx
0x1b4c6: call 0x1b460
0x1b4cb: mov rdx, r8
0x1b4ce: mov rcx, r9
0x1b4d1: add rsp, 0x28
0x1b4d5: jmp 0x1b008
```

---

## AMDPowerProfiler.sys

- **SHA256:** `0af5ccb3d33a9ba92071c9637be6254030d61998733a5eb3583e865e17844e05`
- **Size:** 82,832 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2021-07-26T16:06:46+00:00
- **Imphash:** `N/A`
- **Score:** 24

**Dangerous Imports:**
- `MmMapIoSpace` from `ntoskrnl.exe`
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `ZwMapViewOfSection` from `ntoskrnl.exe`
- `MmGetSystemRoutineAddress` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `ZwSetValueKey` from `ntoskrnl.exe`
- `ZwCreateKey` from `ntoskrnl.exe`
- `MmMapLockedPagesSpecifyCache` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 40,705 | 40,960 | 6.4 | R | - | X |
| .rdata | 0xb000 | 5,884 | 6,144 | 4.68 | R | - | - |
| .data | 0xd000 | 11,604 | 1,536 | 2.11 | R | W | - |
| .pdata | 0x10000 | 2,892 | 3,072 | 4.53 | R | - | - |
| .gfids | 0x11000 | 4 | 512 | 0.02 | R | - | - |
| PAGE | 0x12000 | 6,884 | 7,168 | 6.17 | R | - | X |
| INIT | 0x14000 | 3,074 | 3,584 | 4.8 | R | - | X |
| .rsrc | 0x15000 | 1,056 | 1,536 | 2.48 | R | - | - |
| .reloc | 0x16000 | 44 | 512 | 0.53 | R | - | - |

**Interesting Strings:**
- `ReadPmcCounterData`
- `PwrProf: %s, thread %d bus %d`
- `PwrProf: %s, DPC thread id %d`
- `GetThreadsPerCore`
- `PwrProf: %s, Threads per core :%d`
- `GetThreadsPerSocket`
- `PwrProf: %s, Threads per socket :%d`
- `ReadPCIDev`
- `PwrProf: %s, bus %u , device %u , func %u , reg %u , address %u, data %u`
- `WritePCIDev`

**Entry Point Disassembly:**
```asm
0x140014000: mov qword ptr [rsp + 8], rbx
0x140014005: push rdi
0x140014006: sub rsp, 0x20
0x14001400a: mov rbx, rdx
0x14001400d: mov rdi, rcx
0x140014010: call 0x14001402c
0x140014015: mov rdx, rbx
0x140014018: mov rcx, rdi
```

---

## BdApiUtil.sys

- **SHA256:** `32198295d2a2700b9895fff999c2b233f9befb0bc175815ec4b71ee926b6edfc`
- **Size:** 116,984 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2015-03-19T02:58:13+00:00
- **Imphash:** `N/A`
- **Score:** 24

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `IoDeleteDevice` from `ntoskrnl.exe`
- `MmGetSystemRoutineAddress` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `PsGetCurrentProcessId` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `PsLookupProcessByProcessId` from `ntoskrnl.exe`
- `ZwCreateKey` from `ntoskrnl.exe`
- `ZwSetValueKey` from `ntoskrnl.exe`
- `ZwDeleteKey` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x320 | 24,496 | 24,512 | 6.27 | R | - | X |
| .rdata | 0x62e0 | 8,668 | 8,672 | 4.55 | R | - | - |
| .data | 0x84c0 | 64,128 | 64,128 | 1.41 | R | W | - |
| .pdata | 0x17f40 | 1,644 | 1,664 | 4.47 | R | - | - |
| PAGE | 0x185c0 | 643 | 672 | 5.8 | R | - | X |
| INIT | 0x18860 | 3,790 | 3,808 | 5.73 | R | W | X |
| .rsrc | 0x19740 | 864 | 864 | 3.35 | R | - | - |
| .reloc | 0x19aa0 | 2,632 | 2,656 | 4.76 | R | - | - |

**Interesting Strings:**
- `rdmsr`
- `wrmsr`
- `InitNtKeyCall  error 1`
- `IoDeleteDevice`
- `IoDetachDevice`
- `PsSetCreateProcessNotifyRoutine`
- `ZwQueryValueKey`
- `PsGetCurrentProcessId`
- `IoCreateDevice`
- `ZwOpenKey`

**Entry Point Disassembly:**
```asm
0x28860: mov r11, rsp
0x28863: mov qword ptr [r11 + 8], rbx
0x28867: push rbp
0x28868: push rsi
0x28869: push rdi
0x2886a: push r12
0x2886c: push r13
0x2886e: sub rsp, 0x550
```

---

## LHA.sys

- **SHA256:** `e75714f8e0ff45605f6fc7689a1a89c7dcd34aab66c6131c63fefaca584539cf`
- **Size:** 35,520 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2018-12-27T23:06:43+00:00
- **Imphash:** `N/A`
- **Score:** 24

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `IoDeleteDevice` from `ntoskrnl.exe`
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `MmGetPhysicalAddress` from `ntoskrnl.exe`
- `MmMapIoSpace` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `MmGetSystemRoutineAddress` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `ZwCreateKey` from `ntoskrnl.exe`
- `ZwSetValueKey` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 10,078 | 10,240 | 6.31 | R | - | X |
| .rdata | 0x4000 | 1,948 | 2,048 | 4.2 | R | - | - |
| .data | 0x5000 | 897 | 1,024 | 1.28 | R | W | - |
| .pdata | 0x6000 | 696 | 1,024 | 3.13 | R | - | - |
| PAGE | 0x7000 | 6,727 | 7,168 | 6.03 | R | - | X |
| INIT | 0x9000 | 1,832 | 2,048 | 4.81 | R | W | X |
| .rsrc | 0xa000 | 896 | 1,024 | 3.07 | R | - | - |
| .reloc | 0xb000 | 96 | 512 | 0.32 | R | - | - |

**Interesting Strings:**
- `IoDeleteDevice`
- `MmUnmapIoSpace`
- `MmFreeNonCachedMemory`
- `MmGetPhysicalAddress`
- `MmMapIoSpace`
- `MmAllocateNonCachedMemory`
- `KeStallExecutionProcessor`
- `IoCreateDevice`
- `IoDeviceObjectType`
- `SeExports`

**Entry Point Disassembly:**
```asm
0x19064: sub rsp, 0x28
0x19068: mov r8, rdx
0x1906b: mov r9, rcx
0x1906e: call 0x19008
0x19073: mov rdx, r8
0x19076: mov rcx, r9
0x19079: add rsp, 0x28
0x1907d: jmp 0x11008
```

---

## RootLaser.sys

- **SHA256:** `85b69f4e518c66b8ba7154ecb1ac1e8791dfe2fdf1e20b7c3a707f59639ac10d`
- **Size:** 43,192 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2025-01-30T13:42:06+00:00
- **Imphash:** `N/A`
- **Score:** 24

**Dangerous Imports:**
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `MmGetSystemRoutineAddress` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `ZwSetValueKey` from `ntoskrnl.exe`
- `ZwCreateKey` from `ntoskrnl.exe`
- `ZwOpenProcess` from `ntoskrnl.exe`
- `PsLookupProcessByProcessId` from `ntoskrnl.exe`
- `ZwDeleteKey` from `ntoskrnl.exe`
- `MmMapLockedPagesSpecifyCache` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 12,116 | 12,288 | 6.21 | R | - | X |
| .rdata | 0x4000 | 4,508 | 4,608 | 4.61 | R | - | - |
| .data | 0x6000 | 800 | 1,024 | 2.01 | R | W | - |
| .pdata | 0x7000 | 1,104 | 1,536 | 3.32 | R | - | - |
| PAGE | 0x8000 | 7,356 | 7,680 | 6.12 | R | - | X |
| INIT | 0xa000 | 2,910 | 3,072 | 5.33 | R | - | X |
| .rsrc | 0xb000 | 1,008 | 1,024 | 3.25 | R | - | - |
| .reloc | 0xc000 | 108 | 512 | 1.45 | R | - | - |

**Interesting Strings:**
- `IRP_MJ_READ`
- `IRP_MJ_WRITE`
- `IRP_MJ_DEVICE_CONTROL`
- `IRP_MJ_INTERNAL_DEVICE_CONTROL`
- `IRP_MJ_DEVICE_CHANGE`
- `IoDeleteDevice`
- `IoDeviceObjectType`
- `IoCreateDevice`
- `SeExports`
- `ZwOpenKey`

**Entry Point Disassembly:**
```asm
0x14000a170: mov qword ptr [rsp + 8], rbx
0x14000a175: push rdi
0x14000a176: sub rsp, 0x20
0x14000a17a: mov rbx, rdx
0x14000a17d: mov rdi, rcx
0x14000a180: call 0x14000a19c
0x14000a185: mov rdx, rbx
0x14000a188: mov rcx, rdi
```

---

## AMDRyzenMasterDriver.sys

- **SHA256:** `a13054f349b7baa8c8a3fcbd31789807a493cc52224bbff5e412eb2bd52a6433`
- **Size:** 70,432 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2019-05-13T08:14:16+00:00
- **Imphash:** `N/A`
- **Score:** 22

**Dangerous Imports:**
- `MmMapIoSpace` from `ntoskrnl.exe`
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `MmMapLockedPagesSpecifyCache` from `ntoskrnl.exe`
- `MmGetSystemRoutineAddress` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `ZwSetValueKey` from `ntoskrnl.exe`
- `ZwCreateKey` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 35,626 | 35,840 | 6.02 | R | - | X |
| .rdata | 0xa000 | 2,852 | 3,072 | 4.29 | R | - | - |
| .data | 0xb000 | 134,320 | 1,536 | 2.3 | R | W | - |
| .pdata | 0x2c000 | 1,260 | 1,536 | 3.97 | R | - | - |
| .gfids | 0x2d000 | 4 | 512 | 0.02 | R | - | - |
| PAGE | 0x2e000 | 6,884 | 7,168 | 6.2 | R | - | X |
| INIT | 0x30000 | 1,790 | 2,048 | 4.69 | R | - | X |
| .rsrc | 0x31000 | 968 | 1,024 | 3.14 | R | - | - |
| .reloc | 0x32000 | 52 | 512 | 0.61 | R | - | - |

**Interesting Strings:**
- `!!!AODDriver::SimplDrvDispatch(): ReadSBRegister_SB700`
- `!!!AODDriver::SimplDrvDispatch(): ReadSBRegister_SB700 %d`
- `!!!AODDriver::SimplDrvDispatch(): ReadSBRegister_SB700--->2`
- `!!!AODDriver::SimplDrvDispatch(): ReadSBRegister_SB700--->3`
- `!!!AODDriver::SimplDrvDispatch(): ReadSBRegister_SB700 Failed`
- `!!!AODDriver::SimplDrvDispatch(): ReadSBRegister return 0`
- `!!!AODDriver::SimplDrvDispatch(): ReadSBRegister_SB800`
- `!!!AODDriver::DriverDispatch():IOCTL_WRITE_FUNCTION0:DEVICE_ID not found`
- `!!!AODDriver::DriverDispatch():IOCTL_WRITE_FUNCTION1:DEVICE_ID not found`
- `!!!AODDriver::DriverDispatch():IOCTL_WRITE_FUNCTION2:DEVICE_ID not found`

**Entry Point Disassembly:**
```asm
0x140008a48: mov qword ptr [rsp + 8], rbx
0x140008a4d: push rdi
0x140008a4e: sub rsp, 0x20
0x140008a52: mov rbx, rdx
0x140008a55: mov rdi, rcx
0x140008a58: call 0x140030000
0x140008a5d: mov rdx, rbx
0x140008a60: mov rcx, rdi
```

---

## HwOs2Ec7x64.sys

- **SHA256:** `b179e1ab6dc0b1aee783adbcad4ad6bb75a8a64cb798f30c0dd2ee8aaf43e6de`
- **Size:** 42,104 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2017-04-13T02:51:36+00:00
- **Imphash:** `N/A`
- **Score:** 22

**Dangerous Imports:**
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `ExAllocatePool` from `ntoskrnl.exe`
- `MmMapLockedPagesSpecifyCache` from `ntoskrnl.exe`
- `ZwOpenProcess` from `ntoskrnl.exe`
- `ZwAllocateVirtualMemory` from `ntoskrnl.exe`
- `KeStackAttachProcess` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `MmGetSystemRoutineAddress` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 8,324 | 8,704 | 6.08 | R | - | X |
| .rdata | 0x4000 | 2,452 | 2,560 | 4.23 | R | - | - |
| .data | 0x5000 | 1,564 | 512 | 0.3 | R | W | - |
| .pdata | 0x6000 | 828 | 1,024 | 3.57 | R | - | - |
| .gfids | 0x7000 | 4 | 512 | 0.02 | R | - | - |
| PAGE | 0x8000 | 6,378 | 6,656 | 6.09 | R | - | X |
| INIT | 0xa000 | 2,346 | 2,560 | 5.21 | R | - | X |
| .rsrc | 0xb000 | 776 | 1,024 | 2.59 | R | - | - |
| .reloc | 0xc000 | 20 | 512 | 0.22 | R | - | - |

**Interesting Strings:**
- `CreateProcessW`
- `IoCreateDevice`
- `IoDeleteDevice`
- `IoGetCurrentProcess`
- `MmMapLockedPagesSpecifyCache`
- `MmUnmapLockedPages`
- `PsSetCreateProcessNotifyRoutine`
- `ZwOpenProcess`
- `ZwAllocateVirtualMemory`
- `ZwFreeVirtualMemory`

**Entry Point Disassembly:**
```asm
0x14000a0a0: mov qword ptr [rsp + 8], rbx
0x14000a0a5: push rdi
0x14000a0a6: sub rsp, 0x20
0x14000a0aa: mov rbx, rdx
0x14000a0ad: mov rdi, rcx
0x14000a0b0: call 0x14000a0cc
0x14000a0b5: mov rdx, rbx
0x14000a0b8: mov rcx, rdi
```

---

## ProcessCtr.sys

- **SHA256:** `d64eeb940daffdc8327fb18b160c20e539088cf8407813655f59efa9fdf0022e`
- **Size:** 44,440 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2024-09-05T13:48:53+00:00
- **Imphash:** `N/A`
- **Score:** 22

**Dangerous Imports:**
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `ZwSetValueKey` from `ntoskrnl.exe`
- `ZwOpenProcess` from `ntoskrnl.exe`
- `MmGetSystemRoutineAddress` from `ntoskrnl.exe`
- `MmMapLockedPagesSpecifyCache` from `ntoskrnl.exe`
- `PsLookupProcessByProcessId` from `ntoskrnl.exe`
- `ZwAllocateVirtualMemory` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 10,875 | 11,264 | 6.16 | R | - | X |
| .rdata | 0x4000 | 2,036 | 2,048 | 4.27 | R | - | - |
| .data | 0x5000 | 289,760 | 512 | 0.28 | R | W | - |
| .pdata | 0x4c000 | 600 | 1,024 | 2.66 | R | - | - |
| INIT | 0x4d000 | 1,898 | 2,048 | 5.15 | R | - | X |
| .rsrc | 0x4e000 | 824 | 1,024 | 2.69 | R | - | - |
| .reloc | 0x4f000 | 20 | 512 | 0.26 | R | - | - |

**Interesting Strings:**
- `ProcessCtr LoadConfig fail^^`
- `MyTerminateProcess False: ZwOpenProcess : 0x%08X`
- `\Process\bin\ProcessCtrl64.pdb`
- `IoGetCurrentProcess`
- `PsGetProcessImageFileName`
- `ZwReadFile`
- `KeDelayExecutionThread`
- `PsCreateSystemThread`
- `IoCreateDevice`
- `IoDeleteDevice`

**Entry Point Disassembly:**
```asm
0x14004d000: mov qword ptr [rsp + 8], rbx
0x14004d005: push rdi
0x14004d006: sub rsp, 0x20
0x14004d00a: mov rbx, rdx
0x14004d00d: mov rdi, rcx
0x14004d010: call 0x14004d02c
0x14004d015: mov rdx, rbx
0x14004d018: mov rcx, rdi
```

---

## Truesight

- **SHA256:** `bfc2ef3b404294fe2fa05a8b71c7f786b58519175b7202a69fe30f45e607ff1c`
- **Size:** 53,696 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2023-08-29T12:07:25+00:00
- **Imphash:** `N/A`
- **Score:** 22

**Dangerous Imports:**
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `MmGetSystemRoutineAddress` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `ZwSetValueKey` from `ntoskrnl.exe`
- `ZwCreateKey` from `ntoskrnl.exe`
- `ZwOpenProcess` from `ntoskrnl.exe`
- `ZwDeleteKey` from `ntoskrnl.exe`
- `MmMapLockedPagesSpecifyCache` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 11,732 | 11,776 | 6.24 | R | - | X |
| .rdata | 0x4000 | 4,484 | 4,608 | 4.6 | R | - | - |
| .data | 0x6000 | 800 | 1,024 | 2.02 | R | W | - |
| .pdata | 0x7000 | 1,092 | 1,536 | 3.28 | R | - | - |
| PAGE | 0x8000 | 7,356 | 7,680 | 6.13 | R | - | X |
| INIT | 0xa000 | 2,836 | 3,072 | 5.24 | R | - | X |
| .rsrc | 0xb000 | 992 | 1,024 | 3.21 | R | - | - |
| .reloc | 0xc000 | 108 | 512 | 1.45 | R | - | - |

**Interesting Strings:**
- `IRP_MJ_READ`
- `IRP_MJ_WRITE`
- `IRP_MJ_DEVICE_CONTROL`
- `IRP_MJ_INTERNAL_DEVICE_CONTROL`
- `IRP_MJ_DEVICE_CHANGE`
- `IoDeleteDevice`
- `IoDeviceObjectType`
- `IoCreateDevice`
- `SeExports`
- `ZwOpenKey`

**Entry Point Disassembly:**
```asm
0x14000a170: mov qword ptr [rsp + 8], rbx
0x14000a175: push rdi
0x14000a176: sub rsp, 0x20
0x14000a17a: mov rbx, rdx
0x14000a17d: mov rdi, rcx
0x14000a180: call 0x14000a19c
0x14000a185: mov rdx, rbx
0x14000a188: mov rcx, rdi
```

---

## UCOREW64.SYS

- **SHA256:** `a7c8f4faf3cbb088cac7753d81f8ec4c38ccb97cd9da817741f49272e8d01200`
- **Size:** 14,632 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2008-04-22T11:01:50+00:00
- **Imphash:** `N/A`
- **Score:** 22

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `MmMapLockedPages` from `ntoskrnl.exe`
- `MmGetPhysicalAddress` from `ntoskrnl.exe`
- `MmAllocateContiguousMemory` from `ntoskrnl.exe`
- `MmMapIoSpace` from `ntoskrnl.exe`
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `ZwMapViewOfSection` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x260 | 6,352 | 6,368 | 6.35 | R | - | X |
| .pdata | 0x1b40 | 144 | 160 | 3.18 | R | - | - |
| INIT | 0x1be0 | 794 | 800 | 4.6 | R | W | X |

**Interesting Strings:**
- `GenericDrv.SYS: Inside gdDMIAccessPort`
- `GenericDrv.SYS: Inside gdReadPort`
- `GenericDrv.SYS: Inside gdWritePort`
- `Leaving MapPhysicalMemoryToLinearSpace`
- `ERROR: MappingLength = 0`
- `ERROR: ZwMapViewOfSection failed`
- `Entering MapPhysicalMemoryToLinearSpace`
- `Leaving UnmapPhysicalMemory`
- `ERROR: UnmapViewOfSection failed`
- `Entering UnmapPhysicalMemory`

**Entry Point Disassembly:**
```asm
0x401538: push rbx
0x40153a: push rdi
0x40153b: sub rsp, 0x78
0x40153f: mov rdi, rcx
0x401542: lea rcx, [rip - 0xbb1]
0x401549: call 0x401638
0x40154e: lea rdx, [rip - 0xbe5]
0x401555: lea rcx, [rsp + 0x48]
```

---

## b205835b818d8a50903cf76936fcf8160060762725bd74a523320cfbd091c038.sys

- **SHA256:** `b205835b818d8a50903cf76936fcf8160060762725bd74a523320cfbd091c038`
- **Size:** 55,984 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2020-07-09T09:28:49+00:00
- **Imphash:** `N/A`
- **Score:** 22

**Dangerous Imports:**
- `MmMapIoSpace` from `ntoskrnl.exe`
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `MmGetSystemRoutineAddress` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `MmMapLockedPagesSpecifyCache` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `ZwCreateKey` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `ZwSetValueKey` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 16,123 | 16,384 | 6.38 | R | - | X |
| .rdata | 0x5000 | 3,812 | 4,096 | 4.4 | R | - | - |
| .data | 0x6000 | 776 | 1,024 | 1.27 | R | W | - |
| .pdata | 0x7000 | 984 | 1,024 | 4.21 | R | - | - |
| PAGE | 0x8000 | 8,740 | 9,216 | 6.1 | R | - | X |
| INIT | 0xb000 | 2,672 | 3,072 | 5.22 | R | - | X |
| .rsrc | 0xc000 | 1,120 | 1,536 | 2.58 | R | - | - |
| .reloc | 0xd000 | 48 | 512 | 0.57 | R | - | - |

**Interesting Strings:**
- `IOCTL_PHYMEM_MAP`
- `IOCTL_PHYMEM_UNMAP`
- `IOCTL_PHYMEM_GETPORT`
- `IOCTL_PHYMEM_SETPORT`
- `IOCTL_PHYMEM_SENDSMI`
- `IOCTL_ENUM_RTKNIC`
- `IOCTL_PHYMEM_GETPCIULONG`
- `IOCTL_PHYMEM_GETEEPROM`
- `IOCTL_PHYMEM_SETEEEPROM`
- `IOCTL_PHYMEM_GETCHANNEL`

**Entry Point Disassembly:**
```asm
0x14000b290: mov qword ptr [rsp + 8], rbx
0x14000b295: push rdi
0x14000b296: sub rsp, 0x20
0x14000b29a: mov rbx, rdx
0x14000b29d: mov rdi, rcx
0x14000b2a0: call 0x14000b2bc
0x14000b2a5: mov rdx, rbx
0x14000b2a8: mov rcx, rdi
```

---

## procexp1627.sys

- **SHA256:** `9b6a84f7c40ea51c38cc4d2e93efb3375e9d98d4894a85941190d94fbe73a4e4`
- **Size:** 36,192 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2019-12-13T16:37:59+00:00
- **Imphash:** `N/A`
- **Score:** 22

**Dangerous Imports:**
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `MmGetSystemRoutineAddress` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `ZwOpenProcess` from `ntoskrnl.exe`
- `KeStackAttachProcess` from `ntoskrnl.exe`
- `PsLookupProcessByProcessId` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `ZwCreateKey` from `ntoskrnl.exe`
- `ZwSetValueKey` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 9,632 | 9,728 | 6.1 | R | - | X |
| .rdata | 0x4000 | 3,944 | 4,096 | 4.2 | R | - | - |
| .data | 0x5000 | 556 | 512 | 2.07 | R | W | - |
| .pdata | 0x6000 | 756 | 1,024 | 3.31 | R | - | - |
| PAGE | 0x7000 | 6,683 | 7,168 | 6.03 | R | - | X |
| INIT | 0x9000 | 2,072 | 2,560 | 4.52 | R | - | X |
| .rsrc | 0xa000 | 896 | 1,024 | 2.99 | R | - | - |
| .reloc | 0xb000 | 48 | 512 | 0.64 | R | - | - |

**Interesting Strings:**
- `IoDeleteDevice`
- `ZwOpenProcess`
- `KeStackAttachProcess`
- `KeUnstackDetachProcess`
- `SePrivilegeCheck`
- `PsLookupProcessByProcessId`
- `ZwOpenProcessToken`
- `ZwQueryInformationProcess`
- `PsProcessType`
- `PsThreadType`

**Entry Point Disassembly:**
```asm
0x180009058: sub rsp, 0x28
0x18000905c: mov r8, rdx
0x18000905f: mov r9, rcx
0x180009062: call 0x180009000
0x180009067: mov rdx, r8
0x18000906a: mov rcx, r9
0x18000906d: add rsp, 0x28
0x180009071: jmp 0x1800015d0
```

---

## viraglt64.sys

- **SHA256:** `58a74dceb2022cd8a358b92acd1b48a5e01c524c3b0195d7033e4bd55eff4495`
- **Size:** 82,848 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2016-09-07T08:36:15+00:00
- **Imphash:** `N/A`
- **Score:** 22

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `ZwCreateKey` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `ZwSetValueKey` from `ntoskrnl.exe`
- `MmMapLockedPagesSpecifyCache` from `ntoskrnl.exe`
- `ZwOpenProcess` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `ZwDeleteKey` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 53,338 | 53,760 | 6.36 | R | - | X |
| .rdata | 0xf000 | 3,160 | 3,584 | 4.54 | R | - | - |
| .data | 0x10000 | 14,688 | 512 | 0.93 | R | W | - |
| .pdata | 0x14000 | 876 | 1,024 | 3.88 | R | - | - |
| INIT | 0x15000 | 2,114 | 2,560 | 4.56 | R | W | X |
| .rsrc | 0x16000 | 1,080 | 1,536 | 2.52 | R | - | - |
| .reloc | 0x17000 | 332 | 512 | 1.65 | R | - | - |

**Interesting Strings:**
- `\Registry\User\`
- `\Registry\Machine\`
- `\Registry\Machine\SOFTWARE\Classes\`
- `Processo: `
- `\Device\Harddisk0\DR0`
- `SOFTWARE\Microsoft\Command Processor`
- `Debugger`
- `%SystemRoot%\system32\csrss.exe ObjectDirectory=\Windows SharedSection=1024,20480,768 Windows=On SubSystemType=Windows ServerDll=basesrv,1 ServerDll=winsrv:UserServerDllInitialization,3 ServerDll=winsrv:ConServerDllInitialization,2 ServerDll=sxssrv,4 ProfileControl=Off MaxRequestThreads=16`
- `IRP_MJ_DEVICE_CHANGE`
- `IRP_MJ_INTERNAL_DEVICE_CONTROL`

**Entry Point Disassembly:**
```asm
0x25008: mov rax, qword ptr [rip - 0x4f0f]
0x2500f: movabs r9, 0x2b992ddfa232
0x25019: test rax, rax
0x2501c: je 0x25023
0x2501e: cmp rax, r9
0x25021: jne 0x25052
0x25023: lea r8, [rip - 0x4f2a]
0x2502a: movabs rax, 0xfffff78000000320
```

---

## 0eab16c7f54b61620277977f8c332737081a46bc6bbde50742b6904bdd54f502.sys

- **SHA256:** `0eab16c7f54b61620277977f8c332737081a46bc6bbde50742b6904bdd54f502`
- **Size:** 23,112 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2009-08-07T22:44:51+00:00
- **Imphash:** `N/A`
- **Score:** 20

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `ZwSetValueKey` from `ntoskrnl.exe`
- `MmMapIoSpace` from `ntoskrnl.exe`
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `MmMapLockedPagesSpecifyCache` from `ntoskrnl.exe`
- `ZwCreateKey` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 9,038 | 9,216 | 6.28 | R | - | X |
| .rdata | 0x4000 | 780 | 1,024 | 3.55 | R | - | - |
| .data | 0x5000 | 436 | 512 | 0.3 | R | W | - |
| .pdata | 0x6000 | 336 | 512 | 2.8 | R | - | - |
| INIT | 0x7000 | 2,402 | 2,560 | 5.55 | R | W | X |
| .rsrc | 0x8000 | 1,824 | 2,048 | 3.12 | R | - | - |

**Interesting Strings:**
- `ZwCreateKey`
- `ZwSetValueKey`
- `NtQueryInformationProcess`
- `MmMapIoSpace`
- `MmUnmapIoSpace`
- `IoQueryDeviceDescription`
- `ZwSetInformationThread`
- `MmMapLockedPagesSpecifyCache`
- `MmUnmapLockedPages`
- `IoReportResourceUsage`

**Entry Point Disassembly:**
```asm
0x173e8: sub rsp, 0x28
0x173ec: mov r8, rdx
0x173ef: mov r9, rcx
0x173f2: call 0x1738c
0x173f7: mov rdx, r8
0x173fa: mov rcx, r9
0x173fd: add rsp, 0x28
0x17401: jmp 0x17120
```

---

## LenovoDiagnosticsDriver.sys

- **SHA256:** `f05b1ee9e2f6ab704b8919d5071becbce6f9d0f9d0ba32a460c41d5272134abe`
- **Size:** 40,472 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2022-01-28T17:59:24+00:00
- **Imphash:** `N/A`
- **Score:** 20

**Dangerous Imports:**
- `MmMapIoSpace` from `ntoskrnl.exe`
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `MmGetSystemRoutineAddress` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `ZwSetValueKey` from `ntoskrnl.exe`
- `ZwCreateKey` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 4,534 | 4,608 | 6.15 | R | - | X |
| .rdata | 0x3000 | 3,636 | 4,096 | 3.93 | R | - | - |
| .data | 0x4000 | 640 | 1,024 | 1.45 | R | W | - |
| .pdata | 0x5000 | 732 | 1,024 | 3.17 | R | - | - |
| PAGE | 0x6000 | 7,308 | 7,680 | 6.08 | R | - | X |
| INIT | 0x8000 | 1,390 | 1,536 | 4.81 | R | - | X |
| .rsrc | 0x9000 | 1,176 | 1,536 | 2.85 | R | - | - |
| .reloc | 0xa000 | 64 | 512 | 0.83 | R | - | - |

**Interesting Strings:**
- `Error IoCreateDevice control %#04x`
- `MmMapIoSpace`
- `MmUnmapIoSpace`
- `IoDeleteDevice`
- `IoDeviceObjectType`
- `IoCreateDevice`
- `SeExports`
- `ZwOpenKey`
- `ZwSetValueKey`
- `ZwQueryValueKey`

**Entry Point Disassembly:**
```asm
0x140008000: mov qword ptr [rsp + 8], rbx
0x140008005: push rdi
0x140008006: sub rsp, 0x20
0x14000800a: mov rbx, rdx
0x14000800d: mov rdi, rcx
0x140008010: call 0x14000802c
0x140008015: mov rdx, rbx
0x140008018: mov rcx, rdi
```

---

## NCHGBIOS2x64.SYS

- **SHA256:** `314384b40626800b1cde6fbc51ebc7d13e91398be2688c2a58354aa08d00b073`
- **Size:** 18,840 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2013-01-15T01:59:36+00:00
- **Imphash:** `N/A`
- **Score:** 20

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `MmGetPhysicalAddress` from `ntoskrnl.exe`
- `MmMapLockedPagesSpecifyCache` from `ntoskrnl.exe`
- `MmMapIoSpace` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `MmAllocateContiguousMemory` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 4,364 | 4,608 | 5.99 | R | - | X |
| .rdata | 0x3000 | 424 | 512 | 4.11 | R | - | - |
| .data | 0x4000 | 280 | 512 | 0.3 | R | W | - |
| .pdata | 0x5000 | 108 | 512 | 0.96 | R | - | - |
| INIT | 0x6000 | 686 | 1,024 | 3.68 | R | W | X |
| .rsrc | 0x7000 | 1,040 | 1,536 | 2.53 | R | - | - |

**Interesting Strings:**
- `IoDeleteDevice`
- `MmFreeContiguousMemory`
- `MmUnmapIoSpace`
- `MmGetPhysicalAddress`
- `MmMapLockedPagesSpecifyCache`
- `MmMapIoSpace`
- `RtlCompareMemory`
- `IoCreateDevice`
- `MmAllocateContiguousMemory`

**Entry Point Disassembly:**
```asm
0x16008: mov rax, qword ptr [rip - 0x1f0f]
0x1600f: movabs r9, 0x2b992ddfa232
0x16019: test rax, rax
0x1601c: je 0x16023
0x1601e: cmp rax, r9
0x16021: jne 0x16052
0x16023: lea r8, [rip - 0x1f2a]
0x1602a: movabs rax, 0xfffff78000000320
```

---

## rtkio64.sys

- **SHA256:** `7133a461aeb03b4d69d43f3d26cd1a9e3ee01694e97a0645a3d8aa1a44c39129`
- **Size:** 45,920 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2017-08-23T15:43:11+00:00
- **Imphash:** `N/A`
- **Score:** 20

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `MmMapIoSpace` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `MmGetSystemRoutineAddress` from `ntoskrnl.exe`
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `MmMapLockedPagesSpecifyCache` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 16,491 | 16,896 | 6.31 | R | - | X |
| .rdata | 0x6000 | 1,076 | 1,536 | 3.89 | R | - | - |
| .data | 0x7000 | 512 | 512 | 0.36 | R | W | - |
| .pdata | 0x8000 | 528 | 1,024 | 2.42 | R | - | - |
| PAGE | 0x9000 | 1,364 | 1,536 | 5.63 | R | - | X |
| INIT | 0xa000 | 2,562 | 3,072 | 5.26 | R | W | X |
| .rsrc | 0xb000 | 1,144 | 1,536 | 2.62 | R | - | - |
| .reloc | 0xc000 | 12 | 512 | 0.08 | R | - | - |

**Interesting Strings:**
- `IOCTL_PHYMEM_MAP`
- `IOCTL_PHYMEM_UNMAP`
- `IOCTL_PHYMEM_GETPORT`
- `IOCTL_PHYMEM_SETPORT`
- `IOCTL_PHYMEM_SENDSMI`
- `IOCTL_ENUM_RTKNIC`
- `IOCTL_PHYMEM_GETPCIULONG`
- `IOCTL_PHYMEM_GETEEPROM`
- `IOCTL_PHYMEM_SETEEEPROM`
- `IOCTL_PHYMEM_GETCHANNEL`

**Entry Point Disassembly:**
```asm
0x1a534: sub rsp, 0x28
0x1a538: mov r8, rdx
0x1a53b: mov r9, rcx
0x1a53e: call 0x1a4d8
0x1a543: mov rdx, r8
0x1a546: mov rcx, r9
0x1a549: add rsp, 0x28
0x1a54d: jmp 0x1a008
```

---

## ATSZIO.sys

- **SHA256:** `01e024cb14b34b6d525c642a710bfa14497ea20fd287c39ba404b10a8b143ece`
- **Size:** 20,280 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2014-09-18T12:04:29+00:00
- **Imphash:** `N/A`
- **Score:** 18

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `ExAllocatePool` from `ntoskrnl.exe`
- `MmAllocateContiguousMemory` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `ZwMapViewOfSection` from `ntoskrnl.exe`
- `MmGetPhysicalAddress` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 1,492 | 1,536 | 5.48 | R | - | X |
| .rdata | 0x2000 | 748 | 1,024 | 3.07 | R | - | - |
| .data | 0x3000 | 280 | 512 | 0.3 | R | W | - |
| .pdata | 0x4000 | 96 | 512 | 0.88 | R | - | - |
| PAGE | 0x5000 | 3,326 | 3,584 | 5.95 | R | - | X |
| INIT | 0x6000 | 1,500 | 1,536 | 5.52 | R | W | X |
| .rsrc | 0x7000 | 816 | 1,024 | 2.74 | R | - | - |
| .reloc | 0x8000 | 12 | 512 | 0.08 | R | - | - |

**Interesting Strings:**
- `D:\SOURCE_IMG\ATSZIO\ATSZIO-Support_Qword_Address\ATSZIO_LIB-SRC-217_Test005-Support2008-002\VS2012\ATSZIO\x64\Win7Release\ATSZIO64.pdb`
- `MmAllocateContiguousMemory`
- `MmFreeContiguousMemory`
- `IoCreateDevice`
- `IoDeleteDevice`
- `ZwMapViewOfSection`
- `ZwUnmapViewOfSection`
- `MmGetPhysicalAddress`

**Entry Point Disassembly:**
```asm
0x140006214: mov qword ptr [rsp + 8], rbx
0x140006219: push rdi
0x14000621a: sub rsp, 0x20
0x14000621e: mov rbx, rdx
0x140006221: mov rdi, rcx
0x140006224: call 0x1400061ac
0x140006229: mov rdx, rbx
0x14000622c: mov rcx, rdi
```

---

## PhlashNT.sys

- **SHA256:** `65db1b259e305a52042e07e111f4fa4af16542c8bacd33655f753ef642228890`
- **Size:** 61,496 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2009-09-23T04:36:24+00:00
- **Imphash:** `N/A`
- **Score:** 18

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `IoDeleteDevice` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `MmMapLockedPages` from `ntoskrnl.exe`
- `MmMapIoSpace` from `ntoskrnl.exe`
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 46,615 | 47,104 | 5.62 | R | - | X |
| .rdata | 0xd000 | 1,144 | 1,536 | 3.16 | R | - | - |
| .data | 0xe000 | 2,252 | 1,024 | 1.48 | R | W | - |
| .pdata | 0xf000 | 1,332 | 1,536 | 3.86 | R | - | - |
| INIT | 0x10000 | 530 | 1,024 | 2.98 | R | W | X |
| .rsrc | 0x11000 | 872 | 1,024 | 2.92 | R | - | - |
| .reloc | 0x12000 | 220 | 512 | 0.85 | R | - | - |

**Interesting Strings:**
- `IoctlInitialize()......`
- `IoctlDeinitialize...`
- `GetDeviceID...`
- `Fail to sense DeviceID=%xh`
- `FlashSenseID(Platform->Version=%d, DeviceID=%xh)...`
- `Write(%d, o=%xh, c=%xh, a=%xh)...`
- `DescriptorWriteEnable absent!`
- `DescriptorWriteEnable()`
- `DescriptorWriteDisable absent!`
- `DescriptorWriteDisable()`

**Entry Point Disassembly:**
```asm
0x20010: mov rax, qword ptr [rip - 0x1c7f]
0x20017: movabs r9, 0x2b992ddfa232
0x20021: test rax, rax
0x20024: je 0x2002b
0x20026: cmp rax, r9
0x20029: jne 0x2005a
0x2002b: lea r8, [rip - 0x1c9a]
0x20032: movabs rax, 0xfffff78000000320
```

---

## b16e217cdca19e00c1b68bdfb28ead53b20adeabd6edcd91542f9fbf48942877

- **SHA256:** `b16e217cdca19e00c1b68bdfb28ead53b20adeabd6edcd91542f9fbf48942877`
- **Size:** 27,936 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2015-02-07T11:54:22+00:00
- **Imphash:** `N/A`
- **Score:** 18

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `MmMapLockedPagesSpecifyCache` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `MmGetSystemRoutineAddress` from `ntoskrnl.exe`
- `PsLookupProcessByProcessId` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 12,020 | 12,288 | 6.13 | R | - | X |
| .rdata | 0x4000 | 1,084 | 1,536 | 3.53 | R | - | - |
| .data | 0x5000 | 445 | 512 | 0.53 | R | W | - |
| .pdata | 0x6000 | 348 | 512 | 2.89 | R | - | - |
| INIT | 0x7000 | 1,342 | 1,536 | 4.62 | R | W | X |
| .rsrc | 0x8000 | 1,016 | 1,024 | 3.28 | R | - | - |
| .reloc | 0x9000 | 60 | 512 | 0.12 | R | - | - |

**Interesting Strings:**
- `IoDeleteDevice`
- `KeDelayExecutionThread`
- `MmMapLockedPagesSpecifyCache`
- `IoCreateDevice`
- `PsLookupProcessByProcessId`
- `PsInitialSystemProcess`
- `ZwTerminateProcess`
- `IoGetBaseFileSystemDeviceObject`
- `IoGetDeviceObjectPointer`

**Entry Point Disassembly:**
```asm
0x17008: mov rax, qword ptr [rip - 0x1f0f]
0x1700f: movabs r9, 0x2b992ddfa232
0x17019: test rax, rax
0x1701c: je 0x17023
0x1701e: cmp rax, r9
0x17021: jne 0x17052
0x17023: lea r8, [rip - 0x1f2a]
0x1702a: movabs rax, 0xfffff78000000320
```

---

## libnicm.sys

- **SHA256:** `95d50c69cdbf10c9c9d61e64fe864ac91e6f6caa637d128eb20e1d3510e776d3`
- **Size:** 35,344 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2022-03-03T10:49:58+00:00
- **Imphash:** `N/A`
- **Score:** 18

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `MmMapLockedPagesSpecifyCache` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `ZwCreateKey` from `ntoskrnl.exe`
- `ZwSetValueKey` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 15,136 | 15,360 | 6.27 | R | - | X |
| .rdata | 0x5000 | 1,412 | 1,536 | 4.44 | R | - | - |
| .data | 0x6000 | 2,408 | 1,024 | 2.77 | R | W | - |
| .pdata | 0x7000 | 564 | 1,024 | 2.56 | R | - | - |
| .edata | 0x8000 | 398 | 512 | 4.12 | R | - | - |
| INIT | 0x9000 | 2,892 | 3,072 | 5.58 | R | W | X |
| .rsrc | 0xa000 | 856 | 1,024 | 2.88 | R | - | - |
| .reloc | 0xb000 | 24 | 512 | 0.12 | R | - | - |

**Exports (11):**
- `NicmCreateInstance`
- `NicmDeregisterClassFactory`
- `NicmGetVersion`
- `NicmRegisterClassFactory`
- `XTComCreateInstance`
- `XTComDeregisterClassFactory`
- `XTComFreeUnusedLibrariesEx`
- `XTComGetClassObject`
- `XTComGetVersion`
- `XTComInitialize`
- ... and 1 more

**Interesting Strings:**
- `[NICM] NICM_IOCTL_REQUEST_REPLY Exception 0x%08X detected.`
- `nicm: Failed to create the device object 0x%X`
- `MmUnmapLockedPages`
- `ProbeForRead`
- `IoDeleteDevice`
- `ProbeForWrite`
- `MmMapLockedPagesSpecifyCache`
- `IoCreateDevice`
- `IoGetCurrentProcess`
- `ZwCreateKey`

**Entry Point Disassembly:**
```asm
0x192e0: sub rsp, 0x28
0x192e4: mov r8, rdx
0x192e7: mov r9, rcx
0x192ea: call 0x19284
0x192ef: mov rdx, r8
0x192f2: mov rcx, r9
0x192f5: add rsp, 0x28
0x192f9: jmp 0x19008
```

---

## mtcBSv64.sys

- **SHA256:** `c9cf1d627078f63a36bbde364cd0d5f2be1714124d186c06db5bcdf549a109f8`
- **Size:** 34,328 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2009-08-06T09:55:21+00:00
- **Imphash:** `N/A`
- **Score:** 18

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `MmMapIoSpace` from `ntoskrnl.exe`
- `ExAllocatePool` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 13,347 | 13,824 | 6.39 | R | - | X |
| .rdata | 0x5000 | 2,940 | 3,072 | 4.96 | R | - | - |
| .data | 0x6000 | 1,072 | 1,024 | 2.15 | R | W | - |
| .pdata | 0x7000 | 564 | 1,024 | 2.49 | R | - | - |
| PAGE | 0x8000 | 4,342 | 4,608 | 5.89 | R | - | X |
| INIT | 0xa000 | 1,510 | 1,536 | 5.34 | R | W | X |
| .rsrc | 0xb000 | 976 | 1,024 | 3.22 | R | - | - |
| .reloc | 0xc000 | 278 | 512 | 2.25 | R | - | - |

**Interesting Strings:**
- `IRP_MN_START_DEVICE`
- `IRP_MN_QUERY_REMOVE_DEVICE`
- `IRP_MN_REMOVE_DEVICE`
- `IRP_MN_CANCEL_REMOVE_DEVICE`
- `IRP_MN_STOP_DEVICE`
- `IRP_MN_QUERY_STOP_DEVICE`
- `IRP_MN_CANCEL_STOP_DEVICE`
- `IRP_MN_QUERY_DEVICE_RELATIONS`
- `IRP_MN_QUERY_DEVICE_TEXT`
- `IRP_MN_READ_CONFIG`

**Entry Point Disassembly:**
```asm
0x1a108: sub rsp, 0x28
0x1a10c: mov r8, rdx
0x1a10f: mov r9, rcx
0x1a112: call 0x1a0ac
0x1a117: mov rdx, r8
0x1a11a: mov rcx, r9
0x1a11d: add rsp, 0x28
0x1a121: jmp 0x1a008
```

---

## rtkiow10x64.sys

- **SHA256:** `32e1a8513eee746d17eb5402fb9d8ff9507fb6e1238e7ff06f7a5c50ff3df993`
- **Size:** 56,304 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2017-08-23T15:44:32+00:00
- **Imphash:** `N/A`
- **Score:** 18

**Dangerous Imports:**
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `MmMapIoSpaceEx` from `ntoskrnl.exe`
- `MmGetSystemRoutineAddress` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `MmMapLockedPagesSpecifyCache` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 16,955 | 17,408 | 6.26 | R | - | X |
| .rdata | 0x6000 | 2,260 | 2,560 | 4.25 | R | - | - |
| .data | 0x7000 | 256 | 512 | 0.34 | R | W | - |
| .pdata | 0x8000 | 660 | 1,024 | 2.95 | R | - | - |
| PAGE | 0x9000 | 1,716 | 2,048 | 5.44 | R | - | X |
| INIT | 0xa000 | 1,824 | 2,048 | 5.35 | R | - | X |
| .rsrc | 0xb000 | 1,128 | 1,536 | 2.58 | R | - | - |
| .reloc | 0xc000 | 36 | 512 | 0.4 | R | - | - |

**Interesting Strings:**
- `IOCTL_PHYMEM_MAP`
- `IOCTL_PHYMEM_UNMAP`
- `IOCTL_PHYMEM_GETPORT`
- `IOCTL_PHYMEM_SETPORT`
- `IOCTL_PHYMEM_SENDSMI`
- `IOCTL_ENUM_RTKNIC`
- `IOCTL_PHYMEM_GETPCIULONG`
- `IOCTL_PHYMEM_GETEEPROM`
- `IOCTL_PHYMEM_SETEEEPROM`
- `IOCTL_PHYMEM_GETCHANNEL`

**Entry Point Disassembly:**
```asm
0x14000a220: mov qword ptr [rsp + 8], rbx
0x14000a225: push rdi
0x14000a226: sub rsp, 0x20
0x14000a22a: mov rbx, rdx
0x14000a22d: mov rdi, rcx
0x14000a230: call 0x14000a250
0x14000a235: mov rdx, rbx
0x14000a238: mov rcx, rdi
```

---

## rtkiow8x64.sys

- **SHA256:** `082c39fe2e3217004206535e271ebd45c11eb072efde4cc9885b25ba5c39f91d`
- **Size:** 46,944 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2017-08-23T15:43:48+00:00
- **Imphash:** `N/A`
- **Score:** 18

**Dangerous Imports:**
- `MmMapIoSpace` from `ntoskrnl.exe`
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `MmGetSystemRoutineAddress` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `MmMapLockedPagesSpecifyCache` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 16,955 | 17,408 | 6.26 | R | - | X |
| .rdata | 0x6000 | 2,260 | 2,560 | 4.23 | R | - | - |
| .data | 0x7000 | 232 | 512 | 0.34 | R | W | - |
| .pdata | 0x8000 | 660 | 1,024 | 2.93 | R | - | - |
| PAGE | 0x9000 | 1,716 | 2,048 | 5.43 | R | - | X |
| INIT | 0xa000 | 1,822 | 2,048 | 5.35 | R | - | X |
| .rsrc | 0xb000 | 1,120 | 1,536 | 2.58 | R | - | - |
| .reloc | 0xc000 | 36 | 512 | 0.4 | R | - | - |

**Interesting Strings:**
- `IOCTL_PHYMEM_MAP`
- `IOCTL_PHYMEM_UNMAP`
- `IOCTL_PHYMEM_GETPORT`
- `IOCTL_PHYMEM_SETPORT`
- `IOCTL_PHYMEM_SENDSMI`
- `IOCTL_ENUM_RTKNIC`
- `IOCTL_PHYMEM_GETPCIULONG`
- `IOCTL_PHYMEM_GETEEPROM`
- `IOCTL_PHYMEM_SETEEEPROM`
- `IOCTL_PHYMEM_GETCHANNEL`

**Entry Point Disassembly:**
```asm
0x14000a220: mov qword ptr [rsp + 8], rbx
0x14000a225: push rdi
0x14000a226: sub rsp, 0x20
0x14000a22a: mov rbx, rdx
0x14000a22d: mov rdi, rcx
0x14000a230: call 0x14000a250
0x14000a235: mov rdx, rbx
0x14000a238: mov rcx, rdi
```

---

## BSMEMx64.sys

- **SHA256:** `f929bead59e9424ab90427b379dcdd63fbfe0c4fb5e1792e3a1685541cd5ec65`
- **Size:** 29,344 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2012-07-26T09:34:27+00:00
- **Imphash:** `N/A`
- **Score:** 16

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `MmMapIoSpace` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 12,160 | 12,288 | 5.47 | R | - | X |
| .rdata | 0x4000 | 2,628 | 3,072 | 4.35 | R | - | - |
| .data | 0x5000 | 1,144 | 1,024 | 2.07 | R | W | - |
| .pdata | 0x6000 | 384 | 512 | 3.13 | R | - | - |
| PAGE | 0x7000 | 331 | 512 | 3.82 | R | - | X |
| INIT | 0x8000 | 2,012 | 2,048 | 5.43 | R | W | X |
| .rsrc | 0x9000 | 1,032 | 1,536 | 2.4 | R | - | - |
| .reloc | 0xa000 | 248 | 512 | 2.17 | R | - | - |

**Interesting Strings:**
- `DebugPrint logging started`
- `DebugPrint logging ended`
- `DebugPrint: Could not allocate buffer`
- `TargetDeviceRelation`
- `DeviceIoControl: %d bytes written`
- `DeviceIoControl: Control code %x InputLength %d OutputLength %d`
- `PowerDeviceD3`
- `PowerDeviceD2`
- `PowerDeviceD1`
- `PowerDeviceD0`

**Entry Point Disassembly:**
```asm
0x18230: mov rax, qword ptr [rip - 0x2e5f]
0x18237: movabs r9, 0x2b992ddfa232
0x18241: test rax, rax
0x18244: je 0x1824b
0x18246: cmp rax, r9
0x18249: jne 0x1827a
0x1824b: lea r8, [rip - 0x2e7a]
0x18252: movabs rax, 0xfffff78000000320
```

---

## BSMI.sys

- **SHA256:** `59626cac380d8fe0b80a6d4c4406d62ba0683a2f0f68d50ad506ca1b1cf25347`
- **Size:** 17,056 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2012-08-29T08:16:01+00:00
- **Imphash:** `N/A`
- **Score:** 16

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `IoDeleteDevice` from `ntoskrnl.exe`
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `MmGetPhysicalAddress` from `ntoskrnl.exe`
- `MmMapIoSpace` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 492 | 512 | 5.23 | R | - | X |
| .rdata | 0x2000 | 312 | 512 | 2.91 | R | - | - |
| .data | 0x3000 | 272 | 512 | 0.3 | R | W | - |
| .pdata | 0x4000 | 120 | 512 | 1.06 | R | - | - |
| PAGE | 0x5000 | 4,279 | 4,608 | 5.61 | R | - | X |
| INIT | 0x7000 | 1,098 | 1,536 | 4.34 | R | W | X |
| .rsrc | 0x8000 | 888 | 1,024 | 2.88 | R | - | - |

**Interesting Strings:**
- `Enter DeviceIOControl`
- `SmiDDKDeviceIOControl SMI_WRITE `
- `SMI_WRITE pSmiFlash_HOOK->SmiPhysical == 0 MmGetPhysicalAddress LowPart`
- `SMI_WRITE pSmiFlash_HOOK->SmiPhysical == 0 MmGetPhysicalAddress HighPart`
- `SMI_WRITE pSmiFlash_HOOK->SmiPhysical == 0 MmGetPhysicalAddress`
- ` SMI_WRITE asm end `
- `SMI_WRITE MmGetPhysicalAddress LowPart`
- `SMI_WRITE MmGetPhysicalAddress HighPart`
- `SMI_WRITE MmGetPhysicalAddress`
- `SMI_WRITE BiosAddr.LowPart != 0 BiosAddr.HighPart == 0 MmGetPhysicalAddress LowPart`

**Entry Point Disassembly:**
```asm
0x17230: sub rsp, 0x28
0x17234: mov r8, rdx
0x17237: mov r9, rcx
0x1723a: call 0x171d4
0x1723f: mov rdx, r8
0x17242: mov rcx, r9
0x17245: add rsp, 0x28
0x17249: jmp 0x17010
```

---

## BSMIx64.sys

- **SHA256:** `552f70374715e70c4ade591d65177be2539ec60f751223680dfaccb9e0be0ed9`
- **Size:** 16,504 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2012-04-14T06:29:52+00:00
- **Imphash:** `N/A`
- **Score:** 16

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `IoDeleteDevice` from `ntoskrnl.exe`
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `MmGetPhysicalAddress` from `ntoskrnl.exe`
- `MmMapIoSpace` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 492 | 512 | 5.22 | R | - | X |
| .rdata | 0x2000 | 308 | 512 | 2.81 | R | - | - |
| .data | 0x3000 | 272 | 512 | 0.3 | R | W | - |
| .pdata | 0x4000 | 120 | 512 | 1.04 | R | - | - |
| PAGE | 0x5000 | 3,623 | 4,096 | 5.33 | R | - | X |
| INIT | 0x6000 | 1,098 | 1,536 | 4.35 | R | W | X |
| .rsrc | 0x7000 | 888 | 1,024 | 2.88 | R | - | - |

**Interesting Strings:**
- `Enter DeviceIOControl`
- `SmiDDKDeviceIOControl SMI_WRITE `
- `SMI_WRITE MmGetPhysicalAddress LowPart`
- `SMI_WRITE MmGetPhysicalAddress HighPart`
- `SMI_WRITE MmGetPhysicalAddress`
- ` SMI_WRITE asm end `
- `IOCTL_READ_MEMORY BiosAddr.LowPart`
- `IOCTL_READ_MEMORY pMem_Class->Count`
- `IOCTL_GET_PHYSICALADDRESS`
- `IOCTL_GET_PHYSICALADDRESS MmGetPhysicalAddress LowPart`

**Entry Point Disassembly:**
```asm
0x16230: sub rsp, 0x28
0x16234: mov r8, rdx
0x16237: mov r9, rcx
0x1623a: call 0x161d4
0x1623f: mov rdx, r8
0x16242: mov rcx, r9
0x16245: add rsp, 0x28
0x16249: jmp 0x16010
```

---

## HpPortIox64.sys

- **SHA256:** `c5050a2017490fff7aa53c73755982b339ddb0fd7cef2cde32c81bc9834331c5`
- **Size:** 49,176 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2021-04-21T03:22:47+00:00
- **Imphash:** `N/A`
- **Score:** 16

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `MmGetSystemRoutineAddress` from `ntoskrnl.exe`
- `ExAllocatePool` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 23,905 | 24,064 | 5.59 | R | - | X |
| .rdata | 0x7000 | 3,972 | 4,096 | 4.86 | R | - | - |
| .data | 0x8000 | 440 | 512 | 1.13 | R | W | - |
| .pdata | 0x9000 | 624 | 1,024 | 2.76 | R | - | - |
| INIT | 0xa000 | 1,332 | 1,536 | 4.66 | R | W | X |
| .rsrc | 0xb000 | 936 | 1,024 | 3.07 | R | - | - |
| .reloc | 0xc000 | 12 | 512 | 0.08 | R | - | - |

**Interesting Strings:**
- `Cannot resolve ZwQueryInformationProcess`
- `Current ProcessImageFileName: Unknown`
- `Current ProcessImageFileName: %s`
- `d:\sources\hpportio\sys\crypto\cryptostub.c`
- `d:\sources\hpportio\sys\lib\amd64\HpPortIox64.pdb`
- `IoDeleteDevice`
- `IoCreateDevice`
- `RtlVolumeDeviceToDosName`
- `ZwReadFile`
- `RtlCompareMemory`

**Entry Point Disassembly:**
```asm
0x1a064: sub rsp, 0x28
0x1a068: mov r8, rdx
0x1a06b: mov r9, rcx
0x1a06e: call 0x1a008
0x1a073: mov rdx, r8
0x1a076: mov rcx, r9
0x1a079: add rsp, 0x28
0x1a07d: jmp 0x11008
```

---

## cpuz141.sys

- **SHA256:** `ded2927f9a4e64eefd09d0caba78e94f309e3a6292841ae81d5528cab109f95d`
- **Size:** 46,400 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2016-11-22T13:20:59+00:00
- **Imphash:** `N/A`
- **Score:** 16

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `IoDeleteDevice` from `ntoskrnl.exe`
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `MmMapIoSpace` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 14,854 | 15,360 | 6.1 | R | - | X |
| .rdata | 0x5000 | 1,164 | 1,536 | 3.39 | R | - | - |
| .data | 0x6000 | 1,088 | 512 | 0.38 | R | W | - |
| .pdata | 0x7000 | 240 | 512 | 1.95 | R | - | - |
| INIT | 0x8000 | 1,038 | 1,536 | 3.88 | R | W | X |
| .rsrc | 0x9000 | 848 | 1,024 | 2.92 | R | - | - |

**Interesting Strings:**
- `IoBuildDeviceIoControlRequest`
- `IoDeleteDevice`
- `MmUnmapIoSpace`
- `IoGetDeviceObjectPointer`
- `MmMapIoSpace`
- `IoCreateDevice`
- `KeStallExecutionProcessor`

**Entry Point Disassembly:**
```asm
0x18064: sub rsp, 0x28
0x18068: mov r8, rdx
0x1806b: mov r9, rcx
0x1806e: call 0x18008
0x18073: mov rdx, r8
0x18076: mov rcx, r9
0x18079: add rsp, 0x28
0x1807d: jmp 0x11390
```

---

## elbycdio.sys

- **SHA256:** `eea53103e7a5a55dc1df79797395a2a3e96123ebd71cdd2db4b1be80e7b3f02b`
- **Size:** 23,976 bytes
- **Arch:** x86
- **Signed:** Yes
- **Timestamp:** 2009-01-29T22:57:56+00:00
- **Imphash:** `N/A`
- **Score:** 16

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `IofCompleteRequest` from `ntoskrnl.exe`
- `ExAllocatePool` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `ExFreePool` from `ntoskrnl.exe`
- `PsGetCurrentProcessId` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x480 | 10,736 | 10,752 | 6.4 | R | - | X |
| .rdata | 0x2e80 | 1,476 | 1,536 | 6.99 | R | - | - |
| .data | 0x3480 | 4 | 128 | 0.26 | R | W | - |
| INIT | 0x3500 | 1,190 | 1,280 | 5.16 | R | W | X |
| .rsrc | 0x3a00 | 1,240 | 1,280 | 3.25 | R | - | - |
| .reloc | 0x3f00 | 398 | 512 | 4.23 | R | - | - |

**Interesting Strings:**
- `\Device\`
- `ZwReadFile`
- `ZwWriteFile`
- `PsTerminateSystemThread`
- `ZwSetInformationThread`
- `PsCreateSystemThread`
- `PsGetCurrentProcessId`
- `IoDeleteDevice`
- `IoBuildDeviceIoControlRequest`
- `ProbeForRead`

**Entry Point Disassembly:**
```asm
0x13505: mov eax, dword ptr [0x13480]
0x1350a: test eax, eax
0x1350c: mov ecx, 0xbb40e64e
0x13511: je 0x13517
0x13513: cmp eax, ecx
0x13515: jne 0x13530
0x13517: mov eax, dword ptr [0x12f18]
0x1351c: mov eax, dword ptr [eax]
```

---

## BS_I2c64.sys

- **SHA256:** `55fee54c0d0d873724864dc0b2a10b38b7f40300ee9cae4d9baaf8a202c4049a`
- **Size:** 15,408 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2008-06-16T06:45:18+00:00
- **Imphash:** `N/A`
- **Score:** 14

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `MmMapIoSpace` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 2,512 | 2,560 | 5.92 | R | - | X |
| .rdata | 0x2000 | 456 | 512 | 4.09 | R | - | - |
| .data | 0x3000 | 280 | 512 | 0.3 | R | W | - |
| .pdata | 0x4000 | 156 | 512 | 1.3 | R | - | - |
| PAGE | 0x5000 | 170 | 512 | 2.05 | R | - | X |
| INIT | 0x6000 | 1,224 | 1,536 | 4.65 | R | W | X |
| .rsrc | 0x7000 | 1,032 | 1,536 | 2.4 | R | - | - |

**Interesting Strings:**
- `DeviceIoControl: %d bytes written`
- `DeviceIoControl: Control code %x InputLength %d OutputLength %d`
- `Creating device %T`
- `RegistryPath is %T`
- `IoDeleteDevice`
- `IoCreateDevice`
- `MmUnmapIoSpace`
- `MmMapIoSpace`
- `KeRemoveEntryDeviceQueue`

**Entry Point Disassembly:**
```asm
0x16170: mov rax, qword ptr [rip - 0x306f]
0x16177: movabs r9, 0x2b992ddfa232
0x16181: test rax, rax
0x16184: je 0x1618b
0x16186: cmp rax, r9
0x16189: jne 0x161ba
0x1618b: lea r8, [rip - 0x308a]
0x16192: movabs rax, 0xfffff78000000320
```

---

## CorsairLLAccess64.sys

- **SHA256:** `000547560fea0dd4b477eb28bf781ea67bf83c748945ce8923f90fdd14eb7a4b`
- **Size:** 20,696 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2019-10-28T23:19:22+00:00
- **Imphash:** `N/A`
- **Score:** 14

**Dangerous Imports:**
- `MmMapLockedPagesSpecifyCache` from `ntoskrnl.exe`
- `MmMapIoSpace` from `ntoskrnl.exe`
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 3,978 | 4,096 | 6.07 | R | - | X |
| .rdata | 0x2000 | 1,516 | 1,536 | 3.86 | R | - | - |
| .data | 0x3000 | 896 | 512 | 0.28 | R | W | - |
| .pdata | 0x4000 | 252 | 512 | 2.09 | R | - | - |
| INIT | 0x5000 | 1,132 | 1,536 | 4.24 | R | - | X |
| .rsrc | 0x6000 | 1,064 | 1,536 | 2.62 | R | - | - |
| .reloc | 0x7000 | 20 | 512 | 0.22 | R | - | - |

**Interesting Strings:**
- `MmMapLockedPagesSpecifyCache`
- `MmUnmapLockedPages`
- `MmMapIoSpace`
- `MmUnmapIoSpace`
- `IoCreateDevice`
- `IoDeleteDevice`
- `IoGetRequestorProcessId`

**Entry Point Disassembly:**
```asm
0x140005000: mov qword ptr [rsp + 8], rbx
0x140005005: push rdi
0x140005006: sub rsp, 0x20
0x14000500a: mov rbx, rdx
0x14000500d: mov rdi, rcx
0x140005010: call 0x14000502c
0x140005015: mov rdx, rbx
0x140005018: mov rcx, rdi
```

---

## NTIOLib.sys

- **SHA256:** `d8b58f6a89a7618558e37afc360cd772b6731e3ba367f8d58734ecee2244a530`
- **Size:** 11,888 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2011-01-06T03:05:14+00:00
- **Imphash:** `N/A`
- **Score:** 14

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `MmMapIoSpace` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 1,940 | 2,048 | 5.83 | R | - | X |
| .rdata | 0x2000 | 432 | 512 | 3.87 | R | - | - |
| .data | 0x3000 | 276 | 512 | 0.3 | R | W | - |
| .pdata | 0x4000 | 108 | 512 | 0.95 | R | - | - |
| INIT | 0x5000 | 546 | 1,024 | 3.04 | R | W | X |
| .rsrc | 0x6000 | 880 | 1,024 | 2.89 | R | - | - |

**Interesting Strings:**
- `IoDeleteDevice`
- `MmUnmapIoSpace`
- `MmMapIoSpace`
- `IoCreateDevice`

**Entry Point Disassembly:**
```asm
0x15008: mov rax, qword ptr [rip - 0x1f0f]
0x1500f: movabs r9, 0x2b992ddfa232
0x15019: test rax, rax
0x1501c: je 0x15023
0x1501e: cmp rax, r9
0x15021: jne 0x15052
0x15023: lea r8, [rip - 0x1f2a]
0x1502a: movabs rax, 0xfffff78000000320
```

---

## ProcessMonitorDriver.sys

- **SHA256:** `70bcec00c215fe52779700f74e9bd669ff836f594df92381cbfb7ee0568e7a8b`
- **Size:** 35,400 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2025-05-07T12:52:30+00:00
- **Imphash:** `N/A`
- **Score:** 14

**Dangerous Imports:**
- `PsGetCurrentProcessId` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `ZwOpenProcess` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 17,350 | 17,408 | 6.43 | R | - | X |
| .rdata | 0x6000 | 2,192 | 2,560 | 3.52 | R | - | - |
| .data | 0x7000 | 1,416 | 512 | 0.72 | R | W | - |
| .pdata | 0x8000 | 480 | 512 | 3.8 | R | - | - |
| .edata | 0x9000 | 92 | 512 | 1.04 | R | - | - |
| INIT | 0xa000 | 1,514 | 1,536 | 5.07 | R | - | X |
| .rsrc | 0xb000 | 968 | 1,024 | 3.07 | R | - | - |
| .reloc | 0xc000 | 56 | 512 | 0.72 | R | - | - |

**Exports (1):**
- `_debugBootBuffer`

**Interesting Strings:**
- `FindFreeClient(DeviceExtension=0x%p)`
- `[%u:%u.%u]: FindFreeClient(DeviceExtension=0x%p)`
- `FindClient(DeviceExtension=0x%p; ProcessId=0x%p)`
- `[%u:%u.%u]: FindClient(DeviceExtension=0x%p; ProcessId=0x%p)`
- `SetClient(Client=0x%p; ProcessId=0x%p)`
- `[%u:%u.%u]: SetClient(Client=0x%p; ProcessId=0x%p)`
- `RemoveClient(DeviceExtension=0x%p; ProcessId=0x%p)`
- `[%u:%u.%u]: RemoveClient(DeviceExtension=0x%p; ProcessId=0x%p)`
- `ProcessCallback(Process=0x%p; ProcessId=0x%p; CreateInfo=0x%p)`
- `[%u:%u.%u]: ProcessCallback(Process=0x%p; ProcessId=0x%p; CreateInfo=0x%p)`

**Entry Point Disassembly:**
```asm
0x14000a000: mov qword ptr [rsp + 8], rbx
0x14000a005: push rdi
0x14000a006: sub rsp, 0x20
0x14000a00a: mov rbx, rdx
0x14000a00d: mov rdi, rcx
0x14000a010: call 0x14000a02c
0x14000a015: mov rdx, rbx
0x14000a018: mov rcx, rdi
```

---

## ene.sys

- **SHA256:** `175eed7a4c6de9c3156c7ae16ae85c554959ec350f1c8aaa6dfe8c7e99de3347`
- **Size:** 20,992 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2020-05-08T06:07:19+00:00
- **Imphash:** `N/A`
- **Score:** 14

**Dangerous Imports:**
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `ZwMapViewOfSection` from `ntoskrnl.exe`
- `PsGetCurrentProcessId` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 5,298 | 5,632 | 5.98 | R | - | X |
| .rdata | 0x3000 | 1,988 | 2,048 | 5.31 | R | - | - |
| .data | 0x4000 | 32 | 512 | 0.28 | R | W | - |
| .pdata | 0x5000 | 384 | 512 | 3.03 | R | - | - |
| INIT | 0x6000 | 1,156 | 1,536 | 4.24 | R | - | X |
| .reloc | 0x7000 | 20 | 512 | 0.23 | R | - | - |

**Interesting Strings:**
- `BCryptImportKey`
- `BCryptDestroyKey`
- `IoCreateDevice`
- `IoDeleteDevice`
- `ZwMapViewOfSection`
- `ZwUnmapViewOfSection`
- `PsGetCurrentProcessId`

**Entry Point Disassembly:**
```asm
0x140006000: mov qword ptr [rsp + 8], rbx
0x140006005: push rdi
0x140006006: sub rsp, 0x20
0x14000600a: mov rbx, rdx
0x14000600d: mov rdi, rcx
0x140006010: call 0x14000602c
0x140006015: mov rdx, rbx
0x140006018: mov rcx, rdi
```

---

## fb0dbc3b9c897b7571b94fb2203ffb1ac0facfe366b2cb1f91904ea5335018f0.sys

- **SHA256:** `fb0dbc3b9c897b7571b94fb2203ffb1ac0facfe366b2cb1f91904ea5335018f0`
- **Size:** 39,728 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2018-01-30T18:32:16+00:00
- **Imphash:** `N/A`
- **Score:** 14

**Dangerous Imports:**
- `MmMapLockedPages` from `ntoskrnl.exe`
- `MmMapIoSpace` from `ntoskrnl.exe`
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 6,844 | 7,168 | 6.01 | R | - | X |
| .data | 0x3000 | 320 | 512 | 0.18 | R | W | - |
| .pdata | 0x4000 | 192 | 512 | 1.62 | R | - | - |
| .idata | 0x5000 | 1,396 | 1,536 | 4.1 | R | - | - |
| .rsrc | 0x6000 | 880 | 1,024 | 2.86 | R | - | - |
| .reloc | 0x7000 | 12 | 512 | 0.12 | R | - | - |

**Interesting Strings:**
- `MmMapLockedPages`
- `MmMapIoSpace`
- `MmUnmapIoSpace`
- `IoBuildDeviceIoControlRequest`
- `IoCreateDevice`
- `IoDeleteDevice`
- `IoGetRelatedDeviceObject`

**Entry Point Disassembly:**
```asm
0x119d0: mov qword ptr [rsp + 8], rbx
0x119d5: push rdi
0x119d6: sub rsp, 0x40
0x119da: lea rax, [rsp + 0x60]
0x119df: mov r9d, 0x22
0x119e5: mov qword ptr [rsp + 0x30], rax
0x119ea: lea r8, [rip + 0x161f]
0x119f1: mov byte ptr [rsp + 0x28], 0
```

---

## nvflsh64.sys

- **SHA256:** `a899b659b08fbae30b182443be8ffb6a6471c1d0497b52293061754886a937a3`
- **Size:** 15,648 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2014-01-24T06:53:23+00:00
- **Imphash:** `N/A`
- **Score:** 14

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `ZwMapViewOfSection` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 2,352 | 2,560 | 5.46 | R | - | X |
| .rdata | 0x2000 | 520 | 1,024 | 2.44 | R | - | - |
| .data | 0x3000 | 280 | 512 | 0.3 | R | W | - |
| .pdata | 0x4000 | 216 | 512 | 1.76 | R | - | - |
| INIT | 0x5000 | 674 | 1,024 | 3.6 | R | W | X |

**Interesting Strings:**
- `ZwMapViewOfSection`
- `ZwUnmapViewOfSection`
- `IoDeleteDevice`
- `IoCreateDevice`

**Entry Point Disassembly:**
```asm
0x15010: mov rax, qword ptr [rip - 0x1f0f]
0x15017: movabs r9, 0x2b992ddfa232
0x15021: test rax, rax
0x15024: je 0x1502b
0x15026: cmp rax, r9
0x15029: jne 0x1505a
0x1502b: lea r8, [rip - 0x1f2a]
0x15032: movabs rax, 0xfffff78000000320
```

---

## speedfan.sys

- **SHA256:** `22be050955347661685a4343c51f11c7811674e030386d2264cd12ecbf544b7c`
- **Size:** 14,104 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2006-09-24T13:26:48+00:00
- **Imphash:** `N/A`
- **Score:** 14

**Warnings:**
- RWX section: INIT

**Dangerous Imports:**
- `MmUnmapIoSpace` from `ntoskrnl.exe`
- `MmMapIoSpace` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 2,882 | 3,072 | 5.84 | R | - | X |
| .rdata | 0x2000 | 360 | 512 | 3.57 | R | - | - |
| .data | 0x3000 | 272 | 512 | 0.3 | R | W | - |
| .pdata | 0x4000 | 60 | 512 | 0.56 | R | - | - |
| INIT | 0x5000 | 472 | 512 | 4.54 | R | W | X |
| .rsrc | 0x6000 | 1,024 | 1,024 | 3.36 | R | - | - |

**Interesting Strings:**
- `MmUnmapIoSpace`
- `MmMapIoSpace`
- `IoDeleteDevice`
- `IoCreateDevice`

**Entry Point Disassembly:**
```asm
0x15010: mov rax, qword ptr [rip - 0x1f0f]
0x15017: movabs r9, 0x2b992ddfa232
0x15021: test rax, rax
0x15024: je 0x1502b
0x15026: cmp rax, r9
0x15029: jne 0x1505a
0x1502b: lea r8, [rip - 0x1f2a]
0x15032: movabs rax, 0xfffff78000000320
```

---

## 206f27ae820783b7755bca89f83a0fe096dbb510018dd65b63fc80bd20c03261

- **SHA256:** `206f27ae820783b7755bca89f83a0fe096dbb510018dd65b63fc80bd20c03261`
- **Size:** 25,056 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2020-04-27T08:13:11+00:00
- **Imphash:** `N/A`
- **Score:** 12

**Dangerous Imports:**
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `PsGetCurrentProcessId` from `ntoskrnl.exe`
- `PsLookupProcessByProcessId` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 1,838 | 2,048 | 5.48 | R | - | X |
| .rdata | 0x2000 | 1,164 | 1,536 | 2.9 | R | - | - |
| .data | 0x3000 | 16,432 | 512 | 0.28 | R | W | - |
| .pdata | 0x8000 | 204 | 512 | 1.64 | R | - | - |
| INIT | 0x9000 | 914 | 1,024 | 4.96 | R | - | X |
| .rsrc | 0xa000 | 840 | 1,024 | 2.84 | R | - | - |
| .reloc | 0xb000 | 20 | 512 | 0.26 | R | - | - |

**Interesting Strings:**
- `IoCreateDevice`
- `IoDeleteDevice`
- `PsSetCreateProcessNotifyRoutine`
- `IoGetCurrentProcess`
- `PsGetCurrentProcessId`
- `PsGetProcessId`
- `ZwTerminateProcess`
- `PsLookupProcessByProcessId`
- `PsProcessType`

**Entry Point Disassembly:**
```asm
0x140009000: mov qword ptr [rsp + 8], rbx
0x140009005: push rdi
0x140009006: sub rsp, 0x20
0x14000900a: mov rbx, rdx
0x14000900d: mov rdi, rcx
0x140009010: call 0x14000902c
0x140009015: mov rdx, rbx
0x140009018: mov rcx, rdi
```

---

## vmdrv.sys

- **SHA256:** `32cccc4f249499061c0afa18f534c825d01034a1f6815f5506bf4c4ff55d1351`
- **Size:** 46,952 bytes
- **Arch:** x64
- **Signed:** Yes
- **Timestamp:** 2022-02-22T20:12:24+00:00
- **Imphash:** `N/A`
- **Score:** 12

**Dangerous Imports:**
- `ExFreePool` from `ntoskrnl.exe`
- `IofCompleteRequest` from `ntoskrnl.exe`
- `IoCreateDevice` from `ntoskrnl.exe`
- `IoCreateSymbolicLink` from `ntoskrnl.exe`
- `IoDeleteDevice` from `ntoskrnl.exe`
- `ExAllocatePoolWithTag` from `ntoskrnl.exe`

**Sections:**
| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |
|------|-----|-------------|----------|---------|---|---|---|
| .text | 0x1000 | 5,222 | 5,632 | 5.51 | R | - | X |
| .rdata | 0x3000 | 5,184 | 5,632 | 4.42 | R | - | - |
| .data | 0x5000 | 1,836 | 2,048 | 1.8 | R | W | - |
| .pdata | 0x6000 | 1,392 | 1,536 | 4.06 | R | - | - |
| PAGE | 0x7000 | 11,914 | 12,288 | 5.95 | R | - | X |
| INIT | 0xa000 | 1,754 | 2,048 | 4.91 | R | - | X |
| .rsrc | 0xb000 | 1,120 | 1,536 | 2.81 | R | - | - |
| .reloc | 0xc000 | 392 | 512 | 4.5 | R | - | - |

**Interesting Strings:**
- `IoCreateDevice`
- `IoDeleteDevice`
- `PcAddAdapterDevice`
- `PcRegisterSubdevice`
- `PcRegisterPhysicalConnection`
- `PcNewPort`
- `portcls.sys`

**Entry Point Disassembly:**
```asm
0x14000a140: mov qword ptr [rsp + 8], rbx
0x14000a145: push rdi
0x14000a146: sub rsp, 0x20
0x14000a14a: mov rbx, rdx
0x14000a14d: mov rdi, rcx
0x14000a150: call 0x14000a16c
0x14000a155: mov rdx, rbx
0x14000a158: mov rcx, rdi
```

---

*Generated by driver_static_analysis.py*
