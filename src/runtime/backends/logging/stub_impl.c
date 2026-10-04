#include <stdint.h>
#include <stddef.h>
#include "../../common/logging.h"

/* Stub logging implementation
 * All functions return -1 or 0 to indicate failure/unavailability.
 * No external dependencies, always compiles.
 */

int32_t logging_clear_logs(const char *source) {
    (void)source;  /* Suppress unused parameter warning */
    return -1;
}

int64_t logging_log_event(const char *source, const char *message,
                          int32_t severity) {
    (void)source;
    (void)message;
    (void)severity;
    return -1;
}

int32_t logging_get_entries(const char *source, int32_t severity,
                            size_t max_entries,
                            log_entry_t *entries, size_t entries_size) {
    (void)source;
    (void)severity;
    (void)max_entries;
    (void)entries;
    (void)entries_size;
    return -1;
}

int32_t logging_is_available(void) {
    return 0;
}
