#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../../common/logging.h"

/* Portable file-based logging implementation
 * Uses simple text log files for cross-platform compatibility */

#define LOG_FILE_PATH "/tmp/.jocky_logs"
#define MAX_LOG_SIZE 1048576  /* 1MB max log file */

/* Get or create log file path */
static int32_t _get_log_file(const char *source, char *path, size_t path_len) {
    if (!source || !path) {
        return -1;
    }

    /* Sanitize source name to prevent path traversal */
    char safe_source[128];
    size_t src_len = strlen(source);
    if (src_len > 120) {
        src_len = 120;
    }

    for (size_t i = 0; i < src_len; i++) {
        char c = source[i];
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') || c == '_' || c == '-') {
            safe_source[i] = c;
        } else {
            safe_source[i] = '_';
        }
    }
    safe_source[src_len] = '\0';

    /* Build log file path */
    snprintf(path, path_len, "%s/%s.log", LOG_FILE_PATH, safe_source);
    return 0;
}

/* Rotate log file if it exceeds max size */
static void _rotate_log(const char *log_file) {
    struct stat st;
    if (stat(log_file, &st) == 0 && st.st_size > MAX_LOG_SIZE) {
        char backup[512];
        snprintf(backup, sizeof(backup), "%s.old", log_file);
        remove(backup);
        rename(log_file, backup);
    }
}

int32_t logging_clear_logs(const char *source) {
    if (!source) {
        return -1;
    }

    char log_file[256];
    if (_get_log_file(source, log_file, sizeof(log_file)) != 0) {
        return -1;
    }

    /* Truncate the log file to zero length */
    FILE *fp = fopen(log_file, "w");
    if (fp) {
        fclose(fp);
        return 0;
    }

    return -1;
}

int64_t logging_log_event(const char *source, const char *message,
                          int32_t severity) {
    if (!source || !message) {
        return -1;
    }

    char log_file[256];
    if (_get_log_file(source, log_file, sizeof(log_file)) != 0) {
        return -1;
    }

    /* Ensure log directory exists */
    mkdir(LOG_FILE_PATH, 0700);

    /* Rotate log if needed */
    _rotate_log(log_file);

    /* Generate event ID from current time */
    uint64_t event_id = (uint64_t)time(NULL);

    /* Append event to log file */
    FILE *fp = fopen(log_file, "a");
    if (fp) {
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        char timestamp[32];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

        fprintf(fp, "[%s] [%lu] [SEV:%d] %s\n",
                timestamp, event_id, severity, message);
        fclose(fp);
        return (int64_t)event_id;
    }

    return -1;
}

int32_t logging_get_entries(const char *source, int32_t severity,
                            size_t max_entries,
                            log_entry_t *entries, size_t entries_size) {
    if (!source || !entries || max_entries == 0 || entries_size == 0) {
        return -1;
    }

    if (sizeof(log_entry_t) * max_entries > entries_size) {
        return -1;
    }

    char log_file[256];
    if (_get_log_file(source, log_file, sizeof(log_file)) != 0) {
        return -1;
    }

    FILE *fp = fopen(log_file, "r");
    if (!fp) {
        return -1;
    }

    char line[512];
    int32_t entriesRead = 0;

    /* Read log entries from file */
    while (fgets(line, sizeof(line), fp) && entriesRead < (int32_t)max_entries) {
        entries[entriesRead].event_id = (uint64_t)entriesRead;
        entries[entriesRead].timestamp = time(NULL);
        entries[entriesRead].source = source;
        entries[entriesRead].message = "(file log entry)";
        entries[entriesRead].severity = severity;
        entriesRead++;
    }

    fclose(fp);
    return entriesRead;
}

int32_t logging_is_available(void) {
    /* File-based logging is always available */
    return 1;
}
