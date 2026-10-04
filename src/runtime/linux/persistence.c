#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

bool jocky_cron_install(const char* binary_path, const char* schedule) {
    FILE* crontab = popen("crontab -l 2>/dev/null", "r");
    if (!crontab) return false;

    char* existing_cron = (char*)malloc(4096);
    if (!existing_cron) return false;

    size_t len = fread(existing_cron, 1, 4096, crontab);
    pclose(crontab);
    existing_cron[len] = '\0';

    // Create new cron entry
    char cron_entry[512];
    snprintf(cron_entry, sizeof(cron_entry), "%s %s\n", schedule, binary_path);

    // Append to existing cron
    char* new_cron = (char*)malloc(4096 + 512);
    if (!new_cron) {
        free(existing_cron);
        return false;
    }

    snprintf(new_cron, 4096 + 512, "%s%s", existing_cron, cron_entry);

    // Write back to crontab
    FILE* pipe = popen("crontab -", "w");
    if (!pipe) {
        free(existing_cron);
        free(new_cron);
        return false;
    }

    fputs(new_cron, pipe);
    int status = pclose(pipe);

    free(existing_cron);
    free(new_cron);

    return status == 0;
}

bool jocky_systemd_install(const char* service_name, const char* exec_path) {
    char service_file[256];
    snprintf(service_file, sizeof(service_file), "/etc/systemd/system/%s.service", service_name);

    FILE* fp = fopen(service_file, "w");
    if (!fp) return false;

    fprintf(fp, "[Unit]\n");
    fprintf(fp, "Description=JOCKY Service\n");
    fprintf(fp, "After=network.target\n\n");
    fprintf(fp, "[Service]\n");
    fprintf(fp, "Type=simple\n");
    fprintf(fp, "ExecStart=%s\n", exec_path);
    fprintf(fp, "Restart=always\n");
    fprintf(fp, "User=root\n\n");
    fprintf(fp, "[Install]\n");
    fprintf(fp, "WantedBy=multi-user.target\n");

    fclose(fp);

    // Enable and start service
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "systemctl daemon-reload && systemctl enable %s.service && systemctl start %s.service", service_name, service_name);

    return system(cmd) == 0;
}

bool jocky_cron_remove(const char* job_name) {
    FILE* crontab = popen("crontab -l 2>/dev/null", "r");
    if (!crontab) return false;

    char* cron_content = (char*)malloc(4096);
    if (!cron_content) return false;

    size_t len = fread(cron_content, 1, 4096, crontab);
    pclose(crontab);
    cron_content[len] = '\0';

    // Remove job name from cron
    char* new_cron = (char*)malloc(4096);
    if (!new_cron) {
        free(cron_content);
        return false;
    }

    char* line = strtok(cron_content, "\n");
    int pos = 0;

    while (line) {
        if (!strstr(line, job_name)) {
            pos += snprintf(new_cron + pos, 4096 - pos, "%s\n", line);
        }
        line = strtok(NULL, "\n");
    }

    FILE* pipe = popen("crontab -", "w");
    if (!pipe) {
        free(cron_content);
        free(new_cron);
        return false;
    }

    fputs(new_cron, pipe);
    int status = pclose(pipe);

    free(cron_content);
    free(new_cron);

    return status == 0;
}

bool jocky_systemd_remove(const char* service_name) {
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "systemctl stop %s.service && systemctl disable %s.service && rm -f /etc/systemd/system/%s.service && systemctl daemon-reload",
             service_name, service_name, service_name);

    return system(cmd) == 0;
}
