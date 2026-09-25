# Defensive Forensic Analysis — Architecture & Plan

---

## 1. Architecture Overview

The defensive side is a separate subsystem that shares the runtime's driver arsenal and control panel but runs its own pipeline.

```
┌──────────────────────────────────────────────────────────────────┐
│  FORENSIC SUBSYSTEM                                              │
├──────────────────────────────────────────────────────────────────┤
│                                                                  │
│  ┌────────────────┐   ┌────────────────┐   ┌────────────────┐    │
│  │ DATA SOURCE    │   │ ARTIFACT       │   │ PROVENANCE     │    │
│  │ PLUGINS        │──▶│ PARSERS        │──▶│ ENGINE         │    │
│  │                │   │ (sandboxed)    │   │                │    │
│  │ - Process      │   │                │   │ - source       │    │
│  │ - File         │   │ - PE parser    │   │ - transform    │    │
│  │ - Network      │   │ - Registry     │   │ - output       │    │
│  │ - Registry     │   │ - EVTX         │   │ - hash chain   │    │
│  │ - Memory       │   │ - Prefetch     │   │                │    │
│  └────────────────┘   │ - MFT          │   └───────┬────────┘    │
│                       └────────────────┘           │             │
│                                                    ▼             │
│  ┌────────────────┐   ┌────────────────┐   ┌────────────────┐    │
│  │ AUDIT LOG      │◀──│ ANALYSIS       │◀──│ EVIDENCE       │    │
│  │ (hash-chained) │   │ ENGINE         │   │ STORE          │    │
│  │                │   │                │   │                │    │
│  │ Every action   │   │ - Timeline     │   │ - JSON         │    │
│  │ logged with    │   │ - IOC extract  │   │ - SQLite       │    │
│  │ prev hash      │   │ - Correlation  │   │ - Artifacts    │    │
│  └───────┬────────┘   └───────┬────────┘   └────────────────┘    │
│          │                    │                                  │
│          ▼                    ▼                                  │
│  ┌────────────────┐   ┌────────────────┐                         │
│  │ OUTPUT         │   │ REPORT         │                         │
│  │ PLUGINS        │   │ STORE          │                         │
│  │                │   │                │                         │
│  │ - STIX 2.1     │   │ - JSON report  │                         │
│  │ - Sigma        │   │ - STIX bundle  │                         │
│  │ - SIEM forward │   │ - Sigma rules  │                         │
│  └────────────────┘   └────────────────┘                         │
│                                                                  │
│  SHARED WITH OFFENSIVE SIDE:                                     │
│  - Driver arsenal (for privileged collection)                    │
│  - Control panel (new "Forensics" tab)                           │
│  - Capability permission system                                  │
│                                                                  │
└──────────────────────────────────────────────────────────────────┘
```

---

## 2. Data Flow (End-to-End)

```
[1] ANALYST RUNS COLLECTION
        │
        ▼
[2] DATA SOURCE PLUGINS EXECUTE
    - Loaded dynamically from plugins/collectors/
    - Each returns raw artifacts (bytes + metadata)
        │
        ▼
[3] ARTIFACT PARSERS RUN (SANDBOXED)
    - Each artifact is routed to the appropriate parser
    - Parsers run in a separate process with restricted token
    - Parsed output is structured JSON
        │
        ▼
[4] PROVENANCE ENGINE RECORDS
    - For each parsed artifact, records:
      source (plugin) → transform (parser) → output (structured data)
    - Appends to evidence store
        │
        ▼
[5] ANALYSIS ENGINE PROCESSES
    - Builds timeline from all timestamped events
    - Extracts IOCs (hashes, IPs, domains, paths, regkeys)
    - Correlates events (e.g., process X spawned Y at time T)
        │
        ▼
[6] AUDIT LOG APPENDS
    - Every action (collection, parse, analysis) is logged
    - Each entry: { timestamp, action, actor, input_hash, output_hash, prev_hash, entry_hash }
    - Hash chain ensures tamper-evidence
        │
        ▼
[7] OUTPUT PLUGINS GENERATE
    - STIX 2.1 bundle
    - Sigma detection rules
    - SIEM forward (HTTP to Splunk/Elastic)
        │
        ▼
[8] REPORT STORE PERSISTS
    - JSON report (human-readable)
    - STIX bundle (machine-readable)
    - Evidence archive (encrypted, for chain of custody)
```

---

## 3. Module Breakdown

### 3.1 Data Source Plugins

Each plugin implements a simple interface:

```c
typedef struct {
    const char* name;
    const char* version;
    artifact_list_t* (*collect)(const char* target, void* config);
} data_source_plugin_t;
```

| Plugin | What It Collects | Method |
|--------|------------------|--------|
| `process_collector` | Running processes, command lines, parent PIDs, loaded modules | `NtQuerySystemInformation`, `EnumProcesses` |
| `file_collector` | Recent files, executables, temp files, startup folder contents | `FindFirstFile`, `USN Journal`, `$MFT` |
| `network_collector` | Active connections, listening ports, DNS cache | `GetExtendedTcpTable`, `GetExtendedUdpTable`, `ipconfig /displaydns` |
| `registry_collector` | Run keys, services, scheduled tasks, recent docs | `RegEnumKeyEx`, `RegQueryValueEx` |
| `memory_collector` | Process memory regions, loaded DLLs, heaps | `ReadProcessMemory` (requires driver for cross-process) |

**Output format:** Each artifact is a tuple:
```
{
  "source": "process_collector",
  "timestamp": "2026-09-25T14:30:00Z",
  "artifact_type": "process",
  "raw": "<bytes>",
  "metadata": { "pid": 1234, "name": "evil.exe", "parent_pid": 5678 }
}
```

---

### 3.2 Artifact Parsers (Sandboxed)

Each parser handles one artifact type. **Sandboxing is required** because parsers may process malicious data.

| Parser | Input | Output |
|--------|-------|--------|
| `pe_parser` | PE file bytes | Sections, imports, exports, signatures, compile timestamp, hashes |
| `registry_parser` | Registry hive bytes | Keys, values, timestamps, deletion markers |
| `evtx_parser` | Event log bytes | Events, timestamps, event IDs, subjects |
| `prefetch_parser` | Prefetch file | Executable name, run count, last run time, loaded DLLs |
| `mft_parser` | $MFT bytes | File records, timestamps (SI, FN), deleted entries |
| `network_parser` | PCAP or NetFlow | Connections, protocols, endpoints, payload hashes |

**Sandboxing implementation:**
- Each parser runs in a child process via `CreateProcessAsUser` with a restricted token.
- Token has: `SeChangeNotifyPrivilege` only (no admin, no debug).
- Parser runs in a **job object** with CPU time limit (e.g., 10 seconds).
- Parser output is written to a pipe, not a file.
- If parser crashes or times out, it's killed and logged — no impact on the main process.

**Plugin format:** Native `.dll` files loaded via `LoadLibrary`.

```c
typedef struct {
    const char* name;
    const char* version;
    const char* artifact_type;  // matches the "artifact_type" field
    parsed_artifact_t* (*parse)(const uint8_t* data, size_t len);
} artifact_parser_t;
```

---

### 3.3 Provenance Engine

**Purpose:** For every piece of evidence, record where it came from, what was done to it, and what the output was.

**Data model:**
```json
{
  "evidence_id": "ev-001",
  "source": { "plugin": "process_collector", "target": "WIN11-VM", "timestamp": "..." },
  "transform": { "parser": "pe_parser", "version": "1.0", "duration_ms": 12 },
  "output": { "type": "pe_analysis", "hash": "sha256:...", "data_ref": "store://ev-001" },
  "chain": { "prev_hash": "sha256:...", "entry_hash": "sha256:..." }
}
```

**Why it matters:** If a judge asks "how do I know this artifact wasn't tampered with?", the provenance chain answers it.

---

### 3.4 Analysis Engine

**Three functions:**

**A. Timeline Reconstruction**
- Collect all timestamped events from parsed artifacts.
- Sort by timestamp.
- Output: chronological list of events with source, type, and details.

**B. IOC Extraction**
- Scan parsed data for IOCs:
  - Hashes (MD5, SHA-1, SHA-256) from PE parser.
  - IPs and domains from network parser.
  - File paths, registry keys, service names.
  - Command lines and process names.
- Output: IOC list with type, value, source, confidence.

**C. Correlation**
- Link related events:
  - Process spawned by parent.
  - Network connection initiated by process.
  - File written by process.
  - Registry key modified by process.
- Output: correlation graph (adjacency list).

---

### 3.5 Audit Log (Hash-Chained)

**Every action is logged.** This is non-negotiable for a forensic tool.

**Entry format:**
```json
{
  "index": 42,
  "timestamp": "2026-09-25T14:30:01.123Z",
  "actor": "analyst@jocky",
  "action": "parse_artifact",
  "input_hash": "sha256:abc...",
  "output_hash": "sha256:def...",
  "prev_entry_hash": "sha256:xyz...",
  "entry_hash": "sha256:42abc..."
}
```

**`entry_hash` = SHA-256(index || timestamp || actor || action || input_hash || output_hash || prev_entry_hash)**

**Verification:** Anyone can recompute the chain from index 0 to the end and confirm no entry was tampered with.

**Storage:** Append-only JSON Lines file (`.jsonl`). Each line is one entry. Trivial to parse, trivial to verify.

---

### 3.6 Output Plugins

| Plugin | Format | Purpose |
|--------|--------|---------|
| `stix_output` | STIX 2.1 bundle | Shareable threat intelligence |
| `sigma_output` | Sigma rules (YAML) | Detection rules for SIEM |
| `siem_forwarder` | HTTP POST | Forward to Splunk/Elastic/Chronicle |
| `json_output` | JSON | Human-readable report |
| `html_output` | HTML | Analyst-friendly report with timeline view |

**STIX mapping example:**
```
Parsed process      → STIX Process object
Parsed file         → STIX File object
Parsed network conn → STIX NetworkTraffic object
IOC (hash)          → STIX Indicator with pattern
Correlation         → STIX Relationship
```

**Sigma mapping example:**
```yaml
title: Suspicious process spawned by office application
detection:
  parent:
    Image|endswith: 'winword.exe'
  child:
    Image|endswith: 'powershell.exe'
  condition: parent and child
```

---

### 3.7 Evidence & Report Store

**Storage layout:**
```
/var/jocky/forensics/
├── evidence/
│   ├── ev-001.raw          # original bytes (compressed)
│   ├── ev-001.parsed.json  # parsed output
│   └── ev-001.prov.json    # provenance entry
├── audit/
│   └── audit.jsonl         # hash-chained log
├── reports/
│   ├── report-2026-09-25.json
│   ├── report-2026-09-25.stix.json
│   └── report-2026-09-25.sigma.yaml
└── index.db                # SQLite: evidence_id → metadata
```

---

### 3.8 Capability Permissions

**Static table in each plugin's manifest:**
```yaml
name: "pe_parser"
version: "1.0"
capabilities:
  - read_artifact
  - write_report
permissions:
  filesystem: "read_only"
  network: "none"
  registry: "none"
```

**Enforcement:** Before a plugin runs, the runtime checks its manifest against the operator's role. If the plugin requests a capability the operator doesn't have, the plugin is rejected.

---

### 3.9 Operator Roles

Two roles, enforced by the control panel:

| Role | Can Do | Cannot Do |
|------|--------|-----------|
| **Operator** | Run offensive pipeline, deploy payloads, use all kernel primitives | Cannot modify reports or audit logs |
| **Analyst** | Run collection, parsing, analysis, generate reports | Cannot deploy payloads or use offensive primitives |

**Implementation:** Simple session tokens in the control panel backend. No MFA, no OAuth. Just a role field in the session.

---

## 4. Plan (10 Days)

| Day | Task | Owner |
|-----|------|-------|
| **1** | Define plugin interfaces (data source, artifact parser, output). Define evidence and provenance JSON schemas. | Person 3 |
| **2** | Build `process_collector` and `file_collector`. Test on VM. | Person 3 |
| **3** | Build `network_collector` and `registry_collector`. | Person 3 |
| **4** | Build `pe_parser` and `registry_parser` with sandboxing (child process + job object). | Person 3 |
| **5** | Build `evtx_parser`, `prefetch_parser`, `mft_parser`. | Person 3 |
| **6** | Build provenance engine (record source → transform → output). Build hash-chained audit log. | Person 2 |
| **7** | Build analysis engine: timeline reconstruction + IOC extraction. | Person 2 |
| **8** | Build correlation engine + output plugins (`stix_output`, `sigma_output`, `json_output`). | Person 2 |
| **9** | Build report store (SQLite index + file store). Add "Forensics" tab to control panel. | You |
| **10** | End-to-end test: collect → parse → analyze → report → verify audit chain. | All |

---

## 5. What This Buys You

**For the demo:**
- Second demo track: "Forensic analysis of a compromised VM."
- Analyst role opens the control panel, runs a collection, watches the timeline build, sees the STIX report generated.
- Verify the audit hash chain live — show that no log entry was tampered with.

**For the paper:**
- A complete dual-use narrative: "The same driver arsenal that enables offensive bypass also powers defensive forensic collection."
- A provenance model that is defensible in court.
- STIX/Sigma output that integrates with existing tooling.

**For the judges:**
- Proof that the tool is not just an offensive weapon, but a dual-use platform.
- Demonstrable tamper-evidence.
- Standards-compliant output (STIX 2.1, Sigma).

---

## 6. What's Still Out of Scope

- Merkle trees (hash chains suffice).
- Provenance graph visualization (table format is enough).
- MFA / RBAC with approvals.
- SIEM-specific connectors (generic HTTP forward only).
- SLSA-compliant builds.
- Code-signed plugins (hash verification only).
- Distributed collection (single VM only).

These go in the paper's "Future Work" section.

---

## Summary

| Component | Days | Owner |
|-----------|------|-------|
| Plugin interfaces + schemas | 1 | Person 3 |
| Collectors (4) | 2 | Person 3 |
| Parsers (6) + sandboxing | 3 | Person 3 |
| Provenance + audit log | 1 | Person 2 |
| Analysis + correlation | 1.5 | Person 2 |
| Output plugins | 0.5 | Person 2 |
| Report store + control panel tab | 1 | You |
| **Total** | **10 days** | |

This gives you a complete forensic subsystem that is distinct from the offensive side, integrates with the same control panel, and satisfies the dual-use requirement of the problem statement.