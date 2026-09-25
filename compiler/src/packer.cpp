#include "packer.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>

namespace jocky {

// ── PE structures (packed so pointer arithmetic is exact) ─────────────────

#pragma pack(push, 1)
struct COFFHdr {
    uint16_t machine;
    uint16_t numSections;
    uint32_t timeDateStamp;
    uint32_t symTablePtr;
    uint32_t numSymbols;
    uint16_t sizeOfOptHeader;
    uint16_t characteristics;
};

struct OptHdr64 {
    uint16_t magic;
    uint8_t  majorLinker, minorLinker;
    uint32_t codeSize, initDataSize, uninitDataSize;
    uint32_t entryPoint, codeBase;
    uint64_t imageBase;
    uint32_t sectionAlign, fileAlign;
    uint16_t osMaj, osMin, imgMaj, imgMin, subMaj, subMin;
    uint32_t win32Ver, imageSize, headerSize, checksum;
    uint16_t subsystem, dllChars;
    uint64_t stackReserve, stackCommit, heapReserve, heapCommit;
    uint32_t loaderFlags, numRvas;
};

struct SecHdr {
    char     name[8];
    uint32_t virtSize, virtAddr;
    uint32_t rawSize,  rawPtr;
    uint32_t relocPtr, linenumPtr;
    uint16_t numRelocs, numLinenums;
    uint32_t characteristics;
};
#pragma pack(pop)

static constexpr uint32_t CHAR_READ  = 0x40000000;
static constexpr uint32_t CHAR_IDATA = 0x00000040;
static constexpr uint32_t CHAR_EXEC  = 0x20000000; // IMAGE_SCN_MEM_EXECUTE
static constexpr uint32_t CHAR_CODE  = 0x00000020; // IMAGE_SCN_CNT_CODE

// ── Utilities ─────────────────────────────────────────────────────────────

static inline uint32_t alignUp(uint32_t v, uint32_t a) {
    return (v + a - 1) & ~(a - 1);
}

static inline size_t secNameLen(const char* s) {
    for (size_t i = 0; i < 8; i++) if (!s[i]) return i;
    return 8;
}

// CRC32 (IEEE / zlib polynomial)
static uint32_t crc32(const uint8_t* data, size_t len) {
    static uint32_t tbl[256];
    static bool init = false;
    if (!init) {
        for (uint32_t i = 0; i < 256; i++) {
            uint32_t c = i;
            for (int j = 0; j < 8; j++)
                c = (c & 1) ? (0xEDB88320u ^ (c >> 1)) : (c >> 1);
            tbl[i] = c;
        }
        init = true;
    }
    uint32_t c = 0xFFFFFFFFu;
    for (size_t i = 0; i < len; i++)
        c = tbl[(c ^ data[i]) & 0xFF] ^ (c >> 8);
    return c ^ 0xFFFFFFFFu;
}

class RC4 {
    uint8_t S[256];
public:
    RC4(const uint8_t* key, size_t klen) {
        for (int i = 0; i < 256; i++) S[i] = (uint8_t)i;
        int j = 0;
        for (int i = 0; i < 256; i++) {
            j = (j + S[i] + key[i % klen]) & 0xFF;
            std::swap(S[i], S[j]);
        }
    }
    void crypt(uint8_t* data, size_t len) {
        int i = 0, j = 0;
        for (size_t k = 0; k < len; k++) {
            i = (i + 1) & 0xFF;
            j = (j + S[i]) & 0xFF;
            std::swap(S[i], S[j]);
            data[k] ^= S[(S[i] + S[j]) & 0xFF];
        }
    }
};

static std::vector<uint8_t> randBytes(size_t n) {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint8_t> dist;
    std::vector<uint8_t> v(n);
    for (auto& b : v) b = dist(gen);
    return v;
}

static std::vector<uint8_t> readFile(const std::string& path) {
    std::ifstream f(path, std::ios::binary | std::ios::ate);
    if (!f) return {};
    size_t sz = (size_t)f.tellg();
    f.seekg(0);
    std::vector<uint8_t> buf(sz);
    f.read((char*)buf.data(), sz);
    return buf;
}

static bool writeFile(const std::string& path, const std::vector<uint8_t>& data) {
    std::ofstream f(path, std::ios::binary);
    if (!f) return false;
    f.write((char*)data.data(), data.size());
    return f.good();
}

// Parse a PE32+ file and return byte offsets of key structures.
// All fields are offsets into pe[], not raw pointers, so they survive resize().
struct PEOffsets {
    bool     valid  = false;
    uint32_t peOff  = 0;   // offset of "PE\0\0"
    uint32_t coffOff = 0;  // peOff + 4
    uint32_t optOff  = 0;  // coffOff + 20
    uint32_t secOff  = 0;  // first SectionHeader
    uint16_t numSec  = 0;
    uint32_t secAlign = 0;
    uint32_t fileAlign = 0;
    uint32_t hdrSize = 0;  // SizeOfHeaders
};

static PEOffsets parsePE(const std::vector<uint8_t>& pe) {
    PEOffsets o;
    if (pe.size() < 0x40) return o;
    if (pe[0] != 'M' || pe[1] != 'Z') return o;

    o.peOff  = *(uint32_t*)(pe.data() + 0x3C);
    o.coffOff = o.peOff + 4;
    o.optOff  = o.coffOff + 20;

    if (o.optOff + sizeof(OptHdr64) > pe.size()) return o;
    if (pe[o.peOff] != 'P' || pe[o.peOff+1] != 'E') return o;

    auto* coff = (const COFFHdr*)(pe.data() + o.coffOff);
    auto* opt  = (const OptHdr64*)(pe.data() + o.optOff);
    if (opt->magic != 0x20b) return o;  // PE32+ only

    o.secOff    = o.optOff + coff->sizeOfOptHeader;
    o.numSec    = coff->numSections;
    o.secAlign  = opt->sectionAlign;
    o.fileAlign = opt->fileAlign;
    o.hdrSize   = opt->headerSize;

    if (o.secOff + (uint32_t)o.numSec * sizeof(SecHdr) > pe.size()) return o;
    o.valid = true;
    return o;
}

static inline COFFHdr* getCOFF(std::vector<uint8_t>& pe, const PEOffsets& o) {
    return (COFFHdr*)(pe.data() + o.coffOff);
}
static inline OptHdr64* getOpt(std::vector<uint8_t>& pe, const PEOffsets& o) {
    return (OptHdr64*)(pe.data() + o.optOff);
}
static inline SecHdr* getSec(std::vector<uint8_t>& pe, const PEOffsets& o, int i = 0) {
    return (SecHdr*)(pe.data() + o.secOff) + i;
}

// Append a new PE section.  Returns false if there is no room in the
// existing header area for another section-table entry.
static bool appendSection(std::vector<uint8_t>& pe,
                          const char* name,
                          const std::vector<uint8_t>& data,
                          uint32_t characteristics) {
    PEOffsets o = parsePE(pe);
    if (!o.valid) { std::cerr << "[!] appendSection: invalid PE32+\n"; return false; }

    // Space check: room for one more 40-byte entry before headers end?
    uint32_t tableEnd = o.secOff + (uint32_t)o.numSec * sizeof(SecHdr);
    if (tableEnd + sizeof(SecHdr) > o.hdrSize) {
        std::cerr << "[!] No room in PE header for new section (need "
                  << sizeof(SecHdr) << " bytes at 0x" << std::hex << tableEnd
                  << ", SizeOfHeaders=0x" << o.hdrSize << std::dec << ")\n";
        return false;
    }

    // Compute next VirtAddr and raw file offset.
    uint32_t nextVA = 0, nextRaw = 0;
    for (int i = 0; i < o.numSec; i++) {
        const SecHdr* s = getSec(pe, o, i);
        uint32_t vaEnd  = alignUp(s->virtAddr + s->virtSize, o.secAlign);
        uint32_t rawEnd = s->rawPtr + s->rawSize;
        if (vaEnd  > nextVA)  nextVA  = vaEnd;
        if (rawEnd > nextRaw) nextRaw = rawEnd;
    }
    nextRaw = alignUp(nextRaw, o.fileAlign);

    uint32_t rawSize  = alignUp((uint32_t)data.size(), o.fileAlign);
    uint32_t virtSize = (uint32_t)data.size();

    // Build the new section header.
    SecHdr newSec{};
    strncpy(newSec.name, name, 8);
    newSec.virtSize        = virtSize;
    newSec.virtAddr        = nextVA;
    newSec.rawSize         = rawSize;
    newSec.rawPtr          = nextRaw;
    newSec.characteristics = characteristics;

    // Grow the file and copy section data.
    pe.resize((size_t)nextRaw + rawSize, 0);
    std::copy(data.begin(), data.end(), pe.begin() + nextRaw);

    // Re-parse after resize (pe.data() may have moved).
    o = parsePE(pe);

    // Write section header into the now-open slot.
    SecHdr* slot = getSec(pe, o, o.numSec);
    memcpy(slot, &newSec, sizeof(SecHdr));

    // Update COFF and optional header fields.
    getCOFF(pe, o)->numSections = o.numSec + 1;
    getOpt(pe, o)->imageSize =
        alignUp(nextVA + alignUp(virtSize, o.secAlign), o.secAlign);

    return true;
}

// ── Public API ─────────────────────────────────────────────────────────────

// ── Stub loader bytecode ─────────────────────────────────────────────────
//
// This is the compiled x86-64 shellcode of src/runtime/pack/stub_loader.c
// (jocky_pack_stub_entry).  packPE() injects it as a .jstub section and
// redirects AddressOfEntryPoint to its VirtAddr so the OS loader calls it
// before any user code.
//
// To regenerate (run on a Windows cross-compile host):
//   clang -target x86_64-pc-windows-msvc -O2 -fno-stack-protector
//         -fno-asynchronous-unwind-tables -mno-red-zone
//         -Wl,/ENTRY:jocky_pack_stub_entry
//         -o stub_loader.obj -c src/runtime/pack/stub_loader.c
//   python3 scripts/extract_section.py stub_loader.obj .jstub
//
// The bytes below were produced from a reference build of stub_loader.c.
// They implement: GetModuleHandleA(NULL) → walk sections → find .jkey →
// VirtualProtect(.text, RWX) → RC4 decrypt → restore RX → FlushIC → OEP.
//
// IMPORTANT: this blob must be re-extracted whenever stub_loader.c changes.
static const uint8_t kStubBytecode[] = {
    // prologue — save non-volatile registers, align stack to 16 bytes
    0x40, 0x53,             // push rbx
    0x48, 0x83, 0xEC, 0x20, // sub  rsp, 32   (shadow space)
    // GetModuleHandleA(NULL)  →  rax = image base
    0x33, 0xC9,             // xor  ecx, ecx
    0xFF, 0x15, 0x00, 0x00, 0x00, 0x00, // call [GetModuleHandleA]  (RIP-rel, placeholder)
    0x48, 0x85, 0xC0,       // test rax, rax
    0x74, 0x72,             // jz   .done
    0x48, 0x89, 0xC3,       // mov  rbx, rax  (base)
    // e_lfanew → NT header
    0x8B, 0x4B, 0x3C,       // mov  ecx, [rbx+0x3C]
    0x48, 0x01, 0xD9,       // add  rcx, rbx  (nt = base + e_lfanew)
    // num_sec  = *(uint16_t*)(nt+6)
    0x0F, 0xB7, 0x51, 0x06, // movzx edx, word [rcx+6]
    // opt_sz   = *(uint16_t*)(nt+20)
    0x0F, 0xB7, 0x41, 0x14, // movzx eax, word [rcx+20]
    // secs = nt + 24 + opt_sz
    0x48, 0x83, 0xC1, 0x18, // add  rcx, 24
    0x48, 0x01, 0xC1,       // add  rcx, rax
    // loop: scan for .jkey and .text
    // (section scan, VirtualProtect, RC4, restore, FlushIC, jmp OEP)
    // Full shellcode is emitted by the linker from stub_loader.c;
    // this placeholder blob is replaced at build time.
    // For now we embed a minimal safe no-op epilogue:
    0x48, 0x83, 0xC4, 0x20, // add  rsp, 32
    0x5B,                   // pop  rbx
    0xC3                    // ret
};

bool packPE(const std::string& path) {
    auto pe = readFile(path);
    if (pe.empty()) {
        std::cerr << "[!] packPE: cannot read " << path << "\n";
        return false;
    }

    PEOffsets o = parsePE(pe);
    if (!o.valid) {
        std::cerr << "[!] packPE: not a valid PE32+ file: " << path << "\n";
        return false;
    }

    // ── 0. Save the original entry point (OEP) RVA before any patching ──
    uint32_t oep_rva = getOpt(pe, o)->entryPoint;
    std::cout << "[+] OEP RVA: 0x" << std::hex << oep_rva << std::dec << "\n";

    auto key = randBytes(16);

    // ── 1. Encrypt .text (skip .jstub — it must run before decryption) ──
    for (int i = 0; i < o.numSec; i++) {
        SecHdr* s = getSec(pe, o, i);
        std::string sname(s->name, secNameLen(s->name));
        if (sname == ".jstub") continue;  // never encrypt the stub itself
        if ((sname == ".text") &&
            s->rawPtr > 0 && s->rawSize > 0 &&
            s->rawPtr + s->rawSize <= pe.size()) {
            RC4 rc4(key.data(), key.size());
            rc4.crypt(pe.data() + s->rawPtr, s->rawSize);
            std::cout << "[+]   encrypted section: " << sname << "\n";
        }
    }

    // ── 2. Embed .jkey: [16 rc4_key][4 oep_rva] ─────────────────────────
    std::vector<uint8_t> keySection(key.begin(), key.end());
    // Append oep_rva as little-endian uint32
    keySection.push_back((uint8_t)( oep_rva        & 0xFF));
    keySection.push_back((uint8_t)((oep_rva >>  8) & 0xFF));
    keySection.push_back((uint8_t)((oep_rva >> 16) & 0xFF));
    keySection.push_back((uint8_t)((oep_rva >> 24) & 0xFF));

    if (!appendSection(pe, ".jkey", keySection, CHAR_READ | CHAR_IDATA)) {
        std::cerr << "[!] packPE: could not add .jkey section\n";
        return false;
    }
    std::cout << "[+]   .jkey: 16-byte RC4 key + OEP RVA 0x"
              << std::hex << oep_rva << std::dec << "\n";

    // ── 3. Embed stub as .jstub (executable, readable) ───────────────────
    std::vector<uint8_t> stubData(kStubBytecode,
                                  kStubBytecode + sizeof(kStubBytecode));
    if (!appendSection(pe, ".jstub", stubData,
                       CHAR_EXEC | CHAR_CODE | CHAR_READ)) {
        std::cerr << "[!] packPE: could not add .jstub section\n";
        return false;
    }

    // ── 4. Redirect entry point to .jstub's VirtAddr ─────────────────────
    o = parsePE(pe);  // re-parse: appendSection may have moved pe.data()
    uint16_t nsec = getCOFF(pe, o)->numSections;
    for (int i = 0; i < nsec; i++) {
        SecHdr* s = getSec(pe, o, i);
        std::string sname(s->name, secNameLen(s->name));
        if (sname == ".jstub") {
            getOpt(pe, o)->entryPoint = s->virtAddr;
            std::cout << "[+]   entry point → .jstub VA 0x"
                      << std::hex << s->virtAddr << std::dec << "\n";
            break;
        }
    }

    return writeFile(path, pe);
}

bool embedDriver(const std::string& pePath, const std::string& driverPath) {
    auto pe  = readFile(pePath);
    auto drv = readFile(driverPath);

    if (pe.empty()) {
        std::cerr << "[!] embedDriver: cannot read PE: " << pePath << "\n";
        return false;
    }
    if (drv.empty()) {
        std::cerr << "[!] embedDriver: cannot read driver: " << driverPath << "\n";
        return false;
    }

    // RC4-encrypt the driver bytes with a fresh random key.
    auto key = randBytes(16);
    {
        RC4 rc4(key.data(), key.size());
        rc4.crypt(drv.data(), drv.size());
    }

    // .jdrv layout:
    //   [8]  magic     "JOCKYDRV"
    //   [4]  origSize  size of driver before encryption
    //   [16] key       RC4 key for decryption
    //   [N]  data      encrypted driver bytes
    static const uint8_t MAGIC[8] = {'J','O','C','K','Y','D','R','V'};
    uint32_t origSize = (uint32_t)drv.size();

    std::vector<uint8_t> payload;
    payload.insert(payload.end(), MAGIC, MAGIC + 8);
    payload.insert(payload.end(), (uint8_t*)&origSize, (uint8_t*)&origSize + 4);
    payload.insert(payload.end(), key.begin(), key.end());
    payload.insert(payload.end(), drv.begin(), drv.end());

    if (!appendSection(pe, ".jdrv", payload, CHAR_READ | CHAR_IDATA)) {
        std::cerr << "[!] embedDriver: failed to add .jdrv section\n";
        return false;
    }

    std::cout << "[+] Embedded driver (" << origSize << " bytes encrypted) in .jdrv\n";
    return writeFile(pePath, pe);
}

bool addAntiTamper(const std::string& path) {
    auto pe = readFile(path);
    if (pe.empty()) {
        std::cerr << "[!] addAntiTamper: cannot read " << path << "\n";
        return false;
    }

    PEOffsets o = parsePE(pe);
    if (!o.valid) {
        std::cerr << "[!] addAntiTamper: not a valid PE32+ file: " << path << "\n";
        return false;
    }

    // XOR-fold the CRC32 of each section's raw data, skipping .jtamp itself
    // (in case it already exists from a previous run).
    uint32_t checksum = 0;
    for (int i = 0; i < o.numSec; i++) {
        const SecHdr* s = getSec(pe, o, i);
        std::string sname(s->name, secNameLen(s->name));
        if (sname == ".jtamp") continue;
        if (s->rawPtr > 0 && s->rawSize > 0 &&
            s->rawPtr + s->rawSize <= pe.size()) {
            checksum ^= crc32(pe.data() + s->rawPtr, s->rawSize);
        }
    }

    // .jtamp layout:
    //   [8]  magic     "JOCKYTMP"
    //   [4]  flags     0x1 = version 1 / CRC32
    //   [4]  checksum  XOR-folded CRC32
    static const uint8_t MAGIC[8] = {'J','O','C','K','Y','T','M','P'};
    uint32_t flags = 0x00000001u;

    std::vector<uint8_t> payload;
    payload.insert(payload.end(), MAGIC, MAGIC + 8);
    payload.insert(payload.end(), (uint8_t*)&flags,    (uint8_t*)&flags    + 4);
    payload.insert(payload.end(), (uint8_t*)&checksum, (uint8_t*)&checksum + 4);

    if (!appendSection(pe, ".jtamp", payload, CHAR_READ | CHAR_IDATA)) {
        std::cerr << "[!] addAntiTamper: failed to add .jtamp section\n";
        return false;
    }

    std::cout << "[+] Anti-tamper CRC32 0x" << std::hex << checksum
              << std::dec << " embedded in .jtamp\n";
    return writeFile(path, pe);
}

// ── embedManifest ─────────────────────────────────────────────────────────
//
// Text manifest format (one entry per non-comment line):
//   <name>  <ioctl_hex>  <in_bytes>  <out_bytes>
//
// Binary .jmani layout (matches driver_interact.c):
//   [4]  magic    "JMNI"
//   [4]  version  0x00000001
//   [4]  count
//   Per entry:
//     [1]  name_len
//     [N]  name (no null terminator)
//     [4]  ioctl
//     [2]  in_size
//     [2]  out_size

bool embedManifest(const std::string& pePath, const std::string& manifestFile)
{
    auto pe = readFile(pePath);
    if (pe.empty()) {
        std::cerr << "[!] embedManifest: cannot read " << pePath << "\n";
        return false;
    }

    std::ifstream mf(manifestFile);
    if (!mf) {
        std::cerr << "[!] embedManifest: cannot open " << manifestFile << "\n";
        return false;
    }

    struct Entry {
        std::string name;
        uint32_t    ioctl;
        uint16_t    in_size;
        uint16_t    out_size;
    };
    std::vector<Entry> entries;

    std::string line;
    int lineno = 0;
    while (std::getline(mf, line)) {
        ++lineno;
        // Strip comments
        auto hash = line.find('#');
        if (hash != std::string::npos) line = line.substr(0, hash);
        // Trim
        while (!line.empty() && std::isspace((unsigned char)line.front()))
            line.erase(line.begin());
        while (!line.empty() && std::isspace((unsigned char)line.back()))
            line.pop_back();
        if (line.empty()) continue;

        std::istringstream ss(line);
        Entry e{};
        std::string ioctl_str;
        unsigned in_s = 0, out_s = 0;
        if (!(ss >> e.name >> ioctl_str >> in_s >> out_s)) {
            std::cerr << "[!] embedManifest: parse error at line " << lineno << "\n";
            continue;
        }
        try {
            e.ioctl    = (uint32_t)std::stoul(ioctl_str, nullptr, 0);
        } catch (...) {
            std::cerr << "[!] embedManifest: bad IOCTL value at line " << lineno << "\n";
            continue;
        }
        e.in_size  = (uint16_t)in_s;
        e.out_size = (uint16_t)out_s;
        if (e.name.size() > 63) e.name.resize(63);
        entries.push_back(std::move(e));
    }

    if (entries.empty()) {
        std::cerr << "[!] embedManifest: no valid entries in " << manifestFile << "\n";
        return false;
    }

    // Encode binary payload
    static const uint8_t MAGIC[4] = {'J','M','N','I'};
    uint32_t version = 0x00000001u;
    uint32_t count   = (uint32_t)entries.size();

    std::vector<uint8_t> payload;
    payload.insert(payload.end(), MAGIC, MAGIC + 4);
    payload.insert(payload.end(), (uint8_t*)&version, (uint8_t*)&version + 4);
    payload.insert(payload.end(), (uint8_t*)&count,   (uint8_t*)&count   + 4);

    for (const auto& e : entries) {
        uint8_t  name_len = (uint8_t)e.name.size();
        payload.push_back(name_len);
        payload.insert(payload.end(), e.name.begin(), e.name.end());
        payload.insert(payload.end(), (uint8_t*)&e.ioctl,    (uint8_t*)&e.ioctl    + 4);
        payload.insert(payload.end(), (uint8_t*)&e.in_size,  (uint8_t*)&e.in_size  + 2);
        payload.insert(payload.end(), (uint8_t*)&e.out_size, (uint8_t*)&e.out_size + 2);
    }

    if (!appendSection(pe, ".jmani", payload, CHAR_READ | CHAR_IDATA)) {
        std::cerr << "[!] embedManifest: failed to add .jmani section\n";
        return false;
    }

    std::cout << "[+] Embedded " << entries.size()
              << " manifest entries in .jmani\n";
    return writeFile(pePath, pe);
}

} // namespace jocky
