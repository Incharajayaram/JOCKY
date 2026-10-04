#ifndef JOCKY_RUNTIME_COMMON_LOGGING_H
#define JOCKY_RUNTIME_COMMON_LOGGING_H

#include <stdint.h>
#include <stddef.h>
#include <time.h>

/* Event logging API
 * Platform-agnostic interface for system event logging.
 * Implementations may use Windows Event Log, syslog, or custom backends.
 */

/* Log entry structure for querying and returning log data */
typedef struct {
    uint64_t event_id;
    time_t timestamp;
    const char *source;
    const char *message;
    int32_t severity;
} log_entry_t;

/* Clear system event logs for specified source
 * Removes audit trail from event log or syslog.
 * Returns: 0 on success, -1 on error */
int32_t logging_clear_logs(const char *source);

/* Log a security-relevant event to system log
 * Records activity with timestamp and severity level.
 * Returns: event ID on success, -1 on error */
int64_t logging_log_event(const char *source, const char *message,
                          int32_t severity);

/* Get recent log entries for analysis or exfiltration
 * Retrieves up to max_entries recent logs matching source/severity filter.
 * Returns: number of entries retrieved, -1 on error */
int32_t logging_get_entries(const char *source, int32_t severity,
                            size_t max_entries,
                            log_entry_t *entries, size_t entries_size);

/* Check if logging is enabled/accessible on system
 * Returns: 1 if accessible, 0 if not, -1 on error */
int32_t logging_is_available(void);

#endif /* JOCKY_RUNTIME_COMMON_LOGGING_H */
