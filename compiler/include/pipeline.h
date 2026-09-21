#pragma once

#include <string>
#include <vector>

namespace jocky {

struct CompileOptions {
    std::string inputFile;
    std::string outputFile = "a.out";
    std::string profile = "standard";
    bool keepIntermediates = false;
    bool windows = false;
};

class Pipeline {
public:
    bool run(const CompileOptions& opts);

private:
    bool emitLLVMIR(const CompileOptions& opts, const std::string& llPath);
    bool runObfuscation(const CompileOptions& opts, const std::string& inBc, const std::string& outBc);
    bool compileToObject(const CompileOptions& opts, const std::string& bc, const std::string& obj);
    bool linkExecutable(const CompileOptions& opts, const std::string& obj, const std::string& exe);
    bool findToolchain(std::string& outClang, std::string& outOpt, std::string& outPlugin);
};

} // namespace jocky
