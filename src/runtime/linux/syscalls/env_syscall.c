#include "../include/jocky_env.h"
#include "../include/jocky_syscall.h"
#include <string.h>
#include <stdio.h>

/* Simple environment variable storage (for setenv/unsetenv) */
#define MAX_ENV_VARS 256
static char env_buffer[16384];
static char* env_vars[MAX_ENV_VARS];
static int env_count = 0;

int jocky_getenv(const char* name, char* buffer, size_t size) {
    if (!name || !buffer || size == 0) return -1;

    size_t name_len = strlen(name);

    /* First check our override buffer */
    for (int i = 0; i < env_count; i++) {
        if (strncmp(env_vars[i], name, name_len) == 0 && env_vars[i][name_len] == '=') {
            const char* value = &env_vars[i][name_len + 1];
            strncpy(buffer, value, size - 1);
            buffer[size - 1] = '\0';
            return 0;
        }
    }

    /* Then check /proc/self/environ */
    long fd = jocky_syscall2(SYS_open, (long)"/proc/self/environ", 0);
    if (fd < 0) return -1;

    char buf[8192];
    long nread = jocky_syscall3(SYS_read, fd, (long)buf, sizeof(buf) - 1);
    jocky_syscall1(SYS_close, fd);

    if (nread <= 0) return -1;
    buf[nread] = '\0';

    /* Parse null-separated environment */
    char* pos = buf;
    while (pos < buf + nread) {
        if (strncmp(pos, name, name_len) == 0 && pos[name_len] == '=') {
            const char* value = &pos[name_len + 1];
            strncpy(buffer, value, size - 1);
            buffer[size - 1] = '\0';
            return 0;
        }

        pos += strlen(pos) + 1;
    }

    return -1;  /* Not found */
}

int jocky_setenv(const char* name, const char* value, int overwrite) {
    if (!name || !value) return -1;

    size_t name_len = strlen(name);
    size_t value_len = strlen(value);

    /* Check if already exists */
    for (int i = 0; i < env_count; i++) {
        if (strncmp(env_vars[i], name, name_len) == 0 && env_vars[i][name_len] == '=') {
            if (!overwrite) return 0;  /* Don't overwrite */

            /* Remove old entry */
            for (int j = i; j < env_count - 1; j++) {
                env_vars[j] = env_vars[j + 1];
            }
            env_count--;
            break;
        }
    }

    /* Add new entry */
    if (env_count >= MAX_ENV_VARS - 1) return -1;

    size_t needed = name_len + 1 + value_len + 1;
    if (env_count == 0 || !env_vars[env_count - 1]) {
        /* Allocate from buffer */
        char* entry = env_buffer;
        for (int i = 0; i < env_count; i++) {
            if (env_vars[i]) {
                entry += strlen(env_vars[i]) + 1;
            }
        }

        if (entry + needed > env_buffer + sizeof(env_buffer)) {
            return -1;  /* Buffer full */
        }

        env_vars[env_count] = entry;
        snprintf(env_vars[env_count], (env_buffer + sizeof(env_buffer)) - entry, "%s=%s", name, value);
        env_count++;

        return 0;
    }

    return -1;
}

int jocky_unsetenv(const char* name) {
    if (!name) return -1;

    size_t name_len = strlen(name);

    for (int i = 0; i < env_count; i++) {
        if (strncmp(env_vars[i], name, name_len) == 0 && env_vars[i][name_len] == '=') {
            /* Remove entry */
            for (int j = i; j < env_count - 1; j++) {
                env_vars[j] = env_vars[j + 1];
            }
            env_count--;
            return 0;
        }
    }

    return 0;  /* Not found is not an error */
}

long jocky_getuid(void) {
    return jocky_syscall0(SYS_getuid);
}

long jocky_getgid(void) {
    return jocky_syscall0(SYS_getgid);
}

int jocky_get_username(long uid, char* buffer, size_t size) {
    if (!buffer || size == 0) return -1;

    /* Read /etc/passwd */
    long fd = jocky_syscall2(SYS_open, (long)"/etc/passwd", 0);
    if (fd < 0) return -1;

    char buf[4096];
    long nread = jocky_syscall3(SYS_read, fd, (long)buf, sizeof(buf) - 1);
    jocky_syscall1(SYS_close, fd);

    if (nread <= 0) return -1;
    buf[nread] = '\0';

    /* Parse passwd format: name:pwd:uid:gid:... */
    char* pos = buf;
    while (pos < buf + nread) {
        char* colon = strchr(pos, ':');
        if (!colon) break;

        long parsed_uid = 0;
        char* uidpos = colon + 1;
        uidpos = strchr(uidpos, ':');  /* Skip pwd field */
        if (uidpos) {
            uidpos++;
            while (*uidpos >= '0' && *uidpos <= '9') {
                parsed_uid = parsed_uid * 10 + (*uidpos - '0');
                uidpos++;
            }

            if (parsed_uid == uid) {
                size_t len = colon - pos;
                if (len >= size) len = size - 1;
                strncpy(buffer, pos, len);
                buffer[len] = '\0';
                return 0;
            }
        }

        pos = strchr(pos, '\n');
        if (!pos) break;
        pos++;
    }

    return -1;
}

int jocky_get_groupname(long gid, char* buffer, size_t size) {
    if (!buffer || size == 0) return -1;

    /* Read /etc/group */
    long fd = jocky_syscall2(SYS_open, (long)"/etc/group", 0);
    if (fd < 0) return -1;

    char buf[4096];
    long nread = jocky_syscall3(SYS_read, fd, (long)buf, sizeof(buf) - 1);
    jocky_syscall1(SYS_close, fd);

    if (nread <= 0) return -1;
    buf[nread] = '\0';

    /* Parse group format: name:pwd:gid:members */
    char* pos = buf;
    while (pos < buf + nread) {
        char* colon = strchr(pos, ':');
        if (!colon) break;

        long parsed_gid = 0;
        char* gidpos = colon + 1;
        gidpos = strchr(gidpos, ':');  /* Skip pwd field */
        if (gidpos) {
            gidpos++;
            while (*gidpos >= '0' && *gidpos <= '9') {
                parsed_gid = parsed_gid * 10 + (*gidpos - '0');
                gidpos++;
            }

            if (parsed_gid == gid) {
                size_t len = colon - pos;
                if (len >= size) len = size - 1;
                strncpy(buffer, pos, len);
                buffer[len] = '\0';
                return 0;
            }
        }

        pos = strchr(pos, '\n');
        if (!pos) break;
        pos++;
    }

    return -1;
}

int jocky_get_home_dir(char* buffer, size_t size) {
    if (!buffer || size == 0) return -1;

    long uid = jocky_getuid();
    char username[256];

    if (jocky_get_username(uid, username, sizeof(username)) != 0) {
        return -1;
    }

    /* Read /etc/passwd to find home directory */
    long fd = jocky_syscall2(SYS_open, (long)"/etc/passwd", 0);
    if (fd < 0) return -1;

    char buf[4096];
    long nread = jocky_syscall3(SYS_read, fd, (long)buf, sizeof(buf) - 1);
    jocky_syscall1(SYS_close, fd);

    if (nread <= 0) return -1;
    buf[nread] = '\0';

    /* Find username line and extract home directory (last field before newline) */
    char* pos = buf;
    size_t ulen = strlen(username);

    while (pos < buf + nread) {
        if (strncmp(pos, username, ulen) == 0 && pos[ulen] == ':') {
            /* Found the line, count colons to reach home directory (6th field) */
            int colon_count = 0;
            char* field_start = pos;

            for (char* p = pos; *p && *p != '\n'; p++) {
                if (*p == ':') {
                    colon_count++;
                    if (colon_count == 5) {  /* Home directory is 6th field */
                        field_start = p + 1;
                    }
                    if (colon_count == 6) {  /* End at 7th colon */
                        size_t len = p - field_start;
                        if (len >= size) len = size - 1;
                        strncpy(buffer, field_start, len);
                        buffer[len] = '\0';
                        return 0;
                    }
                }
            }

            /* If no 7th colon, copy to end of line */
            if (colon_count >= 5) {
                char* end = strchr(field_start, '\n');
                if (!end) end = strchr(field_start, '\0');
                size_t len = end - field_start;
                if (len >= size) len = size - 1;
                strncpy(buffer, field_start, len);
                buffer[len] = '\0';
                return 0;
            }
        }

        pos = strchr(pos, '\n');
        if (!pos) break;
        pos++;
    }

    return -1;
}

int jocky_get_cwd(char* buffer, size_t size) {
    if (!buffer || size == 0) return -1;

    long result = jocky_syscall2(SYS_getcwd, (long)buffer, size);
    return (result > 0) ? 0 : -1;
}

int jocky_chdir(const char* path) {
    if (!path) return -1;

    long result = jocky_syscall1(SYS_chdir, (long)path);
    return (result == 0) ? 0 : -1;
}
