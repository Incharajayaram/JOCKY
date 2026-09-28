cmake_minimum_required(VERSION 3.10)
project(JOCKY_Runtime_Tests)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O2")

# Platform detection
if(WIN32)
    message(STATUS "Building for Windows")
    set(WINDOWS_BUILD ON)
else()
    message(STATUS "Building for Linux")
    set(LINUX_BUILD ON)
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/../../src/runtime/include)
include_directories(${CMAKE_SOURCE_DIR}/../../src/runtime)

# Find source files
set(RUNTIME_SOURCES
    ${CMAKE_SOURCE_DIR}/../../src/runtime/ai/mutation_engine.c
)

# Windows-specific sources
if(WINDOWS_BUILD)
    list(APPEND RUNTIME_SOURCES
        ${CMAKE_SOURCE_DIR}/../../src/runtime/windows/evasion/edrhoker.c
        ${CMAKE_SOURCE_DIR}/../../src/runtime/windows/evasion/blindside.c
        ${CMAKE_SOURCE_DIR}/../../src/runtime/windows/anti_forensics/forensics.c
        ${CMAKE_SOURCE_DIR}/../../src/runtime/windows/anti_forensics/logs.c
        ${CMAKE_SOURCE_DIR}/../../src/runtime/windows/anti_forensics/self_delete.c
        ${CMAKE_SOURCE_DIR}/../../src/runtime/windows/byovd/btr_abuse.c
        ${CMAKE_SOURCE_DIR}/../../src/runtime/windows/kernel/patchguard_peekaboo.c
        ${CMAKE_SOURCE_DIR}/../../src/runtime/windows/exfil/enhanced_exfiltration.c
    )
endif()

# Linux-specific sources
if(LINUX_BUILD)
    list(APPEND RUNTIME_SOURCES
        ${CMAKE_SOURCE_DIR}/../../src/runtime/linux/lpe/fence2pwn.c
        ${CMAKE_SOURCE_DIR}/../../src/runtime/windows/exfil/enhanced_exfiltration.c
    )
endif()

# Test executables
if(WINDOWS_BUILD)
    add_executable(test_edrhoker
        test_edrhoker.c
        ${CMAKE_SOURCE_DIR}/../../src/runtime/windows/evasion/edrhoker.c
    )

    add_executable(test_forensics
        test_forensics.c
        ${CMAKE_SOURCE_DIR}/../../src/runtime/windows/anti_forensics/forensics.c
        ${CMAKE_SOURCE_DIR}/../../src/runtime/windows/anti_forensics/logs.c
        ${CMAKE_SOURCE_DIR}/../../src/runtime/windows/anti_forensics/self_delete.c
    )

    add_executable(test_btr_abuse
        test_btr_abuse.c
        ${CMAKE_SOURCE_DIR}/../../src/runtime/windows/byovd/btr_abuse.c
    )

    add_executable(test_patchguard_peekaboo
        test_patchguard_peekaboo.c
        ${CMAKE_SOURCE_DIR}/../../src/runtime/windows/kernel/patchguard_peekaboo.c
    )

    # Windows-only tests
    target_link_libraries(test_edrhoker PRIVATE ws2_32 iphlpapi)
    target_link_libraries(test_forensics PRIVATE advapi32)
    target_link_libraries(test_btr_abuse PRIVATE setupapi)
endif()

if(LINUX_BUILD)
    add_executable(test_fence2pwn
        test_fence2pwn.c
        ${CMAKE_SOURCE_DIR}/../../src/runtime/linux/lpe/fence2pwn.c
    )
endif()

# Cross-platform test
add_executable(test_ai_evasion
    test_ai_evasion.c
    ${CMAKE_SOURCE_DIR}/../../src/runtime/ai/mutation_engine.c
)

# Add test for exfiltration
add_executable(test_exfiltration
    test_exfiltration.c
    ${CMAKE_SOURCE_DIR}/../../src/runtime/windows/exfil/enhanced_exfiltration.c
)

if(WIN32)
    target_link_libraries(test_exfiltration PRIVATE ws2_32)
endif()

# Enable testing
enable_testing()

# Register tests
if(WINDOWS_BUILD)
    add_test(NAME EDRChoker COMMAND test_edrhoker)
    add_test(NAME AntiForensics COMMAND test_forensics)
    add_test(NAME BTRReforged COMMAND test_btr_abuse)
    add_test(NAME PatchGuardPeekaboo COMMAND test_patchguard_peekaboo)
endif()

if(LINUX_BUILD)
    add_test(NAME Fence2Pwn COMMAND test_fence2pwn)
endif()

add_test(NAME AIEvasion COMMAND test_ai_evasion)
add_test(NAME Exfiltration COMMAND test_exfiltration)

# Summary
message(STATUS "")
message(STATUS "=== JOCKY Runtime Test Suite ===")
if(WINDOWS_BUILD)
    message(STATUS "Windows Tests:")
    message(STATUS "  - EDRChoker (QoS throttling)")
    message(STATUS "  - Anti-Forensics (16 test cases)")
    message(STATUS "  - BTR Reforged (BTR.sys abuse)")
    message(STATUS "  - PatchGuard Peekaboo (HVCI)")
endif()
if(LINUX_BUILD)
    message(STATUS "Linux Tests:")
    message(STATUS "  - Fence2Pwn (KFENCE LPE)")
endif()
message(STATUS "Cross-Platform Tests:")
message(STATUS "  - AI Evasion (15 test cases)")
message(STATUS "  - Exfiltration (multiple methods)")
message(STATUS "")
message(STATUS "Run tests with: cmake --build . && ctest --output-on-failure")
message(STATUS "")
