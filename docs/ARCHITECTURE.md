# Kernel Evasion Research Architecture

## Project Overview
This project implements advanced Linux kernel evasion techniques for security research funded by Red Hat and IIT Bombay Cyber Security Team. The goal is to create a sophisticated adversary emulator that challenges detection systems, enabling the defensive team to build stronger OS-level countermeasures.

## Components (Priority Order)

### 1. LKM Loading & Hooking Framework
**Goal**: Implement modern kernel module loading and ftrace-based hooking
- Symbol resolution via kprobe trick (post-5.7 kallsyms)
- ftrace registration for syscall interception
- kprobe fallback for direct function hooks
- Module auto-hiding from kernel module list

**Key Files**:
- `src/lkm_loader.cpp` - Module loading and management
- `src/ftrace_hooking.cpp` - ftrace callback infrastructure
- Kernel modules in `research/lkm/`

### 2. eBPF-Based Evasion
**Goal**: Implement eBPF manipulation techniques
- eBPF event stream filtering
- Netlink socket-diagnostic response manipulation (like VoidLink)
- bpf() syscall hooking to hide malicious eBPF programs
- Integration with ftrace hooks

**Key Files**:
- `src/ebpf_evasion.cpp` - eBPF program injection and hooking
- `research/ebpf/` - eBPF program source

### 3. Artifact Hiding
**Goal**: Hide processes, files, modules, and metadata
- getdents/getdents64 hooking to filter /proc listings
- Process metadata rewriting (disguise as kernel threads)
- File hiding via vfs operations
- Module removal from kernel lists

**Key Files**:
- `src/artifact_hiding.cpp` - Hiding mechanism orchestration
- Syscall hooks in kernel modules

### 4. Userland Evasion
**Goal**: Implement fileless execution and libc interception
- memfd_create + execveat for fileless execution
- LD_PRELOAD-based libc hooking
- Process hiding from userland tools (ps, top)

**Key Files**:
- `src/userland_evasion.cpp` - Userland agent management
- `research/preload/` - Malicious shared library

### 5. Privilege Escalation Research
**Goal**: Demonstrate modern LPE primitives
- CVE-2026-46300 (Fragnesia) - arbitrary page cache write
- CVE-2026-43284 (Dirty Frag) - CoW bypass
- Documentation only (reference implementations from public sources)

**Key Files**:
- `research/cves/` - Public CVE references and analysis

## Architecture Layers

```
┌─────────────────────────────────────┐
│    Orchestrator / Main Pipeline     │
└─────────────────────────────────────┘
           ↓
┌─────────────────────────────────────┐
│   Component Manager (LKM, eBPF)     │
├─────────────────────────────────────┤
│  LKM Loader  │  eBPF Manager  │ ... │
└─────────────────────────────────────┘
           ↓
┌─────────────────────────────────────┐
│    Kernel-Level Hooks & Filters     │
│    (ftrace, kprobe, eBPF)           │
└─────────────────────────────────────┘
```

## Implementation Strategy

1. **Phase 1**: Symbol resolution + basic ftrace hooking
2. **Phase 2**: Complete syscall interception for process hiding
3. **Phase 3**: eBPF event filtering integration
4. **Phase 4**: Userland agent and coordination
5. **Phase 5**: Advanced evasion (Netlink manipulation, metadata rewriting)

## Testing Strategy

- Unit tests for C++ components (userland)
- Integration tests on test VMs with different kernel versions
- Verification against detection tools (Falco, etc.)
- Documentation of detected vs. undetected behavior

## References & Research
See `research/PAPERS.md` for citations to academic work and public implementations.
