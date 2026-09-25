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
    bool isCInput = false;
    bool useMsvc = false;
    std::string embedDriverPath;   // path to .sys driver to embed in .jdrv section
    std::string manifestPath;      // path to .txt manifest to embed in .jmani section
};

class Pipeline {
public:
    bool run(const CompileOptions& opts);

private:
    bool emitLLVMIR(const CompileOptions& opts, const std::string& llPath);
    bool runObfuscation(const CompileOptions& opts, const std::string& inBc, const std::string& outBc);
    bool compileToObject(const CompileOptions& opts, const std::string& bc, const std::string& obj);
    bool compileRuntime(const std::string& clang, const std::string& outDir, std::vector<std::string>& outObjs, const std::string& targetFlag, bool noRuntime);
    bool linkExecutable(const CompileOptions& opts, const std::string& clang,
                        const std::string& obj, const std::vector<std::string>& runtimeObjs,
                        const std::string& exe);
    bool findToolchain(const CompileOptions& opts, std::string& outClang, std::string& outOpt, std::string& outPlugin);
    bool findMsvcToolchain(std::string& outClangCl, std::string& outLldLink,
                           std::string& outWindowsSdk, std::string& outVcTools);
    bool findMLIRTools(std::string& outTranslate, std::string& outMlirOpt, std::string& outMlirPlugin);
    bool runMLIRObfuscation(const std::string& inLl, const std::string& outLl,
                            const std::string& translate, const std::string& mlirOpt,
                            const std::string& mlirPlugin);
};

} // namespace jocky
