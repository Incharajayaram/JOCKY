#pragma once

#include <string>

namespace jocky {

// RC4-encrypt .text and .rdata in-place; store the 16-byte key in a new
// .jkey section so the runtime loader can find it.
bool packPE(const std::string& path);

// .jtamp section layout (all little-endian):
//   [0..7]   magic     "JOCKYTMP"
//   [8..11]  flags     0x00000001  (version / algorithm indicator)
//   [12..15] checksum  XOR-folded CRC32 of all section raw data except .jtamp

// Append a .jdrv section containing the driver bytes RC4-encrypted with a
// random per-build key.  Layout: [8 magic][4 orig_size][16 key][encrypted data].
bool embedDriver(const std::string& pePath, const std::string& driverPath);

// Append a .jmani section parsed from a plain-text manifest file.
//
// manifest_file format – one entry per non-comment line:
//   <name>  <ioctl_hex>  <in_bytes>  <out_bytes>
//   e.g.  read_phys  0x9C402584  8  4
//
// Lines starting with '#' and blank lines are ignored.
// The section matches the binary layout expected by driver_interact.c.
bool embedManifest(const std::string& pePath, const std::string& manifestFile);

// Append a .jtamp section with a CRC32 of every section's raw data
// (excluding .jtamp itself).  Must be called last so it covers the final
// binary state.  The runtime reads this at startup and aborts on mismatch.
bool addAntiTamper(const std::string& path);

} // namespace jocky
