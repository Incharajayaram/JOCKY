// jocky_forensics_rt.c - JOCKY Forensic Runtime
// Implements the FFI functions called from .jky scripts
// This code is compiled and linked with the JOCKY output

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>

// XOR obfuscation for strings - decrypted at runtime
static void decrypt_string(char* str, size_t len, char key) {
    for (size_t i = 0; i < len; i++) {
        str[i] ^= key;
    }
}

// System information collection
int jocky_collect_system_info() {
    struct utsname uts;
    struct sysinfo si;
    
    if (uname(&uts) == 0) {
        printf("Hostname: %s\n", uts.nodename);
        printf("OS: %s %s\n", uts.sysname, uts.release);
        printf("Architecture: %s\n", uts.machine);
    }
    
    if (sysinfo(&si) == 0) {
        printf("Uptime: %ld seconds\n", si.uptime);
        printf("Total RAM: %lu MB\n", si.totalram / (1024 * 1024));
        printf("Free RAM: %lu MB\n", si.freeram / (1024 * 1024));
        printf("Processes: %u\n", si.procs);
    }
    
    long cpus = sysconf(_SC_NPROCESSORS_ONLN);
    printf("CPU cores: %ld\n", cpus);
    
    return 0;
}

// Process enumeration with injection detection
int jocky_enum_processes() {
    DIR* dir = opendir("/proc");
    if (!dir) {
        printf("Failed to open /proc\n");
        return -1;
    }
    
    int count = 0;
    int suspicious = 0;
    struct dirent* entry;
    
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            char* endptr;
            long pid = strtol(entry->d_name, &endptr, 10);
            if (*endptr == '\0' && pid > 0) {
                char path[256];
                snprintf(path, sizeof(path), "/proc/%ld/cmdline", pid);
                
                FILE* f = fopen(path, "r");
                if (f) {
                    char cmdline[256] = {0};
                    size_t n = fread(cmdline, 1, sizeof(cmdline)-1, f);
                    fclose(f);
                    
                    if (n > 0) {
                        // Replace nulls with spaces
                        for (size_t i = 0; i < n-1; i++) {
                            if (cmdline[i] == '\0') cmdline[i] = ' ';
                        }
                        
                        printf("[%ld] %s\n", pid, cmdline);
                        count++;
                        
                        // Check for suspicious process names
                        if (strstr(cmdline, "python") || 
                            strstr(cmdline, "nc ") ||
                            strstr(cmdline, "bash -i") ||
                            strstr(cmdline, "reverse")) {
                            printf("  ^^^ SUSPICIOUS ^^^\n");
                            suspicious++;
                        }
                    }
                }
            }
        }
    }
    closedir(dir);
    
    printf("Total processes: %d\n", count);
    if (suspicious > 0) {
        printf("WARNING: %d suspicious processes detected!\n", suspicious);
    }
    return count;
}

// Network connection enumeration
int jocky_enum_network() {
    FILE* f = fopen("/proc/net/tcp", "r");
    if (!f) {
        printf("Failed to open /proc/net/tcp\n");
        return -1;
    }
    
    char line[512];
    fgets(line, sizeof(line), f); // Skip header
    
    int count = 0;
    int established = 0;
    int listening = 0;
    
    while (fgets(line, sizeof(line), f)) {
        unsigned int local_addr, rem_addr;
        unsigned int local_port, rem_port;
        unsigned int state;
        int uid;
        unsigned int inode;
        
        if (sscanf(line, "%*d: %08X:%04X %08X:%04X %02X %*08X:%*08X %*02X:%*08X %*08X %d %*d %u",
                   &local_addr, &local_port, &rem_addr, &rem_port, 
                   &state, &uid, &inode) >= 5) {
            
            struct in_addr local, remote;
            local.s_addr = local_addr;
            remote.s_addr = rem_addr;
            
            if (state == 1) established++;
            if (state == 10) listening++;
            
            printf("%s:%d -> %s:%d [state=%d]\n",
                   inet_ntoa(local), ntohs((u_short)local_port),
                   inet_ntoa(remote), ntohs((u_short)rem_port),
                   state);
            count++;
        }
    }
    fclose(f);
    
    printf("Total connections: %d (established=%d, listening=%d)\n", 
           count, established, listening);
    return count;
}

// Check common persistence mechanisms
int jocky_check_persistence() {
    // Check cron jobs
    printf("Checking cron jobs...\n");
    FILE* f = popen("crontab -l 2>/dev/null | wc -l", "r");
    int cron_count = 0;
    if (f) {
        fscanf(f, "%d", &cron_count);
        pclose(f);
    }
    printf("User cron jobs: %d\n", cron_count);
    
    // Check /etc/cron.d
    DIR* dir = opendir("/etc/cron.d");
    if (dir) {
        int system_cron = 0;
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_name[0] != '.') system_cron++;
        }
        closedir(dir);
        printf("System cron jobs: %d\n", system_cron);
    }
    
    // Check systemd services
    dir = opendir("/etc/systemd/system");
    if (dir) {
        int services = 0;
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
            if (strstr(entry->d_name, ".service")) services++;
        }
        closedir(dir);
        printf("Custom systemd services: %d\n", services);
    }
    
    // Check /etc/rc.local
    if (access("/etc/rc.local", F_OK) == 0) {
        printf("WARNING: /etc/rc.local exists (persistence vector)\n");
    }
    
    return 0;
}

// Encrypt output data (placeholder)
int jocky_encrypt_output(const char* key, const char* data) {
    // XOR encrypt with key
    size_t key_len = strlen(key);
    size_t data_len = strlen(data);
    
    printf("Encrypted output (%zu bytes): ", data_len);
    for (size_t i = 0; i < data_len && i < 32; i++) {
        printf("%02x", (unsigned char)(data[i] ^ key[i % key_len]));
    }
    printf("...\n");
    
    return 0;
}
