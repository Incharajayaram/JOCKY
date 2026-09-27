#include "common.h"
#include <linux/kprobes.h>
#include <linux/kallsyms.h>
#include <mutex>

namespace kernel_evasion {

// Kprobe-based resolution of kallsyms_lookup_name.
// Adapted from Diamorphine (m0nad/Diamorphine).
static struct kprobe kp = {
    .symbol_name = "kallsyms_lookup_name",
};

typedef unsigned long (*kallsyms_lookup_name_t)(const char *name);
static kallsyms_lookup_name_t lookup_fn = NULL;
static std::mutex symbol_mutex_;
static std::once_flag symbol_init_;

uint64_t resolve_kernel_symbol(const std::string& symbol) {
    // Use std::call_once to ensure thread-safe initialization
    std::call_once(symbol_init_, []() {
        if (register_kprobe(&kp) < 0) {
            lookup_fn = NULL;
            return;
        }
        lookup_fn = (kallsyms_lookup_name_t)kp.addr;
        unregister_kprobe(&kp);
    });

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