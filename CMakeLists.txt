cmake_minimum_required(VERSION 3.16)
project(kernel-evasion C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

# Kernel module building
set(KERNEL_DIR "/lib/modules/$(uname -r)/build" CACHE STRING "Kernel build directory")

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/lkm_loader.cpp
    src/ftrace_hooking.cpp
    src/ebpf_evasion.cpp
    src/artifact_hiding.cpp
    src/userland_evasion.cpp
    src/privilege_escalation.cpp
)

# Main library
add_library(kernel_evasion STATIC ${SOURCES})

# Tests
enable_testing()
add_subdirectory(tests)

# Kernel modules (built separately with make)
# See research/ for kernel module source files
