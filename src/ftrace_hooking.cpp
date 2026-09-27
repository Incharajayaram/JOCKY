#include "ftrace_hooking.h"
#include <linux/kallsyms.h>
#include <linux/ftrace.h>
#include <memory>

namespace kernel_evasion {

// Store hooks with persistent storage for names
struct HookEntry {
    std::string name;
    struct ftrace_hook hook;
};

static std::map<std::string, HookEntry> active_hooks_;
static std::mutex hook_mutex_;

ErrorCode FtraceHooking::register_ftrace_hook(const std::string& function_name,
                                              uint64_t hook_handler_address) {
    std::lock_guard<std::mutex> lock(hook_mutex_);

    if (active_hooks_.count(function_name)) {
        return ErrorCode::HOOK_ALREADY_EXISTS;
    }

    HookEntry entry;
    entry.name = function_name;
    struct ftrace_hook hook = {};
    hook.name = entry.name.c_str();  // Now points to persistent storage in the map
    hook.function = (void *)hook_handler_address;
    entry.hook = hook;

    int err = fh_install_hook(&entry.hook);
    if (err) {
        return ErrorCode::HOOK_INSTALL_FAILED;
    }

    active_hooks_[function_name] = entry;
    // Update the pointer to point to the persistent storage in the map
    active_hooks_[function_name].hook.name = active_hooks_[function_name].name.c_str();

    return ErrorCode::SUCCESS;
}

ErrorCode FtraceHooking::unregister_ftrace_hook(const std::string& function_name) {
    std::lock_guard<std::mutex> lock(hook_mutex_);

    if (!active_hooks_.count(function_name)) {
        return ErrorCode::HOOK_NOT_FOUND;
    }

    // Call fh_remove_hook BEFORE erasing from map to avoid use-after-free
    struct ftrace_hook hook = active_hooks_[function_name].hook;
    fh_remove_hook(&hook);
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
    std::lock_guard<std::mutex> lock(hook_mutex_);
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