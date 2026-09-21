#include "pipeline.h"
#include "lexer.h"
#include "parser.h"
#include "codegen.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
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
        // Try PATH
        const char* path = std::getenv("PATH");
        if (path) {
            std::stringstream ss(path);
            std::string dir;
            while (std::getline(ss, dir, ':')) {
                std::string clang = dir + "/clang";
                std::string opt = dir + "/opt";
                if (std::ifstream(clang).good() && std::ifstream(opt).good()) {
                    prefix = dir + "/..";
                    break;
                }
            }
        }
        if (prefix.empty()) {
            // Common fallback
            prefix = "/home/kamini/projects/llvm-obfuscation-tools-linux-x86_64";
        }
    }

    outClang = prefix + "/bin/clang";
    outOpt = prefix + "/bin/opt";
    outPlugin = prefix + "/lib/LLVMObfuscationPlugin.so";

    if (!std::ifstream(outClang).good()) {
        std::cerr << "[!] clang not found: " << outClang << "\n";
        return false;
    }
    if (!std::ifstream(outOpt).good()) {
        std::cerr << "[!] opt not found: " << outOpt << "\n";
        return false;
    }
    return true;
}

bool Pipeline::emitLLVMIR(const CompileOptions& opts, const std::string& llPath) {
    // Read source
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

    // Compile .ll to .bc with -O1
    std::string cmd1 = clang + " -O1 -c -emit-llvm -x ir " + inBc + " -o " + inBc + ".tmp.bc";
    if (!exec(cmd1)) {
        std::cerr << "[!] Failed to compile IR to bitcode\n";
        return false;
    }

    // Run obfuscation passes
    std::string passes = "function(boguscf,flattening,substitution,linear-mba,opaque-pred)";
    if (opts.profile == "none") passes = "";
    else if (opts.profile == "aggressive") passes = "strip-signature,function(boguscf,flattening,substitution,split,linear-mba,opaque-pred),anti-debug,indirect-call";

    if (!passes.empty()) {
        std::string cmd2 = opt + " -load-pass-plugin=" + plugin +
                           " -passes='" + passes + "' " +
                           inBc + ".tmp.bc -o " + outBc;
        if (!exec(cmd2)) {
            std::cerr << "[!] Obfuscation passes failed\n";
            return false;
        }
    } else {
        std::string cmd2 = "cp " + inBc + ".tmp.bc " + outBc;
        exec(cmd2);
    }

    // Cleanup temp
    std::string rm = "rm -f " + inBc + ".tmp.bc";
    exec(rm);
    return true;
}

bool Pipeline::compileToObject(const CompileOptions& opts, const std::string& bc, const std::string& obj) {
    std::string clang, opt, plugin;
    if (!findToolchain(clang, opt, plugin)) return false;

    std::string cmd = clang + " -c " + bc + " -o " + obj;
    return exec(cmd);
}

bool Pipeline::linkExecutable(const CompileOptions& opts, const std::string& obj, const std::string& exe) {
    std::string clang, opt, plugin;
    if (!findToolchain(clang, opt, plugin)) return false;

    std::string cmd = clang + " " + obj + " -o " + exe;
    return exec(cmd);
}

bool Pipeline::run(const CompileOptions& opts) {
    std::string base = opts.inputFile;
    size_t dot = base.find_last_of('.');
    if (dot != std::string::npos) base = base.substr(0, dot);

    std::string llPath = base + ".ll";
    std::string bcPath = base + ".obf.bc";
    std::string objPath = base + ".o";
    std::string exePath = opts.outputFile;

    std::cout << "[*] Parsing and generating LLVM IR...\n";
    if (!emitLLVMIR(opts, llPath)) return false;

    std::cout << "[*] Running obfuscation passes (profile: " << opts.profile << ")...\n";
    if (!runObfuscation(opts, llPath, bcPath)) return false;

    std::cout << "[*] Compiling to object...\n";
    if (!compileToObject(opts, bcPath, objPath)) return false;

    std::cout << "[*] Linking executable...\n";
    if (!linkExecutable(opts, objPath, exePath)) return false;

    std::cout << "[+] Build succeeded: " << exePath << "\n";

    if (!opts.keepIntermediates) {
        std::string rm = "rm -f " + llPath + " " + bcPath + " " + objPath;
        exec(rm);
    }

    return true;
}

} // namespace jocky
