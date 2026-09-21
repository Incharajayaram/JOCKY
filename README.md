# JOCKY — Compiler & Obfuscation Pipeline

JOCKY is a language and compiler pipeline for the SIH 26148 problem statement. It compiles JOCKY source code into native executables (Linux ELF or Windows PE) with integrated LLVM/MLIR obfuscation passes.

## Quick start

### C++ compiler (recommended)

Build the C++ compiler once, then use it directly:

```bash
cd compiler
mkdir -p build && cd build
cmake ..
make -j$(nproc)
cd ../..

./compiler/build/jockyc examples/hello-world/main.jky -o hello
./compiler/build/jockyc examples/fib/main.jky -p standard -o fib
```

### Python frontend (prototyping / dev)

```bash
./jocky build examples/hello-world/main.jky --profile standard
./jocky run examples/fib/main.jky --profile standard
./jocky verify examples/hello-world/main.jky
```

## Tools

| Tool | Language | Purpose | Input |
|------|----------|---------|-------|
| `compiler/build/jockyc` | C++ | Native compiler (LLVM C++ API) | `.jky` files |
| `./jocky` | Python | Frontend + pipeline (click + rich) | `.jky` files |
| `./jockyc` | Python | C/C++ obfuscation wrapper | `.c` / `.cpp` files |

---

## `compiler/build/jockyc` — Native C++ Compiler

Links against LLVM's C++ API directly. No Python runtime needed.

### Build

```bash
cd compiler
mkdir -p build && cd build
cmake ..
make -j$(nproc)
```

CMake finds the LLVM obfuscation build automatically via the path in `compiler/CMakeLists.txt`. You can override it:

```bash
cmake .. -DLLVM_ROOT=/path/to/llvm-build
```

### Usage

```bash
./compiler/build/jockyc <input.jky> [options]

Options:
  -o <file>      Output executable name (default: a.out)
  -p <profile>   Obfuscation profile: none, light, standard, aggressive (default: standard)
  -k             Keep intermediate files
  -h             Show help
```

### Examples

```bash
# Build without obfuscation
./compiler/build/jockyc examples/hello-world/main.jky -o hello -p none

# Build with standard obfuscation
./compiler/build/jockyc examples/fib/main.jky -o fib -p standard

# Build with aggressive obfuscation
./compiler/build/jockyc examples/c-interop/main.jky -o greet -p aggressive
```

### Architecture

```
compiler/
  include/
    token.h      # Token definitions
    lexer.h      # Lexer interface
    ast.h        # AST node types
    parser.h     # Parser interface
    codegen.h    # LLVM IR generation (IRBuilder)
    pipeline.h   # Compilation pipeline
  src/
    main.cpp     # CLI entry point
    lexer.cpp    # Lexer implementation
    parser.cpp   # Recursive descent parser
    ast.cpp      # Type utilities
    codegen.cpp  # LLVM C++ API codegen
    pipeline.cpp # Build orchestration (opt/clang subprocesses)
```

---

## `./jocky` — Python Frontend

A beautiful CLI built with `click` and `rich`. Useful for rapid prototyping and verification.

### Usage

```bash
./jocky build <file.jky> [--profile PROFILE] [--output PATH] [--keep-intermediates]
./jocky run  <file.jky> [--profile PROFILE] [args...]
./jocky verify <file.jky>
./jocky info <file.jky>
./jocky list-profiles
./jocky list-passes
./jocky clean [--all]
```

### Build profiles

| Profile | Description |
|---------|-------------|
| `none` | Plain compilation, no obfuscation |
| `light` | Minimal obfuscation |
| `standard` | Balanced LLVM IR passes (boguscf, flattening, substitution, linear-mba, opaque-pred) |
| `aggressive` | Full LLVM + MLIR passes |
| `paranoid` | Everything + anti-debug |

---

## `./jockyc` — C/C++ Obfuscation Wrapper

Compiles C/C++ files through the LLVM obfuscation passes directly.

```bash
./jockyc input.c -o output
./jockyc input.c --bcf --fla --sub -o output
./jockyc input.c --windows -o output.exe
```

---

## JOCKY language

A small C-like language with FFI support:

```jocky
ffi printf(fmt: string, ...) -> i32;

fn fib(n: i32) -> i32 {
    if n <= 1 {
        return n;
    }
    return fib(n - 1) + fib(n - 2);
}

fn main() -> i32 {
    let i: i32 = 0;
    while i < 10 {
        let r: i32 = fib(i);
        printf("fib(%d) = %d\n", i, r);
        i = i + 1;
    }
    return 0;
}
```

### Syntax overview

- **FFI declarations:** `ffi name(params) -> type;` (variadic with `...`)
- **Functions:** `fn name(params) -> type { body }`
- **Variables:** `let name: type = expr;` or `let name = expr;`
- **Control flow:** `if`/`else`, `while`, `for`, `return`
- **Types:** `i8`, `i32`, `i64`, `bool`, `void`, `string`, `T*`
- **Operators:** `+ - * / % == != < > <= >= && || ! & *`
- **Comments:** `// line comment`

---

## Runtime Framework

Portable C runtime for anti-analysis, evasion, and execution:

```
src/runtime/
  include/jocky_rt.h    # Public API
  init/
    anti_analysis.c     # Debugger, VM, sandbox detection
  evasion/
    unhook.c            # Ntdll unhooking (Windows)
    syscalls.c          # Direct syscall framework (Windows)
  execution/
    hollow.c            # Process hollowing (Windows)
  cleanup/
    self_delete.c       # Self-deletion
    logs.c              # Log clearing
```

Build with CMake:

```bash
cd src/runtime
mkdir build && cd build
cmake ..
make
```

---

## Toolchain configuration

All tools discover the LLVM obfuscation toolchain automatically:

1. **`JOCKY_LLVM_TOOLCHAIN`** environment variable
2. **`jocky.yaml`** config file (`toolchain.llvm_dir`)
3. **PATH search** (clang + opt + mlir-opt)
4. **Common install locations**

```bash
export JOCKY_LLVM_TOOLCHAIN=/path/to/llvm-obfuscation-tools
```

For Windows cross-compilation with `jockyc`:
- `JOCKY_LLVM_MINGW` — llvm-mingw prefix
- `JOCKY_WINSDK_VCTOOLS` — MSVC VCTools
- `JOCKY_WINSDK_UM` — Windows SDK

---

## Structure

```
compiler/               # C++ compiler (LLVM C++ API)
  CMakeLists.txt
  include/              # Headers
  src/                  # Implementation
src/jocky/              # Python frontend
  language/             # Lexer, parser, AST, checker, codegen
  core/                 # Pipeline, stages, context, profiles
  stages/               # Parse, lower, obfuscate, link, pack
  passes/               # Pass registry and profile YAMLs
  cli.py                # Main CLI (click + rich)
src/runtime/            # C runtime framework
  include/
  init/
  evasion/
  execution/
  cleanup/
jocky                   # Python CLI launcher
jockyc                  # C/C++ obfuscation CLI (Python)
```
