/*
 * JOCKY Advanced Linux Process Manipulation
 * Namespace modification, cgroup injection, credential manipulation
 * Authorized: Red Hat + IIT Bombay Cyber Security Team
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef struct {
    pid_t target_pid;
    int ns_pid;
    int ns_net;
    int ns_ipc;
    int ns_uts;
    int ns_user;
    int ns_mnt;
} namespace_context_t;

int jocky_process_enter_namespace(
    pid_t target_pid,
    namespace_context_t* ctx)
{
    char ns_path[64];
    int flags = 0;

    ctx->target_pid = target_pid;

    /* Try to access target process namespaces */
    snprintf(ns_path, sizeof(ns_path), "/proc/%d/ns/pid", target_pid);
    ctx->ns_pid = open(ns_path, O_RDONLY);

    snprintf(ns_path, sizeof(ns_path), "/proc/%d/ns/net", target_pid);
    ctx->ns_net = open(ns_path, O_RDONLY);

    snprintf(ns_path, sizeof(ns_path), "/proc/%d/ns/ipc", target_pid);
    ctx->ns_ipc = open(ns_path, O_RDONLY);

    if (ctx->ns_pid < 0 || ctx->ns_net < 0 || ctx->ns_ipc < 0) {
        fprintf(stderr, "[!] Cannot access target namespace (requires CAP_SYS_ADMIN)\n");
        return -1;
    }

    fprintf(stdout, "[+] Namespace file descriptors obtained for PID %d\n", target_pid);
    return 0;
}

int jocky_process_create_namespace(
    int namespace_type)
{
    int flags = 0;

    switch (namespace_type) {
        case 0: flags = CLONE_NEWPID; break;   /* PID namespace */
        case 1: flags = CLONE_NEWNET; break;   /* Network namespace */
        case 2: flags = CLONE_NEWIPC; break;   /* IPC namespace */
        case 3: flags = CLONE_NEWUTS; break;   /* UTS (hostname) namespace */
        case 4: flags = CLONE_NEWUSER; break;  /* User namespace */
        case 5: flags = CLONE_NEWNS; break;    /* Mount namespace */
        default:
            fprintf(stderr, "[!] Invalid namespace type\n");
            return -1;
    }

    /* Would require actual clone() call - placeholder */
    fprintf(stdout, "[*] Namespace creation would require fork/clone with flags 0x%x\n", flags);
    return 0;
}

int jocky_process_inject_cgroup(
    pid_t target_pid,
    const char* cgroup_path)
{
    char cgroup_procs_path[256];
    FILE* cgroup_procs;

    if (!cgroup_path) return -1;

    snprintf(cgroup_procs_path, sizeof(cgroup_procs_path),
             "%s/cgroup.procs", cgroup_path);

    cgroup_procs = fopen(cgroup_procs_path, "w");
    if (!cgroup_procs) {
        fprintf(stderr, "[!] Cannot write to cgroup (requires CAP_SYS_ADMIN)\n");
        return -1;
    }

    fprintf(cgroup_procs, "%d\n", target_pid);
    fclose(cgroup_procs);

    fprintf(stdout, "[+] Process %d injected into cgroup %s\n", target_pid, cgroup_path);
    return 0;
}

int jocky_process_manipulate_credentials(
    pid_t target_pid,
    uid_t new_uid,
    gid_t new_gid)
{
    char uid_map_path[64];
    char gid_map_path[64];
    FILE* uid_map;
    FILE* gid_map;

    snprintf(uid_map_path, sizeof(uid_map_path), "/proc/%d/uid_map", target_pid);
    snprintf(gid_map_path, sizeof(gid_map_path), "/proc/%d/gid_map", target_pid);

    uid_map = fopen(uid_map_path, "w");
    if (!uid_map) {
        fprintf(stderr, "[!] Cannot write uid_map (requires CAP_SETUID in user ns)\n");
        return -1;
    }

    fprintf(uid_map, "0 %d 1\n", new_uid);
    fclose(uid_map);

    gid_map = fopen(gid_map_path, "w");
    if (!gid_map) {
        fprintf(stderr, "[!] Cannot write gid_map\n");
        return -1;
    }

    fprintf(gid_map, "0 %d 1\n", new_gid);
    fclose(gid_map);

    fprintf(stdout, "[+] Process credentials mapped: UID %d -> %d, GID %d -> %d\n",
            0, new_uid, 0, new_gid);
    return 0;
}

int jocky_process_hide_from_proc(pid_t target_pid)
{
    char proc_path[64];

    snprintf(proc_path, sizeof(proc_path), "/proc/%d", target_pid);

    /* Would require /proc filesystem modification - requires kernel module */
    fprintf(stdout, "[*] Process hiding requires /proc filesystem modification\n");
    fprintf(stdout, "    Would need eBPF/kernel module to filter %s\n", proc_path);

    return 0;
}

int jocky_process_enumerate_threads(
    pid_t target_pid)
{
    char task_path[64];
    char* tasks;
    FILE* task_list;
    char line[64];
    int thread_count = 0;

    snprintf(task_path, sizeof(task_path), "/proc/%d/task", target_pid);

    struct dirent* entry;
    DIR* d = opendir(task_path);

    if (!d) {
        fprintf(stderr, "[!] Cannot enumerate threads\n");
        return -1;
    }

    fprintf(stdout, "[*] Threads for PID %d:\n", target_pid);

    while ((entry = readdir(d)) != NULL) {
        if (entry->d_type == DT_DIR && atoi(entry->d_name) > 0) {
            fprintf(stdout, "    TID: %s\n", entry->d_name);
            thread_count++;
        }
    }

    closedir(d);
    fprintf(stdout, "[+] Enumerated %d threads\n", thread_count);
    return thread_count;
}

int jocky_process_read_environment(
    pid_t target_pid,
    char* env_buffer,
    size_t buffer_size)
{
    char environ_path[64];
    int fd;
    ssize_t bytes_read;

    snprintf(environ_path, sizeof(environ_path), "/proc/%d/environ", target_pid);

    fd = open(environ_path, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "[!] Cannot read environment\n");
        return -1;
    }

    bytes_read = read(fd, env_buffer, buffer_size - 1);
    close(fd);

    if (bytes_read < 0) return -1;

    env_buffer[bytes_read] = '\0';

    fprintf(stdout, "[+] Read %zd bytes of environment variables\n", bytes_read);
    return bytes_read;
}

int jocky_process_query_limits(
    pid_t target_pid)
{
    char limits_path[64];
    FILE* limits;
    char line[256];

    snprintf(limits_path, sizeof(limits_path), "/proc/%d/limits", target_pid);

    limits = fopen(limits_path, "r");
    if (!limits) {
        fprintf(stderr, "[!] Cannot read process limits\n");
        return -1;
    }

    fprintf(stdout, "[*] Resource limits for PID %d:\n", target_pid);

    while (fgets(line, sizeof(line), limits)) {
        fprintf(stdout, "    %s", line);
    }

    fclose(limits);
    return 0;
}

int jocky_process_modify_signal_handlers(
    pid_t target_pid)
{
    char maps_path[64];
    FILE* maps;
    char line[256];

    snprintf(maps_path, sizeof(maps_path), "/proc/%d/maps", target_pid);

    maps = fopen(maps_path, "r");
    if (!maps) {
        fprintf(stderr, "[!] Cannot read process memory map\n");
        return -1;
    }

    fprintf(stdout, "[*] Signal handlers require memory manipulation\n");
    fprintf(stdout, "    Would need to locate signal_frame and inject handlers\n");

    fclose(maps);
    return 0;
}
