/*
 * JOCKY Pack Stub — in-PE runtime decryptor
 *
 * This translation unit goes entirely into the .jstub PE section (the
 * #pragma below routes all code here away from .text).  packPE() skips
 * .jstub when encrypting and sets AddressOfEntryPoint to .jstub's VirtAddr,
 * so the OS loader calls jocky_pack_stub_entry() before any user code runs.
 *
 * Execution sequence after the loader hands control here:
 *   1. GetModuleHandleA(NULL)  → image base
 *   2. Walk PE section table   → locate .jkey (key + OEP RVA) and .text
 *   3. VirtualProtect(.text)   → PAGE_EXECUTE_READWRITE
 *   4. RC4-decrypt .text       (same key / algorithm as packPE used)
 *   5. Restore PAGE_EXECUTE_READ
 *   6. FlushInstructionCache
 *   7. Call (image_base + oep_rva)
 *
 * .rdata is NOT encrypted by packPE so the IAT is intact when this stub
 * runs; VirtualProtect / FlushInstructionCache / GetModuleHandleA are all
 * resolved normally by the loader before the entry point is called.
 */
#ifdef _WIN32
#include <windows.h>
#include <stdint.h>

/* Route every function in this TU into .jstub so none of them end up in
 * the encrypted .text section. */
#if defined(__clang__)
#  pragma clang section text=".jstub"
#elif defined(_MSC_VER)
#  pragma code_seg(".jstub")
#endif

/* ── Minimal helpers (must not live in .text) ─────────────────────────── */

static int jstub_memeq(const char *a, const char *b, int n)
{
    while (n--) if (*a++ != *b++) return 0;
    return 1;
}

static void jstub_rc4(uint8_t *data, uint32_t len,
                      const uint8_t *key, uint32_t klen)
{
    uint8_t S[256];
    int i, j = 0;
    for (i = 0; i < 256; i++) S[i] = (uint8_t)i;
    for (i = 0; i < 256; i++) {
        j = (j + S[i] + key[i % klen]) & 0xFF;
        uint8_t t = S[i]; S[i] = S[j]; S[j] = t;
    }
    i = 0; j = 0;
    for (uint32_t k = 0; k < len; k++) {
        i = (i + 1) & 0xFF;
        j = (j + S[i]) & 0xFF;
        uint8_t t = S[i]; S[i] = S[j]; S[j] = t;
        data[k] ^= S[(S[i] + S[j]) & 0xFF];
    }
}

/* ── PE section header (packed) ───────────────────────────────────────── */

#pragma pack(push, 1)
typedef struct {
    char     name[8];
    uint32_t virt_size, virt_addr;
    uint32_t raw_size,  raw_ptr;
    uint32_t reloc_ptr, linenum_ptr;
    uint16_t num_relocs, num_linenums;
    uint32_t characteristics;
} jstub_sec_hdr_t;
#pragma pack(pop)

/* ── Entry point ──────────────────────────────────────────────────────── */

/*
 * jocky_pack_stub_entry — packed PE entry point.
 *
 * packPE() sets AddressOfEntryPoint to .jstub's VirtAddr (the address of
 * this function) and stores the real OEP RVA in .jkey[16..19] so we can
 * jump to it after decryption.
 */
void jocky_pack_stub_entry(void)
{
    /* 1. Image base via the PEB-backed handle. */
    uint8_t *base = (uint8_t *)GetModuleHandleA(NULL);
    if (!base) return;

    /* 2. Locate section headers.
     *
     * PE layout:
     *   base[0x3C]                    → e_lfanew  (offset of "PE\0\0")
     *   base + e_lfanew               → "PE\0\0" signature
     *   base + e_lfanew + 4           → COFF header (20 bytes)
     *     + 2  : NumberOfSections
     *     + 16 : SizeOfOptionalHeader
     *   base + e_lfanew + 24 + opt_sz → first section header
     */
    uint32_t e_lfanew = *(uint32_t *)(base + 0x3C);
    uint8_t *nt       = base + e_lfanew;

    uint16_t num_sec  = *(uint16_t *)(nt + 6);   /* COFF +2  */
    uint16_t opt_sz   = *(uint16_t *)(nt + 20);  /* COFF +16 */
    jstub_sec_hdr_t *secs = (jstub_sec_hdr_t *)(nt + 24 + opt_sz);

    const uint8_t *rc4_key = NULL;
    uint32_t       oep_rva  = 0;
    uint8_t       *text_va  = NULL;
    uint32_t       text_sz  = 0;

    for (uint16_t i = 0; i < num_sec; i++) {
        if (jstub_memeq(secs[i].name, ".jkey\0\0\0", 8)) {
            /* .jkey layout: [0..15] RC4 key, [16..19] OEP RVA */
            uint8_t *jkey = base + secs[i].virt_addr;
            rc4_key = jkey;
            oep_rva = *(uint32_t *)(jkey + 16);
        }
        if (jstub_memeq(secs[i].name, ".text\0\0\0", 8)) {
            text_va = base + secs[i].virt_addr;
            /* Prefer VirtualSize; fall back to SizeOfRawData. */
            text_sz = secs[i].virt_size ? secs[i].virt_size : secs[i].raw_size;
        }
    }

    /* Bail out gracefully if the section layout is unexpected. */
    if (!rc4_key || !text_va || !oep_rva) return;

    /* 3. Make .text writable so we can decrypt in place. */
    DWORD old_prot = 0;
    VirtualProtect(text_va, text_sz, PAGE_EXECUTE_READWRITE, &old_prot);

    /* 4. RC4-decrypt .text with the 16-byte key from .jkey. */
    jstub_rc4(text_va, text_sz, rc4_key, 16);

    /* 5. Restore the original protection. */
    VirtualProtect(text_va, text_sz, PAGE_EXECUTE_READ, &old_prot);

    /* 6. Flush the instruction cache so the CPU sees the decrypted bytes. */
    FlushInstructionCache(GetCurrentProcess(), text_va, text_sz);

    /* 7. Transfer control to the original entry point. */
    typedef void (*oep_fn_t)(void);
    ((oep_fn_t)(base + oep_rva))();
}

#if defined(_MSC_VER)
#pragma code_seg()  /* restore default section for anything after */
#endif

#endif /* _WIN32 */
