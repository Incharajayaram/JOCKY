#pragma once

#include "common.h"
#include <functional>
#include <map>

namespace kernel_evasion {

typedef uint64_t (*SyscallHandler)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

class FtraceHooking {
public:
    FtraceHooking();
    ~FtraceHooking();

    // Register ftrace hook on a function
    // Requires loading a kernel module that sets up the ftrace callback
    ErrorCode register_ftrace_hook(const std::string& function_name,
                                   uint64_t hook_handler_address);

    // Unregister ftrace hook
    ErrorCode unregister_ftrace_hook(const std::string& function_name);

    // Hook specific syscalls (__x64_sys_*)
    ErrorCode hook_syscall(const std::string& syscall_name,
                          uint64_t hook_handler_address);

    // Unhook syscall
    ErrorCode unhook_syscall(const std::string& syscall_name);

    // List active hooks
    std::vector<std::string> list_active_hooks();

    // Check if ftrace is available/enabled
    bool is_ftrace_available();

    // Enable/disable ftrace filtering
    ErrorCode set_ftrace_filter(const std::string& filter);

private:
    std::map<std::string, uint64_t> active_hooks_;

    // Kernel probe trick for resolving kallsyms_lookup_name (post-5.7)
    uint64_t resolve_kallsyms_lookup_name();

    // Check if kernel supports ftrace direct (CONFIG_FTRACE_DIRECT)
    bool supports_ftrace_direct();
};

}  // namespace kernel_evasion
