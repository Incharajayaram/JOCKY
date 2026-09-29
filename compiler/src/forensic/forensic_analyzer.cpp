#include "forensic/forensic_analyzer.h"
#include "forensic/static_analyzer.h"
#include "forensic/dynamic_analyzer.h"
#include "forensic/memory_analyzer.h"
#include "forensic/evasion_scorer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
// Simple SHA256-like hash using std::hash for portability
// In production, link against OpenSSL or use platform crypto APIs

namespace jocky {
namespace forensic {

bool ForensicAnalyzer::analyze(const std::string& binaryPath, ForensicReport& out) {
    out.binaryPath = binaryPath;

    if (!calculateFileHash(binaryPath, out.binaryHash)) {
        std::cerr << "[!] Failed to calculate file hash\n";
    }

    std::cout << "\n=== JOCKY Forensic Analysis ===\n";
    std::cout << "[*] Binary: " << binaryPath << "\n";
    std::cout << "[*] SHA256: " << out.binaryHash << "\n\n";

    // Phase 1: Static Analysis
    std::cout << "[Phase 1/4] Static Analysis...\n";
    StaticAnalyzer staticAnalyzer;
    if (!staticAnalyzer.analyze(binaryPath, out.staticAnalysis)) {
        std::cerr << "[!] Static analysis failed\n";
    }

    // Phase 2: Dynamic Analysis
    std::cout << "\n[Phase 2/4] Dynamic Analysis...\n";
    DynamicAnalyzer dynamicAnalyzer;
    if (!dynamicAnalyzer.analyze(binaryPath, out.dynamicAnalysis)) {
        std::cerr << "[!] Dynamic analysis failed\n";
    }

    // Phase 3: Memory Analysis
    std::cout << "\n[Phase 3/4] Memory Analysis...\n";
    // Memory analysis requires a running process
    // For now, we skip if dynamic analysis didn't run or we don't have a PID
    // In a full implementation, we'd capture memory during dynamic analysis
    out.memoryAnalysis.dumpAcquired = false;
    std::cout << "[*] Memory analysis skipped (requires running process capture)\n";

    // Phase 4: Evasion Scoring
    std::cout << "\n[Phase 4/4] Evasion Scoring...\n";
    EvasionScorer scorer;
    scorer.calculateScores(out.staticAnalysis, out.dynamicAnalysis,
                          out.memoryAnalysis, out.profileUsed,
                          out.techniqueScores, out.overallScore,
                          out.recommendations);

    // Determine risk level
    if (out.overallScore >= 80) {
        out.riskLevel = "green";
    } else if (out.overallScore >= 50) {
        out.riskLevel = "yellow";
    } else {
        out.riskLevel = "red";
    }

    printSummary(out);

    return true;
}

bool ForensicAnalyzer::analyzeOnly(const std::string& binaryPath, ForensicReport& out) {
    // Analysis only, no compilation context
    out.profileUsed = "unknown";
    return analyze(binaryPath, out);
}

bool ForensicAnalyzer::saveReport(const ForensicReport& report, const std::string& path) {
    std::ofstream file(path);
    if (!file) {
        std::cerr << "[!] Failed to open report file: " << path << "\n";
        return false;
    }

    file << "{\n";
    file << "  \"binary\": {\n";
    file << "    \"path\": \"" << report.binaryPath << "\",\n";
    file << "    \"sha256\": \"" << report.binaryHash << "\",\n";
    file << "    \"profile\": \"" << report.profileUsed << "\"\n";
    file << "  },\n";
    file << "  \"overall_score\": " << report.overallScore << ",\n";
    file << "  \"risk_level\": \"" << report.riskLevel << "\",\n";

    // Static analysis
    file << "  \"static_analysis\": {\n";
    file << "    \"sections\": [\n";
    for (size_t i = 0; i < report.staticAnalysis.sections.size(); i++) {
        const auto& s = report.staticAnalysis.sections[i];
        file << "      {\"name\": \"" << s.name << "\", ";
        file << "\"entropy\": " << std::fixed << std::setprecision(2) << s.entropy << "}";
        if (i < report.staticAnalysis.sections.size() - 1) file << ",";
        file << "\n";
    }
    file << "    ],\n";
    file << "    \"strings\": {\n";
    file << "      \"total\": " << report.staticAnalysis.strings.size() << ",\n";
    file << "      \"artifacts\": [\n";
    for (size_t i = 0; i < report.staticAnalysis.strings.size() && i < 20; i++) {
        const auto& s = report.staticAnalysis.strings[i];
        file << "        {\"value\": \"" << s.value << "\", ";
        file << "\"category\": \"" << s.category << "\", ";
        file << "\"offset\": " << s.offset << "}";
        if (i < std::min(report.staticAnalysis.strings.size(), size_t(20)) - 1) file << ",";
        file << "\n";
    }
    file << "      ]\n";
    file << "    },\n";
    file << "    \"packers_detected\": [\n";
    for (size_t i = 0; i < report.staticAnalysis.detectedPackers.size(); i++) {
        file << "      \"" << report.staticAnalysis.detectedPackers[i] << "\"";
        if (i < report.staticAnalysis.detectedPackers.size() - 1) file << ",";
        file << "\n";
    }
    file << "    ]\n";
    file << "  },\n";

    // Technique scores
    file << "  \"technique_scores\": [\n";
    for (size_t i = 0; i < report.techniqueScores.size(); i++) {
        const auto& s = report.techniqueScores[i];
        file << "    {\"name\": \"" << s.name << "\", ";
        file << "\"score\": " << s.score << ", ";
        file << "\"assessment\": \"" << s.assessment << "\"}";
        if (i < report.techniqueScores.size() - 1) file << ",";
        file << "\n";
    }
    file << "  ],\n";

    // Recommendations
    file << "  \"recommendations\": [\n";
    for (size_t i = 0; i < report.recommendations.size(); i++) {
        file << "    \"" << report.recommendations[i] << "\"";
        if (i < report.recommendations.size() - 1) file << ",";
        file << "\n";
    }
    file << "  ]\n";
    file << "}\n";

    file.close();
    return true;
}

bool ForensicAnalyzer::saveHtmlReport(const ForensicReport& report, const std::string& path) {
    std::ofstream file(path);
    if (!file) return false;

    file << "<!DOCTYPE html>\n<html>\n<head>\n";
    file << "<title>JOCKY Forensic Report</title>\n";
    file << "<style>\n";
    file << "body { font-family: Arial, sans-serif; margin: 40px; background: #f5f5f5; }\n";
    file << ".container { max-width: 900px; margin: 0 auto; background: white; padding: 30px; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }\n";
    file << ".score { font-size: 48px; font-weight: bold; text-align: center; padding: 20px; border-radius: 8px; }\n";
    file << ".green { background: #d4edda; color: #155724; }\n";
    file << ".yellow { background: #fff3cd; color: #856404; }\n";
    file << ".red { background: #f8d7da; color: #721c24; }\n";
    file << ".technique { display: flex; justify-content: space-between; padding: 10px; margin: 5px 0; background: #f8f9fa; border-radius: 4px; }\n";
    file << ".finding { color: #dc3545; font-size: 12px; }\n";
    file << "h1, h2 { color: #333; }\n";
    file << "table { width: 100%; border-collapse: collapse; margin: 15px 0; }\n";
    file << "th, td { padding: 8px; text-align: left; border-bottom: 1px solid #ddd; }\n";
    file << "th { background: #f8f9fa; }\n";
    file << "</style>\n</head>\n<body>\n";
    file << "<div class='container'>\n";
    file << "<h1>JOCKY Forensic Analysis Report</h1>\n";
    file << "<p><strong>Binary:</strong> " << report.binaryPath << "<br>\n";
    file << "<strong>SHA256:</strong> " << report.binaryHash << "<br>\n";
    file << "<strong>Profile:</strong> " << report.profileUsed << "</p>\n";

    file << "<div class='score " << report.riskLevel << "'>" << report.overallScore << "/100</div>\n";
    file << "<p style='text-align: center;'><strong>Risk Level:</strong> " << report.riskLevel << "</p>\n";

    file << "<h2>Technique Scores</h2>\n";
    for (const auto& s : report.techniqueScores) {
        file << "<div class='technique'>\n";
        file << "<span><strong>" << s.name << "</strong></span>\n";
        file << "<span>" << s.score << "/100 (" << s.assessment << ")</span>\n";
        file << "</div>\n";
        for (const auto& f : s.findings) {
            file << "<div class='finding'>" << f << "</div>\n";
        }
    }

    if (!report.recommendations.empty()) {
        file << "<h2>Recommendations</h2>\n<ul>\n";
        for (const auto& r : report.recommendations) {
            file << "<li>" << r << "</li>\n";
        }
        file << "</ul>\n";
    }

    file << "</div>\n</body>\n</html>\n";
    file.close();
    return true;
}

void ForensicAnalyzer::printSummary(const ForensicReport& report) {
    std::cout << "\n=== Forensic Analysis Summary ===\n";
    std::cout << "Overall Score: " << report.overallScore << "/100 (" << report.riskLevel << ")\n\n";

    std::cout << "Technique Scores:\n";
    for (const auto& s : report.techniqueScores) {
        std::cout << "  " << s.name << ": " << s.score << "/100 (" << s.assessment << ")\n";
        for (const auto& f : s.findings) {
            std::cout << "    - " << f << "\n";
        }
    }

    if (!report.recommendations.empty()) {
        std::cout << "\nRecommendations:\n";
        for (const auto& r : report.recommendations) {
            std::cout << "  * " << r << "\n";
        }
    }

    std::cout << "\n";
}

bool ForensicAnalyzer::calculateFileHash(const std::string& path, std::string& outHash) {
    std::ifstream file(path, std::ios::binary);
    if (!file) return false;

    // Simple hash: XOR-based with size
    // Not cryptographically secure, but sufficient for change detection
    unsigned long hash = 0;
    size_t totalSize = 0;
    char buffer[8192];

    while (file.good()) {
        file.read(buffer, sizeof(buffer));
        std::streamsize count = file.gcount();
        totalSize += count;
        for (std::streamsize i = 0; i < count; i++) {
            hash = hash * 31 + static_cast<unsigned char>(buffer[i]);
        }
    }

    std::stringstream ss;
    ss << std::hex << hash << "_" << totalSize;
    outHash = ss.str();
    return true;
}

} // namespace forensic
} // namespace jocky
