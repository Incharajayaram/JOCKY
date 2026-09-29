"""
Comprehensive tests for generics/polymorphism support in JOCKY.
"""

import pytest
from jocky.language.lexer import Lexer
from jocky.language.parser import Parser
from jocky.language.checker import TypeChecker
from jocky.language.codegen import CodeGen
from jocky.language.ast import (
    Program, FuncDecl, Param, Block, JType, ReturnStmt, VarRef,
    BinaryOp, IntLiteral, ExprStmt, CallExpr, LetStmt
)
from jocky.language.generics import Unifier, TypeBindings, Monomorphizer, GenericTypeChecker


class TestTypeParsing:
    """Test parsing of generic type parameters."""

    def test_parse_generic_function_single_type_var(self):
        """Parse fn max<T>(a: T, b: T) -> T"""
        source = "fn max<T>(a: T, b: T) -> T { a }"
        lexer = Lexer(source)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        prog = parser.parse()

        assert len(prog.decls) == 1
        func = prog.decls[0]
        assert isinstance(func, FuncDecl)
        assert func.name == "max"
        assert func.is_generic
        assert func.type_params == ["T"]
        assert len(func.params) == 2
        assert func.params[0].type.is_type_var
        assert func.params[0].type.type_var_name == "T"
        assert func.params[1].type.is_type_var
        assert func.ret_type.is_type_var

    def test_parse_generic_function_multiple_type_vars(self):
        """Parse fn swap<T, U>(a: T, b: U) -> T"""
        source = "fn swap<T, U>(a: T, b: U) -> T { a }"
        lexer = Lexer(source)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        prog = parser.parse()

        func = prog.decls[0]
        assert func.type_params == ["T", "U"]
        assert func.params[0].type.type_var_name == "T"
        assert func.params[1].type.type_var_name == "U"

    def test_parse_generic_with_pointer_type_var(self):
        """Parse fn deref<T>(p: T*) -> T"""
        source = "fn deref<T>(p: T*) -> T { *p }"
        lexer = Lexer(source)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        prog = parser.parse()

        func = prog.decls[0]
        assert func.params[0].type.is_type_var
        assert func.params[0].type.is_pointer
        assert func.ret_type.is_type_var
        assert not func.ret_type.is_pointer

    def test_parse_non_generic_function(self):
        """Ensure non-generic functions don't have type_params"""
        source = "fn add(a: i32, b: i32) -> i32 { a + b }"
        lexer = Lexer(source)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        prog = parser.parse()

        func = prog.decls[0]
        assert not func.is_generic
        assert not func.params[0].type.is_type_var
        assert not func.ret_type.is_type_var


class TestUnification:
    """Test the unification algorithm."""

    def test_unify_type_var_with_concrete(self):
        """Unify a type variable with a concrete type"""
        bindings = TypeBindings()
        T = JType("T", is_type_var=True, type_var_name="T")
        i32 = JType("i32")

        result = Unifier.unify(T, i32, bindings)
        assert result
        assert bindings.get("T").name == "i32"

    def test_unify_concrete_types_match(self):
        """Unify two concrete types that match"""
        bindings = TypeBindings()
        i32_1 = JType("i32")
        i32_2 = JType("i32")

        result = Unifier.unify(i32_1, i32_2, bindings)
        assert result

    def test_unify_concrete_types_mismatch(self):
        """Unify two concrete types that don't match"""
        bindings = TypeBindings()
        i32 = JType("i32")
        i64 = JType("i64")

        with pytest.raises(TypeError, match="Cannot unify"):
            Unifier.unify(i32, i64, bindings)

    def test_unify_pointer_types(self):
        """Unify pointer type variables"""
        bindings = TypeBindings()
        T_ptr = JType("T", is_type_var=True, type_var_name="T", is_pointer=True)
        i32_ptr = JType("i32", is_pointer=True)

        result = Unifier.unify(T_ptr, i32_ptr, bindings)
        assert result
        assert bindings.get("T").name == "i32"
        assert bindings.get("T").is_pointer

    def test_unify_transitive(self):
        """Unify with already-bound type variables"""
        bindings = TypeBindings()
        T = JType("T", is_type_var=True, type_var_name="T")
        i32 = JType("i32")
        i32_2 = JType("i32")

        # Bind T to i32
        Unifier.unify(T, i32, bindings)
        # Try to unify T (already bound) with i32 again
        result = Unifier.unify(T, i32_2, bindings)
        assert result

    def test_unify_conflicting_bindings(self):
        """Type variable bound to different types should fail"""
        bindings = TypeBindings()
        T = JType("T", is_type_var=True, type_var_name="T")
        i32 = JType("i32")
        i64 = JType("i64")

        Unifier.unify(T, i32, bindings)

        with pytest.raises(TypeError):
            Unifier.unify(T, i64, bindings)


class TestMonomorphization:
    """Test monomorphization of generic functions."""

    def test_monomorphize_simple_generic(self):
        """Instantiate fn max<T>(a: T, b: T) -> T with T=i32"""
        generic = FuncDecl(
            name="max",
            params=[Param("a", JType("T", is_type_var=True, type_var_name="T")),
                    Param("b", JType("T", is_type_var=True, type_var_name="T"))],
            ret_type=JType("T", is_type_var=True, type_var_name="T"),
            body=Block([]),
            type_params=["T"],
            is_generic=True
        )

        bindings = TypeBindings()
        bindings.bind("T", JType("i32"))

        monomorphizer = Monomorphizer()
        instance = monomorphizer.instantiate(generic, bindings)

        assert instance.name.startswith("max__")
        assert not instance.is_generic
        assert instance.params[0].type.name == "i32"
        assert instance.params[1].type.name == "i32"
        assert instance.ret_type.name == "i32"

    def test_monomorphize_different_types_different_instances(self):
        """Two instantiations with different types create different instances"""
        generic = FuncDecl(
            name="max",
            params=[Param("a", JType("T", is_type_var=True, type_var_name="T")),
                    Param("b", JType("T", is_type_var=True, type_var_name="T"))],
            ret_type=JType("T", is_type_var=True, type_var_name="T"),
            body=Block([]),
            type_params=["T"],
            is_generic=True
        )

        monomorphizer = Monomorphizer()

        # Instantiate with i32
        bindings_i32 = TypeBindings()
        bindings_i32.bind("T", JType("i32"))
        instance_i32 = monomorphizer.instantiate(generic, bindings_i32)

        # Instantiate with i64
        bindings_i64 = TypeBindings()
        bindings_i64.bind("T", JType("i64"))
        instance_i64 = monomorphizer.instantiate(generic, bindings_i64)

        assert instance_i32.name != instance_i64.name
        assert instance_i32.params[0].type.name == "i32"
        assert instance_i64.params[0].type.name == "i64"

    def test_monomorphize_caches_instances(self):
        """Monomorphizer caches and reuses instances"""
        generic = FuncDecl(
            name="max",
            params=[Param("a", JType("T", is_type_var=True, type_var_name="T")),
                    Param("b", JType("T", is_type_var=True, type_var_name="T"))],
            ret_type=JType("T", is_type_var=True, type_var_name="T"),
            body=Block([]),
            type_params=["T"],
            is_generic=True
        )

        monomorphizer = Monomorphizer()

        # Instantiate twice with same type
        bindings = TypeBindings()
        bindings.bind("T", JType("i32"))
        instance1 = monomorphizer.instantiate(generic, bindings)

        bindings2 = TypeBindings()
        bindings2.bind("T", JType("i32"))
        instance2 = monomorphizer.instantiate(generic, bindings2)

        # Should be the same instance
        assert instance1.name == instance2.name
        assert instance1 is instance2


class TestTypeInference:
    """Test type inference for generic functions."""

    def test_infer_bindings_simple(self):
        """Infer T=i32 from max(1, 2)"""
        generic = FuncDecl(
            name="max",
            params=[Param("a", JType("T", is_type_var=True, type_var_name="T")),
                    Param("b", JType("T", is_type_var=True, type_var_name="T"))],
            ret_type=JType("T", is_type_var=True, type_var_name="T"),
            body=Block([]),
            type_params=["T"],
            is_generic=True
        )

        arg_types = [JType("i32"), JType("i32")]
        bindings = GenericTypeChecker.infer_type_bindings(generic, arg_types)

        assert bindings.get("T").name == "i32"

    def test_infer_bindings_multiple_type_vars(self):
        """Infer T=i32, U=i64 from swap(1, 2i64)"""
        generic = FuncDecl(
            name="swap",
            params=[Param("a", JType("T", is_type_var=True, type_var_name="T")),
                    Param("b", JType("U", is_type_var=True, type_var_name="U"))],
            ret_type=JType("T", is_type_var=True, type_var_name="T"),
            body=Block([]),
            type_params=["T", "U"],
            is_generic=True
        )

        arg_types = [JType("i32"), JType("i64")]
        bindings = GenericTypeChecker.infer_type_bindings(generic, arg_types)

        assert bindings.get("T").name == "i32"
        assert bindings.get("U").name == "i64"

    def test_infer_bindings_wrong_arg_count(self):
        """Type inference fails with wrong number of arguments"""
        generic = FuncDecl(
            name="max",
            params=[Param("a", JType("T", is_type_var=True, type_var_name="T")),
                    Param("b", JType("T", is_type_var=True, type_var_name="T"))],
            ret_type=JType("T", is_type_var=True, type_var_name="T"),
            body=Block([]),
            type_params=["T"],
            is_generic=True
        )

        arg_types = [JType("i32")]

        with pytest.raises(TypeError, match="expects 2"):
            GenericTypeChecker.infer_type_bindings(generic, arg_types)


class TestTypeChecking:
    """Test type checking with generic functions."""

    def test_check_generic_function_call(self):
        """Type check a call to a generic function"""
        source = """
        fn max<T>(a: T, b: T) -> T { a }
        fn main() -> void {
            let x = max(5, 10);
        }
        """
        lexer = Lexer(source)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        prog = parser.parse()

        checker = TypeChecker()
        checker.check(prog)

        # Generic function should be in generic_functions
        assert "max" in checker.generic_functions
        # Monomorphic instance should be created
        assert len(checker.monomorphic_instances) > 0

    def test_type_error_generic_mismatch(self):
        """Type error when calling generic with mismatched types"""
        source = """
        fn max<T>(a: T, b: T) -> T { a }
        fn main() -> void {
            let x = max(5, "hello");
        }
        """
        lexer = Lexer(source)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        prog = parser.parse()

        checker = TypeChecker()
        with pytest.raises(TypeError):
            checker.check(prog)


class TestCodeGeneration:
    """Test code generation for generic functions."""

    def test_codegen_generic_instance(self):
        """Generate code for a monomorphic instance"""
        source = """
        fn max<T>(a: T, b: T) -> T { a }
        fn main() -> i32 {
            max(5, 10)
        }
        """
        lexer = Lexer(source)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        prog = parser.parse()

        checker = TypeChecker()
        checker.check(prog)

        codegen = CodeGen()
        codegen.structs = checker.structs
        codegen.enums = checker.enums
        codegen.functions = checker.functions
        codegen.monomorphic_instances = checker.monomorphic_instances
        ir = codegen.gen(prog)

        # Check that the monomorphic instance is in the IR
        assert "max__" in ir

    def test_codegen_multiple_instances(self):
        """Generate code for multiple monomorphic instances"""
        source = """
        fn max<T>(a: T, b: T) -> T { a }
        fn main() -> void {
            let x = max(5, 10);
            let y = max(true, false);
        }
        """
        lexer = Lexer(source)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        prog = parser.parse()

        checker = TypeChecker()
        checker.check(prog)

        # Should have two instances: one for i32, one for i64
        assert len(checker.monomorphic_instances) >= 2


class TestComplexGenerics:
    """Test more complex generic scenarios."""

    def test_generic_with_pointers(self):
        """Generic function with pointer type arguments"""
        source = """
        fn deref<T>(p: T*) -> T { *p }
        fn main() -> i32 {
            let x = 42;
            deref(&x)
        }
        """
        lexer = Lexer(source)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        prog = parser.parse()

        checker = TypeChecker()
        checker.check(prog)

        # Should have one monomorphic instance
        assert len(checker.monomorphic_instances) >= 1

    def test_generic_identity_function(self):
        """Generic identity function"""
        source = """
        fn id<T>(x: T) -> T { x }
        fn main() -> void {
            let a = id(42);
            let b = id(true);
        }
        """
        lexer = Lexer(source)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        prog = parser.parse()

        checker = TypeChecker()
        checker.check(prog)

        # Should have at least 2 instances (i32 and bool)
        assert len(checker.monomorphic_instances) >= 2


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
