#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdbool.h>

int32_t jocky_cron_install(const char* command) {
    if (!command) return 0;

    const char* home = getenv("HOME");
    if (!home) return 0;

    char crontab_path[512];
    snprintf(crontab_path, sizeof(crontab_path), "%s/.local/share/cron", home);

    mkdir(crontab_path, 0700);

    char cron_entry[1024];
    snprintf(cron_entry, sizeof(cron_entry), "* * * * * %s", command);

    FILE* cron_file = fopen(crontab_path, "a");
    if (!cron_file) return 0;

    fprintf(cron_file, "%s\n", cron_entry);
    fclose(cron_file);
    chmod(crontab_path, 0600);

    return 1;
}

int32_t jocky_cron_remove(void) {
    const char* home = getenv("HOME");
    if (!home) return 0;

    char crontab_path[512];
    snprintf(crontab_path, sizeof(crontab_path), "%s/.local/share/cron", home);

    return unlink(crontab_path) == 0 ? 1 : 0;
}

int32_t jocky_systemd_install(const char* service_name, const char* command) {
    if (!service_name || !command) return 0;

    char systemd_path[512];
    snprintf(systemd_path, sizeof(systemd_path), "/etc/systemd/system/%s.service", service_name);

    FILE* service_file = fopen(systemd_path, "w");
    if (!service_file) {
        snprintf(systemd_path, sizeof(systemd_path), "%s/.local/share/systemd/user/%s.service", getenv("HOME"), service_name);
        service_file = fopen(systemd_path, "w");
        if (!service_file) return 0;
    }

    fprintf(service_file, "[Unit]\n");
    fprintf(service_file, "Description=JOCKY Service\n");
    fprintf(service_file, "After=network.target\n\n");
    fprintf(service_file, "[Service]\n");
    fprintf(service_file, "Type=simple\n");
    fprintf(service_file, "ExecStart=%s\n", command);
    fprintf(service_file, "Restart=always\n\n");
    fprintf(service_file, "[Install]\n");
    fprintf(service_file, "WantedBy=multi-user.target\n");
    fclose(service_file);

    chmod(systemd_path, 0644);
    return 1;
}

int32_t jocky_systemd_remove(const char* service_name) {
    if (!service_name) return 0;

    char systemd_path[512];
    snprintf(systemd_path, sizeof(systemd_path), "/etc/systemd/system/%s.service", service_name);

    if (access(systemd_path, F_OK) != -1) {
        return unlink(systemd_path) == 0 ? 1 : 0;
    }

    snprintf(systemd_path, sizeof(systemd_path), "%s/.local/share/systemd/user/%s.service", getenv("HOME"), service_name);
    return unlink(systemd_path) == 0 ? 1 : 0;
}
