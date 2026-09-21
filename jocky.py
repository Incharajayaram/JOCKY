#!/usr/bin/env python3
import argparse
import subprocess
import os
import sys
import re

# Paths to the prebuilt tools
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
TOOLCHAIN_DIR = os.path.join(SCRIPT_DIR, "toolchain")
RUN_CLANG = os.path.join(TOOLCHAIN_DIR, "bin", "run-clang.sh")
RUN_CLANG_CPP = os.path.join(TOOLCHAIN_DIR, "bin", "run-clang++.sh")
RUN_OPT = os.path.join(TOOLCHAIN_DIR, "bin", "run-opt.sh")
LLVM_PLUGIN = os.path.join(TOOLCHAIN_DIR, "lib", "LLVMObfuscationPlugin.so")

def run_cmd(cmd, step_name):
    print(f"[*] Running {step_name}...")
    try:
        subprocess.run(cmd, check=True)
    except subprocess.CalledProcessError as e:
        print(f"[!] Error during {step_name}.")
        sys.exit(e.returncode)
    except FileNotFoundError:
        print(f"[!] Command not found: {cmd[0]}")
        sys.exit(1)

def extract_libs(filepath):
    libs = []
    with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
        for line in f:
            if '//' in line:
                matches = re.findall(r'-l[a-zA-Z0-9_]+', line)
                libs.extend(matches)
    return list(set(libs))

def main():
    parser = argparse.ArgumentParser(description="JOCKY: Obfuscation & Packing CLI")
    parser.add_argument("input", help="Input C/C++ source file")
    parser.add_argument("-o", "--output", default="a.out", help="Output binary name")
    parser.add_argument("--pack", action="store_true", help="Pack the final binary using UPX")
    
    parser.add_argument("--windows", action="store_true", help="Cross-compile to Windows (PE32+)")
    parser.add_argument("--msvc", action="store_true", help="Use clang-cl with MSVC environment for Windows (requires xwin extracted SDK)")
    parser.add_argument("--vctoolsdir", help="Path to extracted MSVC VCTools (for --msvc)")
    parser.add_argument("--winsdkdir", help="Path to extracted Windows SDK (for --msvc)")    
    # Individual pass flags
    parser.add_argument("--bcf", action="store_true", help="Enable Bogus Control Flow")
    parser.add_argument("--fla", action="store_true", help="Enable Control Flow Flattening")
    parser.add_argument("--sub", action="store_true", help="Enable Instruction Substitution")
    parser.add_argument("--split", action="store_true", help="Enable Basic Block Splitting")
    parser.add_argument("--mba", action="store_true", help="Enable Linear MBA")
    parser.add_argument("--opaque", action="store_true", help="Enable Opaque Predicates")
    parser.add_argument("--indcall", action="store_true", help="Enable Indirect Call Obfuscation")
    parser.add_argument("--pdata", action="store_true", help="Enable PData Stripping (Windows)")
    parser.add_argument("--antidebug", action="store_true", help="Enable Anti-Debug")
    parser.add_argument("--signature", action="store_true", help="Enable Signature Stripping")
    parser.add_argument("--virtualize", action="store_true", help="Enable Code Virtualization")

    parser.add_argument("--passes", default=None, 
                        help="Comma-separated list of obfuscation passes (overrides individual flags)")
    
    args = parser.parse_args()

    if args.passes is None:
        selected_passes = []
        if args.bcf: selected_passes.append("boguscf")
        if args.fla: selected_passes.append("flattening")
        if args.sub: selected_passes.append("substitution")
        if args.split: selected_passes.append("split")
        if args.mba: selected_passes.append("linear-mba")
        if args.opaque: selected_passes.append("opaque-pred")
        if args.indcall: selected_passes.append("indirect-call")
        if args.pdata: selected_passes.append("pdata-strip")
        if args.antidebug: selected_passes.append("anti-debug")
        if args.signature: selected_passes.append("strip-signature")
        if args.virtualize: selected_passes.append("virtualize")
        
        if not selected_passes:
            args.passes = "boguscf,flattening,substitution,split,linear-mba,opaque-pred"
        else:
            args.passes = ",".join(selected_passes)

    if (args.windows or args.msvc) and args.output == "a.out":
        args.output = "a.exe"
        
    is_windows = args.windows or args.msvc or args.output.endswith(".exe")

    if not os.path.exists(args.input):
        print(f"[!] Input file '{args.input}' not found.")
        sys.exit(1)

    input_ext = os.path.splitext(args.input)[1].lower()
    is_cpp = input_ext in [".cpp", ".cc", ".cxx"]
    
    # Heuristic for Windows C files containing COM C++ code
    force_cpp = False
    if is_windows and not is_cpp:
        with open(args.input, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
            if 'CoCreateInstance' in content and '->' in content:
                print("[*] Detected C++ COM syntax in C file, forcing C++ compilation.")
                force_cpp = True
                is_cpp = True

    bundled_clang = RUN_CLANG_CPP if is_cpp else RUN_CLANG

    temp_bc = "temp.bc"
    temp_obf_bc = "temp_obf.bc"
    temp_obf_o = "temp_obf.o"

    target_args = []
    extra_libs = []
    
    compiler_to_bc = bundled_clang
    linker_exe = bundled_clang

    if args.msvc:
        target_args = ["--driver-mode=cl"]
        vctools = args.vctoolsdir
        if not vctools and os.path.exists("/opt/xwin/crt"):
            vctools = "/opt/xwin/crt"
            
        winsdk = args.winsdkdir
        if not winsdk and os.path.exists("/opt/xwin/sdk"):
            winsdk = "/opt/xwin/sdk"
            
        if vctools:
            target_args.extend(["/vctoolsdir", vctools])
        if winsdk:
            target_args.extend(["/winsdkdir", winsdk])
        extra_libs = extract_libs(args.input)
        if extra_libs:
            extra_libs = [lib[2:] + ".lib" for lib in extra_libs]
            print(f"[*] Found required Windows libraries: {' '.join(extra_libs)}")
    elif is_windows:
        # Use llvm-mingw inside the container if available
        llvm_mingw_clang = "/opt/llvm-mingw/bin/x86_64-w64-mingw32-clang++" if is_cpp else "/opt/llvm-mingw/bin/x86_64-w64-mingw32-clang"
        if os.path.exists(llvm_mingw_clang):
            compiler_to_bc = llvm_mingw_clang
            linker_exe = llvm_mingw_clang
        else:
            print("[!] Warning: llvm-mingw not found at /opt/llvm-mingw. Using default clang which may fail with Windows headers.")
            target_args = [
                "-target", "x86_64-w64-mingw32",
            ]
        extra_libs = extract_libs(args.input)
        if extra_libs:
            print(f"[*] Found required Windows libraries: {' '.join(extra_libs)}")
    else:
        target_args = [
            "-isystem", "/usr/lib/gcc/x86_64-linux-gnu/13/include"
        ]

    # Step 1: Compile source to LLVM bitcode
    emit_llvm_flag = ["-Xclang", "-emit-llvm-bc"] if args.msvc else ["-emit-llvm"]
    compile_cmd = [compiler_to_bc] + target_args + ["-O2", "-Xclang", "-disable-lifetime-markers"] + emit_llvm_flag + ["-c", args.input, "-o", temp_bc]
    if force_cpp:
        if args.msvc:
            compile_cmd.insert(1, "/TP")
        else:
            compile_cmd.insert(1, "-x")
            compile_cmd.insert(2, "c++")
        
    run_cmd(compile_cmd, "Compilation to LLVM Bitcode")

    function_passes_set = {"boguscf", "flattening", "substitution", "split", "linear-mba", "opaque-pred"}
    module_passes_set = {"indirect-call", "pdata-strip", "anti-debug", "strip-signature", "virtualize"}
    
    fp_list = []
    mp_list = []
    if args.passes:
        for p in args.passes.split(','):
            p = p.strip()
            if not p: continue
            if p in function_passes_set: fp_list.append(p)
            elif p in module_passes_set: mp_list.append(p)
            else: mp_list.append(p)
            
    formatted_passes = []
    if fp_list:
        formatted_passes.append(f"function({','.join(fp_list)})")
    if mp_list:
        formatted_passes.extend(mp_list)
        
    final_passes_str = ",".join(formatted_passes) if formatted_passes else "default<O2>"

    # Step 2: Apply Obfuscation passes
    opt_cmd = [
        RUN_OPT,
        f"-load-pass-plugin={LLVM_PLUGIN}",
        f"-passes={final_passes_str}",
        temp_bc,
        "-o", temp_obf_bc
    ]
    run_cmd(opt_cmd, "Obfuscation Passes")

    # Step 3: Compile obfuscated bitcode to Object file (using bundled clang to match LLVM bitcode version)
    obj_cmd = [bundled_clang]
    if args.msvc:
        obj_cmd += target_args
    elif is_windows:
        obj_cmd += ["-target", "x86_64-w64-mingw32"]
    obj_cmd += ["-O2", "-c", temp_obf_bc, "-o", temp_obf_o]
    run_cmd(obj_cmd, "Compilation of Bitcode to Object file")

    # Step 4: Link Object file to Final Executable (using system/llvm-mingw clang for correct libraries)
    link_cmd = [linker_exe] + target_args + ["-O2", temp_obf_o, "-o", args.output] + extra_libs
    if args.msvc:
        link_cmd.insert(1, "-fuse-ld=lld")
    run_cmd(link_cmd, "Linking to Final Executable")

    # Step 5: Cleanup temp files
    print("[*] Cleaning up temporary files...")
    if os.path.exists(temp_bc): os.remove(temp_bc)
    if os.path.exists(temp_obf_bc): os.remove(temp_obf_bc)
    if os.path.exists(temp_obf_o): os.remove(temp_obf_o)

    # Step 6: UPX Packing
    if args.pack:
        run_cmd(["upx", args.output], "UPX Packing")

    print(f"[+] Success! Obfuscated binary created at: {os.path.abspath(args.output)}")

if __name__ == "__main__":
    main()
