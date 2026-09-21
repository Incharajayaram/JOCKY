from dataclasses import dataclass
from typing import Optional, Any, List

# --- Types ---

@dataclass
class JType:
    name: str
    is_pointer: bool = False

    def __str__(self):
        return self.name + ("*" if self.is_pointer else "")

    def llvm_type(self) -> str:
        if self.name == "void":
            return "void"
        if self.name == "bool":
            base = "i1"
        elif self.name == "i8":
            base = "i8"
        elif self.name == "i32":
            base = "i32"
        elif self.name == "i64":
            base = "i64"
        elif self.name == "string":
            base = "i8"
        else:
            base = f"%{self.name}"
        if self.is_pointer or self.name == "string":
            return base + "*"
        return base

    def size_bytes(self) -> int:
        if self.name in ("i8", "bool"):
            return 1
        if self.name == "i32":
            return 4
        if self.name == "i64":
            return 8
        if self.name == "string":
            return 8
        return 8

# --- AST Nodes ---

@dataclass
class Program:
    decls: List[Any]

@dataclass
class Param:
    name: str
    type: JType

@dataclass
class FuncDecl:
    name: str
    params: List[Param]
    ret_type: JType
    body: "Block"

@dataclass
class FFIDecl:
    name: str
    params: List[Param]
    ret_type: JType
    variadic: bool = False

@dataclass
class Block:
    stmts: List[Any]

@dataclass
class LetStmt:
    name: str
    type: Optional[JType]
    init: Any

@dataclass
class AssignStmt:
    target: Any
    value: Any

@dataclass
class IfStmt:
    cond: Any
    then_block: Block
    else_block: Optional[Block]

@dataclass
class WhileStmt:
    cond: Any
    body: Block

@dataclass
class ForStmt:
    init: Any
    cond: Any
    step: Any
    body: Block

@dataclass
class ReturnStmt:
    value: Optional[Any]

@dataclass
class ExprStmt:
    expr: Any

@dataclass
class IntLiteral:
    value: int

@dataclass
class BoolLiteral:
    value: bool

@dataclass
class StringLiteral:
    value: str

@dataclass
class VarRef:
    name: str

@dataclass
class BinaryOp:
    op: str
    left: Any
    right: Any

@dataclass
class UnaryOp:
    op: str
    operand: Any

@dataclass
class CallExpr:
    name: str
    args: List[Any]

@dataclass
class DerefExpr:
    operand: Any

@dataclass
class AddrOfExpr:
    operand: Any

@dataclass
class CastExpr:
    target_type: JType
    operand: Any

@dataclass
class IndexExpr:
    base: Any
    index: Any
