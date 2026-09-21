from jocky.core.stage import Stage
from jocky.core.context import BuildContext
from jocky.core.toolchain import discover_toolchain
from jocky.utils.subprocess import run_cmd
from pathlib import Path

class MLIRObfuscateStage(Stage):
    @property
    def name(self) -> str:
        return "mlir_obfuscate"

    def run(self, ctx: BuildContext) -> BuildContext:
        from jocky.passes.registry import get_mlir_passes
        profile = ctx.state.get("profile")
        passes = []
        if profile:
            passes = get_mlir_passes(profile.passes)
        if not passes:
            return ctx

        tc = discover_toolchain()
        ir_path = ctx.state.get("llvm_ir")
        if not ir_path:
            return ctx

        out_dir = ctx.get_stage_output_dir(self.name)

        # Import LLVM IR to MLIR
        mlir_path = out_dir / "input.mlir"
        run_cmd([
            str(tc.mlir_translate()), "--import-llvm",
            str(ir_path), "-o", str(mlir_path)
        ], "Import LLVM IR to MLIR")

        # Run MLIR obfuscation passes
        obf_mlir = out_dir / "output.mlir"
        pass_args = []
        for p in passes:
            pass_args.append(f"--{p}")
        run_cmd([
            str(tc.mlir_opt()),
            f"--load-pass-plugin={tc.mlir_plugin()}",
            *pass_args,
            str(mlir_path),
            "-o", str(obf_mlir)
        ], "MLIR obfuscation passes")

        # Export back to LLVM IR
        run_cmd([
            str(tc.mlir_translate()), "--mlir-to-llvmir",
            str(obf_mlir), "-o", str(ir_path)
        ], "Export MLIR back to LLVM IR")

        return ctx
