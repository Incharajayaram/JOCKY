from jocky.core.stage import Stage
from jocky.core.context import BuildContext
from jocky.utils.subprocess import run_cmd
from pathlib import Path

LLVM_BUILD = Path("/home/kamini/projects/llvm-obfuscation-tools-linux-x86_64")
CLANG = LLVM_BUILD / "bin" / "clang"

class LinkStage(Stage):
    @property
    def name(self) -> str:
        return "link"

    def run(self, ctx: BuildContext) -> BuildContext:
        obf_bc = ctx.state["llvm_obf_bc"]
        out_dir = ctx.get_stage_output_dir(self.name)
        output = ctx.output_dir / ctx.input_file.stem
        if ctx.config.get("output"):
            output = Path(ctx.config["output"])

        # Compile obfuscated bitcode to object
        obj_path = out_dir / "output.o"
        run_cmd([
            str(CLANG), "-c", str(obf_bc), "-o", str(obj_path)
        ], "Bitcode to object")

        # Link to executable
        run_cmd([
            str(CLANG), str(obj_path), "-o", str(output)
        ], "Linking executable")

        ctx.state["executable"] = output
        ctx.state["object_file"] = obj_path
        return ctx
