"""
Tests for JOCKY Bytecode Compiler
"""

import pytest
from jocky.language.bytecode import BytecodeCompiler, BytecodeOpcode
from jocky.language.ast import (
    FuncDecl, Block, IntLiteral, VarRef, BinaryOp,
    Param, JType, Attribute
)


class TestBytecodeCompiler:
    """Test bytecode compilation."""

    def test_compiler_initialization(self):
        """Test compiler initialization."""
        compiler = BytecodeCompiler()
        assert compiler.bytecode == []
        assert compiler.labels == {}
        assert compiler.label_refs == []
        assert compiler.reg_counter == 0

    def test_emit_u8(self):
        """Test emitting 8-bit values."""
        compiler = BytecodeCompiler()
        compiler.emit_u8(0xFF)
        assert len(compiler.bytecode) == 1
        assert compiler.bytecode[0] == 0xFF

        compiler.emit_u8(0x42)
        assert len(compiler.bytecode) == 2
        assert compiler.bytecode[1] == 0x42

    def test_emit_u32(self):
        """Test emitting 32-bit values."""
        compiler = BytecodeCompiler()
        compiler.emit_u32(0x12345678)
        assert len(compiler.bytecode) == 4
        assert compiler.bytecode[0] == 0x78
        assert compiler.bytecode[3] == 0x12

    def test_emit_push_imm_small(self):
        """Test emitting small immediate values."""
        compiler = BytecodeCompiler()
        compiler.emit_push_imm(42)
        assert compiler.bytecode[0] == BytecodeOpcode.PUSH_IMM8
        assert compiler.bytecode[1] == 42

    def test_emit_push_imm_medium(self):
        """Test emitting 16-bit immediate values."""
        compiler = BytecodeCompiler()
        compiler.emit_push_imm(1000)
        assert compiler.bytecode[0] == BytecodeOpcode.PUSH_IMM16
        assert len(compiler.bytecode) >= 3

    def test_emit_push_imm_large(self):
        """Test emitting 32-bit immediate values."""
        compiler = BytecodeCompiler()
        compiler.emit_push_imm(100000)
        assert compiler.bytecode[0] == BytecodeOpcode.PUSH_IMM32
        assert len(compiler.bytecode) >= 5

    def test_label_definition(self):
        """Test label definition."""
        compiler = BytecodeCompiler()
        compiler.emit_u8(0x01)
        compiler.define_label("test_label")
        assert compiler.labels["test_label"] == 1

        compiler.emit_u8(0x02)
        assert len(compiler.bytecode) == 2

    def test_label_reference(self):
        """Test label reference and resolution."""
        compiler = BytecodeCompiler()
        compiler.emit_u8(BytecodeOpcode.JMP)
        compiler.emit_label_ref("target")
        compiler.emit_u8(0x00)
        compiler.define_label("target")
        compiler.emit_u8(0xFF)

        compiler.resolve_labels()
        assert len(compiler.bytecode) >= 5

    def test_next_label(self):
        """Test label generation."""
        compiler = BytecodeCompiler()
        label1 = compiler.next_label("test")
        label2 = compiler.next_label("test")
        assert label1 != label2
        assert "test" in label1
        assert "test" in label2

    def test_compile_empty_function(self):
        """Test compiling an empty function."""
        compiler = BytecodeCompiler()
        func = FuncDecl(
            name="empty",
            params=[],
            ret_type=JType("void"),
            body=Block(stmts=[]),
            attributes=None,
            is_generic=False,
            location=None
        )

        bytecode = compiler.compile_function(func, obfuscate=False)
        assert isinstance(bytecode, bytes)
        assert len(bytecode) > 0

    def test_compile_function_with_return(self):
        """Test compiling a function with return type."""
        compiler = BytecodeCompiler()
        func = FuncDecl(
            name="returns_int",
            params=[],
            ret_type=JType("i64"),
            body=Block(stmts=[]),
            attributes=None,
            is_generic=False,
            location=None
        )

        bytecode = compiler.compile_function(func, obfuscate=False)
        assert isinstance(bytecode, bytes)
        assert len(bytecode) > 0
        assert bytecode[-1] == BytecodeOpcode.RET

    def test_opcode_definitions(self):
        """Test that all opcodes are defined."""
        assert BytecodeOpcode.NOP == 0x00
        assert BytecodeOpcode.PUSH_IMM8 == 0x01
        assert BytecodeOpcode.ADD == 0x10
        assert BytecodeOpcode.HALT == 0x62

    def test_bytecode_length_preservation(self):
        """Test that bytecode length is preserved correctly."""
        compiler = BytecodeCompiler()
        initial_len = len(compiler.bytecode)
        compiler.emit_u8(0xFF)
        assert len(compiler.bytecode) == initial_len + 1

        compiler.emit_u32(0xDEADBEEF)
        assert len(compiler.bytecode) == initial_len + 5


class TestBytecodeObfuscation:
    """Test bytecode obfuscation."""

    def test_obfuscate_bytecode(self):
        """Test bytecode obfuscation."""
        compiler = BytecodeCompiler()
        bytecode = bytes([0x01, 0x10, 0x00, 0x00, 0x00, 0x05])
        obfuscated = compiler.obfuscate_bytecode(bytecode)
        assert isinstance(obfuscated, bytes)

    def test_obfuscation_preserves_length(self):
        """Test that obfuscation preserves bytecode length."""
        compiler = BytecodeCompiler()
        bytecode = bytes([0x10, 0x42] * 10)
        obfuscated = compiler.obfuscate_bytecode(bytecode)
        assert len(obfuscated) == len(bytecode)


class TestBytecodeOperandEncoding:
    """Test operand encoding."""

    def test_emit_opcode(self):
        """Test emitting opcodes."""
        compiler = BytecodeCompiler()
        compiler.emit_opcode(BytecodeOpcode.ADD)
        assert compiler.bytecode[0] == BytecodeOpcode.ADD

    def test_register_numbering(self):
        """Test register counter."""
        compiler = BytecodeCompiler()
        reg1 = compiler.next_reg()
        reg2 = compiler.next_reg()
        assert reg1 < reg2
        assert reg1 >= 0
        assert reg2 < 256
