#include "../include/jocky_process_control.h"
#include "../include/jocky_syscall.h"
#include <string.h>

long jocky_fork(void) {
    return jocky_syscall0(SYS_fork);
}

int jocky_execve(const char* path, const char* const* argv, const char* const* envp) {
    if (!path) return -1;

    long result = jocky_syscall3(SYS_execve, (long)path, (long)argv, (long)envp);
    return (result < 0) ? -1 : 0;
}

int jocky_execvp(const char* filename, const char* const* argv) {
    if (!filename) return -1;

    /* Simple PATH search: check current directory and /usr/bin, /bin */
    const char* path_dirs[] = {
        "/usr/bin/",
        "/bin/",
        "/usr/local/bin/",
        NULL
    };

    char full_path[256];

    /* Try direct filename first */
    if (jocky_execve(filename, argv, NULL) >= 0) {
        return 0;
    }

    /* Try searching PATH */
    for (int i = 0; path_dirs[i]; i++) {
        strncpy(full_path, path_dirs[i], sizeof(full_path) - 1);
        strncat(full_path, filename, sizeof(full_path) - strlen(full_path) - 1);

        if (jocky_execve(full_path, argv, NULL) >= 0) {
            return 0;
        }
    }

    return -1;
}

long jocky_waitpid(long pid, int* status, int flags) {
    long result = jocky_syscall3(SYS_waitpid, pid, (long)status, flags);
    return (result < 0) ? -1 : result;
}

void jocky_exit(int status) {
    jocky_syscall1(SYS_exit, status);
    /* Never returns */
    while (1);
}

int jocky_wifexited(int status) {
    return ((status & 0xFF) == 0) ? 1 : 0;
}

int jocky_wexitstatus(int status) {
    return (status >> 8) & 0xFF;
}
