# JOCKY Language Builtins

## Overview

JOCKY provides several built-in functions that are always available without explicit declarations. These are compile-time intrinsic functions.

## Reference

### sizeof(x)

Returns the size in bytes of a type or variable.

**Signature:**
```jocky
sizeof(x) -> i32
```

**Parameters:**
- `x` - Any expression (variable, type, etc.)

**Returns:**
- `i32` - Size in bytes

**Examples:**
```jocky
fn main() -> void {
    let x: i32 = 42;
    let size: i32 = sizeof(x);  // Returns 4
    
    let p: i8* = null;
    let ptr_size: i32 = sizeof(p);  // Returns 8 on 64-bit
}
```

**Use Cases:**
- Memory layout calculations
- Buffer allocation
- Struct serialization
- FFI boundary management

### nameof(x)

Returns the name of a variable or type as a string at compile time.

**Signature:**
```jocky
nameof(x) -> string
```

**Parameters:**
- `x` - Variable or type name (must be a direct reference)

**Returns:**
- `string` - Name of the variable/type

**Examples:**
```jocky
fn main() -> void {
    let my_variable: i32 = 10;
    let name: string = nameof(my_variable);  // Returns "my_variable"
    
    let result: string = nameof(result);  // Returns "result"
}
```

**Restrictions:**
- Must be applied to a direct variable reference
- Cannot be used on expressions: `nameof(x + 1)` will fail
- Useful for debugging and logging

**Use Cases:**
- Debug logging: `printf("Value of %s: %d\n", nameof(x), x)`
- Reflection in serialization
- Error messages with variable context
- Testing and diagnostics

---

## Error Messages with Context

When a type error or other compile-time error occurs, JOCKY displays the error with surrounding source code context.

### Format

```
error[E0002]: Type mismatch: expected i32, got bool
  ├─ test.jky:3:18
  ├─
     2 |   let x: i32 = 1;
  >>> 3 |   let y: bool = x;
       |                  ^
     4 |   let z: i32 = 3;
```

### Features

- **Location** - File, line, and column of the error
- **Message** - Clear explanation of what went wrong
- **Context** - 2 lines before and after the error (configurable)
- **Indicator** - `^` points to the exact error location
- **Error Code** - E-prefixed code for reference (e.g., `E0002`)

### Error Codes

| Code | Category | Description |
|------|----------|-------------|
| E0002 | Type Error | Type mismatch in expression |
| E0004 | Parse Error | Syntax error in source code |
| E0010 | CodeGen Error | Error during code generation |

### Context Configuration

Adjust context lines in `src/jocky/language/errors.py`:

```python
class ErrorFormatter:
    CONTEXT_LINES = 2  # Lines before/after error location
```

Increase for more context, decrease for conciseness.

---

## Built-in Type System

### Primitive Types

- `i8`, `i32`, `i64` - Integer types
- `bool` - Boolean type
- `string` - String type

### Composite Types

- `T*` - Pointer to type T
- `T[]` - Array of type T (fixed size)

### Special Values

- `null` - Null pointer (compatible with any pointer type)
- `true`, `false` - Boolean values

---

## FFI Declarations

While not "builtins", FFI functions are declared without implementation:

```jocky
ffi printf(format: string, ...) -> i32;
ffi puts(s: string) -> i32;
```

These map to external C functions and are processed by the compiler.

---

See also: [Language Specification](./language-spec.md), [Type System](./architecture.md)
