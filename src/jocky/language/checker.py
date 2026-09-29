from .ast import *
from .errors import TypeError as BaseTypeError, SourceRange
from typing import Dict, Any, Optional, List, Tuple, Set
try:
    from jocky.core.modules import Module, ModuleRegistry, get_registry
except ImportError:
    from ..core.modules import Module, ModuleRegistry, get_registry

class TypeError(BaseTypeError):
    pass

class GenericContext:
    """Tracks type variable bindings during generic function/struct checking."""
    def __init__(self, parent: Optional['GenericContext'] = None):
        self.bindings: Dict[str, JType] = {}  # T -> i32, U -> string, etc.
        self.parent = parent

    def bind(self, name: str, jtype: JType):
        """Bind a type variable to a concrete type."""
        self.bindings[name] = jtype

    def lookup(self, name: str) -> Optional[JType]:
        """Look up a type variable."""
        if name in self.bindings:
            return self.bindings[name]
        if self.parent:
            return self.parent.lookup(name)
        return None

    def child(self) -> 'GenericContext':
        """Create a child context for nested scopes."""
        return GenericContext(parent=self)

class Monomorphization:
    """Represents a generic function/struct instantiation."""
    def __init__(self, base_name: str, type_bindings: Dict[str, JType]):
        self.base_name = base_name
        self.type_bindings = type_bindings
        self.mangled_name = self._mangle()

    def _mangle(self) -> str:
        """Generate a unique mangled name for this instantiation."""
        if not self.type_bindings:
            return self.base_name
        type_names = [self.type_bindings[k].name for k in sorted(self.type_bindings.keys())]
        return f"{self.base_name}_{'_'.join(type_names)}"

    def __hash__(self):
        items = tuple(sorted((k, v.name) for k, v in self.type_bindings.items()))
        return hash((self.base_name, items))

    def __eq__(self, other):
        return isinstance(other, Monomorphization) and \
               self.base_name == other.base_name and \
               self.type_bindings == other.type_bindings

class TypeChecker:
    def __init__(self, module_registry: Optional[ModuleRegistry] = None):
        self.functions: Dict[str, (List[JType], JType)] = {}
        self.structs: Dict[str, StructDef] = {}  # struct_name -> StructDef
        self.enums: Dict[str, EnumDef] = {}      # enum_name -> EnumDef
        self.type_aliases: Dict[str, JType] = {} # alias_name -> JType
        self.locals: Dict[str, JType] = {}
        self.current_ret: JType = JType("void")
        self.loop_depth: int = 0

        self.globals: Dict[str, JType] = {}

        self.generic_context: GenericContext = GenericContext()
        self.monomorphizations: Set[Monomorphization] = set()

        self.module_registry = module_registry or get_registry()
        self.current_module: Optional[Module] = None
        self.visible_symbols: Dict[str, Any] = {}

    def check(self, prog: Program):
        # Store all declarations for generic resolution
        self._all_decls = prog.decls

        # Phase 0: Process modules and imports
        for decl in prog.decls:
            if isinstance(decl, ModDecl):
                self.process_module_decl(decl)
            elif isinstance(decl, UseStmt):
                self.process_use_stmt(decl)

        # First pass: collect type aliases, struct/enum and function signatures
        for decl in prog.decls:
            if isinstance(decl, UseStmt):
                # Module imports are already resolved; skip them
                pass
            elif isinstance(decl, TypeAlias):
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
            elif isinstance(decl, (ConstDecl, VarDecl)):
                try:
                    init_type = self.typeof(decl.init)
                except Exception:
                    init_type = JType("i64")
                t = decl.type if decl.type else init_type
                self.globals[decl.name] = t

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

    def substitute_type(self, jtype: JType, context: GenericContext = None) -> JType:
        """Substitute type variables with concrete types using generic context."""
        if context is None:
            context = self.generic_context

        # Check if this type should be substituted (either marked as generic or found in context)
        if jtype.is_generic or context.lookup(jtype.name):
            concrete = context.lookup(jtype.name)
            if concrete:
                # Preserve pointer and array flags
                return JType(
                    concrete.name,
                    is_pointer=jtype.is_pointer or concrete.is_pointer,
                    is_array=jtype.is_array or concrete.is_array,
                    array_size=jtype.array_size or concrete.array_size,
                    is_generic=False
                )
        return jtype

    def resolve_generic_call(self, base_name: str, type_args: List[JType]) -> str:
        """Generate mangled name for a generic function call.

        Example: max<i32> -> max_i32
        """
        if not type_args:
            return base_name
        type_names = [t.name for t in type_args]
        return f"{base_name}_{'_'.join(type_names)}"

    def process_module_decl(self, mod_decl: ModDecl):
        """Process a module declaration."""
        module = Module(mod_decl.name)
        self.module_registry.register_module(module)

        # Process declarations within the module
        old_module = self.current_module
        self.current_module = module

        for item in mod_decl.items:
            if isinstance(item, FuncDecl):
                ptypes = [p.type for p in item.params]
                module.add_symbol(item.name, (ptypes, item.ret_type, False), public=True)
            elif isinstance(item, StructDef):
                module.add_symbol(item.name, item, public=True)
            elif isinstance(item, EnumDef):
                module.add_symbol(item.name, item, public=True)

        self.current_module = old_module

    def process_use_stmt(self, use_stmt: UseStmt):
        module_path = use_stmt.path
        module_name = str(module_path)
        target_module = self.module_registry.get_module(module_name)

        if target_module is None:
            return

        # Import symbols
        if use_stmt.all:
            # Import all public symbols
            for name, symbol in target_module.symbols.items():
                if symbol.public:
                    self.visible_symbols[name] = symbol
        else:
            # Import specific symbol
            symbol_name = use_stmt.symbol
            symbol = target_module.get_symbol(symbol_name)

            if symbol is None:
                raise TypeError(f"Cannot find symbol '{symbol_name}' in module {module_name}")

            if not symbol.public:
                raise TypeError(f"Cannot access private symbol '{symbol_name}' from module {module_name}")

            self.visible_symbols[symbol_name] = symbol

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
                pass
        elif isinstance(stmt, IfStmt):
            self.typeof(stmt.cond)
            self.check_block(stmt.then_block)
            if stmt.else_block:
                self.check_block(stmt.else_block)
        elif isinstance(stmt, WhileStmt):
            self.typeof(stmt.cond)
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
        elif isinstance(stmt, ForInStmt):
            iter_type = self.typeof(stmt.iterable)
            if iter_type.is_array:
                elem_type = JType(iter_type.name, is_pointer=False, is_array=False)
            elif iter_type.is_pointer:
                elem_type = JType(iter_type.name, is_pointer=False)
            else:
                elem_type = iter_type
            self.locals[stmt.var_name] = elem_type
            self.loop_depth += 1
            self.check_block(stmt.body)
            self.loop_depth -= 1
        elif isinstance(stmt, ReturnStmt):
            if stmt.value is not None:
                self.typeof(stmt.value)
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
        elif isinstance(expr, FloatLiteral):
            if type_hint is not None and type_hint.name in ("f32", "f64") and not type_hint.is_pointer:
                return type_hint
            return JType("f64")
        elif isinstance(expr, BoolLiteral):
            return JType("bool")
        elif isinstance(expr, StringLiteral):
            return JType("string")
        elif isinstance(expr, NullLiteral):
            # null is a null pointer; compatible with any pointer via cast
            return JType("i8", is_pointer=True)
        elif isinstance(expr, VarRef):
            if expr.name in self.locals:
                var_type = self.locals[expr.name]
                return self.resolve_type_alias(var_type)
            if expr.name in self.globals:
                return self.resolve_type_alias(self.globals[expr.name])
            for enum_name, enum_def in self.enums.items():
                for variant in enum_def.variants:
                    if variant.name == expr.name:
                        return JType(enum_name)
            raise TypeError(f"Undefined variable: {expr.name}")
        elif isinstance(expr, BinaryOp):
            lt = self.typeof(expr.left)
            rt = self.typeof(expr.right)
            if expr.op in ("+", "-", "*", "/", "%"):
                if lt.name == "string" or rt.name == "string":
                    return JType("string")
                if not self.is_numeric(lt) or not self.is_numeric(rt):
                    return lt
                return lt
            elif expr.op in ("==", "!=", "<", ">", "<=", ">="):
                return JType("bool")
            elif expr.op in ("&&", "||"):
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
            # Handle generic function calls: func::<T1, T2>(args)
            if expr.generic_args:
                if expr.name not in self.functions:
                    raise TypeError(f"Undefined generic function: {expr.name}")

                base_sig = self.functions[expr.name]
                if len(base_sig) == 3:
                    ptypes, ret, is_variadic = base_sig
                else:
                    ptypes, ret = base_sig
                    is_variadic = False

                # Find the generic function definition to get type parameters
                generic_func = None
                for decl in getattr(self, '_all_decls', []):
                    if isinstance(decl, FuncDecl) and decl.name == expr.name:
                        if decl.generic_params:
                            generic_func = decl
                            break

                if not generic_func:
                    raise TypeError(f"Function {expr.name} is not generic")

                if len(expr.generic_args) != len(generic_func.generic_params):
                    raise TypeError(f"Function {expr.name} expects {len(generic_func.generic_params)} type args, got {len(expr.generic_args)}")

                # Create type bindings: T -> i32, U -> string, etc.
                type_context = GenericContext()
                for param, arg_type in zip(generic_func.generic_params, expr.generic_args):
                    param_name = param if isinstance(param, str) else param.name
                    type_context.bind(param_name, arg_type)

                # Substitute types in function signature
                substituted_ptypes = [self.substitute_type(pt, type_context) for pt in ptypes]
                substituted_ret = self.substitute_type(ret, type_context)

                # Type-check arguments with substituted types
                min_args = len(substituted_ptypes)
                if len(expr.args) < min_args:
                    raise TypeError(f"Function {expr.name} expects at least {min_args} args, got {len(expr.args)}")
                for i, (pt, arg) in enumerate(zip(substituted_ptypes, expr.args)):
                    at = self.typeof(arg, type_hint=pt)
                    if not self.types_equal(pt, at):
                        raise TypeError(f"Arg {i} to {expr.name}: expected {pt}, got {at}")

                # Record monomorphization for codegen
                type_bindings = {}
                for param, arg_type in zip(generic_func.generic_params, expr.generic_args):
                    param_name = param if isinstance(param, str) else param.name
                    type_bindings[param_name] = arg_type
                mono = Monomorphization(expr.name, type_bindings)
                self.monomorphizations.add(mono)

                return substituted_ret
            else:
                # Non-generic function call
                if expr.name in self.functions:
                    sig = self.functions[expr.name]
                elif expr.name in self.visible_symbols:
                    symbol = self.visible_symbols[expr.name]
                    sig = symbol.type_info
                else:
                    raise TypeError(f"Undefined function: {expr.name}")

                if len(sig) == 3:
                    ptypes, ret, is_variadic = sig
                else:
                    ptypes, ret = sig
                    is_variadic = False
                for arg in expr.args:
                    self.typeof(arg)
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
                if type_hint is not None:
                    return type_hint
                return JType("i8", is_pointer=True, is_array=True, array_size=0)
            elem_type = self.typeof(expr.elements[0])
            for e in expr.elements[1:]:
                self.typeof(e)
            return JType(elem_type.name, is_array=True, array_size=len(expr.elements),
                         is_pointer=elem_type.is_pointer or elem_type.is_array)
        elif isinstance(expr, TupleExpr):
            if len(expr.elements) == 0:
                return JType("i8", is_pointer=True)
            elem_type = self.typeof(expr.elements[0])
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
        elif isinstance(expr, VariantConstructor):
            # Extract enum type and variant name from "EnumType::VariantName"
            parts = expr.variant_name.split("::")
            if len(parts) != 2:
                raise TypeError(f"Invalid variant name: {expr.variant_name}")
            enum_name, variant_name = parts

            if enum_name not in self.enums:
                raise TypeError(f"Unknown enum type: {enum_name}")

            enum_def = self.enums[enum_name]
            # Find the variant
            variant = None
            for v in enum_def.variants:
                if v.name == variant_name:
                    variant = v
                    break

            if variant is None:
                raise TypeError(f"Unknown variant {variant_name} in enum {enum_name}")

            # Verify arguments match variant fields
            if variant.fields:
                if len(expr.args) != len(variant.fields):
                    raise TypeError(f"Variant {variant_name} expects {len(variant.fields)} args, got {len(expr.args)}")
                for i, (arg, field) in enumerate(zip(expr.args, variant.fields)):
                    arg_type = self.typeof(arg)
                    if not self.types_equal(arg_type, field.type):
                        raise TypeError(f"Arg {i}: expected {field.type}, got {arg_type}")
            else:
                if len(expr.args) != 0:
                    raise TypeError(f"Variant {variant_name} expects 0 args, got {len(expr.args)}")

            return JType(enum_name)
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
        elif isinstance(expr, ClosureExpr):
            # Type check closure/lambda
            # Save current locals
            saved_locals = self.locals.copy()

            # Register closure parameters as locals
            param_types = []
            for param in expr.params:
                if param.type is None:
                    # Type will be inferred from context - for now, assume i32
                    param.type = JType("i32")
                param_types.append(param.type)
                self.locals[param.name] = param.type

            # Type check closure body
            if isinstance(expr.body, Block):
                body_type = JType("void")
                for stmt in expr.body.stmts:
                    if isinstance(stmt, ReturnStmt):
                        if stmt.value:
                            body_type = self.typeof(stmt.value)
                        else:
                            body_type = JType("void")
                    elif isinstance(stmt, ExprStmt):
                        body_type = self.typeof(stmt.expr)
            else:
                # Single expression body
                body_type = self.typeof(expr.body)

            # Determine return type
            if expr.ret_type is None:
                ret_type = body_type
            else:
                ret_type = expr.ret_type

            # Restore locals
            self.locals = saved_locals

            # Return a function pointer type (represented as i8* for now)
            # A proper implementation would have a distinct function type
            return JType("i8", is_pointer=True)
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
        return t.name in ("i8", "i32", "i64", "f32", "f64") and not t.is_pointer

    def types_equal(self, a: JType, b: JType) -> bool:
        if a.name != b.name:
            if a.is_pointer and b.is_pointer:
                if a.name == "i8" or b.name == "i8":
                    return True
            if self.is_numeric(a) and self.is_numeric(b):
                return True
            if a.name == "string" and b.name == "i8" and b.is_pointer:
                return True
            if b.name == "string" and a.name == "i8" and a.is_pointer:
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
