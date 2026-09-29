# Quick Reference: Runtime Error Handling

## Error Codes

```c
#define JOCKY_SUCCESS                    0
#define JOCKY_ERR_NULL_PTR              -1
#define JOCKY_ERR_INVALID_SIZE          -2
#define JOCKY_ERR_ALLOCATION_FAILED     -3
#define JOCKY_ERR_OPERATION_FAILED      -4
#define JOCKY_ERR_INVALID_PARAM         -5
#define JOCKY_ERR_BUFFER_OVERFLOW       -6
```

## Safe String Operations (Don't Crash on NULL)

| Function | Purpose | Returns |
|----------|---------|---------|
| `jocky_safe_strlen(s)` | Get string length | 0 if NULL |
| `jocky_safe_strcpy(d, size, s)` | Copy string | error code |
| `jocky_safe_strcat(d, size, s)` | Append string | error code |
| `jocky_safe_strcmp(s1, s2)` | Compare strings | result or error |

## Safe Memory Operations (Validated Inputs)

| Function | Purpose | Returns |
|----------|---------|---------|
| `jocky_safe_memcpy(d, s, len)` | Copy memory | NULL if error |
| `jocky_safe_memset(p, v, len)` | Fill memory | NULL if error |
| `jocky_safe_memmove(d, s, len)` | Move (overlapping safe) | NULL if error |
| `jocky_safe_free(&p)` | Free + NULL assignment | void |

## Crypto with Error Checking

```c
int32_t jocky_decrypt_xor_safe(data, len, key);
int32_t jocky_decrypt_rc4_safe(data, len, key, key_len);
```

## Memory Leak Detection

```c
jocky_mem_track_init();              /* Initialize tracking */
void *ptr = JOCKY_ALLOC_TRACKED(sz); /* Allocate & track */
JOCKY_FREE_TRACKED(ptr);             /* Free & remove from tracking */
jocky_mem_dump_leaks();              /* Print leak report */
```

## Error Handling Pattern

```c
/* String operation */
int32_t result = jocky_safe_strcpy(buf, sizeof(buf), input);
if (result != JOCKY_SUCCESS) {
    fprintf(stderr, "Error: %s\n", jocky_error_message(result));
    return result;  /* Propagate error */
}

/* Memory operation */
if (!jocky_safe_memcpy(dst, src, len)) {
    fprintf(stderr, "Copy failed\n");
    return false;
}

/* Always check allocation */
void *ptr;
if (jocky_alloc_safe(size, &ptr) != JOCKY_SUCCESS) {
    fprintf(stderr, "Allocation failed\n");
    return false;
}
```

## Common Mistakes to Avoid

```c
/* ❌ WRONG: Ignoring NULL possibility */
size_t len = strlen(user_input);  /* Crashes if NULL */

/* ✅ RIGHT: Using safe variant */
size_t len = jocky_safe_strlen(user_input);  /* Returns 0 if NULL */

/* ❌ WRONG: Not checking buffer size */
strcpy(buf, user_input);  /* Buffer overflow risk */

/* ✅ RIGHT: Using safe variant with size check */
int32_t result = jocky_safe_strcpy(buf, sizeof(buf), user_input);
if (result == JOCKY_ERR_BUFFER_OVERFLOW) {
    fprintf(stderr, "Input too long\n");
}

/* ❌ WRONG: Not setting pointer to NULL after free */
free(ptr);
if (ptr) {  /* Bug! ptr still points to freed memory */
    /* ... */
}

/* ✅ RIGHT: Using safe free */
jocky_safe_free(&ptr);  /* ptr is now NULL */
if (ptr) {  /* Safe: ptr is NULL */
    /* ... */
}
```

## Enabling Error Logging

```c
#ifdef DEBUG
jocky_error_logging_enable(true);
#else
jocky_error_logging_enable(false);
#endif
```

## When to Use What

### Use Safe Variants For:
- Untrusted input (network, user, files)
- Boundary conditions
- Security-sensitive code
- Public APIs

### Use Regular Variants For:
- Already-validated data
- Performance-critical code
- Internal operations
- After bounds checking

### Use Memory Tracking For:
- Debug builds
- Testing
- Long-running processes
- Leak detection

## Testing Your Code

```c
/* Test with NULL input */
char buf[10];
assert(jocky_safe_strcpy(buf, 10, NULL) == JOCKY_ERR_NULL_PTR);

/* Test with buffer too small */
assert(jocky_safe_strcpy(buf, 5, "hello") == JOCKY_ERR_BUFFER_OVERFLOW);

/* Test valid operation */
assert(jocky_safe_strcpy(buf, 10, "hi") == JOCKY_SUCCESS);
assert(strcmp(buf, "hi") == 0);
```

## File Locations

- **Headers**: `src/runtime/include/jocky_error.h`
- **Implementation**: `src/runtime/util/error.c`
- **Tests**: `tests/unit/test_error_handling.c`
- **Documentation**: `docs/RUNTIME_ERROR_HANDLING.md`

## Function Signatures

```c
/* Error codes and messages */
const char* jocky_error_message(int32_t err);
void jocky_error_logging_enable(bool enable);
void jocky_error_log(const char *func, const char *file, int line,
                     int32_t err, const char *msg);

/* String safety */
size_t jocky_safe_strlen(const char *str);
int32_t jocky_safe_strcpy(char *dst, size_t dst_size, const char *src);
int32_t jocky_safe_strcat(char *dst, size_t dst_size, const char *src);
int32_t jocky_safe_strcmp(const char *s1, const char *s2);

/* Memory safety */
void* jocky_safe_memcpy(void *dst, const void *src, size_t len);
void* jocky_safe_memset(void *ptr, int value, size_t len);
void* jocky_safe_memmove(void *dst, const void *src, size_t len);
void jocky_safe_free(void **ptr);

/* Allocation safety */
int32_t jocky_alloc_safe(int64_t size, void **out);

/* Memory tracking */
void jocky_mem_track_init(void);
void* jocky_alloc_tracked(size_t size, const char *file, int line);
int32_t jocky_free_tracked(void *ptr, const char *file, int line);
void jocky_mem_dump_leaks(void);
size_t jocky_mem_total_allocated(void);
uint32_t jocky_mem_allocation_count(void);
void jocky_mem_track_clear(void);

/* Crypto safety */
int32_t jocky_decrypt_xor_safe(uint8_t* data, size_t len, uint8_t key);
int32_t jocky_decrypt_rc4_safe(uint8_t* data, size_t len,
                                const uint8_t* key, size_t key_len);
```

## Macros

```c
/* Memory tracking macros */
#define JOCKY_ALLOC_TRACKED(size)        jocky_alloc_tracked(size, __FILE__, __LINE__)
#define JOCKY_FREE_TRACKED(ptr)          jocky_free_tracked(ptr, __FILE__, __LINE__)

/* Error logging macro */
#define JOCKY_ERROR_LOG(err, msg)        jocky_error_log(__func__, __FILE__, __LINE__, err, msg)
```

---

For detailed documentation, see `RUNTIME_ERROR_HANDLING.md`
