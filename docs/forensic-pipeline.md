# JOCKY Defensive Forensic Pipeline

## Problem Context

SIH 26148 requires a programming language framework that enables **computer and network forensic analysis without triggering security solutions**. The core challenge: legitimate forensic tools are often blocked by aggressive AV/EDR on target systems. JOCKY solves this by compiling polymorphic forensic analyzers that evade signature-based detection while performing authorized defensive investigation.

## What This Pipeline Does

The JOCKY forensic pipeline compiles source code into **polymorphic forensic analyzers** - programs that:

1. Collect volatile and non-volatile evidence from target systems
2. Evade AV/EDR detection through compiler-based polymorphism
3. Report findings to a central management console
4. Leave minimal forensic footprint on the target

Each compilation produces a **unique binary** with different hashes, control flow, and import tables, preventing adversaries or security products from fingerprinting and blocking the forensic tool.

## Pipeline Architecture

```
┌─────────────────┐     ┌─────────────────────────┐     ┌──────────────────────────┐
│  JOCKY Source   │────▶│   Forensic Compiler     │────▶│  Polymorphic Analyzer    │
│  (.jky script)  │     │  (LLVM + MLIR + Poly)   │     │  (unique per build)      │
│                 │     │                         │     │                          │
│ - process enum  │     │ - Token diversification │     │ - Different hash         │
│ - memory dump   │     │ - Control flow flatten  │     │ - Modified imports       │
│ - net capture   │     │ - String encryption     │     │ - Altered entry point    │
│ - artifact coll │     │ - Import obfuscation    │     │ - Unique CFG             │
└─────────────────┘     │ - Anti-analysis runtime │     └────────────┬─────────────┘
                        └─────────────────────────┘                  │
                                                                     │
                                                                     ▼
                        ┌─────────────────────────────────────────────────────────┐
                        │              Forensic Execution Environment             │
                        │  ┌─────────────┐  ┌─────────────┐  ┌─────────────────┐ │
                        │  │   Target    │  │   Memory    │  │   Evidence      │ │
                        │  │   System    │  │   Analysis  │  │   Collection    │ │
                        │  │  (AV/EDR)   │  │   Engine    │  │   & Reporting   │ │
                        │  └─────────────┘  └─────────────┘  └─────────────────┘ │
                        └─────────────────────────────────────────────────────────┘
                                                                     │
                                                                     ▼
                        ┌─────────────────────────────────────────────────────────┐
                        │              Central Management Console                 │
                        │  - Multi-system orchestration                           │
                        │  - Evidence aggregation                                 │
                        │  - Timeline correlation                                 │
                        │  - CDN/Cloud fronted reporting                          │
                        └─────────────────────────────────────────────────────────┘
```

## Forensic Capabilities

### 1. Host-Based Forensics

**Process Enumeration**
- List running processes with parent-child relationships
- Detect hidden/unlinked processes (DKOM attacks)
- Identify process injection artifacts (hollowed processes, RWX regions)
- Cross-reference with known-bad process names

**Memory Acquisition**
- Capture physical RAM without third-party drivers
- Use direct syscalls to avoid user-mode hooks
- Compress and encrypt memory dumps in-stream
- Target specific processes (LSASS for credential extraction)

**Registry Analysis**
- Extract persistence keys (Run, RunOnce, Services)
- Parse Amcache.hve for execution history
- Identify shimcache/scdb for first-execution tracking
- Detect password filter DLLs (APT34-style credential harvesting)

**Artifact Collection**
- Event logs (Windows) / syslog (Linux)
- Prefetch files and shimcache
- Browser history, cookies, cached credentials
- Recent documents, jump lists, lnk files
- USB device connection history

### 2. Memory Forensics

**In-Memory Analysis Without Dumping**
- Scan process memory for injected code (MZ headers in private memory)
- Detect API hooks (IAT/EAT modifications)
- Find unlinked DLLs (memory-resident modules not in PEB)
- Identify RWX memory regions (common in shellcode)

**Runtime Detection**
- Kernel callback table inspection (PsSetCreateProcessNotifyRoutine)
- SSDT/GDT/IDT hook detection
- BYOVD driver identification (known vulnerable signed drivers)
- ETW provider tampering detection

### 3. Network Forensics

**Connection State**
- Active TCP/UDP connections with process mapping
- Listening ports and associated services
- DNS cache entries
- ARP table and routing table

**Traffic Capture**
- Raw socket packet capture (requires privileges)
- NetFlow-style connection metadata
- TLS certificate extraction and JA3 fingerprinting
- Beaconing detection (regular-interval connections)

**Network Artifacts**
- Browser proxy settings
- WPAD configuration
- Hosts file modifications
- Firewall rules and exceptions

### 4. File System Forensics

**Timeline Analysis**
- $MFT parsing (Windows) / inode analysis (Linux)
- USN Journal examination
- $LogFile transaction analysis
- Deleted file recovery via carving

**Signature Scanning**
- YARA rule application to files and memory
- Hash-based known-bad detection
- Entropy analysis for packed/encrypted files
- Embedded object extraction (OLE, PDF scripts)

## Polymorphic Compilation Pipeline

### Phase 1: Source Parsing

JOCKY source files define forensic tasks using a domain-specific language:

```jocky
// Example: Process enumeration with injection detection
forensic process_enum {
    target: all_processes
    detect: [injection, hollowing, unlinked]
    output: encrypted_json
}

// Example: Memory acquisition
forensic memory_dump {
    target: specific_process("lsass.exe")
    method: direct_syscall
    compression: lz4
    encryption: aes256_gcm
}
```

### Phase 2: IR Generation

The JOCKY frontend generates LLVM IR with forensic-specific intrinsics:

```llvm
; Intrinsic for process enumeration
%process_list = call %ProcessList @jocky.enumerate.processes(i32 %flags)

; Intrinsic for memory read via direct syscall
%memory = call i8* @jocky.memory.read(i64 %pid, i64 %addr, i64 %size)

; Intrinsic for encrypted output
call void @jocky.output.encrypted(i8* %data, i64 %len, i8* %key)
```

### Phase 3: Polymorphic Obfuscation

Each build introduces unique variations:

**Token Diversification**
- Variable and function names randomized per compilation
- String literals encrypted with unique keys
- Different encryption algorithms selected randomly (AES, ChaCha, RC4)

**Control Flow Mutation**
- Basic blocks reordered with opaque predicates
- Dispatcher-based flattening with unique state machines per build
- Junk code insertion at varying densities

**Import Table Randomization**
- APIs resolved dynamically via PEB walking instead of static imports
- Different subsets of APIs imported directly vs dynamically per build
- Import order shuffled

**Binary Structure Variation**
- Section names randomized (except required ones)
- Section ordering permuted
- Padding and alignment varied
- Entry point obfuscated with trampoline code

### Phase 4: Anti-Analysis Runtime

Embedded runtime detects and evades analysis environments:

**Sandbox Evasion**
- CPU core count check (< 2 = likely sandbox)
- RAM size check (< 2GB = likely VM)
- Hypervisor detection (CPUID leaf 0x40000000)
- Timing checks (RDTSC, Sleep acceleration)

**EDR Evasion**
- Direct syscalls instead of hooked APIs
- API unhooking via fresh ntdll copy from disk
- ETW patch removal
- AMSI bypass for in-memory scanning

**Stealth Techniques**
- Process injection into legitimate processes (when authorized)
- Memory-only execution (no disk artifacts)
- Self-deletion after execution
- Encrypted C2 channels over legitimate cloud APIs

### Phase 5: Output Generation

The final binary is a unique forensic analyzer with:
- **Unique SHA256**: Every build produces a different hash
- **No compiler fingerprints**: No MSVC/GCC/Rich header artifacts
- **Minimal imports**: Only critical APIs imported directly
- **Encrypted strings**: All sensitive strings decrypted at runtime
- **Flattened CFG**: Control flow resistant to static decompilation

## Central Management Console

### Multi-System Orchestration

```
┌─────────────────┐
│  JOCKY Console  │
│                 │
│ - Deploy agents │
│ - Collect data  │
│ - Correlate IOCs│
│ - Build reports │
└────────┬────────┘
         │ HTTPS / CDN fronted
         ▼
┌─────────────────┐     ┌─────────────────┐     ┌─────────────────┐
│  Target Host 1  │     │  Target Host 2  │     │  Target Host N  │
│  (JOCKY agent)  │     │  (JOCKY agent)  │     │  (JOCKY agent)  │
│                 │     │                 │     │                 │
│ - Live response │     │ - Live response │     │ - Live response │
│ - Memory dump   │     │ - Memory dump   │     │ - Memory dump   │
│ - Artifact coll │     │ - Artifact coll │     │ - Artifact coll │
└─────────────────┘     └─────────────────┘     └─────────────────┘
```

### Reporting Channels

1. **Direct HTTPS**: TLS-encrypted to console (blocked by some firewalls)
2. **CDN Fronting**: Azure Front Door, CloudFront, Fastly (blends with legit traffic)
3. **DNS Tunneling**: Slow but rarely blocked (TXT record data exfiltration)
4. **Cloud APIs**: OneDrive, Dropbox, GitHub Gists (using stolen/legitimate accounts)
5. **Email**: Exchange Web Services (blends with corporate mail flow)

### Evidence Processing

Collected evidence is:
1. **Encrypted** at the agent using per-session keys
2. **Compressed** to reduce bandwidth
3. **Chunked** for large memory dumps
4. **Correlated** with MITRE ATT&CK framework
5. **Timeline-merged** across multiple systems
6. **IOC-extracted** for defensive action

## Operational Security

### Authorization Model

JOCKY is strictly defensive and requires:
- Explicit written authorization for target systems
- Chain of custody documentation
- Audit logging of all operations
- Time-bounded deployment windows
- Automatic agent expiration and self-destruction

### Dual-Use Mitigation

To prevent misuse:
1. **Hardware binding**: Agents tied to specific target hardware fingerprints
2. **Certificate pinning**: Console certificates embedded at compile time
3. **Dead man's switch**: Agents stop functioning if not re-authorized within 24 hours
4. **Watermarking**: Each binary contains traceable authorship markers
5. **Kill switch**: Console can remotely terminate all deployed agents

## Implementation Roadmap

### Phase 1: Core Language (Completed)
- [x] Lexer and parser for JOCKY syntax
- [x] LLVM IR code generation
- [x] C compilation pipeline (compile C through JOCKY)

### Phase 2: Obfuscation Engine (Completed)
- [x] Control flow flattening
- [x] String encryption via MLIR
- [x] Import obfuscation
- [x] Symbol obfuscation
- [x] Anti-analysis runtime

### Phase 3: Forensic Intrinsics (In Progress)
- [ ] Process enumeration intrinsics
- [ ] Memory read/write via direct syscalls
- [ ] Registry access intrinsics
- [ ] Network socket enumeration
- [ ] File system traversal with filtering

### Phase 4: Polymorphic Engine (In Progress)
- [ ] Token diversification (randomized names)
- [ ] Per-build encryption key generation
- [ ] Import table randomization
- [ ] Section ordering permutation
- [ ] CI/CD integration for continuous builds

### Phase 5: Management Console (Planned)
- [ ] Web-based console for agent deployment
- [ ] Evidence aggregation and correlation
- [ ] Timeline visualization
- [ ] YARA/Sigma rule integration
- [ ] CDN-fronted reporting channels

## Example: Complete Forensic Script

```jocky
// threat_hunt.jky - Hunt for APT34-style persistence

forensic hunt_apt34 {
    // Phase 1: Collect volatile data
    volatile {
        processes: enumerate_with_injection_detection()
        connections: active_tcp_connections()
        memory_regions: scan_for_rwx()
    }
    
    // Phase 2: Collect persistence artifacts
    persistence {
        registry: [
            "HKLM\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
            "HKLM\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Notification Packages"
        ]
        scheduled_tasks: all_tasks()
        services: suspicious_services()
    }
    
    // Phase 3: Check for APT34 indicators
    ioc_check {
        yara_rules: ["apt34_password_filter", "apt34_exchange_abuse"]
        file_paths: [
            "C:\\Users\\Public\\Public\\dUpdater.ps1",
            "C:\\ProgramData\\Windows\\Microsoft\\java"
        ]
        mutex_names: ["115CF7F6-69B4-49EE-B453-BAF00531AC52"]
    }
    
    // Phase 4: Report findings
    output {
        format: encrypted_json
        destination: console_cdn_fronted
        compression: lz4
    }
}
```

## Testing and Validation

### Detection Evasion Testing

Before deployment, each build is tested against:
1. **VirusTotal**: Upload and verify detection rate < 5/70
2. **Local AV**: Scan with Windows Defender, Kaspersky, etc.
3. **EDR Simulation**: Run in CrowdStrike/SentinelOne test environments
4. **Behavioral Analysis**: Verify no suspicious API call sequences

### Forensic Accuracy Testing

Validate evidence collection against:
1. **Known malware samples**: Ensure detection of Cobalt Strike, Meterpreter, etc.
2. **Red team exercises**: Compare JOCKY findings with manual analysis
3. **Memory forensics**: Cross-check with Volatility 3 on same dumps
4. **Network forensics**: Validate PCAP reconstruction against Wireshark

## References

- `docs/forensic_analysis.md` - Research on forensic techniques and tools
- `docs/forensic_use_cases.md` - Detailed forensic workflows from SIH docs
- `docs/problem_statement_analysis.md` - Original SIH 26148 requirements
- `docs/technical_architecture.md` - Compiler and obfuscation design
