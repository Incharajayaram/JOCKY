/*
 * BYOVD Runtime Header
 * Modular Bring Your Own Vulnerable Driver framework.
 *
 * Part of JOCKY - SIH-148
 */

#ifndef BYOVD_MODULAR_H
#define BYOVD_MODULAR_H

#include <windows.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Opaque context handle */
typedef struct ByovdContext* HBYOVD;

/* Initialize BYOVD framework by loading a signed vulnerable driver.
 * driverPath: wide-char path to the .sys file
 * Returns context handle or NULL on failure.
 */
HBYOVD byovd_init(const wchar_t* driverPath);

/* Shutdown BYOVD and unload the driver */
void byovd_shutdown(HBYOVD ctx);

/* Map physical memory into user-mode address space.
 * Returns true on success, virtualAddr is written with the mapped address.
 */
bool byovd_map_physical(HBYOVD ctx, ULONG_PTR physAddr, ULONG size, void** virtualAddr);

/* Unmap previously mapped physical memory */
bool byovd_unmap_physical(HBYOVD ctx, void* virtualAddr);

/* Read arbitrary physical memory */
bool byovd_read_phys(HBYOVD ctx, ULONG_PTR physAddr, void* buffer, ULONG size);

/* Write arbitrary physical memory */
bool byovd_write_phys(HBYOVD ctx, ULONG_PTR physAddr, const void* buffer, ULONG size);

/* Read an MSR (Model-Specific Register) */
bool byovd_read_msr(HBYOVD ctx, ULONG msrIndex, unsigned long long* value);

/* Scan a physical memory range for a byte pattern */
bool byovd_scan_phys(HBYOVD ctx, ULONG_PTR start, ULONG_PTR end,
                     const unsigned char* pattern, ULONG patternLen,
                     ULONG_PTR* foundAddr);

/* Dump a region of physical memory to a file */
bool byovd_dump_phys(HBYOVD ctx, ULONG_PTR start, ULONG_PTR end, const char* outPath);

/* Check if BYOVD is active and functional */
bool byovd_is_active(HBYOVD ctx);

/* Initialize BYOVD with fallback chain support.
 * Tries each driver in sequence until one loads successfully.
 * Returns context handle or NULL if all drivers fail.
 */
HBYOVD byovd_init_with_fallback(const wchar_t* const* driverPaths, int pathCount);

/* Initialize BYOVD with smart fallback using priority scores.
 * Attempts drivers in order of descending priority scores.
 * If priorityScores is NULL, uses default descending priority (first driver highest).
 */
HBYOVD byovd_init_smart_fallback(const wchar_t* const* driverPaths, int pathCount, const int* priorityScores);

#ifdef __cplusplus
}
#endif

#endif /* BYOVD_MODULAR_H */
