#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <random>
#include <algorithm>
#include <cstring>

// Simple RC4 implementation
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

// Generate random bytes
std::vector<uint8_t> random_bytes(size_t n) {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint8_t> dist(0, 255);
    std::vector<uint8_t> v(n);
    for (size_t i = 0; i < n; i++) v[i] = dist(gen);
    return v;
}

// Random section name
std::string random_section_name() {
    static const char* prefixes[] = {".text", ".data", ".rdata", ".rsrc", ".reloc"};
    static const char* suffixes[] = {"$A", "$B", "$C", "$1", "$2", "$3", "$4", "$5"};
    std::random_device rd;
    std::mt19937_64 gen(rd());
    return std::string(prefixes[rand() % 5]) + suffixes[rand() % 8];
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input.exe> <output.exe> [--stub-size N]\n";
        return 1;
    }
    
    std::string input = argv[1];
    std::string output = argv[2];
    
    // Read input file
    std::ifstream in(input, std::ios::binary | std::ios::ate);
    if (!in) {
        std::cerr << "Cannot open input: " << input << "\n";
        return 1;
    }
    size_t size = in.tellg();
    in.seekg(0);
    std::vector<uint8_t> data(size);
    in.read((char*)data.data(), size);
    
    // For simplicity, we'll encrypt the entire file after the DOS header + PE header
    // In a real implementation, you'd parse PE sections and encrypt .text/.rdata only
    
    // Generate random RC4 key
    std::vector<uint8_t> key = random_bytes(16);
    
    // Encrypt everything after offset 0x200 (past DOS stub + PE header)
    size_t encrypt_offset = 0x200;
    if (size > encrypt_offset) {
        RC4 rc4(key.data(), key.size());
        rc4.crypt(data.data() + encrypt_offset, size - encrypt_offset);
    }
    
    // Prepend key to file (in real impl, key would be derived/obfuscated in stub)
    std::vector<uint8_t> output_data;
    output_data.insert(output_data.end(), key.begin(), key.end());
    output_data.insert(output_data.end(), data.begin(), data.end());
    
    // Write output
    std::ofstream out(output, std::ios::binary);
    out.write((char*)output_data.data(), output_data.size());
    
    std::cout << "[+] Packed: " << input << " -> " << output 
              << " (key at offset 0, encrypted from 0x200)\n";
    return 0;
}
