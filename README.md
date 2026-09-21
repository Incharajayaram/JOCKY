# JOCKY — Compiler & Obfuscation Pipeline

JOCKY is a language and compiler pipeline for the SIH 26148 problem statement. It compiles JOCKY source code into native executables (Linux ELF or Windows PE) with integrated LLVM/MLIR obfuscation passes.

## Quick start

No pip install needed. Just run the tools directly from the repo root:

```bash
./jocky build examples/hello-world/main.jky --profile standard
./jocky run examples/fib/main.jky --profile standard
./jocky verify examples/hello-world/main.jky
```

## Tools

| Tool | Purpose | Input |
|------|---------|-------|
| `./jocky` | JOCKY language compiler | `.jky` files |
| `./jockyc` | C/C++ obfuscation wrapper | `.c` / `.cpp` files |

---

## `./jocky` — JOCKY language compiler

A beautiful CLI built with `click` and `rich`. It shows colored status messages, progress spinners, tables for listings, and panels for results.

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

### Examples

```bash
# Build with default (standard) obfuscation profile
./jocky build examples/hello-world/main.jky

# Build without obfuscation
./jocky build examples/fib/main.jky --profile none

# Build and run
./jocky run examples/fib/main.jky --profile standard

# Verify syntax and types only
./jocky verify examples/hello-world/main.jky

# List available profiles and passes (as formatted tables)
./jocky list-profiles
./jocky list-passes

# Clean all build artifacts
./jocky clean
```

### Build profiles

| Profile | Description |
|---------|-------------|
| `none` | Plain compilation, no obfuscation |
| `light` | Minimal obfuscation |
| `standard` | Balanced LLVM IR passes (boguscf, flattening, substitution, linear-mba, opaque-pred) |
| `aggressive` | Full LLVM + MLIR passes |
| `paranoid` | Everything + anti-debug |

### Pipeline stages

1. **parse** — Lex, parse, and type-check JOCKY source
2. **lower_ir** — Generate LLVM IR text
3. **mlir_obfuscate** — Run MLIR passes (string-encrypt, constant-obfuscate, etc.)
4. **ir_obfuscate** — Run LLVM IR obfuscation passes via `opt`
5. **link** — Compile to object and link executable
6. **pack** — Optional UPX packing

---

## `./jockyc` — C/C++ obfuscation wrapper

Compiles C/C++ files through the LLVM obfuscation passes directly, without the JOCKY language frontend.

```bash
./jockyc input.c -o output
./jockyc input.c --bcf --fla --sub -o output
./jockyc input.c --windows -o output.exe
```

### Pass flags

| Flag | Pass |
|------|------|
| `--bcf` | Bogus control flow |
| `--fla` | Control-flow flattening |
| `--sub` | Instruction substitution |
| `--split` | Basic-block splitting |
| `--mba` | Linear MBA |
| `--opaque` | Opaque predicates |
| `--indcall` | Indirect calls |
| `--pdata` | PData stripping (Windows) |
| `--antidebug` | Anti-debug |
| `--signature` | Signature stripping |
| `--virtualize` | Code virtualization |
| `--passes` | Comma-separated custom pass list |

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

## Toolchain configuration

JOCKY needs an LLVM/MLIR toolchain with the obfuscation plugins. It discovers the toolchain automatically via (in order of priority):

1. **`JOCKY_LLVM_TOOLCHAIN`** environment variable
2. **`jocky.yaml`** config file with a `toolchain.llvm_dir` key
3. **PATH search** — if `clang`, `opt`, and `mlir-opt` are all in the same `bin/` directory
4. **Common install locations** — searches `~/projects/llvm-obfuscation-tools-linux-x86_64`, `/usr/local/llvm-obfuscation`, `/opt/llvm-obfuscation`, etc.

### Quick setup with env var

```bash
export JOCKY_LLVM_TOOLCHAIN=/path/to/llvm-obfuscation-tools
./jocky build hello.jky
```

### Setup with config file

Create `jocky.yaml` in your project root or `~/.config/jocky/config.yaml`:

```yaml
toolchain:
  llvm_dir: /path/to/llvm-obfuscation-tools
```

### Windows cross-compilation (jockyc)

For Windows builds, `jockyc` also looks for:

- **`JOCKY_LLVM_MINGW`** — path to llvm-mingw prefix (e.g. `/opt/llvm-mingw`)
- **`JOCKY_WINSDK_VCTOOLS`** — path to MSVC VCTools
- **`JOCKY_WINSDK_UM`** — path to Windows SDK

Or pass them as CLI flags:

```bash
./jockyc input.c --windows --vctoolsdir /path/to/vctools --winsdkdir /path/to/sdk -o out.exe
```

---

## Structure

```
src/jocky/
  language/         # Lexer, parser, AST, type checker, LLVM IR codegen
  core/             # Pipeline, stages, context, profiles, toolchain discovery
  stages/           # Parse, lower, obfuscate, link, pack
  passes/           # Pass registry and profile YAMLs
  backends/         # LLVM IR, object, executable backends
  cli.py            # Main CLI entry point (click + rich)
jocky               # JOCKY language CLI launcher (sets PYTHONPATH)
jockyc              # C/C++ obfuscation CLI
```
