#include "forensic/evasion_scorer.h"
#include <algorithm>

namespace jocky {
namespace forensic {

void EvasionScorer::calculateScores(const StaticReport& staticReport,
                                   const DynamicReport& dynamicReport,
                                   const MemoryReport& memoryReport,
                                   const std::string& profile,
                                   std::vector<TechniqueScore>& outScores,
                                   int& outOverall,
                                   std::vector<std::string>& outRecommendations) {
    outScores.clear();
    outRecommendations.clear();

    // String encryption score
    {
        TechniqueScore score;
        score.name = "string_encryption";
        score.score = scoreStringEncryption(staticReport, memoryReport);
        if (score.score >= 90) score.assessment = "excellent";
        else if (score.score >= 75) score.assessment = "good";
        else if (score.score >= 50) score.assessment = "fair";
        else score.assessment = "poor";

        if (score.score < 75) {
            int visibleCount = 0;
            for (const auto& s : staticReport.strings) {
                if (!s.isEncrypted) visibleCount++;
            }
            score.findings.push_back("Visible strings in binary: " + std::to_string(visibleCount));
            outRecommendations.push_back("Increase string encryption coverage; " +
                std::to_string(visibleCount) + " strings remain visible");
        }
        if (!memoryReport.decryptedStrings.empty()) {
            score.findings.push_back("Decrypted strings found in memory: " +
                std::to_string(memoryReport.decryptedStrings.size()));
            outRecommendations.push_back("Review memory encryption; " +
                std::to_string(memoryReport.decryptedStrings.size()) +
                " strings decrypted in memory");
        }
        outScores.push_back(score);
    }

    // Import obfuscation score
    {
        TechniqueScore score;
        score.name = "import_obfuscation";
        score.score = scoreImportObfuscation(staticReport);
        if (score.score >= 90) score.assessment = "excellent";
        else if (score.score >= 75) score.assessment = "good";
        else if (score.score >= 50) score.assessment = "fair";
        else score.assessment = "poor";

        if (!staticReport.importedApis.empty()) {
            score.findings.push_back("Resolved APIs in IAT: " +
                std::to_string(staticReport.importedApis.size()));
        }
        outScores.push_back(score);
    }

    // Packing score
    {
        TechniqueScore score;
        score.name = "packing";
        score.score = scorePacking(staticReport);
        if (score.score >= 90) score.assessment = "excellent";
        else if (score.score >= 75) score.assessment = "good";
        else if (score.score >= 50) score.assessment = "fair";
        else score.assessment = "poor";

        if (!staticReport.detectedPackers.empty()) {
            score.findings.push_back("Detected packers: " +
                staticReport.detectedPackers[0]);
            outRecommendations.push_back("Consider custom packer; " +
                staticReport.detectedPackers[0] + " is easily detectable");
        }
        outScores.push_back(score);
    }

    // Anti-analysis score
    {
        TechniqueScore score;
        score.name = "anti_analysis";
        score.score = scoreAntiAnalysis(dynamicReport);
        if (score.score >= 90) score.assessment = "excellent";
        else if (score.score >= 75) score.assessment = "good";
        else if (score.score >= 50) score.assessment = "fair";
        else score.assessment = "poor";

        if (dynamicReport.detectedSandbox) {
            score.findings.push_back("Binary detected sandbox environment");
            outRecommendations.push_back("Sandbox detection triggered; review anti-VM techniques");
        }
        outScores.push_back(score);
    }

    // Direct syscalls score
    {
        TechniqueScore score;
        score.name = "direct_syscalls";
        score.score = scoreDirectSyscalls(memoryReport);
        if (score.score >= 90) score.assessment = "excellent";
        else if (score.score >= 75) score.assessment = "good";
        else if (score.score >= 50) score.assessment = "fair";
        else score.assessment = "poor";

        if (!memoryReport.directSyscalls.empty()) {
            score.findings.push_back("Direct syscalls detected: " +
                std::to_string(memoryReport.directSyscalls.size()));
        }
        outScores.push_back(score);
    }

    // Calculate overall score with profile-specific weights
    int totalWeight = 0;
    int weightedSum = 0;

    if (profile == "light") {
        // Light: packing > strings > imports
        for (const auto& s : outScores) {
            if (s.name == "packing") { weightedSum += s.score * 4; totalWeight += 4; }
            else if (s.name == "string_encryption") { weightedSum += s.score * 3; totalWeight += 3; }
            else if (s.name == "import_obfuscation") { weightedSum += s.score * 3; totalWeight += 3; }
            else { weightedSum += s.score * 1; totalWeight += 1; }
        }
    } else if (profile == "standard") {
        // Standard: balanced
        for (const auto& s : outScores) {
            weightedSum += s.score * 2;
            totalWeight += 2;
        }
    } else if (profile == "aggressive" || profile == "paranoid") {
        // Aggressive/Paranoid: all techniques equal, bonus for anti-analysis
        for (const auto& s : outScores) {
            if (s.name == "anti_analysis" && profile == "paranoid") {
                weightedSum += s.score * 3;
                totalWeight += 3;
            } else {
                weightedSum += s.score * 2;
                totalWeight += 2;
            }
        }
    } else {
        // Default: simple average
        for (const auto& s : outScores) {
            weightedSum += s.score;
            totalWeight++;
        }
    }

    outOverall = totalWeight > 0 ? (weightedSum / totalWeight) : 0;

    // Clamp to 0-100
    outOverall = std::max(0, std::min(100, outOverall));
}

int EvasionScorer::scoreStringEncryption(const StaticReport& staticReport,
                                         const MemoryReport& memoryReport) {
    // Count visible strings
    size_t visibleStrings = 0;
    size_t totalStrings = staticReport.strings.size();

    for (const auto& s : staticReport.strings) {
        if (!s.isEncrypted &&
            (s.category == "url" || s.category == "ip" ||
             s.category == "registry" || s.category == "path")) {
            visibleStrings++;
        }
    }

    // Base score: percentage of sensitive strings that are hidden
    int score = 100;
    if (totalStrings > 0) {
        score = static_cast<int>(100.0 * (1.0 - static_cast<double>(visibleStrings) / totalStrings));
    }

    // Penalty for decrypted strings in memory
    if (!memoryReport.decryptedStrings.empty()) {
        int penalty = std::min(30, static_cast<int>(memoryReport.decryptedStrings.size()) * 5);
        score -= penalty;
    }

    return std::max(0, score);
}

int EvasionScorer::scoreImportObfuscation(const StaticReport& staticReport) {
    if (staticReport.importedApis.empty()) {
        return 100; // No imports = fully obfuscated
    }

    // Fewer imports = better obfuscation
    // Threshold: < 5 imports = excellent, < 15 = good, < 30 = fair, >= 30 = poor
    size_t count = staticReport.importedApis.size();
    if (count < 5) return 95;
    if (count < 10) return 85;
    if (count < 20) return 70;
    if (count < 30) return 55;
    return 40;
}

int EvasionScorer::scoreControlFlowObfuscation(const StaticReport& staticReport) {
    // Control flow obfuscation is hard to measure statically
    // Heuristic: high entropy in .text section suggests obfuscation
    for (const auto& section : staticReport.sections) {
        if (section.isExecutable && section.entropy > 6.5) {
            return 80; // Likely obfuscated
        }
    }
    return 50; // Unknown
}

int EvasionScorer::scorePacking(const StaticReport& staticReport) {
    int score = 85; // Base score for being packed

    // Penalty for detectable packers
    for (const auto& packer : staticReport.detectedPackers) {
        if (packer == "UPX") score -= 20;
        else if (packer == "VMProtect") score -= 5;
        else if (packer == "Themida") score -= 5;
    }

    // Check entropy distribution
    bool hasHighEntropySection = false;
    for (const auto& section : staticReport.sections) {
        if (section.entropy > 7.0) {
            hasHighEntropySection = true;
            break;
        }
    }

    if (!hasHighEntropySection) {
        score -= 10; // No high entropy section = might not be packed well
    }

    return std::max(0, score);
}

int EvasionScorer::scoreAntiAnalysis(const DynamicReport& dynamicReport) {
    if (!dynamicReport.executedSuccessfully) {
        return 50; // Couldn't execute, neutral score
    }

    int score = 70; // Base score

    if (!dynamicReport.detectedSandbox) {
        score += 20; // Didn't detect sandbox = good
    } else {
        score -= 20; // Detected sandbox = might behave differently
    }

    // Bonus for no suspicious behavior
    if (dynamicReport.createdProcesses.empty()) score += 5;
    if (dynamicReport.networkConnections.empty()) score += 5;

    return std::min(100, score);
}

int EvasionScorer::scoreDirectSyscalls(const MemoryReport& memoryReport) {
    if (memoryReport.directSyscalls.empty()) {
        return 100; // No direct syscalls detected = not using them or well hidden
    }

    // Having direct syscalls is a technique; being detected is the issue
    // If we found them in memory, they are detectable
    int count = static_cast<int>(memoryReport.directSyscalls.size());
    if (count < 3) return 85;
    if (count < 5) return 75;
    if (count < 10) return 60;
    return 45;
}

int EvasionScorer::scoreMemoryArtifacts(const MemoryReport& memoryReport) {
    int score = 90;

    if (!memoryReport.injectedCodeRegions.empty()) {
        score -= 10;
    }
    if (!memoryReport.rwxRegions.empty()) {
        score -= 15;
    }
    if (!memoryReport.unlinkedDlls.empty()) {
        score -= 10;
    }

    return std::max(0, score);
}

} // namespace forensic
} // namespace jocky
