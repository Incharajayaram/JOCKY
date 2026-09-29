#include <iostream>
#include <string>
#include <cstring>
#include "pipeline.h"
#include "forensic/forensic_analyzer.h"

using namespace jocky;

static void printUsage(const char* prog) {
    std::cout << "Usage: " << prog << " [options] <input.jky|input.c>\n"
              << "Options:\n"
              << "  -o <file>      Output executable name (default: input stem)\n"
              << "  -p <profile>   Obfuscation profile: none, light, standard, aggressive, paranoid (default: standard)\n"
              << "  --pack         Encrypt PE sections (Windows) or UPX-pack (Linux)\n"
              << "  --embed-driver <path>  Embed a .sys driver in .jdrv PE section (Windows only)\n"
              << "  --manifest <path>      Embed a driver IOCTL manifest in .jmani PE section (Windows only)\n"
              << "  --encrypt-strings  Encrypt string literals via MLIR\n"
              << "  --no-runtime   Skip anti-analysis runtime (cleaner binary)\n"
              << "  --cc           Compile C source through JOCKY pipeline\n"
              << "  --target <triple>  Cross-compile target (e.g., x86_64-w64-mingw32)\n"
              << "  --msvc         Use MSVC toolchain (clang-cl + lld-link) for Windows\n"
              << "  --forensic     Run forensic analysis after compilation\n"
              << "  --analyze-only <file>  Analyze existing binary (no compilation)\n"
              << "  --report-format <fmt>  Forensic report format: json, html (default: json)\n"
              << "  --static       Link statically (no dynamic dependencies)\n"
              << "  --byovd        Include BYOVD runtime (Windows kernel driver exploit)\n"
              << "  -k             Keep intermediate files\n"
              << "  -h             Show this help\n";
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    CompileOptions opts;
    std::string input;

    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            opts.outputFile = argv[++i];
        } else if (std::strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            opts.profile = argv[++i];
        } else if (std::strcmp(argv[i], "--pack") == 0) {
            opts.pack = true;
        } else if (std::strcmp(argv[i], "--encrypt-strings") == 0) {
            opts.encryptStrings = true;
        } else if (std::strcmp(argv[i], "--no-runtime") == 0) {
            opts.noRuntime = true;
        } else if (std::strcmp(argv[i], "--target") == 0 && i + 1 < argc) {
            opts.target = argv[++i];
        } else if (std::strcmp(argv[i], "--forensic") == 0) {
            opts.runForensic = true;
        } else if (std::strcmp(argv[i], "--analyze-only") == 0 && i + 1 < argc) {
            opts.analyzeOnly = true;
            input = argv[++i];
        } else if (std::strcmp(argv[i], "--report-format") == 0 && i + 1 < argc) {
            opts.reportFormat = argv[++i];
        } else if (std::strcmp(argv[i], "--static") == 0) {
            opts.staticLink = true;
        } else if (std::strcmp(argv[i], "--byovd") == 0) {
            opts.byovd = true;
        } else if (std::strcmp(argv[i], "-k") == 0) {
            opts.keepIntermediates = true;
        } else if (std::strcmp(argv[i], "-h") == 0 || std::strcmp(argv[i], "--help") == 0) {
            printUsage(argv[0]);
            return 0;
        } else if (std::strcmp(argv[i], "--cc") == 0) {
            opts.isCInput = true;
        } else if (std::strcmp(argv[i], "--msvc") == 0) {
            opts.useMsvc = true;
        } else if (std::strcmp(argv[i], "--embed-driver") == 0 && i + 1 < argc) {
            opts.embedDriverPath = argv[++i];
        } else if (std::strcmp(argv[i], "--manifest") == 0 && i + 1 < argc) {
            opts.manifestPath = argv[++i];
        } else if (argv[i][0] != '-') {
            input = argv[i];
        } else {
            std::cerr << "[!] Unknown option: " << argv[i] << "\n";
            return 1;
        }
    }

    if (input.empty()) {
        std::cerr << "[!] No input file specified\n";
        return 1;
    }

    opts.inputFile = input;

    // Auto-detect C input from file extension
    if (!opts.isCInput) {
        size_t dot = input.find_last_of('.');
        if (dot != std::string::npos) {
            std::string ext = input.substr(dot);
            if (ext == ".c" || ext == ".C") {
                opts.isCInput = true;
            }
        }
    }
    
    if (opts.outputFile.empty()) {
        // Default output name = input file stem (like Python pipeline)
        size_t slash = input.find_last_of("/\\");
        std::string name = (slash != std::string::npos) ? input.substr(slash + 1) : input;
        size_t dot = name.find_last_of('.');
        if (dot != std::string::npos) name = name.substr(0, dot);
#ifdef _WIN32
        opts.outputFile = name + ".exe";
#else
        opts.outputFile = name;
#endif
    }

    // Handle analyze-only mode
    if (opts.analyzeOnly) {
        if (input.empty()) {
            std::cerr << "[!] No binary file specified for analysis\n";
            return 1;
        }

        jocky::forensic::ForensicAnalyzer analyzer;
        jocky::forensic::ForensicReport report;

        if (!analyzer.analyzeOnly(input, report)) {
            return 1;
        }

        std::string reportPath = input + ".forensic." + opts.reportFormat;
        if (opts.reportFormat == "html") {
            analyzer.saveHtmlReport(report, reportPath);
        } else {
            analyzer.saveReport(report, reportPath);
        }
        std::cout << "[*] Report saved: " << reportPath << "\n";
        return 0;
    }

    Pipeline pipeline;
    if (!pipeline.run(opts)) {
        return 1;
    }

    // Run forensic analysis if requested
    if (opts.runForensic) {
        jocky::forensic::ForensicAnalyzer analyzer;
        jocky::forensic::ForensicReport report;
        report.profileUsed = opts.profile;

        if (analyzer.analyze(opts.outputFile, report)) {
            std::string reportPath = opts.outputFile + ".forensic." + opts.reportFormat;
            if (opts.reportFormat == "html") {
                analyzer.saveHtmlReport(report, reportPath);
            } else {
                analyzer.saveReport(report, reportPath);
            }
            std::cout << "[*] Forensic report saved: " << reportPath << "\n";

            if (report.overallScore < 60) {
                std::cerr << "[!] WARNING: Binary scored " << report.overallScore
                         << "/100. Review recommendations.\n";
            }
        }
    }

    return 0;
}
