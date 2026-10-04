#ifdef _WIN32

#include <wevtapi.h>
#include <winerror.h>
#include <winevt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include "../../common/logging.h"

/* Windows Event Log based implementation using EvtOpenLog and related APIs */

int32_t logging_clear_logs(const char *source) {
    if (!source) {
        return -1;
    }

    HANDLE hEventLog = NULL;
    DWORD flags = EvtOpenChannelPath;
    DWORD result = 0;

    /* Open event log channel by source name */
    hEventLog = EvtOpenLog(NULL, (LPCWSTR)source, flags);
    if (!hEventLog) {
        /* Fallback: try to clear using OpenEventLogA for classic logs */
        HANDLE hLog = OpenEventLogA(NULL, source);
        if (hLog) {
            if (ClearEventLogA(hLog, NULL)) {
                CloseEventLog(hLog);
                return 0;
            }
            CloseEventLog(hLog);
        }
        return -1;
    }

    /* Clear the channel */
    if (EvtClearLog(NULL, (LPCWSTR)source, NULL, 0)) {
        EvtClose(hEventLog);
        return 0;
    }

    EvtClose(hEventLog);
    return -1;
}

int64_t logging_log_event(const char *source, const char *message,
                          int32_t severity) {
    if (!source || !message) {
        return -1;
    }

    /* Windows Event Log write would require event message DLL registration
     * For now, return pseudo event ID to indicate operation attempted */
    DWORD eventId = (DWORD)GetTickCount();

    HANDLE hEventLog = RegisterEventSourceA(NULL, source);
    if (!hEventLog) {
        return -1;
    }

    /* Map severity to Windows event types */
    WORD eventType = EVENTLOG_INFORMATION_TYPE;
    if (severity == 0) {
        eventType = EVENTLOG_INFORMATION_TYPE;
    } else if (severity == 1) {
        eventType = EVENTLOG_WARNING_TYPE;
    } else if (severity == 2) {
        eventType = EVENTLOG_ERROR_TYPE;
    }

    /* Report event to log */
    if (ReportEventA(hEventLog, eventType, 0, eventId, NULL,
                     1, 0, (LPCSTR*)&message, NULL)) {
        DeregisterEventSource(hEventLog);
        return (int64_t)eventId;
    }

    DeregisterEventSource(hEventLog);
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

    /* Open event log for reading */
    HANDLE hEventLog = OpenEventLogA(NULL, source);
    if (!hEventLog) {
        return -1;
    }

    DWORD entriesRead = 0;
    BYTE buffer[4096];
    DWORD dwBytesRead = 0;
    DWORD dwFlags = EVENTLOG_BACKWARDS_READ | EVENTLOG_SEQUENTIAL_READ;

    EVENTLOGA *pEvent = NULL;
    DWORD dwOffset = 0;

    /* Read events from the log */
    if (ReadEventLogA(hEventLog, dwFlags, 0, buffer, sizeof(buffer),
                      &dwBytesRead, &dwOffset)) {
        pEvent = (EVENTLOGA *)buffer;

        while ((BYTE*)pEvent < buffer + dwBytesRead &&
               entriesRead < (DWORD)max_entries) {

            if (entriesRead < (DWORD)max_entries) {
                entries[entriesRead].event_id = pEvent->EventID;
                entries[entriesRead].timestamp = (time_t)pEvent->TimeWritten;
                entries[entriesRead].severity = (int32_t)pEvent->EventType;
                entries[entriesRead].source = source;
                entries[entriesRead].message = "(Windows Event Log Entry)";
                entriesRead++;
            }

            pEvent = (EVENTLOGA *)((BYTE*)pEvent + pEvent->Length);
        }
    }

    CloseEventLog(hEventLog);
    return (int32_t)entriesRead;
}

int32_t logging_is_available(void) {
    /* Check if Event Log service is accessible */
    HANDLE hTest = OpenEventLogA(NULL, "System");
    if (hTest) {
        CloseEventLog(hTest);
        return 1;
    }
    return 0;
}

#endif /* _WIN32 */
