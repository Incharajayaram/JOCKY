from jocky.core.stage import Stage
from jocky.core.context import BuildContext
from jocky.core.toolchain import discover_toolchain
from jocky.passes.registry import format_opt_passes
from jocky.utils.subprocess import run_cmd
from pathlib import Path
import shutil

class IRObfuscateStage(Stage):
    @property
    def name(self) -> str:
        return "ir_obfuscate"

    def run(self, ctx: BuildContext) -> BuildContext:
        tc = discover_toolchain()
        ir_path = ctx.state["llvm_ir"]
        out_dir = ctx.get_stage_output_dir(self.name)

        # Compile .ll to .bc with -O1 first (required for some passes)
        bc_path = out_dir / "output.bc"
        run_cmd([
            str(tc.clang()), "-O1", "-c", "-emit-llvm",
            "-x", "ir", str(ir_path),
            "-o", str(bc_path)
        ], "LLVM IR to bitcode")

        # Determine passes from profile
        profile = ctx.state.get("profile")
        passes = []
        if profile:
            passes = profile.passes

        passes_str = format_opt_passes(passes)
        obf_bc_path = out_dir / "output.obf.bc"
        if passes_str:
            run_cmd([
                str(tc.opt()),
                f"-load-pass-plugin={tc.llvm_plugin()}",
                f"-passes={passes_str}",
                str(bc_path),
                "-o", str(obf_bc_path)
            ], "Obfuscation passes")
        else:
            # No obfuscation requested; just copy the bitcode forward
            shutil.copy(str(bc_path), str(obf_bc_path))

        ctx.state["llvm_obf_bc"] = obf_bc_path
        return ctx
