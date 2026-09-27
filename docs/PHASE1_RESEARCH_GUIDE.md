# Phase 1: Research & Study Guide (Week 1-2)

## Objective
Understand modern Linux kernel hooking techniques, ftrace infrastructure, and symbol resolution mechanisms for post-5.7 kernels.

---

## 1. Kernel Symbol Resolution (Post-5.7)

### The Problem
- In kernels 5.7+, `kallsyms_lookup_name` is no longer exported
- Rootkits can't directly call `kallsyms_lookup_name("function_name")` to get function addresses
- Ftrace hooking requires knowing the exact kernel function addresses

### The Solution: The Kprobe Trick

#### How It Works
```
1. Create a kprobe on a known exported symbol (e.g., do_sys_openat2)
2. Read /sys/kernel/debug/kprobes/list to get the kprobe's handler address
3. Disassemble kernel memory around the handler address
4. Find references to kallsyms_lookup_name within kernel code
5. Use pattern matching to locate and call kallsyms_lookup_name
```

#### Step-by-Step Implementation

**Step 1: Register a kprobe**
```c
// In kernel module
#include <linux/kprobes.h>

static struct kprobe kp = {
    .symbol_name = "do_sys_openat2",
};

int init_module(void) {
    if (register_kprobe(&kp) < 0) {
        return -1;
    }
    // kp.addr now contains the address of do_sys_openat2
    return 0;
}
```

**Step 2: Read /sys/kernel/debug/kprobes/list**
```bash
cat /sys/kernel/debug/kprobes/list
# Output: p:do_sys_openat2+0/0x0 do_sys_openat2 [kprobes_example]
# The address is after "do_sys_openat2"
```

**Step 3: Use the discovered address**
```c
typedef unsigned long (*kallsyms_lookup_name_t)(const char *name);
kallsyms_lookup_name_t kallsyms_lookup_name_func;

// Discover address at module load time
// Search kernel memory for patterns to find kallsyms_lookup_name
```

#### Why This Works
- `do_sys_openat2` and other core functions are still exported
- The kprobe mechanism allows discovering their actual kernel addresses
- From there, we can search for `kallsyms_lookup_name` in kernel code

#### Detection Point
- Monitoring: Check for unusual kprobe registrations
- Detection: Watch for kprobes on common functions (do_sys_openat2, etc.)
- Countermeasure: Restrict kprobe registration to root with audit logging

**References:**
- Reptile rootkit implementation
- Linux kernel source: `kernel/kprobes.c`
- Blog posts: "Resolving Kallsyms Without Kallsyms"

---

## 2. Ftrace Infrastructure (5.7+)

### Overview
Ftrace (function tracer) is a kernel tracer that can intercept function calls with minimal overhead.

### Key Concepts

#### ftrace_ops (Traditional Approach)
```c
static struct ftrace_ops ops = {
    .func = ftrace_callback,
    .flags = FTRACE_OPS_FL_SAVE_REGS,
};

// Register: register_ftrace_function(&ops)
// Unregister: unregister_ftrace_function(&ops)
```

**How it works:**
1. When ftrace is enabled on a function, ftrace replaces the function prologue with `nop` instructions
2. When ftrace_ops is registered, it enables callbacks on all registered functions
3. Your callback is invoked before the original function runs
4. You can examine/modify registers, jump to a different address, etc.

#### ftrace_direct (Modern Approach - 5.5+)
```c
extern asmlinkage long direct_my_syscall(unsigned long a, unsigned long b, ...);

// Register: register_ftrace_direct(func_addr, direct_func_addr)
// This is MUCH faster - direct jump, no callback overhead
```

**Advantages:**
- Direct function replacement - no callback overhead
- Faster than ftrace_ops approach
- Cleaner for syscall hooking

**Limitations:**
- Requires CONFIG_FTRACE_DIRECT
- Limited on older kernels (5.5+)
- Must be careful with argument passing (x86_64 ABI)

### Modern Ftrace + Syscall Hooking

#### Syscall Entry Points (x86_64)
```
System call entry points:
- __x64_sys_read        -> read(fd, buf, count)
- __x64_sys_write       -> write(fd, buf, count)
- __x64_sys_openat      -> openat(dirfd, pathname, flags, mode)
- __x64_sys_getdents64  -> getdents64(fd, dirent, count)
- __x64_sys_stat        -> stat(pathname, statbuf)
```

These are the actual kernel functions called by the syscall table.

#### Hooking getdents64 (Process Hiding)
```c
// Hook __x64_sys_getdents64 to filter /proc entries
// When process calls getdents64("/proc"), intercept and return filtered list
// Remove entries for PIDs we want to hide
```

### Detection Points
- Check `/sys/kernel/debug/tracing/enabled_functions` for unexpected hooks
- Monitor `/sys/kernel/debug/tracing/set_ftrace_filter` changes
- Check loaded ftrace_ops structures
- Monitor register_ftrace_direct() calls
- Look for writable kernel module memory containing hook code

**References:**
- Linux kernel: `kernel/trace/ftrace.c`
- ftrace documentation: `/Documentation/trace/ftrace.rst`
- Steven Rostedt's ftrace talks (LPC, LinuxCon)

---

## 3. Loadable Kernel Modules (LKM)

### Module Loading Process

```bash
# 1. Kernel loads module into memory
insmod /path/to/module.ko

# 2. Module's init_module() is called
# 3. Module registers handlers, hooks, etc.
# 4. Module appears in /proc/modules and lsmod
```

### Key Structures

```c
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Red Team");
MODULE_DESCRIPTION("Kernel evasion module");

static int __init init_module(void) {
    // Called when module is loaded
    // Register ftrace hooks, kprobes, etc.
    return 0;
}

static void __exit cleanup_module(void) {
    // Called when module is unloaded
    // Cleanup hooks, kprobes, etc.
}

module_init(init_module);
module_exit(cleanup_module);
```

### Module Auto-Hiding
```c
// In init_module():
list_del_init(&THIS_MODULE->list);  // Remove from module list
// Now lsmod and /proc/modules won't show the module
```

### Detection Points
- Missing `cleanup_module` symbols (indicates module hid itself)
- Inconsistencies between /sys/module/ and /proc/modules
- Unknown memory ranges (find with /proc/iomem or /proc/vmap)
- Audit logs for insmod/rmmod calls
- Watching for modules that unregister in cleanup_module

**References:**
- Linux kernel module development guide: `/Documentation/kbuild/modules.rst`
- Kernel Module Programming: `https://tldp.org/LDP/lkm/2.4/html/`

---

## 4. Syscall Hooking via getdents64

### How Process Hiding Works

#### Normal Flow
```
User program calls: opendir("/proc")
  -> libc: open("/proc", O_RDONLY | O_DIRECTORY)
    -> kernel: __x64_sys_openat()
  -> libc: readdir()
    -> kernel: getdents64()
      -> Returns buffer with all directory entries
  -> libc: Parses buffer, returns dirent structure
```

#### With Hooking
```
User program calls: opendir("/proc")
  -> kernel: __x64_sys_openat() - NOT hooked, normal
  -> kernel: getdents64() - HOOKED
    -> Original syscall executes
    -> FTRACE CALLBACK intercepts return
    -> Callback filters buffer: remove entries for hidden PIDs
    -> Return modified buffer to user
```

### Implementation Strategy

```c
// In kernel module:

// 1. Define callback to handle getdents64 returns
long hook_getdents64(unsigned long a, unsigned long b, ...) {
    // Call original getdents64
    long ret = original_getdents64(a, b, ...);
    
    if (ret > 0) {
        // ret is bytes written to user buffer
        // We need to modify the buffer in-kernel before returning
        // Filter out hidden process entries
    }
    
    return ret;
}

// 2. In init_module:
// Register the hook
register_ftrace_direct(__x64_sys_getdents64, hook_getdents64);
```

### Challenges
- **Buffer management:** getdents64 writes directly to user memory
- **Entry parsing:** Directory entries have variable length, must parse correctly
- **inode numbers:** Must also hide by inode if process has been re-parented
- **Performance:** Must not slow down normal directory listings

### Detection Points
- Syscall tracing (strace) shows getdents64 being called but entries missing
- Audit logs can track syscall interception
- Comparing /proc with internal kernel process tables (via /proc/sched_debug)
- eBPF monitoring of syscall entry/exit
- Direct inspection of kernel memory for hook code

**References:**
- Linux kernel: `fs/readdir.c`
- getdents64 man page and syscall ABI
- Reptile rootkit: `reptile/source/getdents/getdents.h`

---

## 5. Public Rootkit Implementations to Study

### Reptile (Modern Ftrace-Based)
**Repository:** `f0rb1dd3n/Reptile`
**Key Techniques:**
- ftrace-based syscall hooking
- getdents64 filtering for process hiding
- Network connection hiding
- Sudo privilege preservation

**Study Focus:**
- How it registers ftrace hooks
- Symbol resolution approach
- getdents64 buffer manipulation
- Memory management in kernel hooks

**Files to Study:**
- `source/config.c` - Configuration loading
- `source/plugins/syscall/`  - Syscall hooking
- `source/plugins/getdents/` - Process hiding
- `kernel/hiding.h` - Main hiding logic

### Diamorphine (Simpler LKM)
**Repository:** `m0nad/Diamorphine`
**Key Techniques:**
- Simple process hiding via module parameter
- Module self-hiding
- Syscall hooking (older method, may use sys_call_table)

**Study Focus:**
- Minimal, clean implementation
- How to structure kernel module
- Module parameter passing
- Basic syscall hooking

### Suterusu (eBPF-Based)
**Repository:** `mempodippy/Suterusu`
**Key Techniques:**
- eBPF-based system call hooking
- Process hiding via eBPF
- Root privilege escalation

**Study Focus:**
- eBPF program structure
- How to load/attach eBPF programs
- Event filtering at kernel level

### VoidLink (eBPF + Netlink)
**Description:** Advanced eBPF evasion
**Key Techniques:**
- eBPF event filtering (hide from Falco)
- Netlink socket-diagnostic manipulation
- bpf() syscall hooking

**Study Focus:**
- How Netlink responses are generated
- Manipulating socket diagnostic data
- Hiding eBPF programs from bpftool

---

## 6. Study Schedule

### Week 1: Foundation
- **Day 1-2:** Read about ftrace infrastructure (kernel docs + blogs)
- **Day 2-3:** Study kprobe trick for symbol resolution
- **Day 3-4:** Review Linux module development basics
- **Day 4-5:** Understand getdents64 syscall mechanics
- **Day 5-7:** Study Reptile source code (focus on hooking mechanism)

### Week 2: Deep Dive
- **Day 8-9:** Study Diamorphine (clean reference implementation)
- **Day 9-10:** Understand syscall ABI and argument passing (x86_64)
- **Day 10-12:** Study buffer management in kernel (kmalloc, copy_to_user, etc.)
- **Day 12-14:** Research detection methods (what defensive team should watch for)

---

## 7. Key Learning Resources

### Official Documentation
- Linux kernel source: `https://github.com/torvalds/linux`
- ftrace docs: `kernel/Documentation/trace/ftrace.rst`
- Module docs: `kernel/Documentation/kbuild/modules.rst`

### Blogs & Papers
- Steven Rostedt (ftrace maintainer) talks and blogs
- kernel.org documentation on syscalls and kernel hooking
- Security research papers on rootkit detection

### Code References
- Reptile: `https://github.com/f0rb1dd3n/Reptile`
- Diamorphine: `https://github.com/m0nad/Diamorphine`
- Suterusu: `https://github.com/mempodippy/Suterusu`

### Tools
- `objdump` - disassemble kernel code
- `readelf` - read ELF sections
- `/proc/kallsyms` - kernel symbol table
- `/sys/kernel/debug/tracing/` - ftrace interface
- `gdb` - kernel debugging
- `perf` - performance profiling (includes ftrace)

---

## 8. Critical Implementation Notes

### Security Best Practices
1. Always compile against target kernel headers
2. Use appropriate memory allocation (`kmalloc`, `vmalloc`)
3. Protect shared data with spinlocks or mutexes
4. Use `copy_to_user()` / `copy_from_user()` for user space interaction
5. Always implement proper cleanup in exit function
6. Test on non-production systems first

### Common Pitfalls
- Assuming kernel function signatures without checking kernel version
- Not handling memory alignment issues
- Forgetting to disable preemption when necessary
- Buffer overflows in kernel space (exploitation vectors)
- Not testing on target kernel versions
- Leaving debug printk() statements that leak module presence

### Performance Considerations
- ftrace_direct is faster than ftrace_ops
- Minimize time spent in hook handlers
- Avoid allocations in frequently-called hooks
- Use per-CPU data structures where possible

---

## Next Phase (Week 3-4)

Once research is complete:
1. Implement `lkm_loader.cpp` - Actual module loading
2. Implement symbol resolution using kprobe trick
3. Create first kernel module with ftrace hook registration
4. Test basic syscall hooking
5. Implement process hiding via getdents64

