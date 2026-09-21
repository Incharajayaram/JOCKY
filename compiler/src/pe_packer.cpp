#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <random>
#include <algorithm>
#include <cstring>

struct PEHeader {
    uint16_t machine;
    uint16_t num_sections;
    uint32_t timestamp;
    uint32_t sym_table;
    uint32_t num_symbols;
    uint16_t opt_header_size;
    uint16_t characteristics;
};

struct OptHeader64 {
    uint16_t magic;
    uint8_t major_linker;
    uint8_t minor_linker;
    uint32_t code_size;
    uint32_t init_data_size;
    uint32_t uninit_data_size;
    uint32_t entry_point;
    uint32_t code_base;
    uint64_t image_base;
    uint32_t section_align;
    uint32_t file_align;
    uint16_t os_major;
    uint16_t os_minor;
    uint16_t image_major;
    uint16_t image_minor;
    uint16_t subsys_major;
    uint16_t subsys_minor;
    uint32_t win32_version;
    uint32_t image_size;
    uint32_t header_size;
    uint32_t checksum;
    uint16_t subsystem;
    uint16_t dll_chars;
    uint64_t stack_reserve;
    uint64_t stack_commit;
    uint64_t heap_reserve;
    uint64_t heap_commit;
    uint32_t loader_flags;
    uint32_t num_rvas;
};

struct SectionHeader {
    char name[8];
    uint32_t virt_size;
    uint32_t virt_addr;
    uint32_t raw_size;
    uint32_t raw_ptr;
    uint32_t reloc_ptr;
    uint32_t linenum_ptr;
    uint16_t num_relocs;
    uint16_t num_linenums;
    uint32_t characteristics;
};

class RC4 {
    uint8_t S[256];
public:
    RC4(const uint8_t* key, size_t keylen) {
        for (int i = 0; i < 256; i++) S[i] = i;
        int j = 0;
        for (int i = 0; i < 256; i++) {
            j = (j + S[i] + key[i % keylen]) & 0xFF;
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

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input.exe> <output.exe>\n";
        return 1;
    }
    
    std::string input_path = argv[1];
    std::string output_path = argv[2];
    
    std::ifstream in(input_path, std::ios::binary | std::ios::ate);
    if (!in) { std::cerr << "Cannot open " << input_path << "\n"; return 1; }
    size_t fsize = in.tellg();
    in.seekg(0);
    std::vector<uint8_t> pe(fsize);
    in.read((char*)pe.data(), fsize);
    
    if (fsize < 0x40 || pe[0] != 'M' || pe[1] != 'Z') {
        std::cerr << "Not a valid PE file\n";
        return 1;
    }
    
    uint32_t pe_offset = *(uint32_t*)(pe.data() + 0x3C);
    if (pe_offset + 4 > pe.size() || pe[pe_offset] != 'P' || pe[pe_offset+1] != 'E') {
        std::cerr << "Invalid PE signature\n";
        return 1;
    }
    
    PEHeader* peh = (PEHeader*)(pe.data() + pe_offset + 4);
    if (peh->num_sections == 0 || peh->opt_header_size == 0) {
        std::cerr << "Invalid PE header\n";
        return 1;
    }
    
    struct OptHeader64 {
        uint16_t magic;
        uint8_t major_linker;
        uint8_t minor_linker;
        uint32_t code_size;
        uint32_t init_data_size;
        uint32_t uninit_data_size;
        uint32_t entry_point;
        uint32_t code_base;
        uint64_t image_base;
        uint32_t section_align;
        uint32_t file_align;
        uint16_t os_major;
        uint16_t os_minor;
        uint16_t image_major;
        uint16_t image_minor;
        uint16_t subsys_major;
        uint16_t subsys_minor;
        uint32_t win32_version;
        uint32_t image_size;
        uint32_t header_size;
        uint32_t checksum;
        uint16_t subsystem;
        uint16_t dll_chars;
        uint64_t stack_reserve;
        uint64_t stack_commit;
        uint64_t heap_reserve;
        uint64_t heap_commit;
        uint32_t loader_flags;
        uint32_t num_rvas;
    } __attribute__((packed));
    
    OptHeader64* opt = (OptHeader64*)((uint8_t*)peh + 20);
    if (opt->magic != 0x20b) {
        std::cerr << "Not PE32+ (64-bit), magic = 0x" << std::hex << opt->magic << "\n";
        return 1;
    }
    
    struct SectionHeader {
        char name[8];
        uint32_t virt_size;
        uint32_t virt_addr;
        uint32_t raw_size;
        uint32_t raw_ptr;
        uint32_t reloc_ptr;
        uint32_t linenum_ptr;
        uint16_t num_relocs;
        uint16_t num_linenums;
        uint32_t characteristics;
    } __attribute__((packed));
    
    SectionHeader* sections = (SectionHeader*)((uint8_t*)opt + peh->opt_header_size);
    
    uint8_t key[16];
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint8_t> dist(0, 255);
    for (size_t i = 0; i < 16; i++) key[i] = dist(gen);
    
    for (uint16_t i = 0; i < peh->num_sections; i++) {
        SectionHeader* sec = &sections[i];
        std::string name(sec->name, strnlen(sec->name, 8));
        
        if ((name == ".text" || name == ".rdata") && sec->raw_ptr > 0 && sec->raw_size > 0) {
            if (sec->raw_ptr + sec->raw_size <= pe.size()) {
                RC4 rc4(key, 16);
                rc4.crypt(pe.data() + sec->raw_ptr, sec->raw_size);
                std::cout << "[+] Encrypted: " << name << " (0x" << std::hex << sec->raw_ptr << ", 0x" << sec->raw_size << ")\n";
            }
        }
    }
    
    std::vector<uint8_t> output_data;
    output_data.insert(output_data.end(), key, key + 16);
    output_data.insert(output_data.end(), pe.begin(), pe.end());
    
    std::ofstream out(output_path, std::ios::binary);
    out.write((char*)output_data.data(), output_data.size());
    
    std::cout << "[+] Packed with RC4 key at offset 0, encrypted .text/.rdata\n";
    return 0;
}
