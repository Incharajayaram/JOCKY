#define _GNU_SOURCE
#include "jocky_ai.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <link.h>
#endif

/* ============================================================================
   Runtime Code Section Discovery + Permission Management
   ============================================================================ */

typedef struct { uint8_t* base; size_t size; uint32_t orig_prot; } code_region_t;

#define MAX_CODE_REGIONS 16
static code_region_t g_regions[MAX_CODE_REGIONS];
static int           g_region_count  = 0;
static int           g_regions_ready = 0;

static size_t sys_page_size(void) {
#ifdef _WIN32
    SYSTEM_INFO si; GetSystemInfo(&si); return (size_t)si.dwPageSize;
#else
    return (size_t)sysconf(_SC_PAGESIZE);
#endif
}

#ifndef _WIN32
static int collect_exec_phdr(struct dl_phdr_info* info, size_t sz, void* arg) {
    (void)sz; (void)arg;
    /* Only patch the main executable, not shared libraries */
    if (info->dlpi_name && info->dlpi_name[0] != '\0') return 0;
    for (int i = 0; i < info->dlpi_phnum; i++) {
        const ElfW(Phdr)* ph = &info->dlpi_phdr[i];
        if (ph->p_type != PT_LOAD || !(ph->p_flags & PF_X)) continue;
        if (g_region_count >= MAX_CODE_REGIONS) return 0;
        g_regions[g_region_count].base      = (uint8_t*)(info->dlpi_addr + ph->p_vaddr);
        g_regions[g_region_count].size      = ph->p_filesz;
        g_regions[g_region_count].orig_prot = PROT_READ | PROT_EXEC;
        g_region_count++;
    }
    return 0;
}
#endif

static void load_code_regions(void) {
    if (g_regions_ready) return;
    g_region_count = 0;
#ifdef _WIN32
    uint8_t* base = (uint8_t*)GetModuleHandle(NULL);
    IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)base;
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) goto done;
    IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE) goto done;
    IMAGE_SECTION_HEADER* sec = IMAGE_FIRST_SECTION(nt);
    for (WORD i = 0; i < nt->FileHeader.NumberOfSections; i++, sec++) {
        if (!(sec->Characteristics & IMAGE_SCN_CNT_CODE)) continue;
        if (g_region_count >= MAX_CODE_REGIONS) break;
        g_regions[g_region_count].base      = base + sec->VirtualAddress;
        g_regions[g_region_count].size      = sec->Misc.VirtualSize;
        g_regions[g_region_count].orig_prot = PAGE_EXECUTE_READ;
        g_region_count++;
    }
done:;
#else
    dl_iterate_phdr(collect_exec_phdr, NULL);
#endif
    g_regions_ready = 1;
}

static int make_rwx(uint8_t* base, size_t size, uint32_t* old_prot) {
#ifdef _WIN32
    DWORD old;
    if (!VirtualProtect(base, size, PAGE_EXECUTE_READWRITE, &old)) return -1;
    *old_prot = (uint32_t)old;
    return 0;
#else
    size_t ps    = sys_page_size();
    uintptr_t pg = (uintptr_t)base & ~((uintptr_t)(ps - 1));
    size_t len   = ((uintptr_t)base + size - pg + ps - 1) & ~((uintptr_t)(ps - 1));
    if (mprotect((void*)pg, len, PROT_READ | PROT_WRITE | PROT_EXEC) != 0) return -1;
    *old_prot = PROT_READ | PROT_EXEC;
    return 0;
#endif
}

static void restore_rwx(uint8_t* base, size_t size, uint32_t old_prot) {
#ifdef _WIN32
    DWORD dummy;
    VirtualProtect(base, size, (DWORD)old_prot, &dummy);
#else
    size_t ps    = sys_page_size();
    uintptr_t pg = (uintptr_t)base & ~((uintptr_t)(ps - 1));
    size_t len   = ((uintptr_t)base + size - pg + ps - 1) & ~((uintptr_t)(ps - 1));
    mprotect((void*)pg, len, (int)old_prot);
#endif
}

static void flush_icache_range(uint8_t* addr, size_t len) {
#ifdef _WIN32
    FlushInstructionCache(GetCurrentProcess(), addr, len);
#elif defined(__GNUC__)
    __builtin___clear_cache((char*)addr, (char*)(addr + len));
#else
    (void)addr; (void)len;
#endif
}

/* ============================================================================
   Forward declarations (allow apply_mutation to call any stub in any order)
   ============================================================================ */

static uint64_t jocky_ai_get_current_time_ms(void);
static void jocky_ai_mutate_stack_frame(uint32_t seed);
static void jocky_ai_mutate_syscall_encoding(uint32_t seed);
static void jocky_ai_mutate_memory_access(uint32_t seed);
static void jocky_ai_mutate_api_call_order(uint32_t seed);
static void jocky_ai_mutate_register_usage(uint32_t seed);
static void jocky_ai_inject_code_padding(uint32_t obfuscation_level);
static void jocky_ai_vary_instruction_encoding(uint32_t seed);
static void jocky_ai_shuffle_function_order(uint32_t seed);

/* ============================================================================
   Global AI State
   ============================================================================ */

static struct {
    JOCKY_AI_MODEL* model;
    JOCKY_AI_TELEMETRY latest_telemetry;
    JOCKY_AI_RISK_LEVEL current_risk;
    JOCKY_STRATEGY current_strategy;
    JOCKY_AI_STATS statistics;

    uint64_t syscall_count;
    uint64_t network_bytes_sent;
    uint64_t network_bytes_recv;
    uint32_t file_io_count;
    uint32_t registry_io_count;
    uint32_t edr_alerts;
    uint32_t blocked_ops;

    uint64_t collection_start_ms;
    uint64_t last_collection_ms;
} g_ai_state = {0};

/* ============================================================================
   Core AI Functions
   ============================================================================ */

bool jocky_ai_init(const uint8_t* model_data, size_t model_size)
{
    if (!model_data || model_size < sizeof(JOCKY_AI_MODEL)) return false;

    g_ai_state.model = (JOCKY_AI_MODEL*)malloc(sizeof(JOCKY_AI_MODEL));
    if (!g_ai_state.model) return false;

    memcpy(g_ai_state.model, model_data, sizeof(JOCKY_AI_MODEL));

    if (g_ai_state.model->weights_size > 0) {
        g_ai_state.model->weights = (uint8_t*)malloc(g_ai_state.model->weights_size);
        if (!g_ai_state.model->weights) { free(g_ai_state.model); return false; }
        memcpy(g_ai_state.model->weights, model_data + sizeof(JOCKY_AI_MODEL),
               g_ai_state.model->weights_size);
    }

    g_ai_state.current_risk     = JOCKY_AI_RISK_LOW;
    g_ai_state.current_strategy = JOCKY_STRAT_BASELINE;
    g_ai_state.collection_start_ms = jocky_ai_get_current_time_ms();
    return true;
}

void jocky_ai_shutdown(void)
{
    if (g_ai_state.model) {
        if (g_ai_state.model->weights) free(g_ai_state.model->weights);
        free(g_ai_state.model);
        g_ai_state.model = NULL;
    }
    memset(&g_ai_state, 0, sizeof(g_ai_state));
}

static uint64_t jocky_ai_get_current_time_ms(void)
{
#ifdef _WIN32
    return GetTickCount64();
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000ULL) + (tv.tv_usec / 1000ULL);
#endif
}

bool jocky_ai_collect_telemetry(JOCKY_AI_TELEMETRY* out_telemetry)
{
    if (!out_telemetry) return false;

    uint64_t now_ms     = jocky_ai_get_current_time_ms();
    uint64_t elapsed_ms = now_ms - g_ai_state.collection_start_ms;
    if (elapsed_ms == 0) elapsed_ms = 1;

    memset(out_telemetry, 0, sizeof(*out_telemetry));

    out_telemetry->syscall_frequency    = (float)(g_ai_state.syscall_count * 1000.0 / elapsed_ms);
    out_telemetry->network_entropy      = (g_ai_state.network_bytes_sent > 0) ?
                                          fabs(log((float)g_ai_state.network_bytes_sent)) : 0.0f;
    out_telemetry->memory_pattern_score = (float)(g_ai_state.syscall_count % 100) / 100.0f;
    out_telemetry->file_io_score        = (float)(g_ai_state.file_io_count) / 100.0f;
    if (out_telemetry->file_io_score > 1.0f) out_telemetry->file_io_score = 1.0f;
    out_telemetry->registry_io_score    = (float)(g_ai_state.registry_io_count) / 50.0f;
    if (out_telemetry->registry_io_score > 1.0f) out_telemetry->registry_io_score = 1.0f;
    out_telemetry->blocked_operations   = g_ai_state.blocked_ops;
    out_telemetry->alert_count          = g_ai_state.edr_alerts;
    out_telemetry->crash_likelihood     = (float)g_ai_state.blocked_ops / 1000.0f;
    if (out_telemetry->crash_likelihood > 1.0f) out_telemetry->crash_likelihood = 1.0f;
    out_telemetry->timestamp_ms         = now_ms;
    g_ai_state.last_collection_ms       = now_ms;

    memcpy(&g_ai_state.latest_telemetry, out_telemetry, sizeof(*out_telemetry));
    return true;
}

static float jocky_ai_tree_inference(const JOCKY_AI_TELEMETRY* telemetry)
{
    if (!telemetry) return 0.5f;

    float score = 0.0f;

    if (telemetry->blocked_operations > 8) {
        score += 0.5f;
        if (telemetry->alert_count > 4)            score += 0.25f;
        else                                        score += 0.15f;
        if (telemetry->crash_likelihood > 0.5f)    score += 0.1f;
    } else if (telemetry->blocked_operations > 3) {
        score += 0.35f;
        if (telemetry->syscall_frequency > 800.0f)      score += 0.2f;
        else if (telemetry->syscall_frequency > 400.0f) score += 0.1f;
        if (telemetry->alert_count > 2)                 score += 0.1f;
    } else {
        if      (telemetry->syscall_frequency > 1500.0f) score += 0.35f;
        else if (telemetry->syscall_frequency > 800.0f)  score += 0.25f;
        else if (telemetry->syscall_frequency > 400.0f)  score += 0.15f;
        else                                              score += 0.05f;

        if      (telemetry->network_entropy > 6.0f)  score += 0.15f;
        else if (telemetry->network_entropy > 4.0f)  score += 0.08f;
        if      (telemetry->memory_pattern_score > 0.7f) score += 0.1f;

        score += (float)(telemetry->alert_count) * 0.05f;
    }

    if (score > 1.0f) score = 1.0f;
    if (score < 0.0f) score = 0.0f;
    return score;
}

float jocky_ai_score_threat(const JOCKY_AI_TELEMETRY* telemetry)
{
    if (!telemetry) return 0.5f;
    g_ai_state.statistics.total_predictions++;
    return jocky_ai_tree_inference(telemetry);
}

JOCKY_AI_RISK_LEVEL jocky_ai_classify_threat(const JOCKY_AI_TELEMETRY* telemetry)
{
    float score = jocky_ai_score_threat(telemetry);
    if (score < 0.25f)  return JOCKY_AI_RISK_LOW;
    if (score < 0.5f)   return JOCKY_AI_RISK_MEDIUM;
    if (score < 0.75f)  return JOCKY_AI_RISK_HIGH;
    return JOCKY_AI_RISK_CRITICAL;
}

JOCKY_STRATEGY jocky_ai_recommend_strategy(const JOCKY_AI_TELEMETRY* telemetry)
{
    switch (jocky_ai_classify_threat(telemetry)) {
        case JOCKY_AI_RISK_LOW:      return JOCKY_STRAT_STEALTH;
        case JOCKY_AI_RISK_MEDIUM:   return JOCKY_STRAT_HYBRID;
        case JOCKY_AI_RISK_HIGH:     return JOCKY_STRAT_AGGRESSIVE;
        case JOCKY_AI_RISK_CRITICAL: return JOCKY_STRAT_AI_ADAPTIVE;
        default:                     return JOCKY_STRAT_BASELINE;
    }
}

bool jocky_ai_generate_mutation(const JOCKY_AI_TELEMETRY* telemetry,
                                 JOCKY_AI_MUTATION_STRATEGY* out_strategy)
{
    if (!telemetry || !out_strategy) return false;

    memset(out_strategy, 0, sizeof(*out_strategy));
    out_strategy->strategy = jocky_ai_recommend_strategy(telemetry);

    JOCKY_AI_RISK_LEVEL risk = jocky_ai_classify_threat(telemetry);

    if (risk >= JOCKY_AI_RISK_LOW)      out_strategy->technique_mask |= 0x01;
    if (risk >= JOCKY_AI_RISK_MEDIUM) { out_strategy->technique_mask |= 0x02;
                                        out_strategy->technique_mask |= 0x04; }
    if (risk >= JOCKY_AI_RISK_HIGH)   { out_strategy->technique_mask |= 0x08;
                                        out_strategy->technique_mask |= 0x10;
                                        out_strategy->technique_mask |= 0x20; }
    if (risk == JOCKY_AI_RISK_CRITICAL){ out_strategy->technique_mask |= 0x40;
                                         out_strategy->technique_mask |= 0x80; }

    out_strategy->code_layout_seed     = (uint32_t)rand();
    out_strategy->api_call_order_seed  = (uint32_t)rand();
    out_strategy->obfuscation_level    = 10 - ((risk + 1) * 2);
    if ((int)out_strategy->obfuscation_level < 0) out_strategy->obfuscation_level = 10;

    g_ai_state.current_strategy = out_strategy->strategy;
    g_ai_state.statistics.mutations_applied++;
    return true;
}

bool jocky_ai_apply_mutation(const JOCKY_AI_MUTATION_STRATEGY* strategy)
{
    if (!strategy) return false;

    srand(strategy->code_layout_seed);
    uint32_t mask = strategy->technique_mask;

    if (mask & 0x01) jocky_ai_mutate_stack_frame(strategy->code_layout_seed);
    if (mask & 0x02) jocky_ai_mutate_syscall_encoding(strategy->api_call_order_seed);
    if (mask & 0x04) jocky_ai_mutate_memory_access(strategy->code_layout_seed);
    if (mask & 0x08) jocky_ai_mutate_api_call_order(strategy->api_call_order_seed);
    if (mask & 0x10) jocky_ai_mutate_register_usage(strategy->code_layout_seed);
    if (mask & 0x20) jocky_ai_inject_code_padding(strategy->obfuscation_level);
    if (mask & 0x40) jocky_ai_vary_instruction_encoding(strategy->code_layout_seed);
    if (mask & 0x80) jocky_ai_shuffle_function_order(strategy->api_call_order_seed);

    g_ai_state.statistics.mutations_applied++;
    return true;
}

/* ============================================================================
   Mutation 1: NOP Sled Variation
   Scan for runs of single-byte NOPs (0x90) and replace with Intel multi-byte
   NOP encodings. Changes binary signature without affecting execution.
   ============================================================================ */
static void jocky_ai_inject_code_padding(uint32_t obfuscation_level)
{
    if (obfuscation_level == 0) return;
    load_code_regions();

    /* Intel-recommended multi-byte NOP encodings (volumes 2A/2B) */
    static const uint8_t NOP2[] = {0x66, 0x90};
    static const uint8_t NOP3[] = {0x0F, 0x1F, 0x00};
    static const uint8_t NOP4[] = {0x0F, 0x1F, 0x40, 0x00};
    static const uint8_t NOP5[] = {0x0F, 0x1F, 0x44, 0x00, 0x00};
    static const uint8_t NOP6[] = {0x66, 0x0F, 0x1F, 0x44, 0x00, 0x00};
    static const uint8_t NOP7[] = {0x0F, 0x1F, 0x80, 0x00, 0x00, 0x00, 0x00};
    static const uint8_t NOP8[] = {0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00};

    static const struct { const uint8_t* seq; int len; } NOPS[] = {
        {NOP2,2},{NOP3,3},{NOP4,4},{NOP5,5},{NOP6,6},{NOP7,7},{NOP8,8}
    };

    uint32_t patches = 0;
    uint32_t max_patches = obfuscation_level * 24;

    for (int r = 0; r < g_region_count && patches < max_patches; r++) {
        uint8_t* p   = g_regions[r].base;
        uint8_t* end = p + g_regions[r].size;
        uint32_t saved = 0;
        int made_rwx = 0;

        while (p + 3 <= end && patches < max_patches) {
            if (*p != 0x90) { p++; continue; }

            /* Count run length of single-byte NOPs (max 8) */
            int run = 0;
            while (p + run < end && p[run] == 0x90 && run < 8) run++;
            if (run < 2) { p += run; continue; }

            if (!made_rwx) {
                if (make_rwx(g_regions[r].base, g_regions[r].size, &saved) != 0) break;
                made_rwx = 1;
            }

            /* Pick multi-byte NOP variant that fits the run, cycling by patch count */
            int idx = (run - 2) % 7;                    /* 0..6 → NOP2..NOP8 */
            idx = (idx + patches) % 7;                  /* cycle variants */
            while (NOPS[idx].len > run) idx = (idx + 6) % 7;  /* ensure it fits */

            memcpy(p, NOPS[idx].seq, NOPS[idx].len);
            /* Fill leftover bytes in the run with 1-byte NOPs */
            for (int i = NOPS[idx].len; i < run; i++) p[i] = 0x90;

            flush_icache_range(p, run);
            p += run;
            patches++;
        }

        if (made_rwx) restore_rwx(g_regions[r].base, g_regions[r].size, saved);
    }
}

/* ============================================================================
   Mutation 2: Zero-Register Instruction Encoding Variation
   Replace: 48 C7 Cx 00 00 00 00  (mov r64, 0 — 7 bytes)
   With:    31 Cx 0F 1F 44 00 00  (xor r32,r32 + 5-byte NOP — 7 bytes)
   Semantically identical on x64: xor r32,r32 zeroes upper 32 bits too.
   ============================================================================ */
static void jocky_ai_vary_instruction_encoding(uint32_t seed)
{
    load_code_regions();
    srand(seed);

    /* xor opcode (31 /r) ModRM for each register (mod=11, reg=dst, rm=dst) */
    /* C7 Cx: x = reg index; 31 Cx for xor. Skip rsp(4) and rbp(5). */
    static const uint8_t xor_modrm[8] = {0xC0,0xC9,0xD2,0xDB,0xFF,0xFF,0xF6,0xFF};
    /* rsp(4) and rdi with REX.R(7 as 0xFF) — skip those */

    uint32_t patches = 0;

    for (int r = 0; r < g_region_count; r++) {
        uint8_t* p   = g_regions[r].base;
        uint8_t* end = p + g_regions[r].size;
        uint32_t saved = 0;
        int made_rwx = 0;

        while (p + 7 <= end) {
            /* Match: REX.W + MOV r/m64, imm32 with mod=11 and imm32=0 */
            if (p[0] != 0x48 || p[1] != 0xC7) { p++; continue; }
            uint8_t modrm = p[2];
            if ((modrm & 0xC0) != 0xC0) { p += 3; continue; } /* require mod=11 */
            if (p[3] || p[4] || p[5] || p[6]) { p += 7; continue; } /* imm32 must be 0 */

            int reg = modrm & 0x07;
            if (xor_modrm[reg] == 0xFF) { p += 7; continue; }

            if (!made_rwx) {
                if (make_rwx(g_regions[r].base, g_regions[r].size, &saved) != 0) break;
                made_rwx = 1;
            }

            /* Patch: xor r32,r32 (2B) + 5-byte NOP (5B) = 7 bytes */
            p[0] = 0x31;
            p[1] = xor_modrm[reg];
            p[2] = 0x0F; p[3] = 0x1F; p[4] = 0x44; p[5] = 0x00; p[6] = 0x00;
            flush_icache_range(p, 7);
            p += 7;
            patches++;
            if (patches >= 64) goto next_region_vary;
        }
next_region_vary:
        if (made_rwx) restore_rwx(g_regions[r].base, g_regions[r].size, saved);
    }
}

/* ============================================================================
   Mutation 3: Syscall Alignment Byte Variation
   Find: 0F 05 90 90  (syscall + two single-byte NOPs)
   Replace the two NOPs with a 2-byte NOP (66 90).
   Changes per-syscall signature bytes without touching the syscall itself.
   ============================================================================ */
static void jocky_ai_mutate_syscall_encoding(uint32_t seed)
{
    load_code_regions();
    srand(seed);

    for (int r = 0; r < g_region_count; r++) {
        uint8_t* p   = g_regions[r].base;
        uint8_t* end = p + g_regions[r].size;
        uint32_t saved = 0;
        int made_rwx = 0;
        int patches = 0;

        while (p + 4 <= end) {
            /* syscall (0F 05) followed by two single-byte NOPs */
            if (p[0] != 0x0F || p[1] != 0x05 || p[2] != 0x90 || p[3] != 0x90) {
                p++; continue;
            }

            if (!made_rwx) {
                if (make_rwx(g_regions[r].base, g_regions[r].size, &saved) != 0) break;
                made_rwx = 1;
            }

            /* Replace 90 90 with 66 90 (Intel 2-byte NOP) */
            p[2] = 0x66;
            /* p[3] stays 0x90 — together 66 90 = 2-byte NOP */
            flush_icache_range(p + 2, 2);
            p += 4;
            patches++;
            if (patches >= 48) break;
        }

        if (made_rwx) restore_rwx(g_regions[r].base, g_regions[r].size, saved);
    }
}

/* ============================================================================
   Mutation 4: Function Prologue Alignment Variation
   Find: 90 90 55 48 89 E5  (2 NOPs before push rbp; mov rbp,rsp)
   Replace the 2 NOPs with the Intel 2-byte NOP (66 90).
   Varies function-entry alignment signatures.
   ============================================================================ */
static void jocky_ai_mutate_stack_frame(uint32_t seed)
{
    load_code_regions();
    srand(seed);

    int patches = 0;
    for (int r = 0; r < g_region_count; r++) {
        uint8_t* p   = g_regions[r].base;
        uint8_t* end = p + g_regions[r].size;
        uint32_t saved = 0;
        int made_rwx = 0;

        while (p + 6 <= end) {
            /* Two 1-byte NOPs before standard frame setup */
            if (p[0] != 0x90 || p[1] != 0x90 ||
                p[2] != 0x55 || p[3] != 0x48 || p[4] != 0x89 || p[5] != 0xE5) {
                p++; continue;
            }

            if (!made_rwx) {
                if (make_rwx(g_regions[r].base, g_regions[r].size, &saved) != 0) break;
                made_rwx = 1;
            }

            p[0] = 0x66;  /* 66 90 = Intel 2-byte NOP */
            /* p[1] stays 0x90 */
            flush_icache_range(p, 2);
            p += 6;
            patches++;
            if (patches >= 64) break;
        }

        if (made_rwx) restore_rwx(g_regions[r].base, g_regions[r].size, saved);
    }
}

/* ============================================================================
   Mutation 5: API Call Order Randomization
   Applies the 4 code-patching mutations in seed-shuffled order.
   Different orderings produce different intermediate binary states.
   ============================================================================ */
static void jocky_ai_mutate_api_call_order(uint32_t seed)
{
    srand(seed);

    typedef void (*mut_fn)(uint32_t);
    mut_fn fns[4] = {
        jocky_ai_inject_code_padding,
        jocky_ai_vary_instruction_encoding,
        jocky_ai_mutate_syscall_encoding,
        jocky_ai_mutate_stack_frame,
    };

    /* Fisher-Yates shuffle */
    for (int i = 3; i > 0; i--) {
        int j = rand() % (i + 1);
        mut_fn tmp = fns[i]; fns[i] = fns[j]; fns[j] = tmp;
    }

    for (int i = 0; i < 4; i++) fns[i](seed ^ (uint32_t)(i * 0x9E3779B9u));
}

/* ============================================================================
   Mutation 6: Adjacent XOR Pair Swapping
   Find: 31 Cx 31 Cy  (two consecutive xor r32,r32 for different registers)
   Swap their order — the registers are independent so behavior is unchanged.
   Varies instruction stream without affecting register values.
   ============================================================================ */
static void jocky_ai_mutate_register_usage(uint32_t seed)
{
    load_code_regions();
    srand(seed);

    int patches = 0;
    for (int r = 0; r < g_region_count; r++) {
        uint8_t* p   = g_regions[r].base;
        uint8_t* end = p + g_regions[r].size;
        uint32_t saved = 0;
        int made_rwx = 0;

        while (p + 4 <= end) {
            /* Two consecutive xor r32,r32 instructions (31 Cx 31 Cy) */
            if (p[0] != 0x31 || p[2] != 0x31) { p++; continue; }
            uint8_t m0 = p[1], m1 = p[3];
            /* Both must be mod=11, reg=rm (self-xor pattern) */
            if ((m0 & 0xC0) != 0xC0 || (m1 & 0xC0) != 0xC0) { p++; continue; }
            if ((m0 & 0x07) != ((m0 >> 3) & 0x07)) { p++; continue; }
            if ((m1 & 0x07) != ((m1 >> 3) & 0x07)) { p++; continue; }
            /* Must be different registers */
            if ((m0 & 0x07) == (m1 & 0x07)) { p += 4; continue; }

            if (!made_rwx) {
                if (make_rwx(g_regions[r].base, g_regions[r].size, &saved) != 0) break;
                made_rwx = 1;
            }

            /* Swap the two instructions */
            p[1] = m1; p[3] = m0;
            flush_icache_range(p, 4);
            p += 4;
            patches++;
            if (patches >= 48) break;
        }

        if (made_rwx) restore_rwx(g_regions[r].base, g_regions[r].size, saved);
    }
}

/* ============================================================================
   Mutation 7: Heap Layout Variation
   Randomize allocation sizes and patterns to vary the process heap footprint.
   Affects memory-based behavioral fingerprinting.
   ============================================================================ */
static void jocky_ai_mutate_memory_access(uint32_t seed)
{
    srand(seed);

    /* Allocate and release blocks in a seed-determined pattern */
    void* bufs[16];
    int   count = 0;

    for (int i = 0; i < 16; i++) {
        size_t sz = 32 + (size_t)(rand() % 480);  /* 32..511 bytes */
        bufs[count] = malloc(sz);
        if (!bufs[count]) break;
        /* Fill with seed-derived byte to vary heap contents */
        memset(bufs[count], (int)((seed >> (i & 7)) & 0xFF), sz);
        count++;
    }

    /* Free in a shuffled order (varies heap freelist state) */
    for (int i = count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        void* tmp = bufs[i]; bufs[i] = bufs[j]; bufs[j] = tmp;
    }
    for (int i = 0; i < count; i++) free(bufs[i]);
}

/* ============================================================================
   Mutation 8: Function Entry Sequence Diversification
   Apply stack-frame and NOP-sled mutations with an inverted seed, producing
   a complementary set of patches that together vary the full entry signature.
   ============================================================================ */
static void jocky_ai_shuffle_function_order(uint32_t seed)
{
    /* Vary which function entry NOPs get patched by rotating the seed */
    jocky_ai_mutate_stack_frame(~seed);
    jocky_ai_inject_code_padding((seed & 0xF) ? seed & 0xF : 3u);
    /* Third pass: vary adjacent xor pairs with complementary seed */
    jocky_ai_mutate_register_usage(seed ^ 0xDEADBEEFu);
}

/* ============================================================================
   Telemetry Recording
   ============================================================================ */

void jocky_ai_record_syscall(uint64_t syscall_id)
{
    (void)syscall_id;
    g_ai_state.syscall_count++;
}

void jocky_ai_record_network(uint32_t bytes_sent, uint32_t bytes_recv)
{
    g_ai_state.network_bytes_sent += bytes_sent;
    g_ai_state.network_bytes_recv += bytes_recv;
}

void jocky_ai_record_file_io(const char* operation, const char* filename)
{
    (void)operation; (void)filename;
    g_ai_state.file_io_count++;
}

void jocky_ai_record_registry(const char* operation, const char* keypath)
{
    (void)operation; (void)keypath;
    g_ai_state.registry_io_count++;
}

void jocky_ai_record_edr_alert(uint32_t alert_type)
{
    (void)alert_type;
    g_ai_state.edr_alerts++;
}

void jocky_ai_record_blocked_operation(uint32_t syscall_id, uint32_t error_code)
{
    (void)syscall_id; (void)error_code;
    g_ai_state.blocked_ops++;
}

bool jocky_ai_predict_next_mutation(JOCKY_AI_MUTATION_STRATEGY* out_strategy)
{
    JOCKY_AI_TELEMETRY telemetry;
    if (!jocky_ai_collect_telemetry(&telemetry)) return false;
    return jocky_ai_generate_mutation(&telemetry, out_strategy);
}

JOCKY_AI_RISK_LEVEL jocky_ai_get_current_risk(void)
{
    JOCKY_AI_TELEMETRY telemetry;
    if (jocky_ai_collect_telemetry(&telemetry))
        g_ai_state.current_risk = jocky_ai_classify_threat(&telemetry);
    return g_ai_state.current_risk;
}

JOCKY_STRATEGY jocky_ai_get_current_strategy(void)
{
    return g_ai_state.current_strategy;
}

/* ============================================================================
   Statistics + Model Loading
   ============================================================================ */

bool jocky_ai_get_statistics(JOCKY_AI_STATS* out_stats)
{
    if (!out_stats) return false;
    memcpy(out_stats, &g_ai_state.statistics, sizeof(*out_stats));
    return true;
}

void jocky_ai_reset_statistics(void)
{
    memset(&g_ai_state.statistics, 0, sizeof(g_ai_state.statistics));
}

bool jocky_ai_load_model_file(const char* model_path)
{
    if (!model_path) return false;

    FILE* f = fopen(model_path, "rb");
    if (!f) return false;

    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (file_size < (long)sizeof(JOCKY_AI_MODEL)) { fclose(f); return false; }

    uint8_t* buf = (uint8_t*)malloc((size_t)file_size);
    if (!buf) { fclose(f); return false; }

    if (fread(buf, 1, (size_t)file_size, f) != (size_t)file_size) {
        fclose(f); free(buf); return false;
    }
    fclose(f);

    bool result = jocky_ai_init(buf, (size_t)file_size);
    free(buf);
    return result;
}

bool jocky_ai_load_model_buffer(const uint8_t* buffer, size_t buffer_size)
{
    return jocky_ai_init(buffer, buffer_size);
}

bool jocky_ai_run_inference(const JOCKY_AI_TELEMETRY* telemetry,
                             JOCKY_AI_INFERENCE_RESULT* out_result)
{
    if (!telemetry || !out_result) return false;

    uint64_t t0 = jocky_ai_get_current_time_ms();

    out_result->threat_score  = jocky_ai_score_threat(telemetry);
    out_result->threat_level  = jocky_ai_classify_threat(telemetry);
    out_result->confidence    = (g_ai_state.model && g_ai_state.model->weights)
                                ? 0.85f : 0.70f;

    if (telemetry->alert_count > 0 && telemetry->blocked_operations > 0)
        out_result->confidence = (out_result->confidence + 1.0f) / 2.0f;

    out_result->inference_time_us = (jocky_ai_get_current_time_ms() - t0) * 1000;
    g_ai_state.statistics.total_predictions++;
    return true;
}

bool jocky_ai_is_model_ready(void)
{
    return g_ai_state.model != NULL
        && g_ai_state.model->weights != NULL
        && g_ai_state.model->magic == JOCKY_AI_MODEL_MAGIC;
}

bool jocky_ai_get_model_info(JOCKY_AI_MODEL* out_info)
{
    if (!out_info || !g_ai_state.model) return false;
    memcpy(out_info, g_ai_state.model, sizeof(JOCKY_AI_MODEL));
    return true;
}
