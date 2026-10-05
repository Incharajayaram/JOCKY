#ifndef JOCKY_BYOUD_H
#define JOCKY_BYOUD_H

#include <stdint.h>
#include <stddef.h>

/* BYOUD (Bring Your Own Unwinding Data)
 *
 * Windows PE-specific CET (Control-flow Enforcement Technology) bypass technique
 * that manipulates stack unwinding metadata (.pdata/.xdata sections) instead of
 * return addresses. This allows forging legitimate call stacks even on CET-enabled
 * systems, bypassing return-oriented CFI protections.
 *
 * PLATFORM: Windows PE format only
 * - Targets .pdata section (RUNTIME_FUNCTION array)
 * - Targets .xdata section (UNWIND_INFO structures)
 * - Requires Windows PE image manipulation capabilities
 *
 * Key components:
 * - RUNTIME_FUNCTION: Maps instruction ranges to unwind info
 * - UNWIND_INFO: Describes how to unwind stack frames
 * - UNWIND_CODE: Atomic unwinding operations (push, alloca, etc.)
 */

/* RUNTIME_FUNCTION: MinGW winnt.h provides this inside _AMD64_ guards; skip ours there.
 * UNWIND_CODE/UNWIND_INFO: neither MSVC SDK nor MinGW expose these in public headers;
 * we always define them ourselves (no conflict risk). */
#if !defined(_MSC_VER) && !defined(_AMD64_)
typedef struct {
    uint32_t BeginAddress;
    uint32_t EndAddress;
    uint32_t UnwindData;
} RUNTIME_FUNCTION;
#endif

#ifndef JOCKY_UNWIND_INFO_DEFINED
#define JOCKY_UNWIND_INFO_DEFINED
typedef struct {
    uint8_t Version;
    uint8_t Flags;
    uint8_t SizeOfProlog;
    uint8_t CountOfCodes;
    uint8_t FrameRegister;
    uint8_t FrameOffset;
} UNWIND_INFO;
#endif

#ifndef JOCKY_UNWIND_CODE_DEFINED
#define JOCKY_UNWIND_CODE_DEFINED
typedef struct {
    uint8_t CodeOffset;
    uint8_t OpCode;
    uint8_t OpInfo;
} UNWIND_CODE;
#endif

/* BYOUD function API */

int jocky_byoud_initialize(void);

/* Create fake RUNTIME_FUNCTION entries for spoofed call stack */
int jocky_byoud_create_fake_function(
    uint32_t begin_addr,
    uint32_t end_addr,
    RUNTIME_FUNCTION* out_func
);

/* Create fake UNWIND_INFO for custom unwinding behavior */
int jocky_byoud_create_fake_unwind_info(
    uint8_t frame_register,
    uint8_t frame_offset,
    const UNWIND_CODE* codes,
    int code_count,
    UNWIND_INFO* out_info
);

/* Patch PDATA/.pdata section to inject fake function entries */
int jocky_byoud_patch_pdata(
    const RUNTIME_FUNCTION* fake_funcs,
    int func_count
);

/* Forge call stack by manipulating unwind tables */
int jocky_byoud_forge_call_stack(
    const uint64_t* spoofed_addresses,
    int address_count
);

/* Verify CET status and apply BYOUD if needed */
int jocky_byoud_check_and_bypass_cet(void);

/* Restore original unwind data (cleanup) */
int jocky_byoud_restore_unwind_tables(void);

#endif
