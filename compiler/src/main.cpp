#include <iostream>
#include <string>
#include <cstring>
#include "pipeline.h"

using namespace jocky;

static void printUsage(const char* prog) {
    std::cout << "Usage: " << prog << " [options] <input.jky>\n"
              << "Options:\n"
              << "  -o <file>      Output executable name (default: input stem)\n"
              << "  -p <profile>   Obfuscation profile: none, light, standard, aggressive, paranoid (default: standard)\n"
              << "  --pack         Pack final binary with UPX\n"
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
        } else if (std::strcmp(argv[i], "-k") == 0) {
            opts.keepIntermediates = true;
        } else if (std::strcmp(argv[i], "-h") == 0 || std::strcmp(argv[i], "--help") == 0) {
            printUsage(argv[0]);
            return 0;
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

    Pipeline pipeline;
    if (!pipeline.run(opts)) {
        return 1;
    }

    return 0;
}
