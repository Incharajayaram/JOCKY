from .ast import *
from .errors import CodeGenError as BaseCodeGenError, SourceRange
from .bytecode import BytecodeCompiler
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
        self.metadata_counter = 0
        self.current_func = ""
        self.current_block = "entry"
        self.locals: Dict[str, Tuple[str, JType]] = {}
        self.functions: Dict[str, (List[JType], JType)] = {}
        self.monomorphic_instances: Dict[str, FuncDecl] = {}  # instance_name -> FuncDecl
        self.structs: Dict[str, StructDef] = {}  # struct_name -> StructDef
        self.enums: Dict[str, EnumDef] = {}      # enum_name -> EnumDef
        self.type_aliases: Dict[str, JType] = {} # alias_name -> JType
        self.loop_stack: List[Tuple[str, str]] = []  # [(continue_label, break_label)]
        self.debug_enabled = True  # Enable DWARF debug info generation
        self.source_file = "<unknown>"

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

    def gen(self, prog: Program) -> str:
        # Emit DWARF compilation unit metadata
        if self.debug_enabled:
            self.cu_id, self.file_id = self.emit_dwarf_compile_unit(self.source_file)
        else:
            self.cu_id, self.file_id = -1, -1

        # First pass: collect signatures and definitions
        for decl in prog.decls:
            if isinstance(decl, TypeAlias):
                self.type_aliases[decl.name] = decl.target_type
            elif isinstance(decl, StructDef):
                self.structs[decl.name] = decl
            elif isinstance(decl, EnumDef):
                self.enums[decl.name] = decl
            elif isinstance(decl, FuncDecl):
                # Skip generic functions - they don't generate code directly
                if decl.is_generic:
                    continue
                ptypes = [p.type for p in decl.params]
                self.functions[decl.name] = (ptypes, decl.ret_type, False)
            elif isinstance(decl, FFIDecl):
                ptypes = [p.type for p in decl.params]
                self.functions[decl.name] = (ptypes, decl.ret_type, decl.variadic)

        # Register monomorphic instances
        for instance_name, instance_decl in self.monomorphic_instances.items():
            ptypes = [p.type for p in instance_decl.params]
            self.functions[instance_name] = (ptypes, instance_decl.ret_type, False)

        # Emit struct type definitions
        for struct_name, struct_def in self.structs.items():
            self.emit_struct_def(struct_def)

        # Emit FFI declarations (deduplicated)
        emitted_ffis: set = set()
        for decl in prog.decls:
            if isinstance(decl, FFIDecl):
                if decl.name not in emitted_ffis:
                    self.emit_ffi_decl(decl)
                    emitted_ffis.add(decl.name)

        # Emit monomorphic instances from type checker
        for instance_name, instance_decl in self.monomorphic_instances.items():
            self.emit_func(instance_decl)

        # Emit non-generic function definitions
        for decl in prog.decls:
            if isinstance(decl, FuncDecl):
                # Skip generic functions
                if decl.is_generic:
                    continue
                self.emit_func(decl)

        # Prepend string constants
        prelude = []
        for s, name in self.string_constants.items():
            length = len(s.encode("utf-8")) + 1
            escaped = s.encode("utf-8").replace(b"\\", b"\\5C").replace(b"\n", b"\\0A").replace(b"\t", b"\\09").replace(b'"', b'\\22')
            prelude.append(f'{name} = private constant [{length} x i8] c"{escaped.decode("latin-1")}\\00"')

        return "\n".join(prelude + [""] + self.output_lines)

    def emit_struct_def(self, struct_def: StructDef):
        """Emit LLVM struct type definition."""
        if struct_def.attributes:
            attr_strs = [f"{attr.name}" + (f"({','.join(map(str, attr.args))})" if attr.args else "") for attr in struct_def.attributes]
            self.emit(f"; struct attributes: [{', '.join(attr_strs)}]")

        field_types = ", ".join(self.llvm_type(f.type) for f in struct_def.fields)
        self.emit(f"%{struct_def.name} = type {{ {field_types} }}")

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
        # Resolve type aliases before getting LLVM type
        resolved = self.resolve_type_alias(t)
        return resolved.llvm_type()

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

    def get_attribute_flags(self, attributes: List[Any]) -> str:
        """Generate LLVM attribute string from attributes."""
        if not attributes:
            return ""

        flags = []
        for attr in attributes:
            if attr.name == 'inline':
                if attr.args and attr.args[0] == 'never':
                    flags.append('noinline')
                elif attr.args and attr.args[0] == 'always':
                    flags.append('alwaysinline')
                else:
                    flags.append('inlinehint')
            elif attr.name == 'no_mangle':
                pass
            elif attr.name == 'cold':
                flags.append('cold')
            elif attr.name == 'hot':
                flags.append('hot')
            elif attr.name == 'obfuscate':
                pass

        return " ".join(flags)

    def next_metadata_id(self) -> int:
        """Get next metadata ID for DWARF."""
        mid = self.metadata_counter
        self.metadata_counter += 1
        return mid

    def emit_dwarf_compile_unit(self, source_file: str):
        """Emit DWARF compilation unit metadata."""
        if not self.debug_enabled:
            return

        cu_id = self.next_metadata_id()
        file_id = self.next_metadata_id()

        self.emit(f"!{file_id} = !DIFile(filename: \"{source_file}\", directory: \".\")")
        self.emit(f"!{cu_id} = distinct !DICompileUnit(language: DW_LANG_C, file: !{file_id}, producer: \"JOCKY Compiler\", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug)")

        return cu_id, file_id

    def emit_function_debug_info(self, func: FuncDecl, cu_id: int, file_id: int) -> int:
        """Emit debug info for a function and return its metadata ID."""
        if not self.debug_enabled or not func.location:
            return -1

        # Create function type metadata
        param_types_meta = []
        for param in func.params:
            param_types_meta.append(self.get_type_metadata(param.type))

        ret_type_meta = self.get_type_metadata(func.ret_type)

        # Create DISubroutineType
        subroutine_type_id = self.next_metadata_id()
        types_list_id = self.next_metadata_id()

        type_refs = ", ".join(f"!{m}" for m in [ret_type_meta] + param_types_meta)
        self.emit(f"!{types_list_id} = !{{{type_refs}}}")
        self.emit(f"!{subroutine_type_id} = !DISubroutineType(types: !{types_list_id})")

        # Create DISubprogram for function
        func_id = self.next_metadata_id()
        self.emit(f"!{func_id} = distinct !DISubprogram(name: \"{func.name}\", linkageName: \"{func.name}\", scope: !{cu_id}, file: !{file_id}, line: {func.location.line}, type: !{subroutine_type_id}, isLocal: false, isDefinition: true, scopeLine: {func.location.line}, flags: DIFlagPrototyped, isOptimized: false)")

        return func_id

    def get_type_metadata(self, jtype: JType) -> int:
        """Get metadata ID for a JOCKY type (simplified DWARF info)."""
        if not self.debug_enabled:
            return -1

        type_name = jtype.name
        if type_name == "void":
            type_id = self.next_metadata_id()
            self.emit(f"!{type_id} = !DIBasicType(name: \"void\", size: 0, encoding: DW_ATE_void)")
            return type_id
        elif type_name == "bool":
            type_id = self.next_metadata_id()
            self.emit(f"!{type_id} = !DIBasicType(name: \"bool\", size: 1, encoding: DW_ATE_boolean)")
            return type_id
        elif type_name == "i8":
            type_id = self.next_metadata_id()
            self.emit(f"!{type_id} = !DIBasicType(name: \"i8\", size: 8, encoding: DW_ATE_signed)")
            return type_id
        elif type_name == "i32":
            type_id = self.next_metadata_id()
            self.emit(f"!{type_id} = !DIBasicType(name: \"i32\", size: 32, encoding: DW_ATE_signed)")
            return type_id
        elif type_name == "i64":
            type_id = self.next_metadata_id()
            self.emit(f"!{type_id} = !DIBasicType(name: \"i64\", size: 64, encoding: DW_ATE_signed)")
            return type_id
        elif type_name == "string":
            type_id = self.next_metadata_id()
            self.emit(f"!{type_id} = !DIBasicType(name: \"string\", size: 64, encoding: DW_ATE_address)")
            return type_id
        else:
            # User-defined type
            type_id = self.next_metadata_id()
            self.emit(f"!{type_id} = !DIBasicType(name: \"{type_name}\", size: 64, encoding: DW_ATE_address)")
            return type_id

    def collect_lets(self, stmts: List[Any]) -> List[Tuple[str, JType]]:
        """Walk statement list recursively and collect (name, type) for every LetStmt.

        Also populates self.locals with type information for variables that are defined
        before they are used (important for closures assigned then called).
        """
        result = []

        for stmt in stmts:
            if isinstance(stmt, LetStmt):
                t = stmt.type if stmt.type else self.infer_type(stmt.init)
                # Resolve type aliases
                t = self.resolve_type_alias(t)
                result.append((stmt.name, t))
                # Add to locals so subsequent expressions can reference it
                # This is important for closure assignments that are called later
                dummy_alloca = f"%temp.{stmt.name}"
                self.locals[stmt.name] = (dummy_alloca, t)
            elif isinstance(stmt, IfStmt):
                result.extend(self.collect_lets(stmt.then_block.stmts))
                if stmt.else_block:
                    result.extend(self.collect_lets(stmt.else_block.stmts))
            elif isinstance(stmt, WhileStmt):
                result.extend(self.collect_lets(stmt.body.stmts))
            elif isinstance(stmt, ForStmt):
                if isinstance(stmt.init, LetStmt):
                    t = stmt.init.type if stmt.init.type else self.infer_type(stmt.init.init)
                    # Resolve type aliases
                    t = self.resolve_type_alias(t)
                    result.append((stmt.init.name, t))
                    # Add to locals
                    dummy_alloca = f"%temp.{stmt.init.name}"
                    self.locals[stmt.init.name] = (dummy_alloca, t)
                result.extend(self.collect_lets(stmt.body.stmts))

        return result

    def emit_func(self, decl: FuncDecl):
        self.current_func = decl.name
        self.reg_counter = 0
        self.label_counter = 0
        self.locals = {}
        self.loop_stack = []
        self.current_block = "entry"

        # Check if function should be virtualized
        if decl.attributes:
            for attr in decl.attributes:
                if attr.name == 'virtualize':
                    self.emit_virtualized_function(decl)
                    return

        # Emit function debug info
        func_debug_id = self.emit_function_debug_info(decl, self.cu_id, self.file_id) if self.debug_enabled else -1

        if decl.attributes:
            attr_strs = [f"{attr.name}" + (f"({','.join(map(str, attr.args))})" if attr.args else "") for attr in decl.attributes]
            self.emit(f"; attributes: [{', '.join(attr_strs)}]")

        params = ", ".join(f"{self.llvm_type(p.type)} %{p.name}" for p in decl.params)
        ret = self.llvm_type(decl.ret_type)

        # Attach debug info to function definition
        debug_suffix = f" !dbg !{func_debug_id}" if func_debug_id >= 0 else ""
        self.emit(f"define {ret} @{decl.name}({params}){debug_suffix} {{")
        self.emit("entry:")

        # Allocate params
        for p in decl.params:
            alloca = self.next_reg()
            self.emit(f"  {alloca} = alloca {self.llvm_type(p.type)}")
            self.emit(f"  store {self.llvm_type(p.type)} %{p.name}, {self.llvm_type(p.type)}* {alloca}")
            self.locals[p.name] = (alloca, p.type)

        # Hoist all LetStmt allocas to the entry block for mem2reg compatibility
        all_lets = self.collect_lets(decl.body.stmts)
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

    def emit_virtualized_function(self, decl: FuncDecl):
        """Emit a virtualized function using bytecode VM."""
        compiler = BytecodeCompiler()
        bytecode = compiler.compile_function(decl, obfuscate=True)

        bytecode_name = f"@.vm.{decl.name}"
        bytecode_len = len(bytecode) if bytecode else 1
        if not bytecode:
            bytecode = bytes([0xFF])

        bytecode_bytes = ", ".join(f"i8 {b}" for b in bytecode)
        self.emit(f"{bytecode_name} = internal constant [{bytecode_len} x i8] [{bytecode_bytes}]")

        declare_stubs = [
            "declare i8* @jocky_vm_create()",
            "declare void @jocky_vm_load(i8*, i8*, i64)",
            "declare i64 @jocky_vm_execute(i8*, i64, i64, i64, i64)",
            "declare void @jocky_vm_destroy(i8*)",
        ]
        for stub in declare_stubs:
            if stub not in self.output_lines:
                self.output_lines.insert(0, stub)

        params = ", ".join(f"{self.llvm_type(p.type)} %{p.name}" for p in decl.params)
        ret = self.llvm_type(decl.ret_type)

        self.emit(f"define {ret} @{decl.name}({params}) {{")
        self.emit("entry:")

        self.emit(f"  %vm = call i8* @jocky_vm_create()")
        self.emit(f"  %bytecode_ptr = getelementptr [{bytecode_len} x i8], [{bytecode_len} x i8]* {bytecode_name}, i32 0, i32 0")
        self.emit(f"  call void @jocky_vm_load(i8* %vm, i8* %bytecode_ptr, i64 {bytecode_len})")

        if len(decl.params) > 0:
            self.emit(f"  %arg0 = sext {self.llvm_type(decl.params[0].type)} %{decl.params[0].name} to i64")

        if len(decl.params) > 1:
            self.emit(f"  %arg1 = sext {self.llvm_type(decl.params[1].type)} %{decl.params[1].name} to i64")

        if len(decl.params) > 2:
            self.emit(f"  %arg2 = sext {self.llvm_type(decl.params[2].type)} %{decl.params[2].name} to i64")

        if len(decl.params) > 3:
            self.emit(f"  %arg3 = sext {self.llvm_type(decl.params[3].type)} %{decl.params[3].name} to i64")

        arg0 = "%arg0" if len(decl.params) > 0 else "0"
        arg1 = "%arg1" if len(decl.params) > 1 else "0"
        arg2 = "%arg2" if len(decl.params) > 2 else "0"
        arg3 = "%arg3" if len(decl.params) > 3 else "0"

        self.emit(f"  %result = call i64 @jocky_vm_execute(i8* %vm, i64 {arg0}, i64 {arg1}, i64 {arg2}, i64 {arg3})")
        self.emit(f"  call void @jocky_vm_destroy(i8* %vm)")

        if decl.ret_type.name == "void":
            self.emit("  ret void")
        elif decl.ret_type.name in ["i32", "i16", "i8"]:
            ret_type = self.llvm_type(decl.ret_type)
            self.emit(f"  %ret = trunc i64 %result to {ret_type}")
            self.emit(f"  ret {ret_type} %ret")
        else:
            self.emit(f"  ret i64 %result")

        self.emit("}")

    def last_line_is_terminator(self) -> bool:
        if not self.output_lines:
            return False
        last = self.output_lines[-1].strip()
        return last.startswith("ret ") or last.startswith("br ")

    def emit_block(self, block: Block):
        for i, stmt in enumerate(block.stmts):
            is_last = (i == len(block.stmts) - 1)
            # Check if last statement is a raw expression (implicit return)
            if is_last and not isinstance(stmt, (ExprStmt, LetStmt, AssignStmt, IfStmt, WhileStmt, ForStmt, ReturnStmt, BreakStmt, ContinueStmt)):
                # This is an implicit return expression
                val, vtype = self.emit_expr(stmt)
                # If in a typed function, return the value; otherwise just evaluate
                if self.current_func and hasattr(self, 'current_func'):
                    # Try to get the function's return type
                    if self.current_func in self.functions:
                        func_sig = self.functions[self.current_func]
                        if len(func_sig) >= 2:
                            ret_type = self.resolve_type_alias(func_sig[1])
                            if ret_type.name != "void":
                                self.emit(f"  ret {self.llvm_type(ret_type)} {val}")
                            else:
                                self.emit("  ret void")
                        else:
                            self.emit(f"  ret {self.llvm_type(vtype)} {val}")
            else:
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
            # For function types, preserve the full type info from vt
            if vt.name == "fn":
                self.locals[stmt.name] = (alloca, vt)
            else:
                self.locals[stmt.name] = (alloca, t)
        elif isinstance(stmt, AssignStmt):
            if isinstance(stmt.target, VarRef):
                if stmt.target.name not in self.locals:
                    raise CodeGenError(f"Undefined variable: {stmt.target.name}")
                alloca, t = self.locals[stmt.target.name]
                val, vt = self.emit_expr(stmt.value)
                if t.name != vt.name or t.is_pointer != vt.is_pointer:
                    val = self.emit_cast(val, vt, t)
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

    def emit_to_bool(self, val: str, t: JType) -> str:
        if self.is_numeric(t):
            r = self.next_reg()
            self.emit(f"  {r} = icmp ne {self.llvm_type(t)} {val}, 0")
            return r
        if t.name == "string":
            r = self.next_reg()
            self.emit(f"  {r} = icmp ne i8* {val}, null")
            return r
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
        raise CodeGenError(f"Cannot cast {from_t} to {to_t}")

    def is_numeric(self, t: JType) -> bool:
        return t.name in ("i8", "i32", "i64") and not t.is_pointer

    def infer_type(self, expr: Any) -> JType:
        if isinstance(expr, IntLiteral):
            return JType("i32")
        elif isinstance(expr, BoolLiteral):
            return JType("bool")
        elif isinstance(expr, StringLiteral):
            return JType("string")
        elif isinstance(expr, NullLiteral):
            return JType("i8", is_pointer=True)
        elif isinstance(expr, VarRef):
            if expr.name in self.locals:
                return self.locals[expr.name][1]
            # Check if this is an enum variant reference
            for enum_name, enum_def in self.enums.items():
                for variant in enum_def.variants:
                    if variant.name == expr.name:
                        return JType(enum_name)
            raise CodeGenError(f"Undefined variable: {expr.name}")
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
            # Check if calling a local variable holding a function pointer
            if expr.name in self.locals:
                alloca, var_type = self.locals[expr.name]
                if var_type.name == "fn":
                    if hasattr(var_type, 'return_type'):
                        return var_type.return_type
                    else:
                        # Function type without explicit return type info
                        return JType("i32")
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
        elif isinstance(expr, ArrayLiteralExpr):
            if len(expr.elements) == 0:
                raise CodeGenError("Cannot infer type of empty array")
            elem_type = self.infer_type(expr.elements[0])
            return JType(elem_type.name, is_array=True, array_size=len(expr.elements))
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
            # Closure type is function pointer
            ret_type = expr.ret_type if expr.ret_type else JType("void")
            func_type = JType("fn", is_pointer=True)
            # Store parameter and return types so they can be used later
            func_type.param_types = [p.type for p in expr.params]
            func_type.return_type = ret_type
            return func_type
        raise CodeGenError(f"Cannot infer type for {type(expr).__name__}")

    def emit_expr(self, expr: Any) -> Tuple[str, JType]:
        if isinstance(expr, IntLiteral):
            return (str(expr.value), JType("i32"))
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
            # Check if this is an enum variant reference
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
        elif isinstance(expr, ClosureExpr):
            return self.emit_closure(expr)
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
            # For arrays, element type is stored in the base name
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
        elif isinstance(expr, ArrayLiteralExpr):
            # Allocate array on stack and initialize elements
            elem_type = JType("i32")  # Will be inferred from first element
            if len(expr.elements) > 0:
                elem_val, elem_type = self.emit_expr(expr.elements[0])
            # Allocate array
            array_type = JType(elem_type.name, is_array=True, array_size=len(expr.elements))
            alloca = self.next_reg()
            self.emit(f"  {alloca} = alloca {self.llvm_type(array_type)}")
            # Initialize elements
            for i, elem_expr in enumerate(expr.elements):
                elem_val, elem_type = self.emit_expr(elem_expr)
                gep = self.next_reg()
                self.emit(f"  {gep} = getelementptr {self.llvm_type(elem_type)}, {self.llvm_type(array_type)} {alloca}, i32 0, i32 {i}")
                self.emit(f"  store {self.llvm_type(elem_type)} {elem_val}, {self.llvm_type(elem_type)}* {gep}")
            return (alloca, array_type)
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

    def emit_binary(self, expr: BinaryOp) -> Tuple[str, JType]:
        left, lt = self.emit_expr(expr.left)
        right, rt = self.emit_expr(expr.right)

        # Promote to common type for arithmetic / bitwise
        if self.is_numeric(lt) and self.is_numeric(rt):
            if lt.name != rt.name:
                if lt.size_bytes() < rt.size_bytes():
                    left = self.emit_cast(left, lt, rt)
                    lt = rt
                else:
                    right = self.emit_cast(right, rt, lt)
                    rt = lt

        op = expr.op
        r = self.next_reg()
        typ = lt

        if op == "+":
            self.emit(f"  {r} = add {self.llvm_type(typ)} {left}, {right}")
        elif op == "-":
            self.emit(f"  {r} = sub {self.llvm_type(typ)} {left}, {right}")
        elif op == "*":
            self.emit(f"  {r} = mul {self.llvm_type(typ)} {left}, {right}")
        elif op == "/":
            self.emit(f"  {r} = sdiv {self.llvm_type(typ)} {left}, {right}")
        elif op == "%":
            self.emit(f"  {r} = srem {self.llvm_type(typ)} {left}, {right}")
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
        elif op == "==":
            self.emit(f"  {r} = icmp eq {self.llvm_type(typ)} {left}, {right}")
            typ = JType("bool")
        elif op == "!=":
            self.emit(f"  {r} = icmp ne {self.llvm_type(typ)} {left}, {right}")
            typ = JType("bool")
        elif op == "<":
            self.emit(f"  {r} = icmp slt {self.llvm_type(typ)} {left}, {right}")
            typ = JType("bool")
        elif op == ">":
            self.emit(f"  {r} = icmp sgt {self.llvm_type(typ)} {left}, {right}")
            typ = JType("bool")
        elif op == "<=":
            self.emit(f"  {r} = icmp sle {self.llvm_type(typ)} {left}, {right}")
            typ = JType("bool")
        elif op == ">=":
            self.emit(f"  {r} = icmp sge {self.llvm_type(typ)} {left}, {right}")
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

    def emit_closure(self, expr: ClosureExpr) -> Tuple[str, JType]:
        """Emit LLVM code for closure/lambda expression.

        For capturing closures, generates an environment struct to hold captured variables,
        and modifies the closure function to accept an environment pointer as first parameter.
        """
        # Generate unique function name for this closure
        closure_name = f"__closure_{self.label_counter}"
        self.label_counter += 1

        ret_type = expr.ret_type if expr.ret_type else JType("void")

        # Check if this closure captures variables
        has_captures = len(expr.captures) > 0
        env_struct_name = None
        capture_types = {}

        if has_captures:
            env_struct_name = f"{closure_name}_env"

            # Collect types of captured variables from outer scope
            for capture_name in expr.captures:
                if capture_name in self.locals:
                    _, capture_type = self.locals[capture_name]
                    capture_types[capture_name] = capture_type
                else:
                    raise CodeGenError(f"Captured variable '{capture_name}' not found in scope")

            # Emit environment struct type definition
            field_types = ", ".join(self.llvm_type(capture_types[cap]) for cap in expr.captures)
            self.emit(f"%{env_struct_name} = type {{ {field_types} }}")

        # Build function signature with environment pointer if needed
        param_types = [p.type for p in expr.params]
        if has_captures:
            env_param = f"%{env_struct_name}* %__env"
            param_list_items = [env_param]
            param_list_items.extend(f"{self.llvm_type(pt)} %{pname}"
                                   for pt, pname in zip(param_types, [p.name for p in expr.params]))
            param_list = ", ".join(param_list_items)
        else:
            param_list = ", ".join(f"{self.llvm_type(pt)} %{pname}"
                                  for pt, pname in zip(param_types, [p.name for p in expr.params]))

        # Save current function state
        old_func = self.current_func
        old_locals = self.locals
        old_reg_counter = self.reg_counter
        old_label_counter = self.label_counter

        self.current_func = closure_name
        self.locals = {}
        self.reg_counter = 0

        # If capturing, load captured variables from environment struct
        if has_captures:
            for i, capture_name in enumerate(expr.captures):
                capture_type = capture_types[capture_name]
                # Get pointer to struct field
                gep_reg = self.next_reg()
                self.emit(f"  {gep_reg} = getelementptr %{env_struct_name}, %{env_struct_name}* %__env, i32 0, i32 {i}")
                # Load value from field
                load_reg = self.next_reg()
                self.emit(f"  {load_reg} = load {self.llvm_type(capture_type)}, {self.llvm_type(capture_type)}* {gep_reg}")
                # Store in local variable
                self.locals[capture_name] = (load_reg, capture_type)

        # Add parameters to locals
        for param in expr.params:
            alloca = f"%{param.name}.addr"
            self.locals[param.name] = (alloca, param.type)

        # Emit function header
        self.emit(f"define {self.llvm_type(ret_type)} @{closure_name}({param_list}) {{")

        # Emit body
        if isinstance(expr.body, Block):
            for i, stmt in enumerate(expr.body.stmts):
                is_last = (i == len(expr.body.stmts) - 1)
                # Check if last statement is a raw expression (implicit return)
                if is_last and not isinstance(stmt, (ExprStmt, LetStmt, AssignStmt, IfStmt, WhileStmt, ForStmt, ReturnStmt, BreakStmt, ContinueStmt)):
                    # This is an implicit return expression
                    val, _ = self.emit_expr(stmt)
                    if ret_type.name != "void":
                        self.emit(f"  ret {self.llvm_type(ret_type)} {val}")
                    else:
                        self.emit("  ret void")
                else:
                    self.emit_stmt(stmt)
        else:
            # Expression body
            val, _ = self.emit_expr(expr.body)
            if ret_type.name != "void":
                self.emit(f"  ret {self.llvm_type(ret_type)} {val}")
            else:
                self.emit("  ret void")

        self.emit("}")
        self.emit("")

        # Restore state
        self.current_func = old_func
        self.locals = old_locals
        self.reg_counter = old_reg_counter
        self.label_counter = old_label_counter

        # Return function pointer with environment info
        func_ptr_type = JType("fn", is_pointer=True)
        func_ptr_type.param_types = param_types
        func_ptr_type.return_type = ret_type
        if has_captures:
            func_ptr_type.env_struct = env_struct_name
            func_ptr_type.captures = expr.captures
        return (f"@{closure_name}", func_ptr_type)

    def find_monomorphic_instance(self, func_name: str, arg_types: List[JType]) -> Optional[str]:
        """Find the monomorphic instance that matches the given function name and argument types."""
        for instance_name, instance_sig in self.functions.items():
            if not instance_name.startswith(func_name + "__"):
                continue
            if len(instance_sig) >= 2:
                ptypes, _ = instance_sig[0], instance_sig[1]
                if len(ptypes) == len(arg_types):
                    all_match = True
                    for pt, at in zip(ptypes, arg_types):
                        if pt.name != at.name or pt.is_pointer != at.is_pointer:
                            all_match = False
                            break
                    if all_match:
                        return instance_name
        return None

    def infer_arg_types_simple(self, args: List[Any]) -> List[JType]:
        """Infer types of arguments without emitting code."""
        arg_types = []
        for arg in args:
            if isinstance(arg, IntLiteral):
                arg_types.append(JType("i32"))
            elif isinstance(arg, BoolLiteral):
                arg_types.append(JType("bool"))
            elif isinstance(arg, StringLiteral):
                arg_types.append(JType("string"))
            elif isinstance(arg, VarRef):
                if arg.name in self.locals:
                    alloca, vtype = self.locals[arg.name]
                    arg_types.append(self.resolve_type_alias(vtype))
                else:
                    arg_types.append(JType("unknown"))
            else:
                # For complex expressions, return unknown
                arg_types.append(JType("unknown"))
        return arg_types

    def emit_call(self, expr: CallExpr) -> Tuple[str, JType]:
        # Check if this is a call to a local variable holding a function pointer
        if expr.name in self.locals:
            alloca, var_type = self.locals[expr.name]
            if var_type.name == "fn":
                # Indirect call through function pointer
                return self.emit_indirect_call(expr, alloca, var_type)

        # Check if this is a generic function call that has been instantiated
        actual_func_name = expr.name
        if expr.name not in self.functions:
            # Try to find a monomorphic instance by inferring argument types
            arg_types = self.infer_arg_types_simple(expr.args)
            instance_name = self.find_monomorphic_instance(expr.name, arg_types)
            if instance_name:
                actual_func_name = instance_name
            else:
                raise CodeGenError(f"Undefined function: {expr.name}")

        # Otherwise, direct function call
        if actual_func_name not in self.functions:
            raise CodeGenError(f"Undefined function: {actual_func_name}")
        sig = self.functions[actual_func_name]
        if len(sig) == 3:
            ptypes, ret, _ = sig
        else:
            ptypes, ret = sig
        # Resolve type aliases in parameter and return types
        ptypes = [self.resolve_type_alias(pt) for pt in ptypes]
        ret = self.resolve_type_alias(ret)
        args = []
        for i, arg in enumerate(expr.args):
            val, vt = self.emit_expr(arg)
            vt = self.resolve_type_alias(vt)
            if i < len(ptypes) and (ptypes[i].name != vt.name or ptypes[i].is_pointer != vt.is_pointer):
                val = self.emit_cast(val, vt, ptypes[i])
                vt = ptypes[i]
            args.append(f"{self.llvm_type(vt)} {val}")
        arg_str = ", ".join(args)
        if ret.name == "void":
            self.emit(f"  call void @{actual_func_name}({arg_str})")
            return ("", ret)
        r = self.next_reg()
        self.emit(f"  {r} = call {self.llvm_type(ret)} @{actual_func_name}({arg_str})")
        return (r, ret)

    def emit_indirect_call(self, expr: CallExpr, alloca: str, func_type: JType) -> Tuple[str, JType]:
        """Emit indirect call through a function pointer stored in a variable."""
        # Load the function pointer from the alloca
        func_ptr = self.next_reg()
        self.emit(f"  {func_ptr} = load i8*, i8** {alloca}")

        # Get parameter types and return type from the function type
        if hasattr(func_type, 'param_types'):
            ptypes = func_type.param_types
        else:
            raise CodeGenError(f"Cannot determine parameter types for indirect call through {expr.name}")

        if hasattr(func_type, 'return_type'):
            ret = func_type.return_type
        else:
            ret = JType("void")

        # Resolve type aliases
        ptypes = [self.resolve_type_alias(pt) for pt in ptypes]
        ret = self.resolve_type_alias(ret)

        # Emit arguments
        args = []
        for i, arg in enumerate(expr.args):
            val, vt = self.emit_expr(arg)
            vt = self.resolve_type_alias(vt)
            if i < len(ptypes) and (ptypes[i].name != vt.name or ptypes[i].is_pointer != vt.is_pointer):
                val = self.emit_cast(val, vt, ptypes[i])
                vt = ptypes[i]
            args.append(f"{self.llvm_type(vt)} {val}")

        arg_str = ", ".join(args)

        # Build the function type signature for casting
        # Format: return_type (param_type1, param_type2, ...)
        param_types_str = ", ".join(self.llvm_type(pt) for pt in ptypes)
        func_sig = f"{self.llvm_type(ret)} ({param_types_str})"

        # Bitcast the function pointer to the correct function type and call
        func_ptr_typed = self.next_reg()
        self.emit(f"  {func_ptr_typed} = bitcast i8* {func_ptr} to {func_sig}*")

        if ret.name == "void":
            self.emit(f"  call void {func_sig}* {func_ptr_typed}({arg_str})")
            return ("", ret)

        r = self.next_reg()
        self.emit(f"  {r} = call {func_sig}* {func_ptr_typed}({arg_str})")
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
