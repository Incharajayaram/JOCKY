module attributes {dlti.dl_spec = #dlti.dl_spec<!llvm.ptr = dense<64> : vector<4xi64>, i1 = dense<8> : vector<2xi64>, i8 = dense<8> : vector<2xi64>, i16 = dense<16> : vector<2xi64>, i32 = dense<32> : vector<2xi64>, i64 = dense<[32, 64]> : vector<2xi64>, f16 = dense<16> : vector<2xi64>, f64 = dense<64> : vector<2xi64>, f128 = dense<128> : vector<2xi64>, "dlti.endianness" = "little">, llvm.module_asm = [], llvm.target_triple = ""} {
  llvm.mlir.global private constant @".str.0"("[+] JOCKY Linux Binary Started\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.1"("[*] Testing anti-analysis detection...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.2"("[+] Anti-analysis checks complete\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.3"("[+] System is clean - proceeding with payload\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.4"("[*] Collecting system information...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.5"("[+] Information gathered successfully\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.6"("[*] Attempting exfiltration channels...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.7"("[+] Testing Discord webhook integration\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.8"("[+] Testing DNS tunnel setup\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.9"("[+] Testing CDN upload endpoint\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.10"("[*] Cleanup phase starting...\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.11"("[+] Clearing system logs\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.12"("[+] Removing bash history\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.13"("[+] Wiping artifacts\00") {addr_space = 0 : i32, dso_local}
  llvm.mlir.global private constant @".str.14"("[+] Mission complete - exiting cleanly\00") {addr_space = 0 : i32, dso_local}
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
  llvm.func @main() {
    %0 = llvm.mlir.addressof @".str.0" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.1" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.2" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.3" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.4" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.5" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.6" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.7" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.8" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.9" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.10" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.11" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.12" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.13" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.14" : !llvm.ptr
    %16 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<31 x i8>
    llvm.call @println(%16) : (!llvm.ptr) -> ()
    %17 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<39 x i8>
    llvm.call @println(%17) : (!llvm.ptr) -> ()
    %18 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<34 x i8>
    llvm.call @println(%18) : (!llvm.ptr) -> ()
    %19 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<46 x i8>
    llvm.call @println(%19) : (!llvm.ptr) -> ()
    %20 = llvm.getelementptr %5[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%20) : (!llvm.ptr) -> ()
    %21 = llvm.getelementptr %6[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<38 x i8>
    llvm.call @println(%21) : (!llvm.ptr) -> ()
    %22 = llvm.getelementptr %7[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<40 x i8>
    llvm.call @println(%22) : (!llvm.ptr) -> ()
    %23 = llvm.getelementptr %8[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<40 x i8>
    llvm.call @println(%23) : (!llvm.ptr) -> ()
    %24 = llvm.getelementptr %9[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @println(%24) : (!llvm.ptr) -> ()
    %25 = llvm.getelementptr %10[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<32 x i8>
    llvm.call @println(%25) : (!llvm.ptr) -> ()
    %26 = llvm.getelementptr %11[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    llvm.call @println(%26) : (!llvm.ptr) -> ()
    %27 = llvm.getelementptr %12[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<25 x i8>
    llvm.call @println(%27) : (!llvm.ptr) -> ()
    %28 = llvm.getelementptr %13[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    llvm.call @println(%28) : (!llvm.ptr) -> ()
    %29 = llvm.getelementptr %14[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<21 x i8>
    llvm.call @println(%29) : (!llvm.ptr) -> ()
    %30 = llvm.getelementptr %15[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<39 x i8>
    llvm.call @println(%30) : (!llvm.ptr) -> ()
    llvm.return
  }
}
