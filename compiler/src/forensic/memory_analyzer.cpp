#include "forensic/memory_analyzer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cctype>

#ifdef __linux__
#include <sys/types.h>
#include <unistd.h>
#endif

namespace jocky {
namespace forensic {

bool MemoryAnalyzer::analyze(pid_t pid, MemoryReport& out) {
#ifdef __linux__
    return analyzeLinux(pid, out);
#else
    std::cerr << "[!] Memory analysis not supported on this platform\n";
    out.dumpAcquired = false;
    return false;
#endif
}

#ifdef __linux__
bool MemoryAnalyzer::analyzeLinux(pid_t pid, MemoryReport& out) {
    out.dumpAcquired = false;
    out.totalPrivateMemory = 0;

    // Read memory maps
    std::string mapsPath = "/proc/" + std::to_string(pid) + "/maps";
    std::ifstream mapsFile(mapsPath);
    if (!mapsFile) {
        std::cerr << "[!] Failed to open " << mapsPath << "\n";
        return false;
    }

    std::string memPath = "/proc/" + std::to_string(pid) + "/mem";
    std::ifstream memFile(memPath, std::ios::binary);
    if (!memFile) {
        std::cerr << "[!] Failed to open " << memPath << "\n";
        return false;
    }

    std::string line;
    int regionCount = 0;

    while (std::getline(mapsFile, line)) {
        // Parse maps line: "start-end perms offset dev inode pathname"
        std::istringstream iss(line);
        std::string range, perms, offset, dev, inode;
        std::string pathname;

        iss >> range >> perms >> offset >> dev >> inode;
        std::getline(iss, pathname); // Rest is pathname

        // Parse range
        size_t dashPos = range.find('-');
        if (dashPos == std::string::npos) continue;

        unsigned long start = std::stoul(range.substr(0, dashPos), nullptr, 16);
        unsigned long end = std::stoul(range.substr(dashPos + 1), nullptr, 16);
        size_t size = end - start;

        bool isReadable = perms[0] == 'r';
        bool isWritable = perms[1] == 'w';
        bool isExecutable = perms[2] == 'x';
        bool isPrivate = perms[3] == 'p';

        if (isPrivate) {
            out.totalPrivateMemory += size;
        }

        // Check for RWX regions
        if (isReadable && isWritable && isExecutable) {
            std::stringstream ss;
            ss << "RWX region at 0x" << std::hex << start << "-0x" << end
               << " (" << std::dec << size << " bytes)";
            if (!pathname.empty()) {
                ss << " " << pathname;
            }
            out.rwxRegions.push_back(ss.str());
        }

        // Scan executable regions for syscalls and PE headers
        if (isExecutable && size > 0 && size < 100 * 1024 * 1024) { // Limit to 100MB
            std::vector<uint8_t> regionData(size);
            memFile.seekg(start);
            memFile.read(reinterpret_cast<char*>(regionData.data()), size);

            if (memFile) {
                // Scan for PE header (injected modules)
                if (size > 2 && regionData[0] == 'M' && regionData[1] == 'Z') {
                    std::stringstream ss;
                    ss << "PE header at 0x" << std::hex << start;
                    if (!pathname.empty()) ss << " (" << pathname << ")";
                    out.injectedCodeRegions.push_back(ss.str());
                }

                // Scan for syscall instructions
                scanForSyscalls(regionData, out.directSyscalls);

                // Extract strings from heap/stack (non-executable, writable)
                if (!isExecutable && isWritable && isPrivate) {
                    extractStringsFromMemory(regionData, out.decryptedStrings);
                }

                regionCount++;
            }
        }
    }

    out.dumpAcquired = true;
    std::cout << "[*] Memory analysis: " << regionCount << " regions scanned, "
              << out.rwxRegions.size() << " RWX, "
              << out.directSyscalls.size() << " syscalls, "
              << out.decryptedStrings.size() << " strings\n";

    return true;
}
#endif

void MemoryAnalyzer::scanForSyscalls(const std::vector<uint8_t>& data,
                                     std::vector<std::string>& out) {
    // x86_64 syscall instruction: 0x0F 0x05
    // int 0x2e: 0xCD 0x2E
    // sysenter: 0x0F 0x34

    for (size_t i = 0; i < data.size() - 1; i++) {
        if (data[i] == 0x0F && data[i + 1] == 0x05) {
            out.push_back("syscall at offset 0x" + std::to_string(i));
        } else if (data[i] == 0xCD && data[i + 1] == 0x2E) {
            out.push_back("int 0x2e at offset 0x" + std::to_string(i));
        } else if (data[i] == 0x0F && data[i + 1] == 0x34) {
            out.push_back("sysenter at offset 0x" + std::to_string(i));
        }
    }
}

void MemoryAnalyzer::extractStringsFromMemory(const std::vector<uint8_t>& data,
                                              std::vector<std::string>& out) {
    const size_t MIN_LEN = 8;
    std::string current;

    for (uint8_t byte : data) {
        if (byte >= 0x20 && byte <= 0x7E) {
            current += static_cast<char>(byte);
        } else {
            if (current.length() >= MIN_LEN) {
                // Filter for interesting strings
                bool hasInteresting = false;
                for (char c : current) {
                    if (c == '.' || c == '/' || c == '\\' || c == ':' ||
                        c == '?' || c == '&' || c == '=') {
                        hasInteresting = true;
                        break;
                    }
                }
                if (hasInteresting) {
                    out.push_back(current);
                }
            }
            current.clear();
        }
    }
}

} // namespace forensic
} // namespace jocky
