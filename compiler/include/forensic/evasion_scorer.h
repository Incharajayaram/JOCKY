#pragma once

#include "forensic/forensic_analyzer.h"

namespace jocky {
namespace forensic {

class EvasionScorer {
public:
    void calculateScores(const StaticReport& staticReport,
                        const DynamicReport& dynamicReport,
                        const MemoryReport& memoryReport,
                        const std::string& profile,
                        std::vector<TechniqueScore>& outScores,
                        int& outOverall,
                        std::vector<std::string>& outRecommendations);

private:
    int scoreStringEncryption(const StaticReport& staticReport, const MemoryReport& memoryReport);
    int scoreImportObfuscation(const StaticReport& staticReport);
    int scoreControlFlowObfuscation(const StaticReport& staticReport);
    int scorePacking(const StaticReport& staticReport);
    int scoreAntiAnalysis(const DynamicReport& dynamicReport);
    int scoreDirectSyscalls(const MemoryReport& memoryReport);
    int scoreMemoryArtifacts(const MemoryReport& memoryReport);
};

} // namespace forensic
} // namespace jocky
