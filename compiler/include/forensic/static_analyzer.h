#pragma once

#include "forensic/forensic_analyzer.h"
#include <string>
#include <vector>

namespace jocky {
namespace forensic {

class StaticAnalyzer {
public:
    bool analyze(const std::string& binaryPath, StaticReport& out);

private:
    bool parsePE(const std::string& path, StaticReport& out);
    bool parseELF(const std::string& path, StaticReport& out);
    double calculateEntropy(const std::vector<uint8_t>& data);
    void extractStrings(const std::vector<uint8_t>& data, std::vector<StringArtifact>& out);
    void categorizeString(const std::string& str, StringArtifact& artifact);
    bool detectPackerSignatures(const std::vector<uint8_t>& data, std::vector<std::string>& out);
    bool hasUPXSignature(const std::vector<uint8_t>& data);
    bool isPE(const std::vector<uint8_t>& data);
    bool isELF(const std::vector<uint8_t>& data);
};

} // namespace forensic
} // namespace jocky
