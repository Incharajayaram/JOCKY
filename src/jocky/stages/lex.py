from jocky.core.stage import Stage
from jocky.core.context import BuildContext

class LexStage(Stage):
    @property
    def name(self) -> str:
        return "lex"

    def run(self, ctx: BuildContext) -> BuildContext:
        print(f"Lexing {ctx.input_file}...")
        out_dir = ctx.get_stage_output_dir(self.name)
        # Dummy behavior: just create a file indicating lexing happened
        (out_dir / "tokens.txt").write_text("DUMMY TOKENS")
        return ctx
