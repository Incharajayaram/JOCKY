#include "forensic/static_analyzer.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <regex>
#include <cstring>

namespace jocky {
namespace forensic {

bool StaticAnalyzer::analyze(const std::string& binaryPath, StaticReport& out) {
    std::ifstream file(binaryPath, std::ios::binary);
    if (!file) {
        std::cerr << "[!] Failed to open binary: " << binaryPath << "\n";
        return false;
    }

    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> data(fileSize);
    file.read(reinterpret_cast<char*>(data.data()), fileSize);
    file.close();

    // Detect file type
    if (isPE(data)) {
        if (!parsePE(binaryPath, out)) return false;
    } else if (isELF(data)) {
        if (!parseELF(binaryPath, out)) return false;
    } else {
        std::cerr << "[!] Unknown binary format\n";
        return false;
    }

    // Entropy analysis for entire file
    double fileEntropy = calculateEntropy(data);
    std::cout << "[*] File entropy: " << fileEntropy << "\n";

    // String extraction
    extractStrings(data, out.strings);
    std::cout << "[*] Extracted " << out.strings.size() << " strings\n";

    // Packer detection
    detectPackerSignatures(data, out.detectedPackers);
    if (!out.detectedPackers.empty()) {
        std::cout << "[*] Detected packers: ";
        for (const auto& p : out.detectedPackers) std::cout << p << " ";
        std::cout << "\n";
    }

    // Check for overlay (data appended after PE/ELF)
    // Simplified: compare last section end with file size
    // A proper implementation would parse section headers

    return true;
}

bool StaticAnalyzer::isPE(const std::vector<uint8_t>& data) {
    if (data.size() < 2) return false;
    return data[0] == 'M' && data[1] == 'Z';
}

bool StaticAnalyzer::isELF(const std::vector<uint8_t>& data) {
    if (data.size() < 4) return false;
    return data[0] == 0x7f && data[1] == 'E' && data[2] == 'L' && data[3] == 'F';
}

bool StaticAnalyzer::parsePE(const std::string& path, StaticReport& out) {
    std::ifstream file(path, std::ios::binary);
    if (!file) return false;

    // DOS Header
    uint16_t e_magic;
    file.read(reinterpret_cast<char*>(&e_magic), sizeof(e_magic));
    if (e_magic != 0x5A4D) { // 'MZ'
        return false;
    }

    // e_lfanew at offset 0x3C
    file.seekg(0x3C);
    uint32_t e_lfanew;
    file.read(reinterpret_cast<char*>(&e_lfanew), sizeof(e_lfanew));

    // PE Signature
    file.seekg(e_lfanew);
    uint32_t peSignature;
    file.read(reinterpret_cast<char*>(&peSignature), sizeof(peSignature));
    if (peSignature != 0x00004550) { // 'PE\0\0'
        return false;
    }

    // COFF Header
    uint16_t machine, numSections;
    uint32_t timeDateStamp;
    file.read(reinterpret_cast<char*>(&machine), sizeof(machine));
    file.read(reinterpret_cast<char*>(&numSections), sizeof(numSections));
    file.read(reinterpret_cast<char*>(&timeDateStamp), sizeof(timeDateStamp));

    std::cout << "[*] PE Machine: " << (machine == 0x8664 ? "x64" : (machine == 0x14c ? "x86" : "unknown")) << "\n";
    std::cout << "[*] Sections: " << numSections << "\n";

    // Optional Header
    uint16_t optionalHeaderMagic;
    file.seekg(e_lfanew + 24); // After COFF header
    file.read(reinterpret_cast<char*>(&optionalHeaderMagic), sizeof(optionalHeaderMagic));

    bool isPE32Plus = (optionalHeaderMagic == 0x20b);
    size_t sectionTableOffset = isPE32Plus ? (e_lfanew + 24 + 240) : (e_lfanew + 24 + 224);

    // Section Table
    file.seekg(sectionTableOffset);
    for (uint16_t i = 0; i < numSections; i++) {
        char name[9] = {0};
        uint32_t virtualSize, virtualAddress, rawSize, rawAddress;
        uint32_t characteristics;

        file.read(name, 8);
        file.seekg(4, std::ios::cur); // Misc/PhysicalAddress
        file.read(reinterpret_cast<char*>(&virtualAddress), sizeof(virtualAddress));
        file.read(reinterpret_cast<char*>(&virtualSize), sizeof(virtualSize));
        file.read(reinterpret_cast<char*>(&rawAddress), sizeof(rawAddress));
        file.read(reinterpret_cast<char*>(&rawSize), sizeof(rawSize));
        file.seekg(12, std::ios::cur); // Skip relocations, line numbers
        file.read(reinterpret_cast<char*>(&characteristics), sizeof(characteristics));

        SectionAnalysis section;
        section.name = name;
        section.virtualSize = virtualSize;
        section.rawSize = rawSize;
        section.isExecutable = (characteristics & 0x20000000) != 0;
        section.isReadable = (characteristics & 0x40000000) != 0;
        section.isWritable = (characteristics & 0x80000000) != 0;

        // Calculate entropy for this section
        if (rawSize > 0 && rawAddress + rawSize <= static_cast<size_t>(file.tellg())) {
            std::vector<uint8_t> sectionData(rawSize);
            auto currentPos = file.tellg();
            file.seekg(rawAddress);
            file.read(reinterpret_cast<char*>(sectionData.data()), rawSize);
            section.entropy = calculateEntropy(sectionData);
            file.seekg(currentPos);
        }

        out.sections.push_back(section);
        std::cout << "[*] Section: " << section.name
                  << " | Size: " << section.rawSize
                  << " | Entropy: " << section.entropy
                  << " | X:" << section.isExecutable
                  << " W:" << section.isWritable
                  << " R:" << section.isReadable
                  << "\n";
    }

    return true;
}

bool StaticAnalyzer::parseELF(const std::string& path, StaticReport& out) {
    // ELF parsing would go here
    // For now, just mark as ELF and do basic analysis
    std::cout << "[*] ELF binary detected (full parsing TBD)\n";
    return true;
}

double StaticAnalyzer::calculateEntropy(const std::vector<uint8_t>& data) {
    if (data.empty()) return 0.0;

    int frequency[256] = {0};
    for (uint8_t byte : data) {
        frequency[byte]++;
    }

    double entropy = 0.0;
    size_t len = data.size();
    for (int i = 0; i < 256; i++) {
        if (frequency[i] > 0) {
            double p = static_cast<double>(frequency[i]) / len;
            entropy -= p * std::log2(p);
        }
    }
    return entropy;
}

void StaticAnalyzer::extractStrings(const std::vector<uint8_t>& data, std::vector<StringArtifact>& out) {
    const size_t MIN_STRING_LEN = 4;
    std::string current;

    for (size_t i = 0; i < data.size(); i++) {
        if (data[i] >= 0x20 && data[i] <= 0x7E) {
            current += static_cast<char>(data[i]);
        } else {
            if (current.length() >= MIN_STRING_LEN) {
                StringArtifact artifact;
                artifact.value = current;
                artifact.offset = i - current.length();
                artifact.isEncrypted = false; // Determined later by comparison
                categorizeString(current, artifact);
                out.push_back(artifact);
            }
            current.clear();
        }
    }
}

void StaticAnalyzer::categorizeString(const std::string& str, StringArtifact& artifact) {
    // URL pattern
    static const std::regex urlRegex(R"(https?://[\w\-\.]+(/[\w\-\./?%&=]*)?)");
    if (std::regex_search(str, urlRegex)) {
        artifact.category = "url";
        return;
    }

    // IP address pattern
    static const std::regex ipRegex(R"(\b\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}\b)");
    if (std::regex_search(str, ipRegex)) {
        artifact.category = "ip";
        return;
    }

    // Registry path
    static const std::regex regRegex(R"(HKEY_[A-Z_]+\\.*)");
    if (std::regex_search(str, regRegex)) {
        artifact.category = "registry";
        return;
    }

    // File path (Windows)
    static const std::regex winPathRegex(R"([A-Za-z]:\\[\w\\\.\- ]+)");
    if (std::regex_search(str, winPathRegex)) {
        artifact.category = "path";
        return;
    }

    // File path (Unix)
    static const std::regex unixPathRegex(R"(/[\w/\.\-]+)");
    if (std::regex_search(str, unixPathRegex)) {
        artifact.category = "path";
        return;
    }

    // API name heuristics
    static const std::vector<std::string> apiPrefixes = {
        "Nt", "Zw", "Rtl", "Ldr", "Csr", "Dbg", "Etw", "Ex", "Fs", "Io",
        "Ke", "Ki", "Mm", "Ob", "Po", "Ps", "Se", "IoCreate", "PsCreate"
    };
    for (const auto& prefix : apiPrefixes) {
        if (str.find(prefix) == 0) {
            artifact.category = "api";
            return;
        }
    }

    artifact.category = "other";
}

bool StaticAnalyzer::detectPackerSignatures(const std::vector<uint8_t>& data, std::vector<std::string>& out) {
    if (hasUPXSignature(data)) {
        out.push_back("UPX");
    }

    // Check for other packer signatures
    // VMProtect, Themida, etc. would have specific signatures

    return true;
}

bool StaticAnalyzer::hasUPXSignature(const std::vector<uint8_t>& data) {
    // UPX sections
    const char upx0[] = "UPX0";
    const char upx1[] = "UPX1";
    const char upx2[] = "UPX2";

    for (size_t i = 0; i < data.size() - 4; i++) {
        if (std::memcmp(&data[i], upx0, 4) == 0 ||
            std::memcmp(&data[i], upx1, 4) == 0 ||
            std::memcmp(&data[i], upx2, 4) == 0) {
            return true;
        }
    }
    return false;
}

} // namespace forensic
} // namespace jocky
