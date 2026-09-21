#include "pipeline.h"
#include "lexer.h"
#include "parser.h"
#include "codegen.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <filesystem>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>

namespace jocky {

static bool exec(const std::string& cmd) {
    int ret = std::system(cmd.c_str());
    return ret == 0;
}

bool Pipeline::findToolchain(std::string& outClang, std::string& outOpt, std::string& outPlugin) {
    const char* env = std::getenv("JOCKY_LLVM_TOOLCHAIN");
    std::string prefix;
    if (env) {
        prefix = env;
    } else {
        const char* path = std::getenv("PATH");
        if (path) {
            std::stringstream ss(path);
            std::string dir;
#ifdef _WIN32
            while (std::getline(ss, dir, ';')) {
                std::string clang = dir + "/clang.exe";
                std::string opt = dir + "/opt.exe";
#else
            while (std::getline(ss, dir, ':')) {
                std::string clang = dir + "/clang";
                std::string opt = dir + "/opt";
#endif
                if (std::filesystem::exists(clang) && std::filesystem::exists(opt)) {
                    prefix = dir + "/..";
                    break;
                }
            }
        }
        if (prefix.empty()) {
#ifdef _WIN32
            std::vector<std::string> candidates = {
                "C:/Program Files/LLVM",
                "C:/LLVM",
            };
            namespace fs = std::filesystem;
            fs::path compilerSrc = fs::path(__FILE__).parent_path().parent_path();
            candidates.push_back((compilerSrc / ".." / "llvm").string());
            for (const auto& cand : candidates) {
                if (std::filesystem::exists(cand + "/bin/clang.exe")) {
                    prefix = cand;
                    break;
                }
            }
#else
            prefix = "/home/kamini/projects/llvm-obfuscation-tools-linux-x86_64";
#endif
        }
    }

#ifdef _WIN32
    outClang = prefix + "/bin/clang.exe";
    outOpt = prefix + "/bin/opt.exe";
    outPlugin = prefix + "/lib/LLVMObfuscationPlugin.dll";
#else
    outClang = prefix + "/bin/clang";
    outOpt = prefix + "/bin/opt";
    outPlugin = prefix + "/lib/LLVMObfuscationPlugin.so";
#endif

    if (!std::filesystem::exists(outClang)) {
        std::cerr << "[!] clang not found: " << outClang << "\n";
        return false;
    }
    if (!std::filesystem::exists(outOpt)) {
        std::cerr << "[!] opt not found: " << outOpt << "\n";
        return false;
    }
    return true;
}

bool Pipeline::emitLLVMIR(const CompileOptions& opts, const std::string& llPath) {
    std::ifstream in(opts.inputFile);
    if (!in) {
        std::cerr << "[!] Cannot open: " << opts.inputFile << "\n";
        return false;
    }
    std::string source((std::istreambuf_iterator<char>(in)),
                        std::istreambuf_iterator<char>());

    try {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(std::move(tokens));
        auto ast = parser.parse();

        CodeGen codegen;
        auto mod = codegen.generate(*ast, opts.inputFile);

        std::error_code ec;
        llvm::raw_fd_ostream out(llPath, ec);
        if (ec) {
            std::cerr << "[!] Cannot write " << llPath << ": " << ec.message() << "\n";
            return false;
        }
        mod->print(out, nullptr);
    } catch (const std::exception& e) {
        std::cerr << "[!] Compilation error: " << e.what() << "\n";
        return false;
    }
    return true;
}

bool Pipeline::runObfuscation(const CompileOptions& opts, const std::string& inBc, const std::string& outBc) {
    std::string clang, opt, plugin;
    if (!findToolchain(clang, opt, plugin)) return false;

    std::string cmd1 = clang + " -O1 -c -emit-llvm -x ir -Wno-override-module " + inBc + " -o " + inBc + ".tmp.bc";
    if (!exec(cmd1)) {
        std::cerr << "[!] Failed to compile IR to bitcode\n";
        return false;
    }

    std::string passes;
    if (opts.profile == "none") {
        passes = "";
    } else if (opts.profile == "light") {
        passes = "function(substitution)";
    } else if (opts.profile == "standard") {
        passes = "function(boguscf,flattening,substitution,linear-mba,opaque-pred)";
    } else if (opts.profile == "aggressive") {
        passes = "strip-signature,function(boguscf,flattening,substitution,split,linear-mba,opaque-pred),anti-debug,indirect-call,virtualize";
    } else if (opts.profile == "paranoid") {
        passes = "strip-signature,pdata-strip,function(boguscf,flattening,substitution,split,linear-mba,opaque-pred),anti-debug,indirect-call,virtualize";
    } else {
        std::cerr << "[!] Unknown profile: " << opts.profile << ", using standard\n";
        passes = "function(boguscf,flattening,substitution,linear-mba,opaque-pred)";
    }

    if (!passes.empty()) {
        std::string cmd2 = opt + " -load-pass-plugin=" + plugin +
                           " -passes='" + passes + "' " +
                           inBc + ".tmp.bc -o " + outBc;
        if (!exec(cmd2)) {
            std::cerr << "[!] Obfuscation passes failed\n";
            return false;
        }
    } else {
        std::filesystem::copy(inBc + ".tmp.bc", outBc,
                              std::filesystem::copy_options::overwrite_existing);
    }

    std::filesystem::remove(inBc + ".tmp.bc");
    return true;
}

bool Pipeline::compileToObject(const CompileOptions& opts, const std::string& bc, const std::string& obj) {
    std::string clang, opt, plugin;
    if (!findToolchain(clang, opt, plugin)) return false;

    std::string cmd = clang + " -c " + bc + " -o " + obj;
    return exec(cmd);
}

bool Pipeline::compileRuntime(const std::string& clang, const std::string& outDir,
                              std::vector<std::string>& outObjs) {
    namespace fs = std::filesystem;

    // Find runtime directory relative to compiler source
    fs::path compilerSrc = fs::path(__FILE__).parent_path().parent_path();
    fs::path runtimeDir = compilerSrc / ".." / "src" / "runtime";
    fs::path includeDir = runtimeDir / "include";

    if (!fs::exists(runtimeDir)) {
        std::cerr << "[!] Runtime directory not found: " << runtimeDir << "\n";
        return false;
    }

    // System include paths for standard C headers
    std::vector<std::string> sysIncludes;
#ifdef _WIN32
    // Windows / MSVC paths
    sysIncludes = {
        "C:/Program Files/LLVM/lib/clang/17/include",
        "C:/Program Files/LLVM/lib/clang/18/include",
        "C:/Program Files/LLVM/lib/clang/19/include",
    };
    // Try to pick up MSVC paths from environment
    const char* vcinst = std::getenv("VCINSTALLDIR");
    if (vcinst) {
        sysIncludes.push_back(std::string(vcinst) + "/VC/Tools/MSVC/Current/Include");
    }
    const char* sdkDir = std::getenv("WindowsSdkDir");
    if (sdkDir) {
        sysIncludes.push_back(std::string(sdkDir) + "/Include/10.0.19041.0/ucrt");
    }
#else
    // Linux / Unix paths
    sysIncludes = {
        "/usr/include",
        "/usr/include/x86_64-linux-gnu",
        "/usr/lib/gcc/x86_64-linux-gnu/15/include",
        "/usr/lib/gcc/x86_64-linux-gnu/16/include",
        "/usr/lib/llvm-17/lib/clang/17/include",
        "/usr/lib/llvm-18/lib/clang/18/include",
        "/usr/lib/llvm-19/lib/clang/19/include",
    };
#endif
    std::string incFlags = "-I" + includeDir.string();
    for (const auto& inc : sysIncludes) {
        if (fs::exists(inc)) {
            incFlags += " -isystem " + inc;
        }
    }

    // Portable sources (always compiled)
    std::vector<fs::path> sources = {
        runtimeDir / "init" / "anti_analysis.c",
        runtimeDir / "cleanup" / "self_delete.c",
        runtimeDir / "cleanup" / "logs.c",
    };

    // Windows-only sources
#ifdef _WIN32
    sources.push_back(runtimeDir / "evasion" / "unhook.c");
    sources.push_back(runtimeDir / "evasion" / "syscalls.c");
    sources.push_back(runtimeDir / "execution" / "hollow.c");
#endif

    for (const auto& src : sources) {
        if (!fs::exists(src)) continue;

#ifdef _WIN32
        fs::path obj = fs::path(outDir) / (src.stem().string() + ".obj");
#else
        fs::path obj = fs::path(outDir) / (src.stem().string() + ".o");
#endif
        std::string cmd = clang + " -O2 -c " + incFlags + " " + src.string() + " -o " + obj.string();
        if (!exec(cmd)) {
            std::cerr << "[!] Failed to compile runtime: " << src << "\n";
            return false;
        }
        outObjs.push_back(obj.string());
    }

    return true;
}

bool Pipeline::linkExecutable(const CompileOptions& opts, const std::string& clang,
                              const std::string& obj, const std::vector<std::string>& runtimeObjs,
                              const std::string& exe) {
    std::string cmd = clang + " " + obj;
    for (const auto& ro : runtimeObjs) {
        cmd += " " + ro;
    }
    cmd += " -o " + exe;
#ifdef _WIN32
    cmd += " -lntdll";
#else
    cmd += " -ldl -lpthread";
#endif
    return exec(cmd);
}

bool Pipeline::run(const CompileOptions& opts) {
    std::string base = opts.inputFile;
    size_t dot = base.find_last_of('.');
    if (dot != std::string::npos) base = base.substr(0, dot);

    std::string llPath = base + ".ll";
    std::string bcPath = base + ".obf.bc";
#ifdef _WIN32
    std::string objPath = base + ".obj";
#else
    std::string objPath = base + ".o";
#endif
    std::string exePath = opts.outputFile;

    std::cout << "[*] Parsing and generating LLVM IR...\n";
    if (!emitLLVMIR(opts, llPath)) return false;

    std::cout << "[*] Running obfuscation passes (profile: " << opts.profile << ")...\n";
    if (!runObfuscation(opts, llPath, bcPath)) return false;

    std::cout << "[*] Compiling to object...\n";
    if (!compileToObject(opts, bcPath, objPath)) return false;

    std::cout << "[*] Compiling runtime library...\n";
    std::string clang, opt, plugin;
    if (!findToolchain(clang, opt, plugin)) return false;

    std::string outDir = base + ".build";
    std::filesystem::create_directories(outDir);

    std::vector<std::string> runtimeObjs;
    if (!compileRuntime(clang, outDir, runtimeObjs)) {
        std::cerr << "[!] Runtime compilation failed\n";
        return false;
    }

    std::cout << "[*] Linking executable...\n";
    if (!linkExecutable(opts, clang, objPath, runtimeObjs, exePath)) return false;

    std::cout << "[+] Build succeeded: " << exePath << "\n";

    if (opts.pack) {
        std::cout << "[*] Packing binary with UPX...\n";
        std::string upx;
#ifdef _WIN32
        upx = "upx.exe";
#else
        upx = "/tmp/upx";
        if (!std::filesystem::exists(upx)) {
            upx = "upx";
        }
#endif
        std::string cmd = upx + " --best " + exePath;
#ifdef _WIN32
        cmd += " >nul 2>&1";
#else
        cmd += " 2>/dev/null";
#endif
        if (!exec(cmd)) {
            std::cerr << "[!] Warning: UPX not found. Binary was NOT packed.\n";
            std::cerr << "    Install UPX or place the binary on PATH to enable packing.\n";
        }
    }

    if (!opts.keepIntermediates) {
        std::filesystem::remove_all(llPath);
        std::filesystem::remove_all(bcPath);
        std::filesystem::remove_all(objPath);
        std::filesystem::remove_all(outDir);
    }

    return true;
}

} // namespace jocky
