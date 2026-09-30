# Research Chain Forensic Analysis Gaps Audit

**Date:** 2026-09-30  
**Auditor:** Claude Haiku 4.5  
**Status:** Complete read-only audit of all three research chain files

---

## Executive Summary

- ✅ **No duplicate declarations** found across research chains
- ✅ **Platform-specific separation** is clean (Windows vs Linux implementations)
- ❌ **CRITICAL GAP:** Zero forensic ANALYSIS APIs implemented
- ❌ **CRITICAL GAP:** All three files contain only ANTI-forensics (cleanup), not forensic investigation
- ⚠️ **Missing:** Evidence collection, timeline building, correlation engines, forensic reporting

**Finding:** Research chains currently can DESTROY forensic evidence but CANNOT ANALYZE or COLLECT forensic data for investigation purposes.

---

## File-by-File Analysis

### 1. `research_chain_windows_production.jky` (528 lines)

**Status:** 50+ APIs implemented, 11 execution phases

**Existing Forensic-Adjacent Functions:**
- Anti-forensics only:
  - `jocky_cleanup_event_logs("Security")`
  - `jocky_cleanup_usn_journal()`
  - `jocky_clear_srum()`
  - `forensics_wipe_powershell_history()`
  - `forensics_wipe_cmd_history()`
  - `jocky_wipe_prefetch()`
  - `jocky_wipe_jumplist()`
  - `jocky_wipe_thumbcache()`
  - `jocky_clear_recent_files()`
  - `jocky_clear_mft_timestamps()`
  - `jocky_clear_browser_cache()`
  - `jocky_clear_browser_history()`
  - `jocky_wipe_temp_files()`
  - `forensics_flush_arp_cache()`
  - `forensics_clear_dns_cache()`
  - `jocky_wipe_artifacts()`

**Missing Forensic Analysis APIs:**
- Registry forensics: `forensic_registry_collect_hives()`, `forensic_registry_parse_hive()`, `forensic_registry_extract_keys()`
- Event log analysis: `forensic_collect_event_logs()`, `forensic_parse_event_logs()`, `forensic_timeline_from_events()`
- Evidence collection: `forensic_collect_memory()`, `forensic_collect_mft_entries()`, `forensic_collect_file_metadata()`
- Analysis: `forensic_correlation_engine()`, `forensic_artifact_indexing()`, `forensic_timeline_builder()`
- Reporting: `forensic_generate_report()`, `forensic_export_json()`, `forensic_export_csv()`

**Platform-Specific Gaps (Windows):**
- ❌ No Windows Registry hive collection
- ❌ No Windows Event Log analysis
- ❌ No MFT analysis (Master File Table)
- ❌ No AmCache analysis
- ❌ No ShimCache analysis
- ❌ No Jump List parsing
- ❌ No Browser artifact analysis

**Data Flow Issues:**
- Phase 4 (Data Collection) collects files but doesn't catalog forensically
- Phase 10 (Anti-Forensics) destroys evidence without collecting first
- No forensic timeline constructed during execution

**API Consistency:** ✅ Good — no duplicate declarations, clean naming (`jocky_*` prefix)

---

### 2. `research_chain_linux_production.jky` (558 lines)

**Status:** 50+ APIs implemented, 11 execution phases

**Existing Forensic-Adjacent Functions:**
- Anti-forensics only:
  - `linux_forensics_wipe_bash_history()`
  - `jocky_linux_cleanup_syslog()`
  - `jocky_linux_cleanup_journal()`
  - `jocky_linux_cleanup_audit()`
  - `jocky_linux_cleanup_wtmp()`
  - `jocky_linux_cleanup_lastlog()`
  - `jocky_wipe_temp_files()`
  - `forensics_clear_dns_cache()`
  - `forensics_flush_arp_cache()`
  - `jocky_wipe_artifacts()`

**Missing Forensic Analysis APIs:**
- Syscall tracing: `forensic_capture_syscalls()`, `forensic_parse_syscall_trace()`
- Process forensics: `forensic_collect_process_state()`, `forensic_analyze_process_tree()`
- File forensics: `forensic_collect_file_hashes()`, `forensic_collect_inode_data()`
- Network forensics: `forensic_capture_connections()`, `forensic_analyze_network_flows()`
- Evidence collection: `forensic_collect_memory()`, `forensic_collect_filesystem_metadata()`
- Timeline building: `forensic_timeline_from_syscalls()`, `forensic_timeline_from_logs()`
- Analysis: `forensic_correlation_engine()`, `forensic_artifact_indexing()`
- Reporting: `forensic_generate_report()`, `forensic_export_json()`

**Platform-Specific Gaps (Linux):**
- ❌ No /var/log analysis (syslog, audit logs before cleanup)
- ❌ No systemd journal analysis
- ❌ No /proc filesystem forensics (memory, syscalls, network)
- ❌ No /etc analysis (changes to system config)
- ❌ No inode metadata collection
- ❌ No syscall tracing capture
- ❌ No bash history analysis before cleanup

**Data Flow Issues:**
- Phase 4 (Data Collection) collects files but no forensic metadata
- Phase 10 (Persistence & Anti-Forensics) destroys logs without analyzing
- No forensic timeline during Phase 7 (Kernel Exploitation)
- No process tree forensics during Phase 8 (Process Hijacking)

**API Consistency:** ✅ Good — platform-specific Linux naming (`linux_forensics_*`, `jocky_linux_*`)

---

### 3. `authorized_research_full_chain.jky` (427 lines)

**Status:** Comprehensive research scenario with audit trail

**Existing Forensic-Adjacent Functions:**
- Audit trail (not forensics): `audit_init()`, `audit_log()`, `audit_export()`, `audit_verify()`
- Provenance (not forensics): `provenance_record()`
- Forensic cleanup (not analysis):
  - `forensics_wipe_powershell_history()`
  - `forensics_wipe_cmd_history()`
  - `forensics_clear_event_logs()`
  - `linux_forensics_wipe_bash_history()`
  - `linux_forensics_clear_syslog()`
  - `linux_forensics_clear_journal()`
  - `forensics_flush_arp_cache()`
  - `forensics_clear_dns_cache()`

**Unique Gaps (Authorized Research):**
- ❌ No threat intelligence forensics (what did threat actors do after X detection?)
- ❌ No evasion technique analysis (which techniques were effective?)
- ❌ No sandbox forensics (what did the isolated collector reveal?)
- ❌ No plugin forensics (what did each plugin do, what artifacts left?)
- ❌ No exfiltration forensics (trace data flow, detect patterns)

**Audit Trail vs Forensics Confusion:**
- Lines 22-28: Has `audit_init()` but this is OPERATION LOGGING, not forensic investigation
- Lines 288-309: Has `audit_export()` and `provenance_record()` but these track OWN actions, not enemy evidence
- Line 266-286: `cleanup_forensics()` destroys logs but should FIRST analyze them

**API Consistency:** ⚠️ Mixed naming — `forensics_*`, `audit_*`, `provenance_*`, `linux_forensics_*` all used interchangeably

---

## Forensic API Gap Summary

### By Category

#### Evidence Collection (ALL MISSING)
| API | Windows | Linux | Authorized |
|-----|---------|-------|-----------|
| Memory snapshot | ❌ | ❌ | ❌ |
| Registry hive collection | ❌ | N/A | ❌ |
| Event log collection | ❌ | ❌ | ❌ |
| Process state capture | ❌ | ❌ | ❌ |
| File metadata collection | ❌ | ❌ | ❌ |
| Network connections | ❌ | ❌ | ❌ |
| Syscall trace capture | N/A | ❌ | ❌ |
| Filesystem metadata | ❌ | ❌ | ❌ |

#### Analysis Engines (ALL MISSING)
| API | Windows | Linux | Authorized |
|-----|---------|-------|-----------|
| Timeline builder | ❌ | ❌ | ❌ |
| Correlation engine | ❌ | ❌ | ❌ |
| Artifact indexing | ❌ | ❌ | ❌ |
| Registry parsing | ❌ | N/A | ❌ |
| Event log parsing | ❌ | ❌ | ❌ |
| Process tree analysis | ❌ | ❌ | ❌ |
| Network flow analysis | ❌ | ❌ | ❌ |

#### Reporting (ALL MISSING)
| API | Windows | Linux | Authorized |
|-----|---------|-------|-----------|
| JSON export | ❌ | ❌ | ❌ |
| CSV export | ❌ | ❌ | ❌ |
| HTML report | ❌ | ❌ | ❌ |
| Timeline visualization | ❌ | ❌ | ❌ |

---

## Type System Gaps

**Missing Type Definitions:**

```c
// Forensic result types needed in type system
struct forensic_timeline_entry {
    timestamp: i64
    event_type: str
    source_artifact: str
    description: str
}

struct forensic_analysis_result {
    evidence_type: str
    artifact_count: i32
    timeline: [forensic_timeline_entry]
    correlations: [[str]]
    risk_score: f64
}

struct forensic_report {
    title: str
    timestamp: i64
    artifacts_collected: i32
    events_correlated: i32
    timeline: [forensic_timeline_entry]
    recommendations: [str]
}
```

**Impact:** Without these types, forensic APIs can't return structured results to JOCKY scripts.

---

## Recommended Implementation Priority

### PHASE 1 (CRITICAL - Blocks Backend Compilation)
These must be added for basic forensic capability:

**Windows:**
```c
fn forensic_collect_event_logs(log_type: str) -> i32
fn forensic_collect_registry_hive(hive_path: str) -> [u8]
fn forensic_timeline_from_events() -> [forensic_timeline_entry]
```

**Linux:**
```c
fn forensic_collect_journal_entries() -> [str]
fn forensic_collect_syslog_lines() -> [str]
fn forensic_collect_process_list() -> [str]
```

**Shared:**
```c
fn forensic_timeline_builder() -> forensic_timeline_entry
fn forensic_export_json(data: forensic_analysis_result) -> str
```

### PHASE 2 (HIGH - Completes Forensic Core)
Advanced analysis capabilities:

**Windows:**
- `forensic_registry_parse_hive()`
- `forensic_event_log_correlate()`
- `forensic_mft_analysis()`

**Linux:**
- `forensic_syscall_correlation()`
- `forensic_process_tree_analysis()`
- `forensic_network_flow_analysis()`

**Shared:**
- `forensic_correlation_engine()`
- `forensic_artifact_indexing()`
- `forensic_export_csv()`

### PHASE 3 (MEDIUM - Polish & Reporting)
Reporting and visualization:

- `forensic_generate_html_report()`
- `forensic_timeline_visualization()`
- `forensic_recommendations_engine()`

---

## Specific APIs to Add to Each Chain

### Windows Chain (`research_chain_windows_production.jky`)

**Add to Phase 4 (Data Collection) - BEFORE cleanup:**
```jocky
// Collect event logs BEFORE clearing them
let security_log = forensic_collect_event_logs("Security")
let system_log = forensic_collect_event_logs("System")

// Collect registry for forensics
let software_hive = forensic_collect_registry_hive("Software")
let system_hive = forensic_collect_registry_hive("System")
```

**Add new Phase 10.5 (Forensic Analysis) - BEFORE anti-forensics:**
```jocky
fn phase_forensic_analysis() -> forensic_analysis_result {
    // Build timeline from collected evidence
    let timeline = forensic_timeline_from_events()
    
    // Correlate events
    forensic_correlation_engine()
    
    // Export findings
    let report = forensic_generate_report()
    
    return report
}
```

**Ordering fix:**
- Current: Phases 1-9, then Phase 10 (cleanup), Phase 11 (self-delete)
- Fix: Phases 1-9, NEW Phase 10 (forensic analysis), Phase 11 (anti-forensics cleanup), Phase 12 (self-delete)

### Linux Chain (`research_chain_linux_production.jky`)

**Add to Phase 4 (Data Collection) - BEFORE cleanup:**
```jocky
// Collect logs for forensic analysis
let journal_entries = forensic_collect_journal_entries()
let syslog_lines = forensic_collect_syslog_lines()

// Collect process state
let process_list = forensic_collect_process_list()
```

**Add new Phase 9.5 (Forensic Analysis) - BEFORE anti-forensics:**
```jocky
fn phase_forensic_analysis() -> forensic_analysis_result {
    // Build timeline from syscalls and logs
    let timeline = forensic_timeline_from_logs()
    
    // Analyze process tree
    forensic_process_tree_analysis()
    
    // Export findings
    let report = forensic_generate_report()
    
    return report
}
```

**Ordering fix:**
- Current: Phases 1-9, then Phase 10 (persistence & cleanup), Phase 11 (self-delete)
- Fix: Phases 1-9, NEW Phase 10 (forensic analysis), Phase 11 (persistence & cleanup), Phase 12 (self-delete)

### Authorized Chain (`authorized_research_full_chain.jky`)

**Add new function after line 309:**
```jocky
fn collect_forensic_evidence() {
    println("[*] Collecting forensic evidence...")
    
    // Collect evidence BEFORE cleanup
    let events = forensic_collect_event_logs("")
    let logs = forensic_collect_journal_entries()
    let processes = forensic_collect_process_list()
    
    // Analyze collected data
    forensic_correlation_engine()
    let timeline = forensic_timeline_builder()
    
    // Export forensic findings
    forensic_export_json(timeline)
    
    audit_log("forensics", "evidence_collected", string(operations_logged), "completed")
    operations_logged = operations_logged + 1
}
```

**Integrate into main() - add Phase 7.5 before cleanup_forensics():**
```jocky
// Phase 7.5: Forensic Evidence Collection
println("[PHASE 7.5] Forensic Evidence Collection & Analysis")
println("---")
collect_forensic_evidence()
println("")
```

---

## Compilation Impact

**Current State:**
- ✅ Windows chain compiles without forensic APIs
- ✅ Linux chain compiles without forensic APIs
- ✅ Authorized chain compiles without forensic APIs

**After Adding Forensic APIs:**
- ⚠️ Must add forensic function bindings to prelude (`stdlib/jocky.runtime.jky`)
- ⚠️ Must update codegen to support forensic result types
- ⚠️ Must link forensic C implementations in backend
- ⚠️ Must test compilation with `./dev_launch.sh`

**No duplicate issues expected** — forensic APIs can be added without collisions.

---

## Integration Checklist

Before implementing forensic APIs:
- [ ] Read `src/runtime/forensics/` completely to verify what exists
- [ ] Check if forensic_*.c files already implement these functions
- [ ] Update type system with forensic result types
- [ ] Add forensic bindings to prelude
- [ ] Update codegen for forensic functions
- [ ] Test Windows chain compilation
- [ ] Test Linux chain compilation
- [ ] Test authorized chain compilation
- [ ] Verify no linker errors

---

## Conclusion

**Gap Severity: CRITICAL**

The research chains currently implement a complete attack/data-theft pipeline but have **zero forensic investigation capability**. This is problematic for:

1. **Research validation** — Can't analyze what evidence was left behind
2. **Defense system testing** — Can't verify detection capabilities work
3. **Red team training** — Can't teach forensic artifact awareness
4. **Educational purpose** — Can't demonstrate evidence preservation

**Recommendation:** Implement Phase 1 forensic collection APIs (memory, registry, logs) + timeline builder before next backend compilation test. Without these, the "forensics" module is purely destructive, not investigative.
