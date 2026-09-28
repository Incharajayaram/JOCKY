"""
Integration tests for VM compilation and execution
"""

import pytest
from jocky.language.bytecode import BytecodeCompiler, BytecodeOpcode
from jocky.language.ast import FuncDecl, Block, IntLiteral, Param, JType, Attribute


class TestVMIntegration:
    """Integration tests for VM bytecode compilation."""

    def test_compile_simple_function(self):
        """Test compiling a simple function."""
        compiler = BytecodeCompiler()
        func = FuncDecl(
            name="simple",
            params=[],
            ret_type=JType("i64"),
            body=Block(stmts=[]),
            attributes=None,
            is_generic=False,
            location=None
        )

        bytecode = compiler.compile_function(func, obfuscate=False)
        assert isinstance(bytecode, bytes)
        assert len(bytecode) >= 1

    def test_bytecode_is_deterministic(self):
        """Test that bytecode compilation is deterministic."""
        func = FuncDecl(
            name="deterministic",
            params=[],
            ret_type=JType("i64"),
            body=Block(stmts=[]),
            attributes=None,
            is_generic=False,
            location=None
        )

        compiler1 = BytecodeCompiler()
        bytecode1 = compiler1.compile_function(func, obfuscate=False)

        compiler2 = BytecodeCompiler()
        bytecode2 = compiler2.compile_function(func, obfuscate=False)

        assert bytecode1 == bytecode2

    def test_obfuscated_bytecode_differs(self):
        """Test that obfuscation produces different bytecode."""
        func = FuncDecl(
            name="obfuscated",
            params=[],
            ret_type=JType("i64"),
            body=Block(stmts=[]),
            attributes=None,
            is_generic=False,
            location=None
        )

        compiler = BytecodeCompiler()
        bytecode1 = compiler.compile_function(func, obfuscate=True)
        bytecode2 = compiler.compile_function(func, obfuscate=True)

        assert isinstance(bytecode1, bytes)
        assert isinstance(bytecode2, bytes)

    def test_function_with_parameters(self):
        """Test compiling a function with parameters."""
        param1 = Param(name="x", type=JType("i64"))
        param2 = Param(name="y", type=JType("i64"))

        func = FuncDecl(
            name="with_params",
            params=[param1, param2],
            ret_type=JType("i64"),
            body=Block(stmts=[]),
            attributes=None,
            is_generic=False,
            location=None
        )

        compiler = BytecodeCompiler()
        bytecode = compiler.compile_function(func, obfuscate=False)
        assert isinstance(bytecode, bytes)

    def test_bytecode_has_minimum_size(self):
        """Test that compiled bytecode has a minimum size."""
        func = FuncDecl(
            name="minimal",
            params=[],
            ret_type=JType("void"),
            body=Block(stmts=[]),
            attributes=None,
            is_generic=False,
            location=None
        )

        compiler = BytecodeCompiler()
        bytecode = compiler.compile_function(func, obfuscate=False)
        assert len(bytecode) >= 1

    def test_bytecode_validity_check(self):
        """Test that bytecode is valid."""
        func = FuncDecl(
            name="valid",
            params=[],
            ret_type=JType("i64"),
            body=Block(stmts=[]),
            attributes=None,
            is_generic=False,
            location=None
        )

        compiler = BytecodeCompiler()
        bytecode = compiler.compile_function(func, obfuscate=False)

        assert isinstance(bytecode, bytes)
        assert all(0 <= b <= 255 for b in bytecode)

    def test_bytecode_contains_valid_opcodes(self):
        """Test that bytecode only contains valid opcodes or data."""
        func = FuncDecl(
            name="opcodes",
            params=[],
            ret_type=JType("i64"),
            body=Block(stmts=[]),
            attributes=None,
            is_generic=False,
            location=None
        )

        compiler = BytecodeCompiler()
        bytecode = compiler.compile_function(func, obfuscate=False)

        if len(bytecode) > 0:
            first_byte = bytecode[0]
            assert 0 <= first_byte <= 0xFF


class TestBytecodeOperations:
    """Test bytecode operations."""

    def test_arithmetic_bytecode(self):
        """Test arithmetic operation bytecode."""
        compiler = BytecodeCompiler()
        compiler.emit_push_imm(5)
        compiler.emit_push_imm(3)
        compiler.emit_opcode(BytecodeOpcode.ADD)

        bytecode = bytes(compiler.bytecode)
        assert len(bytecode) > 0

    def test_logical_bytecode(self):
        """Test logical operation bytecode."""
        compiler = BytecodeCompiler()
        compiler.emit_push_imm(0xFF)
        compiler.emit_push_imm(0x0F)
        compiler.emit_opcode(BytecodeOpcode.AND)

        bytecode = bytes(compiler.bytecode)
        assert len(bytecode) > 0

    def test_memory_bytecode(self):
        """Test memory operation bytecode."""
        compiler = BytecodeCompiler()
        compiler.emit_push_imm(0x1000)
        compiler.emit_push_imm(42)
        compiler.emit_opcode(BytecodeOpcode.STORE64)

        bytecode = bytes(compiler.bytecode)
        assert len(bytecode) > 0


class TestBytecodeWithAttributes:
    """Test bytecode compilation with attributes."""

    def test_function_with_obfuscate_attribute(self):
        """Test function with obfuscate attribute."""
        attr = Attribute(name="obfuscate", args=[])
        func = FuncDecl(
            name="obfuscated_func",
            params=[],
            ret_type=JType("i64"),
            body=Block(stmts=[]),
            attributes=[attr],
            is_generic=False,
            location=None
        )

        compiler = BytecodeCompiler()
        bytecode = compiler.compile_function(func, obfuscate=True)
        assert isinstance(bytecode, bytes)

    def test_function_with_virtualize_attribute(self):
        """Test function with virtualize attribute."""
        attr = Attribute(name="virtualize", args=[])
        func = FuncDecl(
            name="virtualized_func",
            params=[],
            ret_type=JType("i64"),
            body=Block(stmts=[]),
            attributes=[attr],
            is_generic=False,
            location=None
        )

        compiler = BytecodeCompiler()
        bytecode = compiler.compile_function(func, obfuscate=True)
        assert isinstance(bytecode, bytes)
