from dataclasses import dataclass
from typing import Optional, Any, List

# --- Types ---

@dataclass
class JType:
    name: str
    is_pointer: bool = False
    is_array: bool = False
    array_size: int = 0  # 0 means not an array or unsized

    def __str__(self):
        base = self.name
        if self.is_array:
            base += f"[{self.array_size}]" if self.array_size > 0 else "[]"
        if self.is_pointer:
            base += "*"
        return base

    def llvm_type(self) -> str:
        """Generate LLVM IR type representation."""
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
            # User-defined struct
            base = f"%{self.name}"

        # Handle array
        if self.is_array:
            if self.array_size > 0:
                base = f"[{self.array_size} x {base}]"
            else:
                # Dynamic array - represent as pointer
                base = base + "*"

        # Handle pointer
        if self.is_pointer or self.name == "string":
            return base + "*"
        return base

    def size_bytes(self, struct_defs: dict = None) -> int:
        """Estimate size in bytes. Requires struct_defs for user-defined types."""
        if self.name in ("i8", "bool"):
            size = 1
        elif self.name == "i32":
            size = 4
        elif self.name == "i64":
            size = 8
        elif self.name == "string":
            size = 8
        elif self.is_pointer:
            size = 8
        elif self.name in struct_defs or True:  # User-defined struct
            size = 8  # Placeholder: should calculate from fields
        else:
            size = 8

        if self.is_array and self.array_size > 0:
            size *= self.array_size

        return size

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
class StructField:
    name: str
    type: JType

@dataclass
class StructDef:
    name: str
    fields: List[StructField]

@dataclass
class ArrayType:
    element_type: JType
    size: int  # 0 means unsized / inferred

@dataclass
class EnumVariant:
    name: str
    value: Optional[int] = None

@dataclass
class EnumDef:
    name: str
    variants: List[EnumVariant]

@dataclass
class TypeAlias:
    name: str
    target_type: JType

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

@dataclass
class BreakStmt:
    pass

@dataclass
class ContinueStmt:
    pass

@dataclass
class NullLiteral:
    pass

@dataclass
class FieldAccessExpr:
    object: Any
    field: str

@dataclass
class StructLiteralExpr:
    struct_type: str
    fields: dict  # field_name -> value

@dataclass
class ArrayLiteralExpr:
    elements: List[Any]

@dataclass
class Pattern:
    """Base class for patterns"""
    pass

@dataclass
class WildcardPattern(Pattern):
    """Match anything: _"""
    pass

@dataclass
class LiteralPattern(Pattern):
    """Match a literal value"""
    value: Any  # IntLiteral, BoolLiteral, etc.

@dataclass
class VariantPattern(Pattern):
    """Match an enum variant: EnumName"""
    name: str

@dataclass
class MatchArm:
    """One arm of a match expression"""
    pattern: Pattern
    body: Block

@dataclass
class MatchExpr:
    """Match expression: match expr { pattern => body, ... }"""
    scrutinee: Any  # The expression being matched
    arms: List[MatchArm]
