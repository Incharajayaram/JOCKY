# Attributes & Decorators

## Overview

Attributes (also called decorators or annotations) provide compile-time directives for functions, types, and modules.

## Syntax

Attributes use the `#[...]` syntax:

```jocky
#[inline]
fn add(x: i32, y: i32) -> i32 {
    x + y
}

#[no_mangle]
ffi c_function(x: i32) -> i32;
```

## Common Attributes

### Function Attributes

#### `#[inline]`
Force inline the function:
```jocky
#[inline]
fn max(a: i32, b: i32) -> i32 {
    if a > b { a } else { b }
}
```

#### `#[inline(never)]`
Never inline the function:
```jocky
#[inline(never)]
fn expensive_operation() -> i32 {
    // Complex logic
}
```

#### `#[inline(always)]`
Always inline, error if not possible:
```jocky
#[inline(always)]
fn critical_path(x: i32) -> i32 {
    x * 2
}
```

#### `#[no_mangle]`
Preserve function name in binary (don't mangle):
```jocky
#[no_mangle]
fn public_api(x: i32) -> i32 {
    x + 1
}
```

#### `#[cold]`
Mark function as rarely called (optimize for size):
```jocky
#[cold]
fn error_handler(code: i32) {
    printf("Error: %d\n", code);
}
```

#### `#[hot]`
Mark function as frequently called (optimize for speed):
```jocky
#[hot]
fn inner_loop(x: i32) -> i32 {
    x + 1
}
```

### Type Attributes

#### `#[repr(C)]`
Use C-compatible memory layout:
```jocky
#[repr(C)]
struct Point {
    x: i32,
    y: i32,
}
```

#### `#[packed]`
Pack struct with no padding:
```jocky
#[packed]
struct Flags {
    a: bool,
    b: bool,
    c: bool,
}
```

### Module Attributes

#### `#[path = "..."]`
Override module file path:
```jocky
#[path = "lib/utils.jky"]
mod utils;
```

## Compiler Attributes

### Obfuscation Control

#### `#[obfuscate(none)]`
Disable all obfuscation:
```jocky
#[obfuscate(none)]
fn debug_function(x: i32) -> i32 {
    x
}
```

#### `#[obfuscate(light)]`
Apply light obfuscation:
```jocky
#[obfuscate(light)]
fn public_key() -> i32 {
    42
}
```

#### `#[obfuscate(aggressive)]`
Apply aggressive obfuscation:
```jocky
#[obfuscate(aggressive)]
fn crypto_key() -> i32 {
    secret_value()
}
```

### Conditional Compilation

#### `#[cfg(target = "windows")]`
Compile only for Windows:
```jocky
#[cfg(target = "windows")]
fn get_system_info() -> i32 {
    // Windows-specific code
}
```

#### `#[cfg(target = "linux")]`
Compile only for Linux:
```jocky
#[cfg(target = "linux")]
fn get_system_info() -> i32 {
    // Linux-specific code
}
```

## Multiple Attributes

Stack multiple attributes:

```jocky
#[inline]
#[hot]
#[obfuscate(aggressive)]
fn critical_function(x: i32) -> i32 {
    x * x
}
```

## Standard Library Attributes

### `#[derive]`
Auto-generate implementations (future):
```jocky
#[derive(Debug, Eq)]
struct Person {
    name: string,
    age: i32,
}
```

### `#[must_use]`
Warn if return value is unused:
```jocky
#[must_use]
fn compute() -> i32 {
    42
}

compute();  // Warning: unused return value
```

## Custom Attributes

Define custom attributes:

```jocky
#[meta(version = "1.0")]
#[meta(author = "author")]
fn versioned_function() {
    // Function with metadata
}
```

## Best Practices

1. **Use `#[inline]` for short, hot functions**
   - Reduces overhead for small computations

2. **Use `#[no_mangle]` for FFI**
   - Ensures predictable symbol names for C interop

3. **Use `#[cfg(...)]` for platform-specific code**
   - Keeps code maintainable across targets

4. **Use `#[obfuscate(...)]` strategically**
   - Don't obfuscate everything (performance cost)
   - Obfuscate only sensitive functions

5. **Document attributes**
   - Explain why an attribute is needed

## Performance Impact

| Attribute | Impact | When to Use |
|-----------|--------|-----------|
| `#[inline]` | Reduces code size | Hot paths, small functions |
| `#[inline(never)]` | Forces out-of-line | Large functions, rarely called |
| `#[cold]` | Optimizes for size | Error handlers, initialization |
| `#[hot]` | Optimizes for speed | Inner loops, critical paths |
| `#[obfuscate]` | Increases code size | Sensitive logic only |

---

See also: [Language Specification](./language-spec.md), [Obfuscation Passes](./obfuscation-passes.md)
