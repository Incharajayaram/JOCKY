from jocky.core.stage import Stage
from jocky.core.context import BuildContext
from jocky.passes.registry import format_opt_passes
from jocky.utils.subprocess import run_cmd
from pathlib import Path
import os

LLVM_BUILD = Path("/home/kamini/projects/llvm-obfuscation-tools-linux-x86_64")
CLANG = LLVM_BUILD / "bin" / "clang"
OPT = LLVM_BUILD / "bin" / "opt"
LLVM_PLUGIN = LLVM_BUILD / "lib" / "LLVMObfuscationPlugin.so"

class IRObfuscateStage(Stage):
    @property
    def name(self) -> str:
        return "ir_obfuscate"

    def run(self, ctx: BuildContext) -> BuildContext:
        ir_path = ctx.state["llvm_ir"]
        out_dir = ctx.get_stage_output_dir(self.name)

        # Compile .ll to .bc with -O1 first (required for some passes)
        bc_path = out_dir / "output.bc"
        run_cmd([
            str(CLANG), "-O1", "-c", "-emit-llvm",
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
                str(OPT),
                f"-load-pass-plugin={LLVM_PLUGIN}",
                f"-passes={passes_str}",
                str(bc_path),
                "-o", str(obf_bc_path)
            ], "Obfuscation passes")
        else:
            # No obfuscation requested; just copy the bitcode forward
            import shutil
            shutil.copy(str(bc_path), str(obf_bc_path))

        ctx.state["llvm_obf_bc"] = obf_bc_path
        return ctx
