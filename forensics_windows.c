// forensics_windows.c - Basic Windows forensic collection tool
// Compiles through JOCKY pipeline for obfuscated deployment

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <iphlpapi.h>
#include <iptypes.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "psapi.lib")

// Simple XOR encryption for output strings
static void xor_encrypt(char* data, size_t len, char key) {
    for (size_t i = 0; i < len; i++) {
        data[i] ^= key;
    }
}

static void print_encrypted(const char* msg, char key) {
    size_t len = strlen(msg);
    char* buf = (char*)malloc(len + 1);
    strcpy(buf, msg);
    xor_encrypt(buf, len, key);
    xor_encrypt(buf, len, key); // XOR twice = original
    printf("%s", buf);
    free(buf);
}

// Enumerate running processes
static int enum_processes() {
    print_encrypted("=== PROCESS ENUMERATION ===\n", 0x42);
    
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        printf("Failed to create process snapshot\n");
        return -1;
    }
    
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);
    
    int count = 0;
    if (Process32First(hSnapshot, &pe)) {
        do {
            printf("[%lu] %s (Parent: %lu)\n", pe.th32ProcessID, pe.szExeFile, pe.th32ParentProcessID);
            count++;
        } while (Process32Next(hSnapshot, &pe));
    }
    
    CloseHandle(hSnapshot);
    printf("Total processes: %d\n", count);
    return count;
}

// Enumerate network connections
static int enum_network() {
    print_encrypted("\n=== NETWORK CONNECTIONS ===\n", 0x42);
    
    PMIB_TCPTABLE pTcpTable = NULL;
    DWORD dwSize = 0;
    DWORD dwRetVal = 0;
    
    // Get required size
    GetTcpTable(NULL, &dwSize, TRUE);
    
    pTcpTable = (PMIB_TCPTABLE)malloc(dwSize);
    if (!pTcpTable) return -1;
    
    dwRetVal = GetTcpTable(pTcpTable, &dwSize, TRUE);
    if (dwRetVal == NO_ERROR) {
        printf("Active TCP connections: %lu\n", pTcpTable->dwNumEntries);
        
        for (DWORD i = 0; i < pTcpTable->dwNumEntries; i++) {
            MIB_TCPROW row = pTcpTable->table[i];
            
            struct in_addr localAddr, remoteAddr;
            localAddr.S_un.S_addr = row.dwLocalAddr;
            remoteAddr.S_un.S_addr = row.dwRemoteAddr;
            
            const char* states[] = {
                "CLOSED", "LISTENING", "SYN_SENT", "SYN_RECEIVED",
                "ESTABLISHED", "FIN_WAIT1", "FIN_WAIT2", "CLOSE_WAIT",
                "CLOSING", "LAST_ACK", "TIME_WAIT", "DELETE_TCB"
            };
            const char* state_str = (row.dwState < 12) ? states[row.dwState] : "UNKNOWN";
            
            printf("%s:%lu -> %s:%lu [%s]\n",
                   inet_ntoa(localAddr), ntohs((u_short)row.dwLocalPort),
                   inet_ntoa(remoteAddr), ntohs((u_short)row.dwRemotePort),
                   state_str);
        }
    }
    
    free(pTcpTable);
    return 0;
}

// Check registry for persistence
static void check_registry_persistence() {
    print_encrypted("\n=== REGISTRY PERSISTENCE CHECK ===\n", 0x42);
    
    HKEY hKey;
    const char* runKeys[] = {
        "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
        "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnce",
        NULL
    };
    
    for (int i = 0; runKeys[i]; i++) {
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, runKeys[i], 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
            printf("\nHKLM\\%s:\n", runKeys[i]);
            
            char valueName[256];
            char valueData[1024];
            DWORD valueNameSize, valueDataSize, type;
            DWORD index = 0;
            
            while (1) {
                valueNameSize = sizeof(valueName);
                valueDataSize = sizeof(valueData);
                
                LONG ret = RegEnumValueA(hKey, index, valueName, &valueNameSize,
                                         NULL, &type, (LPBYTE)valueData, &valueDataSize);
                if (ret != ERROR_SUCCESS) break;
                
                if (type == REG_SZ) {
                    printf("  %s = %s\n", valueName, valueData);
                }
                index++;
            }
            
            RegCloseKey(hKey);
        }
    }
}

// Collect system info
static void collect_system_info() {
    print_encrypted("\n=== SYSTEM INFORMATION ===\n", 0x42);
    
    char computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(computerName);
    if (GetComputerNameA(computerName, &size)) {
        printf("Computer Name: %s\n", computerName);
    }
    
    OSVERSIONINFOA osvi;
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
    #pragma warning(disable: 4996)
    if (GetVersionExA(&osvi)) {
        printf("Windows Version: %lu.%lu (Build %lu)\n",
               osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.dwBuildNumber);
    }
    
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    printf("Processor Architecture: %u\n", si.wProcessorArchitecture);
    printf("Number of Processors: %lu\n", si.dwNumberOfProcessors);
    printf("Page Size: %lu bytes\n", si.dwPageSize);
    
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);
    if (GlobalMemoryStatusEx(&memStatus)) {
        printf("Total Physical RAM: %llu MB\n", memStatus.ullTotalPhys / (1024 * 1024));
        printf("Available RAM: %llu MB\n", memStatus.ullAvailPhys / (1024 * 1024));
    }
}

// Check for suspicious files/directories
static void check_suspicious_paths() {
    print_encrypted("\n=== SUSPICIOUS PATH CHECK ===\n", 0x42);
    
    const char* suspiciousPaths[] = {
        "C:\\Windows\\Temp",
        "C:\\Users\\Public",
        "C:\\ProgramData",
        NULL
    };
    
    for (int i = 0; suspiciousPaths[i]; i++) {
        WIN32_FIND_DATAA findData;
        HANDLE hFind = FindFirstFileA(suspiciousPaths[i], &findData);
        if (hFind != INVALID_HANDLE_VALUE) {
            printf("%s: exists\n", suspiciousPaths[i]);
            FindClose(hFind);
        }
    }
}

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    
    printf("JOCKY Forensic Collection Tool (Windows)\n");
    printf("=========================================\n\n");
    
    collect_system_info();
    enum_processes();
    enum_network();
    check_registry_persistence();
    check_suspicious_paths();
    
    printf("\n=========================================\n");
    printf("Collection complete.\n");
    
    return 0;
}
