# JOCKY Examples & Tutorials

Comprehensive examples demonstrating JOCKY language features and compiler capabilities.

---

## Table of Contents

1. [Hello World](#hello-world)
2. [Variables & Types](#variables--types)
3. [Functions & FFI](#functions--ffi)
4. [Control Flow](#control-flow)
5. [Structs & Arrays](#structs--arrays)
6. [Enums & Pattern Matching](#enums--pattern-matching)
7. [Generics & Polymorphism](#generics--polymorphism)
8. [Closures & Lambdas](#closures--lambdas)
9. [Attributes & Metadata](#attributes--metadata)
10. [Runtime Features](#runtime-features)
11. [Building & Debugging](#building--debugging)

---

## Hello World

The simplest JOCKY program:

```jocky
// hello.jky
fn main() -> void {
    ffi printf(fmt: string, ...) -> i32;
    
    printf("Hello, JOCKY!\n");
}
```

Compile and run:
```bash
./jocky build examples/hello.jky -p none
./a.out
# Output: Hello, JOCKY!
```

With obfuscation:
```bash
./jocky build examples/hello.jky -p standard -o hello_obfuscated
./hello_obfuscated
# Output: Hello, JOCKY!
```

---

## Variables & Types

### Basic Types

```jocky
fn main() -> void {
    // Integer types
    let a: i8 = -128;
    let b: i32 = 42;
    let c: i64 = 9223372036854775807;
    
    // Boolean
    let flag: bool = true;
    
    // Void (used in function returns)
    let result: void = ();
    
    // String (compile-time constant)
    let message: string = "Hello";
}
```

### Type Inference

```jocky
fn main() -> void {
    // Type is inferred from right-hand side
    let x = 10;           // inferred: i32
    let y = true;         // inferred: bool
    let z = "test";       // inferred: string
}
```

### Pointers

```jocky
fn main() -> void {
    let value: i32 = 42;
    let ptr: i32* = &value;
    
    // Dereference
    let retrieved: i32 = *ptr;
    
    ffi printf(fmt: string, ...) -> i32;
    printf("Value: %d\n", retrieved);
}
```

---

## Functions & FFI

### User-Defined Functions

```jocky
fn add(a: i32, b: i32) -> i32 {
    return a + b;
}

fn multiply(x: i32, y: i32) -> i32 {
    x * y  // implicit return
}

fn main() -> void {
    let sum = add(3, 4);      // 7
    let product = multiply(5, 6);  // 30
}
```

### FFI (Foreign Function Interface)

```jocky
fn main() -> void {
    // Declare C functions
    ffi abs(x: i32) -> i32;
    ffi strlen(str: string) -> i32;
    ffi malloc(size: i32) -> i8*;
    ffi free(ptr: i8*) -> void;
    ffi printf(fmt: string, ...) -> i32;
    
    // Use them like normal functions
    let len = strlen("Hello");
    printf("Length: %d\n", len);
    
    let neg = abs(-42);
    printf("Absolute: %d\n", neg);
}
```

### Variadic Functions

```jocky
fn main() -> void {
    ffi printf(fmt: string, ...) -> i32;
    ffi fprintf(file: i8*, fmt: string, ...) -> i32;
    ffi sprintf(buf: i8*, fmt: string, ...) -> i32;
    
    printf("Number: %d\n", 42);
    printf("Multiple: %d, %s\n", 10, "args");
}
```

---

## Control Flow

### If/Else

```jocky
fn absolute(x: i32) -> i32 {
    if x < 0 {
        return -x;
    } else {
        return x;
    }
}

fn max(a: i32, b: i32) -> i32 {
    if a > b {
        a
    } else {
        b
    }
}
```

### While Loops

```jocky
fn factorial(n: i32) -> i32 {
    let result = 1;
    let i = 2;
    while i <= n {
        result = result * i;
        i = i + 1;
    }
    return result;
}
```

### For Loops

```jocky
fn sum_range(start: i32, end: i32) -> i32 {
    let sum = 0;
    for i = start; i < end; i = i + 1 {
        sum = sum + i;
    }
    return sum;
}

fn main() -> void {
    let total = sum_range(1, 11);  // 1+2+...+10 = 55
}
```

---

## Structs & Arrays

### Struct Definitions

```jocky
struct Point {
    x: i32,
    y: i32,
}

struct Person {
    name: string,
    age: i32,
    height: i32,
}

fn main() -> void {
    // Struct initialization
    let p: Point = Point { x: 10, y: 20 };
    
    // Field access
    let x_coord = p.x;
    let y_coord = p.y;
}
```

### Arrays

```jocky
fn main() -> void {
    // Fixed-size array declaration
    let numbers: i32[5] = [1, 2, 3, 4, 5];
    
    // Array indexing
    let first = numbers[0];   // 1
    let last = numbers[4];    // 5
    
    // Array iteration
    for i = 0; i < 5; i = i + 1 {
        ffi printf(fmt: string, ...) -> i32;
        printf("numbers[%d] = %d\n", i, numbers[i]);
    }
}
```

### Struct Arrays

```jocky
struct Record {
    id: i32,
    value: i32,
}

fn main() -> void {
    let records: Record[3] = [
        Record { id: 1, value: 100 },
        Record { id: 2, value: 200 },
        Record { id: 3, value: 300 },
    ];
    
    let first_record = records[0];
    let id = first_record.id;
}
```

---

## Enums & Pattern Matching

### Enum Definitions

```jocky
enum Status {
    Success = 0,
    Error = 1,
    Pending = 2,
}

enum Result {
    Ok(value: i32),
    Err(message: string),
}
```

### Pattern Matching

```jocky
fn handle_status(status: Status) -> void {
    ffi printf(fmt: string, ...) -> i32;
    
    match status {
        Status::Success => printf("Success!\n"),
        Status::Error => printf("Error!\n"),
        Status::Pending => printf("Pending...\n"),
    }
}

fn process_result(result: Result) -> void {
    ffi printf(fmt: string, ...) -> i32;
    
    match result {
        Result::Ok(v) => printf("Value: %d\n", v),
        Result::Err(msg) => printf("Error: %s\n", msg),
    }
}
```

### Pattern Matching with Wildcards

```jocky
fn check_value(n: i32) -> void {
    match n {
        0 => printf("Zero\n"),
        1 => printf("One\n"),
        _ => printf("Other\n"),  // wildcard matches anything
    }
}
```

### Variant Construction

```jocky
fn make_ok(value: i32) -> Result {
    Result::Ok(value)
}

fn make_error(msg: string) -> Result {
    Result::Err(msg)
}

fn main() -> void {
    let success = make_ok(42);
    let failure = make_error("Something went wrong");
}
```

---

## Generics & Polymorphism

### Generic Functions

```jocky
fn max<T>(a: T, b: T) -> T {
    if a > b {
        a
    } else {
        b
    }
}

fn swap<T>(arr: T[2]) -> T[2] {
    let temp = arr[0];
    arr[0] = arr[1];
    arr[1] = temp;
    return arr;
}

fn main() -> void {
    // Monomorphization: compiler generates max for i32
    let max_int = max(10, 20);  // 20
    
    // Type checking ensures correctness
    let arr: i32[2] = [1, 2];
    let swapped = swap(arr);    // [2, 1]
}
```

### Generic Constraints

```jocky
// Constraint: T must be comparable
fn find_index<T>(arr: T[10], target: T) -> i32 {
    for i = 0; i < 10; i = i + 1 {
        if arr[i] == target {
            return i;
        }
    }
    return -1;  // not found
}

fn main() -> void {
    let numbers: i32[10] = [5, 10, 15, 20, 25, 30, 35, 40, 45, 50];
    let idx = find_index(numbers, 30);  // 5
}
```

### Multiple Type Parameters

```jocky
fn pair_first<T, U>(a: T, b: U) -> T {
    return a;
}

fn pair_second<T, U>(a: T, b: U) -> U {
    return b;
}

fn main() -> void {
    let first = pair_first(42, "hello");   // 42 (i32)
    let second = pair_second(42, "hello"); // "hello" (string)
}
```

---

## Closures & Lambdas

### Basic Closures

```jocky
fn main() -> void {
    let x = 10;
    
    // Lambda expression capturing x
    let add_x = lambda(y: i32) -> i32 {
        return x + y;
    };
    
    let result = add_x(5);  // 15 (10 + 5)
}
```

### Closures with Multiple Captures

```jocky
fn main() -> void {
    let a = 5;
    let b = 10;
    
    let compute = lambda() -> i32 {
        return a + b;  // captures both a and b
    };
    
    let result = compute();  // 15
}
```

### Higher-Order Functions

```jocky
fn apply<T>(func: (T) -> T, value: T) -> T {
    return func(value);
}

fn main() -> void {
    let double = lambda(x: i32) -> i32 { x * 2 };
    let result = apply(double, 21);  // 42
}
```

### Closure Mutation & State

```jocky
fn main() -> void {
    let counter = 0;
    
    let increment = lambda() -> i32 {
        counter = counter + 1;
        return counter;
    };
    
    let first = increment();   // 1
    let second = increment();  // 2
    let third = increment();   // 3
}
```

---

## Attributes & Metadata

### Function Attributes

```jocky
#[inline]
fn small_function(x: i32) -> i32 {
    x * 2
}

#[no_mangle]
fn exported_function() -> void {
    ffi printf(fmt: string, ...) -> i32;
    printf("Called from C!\n");
}

#[deprecated]
fn old_function() -> void {
    // compiler warns if used
}
```

### Struct Attributes

```jocky
#[packed]
struct PackedData {
    a: i8,
    b: i32,
    c: i8,
}

#[repr(C)]
struct CCompatible {
    x: i32,
    y: i32,
}
```

### Enum Attributes

```jocky
#[derive(Debug)]
enum Color {
    Red = 0,
    Green = 1,
    Blue = 2,
}
```

---

## Runtime Features

### Anti-Analysis Detection

```jocky
fn main() -> void {
    ffi printf(fmt: string, ...) -> i32;
    
    // Check for debugger
    let has_debugger = jocky_is_debugger_present();
    if has_debugger {
        printf("Debugger detected!\n");
        return;
    }
    
    // Check for VM
    let in_vm = jocky_is_vm();
    if in_vm {
        printf("Running in VM!\n");
        return;
    }
    
    // Check for sandbox
    let in_sandbox = jocky_is_sandbox();
    if in_sandbox {
        printf("Sandbox detected!\n");
        return;
    }
    
    printf("Environment clean, proceeding...\n");
}
```

### Enhanced VM Detection

```jocky
fn main() -> void {
    ffi printf(fmt: string, ...) -> i32;
    
    if jocky_is_hyperv() {
        printf("Hyper-V detected\n");
    }
    if jocky_is_xen() {
        printf("Xen detected\n");
    }
    if jocky_is_kvm() {
        printf("KVM detected\n");
    }
    if jocky_is_vmware() {
        printf("VMware detected\n");
    }
}
```

### Anti-Disassembly

```jocky
fn main() -> void {
    // Initialize anti-disasm defenses
    jocky_anti_disasm_init();
    
    // Check if code has been instrumented
    if jocky_detect_static_analysis() {
        printf("Static analysis detected!\n");
        return;
    }
    
    // Check for Frida hooks
    if jocky_detect_frida_hooks() {
        printf("Frida detected!\n");
        return;
    }
}
```

### Memory Operations

```jocky
fn main() -> void {
    // Secure memory allocation
    let buffer: i8* = jocky_alloc(1024);
    
    // Do some work...
    
    // Secure deallocation (wiped)
    jocky_free(buffer);
}
```

### Cleanup & Artifacts

```jocky
fn main() -> void {
    // Do malicious work...
    
    // Clear logs before exit
    jocky_clear_logs();
    
    // Remove traces
    jocky_wipe_artifacts();
    
    // Clean SRUM database
    jocky_clear_srum();
    
    // Cleanup everything atomically
    jocky_cleanup_all();
}
```

---

## Building & Debugging

### Compilation Profiles

```bash
# No obfuscation (for debugging)
./jocky build program.jky -p none

# Light obfuscation (fast, minimal overhead)
./jocky build program.jky -p light

# Standard obfuscation (recommended)
./jocky build program.jky -p standard

# Aggressive obfuscation (strong protection, slower)
./jocky build program.jky -p aggressive

# Paranoid obfuscation (maximum protection)
./jocky build program.jky -p paranoid
```

### Keeping Intermediate Files

```bash
# Keep intermediate .ll, .s, .o files for inspection
./jocky build program.jky --keep-intermediates
```

### Cross-Compilation

```bash
# Compile for Windows from Linux
./jocky build program.jky --target windows

# Compile for Linux
./jocky build program.jky --target linux

# Compile for both
./jocky build program.jky --target both
```

### Verification Without Compilation

```bash
# Check for syntax errors
./jocky verify program.jky

# Get detailed info about the program
./jocky info program.jky
```

### Running Programs

```bash
# Build and execute immediately
./jocky run program.jky --profile standard

# Pass arguments to the program
./jocky run program.jky -- arg1 arg2 arg3
```

### Cleaning Build Artifacts

```bash
# Remove .jocky-build directories
./jocky clean

# Also remove cache
./jocky clean --all
```

---

## Advanced Examples

### Example: Fibonacci

```jocky
fn fib(n: i32) -> i32 {
    if n <= 1 {
        return n;
    } else {
        return fib(n - 1) + fib(n - 2);
    }
}

fn main() -> void {
    ffi printf(fmt: string, ...) -> i32;
    
    for i = 0; i < 20; i = i + 1 {
        let result = fib(i);
        printf("fib(%d) = %d\n", i, result);
    }
}
```

### Example: Sorting

```jocky
fn bubble_sort<T>(arr: T[10]) -> T[10] {
    for i = 0; i < 10; i = i + 1 {
        for j = 0; j < 10 - i - 1; j = j + 1 {
            if arr[j] > arr[j + 1] {
                // Swap
                let temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    return arr;
}

fn main() -> void {
    let numbers: i32[10] = [64, 34, 25, 12, 22, 11, 90, 88, 45, 50];
    let sorted = bubble_sort(numbers);
}
```

### Example: State Machine

```jocky
enum State {
    Idle = 0,
    Running = 1,
    Paused = 2,
    Stopped = 3,
}

fn next_state(current: State) -> State {
    match current {
        State::Idle => State::Running,
        State::Running => State::Paused,
        State::Paused => State::Running,
        State::Stopped => State::Idle,
    }
}
```

---

## Tips & Best Practices

### 1. Type Safety
- Always specify types for function parameters and return values
- Let the type checker catch errors at compile time, not runtime

### 2. Memory Management
- Use `jocky_alloc()` for secure memory
- Always pair `jocky_alloc()` with `jocky_free()`
- Use stack allocation for small, short-lived data

### 3. Generic Code
- Write generic functions for reusable logic
- Compiler generates specialized versions automatically (monomorphization)
- No runtime performance penalty

### 4. Closures & Captures
- Be explicit about captured variables
- Understand closure lifetime rules
- Use lambdas for simple, one-off operations

### 5. Obfuscation
- Use `--profile none` during development
- Switch to `--profile standard` or higher for distribution
- Test thoroughly with obfuscation enabled

### 6. Error Handling
- Check return values from FFI calls
- Use enums for recoverable errors (Result pattern)
- Return early from error conditions

---

**Examples Version:** 1.0  
**Last Updated:** 2026-09-29
