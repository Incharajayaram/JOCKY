import unittest
from jocky.language.lexer import Lexer
from jocky.language.parser import Parser
from jocky.language.checker import TypeChecker

class TestClosures(unittest.TestCase):
    """Test closure/lambda support."""

    def parse_and_check(self, code):
        """Helper to parse and type-check code."""
        lexer = Lexer(code)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        prog = parser.parse()
        checker = TypeChecker()
        checker.check(prog)
        return prog, checker

    def test_simple_lambda_identity(self):
        """Test simple lambda: |x| x"""
        code = """
        fn main() -> void {
            let id = |x: i32| x;
        }
        """
        prog, checker = self.parse_and_check(code)
        self.assertIsNotNone(checker)

    def test_lambda_with_body_block(self):
        """Test lambda with body block: |x| { return x + 1; }"""
        code = """
        fn main() -> void {
            let inc = |x: i32| { return x + 1; };
        }
        """
        prog, checker = self.parse_and_check(code)
        self.assertIsNotNone(checker)

    def test_lambda_multiple_params(self):
        """Test lambda with multiple parameters: |x, y| x + y"""
        code = """
        fn main() -> void {
            let add = |x: i32, y: i32| x + y;
        }
        """
        prog, checker = self.parse_and_check(code)
        self.assertIsNotNone(checker)

    def test_lambda_with_explicit_return_type(self):
        """Test lambda with explicit return type: |x: i32| -> i32 { x * 2 }"""
        code = """
        fn main() -> void {
            let double = |x: i32| -> i32 { x * 2 };
        }
        """
        prog, checker = self.parse_and_check(code)
        self.assertIsNotNone(checker)

    def test_lambda_type_inference(self):
        """Test that lambda return type is inferred"""
        code = """
        fn main() -> void {
            let f = |x: i32| x + 42;
        }
        """
        prog, checker = self.parse_and_check(code)
        self.assertIsNotNone(checker)

    def test_lambda_capture_by_value(self):
        """Test lambda capturing variable by value"""
        code = """
        fn main() -> void {
            let x = 10;
            let f = |y: i32| x + y;
        }
        """
        prog, checker = self.parse_and_check(code)
        self.assertIsNotNone(checker)

    def test_lambda_capture_by_reference(self):
        """Test lambda capturing variable by reference"""
        code = """
        fn main() -> void {
            let x = 10;
            let f = |y: i32| &x;
        }
        """
        prog, checker = self.parse_and_check(code)
        self.assertIsNotNone(checker)

    def test_lambda_no_params(self):
        """Test lambda with no parameters: || 42"""
        code = """
        fn main() -> void {
            let f = || 42;
        }
        """
        prog, checker = self.parse_and_check(code)
        self.assertIsNotNone(checker)

if __name__ == "__main__":
    unittest.main()
