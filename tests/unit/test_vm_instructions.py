"""
Tests for VM Instruction Execution
"""

import pytest
from jocky.language.bytecode import BytecodeCompiler, BytecodeOpcode


class TestVMInstructions:
    """Test individual VM instructions."""

    def test_bytecode_push_imm8(self):
        """Test PUSH_IMM8 instruction encoding."""
        compiler = BytecodeCompiler()
        compiler.emit_opcode(BytecodeOpcode.PUSH_IMM8)
        compiler.emit_u8(42)
        bytecode = bytes(compiler.bytecode)

        assert bytecode[0] == BytecodeOpcode.PUSH_IMM8
        assert bytecode[1] == 42

    def test_bytecode_push_reg(self):
        """Test PUSH_REG instruction encoding."""
        compiler = BytecodeCompiler()
        compiler.emit_opcode(BytecodeOpcode.PUSH_REG)
        compiler.emit_u8(0)
        bytecode = bytes(compiler.bytecode)

        assert bytecode[0] == BytecodeOpcode.PUSH_REG
        assert bytecode[1] == 0

    def test_bytecode_pop_reg(self):
        """Test POP_REG instruction encoding."""
        compiler = BytecodeCompiler()
        compiler.emit_opcode(BytecodeOpcode.POP_REG)
        compiler.emit_u8(0)
        bytecode = bytes(compiler.bytecode)

        assert bytecode[0] == BytecodeOpcode.POP_REG
        assert bytecode[1] == 0

    def test_bytecode_add_instruction(self):
        """Test ADD instruction encoding."""
        compiler = BytecodeCompiler()
        compiler.emit_opcode(BytecodeOpcode.PUSH_IMM8)
        compiler.emit_u8(5)
        compiler.emit_opcode(BytecodeOpcode.PUSH_IMM8)
        compiler.emit_u8(3)
        compiler.emit_opcode(BytecodeOpcode.ADD)

        bytecode = bytes(compiler.bytecode)
        assert bytecode[0] == BytecodeOpcode.PUSH_IMM8
        assert bytecode[2] == BytecodeOpcode.PUSH_IMM8
        assert bytecode[4] == BytecodeOpcode.ADD

    def test_bytecode_sub_instruction(self):
        """Test SUB instruction encoding."""
        compiler = BytecodeCompiler()
        compiler.emit_opcode(BytecodeOpcode.SUB)
        assert compiler.bytecode[0] == BytecodeOpcode.SUB

    def test_bytecode_mul_instruction(self):
        """Test MUL instruction encoding."""
        compiler = BytecodeCompiler()
        compiler.emit_opcode(BytecodeOpcode.MUL)
        assert compiler.bytecode[0] == BytecodeOpcode.MUL

    def test_bytecode_div_instruction(self):
        """Test DIV instruction encoding."""
        compiler = BytecodeCompiler()
        compiler.emit_opcode(BytecodeOpcode.DIV)
        assert compiler.bytecode[0] == BytecodeOpcode.DIV

    def test_bytecode_and_instruction(self):
        """Test AND instruction encoding."""
        compiler = BytecodeCompiler()
        compiler.emit_opcode(BytecodeOpcode.AND)
        assert compiler.bytecode[0] == BytecodeOpcode.AND

    def test_bytecode_or_instruction(self):
        """Test OR instruction encoding."""
        compiler = BytecodeCompiler()
        compiler.emit_opcode(BytecodeOpcode.OR)
        assert compiler.bytecode[0] == BytecodeOpcode.OR

    def test_bytecode_xor_instruction(self):
        """Test XOR instruction encoding."""
        compiler = BytecodeCompiler()
        compiler.emit_opcode(BytecodeOpcode.XOR)
        assert compiler.bytecode[0] == BytecodeOpcode.XOR

    def test_bytecode_not_instruction(self):
        """Test NOT instruction encoding."""
        compiler = BytecodeCompiler()
        compiler.emit_opcode(BytecodeOpcode.NOT)
        assert compiler.bytecode[0] == BytecodeOpcode.NOT

    def test_bytecode_shl_instruction(self):
        """Test SHL instruction encoding."""
        compiler = BytecodeCompiler()
        compiler.emit_opcode(BytecodeOpcode.SHL)
        assert compiler.bytecode[0] == BytecodeOpcode.SHL

    def test_bytecode_shr_instruction(self):
        """Test SHR instruction encoding."""
        compiler = BytecodeCompiler()
        compiler.emit_opcode(BytecodeOpcode.SHR)
        assert compiler.bytecode[0] == BytecodeOpcode.SHR

    def test_bytecode_load_instruction(self):
        """Test LOAD64 instruction encoding."""
        compiler = BytecodeCompiler()
        compiler.emit_opcode(BytecodeOpcode.LOAD64)
        assert compiler.bytecode[0] == BytecodeOpcode.LOAD64

    def test_bytecode_store_instruction(self):
        """Test STORE64 instruction encoding."""
        compiler = BytecodeCompiler()
        compiler.emit_opcode(BytecodeOpcode.STORE64)
        assert compiler.bytecode[0] == BytecodeOpcode.STORE64

    def test_bytecode_jmp_instruction(self):
        """Test JMP instruction encoding."""
        compiler = BytecodeCompiler()
        compiler.emit_opcode(BytecodeOpcode.JMP)
        compiler.emit_u32(0x100)
        bytecode = bytes(compiler.bytecode)

        assert bytecode[0] == BytecodeOpcode.JMP

    def test_bytecode_jz_instruction(self):
        """Test JZ instruction encoding."""
        compiler = BytecodeCompiler()
        compiler.emit_opcode(BytecodeOpcode.JZ)
        compiler.emit_u32(0x200)
        bytecode = bytes(compiler.bytecode)

        assert bytecode[0] == BytecodeOpcode.JZ

    def test_bytecode_jnz_instruction(self):
        """Test JNZ instruction encoding."""
        compiler = BytecodeCompiler()
        compiler.emit_opcode(BytecodeOpcode.JNZ)
        compiler.emit_u32(0x300)
        bytecode = bytes(compiler.bytecode)

        assert bytecode[0] == BytecodeOpcode.JNZ

    def test_bytecode_call_instruction(self):
        """Test CALL instruction encoding."""
        compiler = BytecodeCompiler()
        compiler.emit_opcode(BytecodeOpcode.CALL)
        compiler.emit_u8(0)
        bytecode = bytes(compiler.bytecode)

        assert bytecode[0] == BytecodeOpcode.CALL
        assert bytecode[1] == 0

    def test_bytecode_ret_instruction(self):
        """Test RET instruction encoding."""
        compiler = BytecodeCompiler()
        compiler.emit_opcode(BytecodeOpcode.RET)
        assert compiler.bytecode[0] == BytecodeOpcode.RET

    def test_bytecode_halt_instruction(self):
        """Test HALT instruction encoding."""
        compiler = BytecodeCompiler()
        compiler.emit_opcode(BytecodeOpcode.HALT)
        assert compiler.bytecode[0] == BytecodeOpcode.HALT

    def test_bytecode_nop_instruction(self):
        """Test NOP instruction encoding."""
        compiler = BytecodeCompiler()
        compiler.emit_opcode(BytecodeOpcode.NOP)
        assert compiler.bytecode[0] == BytecodeOpcode.NOP


class TestInstructionCombinations:
    """Test combinations of instructions."""

    def test_arithmetic_sequence(self):
        """Test a sequence of arithmetic instructions."""
        compiler = BytecodeCompiler()
        compiler.emit_push_imm(10)
        compiler.emit_push_imm(5)
        compiler.emit_opcode(BytecodeOpcode.ADD)
        compiler.emit_push_imm(2)
        compiler.emit_opcode(BytecodeOpcode.MUL)

        bytecode = bytes(compiler.bytecode)
        assert len(bytecode) > 0

    def test_memory_sequence(self):
        """Test a sequence of memory instructions."""
        compiler = BytecodeCompiler()
        compiler.emit_push_imm(0x1000)
        compiler.emit_push_imm(42)
        compiler.emit_opcode(BytecodeOpcode.STORE64)
        compiler.emit_push_imm(0x1000)
        compiler.emit_opcode(BytecodeOpcode.LOAD64)

        bytecode = bytes(compiler.bytecode)
        assert len(bytecode) > 0

    def test_control_flow_sequence(self):
        """Test a sequence of control flow instructions."""
        compiler = BytecodeCompiler()
        compiler.emit_push_imm(1)
        compiler.emit_opcode(BytecodeOpcode.JNZ)
        compiler.emit_u32(10)
        compiler.emit_push_imm(0)
        compiler.emit_opcode(BytecodeOpcode.JMP)
        compiler.emit_u32(15)
        compiler.emit_push_imm(1)

        bytecode = bytes(compiler.bytecode)
        assert len(bytecode) > 0
