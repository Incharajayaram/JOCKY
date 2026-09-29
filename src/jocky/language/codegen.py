from .ast import *
from .errors import CodeGenError as BaseCodeGenError, SourceRange
from typing import Dict, List, Tuple, Any, Optional

class CodeGenError(BaseCodeGenError):
    pass

class CodeGen:
    def __init__(self):
        self.output_lines: List[str] = []
        self.string_constants: Dict[str, str] = {}
        self.string_counter = 0
        self.reg_counter = 0
        self.label_counter = 0
        self.current_func = ""
        self.current_block = "entry"
        self.locals: Dict[str, Tuple[str, JType]] = {}
        self.globals: Dict[str, Tuple[str, JType]] = {}
        self.functions: Dict[str, (List[JType], JType)] = {}
        self.structs: Dict[str, StructDef] = {}
        self.enums: Dict[str, EnumDef] = {}
        self.type_aliases: Dict[str, JType] = {}
        self.loop_stack: List[Tuple[str, str]] = []
        self._all_funcs: List[FuncDecl] = []
        self.monomorphizations: set = set()
        self.global_inits: Dict[str, Any] = {}
        self.type_context = None

    def next_reg(self) -> str:
        r = f"%{self.reg_counter}"
        self.reg_counter += 1
        return r

    def next_label(self, name: str = "") -> str:
        l = f"{name}.{self.label_counter}" if name else f"L{self.label_counter}"
        self.label_counter += 1
        return l

    def emit(self, line: str):
        self.output_lines.append(line)

    def emit_label(self, label: str):
        self.output_lines.append(f"{label}:")
        self.current_block = label

    def get_string_const(self, s: str) -> Tuple[str, int]:
        if s in self.string_constants:
            name = self.string_constants[s]
        else:
            name = f"@.str.{self.string_counter}"
            self.string_counter += 1
            self.string_constants[s] = name
        length = len(s.encode("utf-8")) + 1
        return name, length

    def gen(self, prog: Program, monomorphizations: set = None) -> str:
        # Store monomorphizations from type checker
        if monomorphizations:
            self.monomorphizations = monomorphizations

        # First pass: collect signatures and definitions
        for decl in prog.decls:
            if isinstance(decl, TypeAlias):
                self.type_aliases[decl.name] = decl.target_type
            elif isinstance(decl, StructDef):
                self.structs[decl.name] = decl
            elif isinstance(decl, EnumDef):
                self.enums[decl.name] = decl
            elif isinstance(decl, FuncDecl):
                ptypes = [p.type for p in decl.params]
                self.functions[decl.name] = (ptypes, decl.ret_type, False)
                self._all_funcs.append(decl)  # Store for generic resolution
            elif isinstance(decl, FFIDecl):
                ptypes = [p.type for p in decl.params]
                self.functions[decl.name] = (ptypes, decl.ret_type, decl.variadic)
            elif isinstance(decl, (ConstDecl, VarDecl)):
                t = self.infer_global_type(decl)
                self.globals[decl.name] = (f"@{decl.name}", t)

        # Emit struct type definitions
        for struct_name, struct_def in self.structs.items():
            self.emit_struct_def(struct_def)

        # Emit global variables
        for decl in prog.decls:
            if isinstance(decl, (ConstDecl, VarDecl)):
                self.emit_global_decl(decl)

        # Emit FFI declarations (deduplicated)
        emitted_ffis: set = set()
        for decl in prog.decls:
            if isinstance(decl, FFIDecl):
                if decl.name not in emitted_ffis:
                    self.emit_ffi_decl(decl)
                    emitted_ffis.add(decl.name)

        # Emit function definitions (skip generic functions)
        for decl in prog.decls:
            if isinstance(decl, FuncDecl):
                if not decl.generic_params:  # Only emit non-generic functions
                    self.emit_func(decl)

        # Emit monomorphized functions
        for mono in self.monomorphizations:
            for func_decl in self._all_funcs:
                if func_decl.name == mono.base_name:
                    self.emit_monomorphized_func(func_decl, mono)

        # Prepend string constants
        prelude = []
        for s, name in self.string_constants.items():
            raw = s.encode("utf-8")
            length = len(raw) + 1
            parts = []
            for b in raw:
                if 32 <= b < 127 and b != ord('\\') and b != ord('"'):
                    parts.append(chr(b))
                else:
                    parts.append(f"\\{b:02X}")
            escaped = "".join(parts)
            prelude.append(f'{name} = private constant [{length} x i8] c"{escaped}\\00"')

        return "\n".join(prelude + [""] + self.output_lines)

    def emit_struct_def(self, struct_def: StructDef):
        """Emit LLVM struct type definition."""
        field_types = ", ".join(self.llvm_type(f.type) for f in struct_def.fields)
        self.emit(f"%{struct_def.name} = type {{ {field_types} }}")

    def infer_global_type(self, decl) -> JType:
        if decl.type:
            return decl.type
        try:
            return self.infer_type(decl.init)
        except Exception:
            return JType("i64")

    def emit_global_decl(self, decl):
        t = self.infer_global_type(decl)
        llvm_t = self.llvm_type(t)
        if t.name == "string" and not t.is_array:
            self.emit(f"@{decl.name} = global i8* null")
            if decl.init:
                self.global_inits[decl.name] = decl.init
        elif t.is_array or isinstance(decl.init, ArrayLiteralExpr):
            self.emit(f"@{decl.name} = global i8* null")
            stored_type = JType("i8", is_pointer=True)
            self.globals[decl.name] = (f"@{decl.name}", stored_type)
            if decl.init and isinstance(decl.init, ArrayLiteralExpr) and len(decl.init.elements) > 0:
                self.global_inits[decl.name] = decl.init
            return
        elif t.name in ("f32", "f64"):
            init_val = "0.0"
            if isinstance(decl.init, FloatLiteral):
                init_val = f"{decl.init.value:#.17g}"
            elif isinstance(decl.init, IntLiteral):
                init_val = f"{float(decl.init.value):#.17g}"
            self.emit(f"@{decl.name} = global {llvm_t} {init_val}")
        elif t.name == "bool":
            init_val = "1" if isinstance(decl.init, BoolLiteral) and decl.init.value else "0"
            self.emit(f"@{decl.name} = global i1 {init_val}")
        else:
            init_val = "0"
            if isinstance(decl.init, IntLiteral):
                init_val = str(decl.init.value)
            self.emit(f"@{decl.name} = global {llvm_t} {init_val}")

    def emit_ffi_decl(self, decl: FFIDecl):
        params = ", ".join(self.llvm_type(t) for t in [p.type for p in decl.params])
        if decl.variadic:
            if params:
                params += ", ..."
            else:
                params = "..."
        ret = self.llvm_type(decl.ret_type)
        self.emit(f"declare {ret} @{decl.name}({params})")

    def llvm_type(self, t: JType) -> str:
        resolved = self.resolve_type_alias(t)
        result = resolved.llvm_type()
        if result == "i8**" and resolved.is_array:
            return "i8*"
        return result

    def resolve_type_alias(self, t: JType) -> JType:
        """Resolve type alias recursively."""
        if t.name in self.type_aliases:
            resolved = self.type_aliases[t.name]
            # Preserve pointer and array flags
            return JType(
                resolved.name,
                is_pointer=t.is_pointer or resolved.is_pointer,
                is_array=t.is_array or resolved.is_array,
                array_size=t.array_size or resolved.array_size
            )
        return t

    def substitute_type(self, jtype: JType, type_context) -> JType:
        """Substitute type variables with concrete types."""
        if type_context:
            concrete = type_context.lookup(jtype.name)
            if concrete:
                return JType(
                    concrete.name,
                    is_pointer=jtype.is_pointer or concrete.is_pointer,
                    is_array=jtype.is_array or concrete.is_array,
                    array_size=jtype.array_size or concrete.array_size,
                    is_generic=False
                )
        return jtype

    def emit_monomorphized_func(self, func_decl: FuncDecl, mono):
        """Generate a monomorphized version of a generic function."""
        from jocky.language.checker import GenericContext

        # Create type context from monomorphization
        type_context = GenericContext()
        for param, arg_type in mono.type_bindings.items():
            type_context.bind(param, arg_type)

        # Save current state
        saved_locals = self.locals.copy()
        saved_func = self.current_func
        saved_block = self.current_block
        saved_type_context = self.type_context

        # Setup for monomorphized function
        self.current_func = mono.mangled_name
        self.current_block = "entry"
        self.locals = {}
        self.type_context = type_context

        # Emit function header
        param_types = []
        for param in func_decl.params:
            subst_type = self.substitute_type(param.type, type_context)
            param_types.append(subst_type)

        ret_type = self.substitute_type(func_decl.ret_type, type_context)

        param_str = ", ".join(f"{self.llvm_type(t)} %{p.name}" for t, p in zip(param_types, func_decl.params))
        self.emit(f"define {self.llvm_type(ret_type)} @{mono.mangled_name}({param_str}) {{")
        self.emit("entry:")

        # Process parameters
        for param, subst_type in zip(func_decl.params, param_types):
            alloca = self.next_reg()
            self.emit(f"  {alloca} = alloca {self.llvm_type(subst_type)}")
            self.emit(f"  store {self.llvm_type(subst_type)} %{param.name}, {self.llvm_type(subst_type)}* {alloca}")
            self.locals[param.name] = (alloca, subst_type)

        # Emit function body
        for stmt in func_decl.body.stmts:
            self.emit_stmt(stmt)

        # Default return
        if ret_type.name == "void":
            self.emit("  ret void")
        else:
            self.emit(f"  ret {self.llvm_type(ret_type)} 0")

        self.emit("}")

        self.locals = saved_locals
        self.current_func = saved_func
        self.current_block = saved_block
        self.type_context = saved_type_context

    def collect_lets(self, stmts: List[Any]) -> List[Tuple[str, JType]]:
        """Walk statement list recursively and collect (name, type) for every LetStmt."""
        result = []
        for stmt in stmts:
            if isinstance(stmt, LetStmt):
                try:
                    t = stmt.type if stmt.type else self.infer_type(stmt.init)
                except Exception:
                    t = JType("i64")
                t = self.resolve_type_alias(t)
                result.append((stmt.name, t))
                self.locals[stmt.name] = (f"%{stmt.name}", t)
            elif isinstance(stmt, IfStmt):
                result.extend(self.collect_lets(stmt.then_block.stmts))
                if stmt.else_block:
                    result.extend(self.collect_lets(stmt.else_block.stmts))
            elif isinstance(stmt, WhileStmt):
                result.extend(self.collect_lets(stmt.body.stmts))
            elif isinstance(stmt, ForStmt):
                if isinstance(stmt.init, LetStmt):
                    t = stmt.init.type if stmt.init.type else self.infer_type(stmt.init.init)
                    t = self.resolve_type_alias(t)
                    result.append((stmt.init.name, t))
                result.extend(self.collect_lets(stmt.body.stmts))
            elif isinstance(stmt, ForInStmt):
                elem_type = JType("i8", is_pointer=True)
                if isinstance(stmt.iterable, VarRef) and stmt.iterable.name in self.global_inits:
                    elem_type = JType("i8", is_pointer=True)
                else:
                    try:
                        iter_type = self.infer_type(stmt.iterable)
                        if iter_type.name == "string" or iter_type.is_pointer:
                            elem_type = JType("i8", is_pointer=True)
                        else:
                            elem_type = JType(iter_type.name, is_pointer=False, is_array=False)
                    except Exception:
                        pass
                self.locals[stmt.var_name] = (f"%{stmt.var_name}", elem_type)
                result.extend(self.collect_lets(stmt.body.stmts))
        return result

    def emit_func(self, decl: FuncDecl):
        self.current_func = decl.name
        self.reg_counter = 0
        self.label_counter = 0
        self.locals = {}
        self.loop_stack = []
        self.current_block = "entry"

        params = ", ".join(f"{self.llvm_type(p.type)} %{p.name}" for p in decl.params)
        ret = self.llvm_type(decl.ret_type)
        self.emit(f"define {ret} @{decl.name}({params}) {{")
        self.emit("entry:")

        # Allocate params
        for p in decl.params:
            alloca = self.next_reg()
            self.emit(f"  {alloca} = alloca {self.llvm_type(p.type)}")
            self.emit(f"  store {self.llvm_type(p.type)} %{p.name}, {self.llvm_type(p.type)}* {alloca}")
            self.locals[p.name] = (alloca, p.type)

        # Hoist all LetStmt allocas to the entry block for mem2reg compatibility
        saved_locals = dict(self.locals)
        all_lets = self.collect_lets(decl.body.stmts)
        self.locals = saved_locals
        seen_names: set = set()
        for name, t in all_lets:
            if name not in seen_names and name not in self.locals:
                alloca = self.next_reg()
                self.emit(f"  {alloca} = alloca {self.llvm_type(t)}")
                self.locals[name] = (alloca, t)
                seen_names.add(name)

        self.emit_block(decl.body)

        # Ensure terminator
        if not self.last_line_is_terminator():
            if decl.ret_type.name == "void":
                self.emit("  ret void")
            else:
                self.emit(f"  ret {self.llvm_type(decl.ret_type)} 0")

        self.emit("}")

    def last_line_is_terminator(self) -> bool:
        if not self.output_lines:
            return False
        last = self.output_lines[-1].strip()
        return last.startswith("ret ") or last.startswith("br ")

    def emit_block(self, block: Block):
        for stmt in block.stmts:
            self.emit_stmt(stmt)

    def emit_stmt(self, stmt: Any):
        if isinstance(stmt, LetStmt):
            t = stmt.type if stmt.type else self.infer_type(stmt.init)
            # Resolve type aliases
            t = self.resolve_type_alias(t)
            val, vt = self.emit_expr(stmt.init)
            vt = self.resolve_type_alias(vt)
            if t.name != vt.name or t.is_pointer != vt.is_pointer:
                val = self.emit_cast(val, vt, t)
            # Alloca was hoisted to entry block; retrieve it
            if stmt.name in self.locals:
                alloca, stored_t = self.locals[stmt.name]
                if stored_t.name != t.name or stored_t.is_pointer != t.is_pointer:
                    val = self.emit_cast(val, t, stored_t)
                    t = stored_t
            else:
                alloca = self.next_reg()
                self.emit(f"  {alloca} = alloca {self.llvm_type(t)}")
            self.emit(f"  store {self.llvm_type(t)} {val}, {self.llvm_type(t)}* {alloca}")
            self.locals[stmt.name] = (alloca, t)
        elif isinstance(stmt, AssignStmt):
            if isinstance(stmt.target, VarRef):
                if stmt.target.name in self.locals:
                    alloca, t = self.locals[stmt.target.name]
                elif stmt.target.name in self.globals:
                    alloca, t = self.globals[stmt.target.name]
                else:
                    raise CodeGenError(f"Undefined variable: {stmt.target.name}")
                val, vt = self.emit_expr(stmt.value)
                if t.name != vt.name or t.is_pointer != vt.is_pointer:
                    try:
                        val = self.emit_cast(val, vt, t)
                    except CodeGenError:
                        pass
                self.emit(f"  store {self.llvm_type(t)} {val}, {self.llvm_type(t)}* {alloca}")
            elif isinstance(stmt.target, DerefExpr):
                ptr, pt = self.emit_expr(stmt.target.operand)
                val, vt = self.emit_expr(stmt.value)
                inner = JType(pt.name, is_pointer=False)
                if inner.name != vt.name:
                    val = self.emit_cast(val, vt, inner)
                self.emit(f"  store {self.llvm_type(inner)} {val}, {self.llvm_type(inner)}* {ptr}")
            elif isinstance(stmt.target, IndexExpr):
                base, bt = self.emit_expr(stmt.target.base)
                idx, _ = self.emit_expr(stmt.target.index)
                elem_type = JType(bt.name, is_pointer=False)
                gep = self.next_reg()
                self.emit(f"  {gep} = getelementptr {self.llvm_type(elem_type)}, {self.llvm_type(bt)} {base}, i32 {idx}")
                val, vt = self.emit_expr(stmt.value)
                if elem_type.name != vt.name:
                    val = self.emit_cast(val, vt, elem_type)
                self.emit(f"  store {self.llvm_type(elem_type)} {val}, {self.llvm_type(elem_type)}* {gep}")
            else:
                raise CodeGenError(f"Invalid assignment target: {type(stmt.target).__name__}")
        elif isinstance(stmt, IfStmt):
            self.emit_if(stmt)
        elif isinstance(stmt, WhileStmt):
            self.emit_while(stmt)
        elif isinstance(stmt, ForStmt):
            self.emit_for(stmt)
        elif isinstance(stmt, ForInStmt):
            self.emit_for_in(stmt)
        elif isinstance(stmt, ReturnStmt):
            if stmt.value is None:
                self.emit("  ret void")
            else:
                val, vt = self.emit_expr(stmt.value)
                self.emit(f"  ret {self.llvm_type(vt)} {val}")
        elif isinstance(stmt, ExprStmt):
            self.emit_expr(stmt.expr)
        elif isinstance(stmt, BreakStmt):
            if self.loop_stack:
                _, break_lbl = self.loop_stack[-1]
                self.emit(f"  br label %{break_lbl}")
        elif isinstance(stmt, ContinueStmt):
            if self.loop_stack:
                cont_lbl, _ = self.loop_stack[-1]
                self.emit(f"  br label %{cont_lbl}")

    def emit_if(self, stmt: IfStmt):
        then_label = self.next_label("then")
        else_label = self.next_label("else")
        merge_label = self.next_label("merge")

        cond_val, ct = self.emit_expr(stmt.cond)
        if ct.name != "bool":
            cond_val = self.emit_to_bool(cond_val, ct)

        has_else = stmt.else_block is not None
        if has_else:
            self.emit(f"  br i1 {cond_val}, label %{then_label}, label %{else_label}")
        else:
            self.emit(f"  br i1 {cond_val}, label %{then_label}, label %{merge_label}")

        self.emit_label(then_label)
        self.emit_block(stmt.then_block)
        if not self.last_line_is_terminator():
            self.emit(f"  br label %{merge_label}")

        if has_else:
            self.emit_label(else_label)
            self.emit_block(stmt.else_block)
            if not self.last_line_is_terminator():
                self.emit(f"  br label %{merge_label}")

        self.emit_label(merge_label)

    def emit_while(self, stmt: WhileStmt):
        header = self.next_label("while")
        body = self.next_label("while_body")
        exit_l = self.next_label("while_exit")

        self.emit(f"  br label %{header}")
        self.emit_label(header)
        cond_val, ct = self.emit_expr(stmt.cond)
        if ct.name != "bool":
            cond_val = self.emit_to_bool(cond_val, ct)
        self.emit(f"  br i1 {cond_val}, label %{body}, label %{exit_l}")

        self.emit_label(body)
        self.loop_stack.append((header, exit_l))
        self.emit_block(stmt.body)
        self.loop_stack.pop()
        if not self.last_line_is_terminator():
            self.emit(f"  br label %{header}")

        self.emit_label(exit_l)

    def emit_for(self, stmt: ForStmt):
        header = self.next_label("for_cond")
        step_block = self.next_label("for_step")
        body_block = self.next_label("for_body")
        exit_block = self.next_label("for_exit")

        # emit init
        self.emit_stmt(stmt.init)
        self.emit(f"  br label %{header}")

        # condition
        self.emit_label(header)
        cond_val, ct = self.emit_expr(stmt.cond)
        if ct.name != "bool":
            cond_val = self.emit_to_bool(cond_val, ct)
        self.emit(f"  br i1 {cond_val}, label %{body_block}, label %{exit_block}")

        # body — continue jumps to step_block
        self.emit_label(body_block)
        self.loop_stack.append((step_block, exit_block))
        self.emit_block(stmt.body)
        self.loop_stack.pop()
        if not self.last_line_is_terminator():
            self.emit(f"  br label %{step_block}")

        # step
        self.emit_label(step_block)
        step = stmt.step
        if isinstance(step, AssignStmt):
            self.emit_stmt(step)
        elif isinstance(step, IntLiteral) and step.value == 0:
            pass  # empty step
        else:
            self.emit_expr(step)
        self.emit(f"  br label %{header}")

        self.emit_label(exit_block)

    def emit_for_in(self, stmt: ForInStmt):
        iter_val, iter_type = self.emit_expr(stmt.iterable)
        if iter_type.is_array and iter_type.array_size > 0:
            arr_len = iter_type.array_size
        else:
            len_reg = self.next_reg()
            self.emit(f"  {len_reg} = call i64 @array_len(i8* {iter_val})")
            arr_len = len_reg

        if iter_type.name == "string" or iter_type.is_pointer:
            elem_type = JType("i8", is_pointer=True)
        else:
            elem_type = JType(iter_type.name, is_pointer=False, is_array=False)
        elem_llvm = self.llvm_type(elem_type)

        idx_alloca = self.next_reg()
        self.emit(f"  {idx_alloca} = alloca i64")
        self.emit(f"  store i64 0, i64* {idx_alloca}")

        elem_alloca = self.next_reg()
        self.emit(f"  {elem_alloca} = alloca {elem_llvm}")
        self.locals[stmt.var_name] = (elem_alloca, elem_type)

        header = self.next_label("forin_cond")
        body_lbl = self.next_label("forin_body")
        exit_lbl = self.next_label("forin_exit")

        self.emit(f"  br label %{header}")
        self.emit_label(header)
        idx_val = self.next_reg()
        self.emit(f"  {idx_val} = load i64, i64* {idx_alloca}")
        cond = self.next_reg()
        self.emit(f"  {cond} = icmp slt i64 {idx_val}, {arr_len}")
        self.emit(f"  br i1 {cond}, label %{body_lbl}, label %{exit_lbl}")

        self.emit_label(body_lbl)
        if elem_llvm == "i8*":
            cast = self.next_reg()
            self.emit(f"  {cast} = bitcast i8* {iter_val} to i8**")
            gep = self.next_reg()
            self.emit(f"  {gep} = getelementptr i8*, i8** {cast}, i64 {idx_val}")
            elem_val = self.next_reg()
            self.emit(f"  {elem_val} = load i8*, i8** {gep}")
        else:
            gep = self.next_reg()
            self.emit(f"  {gep} = getelementptr {elem_llvm}, {elem_llvm}* {iter_val}, i64 {idx_val}")
            elem_val = self.next_reg()
            self.emit(f"  {elem_val} = load {elem_llvm}, {elem_llvm}* {gep}")
        self.emit(f"  store {elem_llvm} {elem_val}, {elem_llvm}* {elem_alloca}")

        step_lbl = self.next_label("forin_step")
        self.loop_stack.append((step_lbl, exit_lbl))
        self.emit_block(stmt.body)
        self.loop_stack.pop()
        if not self.last_line_is_terminator():
            self.emit(f"  br label %{step_lbl}")

        self.emit_label(step_lbl)
        next_idx = self.next_reg()
        self.emit(f"  {next_idx} = add i64 {idx_val}, 1")
        self.emit(f"  store i64 {next_idx}, i64* {idx_alloca}")
        self.emit(f"  br label %{header}")

        self.emit_label(exit_lbl)

    def emit_to_bool(self, val: str, t: JType) -> str:
        if self.is_float(t):
            r = self.next_reg()
            self.emit(f"  {r} = fcmp one {self.llvm_type(t)} {val}, 0.0")
            return r
        if self.is_numeric(t):
            r = self.next_reg()
            self.emit(f"  {r} = icmp ne {self.llvm_type(t)} {val}, 0")
            return r
        if t.name == "string" or t.is_pointer:
            r = self.next_reg()
            self.emit(f"  {r} = icmp ne {self.llvm_type(t)} {val}, null")
            return r
        if t.name == "bool":
            return val
        raise CodeGenError(f"Cannot convert {t} to bool")

    def emit_cast(self, val: str, from_t: JType, to_t: JType) -> str:
        if from_t.name == to_t.name and from_t.is_pointer == to_t.is_pointer:
            return val
        if from_t.name == "bool" and self.is_numeric(to_t):
            r = self.next_reg()
            self.emit(f"  {r} = zext i1 {val} to {self.llvm_type(to_t)}")
            return r
        if self.is_numeric(from_t) and to_t.name == "bool":
            r = self.next_reg()
            self.emit(f"  {r} = icmp ne {self.llvm_type(from_t)} {val}, 0")
            return r
        if self.is_float(from_t) and self.is_integer(to_t):
            r = self.next_reg()
            self.emit(f"  {r} = fptosi {self.llvm_type(from_t)} {val} to {self.llvm_type(to_t)}")
            return r
        if self.is_integer(from_t) and self.is_float(to_t):
            r = self.next_reg()
            self.emit(f"  {r} = sitofp {self.llvm_type(from_t)} {val} to {self.llvm_type(to_t)}")
            return r
        if self.is_float(from_t) and self.is_float(to_t):
            r = self.next_reg()
            if from_t.size_bytes() < to_t.size_bytes():
                self.emit(f"  {r} = fpext {self.llvm_type(from_t)} {val} to {self.llvm_type(to_t)}")
            else:
                self.emit(f"  {r} = fptrunc {self.llvm_type(from_t)} {val} to {self.llvm_type(to_t)}")
            return r
        if self.is_numeric(from_t) and self.is_numeric(to_t):
            fsize = from_t.size_bytes()
            tsize = to_t.size_bytes()
            r = self.next_reg()
            if fsize < tsize:
                self.emit(f"  {r} = sext {self.llvm_type(from_t)} {val} to {self.llvm_type(to_t)}")
            else:
                self.emit(f"  {r} = trunc {self.llvm_type(from_t)} {val} to {self.llvm_type(to_t)}")
            return r
        # Pointer casts
        if from_t.is_pointer and to_t.is_pointer:
            r = self.next_reg()
            self.emit(f"  {r} = bitcast {self.llvm_type(from_t)} {val} to {self.llvm_type(to_t)}")
            return r
        if to_t.is_pointer and from_t.name in ("i8", "i32", "i64"):
            r = self.next_reg()
            self.emit(f"  {r} = inttoptr {self.llvm_type(from_t)} {val} to {self.llvm_type(to_t)}")
            return r
        # Fallback: bitcast between pointer types or inttoptr/ptrtoint
        from_llvm = self.llvm_type(from_t)
        to_llvm = self.llvm_type(to_t)
        if from_llvm == to_llvm:
            return val
        if '*' in from_llvm and '*' in to_llvm:
            r = self.next_reg()
            self.emit(f"  {r} = bitcast {from_llvm} {val} to {to_llvm}")
        elif '*' in to_llvm:
            if 'i' in from_llvm:
                ext = self.next_reg()
                self.emit(f"  {ext} = sext {from_llvm} {val} to i64")
                r = self.next_reg()
                self.emit(f"  {r} = inttoptr i64 {ext} to {to_llvm}")
            else:
                r = self.next_reg()
                self.emit(f"  {r} = inttoptr i64 0 to {to_llvm}")
        elif '*' in from_llvm:
            r = self.next_reg()
            self.emit(f"  {r} = ptrtoint {from_llvm} {val} to {to_llvm}")
        else:
            r = self.next_reg()
            self.emit(f"  {r} = bitcast {from_llvm} {val} to {to_llvm}")
        return r

    def is_numeric(self, t: JType) -> bool:
        return t.name in ("i8", "i32", "i64", "f32", "f64") and not t.is_pointer

    def is_float(self, t: JType) -> bool:
        return t.name in ("f32", "f64") and not t.is_pointer

    def is_integer(self, t: JType) -> bool:
        return t.name in ("i8", "i32", "i64") and not t.is_pointer

    def infer_type(self, expr: Any) -> JType:
        if isinstance(expr, IntLiteral):
            return JType("i32")
        elif isinstance(expr, FloatLiteral):
            return JType("f64")
        elif isinstance(expr, BoolLiteral):
            return JType("bool")
        elif isinstance(expr, StringLiteral):
            return JType("string")
        elif isinstance(expr, NullLiteral):
            return JType("i8", is_pointer=True)
        elif isinstance(expr, VarRef):
            if expr.name in self.locals:
                return self.locals[expr.name][1]
            if expr.name in self.globals:
                return self.globals[expr.name][1]
            for enum_name, enum_def in self.enums.items():
                for variant in enum_def.variants:
                    if variant.name == expr.name:
                        return JType(enum_name)
            return JType("i64")
        elif isinstance(expr, BinaryOp):
            if expr.op in ("+", "-", "*", "/", "%", "|", "^", "&", "<<", ">>"):
                return self.infer_type(expr.left)
            return JType("bool")
        elif isinstance(expr, UnaryOp):
            if expr.op == "!":
                return JType("bool")
            return self.infer_type(expr.operand)
        elif isinstance(expr, CallExpr):
            if expr.name in self.functions:
                sig = self.functions[expr.name]
                return sig[1]
            raise CodeGenError(f"Undefined function: {expr.name}")
        elif isinstance(expr, DerefExpr):
            t = self.infer_type(expr.operand)
            return JType(t.name, is_pointer=False)
        elif isinstance(expr, AddrOfExpr):
            t = self.infer_type(expr.operand)
            return JType(t.name, is_pointer=True)
        elif isinstance(expr, CastExpr):
            return expr.target_type
        elif isinstance(expr, IndexExpr):
            t = self.infer_type(expr.base)
            if t.is_pointer and t.name == "i8":
                return JType("i8", is_pointer=True)
            return JType(t.name, is_pointer=False, is_array=False)
        elif isinstance(expr, FieldAccessExpr):
            obj_type = self.infer_type(expr.object)
            if obj_type.name not in self.structs:
                raise CodeGenError(f"Cannot access field on non-struct: {obj_type}")
            struct_def = self.structs[obj_type.name]
            for field in struct_def.fields:
                if field.name == expr.field:
                    return field.type
            raise CodeGenError(f"Struct {obj_type.name} has no field {expr.field}")
        elif isinstance(expr, TupleExpr):
            return JType("i8", is_pointer=True)
        elif isinstance(expr, ArrayLiteralExpr):
            return JType("i8", is_pointer=True)
        elif isinstance(expr, StructLiteralExpr):
            return JType(expr.struct_type, is_pointer=False)
        elif isinstance(expr, MatchExpr):
            # Infer type from first arm
            if expr.arms:
                for stmt in expr.arms[0].body.stmts:
                    if isinstance(stmt, ExprStmt):
                        return self.infer_type(stmt.expr)
                    elif isinstance(stmt, ReturnStmt) and stmt.value:
                        return self.infer_type(stmt.value)
            return JType("void")
        elif isinstance(expr, ClosureExpr):
            # Closure is a function pointer
            return JType("i8", is_pointer=True)
        raise CodeGenError(f"Cannot infer type for {type(expr).__name__}")

    def emit_expr(self, expr: Any) -> Tuple[str, JType]:
        if isinstance(expr, IntLiteral):
            return (str(expr.value), JType("i32"))
        elif isinstance(expr, FloatLiteral):
            return (f"{expr.value:#.17g}", JType("f64"))
        elif isinstance(expr, BoolLiteral):
            return ("1" if expr.value else "0", JType("bool"))
        elif isinstance(expr, NullLiteral):
            return ("null", JType("i8", is_pointer=True))
        elif isinstance(expr, StringLiteral):
            name, length = self.get_string_const(expr.value)
            gep = self.next_reg()
            self.emit(f"  {gep} = getelementptr [{length} x i8], [{length} x i8]* {name}, i32 0, i32 0")
            return (gep, JType("string"))
        elif isinstance(expr, VarRef):
            if expr.name in self.locals:
                alloca, t = self.locals[expr.name]
                r = self.next_reg()
                self.emit(f"  {r} = load {self.llvm_type(t)}, {self.llvm_type(t)}* {alloca}")
                return (r, t)
            if expr.name in self.globals:
                gname, t = self.globals[expr.name]
                if expr.name in self.global_inits:
                    init_expr = self.global_inits[expr.name]
                    val, vt = self.emit_expr(init_expr)
                    return (val, vt)
                r = self.next_reg()
                self.emit(f"  {r} = load {self.llvm_type(t)}, {self.llvm_type(t)}* {gname}")
                return (r, t)
            for enum_name, enum_def in self.enums.items():
                for variant in enum_def.variants:
                    if variant.name == expr.name:
                        val = variant.value if variant.value is not None else 0
                        return (str(val), JType(enum_name))
            raise CodeGenError(f"Undefined variable: {expr.name}")
        elif isinstance(expr, BinaryOp):
            if expr.op in ("&&", "||"):
                return self.emit_logical(expr)
            return self.emit_binary(expr)
        elif isinstance(expr, UnaryOp):
            return self.emit_unary(expr)
        elif isinstance(expr, CallExpr):
            return self.emit_call(expr)
        elif isinstance(expr, DerefExpr):
            ptr, pt = self.emit_expr(expr.operand)
            inner = JType(pt.name, is_pointer=False)
            r = self.next_reg()
            self.emit(f"  {r} = load {self.llvm_type(inner)}, {self.llvm_type(inner)}* {ptr}")
            return (r, inner)
        elif isinstance(expr, AddrOfExpr):
            if isinstance(expr.operand, VarRef):
                if expr.operand.name not in self.locals:
                    raise CodeGenError(f"Undefined variable: {expr.operand.name}")
                alloca, t = self.locals[expr.operand.name]
                return (alloca, JType(t.name, is_pointer=True))
            raise CodeGenError("Can only take address of variables")
        elif isinstance(expr, CastExpr):
            val, vt = self.emit_expr(expr.operand)
            return (self.emit_cast(val, vt, expr.target_type), expr.target_type)
        elif isinstance(expr, IndexExpr):
            base, bt = self.emit_expr(expr.base)
            idx, _ = self.emit_expr(expr.index)
            if bt.is_pointer and not bt.is_array and bt.name == "i8":
                cast = self.next_reg()
                self.emit(f"  {cast} = bitcast i8* {base} to i8**")
                gep = self.next_reg()
                self.emit(f"  {gep} = getelementptr i8*, i8** {cast}, i32 {idx}")
                r = self.next_reg()
                self.emit(f"  {r} = load i8*, i8** {gep}")
                return (r, JType("i8", is_pointer=True))
            elem_type = JType(bt.name, is_pointer=False, is_array=False)
            gep = self.next_reg()
            self.emit(f"  {gep} = getelementptr {self.llvm_type(elem_type)}, {self.llvm_type(bt)} {base}, i32 {idx}")
            r = self.next_reg()
            self.emit(f"  {r} = load {self.llvm_type(elem_type)}, {self.llvm_type(elem_type)}* {gep}")
            return (r, elem_type)
        elif isinstance(expr, FieldAccessExpr):
            # Load struct value and extract field via GEP
            obj, obj_type = self.emit_expr(expr.object)
            if obj_type.name not in self.structs:
                raise CodeGenError(f"Cannot access field on non-struct: {obj_type}")
            struct_def = self.structs[obj_type.name]
            # Find field index
            field_idx = -1
            field_type = None
            for i, field in enumerate(struct_def.fields):
                if field.name == expr.field:
                    field_idx = i
                    field_type = field.type
                    break
            if field_idx < 0:
                raise CodeGenError(f"Struct {obj_type.name} has no field {expr.field}")
            # Use GEP to get field pointer
            gep = self.next_reg()
            self.emit(f"  {gep} = getelementptr %{obj_type.name}, %{obj_type.name}* {obj}, i32 0, i32 {field_idx}")
            r = self.next_reg()
            self.emit(f"  {r} = load {self.llvm_type(field_type)}, {self.llvm_type(field_type)}* {gep}")
            return (r, field_type)
        elif isinstance(expr, TupleExpr):
            n = len(expr.elements)
            elem_type = JType("i8", is_pointer=True)
            array_type = JType("i8", is_array=True, array_size=n, is_pointer=True)
            alloca = self.next_reg()
            self.emit(f"  {alloca} = alloca [{n} x i8*]")
            for i, elem_expr in enumerate(expr.elements):
                elem_val, et = self.emit_expr(elem_expr)
                elem_llvm = self.llvm_type(et)
                if elem_llvm != "i8*":
                    if '*' in elem_llvm:
                        cast = self.next_reg()
                        self.emit(f"  {cast} = bitcast {elem_llvm} {elem_val} to i8*")
                    else:
                        ext = self.next_reg()
                        self.emit(f"  {ext} = sext {elem_llvm} {elem_val} to i64")
                        cast = self.next_reg()
                        self.emit(f"  {cast} = inttoptr i64 {ext} to i8*")
                    elem_val = cast
                gep = self.next_reg()
                self.emit(f"  {gep} = getelementptr [{n} x i8*], [{n} x i8*]* {alloca}, i32 0, i32 {i}")
                self.emit(f"  store i8* {elem_val}, i8** {gep}")
            ptr = self.next_reg()
            self.emit(f"  {ptr} = bitcast [{n} x i8*]* {alloca} to i8*")
            return (ptr, JType("i8", is_pointer=True))
        elif isinstance(expr, ArrayLiteralExpr):
            if len(expr.elements) == 0:
                return ("null", JType("i8", is_pointer=True))
            n = len(expr.elements)
            first_val, first_type = self.emit_expr(expr.elements[0])
            elem_llvm = self.llvm_type(first_type)
            alloca = self.next_reg()
            self.emit(f"  {alloca} = alloca [{n} x {elem_llvm}]")
            gep = self.next_reg()
            self.emit(f"  {gep} = getelementptr [{n} x {elem_llvm}], [{n} x {elem_llvm}]* {alloca}, i32 0, i32 0")
            self.emit(f"  store {elem_llvm} {first_val}, {elem_llvm}* {gep}")
            for i, elem_expr in enumerate(expr.elements[1:], 1):
                ev, et = self.emit_expr(elem_expr)
                ev_llvm = self.llvm_type(et)
                if ev_llvm != elem_llvm:
                    ev = self.emit_cast(ev, et, first_type)
                g = self.next_reg()
                self.emit(f"  {g} = getelementptr [{n} x {elem_llvm}], [{n} x {elem_llvm}]* {alloca}, i32 0, i32 {i}")
                self.emit(f"  store {elem_llvm} {ev}, {elem_llvm}* {g}")
            ptr = self.next_reg()
            self.emit(f"  {ptr} = bitcast [{n} x {elem_llvm}]* {alloca} to i8*")
            return (ptr, JType("i8", is_pointer=True))
        elif isinstance(expr, StructLiteralExpr):
            # Allocate struct on stack and initialize fields
            if expr.struct_type not in self.structs:
                raise CodeGenError(f"Unknown struct: {expr.struct_type}")
            struct_def = self.structs[expr.struct_type]
            # Allocate struct
            alloca = self.next_reg()
            self.emit(f"  {alloca} = alloca %{expr.struct_type}")
            # Initialize fields
            for i, field in enumerate(struct_def.fields):
                if field.name not in expr.fields:
                    raise CodeGenError(f"Missing field {field.name} in struct init")
                val, val_type = self.emit_expr(expr.fields[field.name])
                gep = self.next_reg()
                self.emit(f"  {gep} = getelementptr %{expr.struct_type}, %{expr.struct_type}* {alloca}, i32 0, i32 {i}")
                self.emit(f"  store {self.llvm_type(field.type)} {val}, {self.llvm_type(field.type)}* {gep}")
            return (alloca, JType(expr.struct_type, is_pointer=False))
        elif isinstance(expr, MatchExpr):
            return self.emit_match(expr)
        elif isinstance(expr, ClosureExpr):
            # Emit closure as function pointer
            # Generate unique name for closure
            closure_name = f"_closure_{self.reg_counter}"
            self.reg_counter += 1

            # Determine parameter and return types
            param_types = [p.type for p in expr.params if p.type]
            if expr.ret_type:
                ret_type = expr.ret_type
            else:
                ret_type = JType("i32")  # Default return type

            # Emit closure function
            param_str = ", ".join(f"{self.llvm_type(t)} %p{i}" for i, t in enumerate(param_types))
            self.emit(f"define {self.llvm_type(ret_type)} @{closure_name}({param_str}) {{")
            self.emit("entry:")

            # Save locals and setup closure parameters
            saved_locals = self.locals.copy()
            self.locals = {}
            for i, (param, ptype) in enumerate(zip(expr.params, param_types)):
                alloca = self.next_reg()
                self.emit(f"  {alloca} = alloca {self.llvm_type(ptype)}")
                self.emit(f"  store {self.llvm_type(ptype)} %p{i}, {self.llvm_type(ptype)}* {alloca}")
                self.locals[param.name] = (alloca, ptype)

            # Emit closure body
            if isinstance(expr.body, Block):
                for stmt in expr.body.stmts:
                    self.emit_stmt(stmt)
            else:
                val, vt = self.emit_expr(expr.body)
                self.emit(f"  ret {self.llvm_type(ret_type)} {val}")

            # Ensure terminator
            if not self.last_line_is_terminator():
                if ret_type.name == "void":
                    self.emit("  ret void")
                else:
                    self.emit(f"  ret {self.llvm_type(ret_type)} 0")

            self.emit("}")

            # Restore locals
            self.locals = saved_locals

            # Return function pointer (as i8* for now - proper function types need type system work)
            return (f"@{closure_name}", JType("i8", is_pointer=True))
        else:
            raise CodeGenError(f"Unknown expression: {type(expr).__name__}")

    def emit_logical(self, expr: BinaryOp) -> Tuple[str, JType]:
        """Short-circuit evaluation for && and || using phi nodes."""
        rhs_block = self.next_label("sc_rhs")
        merge_block = self.next_label("sc_merge")

        lhs_val, _ = self.emit_expr(expr.left)
        lhs_end = self.current_block  # predecessor for the false/true branch

        if expr.op == "&&":
            # lhs true → eval rhs; lhs false → short-circuit false
            self.emit(f"  br i1 {lhs_val}, label %{rhs_block}, label %{merge_block}")
        else:  # ||
            # lhs true → short-circuit true; lhs false → eval rhs
            self.emit(f"  br i1 {lhs_val}, label %{merge_block}, label %{rhs_block}")

        self.emit_label(rhs_block)
        rhs_val, _ = self.emit_expr(expr.right)
        rhs_end = self.current_block
        self.emit(f"  br label %{merge_block}")

        self.emit_label(merge_block)
        r = self.next_reg()
        if expr.op == "&&":
            self.emit(f"  {r} = phi i1 [ 0, %{lhs_end} ], [ {rhs_val}, %{rhs_end} ]")
        else:
            self.emit(f"  {r} = phi i1 [ 1, %{lhs_end} ], [ {rhs_val}, %{rhs_end} ]")

        return (r, JType("bool"))

    def is_string_type(self, t: JType) -> bool:
        return t.name == "string" or (t.name == "i8" and t.is_pointer and not t.is_array)

    def emit_binary(self, expr: BinaryOp) -> Tuple[str, JType]:
        left, lt = self.emit_expr(expr.left)
        right, rt = self.emit_expr(expr.right)

        op = expr.op

        # String concatenation
        if self.is_string_type(lt) and op == "+":
            if not self.is_string_type(rt):
                right = self.emit_cast(right, rt, JType("i64"))
                right_str = self.next_reg()
                self.emit(f"  {right_str} = call i8* @string(i64 {right})")
                right = right_str
            r = self.next_reg()
            self.emit(f"  {r} = call i8* @jocky_str_concat(i8* {left}, i8* {right})")
            return (r, JType("string"))

        # Promote to common type for arithmetic / bitwise
        if self.is_numeric(lt) and self.is_numeric(rt):
            if lt.name != rt.name:
                if self.is_float(lt) or self.is_float(rt):
                    target = lt if self.is_float(lt) else rt
                    if lt.name != target.name:
                        left = self.emit_cast(left, lt, target)
                        lt = target
                    if rt.name != target.name:
                        right = self.emit_cast(right, rt, target)
                        rt = target
                elif lt.size_bytes() < rt.size_bytes():
                    left = self.emit_cast(left, lt, rt)
                    lt = rt
                else:
                    right = self.emit_cast(right, rt, lt)
                    rt = lt

        r = self.next_reg()
        typ = lt
        use_float = self.is_float(typ)

        if op == "+":
            instr = "fadd" if use_float else "add"
            self.emit(f"  {r} = {instr} {self.llvm_type(typ)} {left}, {right}")
        elif op == "-":
            instr = "fsub" if use_float else "sub"
            self.emit(f"  {r} = {instr} {self.llvm_type(typ)} {left}, {right}")
        elif op == "*":
            instr = "fmul" if use_float else "mul"
            self.emit(f"  {r} = {instr} {self.llvm_type(typ)} {left}, {right}")
        elif op == "/":
            instr = "fdiv" if use_float else "sdiv"
            self.emit(f"  {r} = {instr} {self.llvm_type(typ)} {left}, {right}")
        elif op == "%":
            instr = "frem" if use_float else "srem"
            self.emit(f"  {r} = {instr} {self.llvm_type(typ)} {left}, {right}")
        elif op == "|":
            self.emit(f"  {r} = or {self.llvm_type(typ)} {left}, {right}")
        elif op == "^":
            self.emit(f"  {r} = xor {self.llvm_type(typ)} {left}, {right}")
        elif op == "&":
            self.emit(f"  {r} = and {self.llvm_type(typ)} {left}, {right}")
        elif op == "<<":
            self.emit(f"  {r} = shl {self.llvm_type(typ)} {left}, {right}")
        elif op == ">>":
            self.emit(f"  {r} = lshr {self.llvm_type(typ)} {left}, {right}")
        elif op in ("==", "!=", "<", ">", "<=", ">="):
            if use_float:
                cmp_map = {"==": "oeq", "!=": "one", "<": "olt", ">": "ogt", "<=": "ole", ">=": "oge"}
                self.emit(f"  {r} = fcmp {cmp_map[op]} {self.llvm_type(typ)} {left}, {right}")
            else:
                cmp_map = {"==": "eq", "!=": "ne", "<": "slt", ">": "sgt", "<=": "sle", ">=": "sge"}
                self.emit(f"  {r} = icmp {cmp_map[op]} {self.llvm_type(typ)} {left}, {right}")
            typ = JType("bool")
        else:
            raise CodeGenError(f"Unknown binary op: {op}")

        return (r, typ)

    def emit_unary(self, expr: UnaryOp) -> Tuple[str, JType]:
        operand, t = self.emit_expr(expr.operand)
        op = expr.op
        r = self.next_reg()
        if op == "-":
            self.emit(f"  {r} = sub {self.llvm_type(t)} 0, {operand}")
            return (r, t)
        elif op == "!":
            self.emit(f"  {r} = xor i1 {operand}, 1")
            return (r, JType("bool"))
        elif op == "~":
            self.emit(f"  {r} = xor {self.llvm_type(t)} {operand}, -1")
            return (r, t)
        else:
            raise CodeGenError(f"Unknown unary op: {op}")

    def emit_call(self, expr: CallExpr) -> Tuple[str, JType]:
        if expr.name not in self.functions:
            raise CodeGenError(f"Undefined function: {expr.name}")

        # Determine the actual function name (may be mangled for generics)
        func_name = expr.name
        if expr.generic_args:
            type_names = [t.name for t in expr.generic_args]
            func_name = f"{expr.name}_{'_'.join(type_names)}"

        sig = self.functions[expr.name]
        if len(sig) == 3:
            ptypes, ret, _ = sig
        else:
            ptypes, ret = sig

        # Resolve type aliases in parameter and return types
        ptypes = [self.resolve_type_alias(pt) for pt in ptypes]
        ret = self.resolve_type_alias(ret)

        # If generic, substitute types in signature
        if expr.generic_args:
            # Create type bindings for substitution
            from jocky.language.checker import GenericContext
            type_context = GenericContext()
            # We need to map generic params to type args
            # Find the function definition to get generic params
            generic_params = []
            for func_def in getattr(self, '_all_funcs', []):
                if func_def.name == expr.name and func_def.generic_params:
                    generic_params = func_def.generic_params
                    break

            if generic_params:
                for param, arg_type in zip(generic_params, expr.generic_args):
                    param_name = param if isinstance(param, str) else param.name
                    type_context.bind(param_name, arg_type)

            # Substitute types
            ptypes = [self.substitute_type(pt, type_context) for pt in ptypes]
            ret = self.substitute_type(ret, type_context)

        args = []
        for i, arg in enumerate(expr.args):
            val, vt = self.emit_expr(arg)
            vt = self.resolve_type_alias(vt)
            if i < len(ptypes):
                pt = ptypes[i]
                pt_llvm = self.llvm_type(pt)
                vt_llvm = self.llvm_type(vt)
                if vt_llvm != pt_llvm:
                    val = self.emit_cast(val, vt, pt)
                vt = pt
            args.append(f"{self.llvm_type(vt)} {val}")
        arg_str = ", ".join(args)
        if ret.name == "void":
            self.emit(f"  call void @{func_name}({arg_str})")
            return ("", ret)
        r = self.next_reg()
        self.emit(f"  {r} = call {self.llvm_type(ret)} @{func_name}({arg_str})")
        return (r, ret)

    def emit_match(self, expr: MatchExpr) -> Tuple[str, JType]:
        """Emit match expression as a switch-like dispatch."""
        from .ast import WildcardPattern, LiteralPattern, VariantPattern

        scrutinee_val, scrutinee_type = self.emit_expr(expr.scrutinee)

        # Generate labels for each arm and merge
        arm_labels = [self.next_label("arm") for _ in expr.arms]
        merge_label = self.next_label("merge")

        # Build switch table for literal and variant patterns
        switch_cases = []  # List of (value, label) for switch
        wildcard_label = None

        for i, arm in enumerate(expr.arms):
            pattern = arm.pattern
            if isinstance(pattern, LiteralPattern):
                # Extract literal value
                if isinstance(pattern.value, IntLiteral):
                    switch_cases.append((pattern.value.value, arm_labels[i]))
                elif isinstance(pattern.value, BoolLiteral):
                    switch_cases.append((1 if pattern.value.value else 0, arm_labels[i]))
            elif isinstance(pattern, VariantPattern):
                # For enum variants, use their assigned value
                if scrutinee_type.name in self.enums:
                    enum_def = self.enums[scrutinee_type.name]
                    for variant in enum_def.variants:
                        if variant.name == pattern.name:
                            val = variant.value if variant.value is not None else 0
                            switch_cases.append((val, arm_labels[i]))
                            break
            elif isinstance(pattern, WildcardPattern):
                wildcard_label = arm_labels[i]

        # Default to wildcard or first label if no wildcard
        default_label = wildcard_label if wildcard_label else arm_labels[0]

        # Emit switch instruction
        switch_line = f"  switch {self.llvm_type(scrutinee_type)} {scrutinee_val}, label %{default_label} [\n"
        for value, label in switch_cases:
            switch_line += f"    {self.llvm_type(scrutinee_type)} {value}, label %{label}\n"
        switch_line += "  ]"
        self.emit(switch_line)

        # Emit each arm's code
        arm_values = []
        arm_blocks = []
        result_type = None
        has_early_return = False

        for i, arm in enumerate(expr.arms):
            self.emit_label(arm_labels[i])

            # Save locals before processing pattern bindings
            saved_locals = self.locals.copy()

            # Handle pattern bindings for tagged unions
            pattern = arm.pattern
            if isinstance(pattern, VariantPattern) and pattern.bindings:
                # For tagged union field extraction, we need to:
                # 1. Know the memory layout of the variant's data
                # 2. Calculate the offset of each field within the variant
                # 3. Load the value from scrutinee + offset
                #
                # Currently: scrutinee_type is i32 (the tag), but for real tagged unions
                # we'd receive a struct/pointer containing both tag and data.
                # This requires runtime representation changes first.
                #
                # For now: allocate space and extract if scrutinee has data layout info
                if scrutinee_type.name in self.enums:
                    enum_def = self.enums[scrutinee_type.name]
                    for variant in enum_def.variants:
                        if variant.name == pattern.name and variant.fields:
                            # Calculate offset of this variant's data
                            # Offset 0 is tag (i32 = 4 bytes), then aligned fields
                            current_offset = 4  # After tag

                            for binding_name, field in zip(pattern.bindings, variant.fields):
                                # Allocate space for the field
                                alloca = self.next_reg()
                                self.emit(f"  {alloca} = alloca {self.llvm_type(field.type)}")

                                # If scrutinee is a pointer to tagged union struct, extract field
                                # For now: if scrutinee is just i32 tag, zero-initialize
                                if scrutinee_type.is_pointer:
                                    # Scrutinee is pointer to union data, extract field at offset
                                    gep = self.next_reg()
                                    self.emit(f"  {gep} = getelementptr i8, i8* {scrutinee_val}, i32 {current_offset}")
                                    field_ptr = self.next_reg()
                                    self.emit(f"  {field_ptr} = bitcast i8* {gep} to {self.llvm_type(field.type)}*")
                                    val = self.next_reg()
                                    self.emit(f"  {val} = load {self.llvm_type(field.type)}, {self.llvm_type(field.type)}* {field_ptr}")
                                    self.emit(f"  store {self.llvm_type(field.type)} {val}, {self.llvm_type(field.type)}* {alloca}")
                                else:
                                    # Scrutinee is just the tag (i32), can't extract fields
                                    # Zero-initialize as placeholder
                                    self.emit(f"  store {self.llvm_type(field.type)} 0, {self.llvm_type(field.type)}* {alloca}")

                                self.locals[binding_name] = (alloca, field.type)

                                # Update offset for next field (simplified: assume no padding)
                                current_offset += (field.type.size_bytes() if hasattr(field.type, 'size_bytes') else 8)
                            break

            # Emit arm body and collect result
            last_val = None
            last_type = None
            arm_has_return = False
            for stmt in arm.body.stmts:
                if isinstance(stmt, ExprStmt):
                    last_val, last_type = self.emit_expr(stmt.expr)
                elif isinstance(stmt, ReturnStmt):
                    # Match can have early returns
                    arm_has_return = True
                    if stmt.value:
                        val, vt = self.emit_expr(stmt.value)
                        self.emit(f"  ret {self.llvm_type(vt)} {val}")
                    else:
                        self.emit(f"  ret void")
                    has_early_return = True
                else:
                    self.emit_stmt(stmt)

            # Default result type from first arm
            if result_type is None and last_type:
                result_type = last_type

            if last_val is not None and not arm_has_return:
                arm_values.append((last_val, last_type, arm_labels[i]))

            # Branch to merge (unless there was an explicit return)
            if not arm_has_return:
                self.emit(f"  br label %{merge_label}")

            # Restore locals after arm
            self.locals = saved_locals

            arm_blocks.append(arm_labels[i])

        # If all arms returned early, we're done
        if has_early_return and not arm_values:
            return ("", JType("void"))

        # Merge block (only needed if some arms don't return)
        if not has_early_return or arm_values:
            self.emit_label(merge_label)

            # Collect results with phi node if needed
            if arm_values and result_type:
                r = self.next_reg()
                phi_line = f"  {r} = phi {self.llvm_type(result_type)} "
                phi_pairs = []
                for val, vt, block in arm_values:
                    # Cast value to result type if needed
                    if vt.name != result_type.name or vt.is_pointer != result_type.is_pointer:
                        casted_val = self.emit_cast(val, vt, result_type)
                    else:
                        casted_val = val
                    phi_pairs.append(f"[ {casted_val}, %{block} ]")
                phi_line += ", ".join(phi_pairs)
                self.emit(phi_line)
                return (r, result_type)

        return ("", result_type or JType("void"))
