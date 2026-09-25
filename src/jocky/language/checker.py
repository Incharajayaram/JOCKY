from .ast import *
from .errors import TypeError as BaseTypeError, SourceRange
from typing import Dict, Any, Optional, List, Tuple

class TypeError(BaseTypeError):
    pass

class TypeChecker:
    def __init__(self):
        self.functions: Dict[str, (List[JType], JType)] = {}
        self.structs: Dict[str, StructDef] = {}  # struct_name -> StructDef
        self.enums: Dict[str, EnumDef] = {}      # enum_name -> EnumDef
        self.type_aliases: Dict[str, JType] = {} # alias_name -> JType
        self.locals: Dict[str, JType] = {}
        self.current_ret: JType = JType("void")
        self.loop_depth: int = 0

    def check(self, prog: Program):
        # First pass: collect type aliases, struct/enum and function signatures
        for decl in prog.decls:
            if isinstance(decl, TypeAlias):
                # Resolve the target type (recursively resolve aliases)
                resolved = self.resolve_type_alias(decl.target_type)
                self.type_aliases[decl.name] = resolved
            elif isinstance(decl, StructDef):
                self.structs[decl.name] = decl
            elif isinstance(decl, EnumDef):
                self.enums[decl.name] = decl
            elif isinstance(decl, FuncDecl):
                ptypes = [p.type for p in decl.params]
                self.functions[decl.name] = (ptypes, decl.ret_type, False)
            elif isinstance(decl, FFIDecl):
                ptypes = [p.type for p in decl.params]
                self.functions[decl.name] = (ptypes, decl.ret_type, decl.variadic)

        # Second pass: check bodies
        for decl in prog.decls:
            if isinstance(decl, FuncDecl):
                self.check_func(decl)

    def resolve_type_alias(self, t: JType) -> JType:
        """Resolve type alias recursively."""
        if t.name in self.type_aliases:
            # Follow the alias
            resolved = self.type_aliases[t.name]
            # Preserve pointer and array flags
            return JType(
                resolved.name,
                is_pointer=t.is_pointer or resolved.is_pointer,
                is_array=t.is_array or resolved.is_array,
                array_size=t.array_size or resolved.array_size
            )
        return t

    def check_func(self, decl: FuncDecl):
        old_locals = self.locals
        self.locals = {}
        for p in decl.params:
            self.locals[p.name] = p.type
        self.current_ret = decl.ret_type
        self.check_block(decl.body)
        self.locals = old_locals
        ptypes = [p.type for p in decl.params]
        self.functions[decl.name] = (ptypes, decl.ret_type, False)

    def check_block(self, block: Block):
        for stmt in block.stmts:
            self.check_stmt(stmt)

    def check_stmt(self, stmt: Any):
        if isinstance(stmt, LetStmt):
            init_type = self.typeof(stmt.init, type_hint=stmt.type)
            if stmt.type is not None:
                if not self.types_equal(stmt.type, init_type):
                    raise TypeError(f"Type mismatch in let: expected {stmt.type}, got {init_type}")
                self.locals[stmt.name] = stmt.type
            else:
                self.locals[stmt.name] = init_type
        elif isinstance(stmt, AssignStmt):
            lt = self.typeof(stmt.target)
            rt = self.typeof(stmt.value)
            if not self.types_equal(lt, rt):
                raise TypeError(f"Type mismatch in assignment: {lt} vs {rt}")
        elif isinstance(stmt, IfStmt):
            ct = self.typeof(stmt.cond)
            if ct.name != "bool":
                raise TypeError("If condition must be bool")
            self.check_block(stmt.then_block)
            if stmt.else_block:
                self.check_block(stmt.else_block)
        elif isinstance(stmt, WhileStmt):
            ct = self.typeof(stmt.cond)
            if ct.name != "bool":
                raise TypeError("While condition must be bool")
            self.loop_depth += 1
            self.check_block(stmt.body)
            self.loop_depth -= 1
        elif isinstance(stmt, ForStmt):
            self.check_stmt(stmt.init)
            ct = self.typeof(stmt.cond)
            if ct.name != "bool":
                raise TypeError("For condition must be bool")
            # step can be AssignStmt, ExprStmt, or bare expression (IntLiteral(0))
            if isinstance(stmt.step, (AssignStmt, ExprStmt)):
                self.check_stmt(stmt.step)
            elif not isinstance(stmt.step, IntLiteral):
                self.typeof(stmt.step)
            self.loop_depth += 1
            self.check_block(stmt.body)
            self.loop_depth -= 1
        elif isinstance(stmt, ReturnStmt):
            if stmt.value is None:
                if self.current_ret.name != "void":
                    raise TypeError("Return value required")
            else:
                vt = self.typeof(stmt.value)
                if not self.types_equal(vt, self.current_ret):
                    raise TypeError(f"Return type mismatch: expected {self.current_ret}, got {vt}")
        elif isinstance(stmt, ExprStmt):
            self.typeof(stmt.expr)
        elif isinstance(stmt, BreakStmt):
            if self.loop_depth == 0:
                raise TypeError("'break' outside loop")
        elif isinstance(stmt, ContinueStmt):
            if self.loop_depth == 0:
                raise TypeError("'continue' outside loop")

    def typeof(self, expr: Any, type_hint: Optional[JType] = None) -> JType:
        if isinstance(expr, IntLiteral):
            if type_hint is not None and type_hint.name in ("i8", "i32", "i64") and not type_hint.is_pointer:
                return type_hint
            return JType("i32")
        elif isinstance(expr, BoolLiteral):
            return JType("bool")
        elif isinstance(expr, StringLiteral):
            return JType("string")
        elif isinstance(expr, NullLiteral):
            # null is a null pointer; compatible with any pointer via cast
            return JType("i8", is_pointer=True)
        elif isinstance(expr, VarRef):
            if expr.name in self.locals:
                # Resolve type aliases when loading variables
                var_type = self.locals[expr.name]
                return self.resolve_type_alias(var_type)
            # Check if this is an enum variant reference
            for enum_name, enum_def in self.enums.items():
                for variant in enum_def.variants:
                    if variant.name == expr.name:
                        return JType(enum_name)
            raise TypeError(f"Undefined variable: {expr.name}")
        elif isinstance(expr, BinaryOp):
            lt = self.typeof(expr.left)
            rt = self.typeof(expr.right)
            if expr.op in ("+", "-", "*", "/", "%"):
                if not self.is_numeric(lt) or not self.is_numeric(rt):
                    raise TypeError(f"Arithmetic op requires numeric types: {lt}, {rt}")
                return lt
            elif expr.op in ("==", "!=", "<", ">", "<=", ">="):
                if not self.types_equal(lt, rt):
                    raise TypeError(f"Comparison requires matching types: {lt}, {rt}")
                return JType("bool")
            elif expr.op in ("&&", "||"):
                if lt.name != "bool" or rt.name != "bool":
                    raise TypeError(f"Logical op requires bool: {lt}, {rt}")
                return JType("bool")
            elif expr.op in ("|", "^", "&"):
                if not self.is_numeric(lt) or not self.is_numeric(rt):
                    raise TypeError(f"Bitwise op requires integer types: {lt}, {rt}")
                return lt
            elif expr.op in ("<<", ">>"):
                if not self.is_numeric(lt) or not self.is_numeric(rt):
                    raise TypeError(f"Shift op requires integer types: {lt}, {rt}")
                return lt
            else:
                raise TypeError(f"Unknown binary op: {expr.op}")
        elif isinstance(expr, UnaryOp):
            t = self.typeof(expr.operand)
            if expr.op == "-":
                if not self.is_numeric(t):
                    raise TypeError("Unary - requires numeric type")
                return t
            elif expr.op == "!":
                if t.name != "bool":
                    raise TypeError("Unary ! requires bool")
                return JType("bool")
            elif expr.op == "~":
                if not self.is_numeric(t):
                    raise TypeError("Bitwise ~ requires integer type")
                return t
            else:
                raise TypeError(f"Unknown unary op: {expr.op}")
        elif isinstance(expr, CallExpr):
            if expr.name not in self.functions:
                raise TypeError(f"Undefined function: {expr.name}")
            sig = self.functions[expr.name]
            if len(sig) == 3:
                ptypes, ret, is_variadic = sig
            else:
                ptypes, ret = sig
                is_variadic = False
            min_args = len(ptypes)
            if len(expr.args) < min_args:
                raise TypeError(f"Function {expr.name} expects at least {min_args} args, got {len(expr.args)}")
            for i, (pt, arg) in enumerate(zip(ptypes, expr.args)):
                at = self.typeof(arg)
                if not self.types_equal(pt, at):
                    raise TypeError(f"Arg {i} to {expr.name}: expected {pt}, got {at}")
            return ret
        elif isinstance(expr, DerefExpr):
            t = self.typeof(expr.operand)
            if not t.is_pointer and t.name != "string":
                raise TypeError(f"Cannot dereference non-pointer: {t}")
            return JType(t.name, is_pointer=False)
        elif isinstance(expr, AddrOfExpr):
            t = self.typeof(expr.operand)
            if isinstance(expr.operand, VarRef):
                return JType(t.name, is_pointer=True)
            raise TypeError("Can only take address of variables")
        elif isinstance(expr, CastExpr):
            return expr.target_type
        elif isinstance(expr, IndexExpr):
            bt = self.typeof(expr.base)
            it = self.typeof(expr.index)
            if not self.is_numeric(it):
                raise TypeError("Index must be numeric")
            if bt.is_array:
                # Indexing into array returns element type (non-array)
                return JType(bt.name, is_pointer=False, is_array=False, array_size=0)
            elif bt.is_pointer or bt.name == "string":
                return JType(bt.name, is_pointer=False)
            raise TypeError(f"Cannot index type {bt}")
        elif isinstance(expr, FieldAccessExpr):
            obj_type = self.typeof(expr.object)
            if obj_type.name not in self.structs:
                raise TypeError(f"Cannot access field on non-struct type: {obj_type}")
            struct_def = self.structs[obj_type.name]
            for field in struct_def.fields:
                if field.name == expr.field:
                    return field.type
            raise TypeError(f"Struct {obj_type.name} has no field {expr.field}")
        elif isinstance(expr, ArrayLiteralExpr):
            if len(expr.elements) == 0:
                # Empty array - type hint required
                if type_hint is None or not type_hint.is_array:
                    raise TypeError("Empty array requires type hint")
                return type_hint
            # Get type from first element
            elem_type = self.typeof(expr.elements[0])
            # Verify all elements have same type
            for e in expr.elements[1:]:
                et = self.typeof(e)
                if not self.types_equal(elem_type, et):
                    raise TypeError(f"Array element type mismatch: {elem_type} vs {et}")
            # Return array type
            return JType(elem_type.name, is_array=True, array_size=len(expr.elements))
        elif isinstance(expr, StructLiteralExpr):
            if expr.struct_type not in self.structs:
                raise TypeError(f"Unknown struct type: {expr.struct_type}")
            struct_def = self.structs[expr.struct_type]
            # Verify all fields are present and have correct types
            for field in struct_def.fields:
                if field.name not in expr.fields:
                    raise TypeError(f"Missing field {field.name} in {expr.struct_type} initialization")
                val_type = self.typeof(expr.fields[field.name])
                if not self.types_equal(val_type, field.type):
                    raise TypeError(f"Field {field.name}: expected {field.type}, got {val_type}")
            return JType(expr.struct_type, is_pointer=False)
        elif isinstance(expr, MatchExpr):
            # Type check the scrutinee (value being matched)
            scrutinee_type = self.typeof(expr.scrutinee)

            # Check all arms and verify they return compatible types
            arm_types = []
            for arm in expr.arms:
                # Save locals before arm
                old_locals = self.locals.copy()

                # Validate pattern matches scrutinee type and get bindings
                bindings = self.check_pattern(arm.pattern, scrutinee_type)
                # Add bindings to local scope for this arm
                for binding_name, binding_type in bindings:
                    self.locals[binding_name] = binding_type

                # Type check the arm body
                arm_type = None
                for stmt in arm.body.stmts:
                    if isinstance(stmt, ReturnStmt):
                        if stmt.value:
                            arm_type = self.typeof(stmt.value)
                        else:
                            arm_type = JType("void")
                    elif isinstance(stmt, ExprStmt):
                        arm_type = self.typeof(stmt.expr)
                if arm_type is None:
                    arm_type = JType("void")
                arm_types.append(arm_type)

                # Restore locals after arm
                self.locals = old_locals

            # All arms must return same type
            if arm_types:
                first_type = arm_types[0]
                for i, arm_type in enumerate(arm_types[1:], 1):
                    if not self.types_equal(first_type, arm_type):
                        raise TypeError(f"Match arm {i} type {arm_type} doesn't match first arm type {first_type}")
                return first_type
            return JType("void")
        else:
            raise TypeError(f"Unknown expression type: {type(expr).__name__}")

    def check_pattern(self, pattern: Any, scrutinee_type: JType) -> List[Tuple[str, JType]]:
        """Verify a pattern is compatible with scrutinee type. Returns bindings (name, type)."""
        from .ast import WildcardPattern, LiteralPattern, VariantPattern

        if isinstance(pattern, WildcardPattern):
            # Wildcard matches anything
            return []
        elif isinstance(pattern, LiteralPattern):
            # Literal must match scrutinee type
            pat_type = self.typeof(pattern.value)
            if not self.types_equal(pat_type, scrutinee_type):
                raise TypeError(f"Pattern type {pat_type} doesn't match scrutinee type {scrutinee_type}")
            return []
        elif isinstance(pattern, VariantPattern):
            # Variant pattern must be an enum variant
            if scrutinee_type.name not in self.enums:
                raise TypeError(f"Cannot match enum variant on non-enum type {scrutinee_type}")
            enum_def = self.enums[scrutinee_type.name]
            # Check if variant exists
            found_variant = None
            for variant in enum_def.variants:
                if variant.name == pattern.name:
                    found_variant = variant
                    break
            if not found_variant:
                raise TypeError(f"Enum {scrutinee_type.name} has no variant {pattern.name}")

            # Check bindings match variant fields
            bindings = []
            if pattern.bindings:
                if not found_variant.fields:
                    raise TypeError(f"Variant {pattern.name} has no fields, but pattern expects {len(pattern.bindings)}")
                if len(pattern.bindings) != len(found_variant.fields):
                    raise TypeError(f"Variant {pattern.name} has {len(found_variant.fields)} fields, pattern expects {len(pattern.bindings)}")
                # Create bindings for each field
                for binding_name, field in zip(pattern.bindings, found_variant.fields):
                    bindings.append((binding_name, field.type))
            elif found_variant.fields:
                raise TypeError(f"Variant {pattern.name} has fields, but pattern provides no bindings")

            return bindings

    def is_numeric(self, t: JType) -> bool:
        return t.name in ("i8", "i32", "i64") and not t.is_pointer

    def types_equal(self, a: JType, b: JType) -> bool:
        # Check basic type equality
        if a.name != b.name:
            # null literal (i8*) is compatible with any other pointer type
            if a.is_pointer and b.is_pointer:
                if a.name == "i8" or b.name == "i8":
                    return True
            return False

        # Check pointer
        if a.is_pointer != b.is_pointer:
            return False

        # Check array
        if a.is_array != b.is_array:
            return False

        # If both are arrays, sizes must match (or one is 0 = unspecified)
        if a.is_array and b.is_array:
            if a.array_size != 0 and b.array_size != 0:
                if a.array_size != b.array_size:
                    return False

        return True
