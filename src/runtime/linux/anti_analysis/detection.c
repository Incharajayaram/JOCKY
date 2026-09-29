/* Anti-analysis detection for Linux */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <errno.h>

int jocky_is_debugger_present(void) {
    /* Check if being traced by debugger using ptrace */
    if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) < 0) {
        return 1;  /* Being traced */
    }

    /* Detach ourselves */
    ptrace(PTRACE_DETACH, getpid(), NULL, NULL);
    return 0;  /* Not traced */
}

int jocky_is_sandbox(void) {
    /* Check for common sandbox indicators */

    /* Check for /proc/self/cgroup with sandbox markers */
    FILE* fp = fopen("/proc/self/cgroup", "r");
    if (fp) {
        char line[256];
        while (fgets(line, sizeof(line), fp)) {
            if (strstr(line, "docker") || strstr(line, "lxc") ||
                strstr(line, "containerd") || strstr(line, "podman")) {
                fclose(fp);
                return 1;
            }
        }
        fclose(fp);
    }

    /* Check for sandbox processes */
    FILE* pf = popen("ps aux 2>/dev/null | grep -i 'qemu\\|virtualbox\\|vmware\\|xen'", "r");
    if (pf) {
        char buf[256];
        if (fgets(buf, sizeof(buf), pf)) {
            pclose(pf);
            return 1;
        }
        pclose(pf);
    }

    return 0;
}

int jocky_is_vm(void) {
    /* Check for VM indicators */

    /* Check for VM in /proc/cpuinfo */
    FILE* fp = fopen("/proc/cpuinfo", "r");
    if (fp) {
        char line[256];
        while (fgets(line, sizeof(line), fp)) {
            if (strstr(line, "hypervisor") || strstr(line, "qemu") ||
                strstr(line, "KVM") || strstr(line, "Xen")) {
                fclose(fp);
                return 1;
            }
        }
        fclose(fp);
    }

    /* Check for VM devices */
    if (access("/dev/vda", F_OK) == 0 || access("/dev/hda", F_OK) == 0) {
        return 1;
    }

    return 0;
}

int jocky_check_analysis_environment(void) {
    /* Combined check for analysis environment */
    if (jocky_is_debugger_present()) return 1;
    if (jocky_is_sandbox()) return 1;
    if (jocky_is_vm()) return 1;

    return 0;
}
