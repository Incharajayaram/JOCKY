#include "forensic/dynamic_analyzer.h"
#include <iostream>
#include <chrono>

#ifdef __linux__
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#endif

namespace jocky {
namespace forensic {

bool DynamicAnalyzer::analyze(const std::string& binaryPath, DynamicReport& out) {
#ifdef __linux__
    return analyzeLinux(binaryPath, out);
#elif defined(_WIN32)
    return analyzeWindows(binaryPath, out);
#else
    std::cerr << "[!] Dynamic analysis not supported on this platform\n";
    return false;
#endif
}

#ifdef __linux__
bool DynamicAnalyzer::analyzeLinux(const std::string& binaryPath, DynamicReport& out) {
    out.executedSuccessfully = false;
    out.detectedSandbox = false;

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "[!] fork() failed\n";
        return false;
    }

    if (pid == 0) {
        // Child process
        if (ptrace(PTRACE_TRACEME, 0, nullptr, nullptr) < 0) {
            std::cerr << "[!] ptrace(TRACEME) failed: " << strerror(errno) << "\n";
            _exit(1);
        }

        // Raise SIGSTOP so parent can set options
        raise(SIGSTOP);

        execl(binaryPath.c_str(), binaryPath.c_str(), nullptr);
        _exit(1);
    }

    // Parent process
    int status;
    auto startTime = std::chrono::high_resolution_clock::now();

    // Wait for initial SIGSTOP
    if (waitpid(pid, &status, 0) < 0) {
        std::cerr << "[!] waitpid failed\n";
        return false;
    }

    // Set ptrace options
    ptrace(PTRACE_SETOPTIONS, pid, nullptr,
           PTRACE_O_TRACESYSGOOD | PTRACE_O_TRACEEXEC | PTRACE_O_TRACEEXIT);

    // Set alarm for timeout (30 seconds)
    alarm(30);

    int syscallCount = 0;
    bool running = true;

    while (running) {
        // Continue execution until next syscall or signal
        if (ptrace(PTRACE_SYSCALL, pid, nullptr, nullptr) < 0) {
            break;
        }

        if (waitpid(pid, &status, 0) < 0) {
            break;
        }

        if (WIFEXITED(status)) {
            running = false;
            out.executedSuccessfully = true;
            break;
        }

        if (WIFSIGNALED(status)) {
            running = false;
            if (WTERMSIG(status) == SIGALRM) {
                std::cerr << "[!] Execution timed out\n";
            }
            break;
        }

        if (WIFSTOPPED(status)) {
            int sig = WSTOPSIG(status);
            if (sig == (SIGTRAP | 0x80)) {
                // Syscall entry/exit
                syscallCount++;

                struct user_regs_struct regs;
                ptrace(PTRACE_GETREGS, pid, nullptr, &regs);

                // On x86_64, syscall number is in rax
                long syscallNum = regs.orig_rax;

                // Track interesting syscalls
                switch (syscallNum) {
                    case 2:   // open
                    case 257: // openat
                        out.fileOperations.push_back("open/openat");
                        break;
                    case 59:  // execve
                        out.createdProcesses.push_back("execve");
                        break;
                    case 42:  // connect
                        out.networkConnections.push_back("connect");
                        break;
                    case 41:  // socket
                        out.networkConnections.push_back("socket");
                        break;
                }
            } else if (sig == SIGTRAP) {
                // Check for exec event
                int event = status >> 16;
                if (event == PTRACE_EVENT_EXEC) {
                    out.createdProcesses.push_back("exec");
                }
            }
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    out.executionTimeMs = std::chrono::duration<double, std::milli>(endTime - startTime).count();

    // Cleanup
    kill(pid, SIGKILL);
    waitpid(pid, &status, 0);

    std::cout << "[*] Dynamic analysis complete: " << syscallCount << " syscalls, "
              << out.executionTimeMs << " ms\n";

    return true;
}
#endif

#ifdef _WIN32
bool DynamicAnalyzer::analyzeWindows(const std::string& binaryPath, DynamicReport& out) {
    // Windows implementation would use CreateProcess with DEBUG_PROCESS
    // and WaitForDebugEvent loop
    std::cout << "[*] Windows dynamic analysis (stub implementation)\n";
    out.executedSuccessfully = false;
    return true; // Return true to not fail the pipeline, but mark as not executed
}
#endif

} // namespace forensic
} // namespace jocky
