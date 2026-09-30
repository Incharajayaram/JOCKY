from jocky.core.stage import Stage
from jocky.core.context import BuildContext
from jocky.language.codegen import CodeGen
from pathlib import Path

class LowerIRStage(Stage):
    @property
    def name(self) -> str:
        return "lower_ir"

    def run(self, ctx: BuildContext) -> BuildContext:
        ast = ctx.state["ast"]
        target_platform = ctx.config.get("target", "windows")
        gen = CodeGen(target_platform=target_platform)
        ir_text = gen.gen(ast)
        out_dir = ctx.get_stage_output_dir(self.name)
        ir_path = out_dir / "output.ll"
        ir_path.write_text(ir_text)
        ctx.state["llvm_ir"] = ir_path
        return ctx
