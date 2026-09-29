#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <random>

namespace jocky {

class TokenDiversifier {
public:
    TokenDiversifier();
    
    // Set a seed for reproducible builds (or random for unique)
    void setSeed(uint64_t seed);
    
    // Get polymorphic name for a given identifier
    std::string diversify(const std::string& original);
    
    // Check if a name should be diversified (skip FFI externals, main, etc.)
    bool shouldDiversify(const std::string& name);
    
    // Reset for new build
    void reset();
    
private:
    std::mt19937_64 rng;
    std::unordered_map<std::string, std::string> nameMap;
    
    // Generate random alphanumeric name
    std::string generateName();
    
    // Names that should never be diversified (linker-visible, runtime, etc.)
    static const std::unordered_set<std::string> reservedNames;
};

} // namespace jocky
