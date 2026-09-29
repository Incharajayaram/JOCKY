"""
JOCKY Bytecode Compiler
Compiles JOCKY AST to stack-based VM bytecode
"""

from .ast import *
from typing import List, Dict, Tuple, Any, Optional
import struct

class BytecodeOpcode:
    NOP = 0x00

    PUSH_IMM8 = 0x01
    PUSH_IMM16 = 0x02
    PUSH_IMM32 = 0x03
    PUSH_IMM64 = 0x04
    PUSH_REG = 0x05

    POP_REG = 0x06
    POP_DISCARD = 0x07

    MOV_REG = 0x08
    SWAP = 0x09
    DUP = 0x0A

    ADD = 0x10
    SUB = 0x11
    MUL = 0x12
    DIV = 0x13
    UDIV = 0x14
    MOD = 0x15
    UMOD = 0x16

    AND = 0x20
    OR = 0x21
    XOR = 0x22
    NOT = 0x23
    SHL = 0x24
    SHR = 0x25
    SAR = 0x26

    CMP = 0x30
    CMP_EQ = 0x31
    CMP_NE = 0x32
    CMP_LT = 0x33
    CMP_LE = 0x34
    CMP_GT = 0x35
    CMP_GE = 0x36
    CMP_ULT = 0x37
    CMP_ULE = 0x38
    CMP_UGT = 0x39
    CMP_UGE = 0x3A

    LOAD8 = 0x40
    LOAD16 = 0x41
    LOAD32 = 0x42
    LOAD64 = 0x43
    STORE8 = 0x44
    STORE16 = 0x45
    STORE32 = 0x46
    STORE64 = 0x47

    JMP = 0x50
    JZ = 0x51
    JNZ = 0x52
    JLT = 0x53
    JLE = 0x54
    JGT = 0x55
    JGE = 0x56

    CALL = 0x60
    RET = 0x61
    HALT = 0x62

    SYSCALL = 0x70


class BytecodeCompiler:
    """Compile JOCKY functions to bytecode."""

    def __init__(self):
        self.bytecode: List[int] = []
        self.labels: Dict[str, int] = {}
        self.label_refs: List[Tuple[int, str]] = []
        self.local_vars: Dict[str, int] = {}
        self.reg_counter = 0
        self.label_counter = 0

    def next_label(self, name: str = "") -> str:
        label = f"{name}.{self.label_counter}" if name else f"L{self.label_counter}"
        self.label_counter += 1
        return label

    def next_reg(self) -> int:
        reg = self.reg_counter
        self.reg_counter += 1
        if self.reg_counter >= 256:
            self.reg_counter = 0
        return reg

    def emit_u8(self, val: int):
        """Emit a single byte."""
        self.bytecode.append(val & 0xFF)

    def emit_u16(self, val: int):
        """Emit a 16-bit value."""
        for byte in struct.pack('<H', val & 0xFFFF):
            self.bytecode.append(byte)

    def emit_u32(self, val: int):
        """Emit a 32-bit value."""
        for byte in struct.pack('<I', val & 0xFFFFFFFF):
            self.bytecode.append(byte)

    def emit_u64(self, val: int):
        """Emit a 64-bit value."""
        for byte in struct.pack('<Q', val & 0xFFFFFFFFFFFFFFFF):
            self.bytecode.append(byte)

    def emit_opcode(self, opcode: int):
        """Emit an opcode."""
        self.emit_u8(opcode)

    def emit_label_ref(self, label: str):
        """Emit a reference to a label (will be resolved later)."""
        self.label_refs.append((len(self.bytecode), label))
        self.emit_u32(0)

    def define_label(self, label: str):
        """Define a label at the current position."""
        self.labels[label] = len(self.bytecode)

    def resolve_labels(self):
        """Resolve all label references."""
        bytecode_array = bytearray(self.bytecode)
        for offset, label in self.label_refs:
            if label in self.labels:
                target = self.labels[label]
                struct.pack_into('<I', bytecode_array, offset, target)
        self.bytecode = list(bytecode_array)

    def emit_push_imm(self, value: int):
        """Emit an immediate value push."""
        if -128 <= value <= 127:
            self.emit_opcode(BytecodeOpcode.PUSH_IMM8)
            self.emit_u8(value)
        elif -32768 <= value <= 32767:
            self.emit_opcode(BytecodeOpcode.PUSH_IMM16)
            self.emit_u16(value)
        elif -2147483648 <= value <= 2147483647:
            self.emit_opcode(BytecodeOpcode.PUSH_IMM32)
            self.emit_u32(value)
        else:
            self.emit_opcode(BytecodeOpcode.PUSH_IMM64)
            self.emit_u64(value)

    def compile_function(self, func: FuncDecl, obfuscate: bool = True) -> bytes:
        """Compile a function to bytecode."""
        self.bytecode = []
        self.labels = {}
        self.label_refs = []
        self.local_vars = {}
        self.reg_counter = 4

        for i, param in enumerate(func.params):
            self.local_vars[param.name] = i

        for stmt in func.body.stmts:
            self.compile_stmt(stmt)

        if func.ret_type.name == "void":
            self.emit_opcode(BytecodeOpcode.HALT)
        else:
            self.emit_opcode(BytecodeOpcode.RET)

        self.resolve_labels()

        result = bytes(self.bytecode)

        if obfuscate:
            result = self.obfuscate_bytecode(result)

        return result

    def compile_stmt(self, stmt: Any):
        """Compile a statement."""
        if isinstance(stmt, ExprStmt):
            self.compile_expr(stmt.expr)
            self.emit_opcode(BytecodeOpcode.POP_DISCARD)
        elif isinstance(stmt, LetStmt):
            self.compile_expr(stmt.init)
            reg = self.next_reg()
            self.local_vars[stmt.name] = reg
            self.emit_opcode(BytecodeOpcode.POP_REG)
            self.emit_u8(reg)
        elif isinstance(stmt, AssignStmt):
            self.compile_expr(stmt.value)
            if isinstance(stmt.target, Identifier):
                reg = self.local_vars.get(stmt.target.name, 0)
                self.emit_opcode(BytecodeOpcode.POP_REG)
                self.emit_u8(reg)
        elif isinstance(stmt, IfStmt):
            self.compile_if_stmt(stmt)
        elif isinstance(stmt, WhileStmt):
            self.compile_while_stmt(stmt)
        elif isinstance(stmt, ReturnStmt):
            if stmt.value:
                self.compile_expr(stmt.value)
            self.emit_opcode(BytecodeOpcode.RET)

    def compile_if_stmt(self, stmt: IfStmt):
        """Compile an if statement."""
        else_label = self.next_label("else")
        end_label = self.next_label("endif")

        self.compile_expr(stmt.condition)
        self.emit_opcode(BytecodeOpcode.JZ)
        self.emit_label_ref(else_label)

        for s in stmt.then_block.stmts:
            self.compile_stmt(s)

        if stmt.else_block:
            self.emit_opcode(BytecodeOpcode.JMP)
            self.emit_label_ref(end_label)

            self.define_label(else_label)
            for s in stmt.else_block.stmts:
                self.compile_stmt(s)
        else:
            self.define_label(else_label)

        self.define_label(end_label)

    def compile_while_stmt(self, stmt: WhileStmt):
        """Compile a while statement."""
        loop_label = self.next_label("loop")
        end_label = self.next_label("endloop")

        self.define_label(loop_label)
        self.compile_expr(stmt.condition)
        self.emit_opcode(BytecodeOpcode.JZ)
        self.emit_label_ref(end_label)

        for s in stmt.body.stmts:
            self.compile_stmt(s)

        self.emit_opcode(BytecodeOpcode.JMP)
        self.emit_label_ref(loop_label)

        self.define_label(end_label)

    def compile_expr(self, expr: Any):
        """Compile an expression."""
        if isinstance(expr, IntLiteral):
            self.emit_push_imm(expr.value)
        elif isinstance(expr, Identifier):
            reg = self.local_vars.get(expr.name, 0)
            self.emit_opcode(BytecodeOpcode.PUSH_REG)
            self.emit_u8(reg)
        elif isinstance(expr, BinaryOp):
            self.compile_expr(expr.left)
            self.compile_expr(expr.right)
            self.compile_binary_op(expr.op)
        elif isinstance(expr, UnaryOp):
            self.compile_expr(expr.operand)
            self.compile_unary_op(expr.op)
        elif isinstance(expr, CallExpr):
            for arg in expr.args:
                self.compile_expr(arg)
            self.emit_opcode(BytecodeOpcode.CALL)
            self.emit_u8(0)
        else:
            self.emit_opcode(BytecodeOpcode.NOP)

    def compile_binary_op(self, op: str):
        """Compile a binary operation."""
        ops = {
            '+': BytecodeOpcode.ADD,
            '-': BytecodeOpcode.SUB,
            '*': BytecodeOpcode.MUL,
            '/': BytecodeOpcode.DIV,
            '%': BytecodeOpcode.MOD,
            '&': BytecodeOpcode.AND,
            '|': BytecodeOpcode.OR,
            '^': BytecodeOpcode.XOR,
            '<<': BytecodeOpcode.SHL,
            '>>': BytecodeOpcode.SHR,
            '==': BytecodeOpcode.CMP_EQ,
            '!=': BytecodeOpcode.CMP_NE,
            '<': BytecodeOpcode.CMP_LT,
            '<=': BytecodeOpcode.CMP_LE,
            '>': BytecodeOpcode.CMP_GT,
            '>=': BytecodeOpcode.CMP_GE,
        }

        opcode = ops.get(op, BytecodeOpcode.NOP)
        self.emit_opcode(opcode)

    def compile_unary_op(self, op: str):
        """Compile a unary operation."""
        if op == '~':
            self.emit_opcode(BytecodeOpcode.NOT)
        elif op == '-':
            self.emit_push_imm(0)
            self.emit_opcode(BytecodeOpcode.SUB)

    def obfuscate_bytecode(self, bytecode: bytes) -> bytes:
        """Apply obfuscation techniques to bytecode."""
        result = bytearray(bytecode)

        i = 0
        while i < len(result):
            opcode = result[i]

            if opcode == BytecodeOpcode.PUSH_IMM32:
                result[i] = BytecodeOpcode.PUSH_IMM32
                i += 5
            elif opcode == BytecodeOpcode.PUSH_IMM64:
                result[i] = BytecodeOpcode.PUSH_IMM64
                i += 9
            else:
                i += 1

        return bytes(result)
