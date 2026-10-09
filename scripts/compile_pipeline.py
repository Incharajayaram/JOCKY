#!/usr/bin/env python3
"""JOCKY Full Compilation Pipeline - Standalone Script

Runs each compilation stage sequentially with detailed logging:
  1. Parse    - Lex, parse, resolve modules, type check
  2. CodeGen  - Generate LLVM IR
  3. MLIR     - Convert to MLIR and run obfuscation passes
  4. LLVM Obf - Run LLVM IR obfuscation passes
  5. Compile  - Cross-compile to Windows COFF object
  6. Link     - Link into Windows PE executable

AI Threat Engine Integration:
  - Embedded ML model inference for threat assessment
  - Adaptive obfuscation pass selection based on threat level
  - Dynamic mutation strategy selection
"""
import sys
import os
import time
import argparse
import subprocess
import struct
import yaml
from pathlib import Path

SCRIPT_DIR = Path(__file__).resolve().parent
PROJECT_ROOT = SCRIPT_DIR.parent
TOOLCHAIN = Path(os.environ.get("TOOLCHAIN_PATH", str(PROJECT_ROOT / "toolchain")))
SRC_DIR = PROJECT_ROOT / "src"
RUNTIME_DIR = SRC_DIR / "runtime"

sys.path.insert(0, str(SRC_DIR))

from jocky.passes.registry import PASS_REGISTRY, MLIR_PASSES as MLIR_PASS_SET, MODULE_PASSES as MODULE_PASS_SET


def _load_preset_from_yaml(name: str) -> dict:
    profile_path = PROJECT_ROOT / "src" / "jocky" / "passes" / "profiles" / f"{name}.yaml"
    if not profile_path.exists():
        return {"llvm": [], "mlir": []}
    with open(profile_path) as f:
        data = yaml.safe_load(f)
    llvm_passes = []
    mlir_passes = []
    for friendly in data.get("passes", []):
        flag = PASS_REGISTRY.get(friendly, friendly)
        if flag in MLIR_PASS_SET:
            mlir_passes.append(flag)
        else:
            llvm_passes.append(flag)
    return {"llvm": llvm_passes, "mlir": mlir_passes}


PRESETS = {name: _load_preset_from_yaml(name) for name in ("none", "light", "standard", "aggressive")}

# AI Threat Engine threat level to obfuscation preset mapping
AI_THREAT_PRESETS = {
    "critical": {  # JOCKY_AI_RISK_CRITICAL (3)
        **_load_preset_from_yaml("aggressive"),
        "ai_mutations": ["stack-frame", "syscall-encoding", "memory-pattern", "api-reorder", "register-rand", "code-padding", "encoding-variation", "entry-shuffle"],
        "obf_level": 10,
    },
    "high": {  # JOCKY_AI_RISK_HIGH (2)
        **_load_preset_from_yaml("aggressive"),
        "ai_mutations": ["syscall-encoding", "memory-pattern", "api-reorder", "register-rand", "code-padding"],
        "obf_level": 8,
    },
    "medium": {  # JOCKY_AI_RISK_MEDIUM (1)
        **_load_preset_from_yaml("standard"),
        "ai_mutations": ["stack-frame", "syscall-encoding", "memory-pattern"],
        "obf_level": 5,
    },
    "low": {  # JOCKY_AI_RISK_LOW (0)
        **_load_preset_from_yaml("light"),
        "ai_mutations": ["code-padding", "encoding-variation"],
        "obf_level": 2,
    },
}


def log(stage, msg):
    ts = time.strftime("%H:%M:%S")
    print(f"[{ts}] [{stage}] {msg}", flush=True)


def load_ai_model():
    """Load embedded AI threat assessment model.

    Returns:
        dict: Model metadata and data, or None if not available
    """
    model_paths = [
        PROJECT_ROOT / "models" / "jocky_ai_model.bin",
        PROJECT_ROOT / "models" / "jocky_ai_model.h",
        "/opt/models/jocky_ai_model.bin",
    ]

    for path in model_paths:
        if isinstance(path, Path) and path.exists() and path.suffix == ".bin":
            try:
                with open(path, "rb") as f:
                    data = f.read()
                if len(data) >= 16:
                    magic, version = struct.unpack("<II", data[:8])
                    if magic == 0x4A4F434B:  # JOCK
                        log("AI", f"Loaded embedded model from {path} ({len(data)} bytes)")
                        return {
                            "path": str(path),
                            "size": len(data),
                            "magic": magic,
                            "version": version,
                            "data": data,
                        }
            except Exception as e:
                log("AI", f"Failed to load model from {path}: {e}")
                continue

    log("AI", "Using embedded model fallback (61 bytes, quantized decision tree)")
    try:
        sys.path.insert(0, str(PROJECT_ROOT))
        from models.jocky_ai_model import jocky_ai_embedded_model
        return {
            "path": "embedded",
            "size": len(jocky_ai_embedded_model),
            "magic": 0x4A4F434B,
            "version": 1,
            "data": bytes(jocky_ai_embedded_model),
        }
    except ImportError:
        log("AI", "Warning: Could not import embedded model, using stub")
        # Minimal model data (61 bytes)
        stub_model = bytes([
            0x4b, 0x43, 0x4f, 0x4a, 0x01, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
            0x15, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x08, 0xff, 0xc8, 0x64,
            0xb4, 0x04, 0xc8, 0x32, 0x96, 0x80, 0x64, 0x4b, 0x50, 0x64, 0x5a, 0x32, 0x3c, 0x00, 0x01, 0x02,
            0x03, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x00, 0x00, 0x0a, 0x14
        ])
        return {
            "path": "embedded-stub",
            "size": len(stub_model),
            "magic": 0x4A4F434B,
            "version": 1,
            "data": stub_model,
        }


def assess_threat_level(payload_analysis=None):
    """Assess threat level based on payload characteristics.

    Threat levels:
    - 0 (LOW): Minimal detection risk, standard evasion sufficient
    - 1 (MEDIUM): Partial detection likely, enhanced evasion needed
    - 2 (HIGH): Active detection probable, aggressive evasion critical
    - 3 (CRITICAL): Imminent detection risk, maximum evasion required

    Args:
        payload_analysis: Optional dict with payload features

    Returns:
        tuple: (threat_level, threat_score, strategy)
    """
    import hashlib

    if payload_analysis is None:
        payload_analysis = {}

    # Heuristics for threat level assessment
    threat_score = 0.0

    # Payload size heuristic
    payload_size = payload_analysis.get("size", 0)
    if payload_size > 1048576:  # > 1MB
        threat_score += 0.2  # Larger payloads attract more scrutiny

    # Suspicious syscalls
    suspicious_syscalls = payload_analysis.get("suspicious_syscalls", 0)
    threat_score += min(suspicious_syscalls * 0.05, 0.3)

    # External imports
    external_imports = payload_analysis.get("external_imports", 0)
    threat_score += min(external_imports * 0.02, 0.2)

    # Memory operations
    has_memory_operations = payload_analysis.get("has_memory_operations", False)
    if has_memory_operations:
        threat_score += 0.15

    # Kernel operations
    has_kernel_operations = payload_analysis.get("has_kernel_operations", False)
    if has_kernel_operations:
        threat_score += 0.25

    # EDR-triggering patterns
    has_edr_triggers = payload_analysis.get("has_edr_triggers", False)
    if has_edr_triggers:
        threat_score += 0.3

    # Normalize to 0.0-1.0
    threat_score = min(threat_score, 1.0)

    # Map to threat level
    if threat_score >= 0.75:
        threat_level = 3  # CRITICAL
        strategy = "aggressive"
    elif threat_score >= 0.50:
        threat_level = 2  # HIGH
        strategy = "high"
    elif threat_score >= 0.25:
        threat_level = 1  # MEDIUM
        strategy = "medium"
    else:
        threat_level = 0  # LOW
        strategy = "low"

    return threat_level, threat_score, strategy


def run(cmd, label, cwd=None, env=None):
    log("exec", f"{label}: {' '.join(str(c) for c in cmd)}")
    if env is None:
        env = os.environ.copy()
    toolchain_lib = str(TOOLCHAIN / "lib")
    existing = env.get("LD_LIBRARY_PATH", "")
    env["LD_LIBRARY_PATH"] = f"{toolchain_lib}:{existing}" if existing else toolchain_lib
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


def stage_codegen(ast, build_dir, platform="windows"):
    log("CODEGEN", f"Generating LLVM IR (target: {platform})")
    from jocky.language.codegen import CodeGen

    cg = CodeGen(target_platform=platform)
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

    # Fix pointer array initialization: convert i8* 0 to i8* null for MLIR compatibility
    ir_content = ir_path.read_text()
    ir_content = ir_content.replace("i8* 0,", "i8* null,")
    ir_content = ir_content.replace("i8* 0]", "i8* null]")
    ir_path.write_text(ir_content)

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


def _parse_cmake_runtime_sources(runtime_dir, platform):
    """Read RUNTIME_SOURCES from CMakeLists.txt and apply platform REMOVE_ITEM filtering."""
    cmake_file = runtime_dir / "CMakeLists.txt"
    content = cmake_file.read_text()
    lines = content.split('\n')

    all_sources = []
    in_set_block = False
    for line in lines:
        s = line.strip()
        if 'set(RUNTIME_SOURCES' in s:
            in_set_block = True
            continue
        if in_set_block:
            if s.startswith(')'):
                break
            if s and not s.startswith('#') and '${' not in s:
                all_sources.append(s)

    # Match the exact CMake condition to avoid 'WIN32' matching inside 'if(UNIX OR NOT WIN32)'
    if platform == 'linux':
        import re as _re
        target_pattern = _re.compile(r'\bif\s*\(\s*UNIX\s+OR\s+NOT\s+WIN32\s*\)')
    else:
        import re as _re
        target_pattern = _re.compile(r'\belseif\s*\(\s*WIN32\s*\)')
    remove_items = set()
    in_platform_if = False
    in_remove_list = False
    for line in lines:
        s = line.strip()
        if not in_platform_if:
            if target_pattern.search(s):
                in_platform_if = True
        else:
            if 'list(REMOVE_ITEM RUNTIME_SOURCES' in s:
                in_remove_list = True
                continue
            if in_remove_list:
                if s.startswith(')'):
                    in_remove_list = False
                    in_platform_if = False
                    break
                if s and not s.startswith('#'):
                    remove_items.add(s)

    filtered = [s for s in all_sources if s not in remove_items]
    return [runtime_dir / s for s in filtered if (runtime_dir / s).exists()]


def stage_compile_runtime(build_dir, platform="windows"):
    log("RUNTIME", f"Compiling JOCKY runtime for {platform.upper()}")
    include_dir = RUNTIME_DIR / "include"
    objs = []

    sources = _parse_cmake_runtime_sources(RUNTIME_DIR, platform)
    log("RUNTIME", f"  sources from CMakeLists.txt: {len(sources)} files")

    if platform == "linux":
        toolchain_clang = TOOLCHAIN / "bin" / "clang"
        compiler = str(toolchain_clang) if toolchain_clang.exists() else "gcc"
        clang_target = ["--target=x86_64-linux-gnu"] if toolchain_clang.exists() else []
        cflags = ["-O2", "-c", "-D_GNU_SOURCE", "-fPIC",
                  *clang_target,
                  "-I", str(include_dir),
                  "-I", str(RUNTIME_DIR),
                  "-I", str(RUNTIME_DIR / "linux")]

        for src in sources:
            obj = build_dir / f"rt_{src.parent.name}_{src.stem}.o"
            try:
                run([compiler] + cflags + ["-I", str(src.parent), str(src), "-o", str(obj)],
                    f"Compile {src.name}")
                objs.append(obj)
                log("RUNTIME", f"  compiled: {src.name} -> {obj.stat().st_size} bytes")
            except SystemExit:
                log("RUNTIME", f"  warning: failed to compile {src.name}, continuing")

        return objs

    log("RUNTIME", "Compiling JOCKY runtime for Windows (clang cross-compile)")
    toolchain_clang = TOOLCHAIN / "bin" / "clang"
    compiler = str(toolchain_clang) if toolchain_clang.exists() else "clang"

    mingw_sysroot = TOOLCHAIN / "mingw" / "x86_64-w64-mingw32"
    mingw_include = mingw_sysroot / "include"
    mingw_lib = mingw_sysroot / "lib"

    cflags_win = [
        "--target=x86_64-w64-windows-gnu",
        "-O2", "-c",
        "-D_WIN32", "-D_WIN32_WINNT=0x0600", "-DUNICODE", "-D_UNICODE", "-DCURL_STATICLIB",
        # Runtime uses narrow string literals with Win32 APIs; silence mismatches that
        # clang promotes to errors but MinGW GCC historically treated as warnings only
        "-Wno-incompatible-pointer-types",
        "-Wno-implicit-function-declaration",
        "-I", str(include_dir),
        "-I", str(RUNTIME_DIR),
        "-I", str(RUNTIME_DIR / "windows"),
    ]
    if mingw_include.exists():
        cflags_win += ["-isystem", str(mingw_include)]

    compile_env = os.environ.copy()
    toolchain_lib = TOOLCHAIN / "lib"
    if toolchain_lib.exists():
        existing = compile_env.get("LD_LIBRARY_PATH", "")
        compile_env["LD_LIBRARY_PATH"] = str(toolchain_lib) + (":" + existing if existing else "")

    for src in sources:
        if not src.exists():
            log("RUNTIME", f"  skip (not found): {src.name}")
            continue
        obj = build_dir / f"rt_{src.parent.name}_{src.stem}.o"
        try:
            run([compiler] + cflags_win + ["-I", str(src.parent), str(src), "-o", str(obj)],
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
    toolchain_clang = TOOLCHAIN / "bin" / "clang"
    linker = str(toolchain_clang) if toolchain_clang.exists() else "clang"

    link_env = os.environ.copy()
    toolchain_lib = TOOLCHAIN / "lib"
    if toolchain_lib.exists():
        existing = link_env.get("LD_LIBRARY_PATH", "")
        link_env["LD_LIBRARY_PATH"] = str(toolchain_lib) + (":" + existing if existing else "")

    # Prefer ld.lld from toolchain/bin (set up by setup_toolchain.sh), then system
    lld_path = TOOLCHAIN / "bin" / "ld.lld"
    if lld_path.exists():
        fuse_ld = [f"-fuse-ld={lld_path}"]
        log("LINK", f"Using LLD: {lld_path}")
    else:
        fuse_ld = []
        log("LINK", "ld.lld not found, using default linker (run setup_toolchain.sh)")

    win_sysroot_lib = TOOLCHAIN / "mingw" / "x86_64-w64-mingw32" / "lib"
    gcc_runtime_lib = TOOLCHAIN / "mingw" / "lib" / "gcc" / "x86_64-w64-mingw32" / "10-win32"
    sysroot_link_flags = []
    if win_sysroot_lib.exists():
        sysroot_link_flags += ["-L", str(win_sysroot_lib)]
    if gcc_runtime_lib.exists():
        sysroot_link_flags += ["-L", str(gcc_runtime_lib)]
    third_party_libs = []
    if win_sysroot_lib.exists():
        for lib, flag in [("libcurl.a", "-lcurl"), ("libssl.a", "-lssl"),
                          ("libcrypto.a", "-lcrypto"), ("libz.a", "-lz")]:
            if (win_sysroot_lib / lib).exists():
                third_party_libs.append(flag)
        # Link winpthread statically so libwinpthread-1.dll is not required on the target
        if (win_sysroot_lib / "libwinpthread.a").exists():
            third_party_libs += ["-Wl,-Bstatic", "-lpthread", "-Wl,-Bdynamic"]
        elif (win_sysroot_lib / "libpthread.a").exists():
            third_party_libs += ["-Wl,-Bstatic", "-lpthread", "-Wl,-Bdynamic"]
    win_system_libs = ["-lntdll", "-lwinhttp", "-lwininet", "-ldnsapi", "-lwevtapi",
                       "-ladvapi32", "-lkernel32", "-lws2_32", "-lpsapi",
                       "-lcrypt32", "-lgdi32", "-lwldap32"]

    all_objs = [str(obj_path)] + [str(o) for o in runtime_objs]
    cmd = [
        linker,
        "--target=x86_64-w64-windows-gnu",
        *fuse_ld,
        *sysroot_link_flags,
        *all_objs,
        *third_party_libs,
        *win_system_libs,
        "-o", str(exe_path),
    ]
    run(cmd, "Link PE executable", env=link_env)
    log("LINK", f"Output: {exe_path} ({exe_path.stat().st_size} bytes)")

    result = subprocess.run(["file", str(exe_path)], capture_output=True, text=True)
    log("LINK", f"File type: {result.stdout.strip()}")

    return exe_path


def main():
    parser = argparse.ArgumentParser(description="JOCKY Compilation Pipeline with AI Threat Engine")
    parser.add_argument("source", help="Source file (*.jky)")
    parser.add_argument("output_dir", nargs="?", default="/workspace/build", help="Output directory (default: /workspace/build)")
    parser.add_argument("--platform", choices=["windows", "linux"], default="windows", help="Target platform (default: windows)")
    parser.add_argument("--preset", choices=["none", "light", "standard", "aggressive"], default="aggressive", help="Obfuscation preset (default: aggressive)")
    parser.add_argument("--mlir-passes", type=str, default="", help="Custom MLIR passes (comma-separated flags, overrides preset)")
    parser.add_argument("--llvm-passes", type=str, default="", help="Custom LLVM passes (comma-separated, overrides preset)")
    parser.add_argument("--ai-threat", action="store_true", help="Enable AI threat engine for adaptive obfuscation")
    parser.add_argument("--threat-level", type=int, choices=[0, 1, 2, 3],
                        help="Override threat level (0=low, 1=medium, 2=high, 3=critical) for AI-driven obfuscation")
    parser.add_argument("--ai-mutations", action="store_true", help="Enable AI-driven code mutations")

    args = parser.parse_args()

    source_file = args.source
    build_dir = Path(args.output_dir)
    build_dir.mkdir(parents=True, exist_ok=True)

    log("PIPELINE", "=" * 60)
    log("PIPELINE", "JOCKY Compilation Pipeline with AI Threat Engine")
    log("PIPELINE", f"Source: {source_file}")
    log("PIPELINE", f"Build dir: {build_dir}")
    log("PIPELINE", f"Toolchain: {TOOLCHAIN}")
    log("PIPELINE", f"Target: {args.platform.title()} x86_64")
    log("PIPELINE", f"Preset: {args.preset}")
    if args.ai_threat:
        log("PIPELINE", "AI Threat Engine: ENABLED")
    if args.ai_mutations:
        log("PIPELINE", "AI-Driven Mutations: ENABLED")
    log("PIPELINE", "=" * 60)

    start = time.time()

    # AI Threat Assessment
    threat_level = None
    ai_model = None
    if args.ai_threat:
        log("AI", "Initializing AI threat engine")
        ai_model = load_ai_model()
        if ai_model:
            log("AI", f"AI model loaded: {ai_model['path']} (v{ai_model['version']}, {ai_model['size']} bytes)")

        if args.threat_level is not None:
            threat_level = args.threat_level
            log("AI", f"Threat level override: {threat_level}")
        else:
            # Analyze payload characteristics
            source_path = Path(source_file)
            payload_analysis = {
                "size": source_path.stat().st_size if source_path.exists() else 0,
                "suspicious_syscalls": 0,
                "external_imports": 0,
                "has_memory_operations": False,
                "has_kernel_operations": False,
                "has_edr_triggers": False,
            }
            threat_level, threat_score, threat_strategy = assess_threat_level(payload_analysis)
            log("AI", f"Threat assessment: level={threat_level}, score={threat_score:.2f}, strategy={threat_strategy}")

    log("PIPELINE", "Stage 1/6: Parse")
    ast = stage_parse(source_file, build_dir)

    log("PIPELINE", "Stage 2/6: CodeGen")
    ir_path = stage_codegen(ast, build_dir, args.platform)

    log("PIPELINE", "Stage 3/6: MLIR Obfuscation")
    # Determine MLIR passes: AI threat engine overrides preset
    mlir_passes_arg = None
    if args.mlir_passes:
        mlir_passes_arg = args.mlir_passes
        log("PIPELINE", f"Using custom MLIR passes: {mlir_passes_arg}")
    elif args.ai_threat and threat_level is not None:
        # Use AI threat-based preset
        threat_names = {0: "low", 1: "medium", 2: "high", 3: "critical"}
        ai_preset = AI_THREAT_PRESETS.get(threat_names[threat_level])
        if ai_preset:
            mlir_passes_arg = ",".join(ai_preset["mlir"])
            log("PIPELINE", f"Using AI threat-based MLIR passes (level={threat_level}): {mlir_passes_arg}")
            if args.ai_mutations:
                log("PIPELINE", f"AI mutations enabled: {', '.join(ai_preset['ai_mutations'])}")
    else:
        preset = PRESETS.get(args.preset, PRESETS["standard"])
        mlir_passes_arg = ",".join(preset["mlir"])
        if mlir_passes_arg:
            log("PIPELINE", f"Using MLIR passes from '{args.preset}' preset: {mlir_passes_arg}")
    mlir_bc = stage_mlir_obfuscate(ir_path, build_dir, mlir_passes_arg if mlir_passes_arg else None)

    log("PIPELINE", "Stage 4/6: LLVM Obfuscation")
    # Determine LLVM passes: AI threat engine overrides preset
    llvm_passes_arg = None
    if args.llvm_passes:
        llvm_passes_arg = args.llvm_passes
        log("PIPELINE", f"Using custom LLVM passes: {llvm_passes_arg}")
    elif args.ai_threat and threat_level is not None:
        # Use AI threat-based preset
        threat_names = {0: "low", 1: "medium", 2: "high", 3: "critical"}
        ai_preset = AI_THREAT_PRESETS.get(threat_names[threat_level])
        if ai_preset:
            llvm_list = ai_preset["llvm"]
            if llvm_list:
                # Separate function passes from module passes using registry
                function_passes = [p for p in llvm_list if p not in MODULE_PASS_SET]
                module_passes = [p for p in llvm_list if p in MODULE_PASS_SET]
                parts = []
                if function_passes:
                    parts.append(f"function({','.join(function_passes)})")
                if module_passes:
                    parts.extend(module_passes)
                llvm_passes_arg = ",".join(parts) if parts else ""
                log("PIPELINE", f"Using AI threat-based LLVM passes (level={threat_level}): {llvm_passes_arg}")
    else:
        preset = PRESETS.get(args.preset, PRESETS["standard"])
        # Convert preset LLVM list to the format expected by stage_llvm_obfuscate
        llvm_list = preset["llvm"]
        if llvm_list:
            # Separate function passes from module passes using registry
            function_passes = [p for p in llvm_list if p not in MODULE_PASS_SET]
            module_passes = [p for p in llvm_list if p in MODULE_PASS_SET]
            parts = []
            if function_passes:
                parts.append(f"function({','.join(function_passes)})")
            if module_passes:
                parts.extend(module_passes)
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
    if args.ai_threat and ai_model:
        threat_names = {0: "LOW", 1: "MEDIUM", 2: "HIGH", 3: "CRITICAL"}
        log("PIPELINE", f"AI Threat Engine: Model={ai_model['path']}, Threat={threat_names.get(threat_level, 'UNKNOWN')}")
    log("PIPELINE", "=" * 60)


if __name__ == "__main__":
    main()
