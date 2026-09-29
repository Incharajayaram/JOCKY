#pragma once

#include "forensic/forensic_analyzer.h"

namespace jocky {
namespace forensic {

class MemoryAnalyzer {
public:
    bool analyze(pid_t pid, MemoryReport& out);

private:
    bool analyzeLinux(pid_t pid, MemoryReport& out);
    void scanForSyscalls(const std::vector<uint8_t>& data, std::vector<std::string>& out);
    void extractStringsFromMemory(const std::vector<uint8_t>& data, std::vector<std::string>& out);
};

} // namespace forensic
} // namespace jocky
