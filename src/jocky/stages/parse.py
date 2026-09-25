from jocky.core.stage import Stage
from jocky.core.context import BuildContext
from jocky.language.lexer import Lexer
from jocky.language.parser import Parser
from jocky.language.checker import TypeChecker
from pathlib import Path

_PRELUDE_PATH = Path(__file__).parent.parent / "stdlib" / "prelude.jky"

class ParseStage(Stage):
    @property
    def name(self) -> str:
        return "parse"

    def run(self, ctx: BuildContext) -> BuildContext:
        src = ctx.input_file.read_text()
        no_prelude = ctx.config.get("no_prelude", False)

        if not no_prelude and _PRELUDE_PATH.exists():
            prelude_src = _PRELUDE_PATH.read_text()
            src = prelude_src + "\n" + src

        lexer = Lexer(src)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        ast = parser.parse()
        ctx.state["ast"] = ast

        checker = TypeChecker()
        checker.check(ast)
        ctx.state["type_checked"] = True
        return ctx
