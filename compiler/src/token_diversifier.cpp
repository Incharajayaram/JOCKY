#include "token_diversifier.h"
#include <sstream>
#include <iomanip>

namespace jocky {

const std::unordered_set<std::string> TokenDiversifier::reservedNames = {
    // C runtime / libc
    "main", "printf", "puts", "sprintf", "fprintf",
    "malloc", "free", "calloc", "realloc",
    "memset", "memcpy", "memmove", "strlen", "strcmp", "strcpy",
    "fopen", "fclose", "fread", "fwrite", "fseek", "ftell",
    "exit", "abort", "assert",
    
    // JOCKY runtime
    "jocky_runtime_init",
    "jocky_collect_system_info",
    "jocky_enum_processes", 
    "jocky_enum_network",
    "jocky_check_persistence",
    "jocky_encrypt_output",
    
    // Windows API (common)
    "CreateProcess", "CreateThread", "VirtualAlloc", "VirtualProtect",
    "WriteProcessMemory", "ReadProcessMemory", "CreateRemoteThread",
    "LoadLibrary", "GetProcAddress", "GetModuleHandle",
    "NtAllocateVirtualMemory", "NtProtectVirtualMemory", "NtCreateThread",
    
    // LLVM / linker internals
    "llvm", "_start", "_init", "_fini",
};

TokenDiversifier::TokenDiversifier() {
    std::random_device rd;
    rng.seed(rd());
}

void TokenDiversifier::setSeed(uint64_t seed) {
    rng.seed(seed);
    nameMap.clear();
}

void TokenDiversifier::reset() {
    nameMap.clear();
    std::random_device rd;
    rng.seed(rd());
}

bool TokenDiversifier::shouldDiversify(const std::string& name) {
    // Never diversify reserved names
    if (reservedNames.count(name)) return false;
    
    // Don't diversify names starting with underscore (compiler internals)
    if (!name.empty() && name[0] == '_') return false;
    
    // Don't diversify LLVM intrinsic names
    if (name.find("llvm.") == 0) return false;
    
    // Don't diversify names that look like labels
    if (name.find(".") != std::string::npos) return false;
    
    return true;
}

std::string TokenDiversifier::diversify(const std::string& original) {
    // Don't diversify reserved names
    if (!shouldDiversify(original)) {
        return original;
    }
    
    // Check if we already generated a name for this
    auto it = nameMap.find(original);
    if (it != nameMap.end()) {
        return it->second;
    }
    
    // Generate new random name
    std::string newName = generateName();
    nameMap[original] = newName;
    return newName;
}

std::string TokenDiversifier::generateName() {
    // Generate random alphanumeric name: prefix + 8 hex chars
    // Prefix alternates between common compiler patterns to blend in
    static const char* prefixes[] = {
        "func_", "var_", "tmp_", "loc_", "arg_", "lbl_",
        "sub_", "ptr_", "arr_", "buf_", "val_", "ref_",
        // Also use some ambiguous names that look like optimized code
        "f", "v", "t", "l", "a", "p", "b", "n", "x", "y", "z",
    };
    
    std::uniform_int_distribution<size_t> prefixDist(0, sizeof(prefixes)/sizeof(prefixes[0]) - 1);
    std::uniform_int_distribution<uint64_t> hexDist(0, 0xFFFFFFFF);
    
    std::string prefix = prefixes[prefixDist(rng)];
    
    std::stringstream ss;
    ss << prefix << std::hex << std::setfill('0');
    
    // For single-letter prefixes, add more randomness
    if (prefix.length() == 1) {
        ss << std::setw(12) << hexDist(rng);
    } else {
        ss << std::setw(8) << hexDist(rng);
    }
    
    return ss.str();
}

} // namespace jocky
