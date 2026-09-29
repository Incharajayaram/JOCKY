module attributes {dlti.dl_spec = #dlti.dl_spec<!llvm.ptr = dense<64> : vector<4xi64>, i1 = dense<8> : vector<2xi64>, i8 = dense<8> : vector<2xi64>, i16 = dense<16> : vector<2xi64>, i32 = dense<32> : vector<2xi64>, i64 = dense<[32, 64]> : vector<2xi64>, f16 = dense<16> : vector<2xi64>, f64 = dense<64> : vector<2xi64>, f128 = dense<128> : vector<2xi64>, "dlti.endianness" = "little">, llvm.module_asm = [], llvm.target_triple = ""} {
  llvm.func internal @__obfs_wrap_jocky_ftrace_detach(%arg0: !llvm.ptr) -> i32 {
    %0 = llvm.call @jocky_ftrace_detach(%arg0) : (!llvm.ptr) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_ftrace_attach(%arg0: !llvm.ptr, %arg1: !llvm.ptr) -> i32 {
    %0 = llvm.call @jocky_ftrace_attach(%arg0, %arg1) : (!llvm.ptr, !llvm.ptr) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_syscall_trace(%arg0: i32) -> i32 {
    %0 = llvm.call @jocky_syscall_trace(%arg0) : (i32) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_syscall_unhook(%arg0: i32) -> i32 {
    %0 = llvm.call @jocky_syscall_unhook(%arg0) : (i32) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_syscall_hook(%arg0: i32, %arg1: !llvm.ptr) -> i32 {
    %0 = llvm.call @jocky_syscall_hook(%arg0, %arg1) : (i32, !llvm.ptr) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_lkm_get_symbol(%arg0: !llvm.ptr, %arg1: !llvm.ptr) -> !llvm.ptr {
    %0 = llvm.call @jocky_lkm_get_symbol(%arg0, %arg1) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_jocky_lkm_unload(%arg0: !llvm.ptr) -> i32 {
    %0 = llvm.call @jocky_lkm_unload(%arg0) : (!llvm.ptr) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_lkm_load(%arg0: !llvm.ptr, %arg1: !llvm.ptr) -> i32 {
    %0 = llvm.call @jocky_lkm_load(%arg0, %arg1) : (!llvm.ptr, !llvm.ptr) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_ebpf_run(%arg0: i32, %arg1: !llvm.ptr, %arg2: i32) -> i64 {
    %0 = llvm.call @jocky_ebpf_run(%arg0, %arg1, %arg2) : (i32, !llvm.ptr, i32) -> i64
    llvm.return %0 : i64
  }
  llvm.func internal @__obfs_wrap_jocky_ebpf_attach(%arg0: i32, %arg1: i32, %arg2: i32) -> i32 {
    %0 = llvm.call @jocky_ebpf_attach(%arg0, %arg1, %arg2) : (i32, i32, i32) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_ebpf_load(%arg0: !llvm.ptr, %arg1: i32, %arg2: i32) -> i32 {
    %0 = llvm.call @jocky_ebpf_load(%arg0, %arg1, %arg2) : (!llvm.ptr, i32, i32) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_fence2pwn_find_uaf_primitive() -> i32 {
    %0 = llvm.call @jocky_fence2pwn_find_uaf_primitive() : () -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_fence2pwn_elevate_to_root() -> i32 {
    %0 = llvm.call @jocky_fence2pwn_elevate_to_root() : () -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_fence2pwn_trigger_reclamation() -> i32 {
    %0 = llvm.call @jocky_fence2pwn_trigger_reclamation() : () -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_fence2pwn_write_cred(%arg0: !llvm.ptr, %arg1: i32, %arg2: i32) -> i32 {
    %0 = llvm.call @jocky_fence2pwn_write_cred(%arg0, %arg1, %arg2) : (!llvm.ptr, i32, i32) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_fence2pwn_allocate_cred_objects(%arg0: i32) -> i32 {
    %0 = llvm.call @jocky_fence2pwn_allocate_cred_objects(%arg0) : (i32) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_fence2pwn_manipulate_creds(%arg0: i32, %arg1: i32) -> i32 {
    %0 = llvm.call @jocky_fence2pwn_manipulate_creds(%arg0, %arg1) : (i32, i32) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_fence2pwn_exploit_uaf(%arg0: i32, %arg1: i32, %arg2: !llvm.ptr) -> i32 {
    %0 = llvm.call @jocky_fence2pwn_exploit_uaf(%arg0, %arg1, %arg2) : (i32, i32, !llvm.ptr) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_fence2pwn_trigger_allocations(%arg0: i64, %arg1: i32) -> i32 {
    %0 = llvm.call @jocky_fence2pwn_trigger_allocations(%arg0, %arg1) : (i64, i32) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_fence2pwn_get_pool_info(%arg0: !llvm.ptr) -> i32 {
    %0 = llvm.call @jocky_fence2pwn_get_pool_info(%arg0) : (!llvm.ptr) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_fence2pwn_detect_kfence() -> i32 {
    %0 = llvm.call @jocky_fence2pwn_detect_kfence() : () -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_process_get_maps(%arg0: i32, %arg1: !llvm.ptr, %arg2: i64) -> i32 {
    %0 = llvm.call @jocky_process_get_maps(%arg0, %arg1, %arg2) : (i32, !llvm.ptr, i64) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_process_ptrace_detach(%arg0: i32) -> i32 {
    %0 = llvm.call @jocky_process_ptrace_detach(%arg0) : (i32) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_process_ptrace_attach(%arg0: i32) -> i32 {
    %0 = llvm.call @jocky_process_ptrace_attach(%arg0) : (i32) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_process_hollow_linux(%arg0: i32, %arg1: !llvm.ptr, %arg2: i64) -> i1 {
    %0 = llvm.call @jocky_process_hollow_linux(%arg0, %arg1, %arg2) : (i32, !llvm.ptr, i64) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_module_resolve_symbol(%arg0: !llvm.ptr, %arg1: !llvm.ptr) -> !llvm.ptr {
    %0 = llvm.call @jocky_module_resolve_symbol(%arg0, %arg1) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_jocky_module_base(%arg0: !llvm.ptr) -> i64 {
    %0 = llvm.call @jocky_module_base(%arg0) : (!llvm.ptr) -> i64
    llvm.return %0 : i64
  }
  llvm.func internal @__obfs_wrap_jocky_module_has_symbol(%arg0: !llvm.ptr, %arg1: !llvm.ptr) -> i1 {
    %0 = llvm.call @jocky_module_has_symbol(%arg0, %arg1) : (!llvm.ptr, !llvm.ptr) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_module_unload(%arg0: !llvm.ptr) -> i1 {
    %0 = llvm.call @jocky_module_unload(%arg0) : (!llvm.ptr) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_module_load(%arg0: !llvm.ptr) -> !llvm.ptr {
    %0 = llvm.call @jocky_module_load(%arg0) : (!llvm.ptr) -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_exfil_local_cdn(%arg0: !llvm.ptr, %arg1: !llvm.ptr, %arg2: !llvm.ptr, %arg3: !llvm.ptr) -> i32 {
    %0 = llvm.call @exfil_local_cdn(%arg0, %arg1, %arg2, %arg3) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_exfil_discord_webhook(%arg0: !llvm.ptr, %arg1: !llvm.ptr) -> i32 {
    %0 = llvm.call @exfil_discord_webhook(%arg0, %arg1) : (!llvm.ptr, !llvm.ptr) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_exfil_dns_tunnel(%arg0: !llvm.ptr, %arg1: !llvm.ptr) -> i32 {
    %0 = llvm.call @exfil_dns_tunnel(%arg0, %arg1) : (!llvm.ptr, !llvm.ptr) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_blindside_unhook_ntdll() -> i1 {
    %0 = llvm.call @blindside_unhook_ntdll() : () -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_edrhoker_detect() -> i1 {
    %0 = llvm.call @edrhoker_detect() : () -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_exploit_token_replacement(%arg0: i32, %arg1: i32) -> i32 {
    %0 = llvm.call @jocky_exploit_token_replacement(%arg0, %arg1) : (i32, i32) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_exploit_disable_callbacks() -> i32 {
    %0 = llvm.call @jocky_exploit_disable_callbacks() : () -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_btr_mask_module(%arg0: !llvm.ptr) -> i1 {
    %0 = llvm.call @btr_mask_module(%arg0) : (!llvm.ptr) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_btr_disable_notifications() -> i1 {
    %0 = llvm.call @btr_disable_notifications() : () -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_byovd_test_exploit(%arg0: i32) -> i32 {
    %0 = llvm.call @byovd_test_exploit(%arg0) : (i32) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_byovd_load_driver(%arg0: !llvm.ptr) -> i32 {
    %0 = llvm.call @byovd_load_driver(%arg0) : (!llvm.ptr) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_ai_score_threat() -> f64 {
    %0 = llvm.call @ai_score_threat() : () -> f64
    llvm.return %0 : f64
  }
  llvm.func internal @__obfs_wrap_ai_collect_telemetry() -> !llvm.ptr {
    %0 = llvm.call @ai_collect_telemetry() : () -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_ai_init() -> i32 {
    %0 = llvm.call @ai_init() : () -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_provenance_record(%arg0: !llvm.ptr, %arg1: !llvm.ptr, %arg2: !llvm.ptr) {
    llvm.call @provenance_record(%arg0, %arg1, %arg2) : (!llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.return
  }
  llvm.func internal @__obfs_wrap_audit_verify() -> i32 {
    %0 = llvm.call @audit_verify() : () -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_audit_export(%arg0: !llvm.ptr) -> i1 {
    %0 = llvm.call @audit_export(%arg0) : (!llvm.ptr) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_audit_init(%arg0: i32) -> i1 {
    %0 = llvm.call @audit_init(%arg0) : (i32) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_audit_log(%arg0: !llvm.ptr, %arg1: !llvm.ptr, %arg2: !llvm.ptr, %arg3: !llvm.ptr) {
    llvm.call @audit_log(%arg0, %arg1, %arg2, %arg3) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.return
  }
  llvm.func internal @__obfs_wrap_jocky_registry_close_key(%arg0: !llvm.ptr) -> i1 {
    %0 = llvm.call @jocky_registry_close_key(%arg0) : (!llvm.ptr) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_registry_set_value(%arg0: !llvm.ptr, %arg1: !llvm.ptr, %arg2: !llvm.ptr, %arg3: i32, %arg4: i32) -> i1 {
    %0 = llvm.call @jocky_registry_set_value(%arg0, %arg1, %arg2, %arg3, %arg4) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, i32, i32) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_registry_create_key(%arg0: i32, %arg1: !llvm.ptr, %arg2: !llvm.ptr) -> i1 {
    %0 = llvm.call @jocky_registry_create_key(%arg0, %arg1, %arg2) : (i32, !llvm.ptr, !llvm.ptr) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_sandbox_export_trace(%arg0: i32, %arg1: !llvm.ptr) -> i1 {
    %0 = llvm.call @sandbox_export_trace(%arg0, %arg1) : (i32, !llvm.ptr) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_sandbox_wait(%arg0: i32) -> i32 {
    %0 = llvm.call @sandbox_wait(%arg0) : (i32) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_sandbox_monitor(%arg0: i32) -> i32 {
    %0 = llvm.call @sandbox_monitor(%arg0) : (i32) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_sandbox_set_limits(%arg0: i32, %arg1: i64, %arg2: i32, %arg3: i64) -> i1 {
    %0 = llvm.call @sandbox_set_limits(%arg0, %arg1, %arg2, %arg3) : (i32, i64, i32, i64) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_sandbox_spawn(%arg0: !llvm.ptr, %arg1: !llvm.ptr) -> i32 {
    %0 = llvm.call @sandbox_spawn(%arg0, %arg1) : (!llvm.ptr, !llvm.ptr) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_crypto_aes256_decrypt(%arg0: !llvm.ptr, %arg1: i32, %arg2: !llvm.ptr) -> !llvm.ptr {
    %0 = llvm.call @crypto_aes256_decrypt(%arg0, %arg1, %arg2) : (!llvm.ptr, i32, !llvm.ptr) -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_crypto_aes256_encrypt(%arg0: !llvm.ptr, %arg1: i32, %arg2: !llvm.ptr) -> !llvm.ptr {
    %0 = llvm.call @crypto_aes256_encrypt(%arg0, %arg1, %arg2) : (!llvm.ptr, i32, !llvm.ptr) -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_crypto_generate_key(%arg0: i32) -> !llvm.ptr {
    %0 = llvm.call @crypto_generate_key(%arg0) : (i32) -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_fs_write_file(%arg0: !llvm.ptr, %arg1: !llvm.ptr, %arg2: i64) -> i1 {
    %0 = llvm.call @fs_write_file(%arg0, %arg1, %arg2) : (!llvm.ptr, !llvm.ptr, i64) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_fs_read_file(%arg0: !llvm.ptr) -> !llvm.ptr {
    %0 = llvm.call @fs_read_file(%arg0) : (!llvm.ptr) -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_fs_file_size(%arg0: !llvm.ptr) -> i64 {
    %0 = llvm.call @fs_file_size(%arg0) : (!llvm.ptr) -> i64
    llvm.return %0 : i64
  }
  llvm.func internal @__obfs_wrap_fs_list_files(%arg0: !llvm.ptr, %arg1: i1) -> !llvm.ptr {
    %0 = llvm.call @fs_list_files(%arg0, %arg1) : (!llvm.ptr, i1) -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_fs_exists(%arg0: !llvm.ptr) -> i1 {
    %0 = llvm.call @fs_exists(%arg0) : (!llvm.ptr) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_cleanup_usn_journal() -> i32 {
    %0 = llvm.call @jocky_cleanup_usn_journal() : () -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_cleanup_event_logs(%arg0: !llvm.ptr) -> i32 {
    %0 = llvm.call @jocky_cleanup_event_logs(%arg0) : (!llvm.ptr) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_linux_cleanup_journal() -> i32 {
    %0 = llvm.call @jocky_linux_cleanup_journal() : () -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_linux_cleanup_syslog() -> i32 {
    %0 = llvm.call @jocky_linux_cleanup_syslog() : () -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_linux_forensics_wipe_bash_history() -> i32 {
    %0 = llvm.call @linux_forensics_wipe_bash_history() : () -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_forensics_clear_dns_cache() -> i32 {
    %0 = llvm.call @forensics_clear_dns_cache() : () -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_forensics_flush_arp_cache() -> i32 {
    %0 = llvm.call @forensics_flush_arp_cache() : () -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_forensics_wipe_cmd_history() -> i32 {
    %0 = llvm.call @forensics_wipe_cmd_history() : () -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_forensics_wipe_powershell_history() -> i32 {
    %0 = llvm.call @forensics_wipe_powershell_history() : () -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_cleanup_all() {
    llvm.call @jocky_cleanup_all() : () -> ()
    llvm.return
  }
  llvm.func internal @__obfs_wrap_jocky_clear_srum() -> i1 {
    %0 = llvm.call @jocky_clear_srum() : () -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_patch_amcache() -> i1 {
    %0 = llvm.call @jocky_patch_amcache() : () -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_patch_shimcache() -> i1 {
    %0 = llvm.call @jocky_patch_shimcache() : () -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_wipe_prefetch() {
    llvm.call @jocky_wipe_prefetch() : () -> ()
    llvm.return
  }
  llvm.func internal @__obfs_wrap_jocky_wipe_artifacts(%arg0: !llvm.ptr) {
    llvm.call @jocky_wipe_artifacts(%arg0) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func internal @__obfs_wrap_jocky_clear_logs() {
    llvm.call @jocky_clear_logs() : () -> ()
    llvm.return
  }
  llvm.func internal @__obfs_wrap_jocky_self_delete() {
    llvm.call @jocky_self_delete() : () -> ()
    llvm.return
  }
  llvm.func internal @__obfs_wrap_jocky_exfil_github(%arg0: !llvm.ptr, %arg1: !llvm.ptr, %arg2: !llvm.ptr, %arg3: i32) -> i1 {
    %0 = llvm.call @jocky_exfil_github(%arg0, %arg1, %arg2, %arg3) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, i32) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_exfil_telegram(%arg0: !llvm.ptr, %arg1: !llvm.ptr, %arg2: !llvm.ptr, %arg3: i32) -> i1 {
    %0 = llvm.call @jocky_exfil_telegram(%arg0, %arg1, %arg2, %arg3) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, i32) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_exfil_discord(%arg0: !llvm.ptr, %arg1: !llvm.ptr, %arg2: i32) -> i1 {
    %0 = llvm.call @jocky_exfil_discord(%arg0, %arg1, %arg2) : (!llvm.ptr, !llvm.ptr, i32) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_exfil_dns(%arg0: !llvm.ptr, %arg1: !llvm.ptr, %arg2: i32) -> i1 {
    %0 = llvm.call @jocky_exfil_dns(%arg0, %arg1, %arg2) : (!llvm.ptr, !llvm.ptr, i32) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_exfil_front(%arg0: !llvm.ptr, %arg1: !llvm.ptr, %arg2: !llvm.ptr, %arg3: !llvm.ptr, %arg4: i32) -> i1 {
    %0 = llvm.call @jocky_exfil_front(%arg0, %arg1, %arg2, %arg3, %arg4) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr, i32) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_exfil_encrypt(%arg0: !llvm.ptr, %arg1: i32, %arg2: !llvm.ptr, %arg3: i32) {
    llvm.call @jocky_exfil_encrypt(%arg0, %arg1, %arg2, %arg3) : (!llvm.ptr, i32, !llvm.ptr, i32) -> ()
    llvm.return
  }
  llvm.func internal @__obfs_wrap_jocky_thread_hijack(%arg0: i32, %arg1: !llvm.ptr, %arg2: i32) -> i1 {
    %0 = llvm.call @jocky_thread_hijack(%arg0, %arg1, %arg2) : (i32, !llvm.ptr, i32) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_rdll_inject(%arg0: i32, %arg1: !llvm.ptr, %arg2: i32) -> i1 {
    %0 = llvm.call @jocky_rdll_inject(%arg0, %arg1, %arg2) : (i32, !llvm.ptr, i32) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_module_stomp(%arg0: !llvm.ptr, %arg1: !llvm.ptr, %arg2: i32) -> i1 {
    %0 = llvm.call @jocky_module_stomp(%arg0, %arg1, %arg2) : (!llvm.ptr, !llvm.ptr, i32) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_process_hollow(%arg0: !llvm.ptr, %arg1: !llvm.ptr, %arg2: i32) -> i1 {
    %0 = llvm.call @jocky_process_hollow(%arg0, %arg1, %arg2) : (!llvm.ptr, !llvm.ptr, i32) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_elevate_token(%arg0: !llvm.ptr, %arg1: i32) -> i1 {
    %0 = llvm.call @jocky_elevate_token(%arg0, %arg1) : (!llvm.ptr, i32) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_disable_etw(%arg0: !llvm.ptr) -> i1 {
    %0 = llvm.call @jocky_disable_etw(%arg0) : (!llvm.ptr) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_disable_edr_callbacks(%arg0: !llvm.ptr) -> i32 {
    %0 = llvm.call @jocky_disable_edr_callbacks(%arg0) : (!llvm.ptr) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_kwrite(%arg0: !llvm.ptr, %arg1: i64, %arg2: !llvm.ptr, %arg3: i32) -> i1 {
    %0 = llvm.call @jocky_kwrite(%arg0, %arg1, %arg2, %arg3) : (!llvm.ptr, i64, !llvm.ptr, i32) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_kread(%arg0: !llvm.ptr, %arg1: i64, %arg2: !llvm.ptr, %arg3: i32) -> i1 {
    %0 = llvm.call @jocky_kread(%arg0, %arg1, %arg2, %arg3) : (!llvm.ptr, i64, !llvm.ptr, i32) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_driver_map_kernel(%arg0: !llvm.ptr, %arg1: i64, %arg2: i32) -> !llvm.ptr {
    %0 = llvm.call @jocky_driver_map_kernel(%arg0, %arg1, %arg2) : (!llvm.ptr, i64, i32) -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_jocky_driver_write_phys(%arg0: !llvm.ptr, %arg1: i64, %arg2: !llvm.ptr, %arg3: i32) -> i1 {
    %0 = llvm.call @jocky_driver_write_phys(%arg0, %arg1, %arg2, %arg3) : (!llvm.ptr, i64, !llvm.ptr, i32) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_driver_read_phys(%arg0: !llvm.ptr, %arg1: i64, %arg2: !llvm.ptr, %arg3: i32) -> i1 {
    %0 = llvm.call @jocky_driver_read_phys(%arg0, %arg1, %arg2, %arg3) : (!llvm.ptr, i64, !llvm.ptr, i32) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_byovd_unload(%arg0: !llvm.ptr) {
    llvm.call @jocky_byovd_unload(%arg0) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func internal @__obfs_wrap_jocky_byovd_load(%arg0: !llvm.ptr, %arg1: !llvm.ptr, %arg2: !llvm.ptr) -> i1 {
    %0 = llvm.call @jocky_byovd_load(%arg0, %arg1, %arg2) : (!llvm.ptr, !llvm.ptr, !llvm.ptr) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_spoof_syscall(%arg0: i32, %arg1: i64, %arg2: i64, %arg3: i64, %arg4: i64) -> i64 {
    %0 = llvm.call @jocky_spoof_syscall(%arg0, %arg1, %arg2, %arg3, %arg4) : (i32, i64, i64, i64, i64) -> i64
    llvm.return %0 : i64
  }
  llvm.func internal @__obfs_wrap_jocky_spoof_call(%arg0: !llvm.ptr, %arg1: i64, %arg2: i64, %arg3: i64, %arg4: i64) -> i64 {
    %0 = llvm.call @jocky_spoof_call(%arg0, %arg1, %arg2, %arg3, %arg4) : (!llvm.ptr, i64, i64, i64, i64) -> i64
    llvm.return %0 : i64
  }
  llvm.func internal @__obfs_wrap_jocky_get_syscall_number(%arg0: !llvm.ptr) -> i32 {
    %0 = llvm.call @jocky_get_syscall_number(%arg0) : (!llvm.ptr) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_unhook_ntdll() -> i1 {
    %0 = llvm.call @jocky_unhook_ntdll() : () -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_decrypt_rc4(%arg0: !llvm.ptr, %arg1: i32, %arg2: !llvm.ptr, %arg3: i32) {
    llvm.call @jocky_decrypt_rc4(%arg0, %arg1, %arg2, %arg3) : (!llvm.ptr, i32, !llvm.ptr, i32) -> ()
    llvm.return
  }
  llvm.func internal @__obfs_wrap_jocky_decrypt_xor(%arg0: !llvm.ptr, %arg1: i32, %arg2: i8, %arg3: i32) {
    llvm.call @jocky_decrypt_xor(%arg0, %arg1, %arg2, %arg3) : (!llvm.ptr, i32, i8, i32) -> ()
    llvm.return
  }
  llvm.func internal @__obfs_wrap_jocky_sleep_and_recheck() {
    llvm.call @jocky_sleep_and_recheck() : () -> ()
    llvm.return
  }
  llvm.func internal @__obfs_wrap_jocky_is_sandbox() -> i1 {
    %0 = llvm.call @jocky_is_sandbox() : () -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_is_vm() -> i1 {
    %0 = llvm.call @jocky_is_vm() : () -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_is_debugger_present() -> i1 {
    %0 = llvm.call @jocky_is_debugger_present() : () -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_check_analysis_environment() -> i32 {
    %0 = llvm.call @jocky_check_analysis_environment() : () -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_runtime_init() -> i32 {
    %0 = llvm.call @jocky_runtime_init() : () -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_byovd_destroy(%arg0: !llvm.ptr) {
    llvm.call @jocky_byovd_destroy(%arg0) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func internal @__obfs_wrap_jocky_byovd_new() -> !llvm.ptr {
    %0 = llvm.call @jocky_byovd_new() : () -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_jocky_free(%arg0: !llvm.ptr) {
    llvm.call @jocky_free(%arg0) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func internal @__obfs_wrap_jocky_alloc(%arg0: i64) -> !llvm.ptr {
    %0 = llvm.call @jocky_alloc(%arg0) : (i64) -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_array_append(%arg0: !llvm.ptr, %arg1: !llvm.ptr) -> !llvm.ptr {
    %0 = llvm.call @array_append(%arg0, %arg1) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_array_len(%arg0: !llvm.ptr) -> i64 {
    %0 = llvm.call @array_len(%arg0) : (!llvm.ptr) -> i64
    llvm.return %0 : i64
  }
  llvm.func internal @__obfs_wrap_exit(%arg0: i32) {
    llvm.call @exit(%arg0) : (i32) -> ()
    llvm.return
  }
  llvm.func internal @__obfs_wrap_strlen(%arg0: !llvm.ptr) -> i64 {
    %0 = llvm.call @strlen(%arg0) : (!llvm.ptr) -> i64
    llvm.return %0 : i64
  }
  llvm.func internal @__obfs_wrap_memcpy(%arg0: !llvm.ptr, %arg1: !llvm.ptr, %arg2: i64) -> !llvm.ptr {
    %0 = llvm.call @memcpy(%arg0, %arg1, %arg2) : (!llvm.ptr, !llvm.ptr, i64) -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_memset(%arg0: !llvm.ptr, %arg1: i32, %arg2: i64) -> !llvm.ptr {
    %0 = llvm.call @memset(%arg0, %arg1, %arg2) : (!llvm.ptr, i32, i64) -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_free(%arg0: !llvm.ptr) {
    llvm.call @free(%arg0) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func internal @__obfs_wrap_malloc(%arg0: i64) -> !llvm.ptr {
    %0 = llvm.call @malloc(%arg0) : (i64) -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_jocky_str_concat(%arg0: !llvm.ptr, %arg1: !llvm.ptr) -> !llvm.ptr {
    %0 = llvm.call @jocky_str_concat(%arg0, %arg1) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_string(%arg0: i64) -> !llvm.ptr {
    %0 = llvm.call @string(%arg0) : (i64) -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_println(%arg0: !llvm.ptr) {
    llvm.call @println(%arg0) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func internal @__obfs_wrap_puts(%arg0: !llvm.ptr) -> i32 {
    %0 = llvm.call @puts(%arg0) : (!llvm.ptr) -> i32
    llvm.return %0 : i32
  }
  llvm.mlir.global private unnamed_addr constant @__obfs_key("default_key") {addr_space = 0 : i32}
  llvm.func @f_658275b5bcb4(%arg0: !llvm.ptr, %arg1: i64) -> !llvm.ptr attributes {sym_visibility = "private"} {
    %0 = llvm.mlir.constant(0 : i64) : i64
    %1 = llvm.mlir.constant(1 : i64) : i64
    %2 = llvm.mlir.constant(77 : i8) : i8
    %3 = llvm.alloca %1 x i64 : (i64) -> !llvm.ptr
    llvm.store %0, %3 : i64, !llvm.ptr
    llvm.br ^bb1
  ^bb1:  // 2 preds: ^bb0, ^bb2
    %4 = llvm.load %3 : !llvm.ptr -> i64
    %5 = llvm.icmp "slt" %4, %arg1 : i64
    llvm.cond_br %5, ^bb2, ^bb3
  ^bb2:  // pred: ^bb1
    %6 = llvm.load %3 : !llvm.ptr -> i64
    %7 = llvm.getelementptr %arg0[%6] : (!llvm.ptr, i64) -> !llvm.ptr, i8
    %8 = llvm.load %7 : !llvm.ptr -> i8
    %9 = llvm.trunc %6 : i64 to i8
    %10 = llvm.sub %8, %9 : i8
    %11 = llvm.xor %10, %2 : i8
    llvm.store %11, %7 : i8, !llvm.ptr
    %12 = llvm.add %6, %1 : i64
    llvm.store %12, %3 : i64, !llvm.ptr
    llvm.br ^bb1
  ^bb3:  // pred: ^bb1
    llvm.return %arg0 : !llvm.ptr
  }
  llvm.mlir.global private @".str.0.enc"("r\02t\11~k`Rw\13rKJ#\22\09sAjT60\E7P7$,80\19\00") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.1.enc"("r\0Dt\11hA0\1FGIM\1C]V&F\1CIjTP00^0\E76/!\1A'==' \E0\E6\FD\1F") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.2.enc"("r\02t\11eDKt\03PTSH'-F\22\0A\1FSY:X0\E3&IW-a-=1\0A") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.3.enc"("r\02t\11WU0\1F[L\0EK/\1C]ETIj\EB\11\FB73\\&7/1\1E*,\F4?#:(\E7\0F'\03!).1\1F") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.4.enc"("r\0Dt\11gKSw[R:KJQ\1A8(;\14PQ\FB^_%Z,W<\09)$:\E0\E0\E0\10") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.5.enc"("r\02t\11}DEv,LO UIP\09VI\14SY-Z%\E36'%>\1A782=&\229\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.6.enc"("r\0Dt\11eRKpC#:KJQ\1AB)Hi_(-&5ZZH\E4>\1D!':-&\07\E6\FD\E5\00") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.7.enc"("r\02t\11hA0\1FGIM\1CpW-H^$d\EB+D!Y\\Z5\E44\1F<\223\04+:)2%\00") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.8.enc"("r\02t\11hA0\1FGIM\1CpvM\09%=j]YO\E70&3'4\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.9.enc"("r\02t\11hA0\1FGIM\1C\7Fpp\09$:l^%G\E7Z]#\22Q4\1F<\09") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.10.enc"("r\0Dt\11gJZl@$>\1C,T[8T\0A\0F'%-+^]\22\08\12\0B5") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.11.enc"("r\02t\11gJZl,HTQ\1C-#8%Mm\EBPN$0\03") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.12.enc"("r\02t\11VARv(HTQ\1CZ[8Y\0Ah\\77\\3*\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.13.enc"("r\02t\11kE7t@V\0ES. SOPK\14:\04") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.14.enc"("r\02t\11qE0\1AGNT\1C_IO9]M\14P\E4\0C\E7Z+\\&SK\18\E0(8-+ $,\1F") {addr_space = 0 : i32}
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
    %0 = llvm.mlir.addressof @".str.0.enc" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.1.enc" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.2.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.3.enc" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.4.enc" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.5.enc" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.6.enc" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.7.enc" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.8.enc" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.9.enc" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.10.enc" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.11.enc" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.12.enc" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.13.enc" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.14.enc" : !llvm.ptr
    %16 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<31 x i8>
    llvm.call @__obfs_wrap_println(%16) : (!llvm.ptr) -> ()
    %17 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<39 x i8>
    llvm.call @__obfs_wrap_println(%17) : (!llvm.ptr) -> ()
    %18 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<34 x i8>
    llvm.call @__obfs_wrap_println(%18) : (!llvm.ptr) -> ()
    %19 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<46 x i8>
    llvm.call @__obfs_wrap_println(%19) : (!llvm.ptr) -> ()
    %20 = llvm.getelementptr %5[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @__obfs_wrap_println(%20) : (!llvm.ptr) -> ()
    %21 = llvm.getelementptr %6[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<38 x i8>
    llvm.call @__obfs_wrap_println(%21) : (!llvm.ptr) -> ()
    %22 = llvm.getelementptr %7[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<40 x i8>
    llvm.call @__obfs_wrap_println(%22) : (!llvm.ptr) -> ()
    %23 = llvm.getelementptr %8[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<40 x i8>
    llvm.call @__obfs_wrap_println(%23) : (!llvm.ptr) -> ()
    %24 = llvm.getelementptr %9[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @__obfs_wrap_println(%24) : (!llvm.ptr) -> ()
    %25 = llvm.getelementptr %10[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<32 x i8>
    llvm.call @__obfs_wrap_println(%25) : (!llvm.ptr) -> ()
    %26 = llvm.getelementptr %11[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    llvm.call @__obfs_wrap_println(%26) : (!llvm.ptr) -> ()
    %27 = llvm.getelementptr %12[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<25 x i8>
    llvm.call @__obfs_wrap_println(%27) : (!llvm.ptr) -> ()
    %28 = llvm.getelementptr %13[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    llvm.call @__obfs_wrap_println(%28) : (!llvm.ptr) -> ()
    %29 = llvm.getelementptr %14[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<21 x i8>
    llvm.call @__obfs_wrap_println(%29) : (!llvm.ptr) -> ()
    %30 = llvm.getelementptr %15[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<39 x i8>
    llvm.call @__obfs_wrap_println(%30) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func internal @f_42fc974f80dc() attributes {no_inline} {
    %0 = llvm.mlir.addressof @".str.0.enc" : !llvm.ptr
    %1 = llvm.mlir.constant(31 : i64) : i64
    %2 = llvm.call @f_658275b5bcb4(%0, %1) : (!llvm.ptr, i64) -> !llvm.ptr
    %3 = llvm.mlir.addressof @".str.1.enc" : !llvm.ptr
    %4 = llvm.mlir.constant(39 : i64) : i64
    %5 = llvm.call @f_658275b5bcb4(%3, %4) : (!llvm.ptr, i64) -> !llvm.ptr
    %6 = llvm.mlir.addressof @".str.2.enc" : !llvm.ptr
    %7 = llvm.mlir.constant(34 : i64) : i64
    %8 = llvm.call @f_658275b5bcb4(%6, %7) : (!llvm.ptr, i64) -> !llvm.ptr
    %9 = llvm.mlir.addressof @".str.3.enc" : !llvm.ptr
    %10 = llvm.mlir.constant(46 : i64) : i64
    %11 = llvm.call @f_658275b5bcb4(%9, %10) : (!llvm.ptr, i64) -> !llvm.ptr
    %12 = llvm.mlir.addressof @".str.4.enc" : !llvm.ptr
    %13 = llvm.mlir.constant(37 : i64) : i64
    %14 = llvm.call @f_658275b5bcb4(%12, %13) : (!llvm.ptr, i64) -> !llvm.ptr
    %15 = llvm.mlir.addressof @".str.5.enc" : !llvm.ptr
    %16 = llvm.mlir.constant(38 : i64) : i64
    %17 = llvm.call @f_658275b5bcb4(%15, %16) : (!llvm.ptr, i64) -> !llvm.ptr
    %18 = llvm.mlir.addressof @".str.6.enc" : !llvm.ptr
    %19 = llvm.mlir.constant(40 : i64) : i64
    %20 = llvm.call @f_658275b5bcb4(%18, %19) : (!llvm.ptr, i64) -> !llvm.ptr
    %21 = llvm.mlir.addressof @".str.7.enc" : !llvm.ptr
    %22 = llvm.mlir.constant(40 : i64) : i64
    %23 = llvm.call @f_658275b5bcb4(%21, %22) : (!llvm.ptr, i64) -> !llvm.ptr
    %24 = llvm.mlir.addressof @".str.8.enc" : !llvm.ptr
    %25 = llvm.mlir.constant(29 : i64) : i64
    %26 = llvm.call @f_658275b5bcb4(%24, %25) : (!llvm.ptr, i64) -> !llvm.ptr
    %27 = llvm.mlir.addressof @".str.9.enc" : !llvm.ptr
    %28 = llvm.mlir.constant(32 : i64) : i64
    %29 = llvm.call @f_658275b5bcb4(%27, %28) : (!llvm.ptr, i64) -> !llvm.ptr
    %30 = llvm.mlir.addressof @".str.10.enc" : !llvm.ptr
    %31 = llvm.mlir.constant(30 : i64) : i64
    %32 = llvm.call @f_658275b5bcb4(%30, %31) : (!llvm.ptr, i64) -> !llvm.ptr
    %33 = llvm.mlir.addressof @".str.11.enc" : !llvm.ptr
    %34 = llvm.mlir.constant(25 : i64) : i64
    %35 = llvm.call @f_658275b5bcb4(%33, %34) : (!llvm.ptr, i64) -> !llvm.ptr
    %36 = llvm.mlir.addressof @".str.12.enc" : !llvm.ptr
    %37 = llvm.mlir.constant(26 : i64) : i64
    %38 = llvm.call @f_658275b5bcb4(%36, %37) : (!llvm.ptr, i64) -> !llvm.ptr
    %39 = llvm.mlir.addressof @".str.13.enc" : !llvm.ptr
    %40 = llvm.mlir.constant(21 : i64) : i64
    %41 = llvm.call @f_658275b5bcb4(%39, %40) : (!llvm.ptr, i64) -> !llvm.ptr
    %42 = llvm.mlir.addressof @".str.14.enc" : !llvm.ptr
    %43 = llvm.mlir.constant(39 : i64) : i64
    %44 = llvm.call @f_658275b5bcb4(%42, %43) : (!llvm.ptr, i64) -> !llvm.ptr
    llvm.return
  }
  llvm.mlir.global_ctors ctors = [@f_42fc974f80dc, @f_59bf8ee0007f], priorities = [101 : i32, 101 : i32], data = [#llvm.zero, #llvm.zero]
  llvm.func internal @f_40b5962dd677(%arg0: !llvm.ptr, %arg1: i32) attributes {no_inline} {
    %0 = llvm.mlir.addressof @__obfs_key : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.constant(1 : i32) : i32
    %3 = llvm.mlir.constant(11 : i32) : i32
    %4 = llvm.alloca %2 x i32 : (i32) -> !llvm.ptr
    llvm.store %1, %4 : i32, !llvm.ptr
    llvm.br ^bb1
  ^bb1:  // 2 preds: ^bb0, ^bb2
    %5 = llvm.load %4 : !llvm.ptr -> i32
    %6 = llvm.icmp "slt" %5, %arg1 : i32
    llvm.cond_br %6, ^bb2, ^bb3
  ^bb2:  // pred: ^bb1
    %7 = llvm.load %4 : !llvm.ptr -> i32
    %8 = llvm.sext %7 : i32 to i64
    %9 = llvm.getelementptr %arg0[%8] : (!llvm.ptr, i64) -> !llvm.ptr, i8
    %10 = llvm.load %9 : !llvm.ptr -> i8
    %11 = llvm.srem %7, %3 : i32
    %12 = llvm.sext %11 : i32 to i64
    %13 = llvm.getelementptr %0[%12] : (!llvm.ptr, i64) -> !llvm.ptr, i8
    %14 = llvm.load %13 : !llvm.ptr -> i8
    %15 = llvm.xor %10, %14 : i8
    llvm.store %15, %9 : i8, !llvm.ptr
    %16 = llvm.add %7, %2 : i32
    llvm.store %16, %4 : i32, !llvm.ptr
    llvm.br ^bb1
  ^bb3:  // pred: ^bb1
    llvm.return
  }
  llvm.func internal @f_59bf8ee0007f() attributes {no_inline} {
    %0 = llvm.mlir.constant(31 : i32) : i32
    %1 = llvm.mlir.addressof @".str.0.enc" : !llvm.ptr
    llvm.call @f_40b5962dd677(%1, %0) : (!llvm.ptr, i32) -> ()
    %2 = llvm.mlir.constant(39 : i32) : i32
    %3 = llvm.mlir.addressof @".str.1.enc" : !llvm.ptr
    llvm.call @f_40b5962dd677(%3, %2) : (!llvm.ptr, i32) -> ()
    %4 = llvm.mlir.constant(34 : i32) : i32
    %5 = llvm.mlir.addressof @".str.2.enc" : !llvm.ptr
    llvm.call @f_40b5962dd677(%5, %4) : (!llvm.ptr, i32) -> ()
    %6 = llvm.mlir.constant(46 : i32) : i32
    %7 = llvm.mlir.addressof @".str.3.enc" : !llvm.ptr
    llvm.call @f_40b5962dd677(%7, %6) : (!llvm.ptr, i32) -> ()
    %8 = llvm.mlir.constant(37 : i32) : i32
    %9 = llvm.mlir.addressof @".str.4.enc" : !llvm.ptr
    llvm.call @f_40b5962dd677(%9, %8) : (!llvm.ptr, i32) -> ()
    %10 = llvm.mlir.constant(38 : i32) : i32
    %11 = llvm.mlir.addressof @".str.5.enc" : !llvm.ptr
    llvm.call @f_40b5962dd677(%11, %10) : (!llvm.ptr, i32) -> ()
    %12 = llvm.mlir.constant(40 : i32) : i32
    %13 = llvm.mlir.addressof @".str.6.enc" : !llvm.ptr
    llvm.call @f_40b5962dd677(%13, %12) : (!llvm.ptr, i32) -> ()
    %14 = llvm.mlir.constant(40 : i32) : i32
    %15 = llvm.mlir.addressof @".str.7.enc" : !llvm.ptr
    llvm.call @f_40b5962dd677(%15, %14) : (!llvm.ptr, i32) -> ()
    %16 = llvm.mlir.constant(29 : i32) : i32
    %17 = llvm.mlir.addressof @".str.8.enc" : !llvm.ptr
    llvm.call @f_40b5962dd677(%17, %16) : (!llvm.ptr, i32) -> ()
    %18 = llvm.mlir.constant(32 : i32) : i32
    %19 = llvm.mlir.addressof @".str.9.enc" : !llvm.ptr
    llvm.call @f_40b5962dd677(%19, %18) : (!llvm.ptr, i32) -> ()
    %20 = llvm.mlir.constant(30 : i32) : i32
    %21 = llvm.mlir.addressof @".str.10.enc" : !llvm.ptr
    llvm.call @f_40b5962dd677(%21, %20) : (!llvm.ptr, i32) -> ()
    %22 = llvm.mlir.constant(25 : i32) : i32
    %23 = llvm.mlir.addressof @".str.11.enc" : !llvm.ptr
    llvm.call @f_40b5962dd677(%23, %22) : (!llvm.ptr, i32) -> ()
    %24 = llvm.mlir.constant(26 : i32) : i32
    %25 = llvm.mlir.addressof @".str.12.enc" : !llvm.ptr
    llvm.call @f_40b5962dd677(%25, %24) : (!llvm.ptr, i32) -> ()
    %26 = llvm.mlir.constant(21 : i32) : i32
    %27 = llvm.mlir.addressof @".str.13.enc" : !llvm.ptr
    llvm.call @f_40b5962dd677(%27, %26) : (!llvm.ptr, i32) -> ()
    %28 = llvm.mlir.constant(39 : i32) : i32
    %29 = llvm.mlir.addressof @".str.14.enc" : !llvm.ptr
    llvm.call @f_40b5962dd677(%29, %28) : (!llvm.ptr, i32) -> ()
    llvm.return
  }
}

