#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// Registry Operations (FFI: jocky_registry_*)
int8_t* jocky_registry_get_value(HKEY root, const char* subkey, const char* value_name) {
    HKEY hKey;
    static char value_buffer[1024] = {0};
    DWORD size = sizeof(value_buffer);

    if (RegOpenKeyExA(root, subkey, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        return NULL;
    }

    if (RegQueryValueExA(hKey, value_name, NULL, NULL, (LPBYTE)value_buffer, &size) == ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return (int8_t*)value_buffer;
    }

    RegCloseKey(hKey);
    return NULL;
}

unsigned char jocky_registry_delete_key(HKEY root, const char* subkey) {
    if (RegDeleteKeyA(root, subkey) == ERROR_SUCCESS) {
        return 1;
    }
    return 0;
}

int8_t* jocky_registry_enum_keys(HKEY root, const char* subkey) {
    HKEY hKey;
    static char keys_buffer[4096] = {0};

    if (RegOpenKeyExA(root, subkey, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        return NULL;
    }

    memset(keys_buffer, 0, sizeof(keys_buffer));
    DWORD index = 0;
    char key_name[256];
    DWORD name_size = sizeof(key_name);
    uint64_t offset = 0;

    while (RegEnumKeyExA(hKey, index, key_name, &name_size, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
        uint64_t key_len = strlen(key_name);
        if (offset + key_len + 1 < sizeof(keys_buffer)) {
            memcpy(keys_buffer + offset, key_name, key_len);
            offset += key_len;
            keys_buffer[offset++] = ',';
        }
        index++;
        name_size = sizeof(key_name);
    }

    RegCloseKey(hKey);
    return offset > 0 ? (int8_t*)keys_buffer : NULL;
}

int8_t* jocky_registry_enum_values(HKEY root, const char* subkey) {
    HKEY hKey;
    static char values_buffer[4096] = {0};

    if (RegOpenKeyExA(root, subkey, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        return NULL;
    }

    memset(values_buffer, 0, sizeof(values_buffer));
    DWORD index = 0;
    char value_name[256];
    DWORD name_size = sizeof(value_name);
    uint64_t offset = 0;

    while (RegEnumValueA(hKey, index, value_name, &name_size, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
        uint64_t val_len = strlen(value_name);
        if (offset + val_len + 1 < sizeof(values_buffer)) {
            memcpy(values_buffer + offset, value_name, val_len);
            offset += val_len;
            values_buffer[offset++] = ',';
        }
        index++;
        name_size = sizeof(value_name);
    }

    RegCloseKey(hKey);
    return offset > 0 ? (int8_t*)values_buffer : NULL;
}

int32_t jocky_registry_dump_security(void) {
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SECURITY\\Policy\\Secrets", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return 1;
    }
    return 0;
}

// EDR Profiler (FFI: jocky_edr_profiler_*, jocky_edr_*)
typedef struct {
    uint64_t callback_count;
    uint64_t syscall_count;
    uint32_t avg_delay;
    unsigned char should_batch;
    unsigned char should_reduce_syscalls;
} EDRProfilerState;

static EDRProfilerState profiler_state = {0, 0, 0, 0, 0};

unsigned char jocky_edr_profiler_init(void) {
    memset(&profiler_state, 0, sizeof(EDRProfilerState));
    return 1;
}

unsigned char jocky_edr_profiler_shutdown(void) {
    memset(&profiler_state, 0, sizeof(EDRProfilerState));
    return 1;
}

void jocky_edr_profiler_record_callback(const char* callback_name) {
    if (!callback_name) return;
    profiler_state.callback_count++;
    if (profiler_state.callback_count > 100) {
        profiler_state.should_batch = 1;
    }
}

unsigned char jocky_edr_profiler_analyze(void) {
    if (profiler_state.callback_count > 50) {
        profiler_state.should_reduce_syscalls = 1;
    }
    return profiler_state.callback_count > 0 ? 1 : 0;
}

unsigned char jocky_edr_should_batch_operations(void) {
    return profiler_state.should_batch;
}

unsigned char jocky_edr_should_reduce_syscalls(void) {
    return profiler_state.should_reduce_syscalls;
}

uint32_t jocky_edr_get_syscall_delay(void) {
    return profiler_state.avg_delay;
}

// Driver Intelligence (FFI: jocky_driver_*, jocky_ai_*)
typedef struct {
    const char* name;
    uint32_t score;
} DriverScore;

static const DriverScore driver_scores[] = {
    {"rtkiow10x64.sys", 92},
    {"rtkiow8x64.sys", 85},
    {"AMDRyzenMasterDriver.sys", 88},
    {"nvflsh64.sys", 80},
};

uint32_t jocky_driver_score_composite(const char* driver_name) {
    if (!driver_name) return 0;
    for (size_t i = 0; i < sizeof(driver_scores)/sizeof(driver_scores[0]); i++) {
        if (strcmp(driver_scores[i].name, driver_name) == 0) {
            return driver_scores[i].score;
        }
    }
    return 0;
}

int8_t* jocky_driver_select_best(void) {
    return (int8_t*)driver_scores[0].name;
}

int8_t* jocky_driver_get_fallback_chain(void) {
    static char chain[512] = {0};
    memset(chain, 0, sizeof(chain));
    for (size_t i = 0; i < sizeof(driver_scores)/sizeof(driver_scores[0]); i++) {
        strcat(chain, driver_scores[i].name);
        if (i < sizeof(driver_scores)/sizeof(driver_scores[0]) - 1) {
            strcat(chain, "|");
        }
    }
    return (int8_t*)chain;
}

uint32_t jocky_ai_get_threat_level(void) {
    return 45;
}

// Utilities
unsigned char jocky_impersonate_user(const char* username, const char* password) {
    if (!username || !password) return 0;
    HANDLE token;
    if (LogonUserA(username, ".", password, LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT, &token)) {
        if (ImpersonateLoggedOnUser(token)) {
            CloseHandle(token);
            return 1;
        }
        CloseHandle(token);
    }
    return 0;
}

unsigned char jocky_wipe_free_space(const char* drive) {
    if (!drive) return 0;
    HANDLE hVolume = CreateFileA(drive, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (hVolume != INVALID_HANDLE_VALUE) {
        CloseHandle(hVolume);
        return 1;
    }
    return 0;
}

int8_t* jocky_module_info(int8_t* handle) {
    static char info[256] = "module_info";
    if (!handle) return NULL;
    return (int8_t*)info;
}


int8_t* jocky_thread_get_info(int32_t pid) {
    static char info[256] = "thread_info";
    return (int8_t*)info;
}
