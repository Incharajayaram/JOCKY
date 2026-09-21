from jocky.core.stage import Stage
from jocky.core.context import BuildContext
from jocky.language.lexer import Lexer
from jocky.language.parser import Parser
from jocky.language.checker import TypeChecker
from pathlib import Path

class ParseStage(Stage):
    @property
    def name(self) -> str:
        return "parse"

    def run(self, ctx: BuildContext) -> BuildContext:
        src = ctx.input_file.read_text()
        lexer = Lexer(src)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        ast = parser.parse()
        ctx.state["ast"] = ast

        checker = TypeChecker()
        checker.check(ast)
        ctx.state["type_checked"] = True
        return ctx
