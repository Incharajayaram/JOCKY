#ifndef FTRACE_HELPER_H
#define FTRACE_HELPER_H

#include <linux/ftrace.h>
#include <linux/kprobes.h>
#include <linux/version.h>

/* Kernel version gating for the ftrace recursion bug. */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 6, 0)
    #define USE_KPROBE_HOOKS 1
#else
    #define USE_FTRACE_HOOKS 1
#endif

struct ftrace_hook {
    const char *name;
    void *function;
    void *original;
    unsigned long address;
    struct ftrace_ops ops;
    struct kprobe kp;
};

#define HOOK(_name, _function, _original) \
    { .name = _name, .function = (void *)_function, .original = (void *)_original }

int fh_install_hook(struct ftrace_hook *hook);
void fh_remove_hook(struct ftrace_hook *hook);
int fh_install_hooks(struct ftrace_hook *hooks, size_t count);
void fh_remove_hooks(struct ftrace_hook *hooks, size_t count);

#endif