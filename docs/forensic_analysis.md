# Forensic Analysis of Obfuscated Malware: A Comprehensive Guide

This document compiles research and techniques for analyzing obfuscated, packed, and evasive malware, with particular attention to APT34-style tradecraft and modern compiler-obfuscated binaries.

---

## Table of Contents

1. [Overview](#overview)
2. [Static Analysis](#static-analysis)
3. [Dynamic Analysis](#dynamic-analysis)
4. [Memory Forensics](#memory-forensics)
5. [PE File Analysis](#pe-file-analysis)
6. [Deobfuscation Techniques](#deobfuscation-techniques)
7. [APT34-Specific Indicators](#apt34-specific-indicators)
8. [Timeline Analysis](#timeline-analysis)
9. [Network Forensics](#network-forensics)
10. [Registry and File System Forensics](#registry-and-file-system-forensics)
11. [Detection Countermeasures](#detection-countermeasures)
12. [Tools and Frameworks](#tools-and-frameworks)

---

## Overview

Malware authors use multiple layers of obfuscation to evade detection. Modern toolchains combine:

- **Packing**: UPX, custom packers (RC4, XOR, LZNT1 compression)
- **Control Flow Obfuscation**: Flattening, bogus blocks, indirect jumps
- **API Obfuscation**: Import table destruction, trampoline code, dynamic resolution
- **String Encryption**: Runtime decryption of strings
- **Anti-Analysis**: Debugger detection, VM detection, sandbox evasion
- **Direct Syscalls**: Hell's Gate, Halo's Gate, Tartarus Gate to bypass hooks
- **Process Injection**: Process hollowing, APC injection, thread hijacking

The forensic challenge is reconstructing the original program from these layers without executing it (statically) or by observing its runtime behavior (dynamically) or by capturing its memory state.

---

## Static Analysis

Static analysis examines the binary without execution. It is the first line of investigation but is often severely limited by obfuscation.

### Initial Triage

- **File Type**: `file`, `pefile`, `exiftool`
- **Entropy Analysis**: High entropy (>7.0) suggests packing or encryption
- **Section Names**: Non-standard names (`.upx0`, `.vmp0`, `.themida`) indicate packers
- **Import Table**: Sparse imports or LoadLibrary/GetProcAddress heavy usage suggests dynamic API resolution
- **Strings**: Look for URLs, IPs, registry keys, file paths, mutex names

### Limitations of Static Analysis

Modern obfuscation breaks static analysis tools:

- **IDA Pro** and **Ghidra** fail to recover indirect jump destinations in control flow flattening
- **Decompilation** produces unreadable pseudo-code with flattened dispatchers
- **String analysis** misses runtime-decrypted strings
- **Import reconstruction** fails when IAT is destroyed or trampolined

Research from Google Cloud's analysis of LummaC2 (2024) confirms that indirect control flow obfuscation thwarts all major binary analysis platforms.

### Advanced Static Techniques

| Technique | Purpose | Tool |
|---|---|---|
| Entropy analysis | Detect packed/encrypted sections | `binwalk`, `pefile` |
| Signature scanning | Identify known packers | `Detect It Easy`, `Exeinfo PE` |
| Control flow graph recovery | Reconstruct function logic | IDA Pro, Ghidra, Binary Ninja |
| Symbolic execution | Reason about code without running | Angr, Triton, Manticore |

---

## Dynamic Analysis

Dynamic analysis executes the malware in a controlled environment to observe behavior.

### Sandboxing

- **Cuckoo Sandbox**: Automated malware analysis with behavioral reports
- **ANY.RUN**: Interactive cloud sandbox
- **Joe Sandbox**: Multi-OS analysis platform

### Anti-Sandbox Evasion

Malware checks for sandbox artifacts:

- CPU core count (< 2 cores = likely VM)
- RAM size (< 2 GB = likely sandbox)
- Hard disk size (< 60 GB = likely VM)
- Presence of VM tools (VMware Tools, VirtualBox Guest Additions)
- Debugger detection (IsDebuggerPresent, CheckRemoteDebuggerPresent)
- Timing checks (RDTSC, Sleep acceleration detection)

### Behavioral Indicators

Monitor for:

- Process creation (especially `cmd.exe`, `powershell.exe`, `wscript.exe`)
- Network connections to rare domains or IPs
- Registry modifications for persistence
- File drops in temporary directories
- DLL injection or process hollowing
- WMI and COM object creation

### API Tracing

Tools like **API Monitor** or **WinDbg** trace API calls:

- `CreateProcess`, `CreateRemoteThread` (injection)
- `VirtualAllocEx`, `WriteProcessMemory` (process manipulation)
- `RegSetValueEx`, `RegCreateKeyEx` (persistence)
- `WSAConnect`, `InternetConnect` (network C2)

---

## Memory Forensics

Memory forensics captures and analyzes the RAM state of an infected system. It is one of the most powerful techniques against obfuscated malware because it bypasses anti-analysis checks and reveals decrypted code, injected payloads, and runtime behavior.

### Why Memory Forensics Works

- Malware must decrypt/unpack itself into memory to execute
- Anti-analysis checks query the OS, but memory forensics reads raw RAM
- Injected code, hollowed processes, and unhooked syscalls are visible in memory
- Network connections, loaded drivers, and open handles persist in RAM

### Key Volatility 3 Plugins

| Plugin | Purpose | Detection Target |
|---|---|---|
| `windows.pslist` | List processes | Unlinked processes, masquerading |
| `windows.psscan` | Scan for processes | Hidden/detached processes |
| `windows.pstree` | Process tree | Anomalous parent-child relationships |
| `windows.malfind` | Find injected code | RWX memory regions, PE headers in private memory |
| `windows.ldrmodules` | Check DLL linking | Unlinked DLLs, memory-only modules |
| `windows.handles` | Enumerate handles | File handles to dropped payloads |
| `windows.svcscan` | List services | Malicious services (drivers) |
| `windows.registry.hivelist` | Registry hives | Persistence keys, recently accessed files |
| `windows.netstat` | Network connections | C2 beacons, unusual ports |
| `windows.driverirp` | Driver IRP hooks | Rootkit-level modifications |
| `windows.callbacks` | System callbacks | Malicious callback registration |
| `windows.unhooked_syscalls` | Detect unhooked ntdll | EDR bypass via DLL unhooking |
| `windows.direct_system_calls` | Find syscall instructions outside ntdll | Hell's Gate / direct syscalls |
| `windows.cloned_processes` | Detect process forking | Dirty Vanity / process hollowing |
| `windows.pebmasquerade` | Detect PEB spoofing | Process name masquerading |

### Detecting EDR Evasion Techniques

Research by Volexity (2024) developed Volatility 3 plugins to detect all known EDR bypass techniques:

1. **Module Unhooking**: Compares syscall implementations across processes. Hooked processes group together; unhooked processes stand out.

2. **Direct Syscalls**: Searches for `syscall; ret` or `int 0x2e` patterns outside `ntdll.dll`, `wow64win.dll`, and `win32u.dll`. Also checks for `mov rax, SSN` preceding the syscall.

3. **Process Cloning (Dirty Vanity)**: Detects processes created via `RtlCreateProcessReflection` by checking if thread start addresses point to `RtlpProcessReflectionStartup`.

4. **PEB Masquerading**: Compares `EPROCESS.ImageFileName` with `PEB.ImagePathName` to detect process name spoofing.

### Memory Dump Analysis Workflow

```bash
# Identify processes
vol3 -f memory.dmp windows.pslist

# Find injected code
vol3 -f memory.dmp windows.malfind

# Check for unlinked DLLs
vol3 -f memory.dmp windows.ldrmodules | grep -i false

# Detect direct syscalls (Hell's Gate)
vol3 -f memory.dmp windows.direct_system_calls

# Check for unhooked ntdll (EDR bypass)
vol3 -f memory.dmp windows.unhooked_syscalls

# Network connections
vol3 -f memory.dmp windows.netstat

# Dump suspicious process
vol3 -f memory.dmp windows.pslist --pid <PID> --dump
```

### PE Reconstruction from Memory

PE-sieve and Volatility 3's `pedump` plugin can reconstruct PE files from memory sections, even when the on-disk file has been deleted or overwritten.

Key insight: the `EPROCESS->SectionObject` field references a cached copy of the original image data for the process's lifetime. This can be extracted via `NtQueryInformationProcess` with `ProcessImageSection` (cross-process requires injection).

---

## PE File Analysis

### Header Analysis

- **DOS Header**: Check `e_magic` == `MZ`, `e_lfanew` points to PE header
- **PE Header**: Check `Machine` (x86 vs x64), `NumberOfSections`, `TimeDateStamp`
- **Optional Header**: `ImageBase`, `EntryPoint`, `SectionAlignment`, `FileAlignment`
- **Data Directories**: Import Table, Export Table, Resource Directory, Relocation Table

### Section Analysis

| Indicator | Suspicion Level |
|---|---|
| Section names like `UPX0`, `UPX1`, `.vmp0`, `.themida` | High (known packer) |
| Executable section with zero raw size | High (packed code) |
| RWX (Read-Write-Execute) permissions | High (self-modifying code) |
| High entropy (>7.0) in code sections | Medium (packed/encrypted) |
| Very low entropy in code sections | Medium (junk data or very small) |
| Writable `.text` section | High (runtime patching) |
| Missing `.reloc` section in DLL | Medium (may be packed) |

### Import Table Analysis

- **Few imports**: Malware dynamically resolves APIs
- **Only `LoadLibraryA` and `GetProcAddress`**: Classic dynamic resolution
- **Missing expected imports**: Packed/obfuscated imports
- **Ordinal-only imports**: Attempt to hide API names

### Export Table Analysis

- Malicious DLLs often export common names (`Start`, `Init`, `DllRegisterServer`) to blend in
- Check for forwarded exports (proxying to legitimate DLLs)

### Rich Header

- Contains compiler and build tool version information
- Can reveal the toolchain used (Visual Studio version, linker version)
- Malware authors sometimes strip or corrupt this header

---

## Deobfuscation Techniques

### Control Flow Deobfuscation

#### Symbolic Backward Slicing

Used by Google Cloud to deobfuscate LummaC2's indirect control flow:

1. Identify the final indirect jump in each dispatcher block
2. Use symbolic execution (Triton engine) to trace which instructions influence the jump destination
3. Extract the backward slice - these are the dispatcher instructions
4. Instructions NOT in the slice are the original program instructions
5. Rebuild the function from original instructions in execution order
6. Fix relocations for jump/call offsets

#### Trace-Informed Compositional Synthesis

Research from UT Austin (2024) proposes:

1. Execute the obfuscated program to collect dynamic traces
2. Infer a control-flow skeleton from the trace
3. Synthesize each basic block independently using the trace as hints
4. Combine synthesized blocks into the deobfuscated program

Achieves 86% accuracy in producing code nearly identical to the original.

### API Deobfuscation

#### Trampoline Code Analysis

Modern packers insert trampoline code to hide API calls:

- **Argument-insensitive**: All APIs route through one trampoline, argument determines the target
- **Argument-sensitive**: Each API has its own trampoline

**Pinicorn approach** (2024):
1. Use Intel Pin (DBI) to execute the packed program and dump memory at OEP candidates
2. Identify trampoline code locations in the memory dump
3. Use Unicorn emulator to execute each trampoline independently
4. Decode the obfuscated API from the trampoline's return address or behavior

### String Decryption

- Identify decryption routines (XOR loops, RC4, AES)
- Use dynamic analysis to capture decrypted strings at runtime
- For simple XOR: brute-force key by looking for printable output
- For RC4: extract the S-box initialization and keystream generation

### Unpacking

**Generic Unpacking Methods**:

1. **ESP Trick**: Set hardware breakpoint on ESP after entry point (stack balance at OEP)
2. **Memory Access Breakpoint**: Break when packed section is written to (unpacking)
3. **API Hooking**: Hook `VirtualProtect`, `WriteProcessMemory` to catch payload extraction
4. **Heuristic OEP Detection**: Look for transitions from high-entropy to normal code

**Automated Unpackers**:

- **Unipacker**: Automated generic unpacker using memory forensics
- **PinDemonium**: Pin-based OEP detection and dumping
- **Pinicorn**: Handles Themida, VMProtect, ASProtect with trampoline analysis

---

## APT34-Specific Indicators

### Known Malware Families

| Family | Language | Purpose |
|---|---|---|
| TONEDEAF | C/C++ | Backdoor with HTTP/DNS C2 |
| VALUEVAULT | Go | Browser credential theft |
| LONGWATCH | C/C++ | Keylogger |
| PICKPOCKET | C/C++ | Browser credential theft |
| Glimpse | PowerShell | DNS-based RAT |
| PoisonFrog | PowerShell | DNS-based RAT |
| Karkoff | .NET | Exchange-based C2 |
| Saitama | .NET | Exchange-based C2 |
| Menorah | .NET | Backdoor with file operations |
| STEALHOOK | .NET | Exchange exfiltration |
| psgfilter | C (DLL) | Password filter credential harvester |

### Network Indicators

- **DNS tunneling**: Subdomain-based C2 with DGA patterns
- **Exchange Web Services (EWS)**: Email-based C2 using legitimate mail servers
- **HTTP C2**: Hardcoded domains like `offlineearthquake[.]com`, `c[.]cdn-edge-akamai[.]com`
- **DNS redirection**: Hijacks victim DNS to resolve attacker-controlled domains

### Host Indicators

- **Scheduled Tasks**: `\UpdateTasks\UpdateTaskHosts`, `\UpdateTasks\JavaUpdatesTasksHosts`
- **Persistence Registry**: `HKLM\Software\Microsoft\Windows\CurrentVersion\Run`
- **File Paths**:
  - `C:\Users\Public\Public\atag[0-9]{4}[A-Z]{2}`
  - `C:\Users\Public\Public\dUpdater.ps1`
  - `C:\Users\Public\Public\hUpdated.ps1`
  - `C:\Users\Public\Public\UpdateTask.vbs`
  - `C:\ProgramData\Windows\Microsoft\java`
  - `<%ProgramData%>\WindowsSoftwareDevices\DevicesTemp\ngb`
- **Mutex**: `115CF7F6-69B4-49EE-B453-BAF00531AC52` (Menorah)

### YARA Rules

```yara
rule APT34_PowerShell_Malware {
    meta:
        description = "Detects APT34 PowerShell malware"
        author = "Florian Roth"
    strings:
        $x1 = "= \"http://\" + [System.Net.Dns]::GetHostAddresses(\"" ascii
        $x2 = "$t = get-wmiobject Win32_ComputerSystemProduct | Select-Object -ExpandProperty UUID" ascii
        $x3 = "schtasks /create /F /ru SYSTEM /sc minute /mo" ascii
        $x4 = "Powershell.exe -exec bypass -file ${global:$address1}" ascii
    condition:
        1 of them
}

rule APT34_VALUEVAULT {
    meta:
        description = "APT34 ValueVault credential stealer"
    strings:
        $fsociety = "fsociety.dat" ascii
        $gobuild = "Go build ID: " ascii
        $str1 = "main.Decrypt" ascii
        $str2 = "main.CrackChromeBased" ascii
        $str3 = "main.CrackMozila" ascii
    condition:
        uint16(0) == 0x5a4d and ($fsociety or 5 of ($str*))
}
```

### MITRE ATT&CK Mapping

| Technique ID | Technique Name | APT34 Usage |
|---|---|---|
| T1071.004 | DNS Application Layer Protocol | DNS tunneling (ISMAgent, DNSpionage) |
| T1071.003 | Mail Protocols | Exchange/EWS C2 |
| T1137.004 | Outlook Home Page | Persistence on Exchange |
| T1505.003 | Web Shell | IIS backdoors (TwoFace, RGDoor, HighShell) |
| T1556.002 | Password Filter DLL | Domain credential harvesting |
| T1053.005 | Scheduled Task | Re-running backdoors |
| T1078.002 | Valid Accounts | Operating with stolen credentials |
| T1003.001 | LSASS Memory | Credential dumping with Mimikatz |
| T1555.003 | Credentials from Web Browsers | Browser credential theft |
| T1047 | Windows Management Instrumentation | Lateral movement |
| T1021.001 | Remote Desktop Protocol | Lateral movement |
| T1105 | Ingress Tool Transfer | Moving tools into environment |
| T1567 | Exfiltration Over Web Service | Data staging |
| T1048.003 | Exfiltration Over DNS | DNS tunneling exfiltration |
| T1562.004 | Disable or Modify System Firewall | Clearing tunnel paths |

---

## Timeline Analysis

Timeline analysis reconstructs the sequence of events during an incident.

### Windows Timeline Sources

| Source | Forensic Value |
|---|---|
| `$MFT` | File creation, modification, access times |
| `$LogFile` | NTFS journal of all file system operations |
| `$UsnJrnl` | Update sequence number journal (change tracking) |
| Registry key LastWrite times | Persistence installation, configuration changes |
| Event Logs | Process creation, logon events, PowerShell execution |
| Prefetch | Program execution times and frequency |
| ShimCache | First execution of programs |
| Amcache | Program installation and execution metadata |
| SRUM | System Resource Usage Monitor (30 days of process/network data) |
| Jump Lists | User access to files and applications |
| LNK Files | Shortcut metadata including target paths |

### Building a Timeline

```bash
# Using plaso/log2timeline
log2timeline.py --storage-file timeline.plaso disk_image.raw

# Using MFTECmd
MFTECmd.exe -f "$MFT" --csv timeline.csv

# Using fls (Sleuth Kit)
fls -r -m / disk_image.raw > bodyfile
mactime -b bodyfile -d > timeline.csv
```

### Key Timeline Indicators

- **Clustered file creation**: Multiple files created in rapid succession (dropper activity)
- **Prefetch gaps**: Malware that deletes its own prefetch file
- **Registry changes before network activity**: Persistence established before C2
- **Unusual execution times**: Programs running outside business hours
- **Service creation followed by process injection**: Rootkit deployment pattern

---

## Network Forensics

### DNS Analysis

APT34 heavily uses DNS tunneling:

- Monitor for high volume of DNS queries to rare domains
- Look for subdomain patterns encoding data (DGA)
- Check for TXT records carrying commands
- Monitor query frequency (APT34 queries every 50ms in some variants)

### PCAP Analysis

- **Wireshark**: Manual packet inspection
- **Zeek (Bro)**: Automated protocol analysis and connection logging
- **Suricata**: IDS/IPS with rule-based detection
- **NetworkMiner**: Extract files and credentials from PCAP

### Indicators in Network Traffic

| Indicator | Description |
|---|---|
| HTTP User-Agent anomalies | Custom or outdated UA strings |
| beaconing patterns | Regular intervals between connections |
| large DNS TXT records | Data exfiltration via DNS |
| HTTPS with self-signed certs | C2 over encrypted channels |
| SMTP to external domains | Email-based exfiltration |
| EWS API calls from non-Outlook clients | Exchange abuse |

### C2 Communication Patterns

APT34 C2 characteristics:

- **HTTP**: GET/POST to hardcoded domains with encoded parameters
- **DNS**: A/TXT record queries with encoded subdomains
- **Email**: EWS API calls to send stolen data as attachments
- **Steganography**: RDAT embeds data in BMP images attached to emails

---

## Registry and File System Forensics

### Registry Persistence Locations

| Registry Path | Purpose |
|---|---|
| `HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Run` | System-wide persistence |
| `HKCU\SOFTWARE\Microsoft\Windows\CurrentVersion\Run` | User-level persistence |
| `HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon\Userinit` | Logon persistence |
| `HKLM\SYSTEM\CurrentControlSet\Services` | Service-based persistence |
| `HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders` | Startup folder path |
| `HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Notification Packages` | Password filter DLL |

### File System Artifacts

| Artifact | Location | Significance |
|---|---|---|
| Prefetch files | `C:\Windows\Prefetch` | Program execution evidence |
| ShimCache | Registry | First execution tracking |
| Amcache | `C:\Windows\AppCompat\Programs\Amcache.hve` | Program metadata |
| SRUM database | `C:\Windows\System32\sru\SRUDB.dat` | 30-day resource usage |
| Jump Lists | `C:\Users\<user>\AppData\Roaming\Microsoft\Windows\Recent\AutomaticDestinations` | File access history |
| LNK files | User profile | Shortcut targets and times |
| Web cache | Browser profiles | C2 URLs, downloaded payloads |
| Event logs | `C:\Windows\System32\winevt\Logs` | System activity |
| PowerShell history | `C:\Users\<user>\AppData\Roaming\Microsoft\Windows\PowerShell\PSReadLine\ConsoleHost_history.txt` | Command history |
| WMI repository | `C:\Windows\System32\wbem\Repository` | WMI persistence |

### Deleted File Recovery

- **$MFT analysis**: Track file deletions via `$MFT` record flags
- **Carving**: Use PhotoRec, Foremost, or Scalpel to recover deleted files
- **Shadow Copies**: Check Volume Shadow Copy Service (VSS) for previous versions
- **Recycle Bin**: Parse `$I` and `$R` files in Recycle Bin

---

## Detection Countermeasures

### Detecting Compiler-Obfuscated Binaries

Modern obfuscation toolchains (like the JOCKY pipeline) present unique detection challenges:

| Obfuscation Layer | Detection Approach |
|---|---|
| MLIR string encryption | Memory dump string extraction |
| Constant obfuscation | Dynamic analysis with symbolic execution |
| Import obfuscation | API call tracing in sandbox |
| Control flow flattening | Memory forensics + CFG recovery |
| Symbol obfuscation | String entropy analysis, import table reconstruction |
| UPX packing | UPX signature detection, entropy analysis |
| Hell's Gate syscalls | Volatility `direct_system_calls` plugin |
| VM bytecode | Emulation + pattern matching on VM handlers |
| Anti-analysis runtime | Behavioral analysis in sandbox |

### EDR Detection Strategies

1. **Kernel callbacks**: Register callbacks for process creation, thread creation, image loading, registry operations
2. **ETW (Event Tracing for Windows)**: Subscribe to system-wide events
3. **AMSI (Anti-Malware Scan Interface)**: Scan scripts and memory buffers
4. **HVCI (Hypervisor-Protected Code Integrity)**: Prevent unsigned kernel drivers
5. **Credential Guard**: Isolate LSASS in virtual secure mode

### Memory Forensics as Ultimate Fallback

When EDR is bypassed:

- Capture memory dump before shutdown
- Use Volatility 3 to detect:
  - Unlinked processes
  - Injected code (malfind)
  - Unhooked system calls
  - Direct syscalls outside ntdll
  - PEB masquerading
  - Cloned processes

---

## Tools and Frameworks

### Static Analysis

| Tool | Purpose |
|---|---|
| IDA Pro | Disassembly and decompilation |
| Ghidra | Open-source disassembly and decompilation |
| Binary Ninja | Modern reverse engineering platform |
| Radare2 | Open-source reverse engineering framework |
| Cutter | GUI for Radare2 |
| Detect It Easy (DIE) | Packer and compiler identification |
| Exeinfo PE | Packer detection |
| PEStudio | Static PE analysis |
| pestudio | PE file indicators |
| Manalyze | PE file analysis framework |
| capstone | Disassembly engine |
| yara | Pattern matching and signature scanning |

### Dynamic Analysis

| Tool | Purpose |
|---|---|
| Cuckoo Sandbox | Automated malware analysis |
| ANY.RUN | Interactive cloud sandbox |
| Joe Sandbox | Multi-OS analysis |
| VxStream ( Falcon Sandbox) | Automated analysis |
| API Monitor | API call tracing |
| Process Monitor (ProcMon) | File, registry, process monitoring |
| Process Hacker / System Informer | Advanced task manager |
| x64dbg / OllyDbg | User-mode debugger |
| WinDbg | Kernel and user-mode debugger |
| Intel Pin | Dynamic binary instrumentation |
| DynamoRIO | Dynamic instrumentation framework |

### Memory Forensics

| Tool | Purpose |
|---|---|
| Volatility 3 | Memory forensics framework |
| Rekall | Memory forensics framework |
| MemProcFS | Memory process file system |
| PE-sieve | Process memory scanner and dumper |
| Hollows_Hunter | Process scanner for implants |
| Moneta | Memory analysis for detection |
| VolMemLyzer | Memory feature extraction for ML |

### Deobfuscation

| Tool | Purpose |
|---|---|
| Triton | Symbolic execution and backward slicing |
| Angr | Binary analysis platform |
| Manticore | Symbolic execution tool |
| Unicorn | CPU emulator |
| de4dot | .NET deobfuscator |
| Unipacker | Automated generic unpacker |
| PinDemonium | Pin-based OEP detection |
| Pinicorn | Advanced unpacker with trampoline analysis |
| API-Xray | Import table reconstruction |

### Network Analysis

| Tool | Purpose |
|---|---|
| Wireshark | Packet analysis |
| Zeek (Bro) | Network protocol analysis |
| Suricata | IDS/IPS |
| NetworkMiner | Network forensic analysis |
| tshark | Command-line packet analysis |
| tcpdump | Packet capture |

### Timeline and File System

| Tool | Purpose |
|---|---|
| log2timeline/plaso | Timeline generation |
| MFTECmd | MFT parser |
| Sleuth Kit | File system analysis |
| Autopsy | Digital forensics platform |
| Registry Explorer | Registry analysis |
| RegRipper | Registry data extraction |
| EZ Tools (Eric Zimmerman) | Windows forensics tools suite |

---

## References

1. Google Cloud Threat Intelligence. "LummaC2: Obfuscation Through Indirect Control Flow." September 2024.
2. Raubitzek et al. "Obfuscation undercover: Unraveling the impact of obfuscation layering on structural code patterns." Journal of Information Security and Applications, 2024.
3. Mariano et al. "Control-Flow Deobfuscation using Trace-Informed Compositional Program Synthesis." OOPSLA, 2024.
4. HarfangLab. "Unpacking the unpleasant FIN7 gift: PackXOR." September 2024.
5. meekochii. "Dissecting the xz-utils Backdoor." April 2024.
6. Lee et al. "Pinicorn: Towards Automated Dynamic Analysis for Unpacking 32-Bit PE Malware." Electronics, 2024.
7. Xu et al. "LLM4Decompile: Decompiling Binary Code with Large Language Models." EMNLP, 2024.
8. Hu et al. "Now You See Me, Now You Don't: Using LLMs to Obfuscate Malicious JavaScript." Unit 42, 2024.
9. Volexity. "Defeating EDR Evading Malware with Memory." DEF CON 32, 2024.
10. NSFOCUS. "APT34 Event Analysis Report." 2019.
11. FireEye. "Hard Pass: Declining APT34's Invite to Join Their Professional Network." 2019.
12. Intruvent. "APT34-OilRig Threat Hunting Guide." 2024.
13. Neo23x0/signature-base. YARA rules for APT34.
14. deadbits/yara-rules. APT34_VALUEVAULT YARA rule.
15. SecurityScorecard. "A detailed analysis of the Menorah malware used by APT34." 2024.
16. Trend Micro. "New APT34 Malware Targets The Middle East." 2023.
17. Treadstone 71. "APT34 Intelligence Analysis Brief."
18. KELA Cyber. "APT34 (OilRig): Espionage on Your Infrastructure." 2026.
19. diversenok's blog. "Reconstructing Executables Part 1: Between Files and Memory." 2024.
20. Volatility Foundation. Volatility 3 documentation and plugins.

---

*This document is for educational and defensive security research purposes only.*
