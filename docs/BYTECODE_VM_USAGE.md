# JOCKY Bytecode VM Usage Guide

## Quick Start

### Basic Virtualization

Mark a function with the `@virtualize` attribute to compile it to bytecode:

```jocky
#[virtualize]
fn critical_check(key: u64) -> bool {
    key ^ 0xDEADBEEF == 0x13371337
}
```

### Compilation

The compiler automatically:
1. Detects the `@virtualize` attribute
2. Compiles the function to JOCKY bytecode
3. Embeds bytecode in `.jvm` section
4. Generates a VM stub function

### Execution

The virtualized function behaves identically to native code:

```jocky
let result = critical_check(key);
```

No changes needed to caller code!

## Virtualization Attributes

### `@virtualize`

Basic virtualization with default obfuscation:

```jocky
#[virtualize]
fn my_function(x: i64) -> i64 {
    x + 1
}
```

### `@virtualize(level=1)`

Light obfuscation (better performance):

```jocky
#[virtualize(level=1)]
fn performance_critical(x: i64) -> i64 {
    x * x
}
```

### `@virtualize(level=2)`

Standard obfuscation (balanced):

```jocky
#[virtualize(level=2)]
fn normal_security(key: u64) -> u64 {
    key ^ 0x1234567890ABCDEF
}
```

### `@virtualize(level=3)`

Aggressive obfuscation (maximum security):

```jocky
#[virtualize(level=3)]
fn ultra_sensitive(data: u64) -> u64 {
    crypto_transform(data)
}
```

## Function Types

### Simple Arithmetic

```jocky
#[virtualize]
fn fibonacci(n: i32) -> i32 {
    if n <= 1 {
        n
    } else {
        fibonacci(n - 1) + fibonacci(n - 2)
    }
}
```

### Cryptographic Operations

```jocky
#[virtualize(level=3)]
fn decrypt_block(block: u64, key: u64) -> u64 {
    let mut result = block;
    result = result ^ key;
    result = result >> 8;
    result = result ^ (key << 1);
    result
}
```

### Validation Functions

```jocky
#[virtualize(level=2)]
fn validate_signature(sig: u64, expected: u64) -> bool {
    sig == expected
}
```

### State Machine Logic

```jocky
#[virtualize]
fn state_transition(current: i32, event: i32) -> i32 {
    if current == 0 {
        if event == 1 { 1 } else { 0 }
    } else if current == 1 {
        if event == 2 { 2 } else { 1 }
    } else {
        0
    }
}
```

## Obfuscation Levels

### Level 0 (None)
No virtualization, regular compilation:
```jocky
#[virtualize(level=0)]
fn normal_function(x: i64) -> i64 { x }
```

### Level 1 (Light)
- Polymorphic instruction dispatch
- Basic register randomization
- Minimal overhead

```jocky
#[virtualize(level=1)]
fn light_obfuscation() { }
```

**Performance**: ~3-5x slower
**Reverse engineering difficulty**: Medium

### Level 2 (Standard) - Default

- Polymorphic instructions
- Register randomization
- Opaque predicates
- Control flow flattening

```jocky
#[virtualize(level=2)]  // or just #[virtualize]
fn standard_obfuscation() { }
```

**Performance**: ~5-10x slower
**Reverse engineering difficulty**: High

### Level 3 (Aggressive)

- All Level 2 techniques
- Enhanced control flow obfuscation
- Bytecode shuffling
- Instruction reordering
- Dummy instructions

```jocky
#[virtualize(level=3)]
fn aggressive_obfuscation() { }
```

**Performance**: ~10-15x slower
**Reverse engineering difficulty**: Very High

## Performance Optimization

### Selective Virtualization

Only virtualize security-critical functions:

```jocky
#[virtualize]  // Virtualized - slow but secure
fn secret_algorithm(key: u64) -> u64 { /* ... */ }

fn fast_helper(x: i64) -> i64 {  // Not virtualized - fast
    x + 1
}

fn caller() {
    let result = secret_algorithm(key);  // OK: one VM call
    for i in 0..1000000 {
        result = fast_helper(result);    // OK: hot path not virtualized
    }
}
```

### Hot Loop Optimization

Don't virtualize loop bodies:

```jocky
#[virtualize]
fn setup(key: u64) -> u64 { /* setup code */ }

fn process_items(items: &[u64], key: u64) -> u64 {
    let config = setup(key);  // Virtualized once
    
    let mut result = 0u64;
    for item in items {
        result = result ^ item;  // Fast, not virtualized
    }
    result
}
```

### Parameter Passing

Keep virtualized functions small:

```jocky
// GOOD: Small virtualized function
#[virtualize]
fn check_key(key: u64) -> bool {
    key == 0xDEADBEEF
}

// AVOID: Large virtualized function
#[virtualize]
fn process_large_data(data: &[u8]) -> u64 {
    let mut sum = 0u64;
    for byte in data {
        sum = sum ^ (*byte as u64);
    }
    sum
}
```

## Debugging Virtualized Functions

### Inspection

Virtualized functions appear in LLVM IR:

```llvm
define i64 @critical_check(i64 %key) {
entry:
  %vm = call i8* @jocky_vm_create()
  %bytecode_ptr = getelementptr ...
  call void @jocky_vm_load(i8* %vm, i8* %bytecode_ptr, i64 128)
  %result = call i64 @jocky_vm_execute(i8* %vm, i64 %key, i64 0, i64 0, i64 0)
  call void @jocky_vm_destroy(i8* %vm)
  ret i64 %result
}
```

### Testing

Test virtualized functions like normal functions:

```jocky
#[test]
fn test_virtualized_function() {
    assert_eq!(critical_check(0xDEADBEEF), true);
    assert_eq!(critical_check(0x12345678), false);
}
```

### Disabling Virtualization

Temporarily remove `@virtualize` for debugging:

```jocky
// #[virtualize]  // Disabled for debugging
fn critical_check(key: u64) -> bool {
    key == 0xDEADBEEF
}
```

## Advanced Topics

### Multiple Parameters

Functions with up to 4 parameters are fully supported:

```jocky
#[virtualize]
fn transform(a: u64, b: u64, c: u64, d: u64) -> u64 {
    a ^ b ^ c ^ d
}
```

Note: Only first 4 parameters passed through registers. Additional parameters need manual stack handling.

### Return Values

All standard types supported:

```jocky
#[virtualize]
fn returns_int() -> i32 { 42 }

#[virtualize]
fn returns_u64() -> u64 { 0xFFFFFFFFFFFFFFFF }

#[virtualize]
fn returns_void() { /* side effects */ }

#[virtualize]
fn returns_bool() -> bool { true }
```

### Recursive Functions

Recursion works but is inefficient due to VM overhead:

```jocky
#[virtualize(level=1)]  // Use lighter obfuscation
fn factorial(n: i32) -> i32 {
    if n <= 1 { 1 } else { n * factorial(n - 1) }
}
```

Avoid deep recursion - each VM call has overhead.

### Inlining Hints

Works as usual with virtualized functions:

```jocky
#[virtualize]
#[inline(always)]
fn small_critical() -> u64 { 0x1337 }

#[virtualize]
#[inline(never)]
fn large_function() { /* ... */ }
```

## Troubleshooting

### "Virtualization failed"

Check function has:
- Valid parameter types (i8-i64, bool, pointers)
- Valid return type
- No unsupported LLVM operations

### Function too large

Bytecode size exceeds limits. Solution:
- Break into smaller functions
- Use lower obfuscation level
- Remove obfuscation on non-critical parts

### Incorrect behavior after virtualization

Common causes:
1. Undefined behavior in original code
2. Compiler bug (report with minimal example)
3. Stack corruption (check for buffer overflows)

### Performance regression

If virtualization causes unacceptable slowdown:
- Reduce obfuscation level
- Use `@virtualize(level=1)` instead of level 2-3
- Only virtualize truly critical functions
- Profile to identify bottlenecks

## Security Best Practices

### 1. Defense in Depth

Combine virtualization with other techniques:
```jocky
#[virtualize(level=3)]
#[obfuscate]
#[inline(never)]
fn ultra_critical() { /* ... */ }
```

### 2. Anti-Analysis Integration

Add anti-debugging/anti-analysis checks:
```jocky
#[virtualize]
fn with_anti_analysis() {
    // Check if being debugged
    if is_debugged() {
        panic!()
    }
    // Actual logic
}
```

### 3. Key Rotation

Virtualize key derivation, not raw keys:
```jocky
#[virtualize(level=3)]
fn derive_key(seed: u64) -> u64 {
    crypto_kdf(seed)
}
```

### 4. Sensitive Operations

Virtualize validation and crypto operations:
```jocky
#[virtualize(level=2)]
fn validate_token(token: u64, expected: u64) -> bool {
    token == expected
}
```

## Example Programs

### Example 1: Simple Arithmetic

```jocky
#[virtualize]
fn add_one(x: i64) -> i64 {
    x + 1
}

fn main() {
    let result = add_one(41);
    println!("{}", result);  // Output: 42
}
```

### Example 2: Cryptographic Check

```jocky
#[virtualize(level=3)]
fn validate_key(key: u64) -> bool {
    (key ^ 0xDEADBEEF) == (0x13371337 ^ 0xCAFEBABE)
}

fn main() {
    if validate_key(0xDEADBEEF ^ 0x13371337 ^ 0xCAFEBABE) {
        println!("Key valid");
    }
}
```

### Example 3: State Machine

```jocky
#[virtualize(level=2)]
fn state_machine(state: i32, input: i32) -> i32 {
    match state {
        0 => if input == 1 { 1 } else { 0 },
        1 => if input == 2 { 2 } else { 1 },
        2 => if input == 3 { 3 } else { 2 },
        _ => 0,
    }
}

fn main() {
    let mut s = 0;
    s = state_machine(s, 1);
    s = state_machine(s, 2);
    s = state_machine(s, 3);
    assert_eq!(s, 3);
}
```

## Further Reading

- [VM Architecture](VM_ARCHITECTURE.md) - Technical details
- [Bytecode Format](BYTECODE_FORMAT.md) - Binary format specification
- [Obfuscation Techniques](OBFUSCATION_TECHNIQUES.md) - Deep dive into obfuscation
