#include "lkm_loader.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <sys/utsname.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <regex>

namespace kernel_evasion {

// Helper function to validate module path/name (prevent command injection)
static bool is_valid_module_name(const std::string& name) {
    static const std::regex valid_pattern("^[a-zA-Z0-9_-]+(\\.ko)?$");
    return std::regex_match(name, valid_pattern);
}

// Helper function to validate file path (basic checks)
static bool is_valid_file_path(const std::string& path) {
    if (path.empty() || path.length() > 4096) return false;
    if (path.find("..") != std::string::npos) return false;
    if (path.find(";") != std::string::npos) return false;
    if (path.find("|") != std::string::npos) return false;
    if (path.find("&") != std::string::npos) return false;
    return true;
}

LKMLoader::LKMLoader() {}
LKMLoader::~LKMLoader() {}

Result<int> LKMLoader::load_module(const std::string& module_path,
                                   const std::string& module_name,
                                   const std::vector<std::string>& params) {
    if (!is_root()) {
        return Result<int>(ErrorCode::INSUFFICIENT_PRIVILEGES);
    }

    // Validate inputs to prevent command injection
    if (!is_valid_file_path(module_path) || !is_valid_module_name(module_name)) {
        return Result<int>(ErrorCode::INVALID_ARGUMENT);
    }

    ErrorCode compat = validate_module_compatibility(module_path);
    if (compat != ErrorCode::SUCCESS) {
        return Result<int>(compat);
    }

    // Use execve instead of system() to avoid shell interpretation
    // For now, use safer argument passing with proper escaping
    std::vector<const char*> argv;
    argv.push_back("insmod");
    argv.push_back(module_path.c_str());

    std::vector<std::string> param_copies;
    for (const auto& param : params) {
        if (param.find(";") != std::string::npos ||
            param.find("|") != std::string::npos ||
            param.find("&") != std::string::npos) {
            return Result<int>(ErrorCode::INVALID_ARGUMENT);
        }
        param_copies.push_back(param);
        argv.push_back(param_copies.back().c_str());
    }
    argv.push_back(nullptr);

    pid_t pid = fork();
    if (pid == -1) {
        return Result<int>(ErrorCode::MODULE_LOAD_FAILED);
    }

    if (pid == 0) {
        // Child process
        execvp(argv[0], const_cast<char* const*>(argv.data()));
        _exit(1);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            loaded_modules_[module_name] = 0;
            return Result<int>(0);
        }
    }

    return Result<int>(ErrorCode::MODULE_LOAD_FAILED);
}

ErrorCode LKMLoader::unload_module(const std::string& module_name) {
    if (!is_root()) {
        return ErrorCode::INSUFFICIENT_PRIVILEGES;
    }

    // Validate module name to prevent command injection
    if (!is_valid_module_name(module_name)) {
        return ErrorCode::INVALID_ARGUMENT;
    }

    pid_t pid = fork();
    if (pid == -1) {
        return ErrorCode::MODULE_LOAD_FAILED;
    }

    if (pid == 0) {
        // Child process - use execvp to avoid shell
        const char* argv[] = {"rmmod", module_name.c_str(), nullptr};
        execvp(argv[0], const_cast<char* const*>(argv));
        _exit(1);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            loaded_modules_.erase(module_name);
            return ErrorCode::SUCCESS;
        }
    }

    return ErrorCode::MODULE_LOAD_FAILED;
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
    // Validate path to prevent command injection
    if (!is_valid_file_path(module_path)) {
        return ErrorCode::INVALID_ARGUMENT;
    }

    pid_t pid = fork();
    if (pid == -1) {
        return ErrorCode::MODULE_LOAD_FAILED;
    }

    if (pid == 0) {
        // Child process
        const char* argv[] = {"modinfo", "-F", "vermagic", module_path.c_str(), nullptr};
        execvp(argv[0], const_cast<char* const*>(argv));
        _exit(1);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            return ErrorCode::SUCCESS;
        }
    }

    return ErrorCode::KERNEL_VERSION_UNSUPPORTED;
}

}  // namespace kernel_evasion