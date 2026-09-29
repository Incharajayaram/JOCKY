#include <stddef.h>
#include <stdint.h>
void println(const char* s) { (void)s; }
const char* string(int64_t val) { (void)val; return ""; }
const char* jocky_str_concat(const char* a, const char* b) { (void)a; (void)b; return ""; }
int64_t array_len(void* arr) { (void)arr; return 0; }
void* array_append(void* arr, void* elem) { (void)arr; (void)elem; return (void*)0; }
void jocky_sleep_and_recheck(void) {}
typedef int jocky_aes_ctx_t;
typedef int jocky_rsa_key_t;
typedef int jocky_ecdh_key_t;
jocky_aes_ctx_t jocky_aes_create(const void* k, int ks, const void* i, int is) { (void)k;(void)ks;(void)i;(void)is; return 0; }
int jocky_aes_encrypt(jocky_aes_ctx_t c, const void* p, int pl, void* o, int* ol) { (void)c;(void)p;(void)pl;(void)o;(void)ol; return -1; }
int jocky_aes_decrypt(jocky_aes_ctx_t c, const void* p, int pl, void* o, int* ol) { (void)c;(void)p;(void)pl;(void)o;(void)ol; return -1; }
void jocky_aes_destroy(jocky_aes_ctx_t c) { (void)c; }
jocky_rsa_key_t jocky_rsa_generate_keypair(int b) { (void)b; return 0; }
int jocky_rsa_encrypt(jocky_rsa_key_t k, const void* p, int pl, void* o, int* ol) { (void)k;(void)p;(void)pl;(void)o;(void)ol; return -1; }
int jocky_rsa_decrypt(jocky_rsa_key_t k, const void* p, int pl, void* o, int* ol) { (void)k;(void)p;(void)pl;(void)o;(void)ol; return -1; }
int jocky_rsa_sign(jocky_rsa_key_t k, const void* d, int dl, void* s, int* sl) { (void)k;(void)d;(void)dl;(void)s;(void)sl; return -1; }
int jocky_rsa_verify(jocky_rsa_key_t k, const void* d, int dl, const void* s, int sl) { (void)k;(void)d;(void)dl;(void)s;(void)sl; return -1; }
int jocky_rsa_export_public_pem(jocky_rsa_key_t k, void* o, int* ol) { (void)k;(void)o;(void)ol; return -1; }
int jocky_rsa_export_private_pem(jocky_rsa_key_t k, void* o, int* ol) { (void)k;(void)o;(void)ol; return -1; }
void jocky_rsa_destroy(jocky_rsa_key_t k) { (void)k; }
jocky_ecdh_key_t jocky_ecdh_generate_keypair(int c) { (void)c; return 0; }
int jocky_ecdh_export_public_key(jocky_ecdh_key_t k, void* o, int* ol) { (void)k;(void)o;(void)ol; return -1; }
int jocky_ecdh_compute_shared_secret(jocky_ecdh_key_t k, const void* pk, int pkl, void* s, int* sl) { (void)k;(void)pk;(void)pkl;(void)s;(void)sl; return -1; }
void jocky_ecdh_destroy(jocky_ecdh_key_t k) { (void)k; }
int jocky_exfil_local_cdn(const void* d, int l, const char* n) { (void)d;(void)l;(void)n; return -1; }
int jocky_exfil_list_cdn_files(void* o, int c) { (void)o;(void)c; return 0; }
int jocky_exfil_verify_cdn_hash(const char* n) { (void)n; return -1; }
int jocky_exfil_download_from_cdn(const char* n, void* o, int* ol) { (void)n;(void)o;(void)ol; return -1; }
int jocky_linux_wipe_bash_history(void) { return 0; }
int jocky_linux_wipe_zsh_history(void) { return 0; }
int jocky_linux_wipe_shell_history(void) { return 0; }
int jocky_linux_clear_syslog(void) { return 0; }
int jocky_linux_clear_audit_log(void) { return 0; }
int jocky_linux_clear_journal(void) { return 0; }
int jocky_linux_clear_dmesg(void) { return 0; }
int jocky_linux_wipe_tmp(void) { return 0; }
int jocky_linux_wipe_home_cache(void) { return 0; }
int jocky_linux_clear_command_history(void) { return 0; }
int jocky_linux_wipe_sudo_logs(void) { return 0; }
int jocky_linux_wipe_wtmp_utmp(void) { return 0; }
int jocky_linux_cleanup_all_forensics(void) { return 0; }
int blindside_unhook_ntdll(void) { return 0; }
int btr_disable_notifications(void) { return 0; }
void* crypto_aes256_encrypt(void* d, int l, void* k) { (void)d;(void)l;(void)k; return 0; }
int sandbox_spawn(const char* e, const char* a) { (void)e;(void)a; return 0; }
int sandbox_set_limits(int p, long m, int t, long c) { (void)p;(void)m;(void)t;(void)c; return 0; }
int sandbox_monitor(int p) { (void)p; return 0; }
int sandbox_wait(int p) { (void)p; return 0; }
int sandbox_export_trace(int p, const char* f) { (void)p;(void)f; return 0; }
int exfil_local_cdn(const char* e, const char* n, const char* t, const char* m) { (void)e;(void)n;(void)t;(void)m; return 0; }
int exfil_discord_webhook(const char* w, const char* m) { (void)w;(void)m; return 0; }
int exfil_dns_tunnel(const char* d, const char* m) { (void)d;(void)m; return 0; }
int jocky_registry_create_key(int h, const char* p, void* o) { (void)h;(void)p;(void)o; return 0; }
int jocky_registry_set_value(void* h, const char* n, const char* v, int t, int f) { (void)h;(void)n;(void)v;(void)t;(void)f; return 0; }
int jocky_registry_close_key(void* h) { (void)h; return 0; }
int forensics_wipe_powershell_history(void) { return 0; }
int forensics_wipe_cmd_history(void) { return 0; }
int forensics_flush_arp_cache(void) { return 0; }
int forensics_clear_dns_cache(void) { return 0; }
int jocky_cleanup_event_logs(const char* c) { (void)c; return 0; }
int jocky_cleanup_usn_journal(void) { return 0; }
int linux_forensics_wipe_bash_history(void) { return 0; }
int jocky_linux_cleanup_syslog(void) { return 0; }
int jocky_linux_cleanup_journal(void) { return 0; }
int audit_export(const char* p) { (void)p; return 0; }
int audit_verify(void) { return 0; }
void audit_log(const char* c, const char* a, const char* d, const char* r) { (void)c;(void)a;(void)d;(void)r; }
void* fs_read_file(const char* p) { (void)p; return 0; }
void* fs_list_files(const char* p, int r) { (void)p;(void)r; return 0; }
int fs_file_size(const char* p) { (void)p; return 0; }
int fs_exists(const char* p) { (void)p; return 0; }
void provenance_record(const char* p, const char* o, const char* d) { (void)p;(void)o;(void)d; }
void* plugin_load(const char* p) { (void)p; return 0; }
int plugin_run(void* h, const char* a) { (void)h;(void)a; return 0; }
double ai_score_threat(void) { return 0.0; }
void* crypto_generate_key(int s) { (void)s; return 0; }
int audit_init(int sz) { (void)sz; return 0; }
int byovd_load_driver(const char* p) { (void)p; return 0; }
int byovd_test_exploit(int h) { (void)h; return 0; }
int ai_init(void) { return 0; }
void* ai_collect_telemetry(void) { return 0; }
int btr_mask_module(const char* n) { (void)n; return 0; }
int jocky_exploit_disable_callbacks(void) { return 0; }
int jocky_exploit_token_replacement(int a1, int a2) { (void)a1;(void)a2; return 0; }
int edrhoker_detect(void) { return 0; }
