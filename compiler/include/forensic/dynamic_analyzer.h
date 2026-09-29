#pragma once

#include "forensic/forensic_analyzer.h"

namespace jocky {
namespace forensic {

class DynamicAnalyzer {
public:
    bool analyze(const std::string& binaryPath, DynamicReport& out);

private:
    bool analyzeLinux(const std::string& binaryPath, DynamicReport& out);
    bool analyzeWindows(const std::string& binaryPath, DynamicReport& out);
};

} // namespace forensic
} // namespace jocky
