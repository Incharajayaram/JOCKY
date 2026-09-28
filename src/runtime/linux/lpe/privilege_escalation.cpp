#include "common.h"

namespace kernel_evasion {

// Privilege escalation is intentionally documented only, not implemented
// Public references and CVE details are in research/PAPERS_AND_REFERENCES.md
//
// Modern LPE techniques include:
// - CVE-2026-46300 (Fragnesia): Arbitrary page cache write via XFRM ESP-in-TCP
// - CVE-2026-43284 (Dirty Frag): CoW bypass for page cache writes
//
// These are reference implementations only, studying how detection systems
// should identify and mitigate kernel privilege escalation vectors.

}  // namespace kernel_evasion
