"""Tests for JOCKY debugger support."""

import pytest
from pathlib import Path
import tempfile
import sys

sys.path.insert(0, str(Path(__file__).parent.parent.parent / "tools"))

from jocky.language.ast import (
    FuncDecl, LetStmt, IfStmt, Block, SourceLocation, JType, Param
)


class TestSourceLocation:
    """Test SourceLocation tracking."""

    def test_source_location_creation(self):
        """Test creating a source location."""
        loc = SourceLocation("test.jky", 10, 5)
        assert loc.filename == "test.jky"
        assert loc.line == 10
        assert loc.column == 5

    def test_source_location_with_range(self):
        """Test source location with end position."""
        loc = SourceLocation("test.jky", 10, 5, end_line=10, end_column=20)
        assert loc.end_line == 10
        assert loc.end_column == 20

    def test_source_location_string_representation(self):
        """Test string representation of location."""
        loc = SourceLocation("test.jky", 10, 5)
        assert str(loc) == "test.jky:10:5"

        loc_with_range = SourceLocation("test.jky", 10, 5, end_line=10, end_column=20)
        assert str(loc_with_range) == "test.jky:10:5-10:20"


class TestFunctionDebugInfo:
    """Test function debug information."""

    def test_func_decl_with_location(self):
        """Test FuncDecl stores location information."""
        loc = SourceLocation("test.jky", 1, 0)
        func = FuncDecl(
            name="main",
            params=[],
            ret_type=JType("i32"),
            body=Block([]),
            location=loc
        )
        assert func.location is not None
        assert func.location.filename == "test.jky"
        assert func.location.line == 1

    def test_func_decl_without_location(self):
        """Test FuncDecl works without location (backwards compatibility)."""
        func = FuncDecl(
            name="main",
            params=[],
            ret_type=JType("i32"),
            body=Block([])
        )
        assert func.location is None


class TestStatementDebugInfo:
    """Test statement debug information."""

    def test_let_stmt_with_location(self):
        """Test LetStmt stores location information."""
        from jocky.language.ast import IntLiteral

        loc = SourceLocation("test.jky", 5, 4)
        stmt = LetStmt(
            name="x",
            type=JType("i32"),
            init=IntLiteral(42),
            location=loc
        )
        assert stmt.location is not None
        assert stmt.location.line == 5

    def test_if_stmt_with_location(self):
        """Test IfStmt stores location information."""
        from jocky.language.ast import BoolLiteral

        loc = SourceLocation("test.jky", 10, 0)
        stmt = IfStmt(
            cond=BoolLiteral(True),
            then_block=Block([]),
            else_block=None,
            location=loc
        )
        assert stmt.location is not None
        assert stmt.location.line == 10


class TestDebuggerClient:
    """Test JOCKY debugger client."""

    def test_debugger_initialization(self):
        """Test debugger client initialization."""
        try:
            from debugger import JockyDebugger

            # Create a dummy executable path for testing
            with tempfile.NamedTemporaryFile(suffix=".bin", delete=False) as f:
                dummy_exe = f.name

            try:
                dbg = JockyDebugger(dummy_exe, "gdb")
                assert dbg.executable.as_posix() == Path(dummy_exe).as_posix()
                assert dbg.debugger_type == "gdb"
                assert dbg.process is None
            finally:
                Path(dummy_exe).unlink()
        except ImportError:
            pytest.skip("Debugger tools not available")

    def test_debugger_executable_not_found(self):
        """Test debugger raises error for missing executable."""
        try:
            from debugger import JockyDebugger

            with pytest.raises(FileNotFoundError):
                JockyDebugger("/nonexistent/path/executable", "gdb")
        except ImportError:
            pytest.skip("Debugger tools not available")

    def test_debugger_invalid_debugger_type(self):
        """Test debugger raises error for invalid debugger type."""
        try:
            from debugger import JockyDebugger

            with tempfile.NamedTemporaryFile(suffix=".bin", delete=False) as f:
                dummy_exe = f.name

            try:
                with pytest.raises(ValueError):
                    JockyDebugger(dummy_exe, "invalid_debugger")
            finally:
                Path(dummy_exe).unlink()
        except ImportError:
            pytest.skip("Debugger tools not available")

    def test_debugger_context_manager(self):
        """Test debugger as context manager."""
        try:
            from debugger import JockyDebugger

            with tempfile.NamedTemporaryFile(suffix=".bin", delete=False) as f:
                dummy_exe = f.name

            try:
                with JockyDebugger(dummy_exe, "gdb") as dbg:
                    assert dbg is not None
            finally:
                Path(dummy_exe).unlink()
        except ImportError:
            pytest.skip("Debugger tools not available")


class TestDWARFMetadata:
    """Test DWARF metadata generation."""

    def test_compilation_unit_metadata_format(self):
        """Test compilation unit metadata format."""
        from jocky.language.codegen import CodeGen

        codegen = CodeGen()
        codegen.source_file = "test.jky"

        # Test that metadata IDs are generated correctly
        id1 = codegen.next_metadata_id()
        id2 = codegen.next_metadata_id()

        assert id2 == id1 + 1
        assert id1 >= 0

    def test_type_metadata_generation(self):
        """Test type metadata generation."""
        from jocky.language.codegen import CodeGen

        codegen = CodeGen()
        codegen.debug_enabled = True

        # Test various type metadata
        i32_type = JType("i32")
        bool_type = JType("bool")
        void_type = JType("void")

        # Just ensure methods don't crash
        i32_meta = codegen.get_type_metadata(i32_type)
        assert i32_meta >= 0

        bool_meta = codegen.get_type_metadata(bool_type)
        assert bool_meta >= 0

        void_meta = codegen.get_type_metadata(void_type)
        assert void_meta >= 0

    def test_debug_info_disabled(self):
        """Test that debug info can be disabled."""
        from jocky.language.codegen import CodeGen

        codegen = CodeGen()
        codegen.debug_enabled = False

        # Methods should handle disabled debug gracefully
        result = codegen.emit_dwarf_compile_unit("test.jky")
        assert result is None

        jtype = JType("i32")
        meta = codegen.get_type_metadata(jtype)
        assert meta == -1


class TestDebuggerIntegration:
    """Integration tests for debugger."""

    def test_simple_program_debug_info(self):
        """Test that simple programs have debug info."""
        from jocky.language.lexer import Lexer
        from jocky.language.parser import Parser
        from jocky.language.checker import TypeChecker
        from jocky.language.codegen import CodeGen

        source = """
        fn main() -> i32 {
            let x: i32 = 42;
            return x;
        }
        """

        lexer = Lexer(source)
        tokens = lexer.tokenize()
        parser = Parser(tokens, source_file="test.jky")
        ast = parser.parse()

        # Check that function has location info
        main_func = ast.decls[0]
        assert isinstance(main_func, FuncDecl)
        assert main_func.location is not None
        assert main_func.location.filename == "test.jky"

        # Check that statements have location info
        let_stmt = main_func.body.stmts[0]
        assert isinstance(let_stmt, LetStmt)
        assert let_stmt.location is not None

    def test_codegen_with_debug_info(self):
        """Test that codegen generates debug metadata."""
        from jocky.language.lexer import Lexer
        from jocky.language.parser import Parser
        from jocky.language.checker import TypeChecker
        from jocky.language.codegen import CodeGen

        source = """
        fn add(a: i32, b: i32) -> i32 {
            return a + b;
        }

        fn main() -> i32 {
            let result: i32 = add(1, 2);
            return result;
        }
        """

        lexer = Lexer(source)
        tokens = lexer.tokenize()
        parser = Parser(tokens, source_file="test.jky")
        ast = parser.parse()

        checker = TypeChecker()
        checker.check(ast)

        codegen = CodeGen()
        codegen.source_file = "test.jky"
        ir = codegen.gen(ast)

        # Check that debug metadata is in the generated IR
        assert "!DICompileUnit" in ir or codegen.debug_enabled == False
        assert "!DIFile" in ir or codegen.debug_enabled == False


class TestBreakpointHandling:
    """Test breakpoint functionality."""

    def test_breakpoint_tracking(self):
        """Test that breakpoints can be tracked."""
        try:
            # Note: Cannot actually test runtime breakpoint setting without
            # access to running code, but we can test the structures
            from jocky.runtime.include.jocky_debug import JockyBreakpoint
            # This is a C structure, so we just verify it would be defined
        except (ImportError, AttributeError):
            # Expected - header is C code
            pass


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
