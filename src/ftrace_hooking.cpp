#include "ftrace_hooking.h"
#include <linux/kallsyms.h>
#include <linux/ftrace.h>

namespace kernel_evasion {

static std::map<std::string, struct ftrace_hook> active_hooks_;

ErrorCode FtraceHooking::register_ftrace_hook(const std::string& function_name,
                                              uint64_t hook_handler_address) {
    if (active_hooks_.count(function_name)) {
        return ErrorCode::HOOK_ALREADY_EXISTS;
    }

    struct ftrace_hook hook = {};
    hook.name = function_name.c_str();
    hook.function = (void *)hook_handler_address;

    int err = fh_install_hook(&hook);
    if (err) {
        return ErrorCode::HOOK_INSTALL_FAILED;
    }

    active_hooks_[function_name] = hook;
    return ErrorCode::SUCCESS;
}

ErrorCode FtraceHooking::unregister_ftrace_hook(const std::string& function_name) {
    if (!active_hooks_.count(function_name)) {
        return ErrorCode::HOOK_NOT_FOUND;
    }
    fh_remove_hook(&active_hooks_[function_name]);
    active_hooks_.erase(function_name);
    return ErrorCode::SUCCESS;
}

ErrorCode FtraceHooking::hook_syscall(const std::string& syscall_name,
                                     uint64_t hook_handler_address) {
    std::string func_name = "__x64_sys_" + syscall_name;
    return register_ftrace_hook(func_name, hook_handler_address);
}

ErrorCode FtraceHooking::unhook_syscall(const std::string& syscall_name) {
    std::string func_name = "__x64_sys_" + syscall_name;
    return unregister_ftrace_hook(func_name);
}

std::vector<std::string> FtraceHooking::list_active_hooks() {
    std::vector<std::string> hooks;
    for (const auto& [func, _] : active_hooks_) {
        hooks.push_back(func);
    }
    return hooks;
}

bool FtraceHooking::is_ftrace_available() {
    return true;
}

ErrorCode FtraceHooking::set_ftrace_filter(const std::string& filter) {
    return ErrorCode::SUCCESS;
}

uint64_t FtraceHooking::resolve_kallsyms_lookup_name() {
    return resolve_kernel_symbol("kallsyms_lookup_name");
}

bool FtraceHooking::supports_ftrace_direct() {
    return true;
}

}  // namespace kernel_evasion