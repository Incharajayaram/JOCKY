import sys
import pathlib

sys.path.insert(0, str(pathlib.Path(__file__).parent.parent / "src"))

from jocky.language.parser import parse_source
from jocky.language.checker import TypeChecker
from jocky.language.checker import TypeError as JockyTypeError
from jocky.language.codegen import CodeGen


def check_source(source: str):
    """Parse + type-check source; raises JockyTypeError on type errors."""
    ast = parse_source(source)
    tc = TypeChecker()
    tc.check(ast)


def compile_to_ir(source: str) -> str:
    """Parse + type-check + codegen; returns LLVM IR string."""
    ast = parse_source(source)
    tc = TypeChecker()
    tc.check(ast)
    return CodeGen().gen(ast)
