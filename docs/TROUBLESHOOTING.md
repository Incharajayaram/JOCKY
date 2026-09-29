# JOCKY Troubleshooting Guide

Solutions to common issues and errors encountered when using JOCKY.

---

## Table of Contents

1. [Compilation Errors](#compilation-errors)
2. [Runtime Issues](#runtime-issues)
3. [Performance Problems](#performance-problems)
4. [Platform-Specific Issues](#platform-specific-issues)
5. [Debugging Setup](#debugging-setup)
6. [Build System Issues](#build-system-issues)
7. [Obfuscation Problems](#obfuscation-problems)

---

## Compilation Errors

### Error: "Unknown type 'i32'"

**Symptom:**
```
Error E0001: Unknown type 'i32'
  at program.jky:1:10
```

**Cause:** JOCKY requires explicit type annotations in most contexts.

**Solution:**
```jocky
// Wrong
let x = 42;  // Type cannot be inferred globally

// Correct
let x: i32 = 42;

// Also correct (in function context)
fn add(a: i32, b: i32) -> i32 { a + b }
let result = add(1, 2);  // Type is inferred from function return type
```

---

### Error: "Function not found"

**Symptom:**
```
Error E0002: Unknown function 'printf'
  at program.jky:5:5
```

**Cause:** FFI functions must be declared before use.

**Solution:**
```jocky
fn main() -> void {
    // Declare before using
    ffi printf(fmt: string, ...) -> i32;
    
    printf("Hello\n");
}
```

---

### Error: "Type mismatch in function call"

**Symptom:**
```
Error E0003: Type mismatch
  at program.jky:10:15
  expected: i32, got: string
```

**Cause:** Function argument types don't match declaration.

**Solution:**
```jocky
fn add(a: i32, b: i32) -> i32 { a + b }

fn main() -> void {
    // Wrong: passing string to i32 parameter
    let x = add(5, "10");  // ERROR
    
    // Correct: pass i32 values
    let x = add(5, 10);    // OK
    
    // Or convert first
    ffi atoi(str: string) -> i32;
    let y = add(5, atoi("10"));  // OK
}
```

---

### Error: "Match statement not exhaustive"

**Symptom:**
```
Error E0004: Match pattern not exhaustive
  at program.jky:15:5
```

**Cause:** Pattern match must cover all possible values.

**Solution:**
```jocky
enum Status { Success = 0, Error = 1, Pending = 2 }

// Wrong: missing Pending case
match status {
    Status::Success => {},
    Status::Error => {},
    // Missing Status::Pending
}

// Correct: use wildcard
match status {
    Status::Success => {},
    Status::Error => {},
    _ => {},  // handles all other cases
}

// Correct: handle all explicitly
match status {
    Status::Success => {},
    Status::Error => {},
    Status::Pending => {},
}
```

---

### Error: "Undefined variable"

**Symptom:**
```
Error E0005: Undefined variable 'x'
  at program.jky:8:10
```

**Cause:** Variable used before declaration or out of scope.

**Solution:**
```jocky
fn main() -> void {
    // Wrong: using x before declaration
    let y = x + 1;  // ERROR
    let x: i32 = 5;
    
    // Correct: declare before use
    let x: i32 = 5;
    let y = x + 1;
}

// Correct: scope handling
{
    let x: i32 = 5;
}
// x is out of scope here
let z = x;  // ERROR
```

---

### Error: "LLVM compilation failed"

**Symptom:**
```
error: LLVM compilation failed
Details: llc exited with code 1
```

**Cause:** LLVM backend encountered invalid IR or unsupported operation.

**Solution:**
```bash
# Keep intermediate files to inspect the generated LLVM IR
./jocky build program.jky --keep-intermediates

# Check .ll file for invalid IR
cat .jocky-build/program/main.ll

# Reduce program to minimal example that reproduces the issue
# Report with both .jky source and .ll IR output
```

---

### Error: "Linker error: undefined reference"

**Symptom:**
```
/usr/bin/ld: undefined reference to 'some_symbol'
collect2: error: ld returned 1 exit status
```

**Cause:** Missing FFI declaration or incorrect function name.

**Solution:**
```jocky
fn main() -> void {
    // Wrong: function not declared
    some_c_function();  // Linker error
    
    // Correct: declare with exact name
    ffi some_c_function() -> void;
    some_c_function();  // OK
    
    // Check C library availability
    // Some functions require specific libraries to be linked
}
```

---

### Error: "Parse error" with unclear message

**Symptom:**
```
Parse error: unexpected token at line X
```

**Cause:** Syntax error in JOCKY code.

**Solution:**
1. Check line numbers around the error
2. Verify matching braces, parentheses, brackets
3. Ensure all statements end with semicolons (where required)
4. Check for missing closing quotes in strings

```jocky
// Wrong: missing semicolon
let x: i32 = 5
let y: i32 = 10;  // Parse error

// Wrong: unclosed brace
fn main() -> void {
    let x: i32 = 5;
// Missing }

// Wrong: unclosed string
ffi printf(fmt: string, ...) -> i32;
printf("Hello  // Missing closing quote
```

---

## Runtime Issues

### Program exits unexpectedly

**Symptom:** Program runs but exits without output or error message.

**Cause:** Often due to anti-analysis or integrity checks failing.

**Solution:**
```bash
# Compile without obfuscation to isolate issue
./jocky build program.jky -p none

# Check for anti-analysis checks in runtime
# By default, jocky_runtime_init() runs checks that may exit

# To debug, compile without runtime init
# (not recommended for production)
```

---

### "Integrity check failed" error

**Symptom:** Binary exits with code `0xDEAD1337`

**Cause:** Binary was modified after packing, or CRC32 check failed.

**Solution:**
```bash
# Don't modify the binary after building
# If using packing, don't apply additional modifications

# For debugging, compile without anti-tamper
./jocky build program.jky -p light

# Verify binary hasn't been corrupted
md5sum program
# Should match if binary is intact
```

---

### Debugger detection false positive

**Symptom:** Program exits even when not being debugged.

**Cause:** Overly aggressive anti-analysis checks.

**Solution:**
```jocky
fn main() -> void {
    // Check what's being detected
    let threats = jocky_runtime_init();
    
    // Instead of exiting immediately, handle gracefully
    if (threats & 1) != 0 {
        ffi printf(fmt: string, ...) -> i32;
        printf("Debugger detected\n");
        // Don't exit, just log and continue
    }
}

// Or bypass during development
./jocky build program.jky -p none
```

---

### Memory access violation / Segmentation fault

**Symptom:**
```
Segmentation fault (core dumped)
```

**Cause:** Pointer dereference of null or invalid memory.

**Solution:**
```jocky
fn main() -> void {
    let ptr: i32* = &10;  // Pointer to stack var (valid)
    let value = *ptr;     // OK
    
    // Wrong: null pointer dereference
    let null_ptr: i32* = 0;  // NULL
    let bad = *null_ptr;      // Segfault
    
    // Correct: check before dereferencing
    if ptr != 0 {
        let value = *ptr;  // Safe
    }
}
```

---

### Array bounds error

**Symptom:** Unexpected behavior or crash when accessing array element.

**Cause:** Index out of bounds.

**Solution:**
```jocky
fn main() -> void {
    let arr: i32[5] = [1, 2, 3, 4, 5];
    
    // Wrong: index out of bounds
    let value = arr[10];  // No compile-time check
    
    // Correct: check bounds at runtime
    let index: i32 = 10;
    if index >= 0 && index < 5 {
        let value = arr[index];
    } else {
        ffi printf(fmt: string, ...) -> i32;
        printf("Index out of bounds\n");
    }
}
```

---

## Performance Problems

### Compilation takes too long

**Symptom:** `jocky build` takes many seconds/minutes.

**Cause:** Obfuscation passes are expensive, LLVM optimization takes time.

**Solution:**
```bash
# Use lighter obfuscation during development
./jocky build program.jky -p light

# Use parallel compilation if available
export LLVM_THREAD_COUNT=8
./jocky build program.jky -p standard

# Keep intermediates to avoid recompiling
./jocky build program.jky --keep-intermediates

# For fast iteration, use --profile none
./jocky build program.jky -p none

# Check build times with verbose output
VERBOSE=1 ./jocky build program.jky
```

---

### Program runs slowly

**Symptom:** Obfuscated binary is significantly slower than non-obfuscated version.

**Cause:** Obfuscation overhead, especially with aggressive profiles.

**Solution:**
```bash
# Profile different obfuscation levels
time ./jocky build program.jky -p none
time ./jocky build program.jky -p light
time ./jocky build program.jky -p standard

# Some passes are more expensive than others
# Use lighter profiles if performance is critical

# Identify hot paths and reduce their obfuscation
# Use #[no_obfuscate] attribute if implemented
```

---

### Large binary size

**Symptom:** Compiled binary is unexpectedly large.

**Cause:** Obfuscation adds instructions, debugging symbols, unused code.

**Solution:**
```bash
# Strip debugging symbols
strip a.out

# Use UPX packing
upx -9 a.out

# Check what's in the binary
nm a.out | wc -l  # Number of symbols

# Obfuscation with duplication increases size
# Use lighter profiles if size is critical
./jocky build program.jky -p light

# Remove unnecessary functions/data
# Compile with -p none first to measure baseline
```

---

## Platform-Specific Issues

### Windows (PE)

#### Error: "Cannot find MSVC compiler"

**Symptom:**
```
Error: MSVC compiler not found
```

**Cause:** Visual C++ toolchain not installed.

**Solution:**
```bash
# Install Visual Studio with C++ tools
# Or install MinGW-w64 for cross-compilation

# On Linux, use mingw-w64 for Windows PE compilation
sudo apt-get install mingw-w64

# Set target explicitly
./jocky build program.jky --target windows
```

#### Driver loading fails with "Access denied"

**Symptom:**
```
Error: Failed to load driver - Access denied
```

**Cause:** Running without administrator privileges.

**Solution:**
```bash
# Run with administrator privileges
sudo ./a.out

# Or use runas on Windows
runas /user:Administrator a.exe
```

#### Anti-analysis gives false positives

**Symptom:**
```
Program exits due to debugger/VM/sandbox detection
```

**Cause:** System legitimately triggers detection heuristics.

**Solution:**
```jocky
fn main() -> void {
    // Check specific conditions instead of exiting
    if jocky_is_debugger_present() {
        // Handle gracefully instead of exiting
        ffi printf(fmt: string, ...) -> i32;
        printf("Debug mode\n");
    }
    
    // Continue execution
}
```

---

### Linux (ELF)

#### Binary fails to run: "No such file or directory"

**Symptom:**
```
./a.out: No such file or directory
```

**Cause:** ELF interpreter or library not found (usually /lib64/ld-linux-x86-64.so.2).

**Solution:**
```bash
# Check dependencies
ldd ./a.out

# Install missing dependencies
sudo apt-get install libc6 libc6-dev

# Use static linking if possible
# (Not currently supported in JOCKY, but can be added)
```

#### Kernel exploitation fails

**Symptom:**
```
kernel read/write failed
```

**Cause:** `/proc/kcore` not accessible or kernel too new.

**Solution:**
```bash
# Check kernel version
uname -r

# Some newer kernels restrict /proc/kcore access
# Try with elevated privileges
sudo ./a.out

# Or disable ASLR temporarily
echo 0 | sudo tee /proc/sys/kernel/randomize_va_space
```

---

## Debugging Setup

### GDB Integration

```bash
# Compile with debugging info
./jocky build program.jky --keep-intermediates

# Run with gdb
gdb ./a.out

# Set breakpoint
(gdb) break main

# Run
(gdb) run

# Step
(gdb) step

# Continue
(gdb) continue

# Print variable
(gdb) print x

# Disassemble
(gdb) disas main
```

### LLDB Integration (macOS/Linux)

```bash
lldb ./a.out
(lldb) breakpoint set --name main
(lldb) run
(lldb) step
(lldb) frame variable
```

### Inspect Generated LLVM IR

```bash
# Keep intermediate files
./jocky build program.jky --keep-intermediates

# View LLVM IR
cat .jocky-build/program/main.ll

# View optimized IR
cat .jocky-build/program/optimized.ll

# View obfuscated IR
cat .jocky-build/program/obfuscated.ll

# View assembly
cat .jocky-build/program/main.s

# Disassemble binary
objdump -d a.out | less
```

### Runtime Debugging

```jocky
fn main() -> void {
    ffi printf(fmt: string, ...) -> i32;
    
    // Add debug output
    printf("[DEBUG] Starting main\n");
    
    let x: i32 = 42;
    printf("[DEBUG] x = %d\n", x);
    
    if x > 40 {
        printf("[DEBUG] x is large\n");
    }
}
```

---

## Build System Issues

### CMake not finding LLVM

**Symptom:**
```
CMake Error: Could not find LLVM
```

**Cause:** LLVM installation not in standard location.

**Solution:**
```bash
# Find your LLVM installation
llvm-config --prefix
# Output: /usr/lib/llvm-14

# Set it when running cmake
cd compiler/build
cmake .. -DLLVM_ROOT=/usr/lib/llvm-14
make -j$(nproc)
```

---

### Python module import errors

**Symptom:**
```
ModuleNotFoundError: No module named 'jocky'
```

**Cause:** Python path not set or module not installed.

**Solution:**
```bash
# Install in development mode
pip install -e .

# Or set PYTHONPATH
export PYTHONPATH=/path/to/JOCKY:$PYTHONPATH

# Verify installation
python -c "import jocky; print(jocky.__version__)"
```

---

### Cache invalidation issues

**Symptom:** Changes don't seem to take effect after rebuild.

**Cause:** Stale cache entries.

**Solution:**
```bash
# Clear cache
./jocky clean --all

# Force rebuild
./jocky build program.jky --force

# Check cache location
ls -la .jocky-cache/
```

---

## Obfuscation Problems

### Obfuscated binary doesn't run

**Symptom:** Builds successfully with `--profile none` but fails with obfuscation.

**Cause:** Bug in obfuscation pass or incompatible transformation.

**Solution:**
```bash
# Try progressively higher profiles to isolate issue
./jocky build program.jky -p none    # Works?
./jocky build program.jky -p light   # Works?
./jocky build program.jky -p standard # Fails?

# The failing profile tells you which pass has the bug

# Use lighter profile as workaround
./jocky build program.jky -p light

# Report with both programs:
# - Working: -p light
# - Not working: -p standard
```

---

### String encryption breaks program

**Symptom:** Program runs but strings are garbled or missing.

**Cause:** String decryption failed at runtime.

**Solution:**
```bash
# Disable string encryption temporarily
./jocky build program.jky -p light

# String encryption happens in pass selection
# For debugging, inspect what profile includes

./jocky list-profiles  # Shows which passes per profile

# If using custom profile, check string-encrypt pass is configured
```

---

### Control flow flattening breaks logic

**Symptom:** Complex control flow logic produces wrong results after obfuscation.

**Cause:** Flattening pass has a bug or incompatibility.

**Solution:**
```bash
# Try without flattening
./jocky build program.jky -p light

# Inspect obfuscated IR to see what went wrong
./jocky build program.jky -p standard --keep-intermediates
cat .jocky-build/program/obfuscated.ll

# Report the issue with clear minimal example
# Include both source (.jky) and obfuscated IR (.ll)
```

---

## Getting Help

### Resources

1. **GitHub Issues** – Report bugs at https://github.com/JOCKY/JOCKY/issues
2. **Documentation** – Check docs/ folder for detailed guides
3. **Examples** – Study examples/ folder for working code
4. **Runtime API** – See src/runtime/RUNTIME_API.md for API reference

### Providing Good Bug Reports

When reporting issues, include:

1. **Minimal reproducible example** – Smallest code that reproduces the issue
2. **Expected vs. actual behavior** – What should happen vs. what does
3. **Environment** – OS, compiler version, LLVM version
4. **Reproduction steps** – Exact commands to reproduce
5. **Error messages** – Full error output with traceback if applicable
6. **Build artifacts** – .ll files if compilation-related

Example report:
```
**Title:** Segmentation fault with nested closures

**Environment:**
- OS: Ubuntu 20.04
- JOCKY version: 1.0
- LLVM version: 14

**Minimal example:** (program.jky included)

**Steps to reproduce:**
1. ./jocky build program.jky -p standard
2. ./a.out
3. Segmentation fault

**Expected behavior:** Program should print numbers 1-10

**Actual behavior:** Segmentation fault after printing 5

**Error output:**
Segmentation fault (core dumped)
```

---

**Troubleshooting Guide Version:** 1.0  
**Last Updated:** 2026-09-29
