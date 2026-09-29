// forensics_linux.c - Basic Linux forensic collection tool
// Compiles through JOCKY pipeline for obfuscated deployment

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

// Simple XOR encryption for output strings
static void xor_encrypt(char* data, size_t len, char key) {
    for (size_t i = 0; i < len; i++) {
        data[i] ^= key;
    }
}

static void print_encrypted(const char* msg, char key) {
    size_t len = strlen(msg);
    char* buf = malloc(len + 1);
    strcpy(buf, msg);
    xor_encrypt(buf, len, key);
    xor_encrypt(buf, len, key); // XOR twice = original
    printf("%s", buf);
    free(buf);
}

// Enumerate running processes via /proc
static int enum_processes() {
    DIR* dir = opendir("/proc");
    if (!dir) return -1;

    print_encrypted("=== PROCESS ENUMERATION ===\n", 0x42);
    
    struct dirent* entry;
    int count = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            char* endptr;
            long pid = strtol(entry->d_name, &endptr, 10);
            if (*endptr == '\0' && pid > 0) {
                char cmdline[256];
                snprintf(cmdline, sizeof(cmdline), "/proc/%ld/cmdline", pid);
                FILE* f = fopen(cmdline, "r");
                if (f) {
                    char name[256] = {0};
                    size_t n = fread(name, 1, sizeof(name)-1, f);
                    fclose(f);
                    if (n > 0) {
                        // Replace nulls with spaces
                        for (size_t i = 0; i < n-1; i++) {
                            if (name[i] == '\0') name[i] = ' ';
                        }
                        printf("[%ld] %s\n", pid, name);
                        count++;
                    }
                }
            }
        }
    }
    closedir(dir);
    
    printf("Total processes: %d\n", count);
    return count;
}

// Enumerate network connections via /proc/net/tcp
static int enum_network() {
    FILE* f = fopen("/proc/net/tcp", "r");
    if (!f) return -1;

    print_encrypted("\n=== NETWORK CONNECTIONS ===\n", 0x42);
    
    char line[512];
    // Skip header
    fgets(line, sizeof(line), f);
    
    int count = 0;
    while (fgets(line, sizeof(line), f)) {
        unsigned int local_addr, rem_addr;
        unsigned int local_port, rem_port;
        unsigned int state;
        int uid;
        unsigned int inode;
        
        if (sscanf(line, "%*d: %08X:%04X %08X:%04X %02X %*08X:%*08X %*02X:%*08X %*08X %d %*d %u",
                   &local_addr, &local_port, &rem_addr, &rem_port, &state, &uid, &inode) >= 5) {
            struct in_addr local, remote;
            local.s_addr = local_addr;
            remote.s_addr = rem_addr;
            
            const char* states[] = {"", "ESTABLISHED", "SYN_SENT", "SYN_RECV", "FIN_WAIT1",
                                   "FIN_WAIT2", "TIME_WAIT", "CLOSE", "CLOSE_WAIT", "LAST_ACK",
                                   "LISTEN", "CLOSING"};
            const char* state_str = (state < 12) ? states[state] : "UNKNOWN";
            
            printf("%s:%d -> %s:%d [%s] (uid=%d)\n",
                   inet_ntoa(local), local_port,
                   inet_ntoa(remote), rem_port,
                   state_str, uid);
            count++;
        }
    }
    fclose(f);
    
    printf("Total connections: %d\n", count);
    return count;
}

// Check for suspicious files in common locations
static int check_suspicious_files() {
    print_encrypted("\n=== SUSPICIOUS FILE CHECKS ===\n", 0x42);
    
    const char* suspicious_paths[] = {
        "/tmp/.X11-unix",
        "/dev/shm",
        "/var/tmp",
        "/etc/cron.d",
        "/etc/systemd/system",
        NULL
    };
    
    int count = 0;
    for (int i = 0; suspicious_paths[i]; i++) {
        DIR* dir = opendir(suspicious_paths[i]);
        if (dir) {
            struct dirent* entry;
            int files = 0;
            while ((entry = readdir(dir)) != NULL) {
                if (entry->d_name[0] != '.') {
                    files++;
                }
            }
            closedir(dir);
            printf("%s: %d entries\n", suspicious_paths[i], files);
            count += files;
        }
    }
    
    // Check for hidden processes (difference between /proc and ps)
    print_encrypted("\n=== HIDDEN PROCESS CHECK ===\n", 0x42);
    
    FILE* ps = popen("ps aux | wc -l", "r");
    int ps_count = 0;
    if (ps) {
        fscanf(ps, "%d", &ps_count);
        pclose(ps);
    }
    
    DIR* proc = opendir("/proc");
    int proc_count = 0;
    if (proc) {
        struct dirent* entry;
        while ((entry = readdir(proc)) != NULL) {
            if (entry->d_type == DT_DIR) {
                char* endptr;
                long pid = strtol(entry->d_name, &endptr, 10);
                if (*endptr == '\0' && pid > 0) {
                    proc_count++;
                }
            }
        }
        closedir(proc);
    }
    
    printf("PS count: %d, /proc count: %d\n", ps_count - 1, proc_count); // -1 for header
    if (proc_count != ps_count - 1) {
        printf("WARNING: Process count mismatch! Possible hidden processes.\n");
    }
    
    return count;
}

// Collect system info
static void collect_system_info() {
    print_encrypted("\n=== SYSTEM INFORMATION ===\n", 0x42);
    
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        printf("Hostname: %s\n", hostname);
    }
    
    FILE* f = fopen("/proc/version", "r");
    if (f) {
        char version[256];
        if (fgets(version, sizeof(version), f)) {
            printf("Kernel: %s", version);
        }
        fclose(f);
    }
    
    f = fopen("/proc/uptime", "r");
    if (f) {
        double uptime;
        if (fscanf(f, "%lf", &uptime) == 1) {
            printf("Uptime: %.2f seconds (%.2f hours)\n", uptime, uptime / 3600.0);
        }
        fclose(f);
    }
    
    long pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    printf("Total RAM: %ld MB\n", (pages * page_size) / (1024 * 1024));
    
    long cpus = sysconf(_SC_NPROCESSORS_ONLN);
    printf("CPU cores: %ld\n", cpus);
}

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    
    printf("JOCKY Forensic Collection Tool\n");
    printf("==============================\n\n");
    
    collect_system_info();
    enum_processes();
    enum_network();
    check_suspicious_files();
    
    printf("\n==============================\n");
    printf("Collection complete.\n");
    
    return 0;
}
