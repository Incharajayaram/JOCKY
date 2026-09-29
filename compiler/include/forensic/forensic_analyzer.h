#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace jocky {
namespace forensic {

struct StringArtifact {
    std::string value;
    size_t offset;
    bool isEncrypted;
    std::string category;
};

struct SectionAnalysis {
    std::string name;
    size_t virtualSize;
    size_t rawSize;
    double entropy;
    bool isExecutable;
    bool isWritable;
    bool isReadable;
    std::string packerHint;
};

struct StaticReport {
    std::vector<SectionAnalysis> sections;
    std::vector<StringArtifact> strings;
    std::vector<std::string> importedDlls;
    std::vector<std::string> importedApis;
    std::vector<std::string> detectedPackers;
    bool hasRichHeader = false;
    bool hasDebugInfo = false;
    bool hasOverlay = false;
    size_t overlaySize = 0;
};

struct DynamicReport {
    bool executedSuccessfully = false;
    bool detectedSandbox = false;
    std::vector<std::string> createdProcesses;
    std::vector<std::string> fileOperations;
    std::vector<std::string> registryOperations;
    std::vector<std::string> networkConnections;
    double executionTimeMs = 0.0;
};

struct MemoryReport {
    bool dumpAcquired = false;
    std::vector<std::string> injectedCodeRegions;
    std::vector<std::string> unlinkedDlls;
    std::vector<std::string> rwxRegions;
    std::vector<std::string> decryptedStrings;
    std::vector<std::string> directSyscalls;
    size_t totalPrivateMemory = 0;
};

struct TechniqueScore {
    std::string name;
    int score;
    std::string assessment;
    std::vector<std::string> findings;
};

struct ForensicReport {
    std::string binaryPath;
    std::string binaryHash;
    std::string profileUsed;
    int overallScore = 0;
    std::string riskLevel;
    StaticReport staticAnalysis;
    DynamicReport dynamicAnalysis;
    MemoryReport memoryAnalysis;
    std::vector<TechniqueScore> techniqueScores;
    std::vector<std::string> recommendations;
};

class ForensicAnalyzer {
public:
    bool analyze(const std::string& binaryPath, ForensicReport& out);
    bool analyzeOnly(const std::string& binaryPath, ForensicReport& out);
    bool saveReport(const ForensicReport& report, const std::string& path);
    bool saveHtmlReport(const ForensicReport& report, const std::string& path);
    void printSummary(const ForensicReport& report);

private:
    bool calculateFileHash(const std::string& path, std::string& outHash);
};

} // namespace forensic
} // namespace jocky
