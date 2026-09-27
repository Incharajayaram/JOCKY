# Research Papers & References

## Key Academic Papers

### 1. Rootkit Detection & Evasion
- **"Enabling Secure Kernel Integrity Monitoring"** (CVE-2022-46305, etc.)
  - Discusses modern ftrace protection mechanisms
  - References: linux-hardening mailing list archives
  
- **"The Art of Intrusion"** (Advanced Linux Rootkits)
  - Comprehensive coverage of kernel hooking techniques
  - Details on symbol resolution post-5.7

### 2. eBPF Security & Exploitation
- **"eBPF as a Tool for Defensive and Offensive Security"** (USENIX)
  - eBPF program filtering
  - Event stream manipulation
  
- **"VoidLink: Advanced eBPF Evasion"** (Security researchers)
  - Netlink socket-diagnostic response manipulation
  - bpf_probe_write_user() technique
  
### 3. Privilege Escalation
- **CVE-2026-46300 (Fragnesia)** - XFRM ESP-in-TCP arbitrary write
  - Affects page cache of read-only files
  - Public PoC available

- **CVE-2026-43284 (Dirty Frag)** - CoW bypass for page cache
  - Similar impact to Fragnesia
  - Alternative exploitation path

## Public Implementations to Study

### 1. Reptile Rootkit
- GitHub: `f0rb1dd3n/Reptile`
- Modern ftrace-based hooking
- Process hiding via getdents interception
- **Key techniques**: Symbol resolution, ftrace registration

### 2. DIAMORPHINE
- GitHub: `m0nad/Diamorphine`
- LKM-based process hiding
- Simple but effective approach
- **Key techniques**: Module auto-hiding, process filtering

### 3. Suterusu Rootkit
- GitHub: `mempodippy/Suterusu`
- eBPF-based evasion
- Network connection hiding
- **Key techniques**: eBPF filtering, syscall hooking

### 4. VoidLink (referenced papers)
- eBPF Netlink manipulation
- Process metadata rewriting
- Advanced detection evasion

## Kernel Source References

### ftrace Infrastructure
- `kernel/trace/ftrace.c` - ftrace implementation
- `kernel/trace/ftrace_internal.h` - Internal ftrace structures
- Key functions: `register_ftrace_direct()`, `ftrace_set_filter()`

### eBPF Infrastructure  
- `kernel/bpf/core.c` - eBPF runtime
- `kernel/bpf/syscall.c` - bpf() syscall
- Key hooks: `bpf_prog_run`, `BPF_PROG_SYSCALL_HOOKS`

### Syscall Hooks
- `fs/readdir.c` - getdents/getdents64 implementation
- `kernel/sys.c` - Process enumeration syscalls
- **Modern approach**: Use ftrace on `__x64_sys_getdents64` instead of direct table patching

## Build & Test Infrastructure

### Kernel Module Development
- Build against target kernel headers: `/lib/modules/$(uname -r)/build`
- Modern approach: Use kbuild Makefiles (see `research/lkm/Makefile.template`)
- Testing: Load with `insmod`, verify with `dmesg`, `lsmod`

### eBPF Program Development
- Use `libbpf` for program loading
- Reference: `libbpf/examples/` in kernel source
- Testing: Load with `bpftool`, verify with `bpftool prog list`

### Detection Evasion Testing
- **Falco**: eBPF-based security monitoring
- **auditd**: Kernel audit framework
- **strace**: Process syscall tracing
- **Procmon**: Process event monitoring

## Research Approach

1. **Week 1-2**: Study kernel ftrace infrastructure + symbol resolution
2. **Week 3-4**: Implement LKM loader + basic ftrace hooking
3. **Week 5-6**: Syscall interception + process hiding
4. **Week 7-8**: eBPF filtering + advanced evasion
5. **Week 9-10**: Testing + documentation

## Notes for Implementation

- **Post-5.7 kernels**: No kallsyms_lookup_name export → use kprobe trick
- **ftrace optimization**: Use CONFIG_FTRACE_DIRECT for minimal overhead
- **eBPF constraints**: 4KB stack, limited loops, verifier restrictions
- **Testing**: Always test on multiple kernel versions (5.10, 5.15, 6.0, 6.5+)

## Ethical & Legal Notes

This research is conducted under:
- **Authorization**: Red Hat + IIT Bombay Cyber Security Team
- **Purpose**: Adversarial security research (red team)
- **Use**: Validation of detection systems (blue team)
- **Scope**: Closed research environment
