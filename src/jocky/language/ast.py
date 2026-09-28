from dataclasses import dataclass
from typing import Optional, Any, List

# --- Types ---

@dataclass
class TypeVar:
    """Represents a generic type variable: T, U, V, etc."""
    name: str

    def __str__(self):
        return self.name

@dataclass
class JType:
    name: str
    is_pointer: bool = False
    is_array: bool = False
    array_size: int = 0  # 0 means not an array or unsized
    is_generic: bool = False  # True if this is a type variable like T, U, etc.

    def __str__(self):
        base = self.name
        if self.is_array:
            base += f"[{self.array_size}]" if self.array_size > 0 else "[]"
        if self.is_pointer:
            base += "*"
        return base

    @staticmethod
    def generic(name: str) -> "JType":
        """Create a generic type variable."""
        return JType(name, is_generic=True)

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
        elif self.name == "f32":
            base = "float"
        elif self.name == "f64":
            base = "double"
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
        elif self.name in ("i32", "f32"):
            size = 4
        elif self.name in ("i64", "f64"):
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
class Attribute:
    """Function/type attribute: #[name(args)]"""
    name: str
    args: List[str] = None  # Optional arguments

    def __post_init__(self):
        if self.args is None:
            self.args = []

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
    attributes: List["Attribute"] = None  # #[inline], #[no_mangle], etc.
    generic_params: List[str] = None  # Type parameter names: [T, U, V]

    def __post_init__(self):
        if self.attributes is None:
            self.attributes = []
        if self.generic_params is None:
            self.generic_params = []

@dataclass
class FFIDecl:
    name: str
    params: List[Param]
    ret_type: JType
    variadic: bool = False
    attributes: List[Attribute] = None  # #[no_mangle], etc.

    def __post_init__(self):
        if self.attributes is None:
            self.attributes = []

@dataclass
class StructField:
    name: str
    type: JType

@dataclass
class StructDef:
    name: str
    fields: List[StructField]
    attributes: List[Attribute] = None  # #[packed], #[repr], etc.

    def __post_init__(self):
        if self.attributes is None:
            self.attributes = []

@dataclass
class ArrayType:
    element_type: JType
    size: int  # 0 means unsized / inferred

@dataclass
class EnumVariant:
    name: str
    value: Optional[int] = None
    fields: Optional[List['StructField']] = None  # For tagged unions

@dataclass
class EnumDef:
    name: str
    variants: List[EnumVariant]
    attributes: List[Attribute] = None  # #[repr], etc.

    def __post_init__(self):
        if self.attributes is None:
            self.attributes = []

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
class ConstDecl:
    name: str
    type: Optional[JType]
    init: Any

@dataclass
class VarDecl:
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
class ForInStmt:
    var_name: str
    iterable: Any
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
class FloatLiteral:
    value: float

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
    generic_args: Optional[List[Any]] = None

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
class TupleExpr:
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
    """Match an enum variant: EnumName or EnumName(x, y, ...)"""
    name: str
    bindings: Optional[List[str]] = None  # For tagged union field bindings

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

@dataclass
class VariantConstructor:
    """Construct a tagged union variant: Ok(value) or Error(code, msg)"""
    variant_name: str
    args: List[Any]  # Arguments for variant fields


@dataclass
class ModulePath:
    """Represents a module path: mod::submod::name"""
    components: List[str]

    def __str__(self):
        return "::".join(self.components)

    @staticmethod
    def from_string(path_str: str):
        return ModulePath(path_str.split("::"))


@dataclass
class UseStmt:
    """Import statement: use module::symbol or use module::*"""
    path: ModulePath
    all: bool = False  # True for use module::*, False for specific symbol

    @property
    def module_path(self) -> ModulePath:
        if self.all:
            return self.path
        return ModulePath(self.path.components[:-1])

    @property
    def symbol(self) -> Optional[str]:
        if self.all:
            return None
        return self.path.components[-1] if self.path.components else None


@dataclass
class ModDecl:
    """Module declaration: mod name { items }"""
    name: str
    items: List[Any]  # Functions, structs, enums, other modules
    public: bool = False  # pub mod vs private mod


@dataclass
class ClosureExpr:
    """Closure/lambda expression: |args| -> type { body }"""
    params: List[Param]
    ret_type: Optional[JType]
    captures: List[str]  # Variables captured from outer scope
    body: Any  # Expression or Block


@dataclass
class CaptureVar:
    """Captured variable in closure: var or ref var"""
    name: str
    by_ref: bool = False  # True for &var (reference), False for var (value)
