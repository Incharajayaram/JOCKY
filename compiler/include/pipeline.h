#pragma once

#include <string>
#include <vector>

namespace jocky {

struct CompileOptions {
    std::string inputFile;
    std::string outputFile;
    std::string profile = "standard";
    bool keepIntermediates = false;
    bool windows = false;
    bool pack = false;
    bool noRuntime = false;
    bool encryptStrings = false;
    std::string target;
};

class Pipeline {
public:
    bool run(const CompileOptions& opts);

private:
    bool emitLLVMIR(const CompileOptions& opts, const std::string& llPath);
    bool runObfuscation(const CompileOptions& opts, const std::string& inBc, const std::string& outBc);
    bool compileToObject(const CompileOptions& opts, const std::string& bc, const std::string& obj);
    bool compileRuntime(const std::string& clang, const std::string& outDir, std::vector<std::string>& outObjs, const std::string& targetFlag);
    bool linkExecutable(const CompileOptions& opts, const std::string& clang,
                        const std::string& obj, const std::vector<std::string>& runtimeObjs,
                        const std::string& exe);
    bool findToolchain(std::string& outClang, std::string& outOpt, std::string& outPlugin);
    bool findMLIRTools(std::string& outTranslate, std::string& outMlirOpt, std::string& outMlirPlugin);
    bool runMLIRObfuscation(const std::string& inLl, const std::string& outLl,
                            const std::string& translate, const std::string& mlirOpt,
                            const std::string& mlirPlugin);
};

} // namespace jocky
