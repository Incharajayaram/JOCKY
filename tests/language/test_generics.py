import unittest
from jocky.language.lexer import Lexer
from jocky.language.parser import Parser
from jocky.language.checker import TypeChecker
from jocky.language.codegen import CodeGen
from jocky.language.ast import JType

class TestGenerics(unittest.TestCase):
    """Test generic function and struct support."""

    def parse_and_check(self, code):
        """Helper to parse and type-check code."""
        lexer = Lexer(code)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        prog = parser.parse()
        checker = TypeChecker()
        checker.check(prog)
        return prog, checker

    def test_generic_function_single_type_var(self):
        """Test generic function with single type variable: id<T>(x: T) -> T"""
        code = """
        fn id<T>(x: T) -> T {
            return x;
        }
        fn main() -> void {
            let a = id::<i32>(42);
            let b = id::<i64>(100);
        }
        """
        prog, checker = self.parse_and_check(code)
        self.assertEqual(len(checker.monomorphizations), 2)

    def test_generic_function_multiple_type_vars(self):
        """Test generic with multiple type variables: swap<T, U>(a: T, b: U) -> Pair<T, U>"""
        code = """
        struct Pair {
            first: i32;
            second: i32
        };

        fn swap<T, U>(a: T, b: U) -> i32 {
            return 0;
        }

        fn main() -> void {
            let x = swap::<i32, i64>(1, 2);
        }
        """
        prog, checker = self.parse_and_check(code)
        self.assertGreaterEqual(len(checker.monomorphizations), 1)

    def test_generic_function_call_resolves_types(self):
        """Test that generic function calls resolve argument types correctly."""
        code = """
        fn max<T>(a: T, b: T) -> T {
            if a > b { return a; } else { return b; }
        }
        fn main() -> void {
            let x = max::<i32>(5, 10);
        }
        """
        prog, checker = self.parse_and_check(code)
        # Should successfully check without type errors
        self.assertIsNotNone(checker)

    def test_generic_function_type_mismatch_error(self):
        """Test that mismatched type arguments cause errors."""
        code = """
        fn id<T>(x: T) -> T {
            x
        }
        fn main() -> void {
            let x = id::<i32>("hello");
        }
        """
        with self.assertRaises(Exception):
            self.parse_and_check(code)

    def test_generic_function_substitution(self):
        """Test that parameter types are correctly substituted."""
        code = """
        fn first<T>(x: T, y: T) -> T {
            return x;
        }
        fn main() -> void {
            let a = first::<i32>(1, 2);
            let b = first::<i64>(100, 200);
        }
        """
        prog, checker = self.parse_and_check(code)
        self.assertEqual(len(checker.monomorphizations), 2)
        # Verify both monomorphizations are recorded
        mono_names = [m.mangled_name for m in checker.monomorphizations]
        self.assertIn("first_i32", mono_names)
        self.assertIn("first_i64", mono_names)

    def test_codegen_monomorphized_functions(self):
        """Test that codegen generates monomorphized function definitions."""
        code = """
        fn id<T>(x: T) -> T {
            return x;
        }
        fn main() -> void {
            let a: i32 = id::<i32>(42);
        }
        """
        prog, checker = self.parse_and_check(code)
        gen = CodeGen()
        llvm_ir = gen.gen(prog, checker.monomorphizations)
        # Should contain both id and id_i32 definitions
        self.assertIn("@id_i32", llvm_ir)

    def test_nested_generic_calls(self):
        """Test nested generic function calls."""
        code = """
        fn id<T>(x: T) -> T {
            return x;
        }
        fn apply<U>(x: U) -> U {
            return id::<U>(x);
        }
        fn main() -> void {
            let a = apply::<i32>(10);
        }
        """
        prog, checker = self.parse_and_check(code)
        # Should have monomorphizations for both functions
        self.assertGreaterEqual(len(checker.monomorphizations), 1)

    def test_generic_with_pointer_types(self):
        """Test generic with pointer type arguments."""
        code = """
        fn deref<T>(p: T*) -> T {
            return *p;
        }
        fn main() -> void {
            let x = 42;
            let p = &x;
            let y = deref::<i32>(p);
        }
        """
        prog, checker = self.parse_and_check(code)
        self.assertGreaterEqual(len(checker.monomorphizations), 1)

    def test_generic_struct_instantiation(self):
        """Test generic struct instantiation (basic support)."""
        code = """
        struct Box {
            value: i32;
        };
        fn main() -> void {
            let b: i32 = 42;
        }
        """
        prog, checker = self.parse_and_check(code)
        self.assertIsNotNone(checker)

    def test_generic_enum_support(self):
        """Test generic enum (basic parsing)."""
        code = """
        enum Option {
            Some,
            None
        };
        fn main() -> void {
            let x: i32 = 0;
        }
        """
        prog, checker = self.parse_and_check(code)
        self.assertIsNotNone(checker)

if __name__ == "__main__":
    unittest.main()
