# Prelude Bindings Audit

**Date:** 2026-09-30  
**Audit Scope:** `stdlib/jocky.runtime.jky` vs `src/runtime/` implementations  
**Status:** ⚠️ CRITICAL GAPS FOUND

---

## Executive Summary

| Metric | Count |
|--------|-------|
| **APIs in prelude** | 71 |
| **APIs documented in RUNTIME_API.md** | 72 |
| **Missing from prelude (RUNTIME_API)** | 1 |
| **Forensic functions in C headers** | 38+ |
| **Forensic functions bound in prelude** | 0 |
| **Forensic header files** | 7 |
| **Gap severity** | 🔴 CRITICAL |

**Finding:** Forensic pipeline is completely unbound. Main runtime is 98% complete (1 API missing).

---

## Current Prelude Bindings (71 APIs)

### Audit & Provenance (8)
- ✅ `audit_init`
- ✅ `audit_log`
- ✅ `audit_verify`
- ✅ `audit_export`
- ✅ `provenance_record`
- ❌ `jocky_alloc` (in RUNTIME_API, not in prelude by this name)
- ⚠️ Partially bound — some helper functions missing

### BYOVD Driver Selection (4)
- ✅ `byovd_get_os_version`
- ✅ `byovd_select_best_driver`
- ✅ `byovd_load_driver`
- ✅ `byovd_unload_driver`

### Plugin System (4)
- ✅ `plugin_load`
- ✅ `plugin_run`
- ✅ `plugin_unload`
- ✅ `plugin_list`

### Sandbox APIs (6)
- ✅ `sandbox_spawn`
- ✅ `sandbox_wait`
- ✅ `sandbox_kill`
- ✅ `sandbox_set_limits`
- ✅ `sandbox_monitor`
- ✅ `sandbox_export_trace`

### Linux Kernel Module (LKM) (5)
- ✅ `lkm_load`
- ✅ `lkm_unload`
- ✅ `lkm_hook_syscall`
- ✅ `lkm_unhook_syscall`
- ✅ `lkm_get_syscall_table`

### eBPF Program Loading (6)
- ✅ `ebpf_load`
- ✅ `ebpf_attach`
- ✅ `ebpf_detach`
- ✅ `ebpf_map_update`
- ✅ `ebpf_map_lookup`
- ✅ `ebpf_unload`

### AI Evasion Engine (7)
- ✅ `ai_init`
- ✅ `ai_collect_telemetry`
- ✅ `ai_score_threat`
- ✅ `ai_recommend_strategy`
- ✅ `ai_generate_mutation`
- ✅ `ai_get_current_risk`
- ✅ `ai_get_statistics`

### EDRChoker (2)
- ✅ `edrhoker_detect`
- ✅ `edrhoker_throttle`
- ✅ `edrhoker_restore`

### Blindside (2)
- ✅ `blindside_unhook_ntdll`
- ✅ `blindside_create_debug_child`

### Fence2Pwn (2)
- ✅ `fence2pwn_detect_kfence`
- ✅ `fence2pwn_exploit`

### BTR Reforged (5)
- ✅ `btr_load_driver`
- ✅ `btr_disable_notifications`
- ✅ `btr_mask_module`
- ✅ `btr_delete_file`
- ✅ `btr_kill_process`

### PatchGuard Peekaboo (3)
- ✅ `patchguard_hide_process`
- ✅ `patchguard_unhide_process`
- ✅ `patchguard_hidden_count`

### Anti-Forensics Windows (7)
- ✅ `forensics_wipe_powershell_history`
- ✅ `forensics_wipe_cmd_history`
- ✅ `forensics_clear_event_logs`
- ✅ `forensics_flush_arp_cache`
- ✅ `forensics_clear_dns_cache`
- ✅ `forensics_clear_usn_journal`
- ✅ `forensics_cleanup_all`

### Anti-Forensics Linux (6)
- ✅ `linux_forensics_wipe_bash_history`
- ✅ `linux_forensics_wipe_zsh_history`
- ✅ `linux_forensics_clear_syslog`
- ✅ `linux_forensics_clear_audit_log`
- ✅ `linux_forensics_clear_journal`
- ✅ `linux_forensics_cleanup_all`

### Exfiltration (5)
- ✅ `exfil_discord_webhook`
- ✅ `exfil_telegram_bot`
- ✅ `exfil_github_gist`
- ✅ `exfil_dns_tunnel`
- ✅ `exfil_underminr_cdn`

---

## Missing from Main RUNTIME_API (1 gap)

**Function:** Memory allocation wrapper  
**Location:** RUNTIME_API.md section 8 (helpers)  
**Missing from prelude:** ❌ `jocky_alloc()` or `jocky_malloc()`  
**Type Signature:** Should be `jocky_alloc(size: i32) -> i8*`  
**Impact:** LOW — Users can use language-built-in allocation  
**Priority:** LOW

---

## Forensic Pipeline — COMPLETELY UNBOUND (38+ gaps)

### Status
- ✅ Forensic implementations exist in `src/runtime/forensics/`
- ❌ **ZERO forensic APIs are exposed in prelude**
- ❌ Codegen cannot call forensic functions (not in prelude = not callable)
- ❌ Research chains cannot use forensic APIs (compilation will fail)

### Forensic Headers (7 files)
1. **forensic_engine.h** — Core engine API
2. **forensic_types.h** — Type definitions (no functions)
3. **analysis.h** — Analysis functions
4. **provenance.h** — Provenance tracking
5. **capabilities.h** — Permission system
6. **evidence_store.h** — Evidence storage
7. **audit_log.h** — Audit logging

### Forensic Functions by Category

#### Engine Control (7 functions)
- `forensic_engine_create()` — **NOT BOUND**
- `forensic_engine_destroy()` — **NOT BOUND**
- `forensic_engine_register_default_plugins()` — **NOT BOUND**
- `forensic_engine_run_collection()` — **NOT BOUND**
- `forensic_engine_run_analysis()` — **NOT BOUND**
- `forensic_engine_generate_outputs()` — **NOT BOUND**
- `forensic_engine_run_full_pipeline()` — **NOT BOUND**
- `forensic_engine_config_default()` — **NOT BOUND**

**Priority:** 🔴 CRITICAL — Core to forensic pipeline

#### Artifact Management (6 functions)
- `forensic_artifact_list_create()` — **NOT BOUND**
- `forensic_artifact_list_destroy()` — **NOT BOUND**
- `forensic_artifact_list_add()` — **NOT BOUND**
- `forensic_artifact_list_add_deep()` — **NOT BOUND**
- `forensic_bytes_create()` — **NOT BOUND**
- `forensic_bytes_destroy()` — **NOT BOUND**

**Priority:** 🔴 CRITICAL — Required for evidence collection

#### Metadata Operations (4 functions)
- `forensic_metadata_create()` — **NOT BOUND**
- `forensic_metadata_destroy()` — **NOT BOUND**
- `forensic_metadata_add()` — **NOT BOUND**
- `forensic_metadata_get()` — **NOT BOUND**

**Priority:** 🔴 CRITICAL — Evidence annotation

#### Analysis Functions (6 functions)
- `forensic_analysis_create()` — **NOT BOUND**
- `forensic_analysis_destroy()` — **NOT BOUND**
- `forensic_analysis_build_timeline()` — **NOT BOUND**
- `forensic_analysis_extract_iocs()` — **NOT BOUND**
- `forensic_analysis_correlate()` — **NOT BOUND**
- `forensic_analysis_run()` — **NOT BOUND**

**Priority:** 🔴 CRITICAL — Core to analysis stage

#### Provenance Tracking (6 functions)
- `forensic_provenance_create()` — **NOT BOUND**
- `forensic_provenance_destroy()` — **NOT BOUND**
- `forensic_provenance_record()` — **NOT BOUND**
- `forensic_provenance_verify_chain()` — **NOT BOUND**
- `forensic_provenance_get()` — **NOT BOUND**
- `forensic_provenance_save()` — **NOT BOUND**
- `forensic_provenance_load()` — **NOT BOUND**

**Priority:** 🔴 CRITICAL — Chain of custody

#### Evidence Storage (8 functions)
- `forensic_evidence_store_open()` — **NOT BOUND**
- `forensic_evidence_store_close()` — **NOT BOUND**
- `forensic_evidence_store_add()` — **NOT BOUND**
- `forensic_evidence_store_get()` — **NOT BOUND**
- `forensic_evidence_store_list()` — **NOT BOUND**
- `forensic_evidence_store_query()` — **NOT BOUND**
- `forensic_evidence_store_save_index()` — **NOT BOUND**
- `forensic_evidence_store_cleanup()` — **NOT BOUND**

**Priority:** 🔴 CRITICAL — Evidence persistence

#### Permission/Capabilities (5 functions)
- `forensic_permission_check_role()` — **NOT BOUND**
- `forensic_permission_check_set()` — **NOT BOUND**
- `forensic_permission_check_plugin()` — **NOT BOUND**
- `forensic_capability_name()` — **NOT BOUND**
- `forensic_role_name()` — **NOT BOUND**

**Priority:** 🟡 HIGH — Access control (may defer)

#### Audit Logging (4 functions)
- `forensic_audit_log_open()` — **NOT BOUND**
- `forensic_audit_log_close()` — **NOT BOUND**
- `forensic_audit_log_append()` — **NOT BOUND**
- `forensic_audit_log_verify()` — **NOT BOUND**

**Priority:** 🟡 HIGH — Audit trail

#### Plugin Registry (6 functions)
- `forensic_plugin_registry_create()` — **NOT BOUND**
- `forensic_plugin_registry_destroy()` — **NOT BOUND**
- `forensic_plugin_registry_register_collector()` — **NOT BOUND**
- `forensic_plugin_registry_register_parser()` — **NOT BOUND**
- `forensic_plugin_registry_register_output()` — **NOT BOUND**
- `forensic_plugin_registry_find_collector()` — **NOT BOUND**
- `forensic_plugin_registry_find_parser()` — **NOT BOUND**
- `forensic_plugin_registry_find_output()` — **NOT BOUND**

**Priority:** 🟡 HIGH — Extensibility

#### Utility Functions (3 functions)
- `forensic_hash_compute()` — **NOT BOUND**
- `forensic_hash_chain_compute()` — **NOT BOUND**
- (others in utilities)

**Priority:** 🟢 LOW — Helpers, can defer

---

## Type Bindings Missing

### Complex Forensic Types (NOT defined in prelude)
These C structs need JOCKY type equivalents:

| Type | Location | JOCKY Binding | Status |
|------|----------|---------------|--------|
| `forensic_engine_t` | forensic_engine.h | ❌ Missing | CRITICAL |
| `forensic_artifact_t` | forensic_types.h | ❌ Missing | CRITICAL |
| `forensic_artifact_list_t` | forensic_types.h | ❌ Missing | CRITICAL |
| `forensic_parsed_artifact_t` | forensic_types.h | ❌ Missing | CRITICAL |
| `forensic_analysis_result_t` | analysis.h | ❌ Missing | CRITICAL |
| `forensic_timeline_t` | analysis.h | ❌ Missing | CRITICAL |
| `forensic_ioc_list_t` | analysis.h | ❌ Missing | CRITICAL |
| `forensic_provenance_t` | provenance.h | ❌ Missing | CRITICAL |
| `forensic_evidence_store_t` | evidence_store.h | ❌ Missing | HIGH |
| `forensic_audit_log_t` | audit_log.h | ❌ Missing | HIGH |
| `forensic_plugin_registry_t` | forensic_engine.h | ❌ Missing | HIGH |

**Problem:** Codegen cannot convert C structs to JOCKY types without type bindings. Result types will fail at compile-time.

---

## Suggested Function Signatures for Prelude

### Forensic Engine (Binding Template)
```
// Core engine lifecycle
extern fn forensic_engine_create(config: i8*) -> i8*
extern fn forensic_engine_destroy(engine: i8*) -> i32
extern fn forensic_engine_config_default(config: i8*) -> i32

// Pipeline stages
extern fn forensic_engine_register_default_plugins(engine: i8*) -> i32
extern fn forensic_engine_run_collection(engine: i8*, output: i8*) -> i32
extern fn forensic_engine_run_analysis(engine: i8*, parsed: i8*, result: i8*) -> i32
extern fn forensic_engine_generate_outputs(engine: i8*, result: i8*) -> i32
extern fn forensic_engine_run_full_pipeline(engine: i8*, result: i8*) -> i32
```

### Artifact Management (Binding Template)
```
extern fn forensic_artifact_list_create(capacity: i32) -> i8*
extern fn forensic_artifact_list_destroy(list: i8*) -> i32
extern fn forensic_artifact_list_add(list: i8*, artifact: i8*) -> i32

extern fn forensic_bytes_create(data: i8*, len: i32) -> i8*
extern fn forensic_bytes_destroy(bytes: i8*) -> i32

extern fn forensic_metadata_create(capacity: i32) -> i8*
extern fn forensic_metadata_destroy(meta: i8*) -> i32
extern fn forensic_metadata_add(meta: i8*, key: &str, value: &str) -> i32
extern fn forensic_metadata_get(meta: i8*, key: &str) -> i8*
```

### Analysis Functions (Binding Template)
```
extern fn forensic_analysis_create() -> i8*
extern fn forensic_analysis_destroy(result: i8*) -> i32
extern fn forensic_analysis_build_timeline(parsed: i8*, timeline: i8*) -> i32
extern fn forensic_analysis_extract_iocs(parsed: i8*, iocs: i8*) -> i32
extern fn forensic_analysis_correlate(timeline: i8*, iocs: i8*, graph: i8*) -> i32
extern fn forensic_analysis_run(parsed: i8*, result: i8*) -> i32
```

### Provenance Tracking (Binding Template)
```
extern fn forensic_provenance_create() -> i8*
extern fn forensic_provenance_destroy(prov: i8*) -> i32
extern fn forensic_provenance_record(prov: i8*, source: i8*, transform: i8*, output: i8*) -> i32
extern fn forensic_provenance_verify_chain(prov: i8*) -> i32
extern fn forensic_provenance_save(prov: i8*, path: &str) -> i32
extern fn forensic_provenance_load(prov: i8*, path: &str) -> i32
```

### Evidence Storage (Binding Template)
```
extern fn forensic_evidence_store_open(base_path: &str) -> i8*
extern fn forensic_evidence_store_close(store: i8*) -> i32
extern fn forensic_evidence_store_add(store: i8*, artifact: i8*, parsed: i8*, prov: i8*) -> i32
extern fn forensic_evidence_store_query(store: i8*, params: i8*, result: i8*) -> i32
extern fn forensic_evidence_store_cleanup(store: i8*) -> i32
```

---

## Priority Binding Order

### Phase 1: CRITICAL (blocks main pipeline compilation)
1. **forensic_engine_*** (7 functions) — Required for pipeline
2. **forensic_artifact_list_*** (6 functions) — Required for evidence
3. **forensic_analysis_*** (6 functions) — Required for analysis
4. **forensic_provenance_*** (7 functions) — Required for chain-of-custody

**Estimate:** 26 functions, ~50 lines in prelude

### Phase 2: HIGH (enables full forensic functionality)
5. **forensic_evidence_store_*** (8 functions) — Evidence persistence
6. **forensic_audit_log_*** (4 functions) — Audit trail
7. **forensic_plugin_registry_*** (8 functions) — Extensibility

**Estimate:** 20 functions, ~40 lines in prelude

### Phase 3: LOW (optional helpers)
8. **forensic_metadata_*** (4 functions) — Annotation
9. **forensic_bytes_*** (2 functions) — Low-level utilities
10. **forensic_permission_*** (5 functions) — Access control
11. **forensic_utility_*** (3 functions) — Hash/misc

**Estimate:** 14 functions, ~30 lines in prelude

---

## Type System Impact

### Codegen Problem
When research chain calls `forensic_engine_run_analysis(engine, parsed, &result)`, codegen must:
1. Call C function (✅ can do with pointer types)
2. Convert result struct to JOCKY type (❌ **BLOCKED** — no type binding)

### Current Workaround (NOT VIABLE)
Research chains would need to treat all forensic results as opaque `i8*` pointers, then manually deserialize (JSON parsing in JOCKY = complex, slow).

### Proper Solution
Add type bindings in codegen phase:
- Map `forensic_analysis_result_t` → JOCKY record type
- Map `forensic_timeline_t` → JOCKY sequence type
- Map `forensic_ioc_list_t` → JOCKY list type

(See **CODEGEN_COMPILER_ISSUES.md** issue ISSUE-0005 for codegen status)

---

## Backend Impact

### Current State
Backend cannot load forensic library at startup because:
1. Forensic functions not in prelude = not in compiled binary
2. Binary has no reference to forensic `.so` / `.dll`
3. Backend linking fails or forensic functions undefined

### After Prelude Binding
Backend will:
1. Include forensic library in compilation
2. Link forensic symbols correctly
3. Load forensic library at startup

(See **CODEGEN_COMPILER_ISSUES.md** issue ISSUE-0010 for backend status)

---

## Action Items

### Immediate (Today)
- [ ] Add 7 forensic engine functions to prelude
- [ ] Add 6 artifact list functions to prelude
- [ ] Add 6 analysis functions to prelude
- [ ] Add 7 provenance functions to prelude
- [ ] Test compilation with forensic functions
- [ ] Verify codegen can call new functions

### Before Research Chain Integration
- [ ] Add evidence store functions (8)
- [ ] Add audit log functions (4)
- [ ] Add plugin registry functions (8)
- [ ] Update codegen for forensic struct returns
- [ ] Test research chain compilation with forensics

### Deferred (Low Priority)
- [ ] Add metadata helper functions (4)
- [ ] Add bytes utility functions (2)
- [ ] Add permission check functions (5)
- [ ] Add hash utility functions (3)

---

## Validation Checklist

- [ ] All 26 Phase 1 functions added to prelude
- [ ] Prelude compiles without errors
- [ ] Forensic functions callable from JOCKY code
- [ ] Research chain can import prelude with forensics
- [ ] Backend loads forensic library correctly
- [ ] No undefined symbol errors at link-time
- [ ] Forensic pipeline integration checklist updated

---

## Related Documents
- **FORENSIC_INTEGRATION_STATUS.md** — Integration roadmap
- **CODEGEN_COMPILER_ISSUES.md** — Type system and return type issues
- **RUNTIME_API_STRUCTURE.md** — Runtime organization
