from .lexer import Token, TokenType, Lexer
from .ast import (
    Program, FuncDecl, FFIDecl, Param, Block,
    LetStmt, AssignStmt, IfStmt, WhileStmt, ForStmt,
    ReturnStmt, ExprStmt, BreakStmt, ContinueStmt,
    IntLiteral, BoolLiteral, StringLiteral, VarRef, NullLiteral,
    BinaryOp, UnaryOp, CallExpr, DerefExpr, AddrOfExpr,
    CastExpr, IndexExpr,
    StructDef, StructField, StructLiteralExpr, FieldAccessExpr,
    ArrayType, ArrayLiteralExpr,
    EnumDef, EnumVariant,
    TypeAlias,
    Pattern, WildcardPattern, LiteralPattern, VariantPattern, MatchArm, MatchExpr,
    ModulePath, UseStmt, ModDecl,
    ClosureExpr, CaptureVar,
    Attribute,
    JType,
    SourceLocation,
)
from .errors import ParseError as BaseParseError, SourceRange
from typing import Optional, Any, List

class ParseError(BaseParseError):
    pass

class Parser:
    def __init__(self, tokens: List[Token], source_file: str = "<unknown>"):
        self.tokens = tokens
        self.pos = 0
        self.source_file = source_file

    def peek(self) -> Token:
        if self.pos < len(self.tokens):
            return self.tokens[self.pos]
        return self.tokens[-1]

    def advance(self) -> Token:
        tok = self.peek()
        self.pos += 1
        return tok

    def expect(self, ttype: TokenType, msg: str = "") -> Token:
        tok = self.peek()
        if tok.type != ttype:
            source_range = SourceRange.at(tok.line, tok.column)
            error_msg = f"Expected {ttype.name}, got {tok.type.name} ({tok.value})"
            if msg:
                error_msg += f": {msg}"
            raise ParseError(error_msg, source_range)
        return self.advance()

    def match(self, *types: TokenType) -> bool:
        return self.peek().type in types

    def make_location(self, start_tok: Token, end_tok: Optional[Token] = None) -> SourceLocation:
        """Create a SourceLocation from token(s)."""
        if end_tok is None:
            end_tok = start_tok
        return SourceLocation(
            filename=self.source_file,
            line=start_tok.line,
            column=start_tok.column,
            end_line=end_tok.line,
            end_column=end_tok.column
        )

    def parse_type(self, allow_type_vars: bool = False) -> JType:
        tok = self.peek()
        if tok.type == TokenType.I8:
            self.advance()
            t = JType("i8")
        elif tok.type == TokenType.I32:
            self.advance()
            t = JType("i32")
        elif tok.type == TokenType.I64:
            self.advance()
            t = JType("i64")
        elif tok.type == TokenType.BOOL:
            self.advance()
            t = JType("bool")
        elif tok.type == TokenType.VOID:
            self.advance()
            t = JType("void")
        elif tok.type == TokenType.STRING_KW:
            self.advance()
            t = JType("string")
        elif tok.type == TokenType.IDENT:
            # Could be user-defined type or type variable
            name = self.advance().value
            # Type variables are single uppercase letters (T, U, V, etc.)
            is_type_var = allow_type_vars and name[0].isupper() and len(name) == 1
            t = JType(name, is_type_var=is_type_var, type_var_name=name if is_type_var else None)
        else:
            source_range = SourceRange.at(tok.line, tok.column)
            raise ParseError(f"Expected type, got {tok.type.name}", source_range)

        # Array type
        if self.match(TokenType.LBRACKET):
            self.advance()
            size = 0
            if self.match(TokenType.NUMBER):
                size = self.advance().value
            self.expect(TokenType.RBRACKET)
            t = JType(t.name, is_array=True, array_size=size, is_type_var=t.is_type_var, type_var_name=t.type_var_name)

        # Pointer type
        while self.match(TokenType.STAR):
            self.advance()
            t = JType(t.name, is_pointer=True, is_array=t.is_array, array_size=t.array_size, is_type_var=t.is_type_var, type_var_name=t.type_var_name)

        return t

    def parse_attributes(self) -> List[Attribute]:
        """Parse attributes: #[name], #[name(arg1, arg2)], etc."""
        attributes = []

        while self.match(TokenType.HASH):
            self.advance()
            self.expect(TokenType.LBRACKET)

            attr_name = self.expect(TokenType.IDENT).value

            attr_args = []
            if self.match(TokenType.LPAREN):
                self.advance()
                while not self.match(TokenType.RPAREN):
                    if self.match(TokenType.STRING):
                        attr_args.append(self.advance().value)
                    elif self.match(TokenType.IDENT):
                        attr_args.append(self.advance().value)
                    elif self.match(TokenType.NUMBER):
                        attr_args.append(self.advance().value)
                    elif self.match(TokenType.I8, TokenType.I32, TokenType.I64,
                                   TokenType.BOOL, TokenType.VOID, TokenType.STRING_KW):
                        attr_args.append(self.advance().value)
                    else:
                        tok = self.peek()
                        source_range = SourceRange.at(tok.line, tok.column)
                        raise ParseError(f"Invalid attribute argument: {tok.type.name}", source_range)

                    if self.match(TokenType.COMMA):
                        self.advance()

                self.expect(TokenType.RPAREN)

            self.expect(TokenType.RBRACKET)
            attributes.append(Attribute(attr_name, attr_args))

        return attributes

    def parse(self) -> Program:
        decls = []
        while not self.match(TokenType.EOF):
            decls.append(self.parse_decl())
        return Program(decls)

    def parse_decl(self) -> Any:
        attributes = self.parse_attributes()

        if self.match(TokenType.FN):
            return self.parse_func_decl_with_attributes(attributes)
        elif self.match(TokenType.FFI):
            return self.parse_ffi_decl_with_attributes(attributes)
        elif self.match(TokenType.STRUCT):
            return self.parse_struct_decl_with_attributes(attributes)
        elif self.match(TokenType.ENUM):
            return self.parse_enum_decl_with_attributes(attributes)
        elif self.match(TokenType.TYPE):
            return self.parse_type_alias()
        elif self.match(TokenType.USE):
            return self.parse_use_stmt()
        elif self.match(TokenType.MOD):
            return self.parse_mod_decl()
        elif attributes:
            tok = self.peek()
            source_range = SourceRange.at(tok.line, tok.column)
            raise ParseError(f"Attributes can only be applied to fn, ffi, struct, or enum declarations", source_range)
        else:
            tok = self.peek()
            source_range = SourceRange.at(tok.line, tok.column)
            raise ParseError(f"Unexpected token {tok.type.name}; expected fn, ffi, struct, enum, type, mod, or use", source_range)

    def parse_func_decl_with_attributes(self, attributes: List[Attribute]) -> FuncDecl:
        start_tok = self.peek()
        self.expect(TokenType.FN)
        name = self.expect(TokenType.IDENT).value

        # Parse optional type parameters: <T, U, V>
        type_params = []
        if self.match(TokenType.LT):
            self.advance()
            while not self.match(TokenType.GT):
                param_name = self.expect(TokenType.IDENT).value
                type_params.append(param_name)
                if self.match(TokenType.COMMA):
                    self.advance()
            self.expect(TokenType.GT)

        self.expect(TokenType.LPAREN)
        params = self.parse_params(allow_type_vars=len(type_params) > 0)
        self.expect(TokenType.RPAREN)
        ret_type = JType("void")
        if self.match(TokenType.ARROW):
            self.advance()
            ret_type = self.parse_type(allow_type_vars=len(type_params) > 0)
        body = self.parse_block()

        location = self.make_location(start_tok)
        return FuncDecl(name, params, ret_type, body, attributes, type_params=type_params if type_params else None, location=location)

    def parse_func_decl(self) -> FuncDecl:
        return self.parse_func_decl_with_attributes([])

    def parse_ffi_decl_with_attributes(self, attributes: List[Attribute]) -> FFIDecl:
        self.expect(TokenType.FFI)
        name = self.expect(TokenType.IDENT).value
        self.expect(TokenType.LPAREN)
        params, variadic = self.parse_params_variadic()
        self.expect(TokenType.RPAREN)
        self.expect(TokenType.ARROW)
        ret_type = self.parse_type()
        self.expect(TokenType.SEMICOLON)
        return FFIDecl(name, params, ret_type, variadic, attributes)

    def parse_ffi_decl(self) -> FFIDecl:
        return self.parse_ffi_decl_with_attributes([])

    def parse_struct_decl_with_attributes(self, attributes: List[Attribute]) -> StructDef:
        self.expect(TokenType.STRUCT)
        name = self.expect(TokenType.IDENT).value
        self.expect(TokenType.LBRACE)
        fields = []
        while not self.match(TokenType.RBRACE):
            field_name = self.expect(TokenType.IDENT).value
            self.expect(TokenType.COLON)
            field_type = self.parse_type()
            fields.append(StructField(field_name, field_type))
            if self.match(TokenType.SEMICOLON):
                self.advance()
        self.expect(TokenType.RBRACE)
        self.expect(TokenType.SEMICOLON)
        return StructDef(name, fields, attributes)

    def parse_struct_decl(self) -> StructDef:
        return self.parse_struct_decl_with_attributes([])

    def parse_enum_decl_with_attributes(self, attributes: List[Attribute]) -> EnumDef:
        self.expect(TokenType.ENUM)
        name = self.expect(TokenType.IDENT).value
        self.expect(TokenType.LBRACE)
        variants = []
        while not self.match(TokenType.RBRACE):
            var_name = self.expect(TokenType.IDENT).value
            value = None
            fields = None

            # Check for tagged union fields: VariantName(field: Type, ...)
            if self.match(TokenType.LPAREN):
                self.advance()
                fields = []
                while not self.match(TokenType.RPAREN):
                    field_name = self.expect(TokenType.IDENT).value
                    self.expect(TokenType.COLON)
                    field_type = self.parse_type()
                    fields.append(StructField(field_name, field_type))
                    if self.match(TokenType.COMMA):
                        self.advance()
                self.expect(TokenType.RPAREN)
            # Or plain variant with value: VariantName = 42
            elif self.match(TokenType.EQ):
                self.advance()
                value = self.expect(TokenType.NUMBER).value

            variants.append(EnumVariant(var_name, value, fields))
            if self.match(TokenType.COMMA):
                self.advance()
        self.expect(TokenType.RBRACE)
        self.expect(TokenType.SEMICOLON)
        return EnumDef(name, variants, attributes)

    def parse_enum_decl(self) -> EnumDef:
        return self.parse_enum_decl_with_attributes([])

    def parse_type_alias(self) -> TypeAlias:
        self.expect(TokenType.TYPE)
        name = self.expect(TokenType.IDENT).value
        self.expect(TokenType.EQ)
        target_type = self.parse_type()
        self.expect(TokenType.SEMICOLON)
        return TypeAlias(name, target_type)

    def parse_use_stmt(self) -> UseStmt:
        self.expect(TokenType.USE)
        components = []
        components.append(self.expect(TokenType.IDENT).value)

        all_flag = False
        while self.match(TokenType.COLON):
            self.advance()
            self.expect(TokenType.COLON)
            if self.match(TokenType.STAR):
                self.advance()
                all_flag = True
                break
            else:
                components.append(self.expect(TokenType.IDENT).value)

        self.expect(TokenType.SEMICOLON)
        return UseStmt(ModulePath(components), all=all_flag)

    def parse_mod_decl(self) -> ModDecl:
        public = False
        if self.match(TokenType.IDENT) and self.peek().value == "pub":
            self.advance()
            public = True

        self.expect(TokenType.MOD)
        name = self.expect(TokenType.IDENT).value
        self.expect(TokenType.LBRACE)

        items = []
        while not self.match(TokenType.RBRACE):
            items.append(self.parse_decl())

        self.expect(TokenType.RBRACE)
        return ModDecl(name, items, public=public)

    def parse_match_expr(self) -> MatchExpr:
        self.expect(TokenType.MATCH)
        scrutinee = self.parse_expr()
        self.expect(TokenType.LBRACE)
        arms = []
        while not self.match(TokenType.RBRACE):
            pattern = self.parse_pattern()
            self.expect(TokenType.ARROW_FAT)
            # Parse the arm body (could be a block or single expression)
            if self.match(TokenType.LBRACE):
                body = self.parse_block()
            else:
                # Single expression arm
                expr = self.parse_expr()
                body = Block([ExprStmt(expr)])
            arms.append(MatchArm(pattern, body))
            if self.match(TokenType.COMMA):
                self.advance()
        self.expect(TokenType.RBRACE)
        return MatchExpr(scrutinee, arms)

    def parse_closure_or_expr_block(self) -> Block:
        """Parse block allowing implicit return (last expr without semicolon)."""
        self.expect(TokenType.LBRACE)
        stmts = []
        while not self.match(TokenType.RBRACE):
            if self.match(TokenType.LET):
                stmts.append(self.parse_let_stmt())
            elif self.match(TokenType.IF):
                stmts.append(self.parse_if_stmt())
            elif self.match(TokenType.WHILE):
                stmts.append(self.parse_while_stmt())
            elif self.match(TokenType.FOR):
                stmts.append(self.parse_for_stmt())
            elif self.match(TokenType.RETURN):
                stmts.append(self.parse_return_stmt())
            elif self.match(TokenType.BREAK):
                self.advance()
                self.expect(TokenType.SEMICOLON)
                stmts.append(BreakStmt())
            elif self.match(TokenType.CONTINUE):
                self.advance()
                self.expect(TokenType.SEMICOLON)
                stmts.append(ContinueStmt())
            else:
                # Expression statement - might be implicit return if last
                expr = self.parse_expr()
                if self.match(TokenType.SEMICOLON):
                    # Explicit semicolon - this is a statement
                    self.advance()
                    stmts.append(ExprStmt(expr))
                elif self.match(TokenType.RBRACE):
                    # No semicolon before closing brace - implicit return expression
                    stmts.append(expr)
                    break
                else:
                    # No semicolon and not at closing brace - error
                    self.expect(TokenType.SEMICOLON)
        self.expect(TokenType.RBRACE)
        return Block(stmts)

    def parse_closure_expr(self) -> ClosureExpr:
        """Parse closure expression: |params| { body } or |params| expr"""
        self.expect(TokenType.PIPE)

        # Parse parameters and captures
        params = []
        captures = []

        while not self.match(TokenType.PIPE):
            by_ref = False

            # Check for reference capture: &var
            if self.match(TokenType.AMPERSAND):
                self.advance()
                by_ref = True

            name = self.expect(TokenType.IDENT).value

            # Optional type annotation: var: Type
            if self.match(TokenType.COLON):
                self.advance()
                param_type = self.parse_type()
                params.append(Param(name, param_type))
            else:
                # No type annotation means it's a capture
                captures.append(name)

            if self.match(TokenType.COMMA):
                self.advance()

        self.expect(TokenType.PIPE)

        # Parse return type annotation: | -> Type
        ret_type = None
        if self.match(TokenType.ARROW):
            self.advance()
            ret_type = self.parse_type()

        # Parse body (block or expression)
        if self.match(TokenType.LBRACE):
            body = self.parse_block()
        else:
            # Single expression body
            body = self.parse_expr()

        return ClosureExpr(params, ret_type, captures, body)

    def collect_free_variables(self, expr: Any, bound_vars: set) -> set:
        """Collect all free variables (not in bound_vars) referenced in expression."""
        free_vars = set()

        if isinstance(expr, VarRef):
            if expr.name not in bound_vars:
                free_vars.add(expr.name)
        elif isinstance(expr, BinaryOp):
            free_vars.update(self.collect_free_variables(expr.left, bound_vars))
            free_vars.update(self.collect_free_variables(expr.right, bound_vars))
        elif isinstance(expr, UnaryOp):
            free_vars.update(self.collect_free_variables(expr.operand, bound_vars))
        elif isinstance(expr, CallExpr):
            for arg in expr.args:
                free_vars.update(self.collect_free_variables(arg, bound_vars))
        elif isinstance(expr, FieldAccessExpr):
            free_vars.update(self.collect_free_variables(expr.object, bound_vars))
        elif isinstance(expr, IndexExpr):
            free_vars.update(self.collect_free_variables(expr.base, bound_vars))
            free_vars.update(self.collect_free_variables(expr.index, bound_vars))
        elif isinstance(expr, CastExpr):
            free_vars.update(self.collect_free_variables(expr.operand, bound_vars))
        elif isinstance(expr, DerefExpr):
            free_vars.update(self.collect_free_variables(expr.operand, bound_vars))
        elif isinstance(expr, AddrOfExpr):
            free_vars.update(self.collect_free_variables(expr.operand, bound_vars))
        elif isinstance(expr, StructLiteralExpr):
            for field_val in expr.fields.values():
                free_vars.update(self.collect_free_variables(field_val, bound_vars))
        elif isinstance(expr, ArrayLiteralExpr):
            for elem in expr.elements:
                free_vars.update(self.collect_free_variables(elem, bound_vars))
        elif isinstance(expr, Block):
            for stmt in expr.stmts:
                if isinstance(stmt, ExprStmt):
                    free_vars.update(self.collect_free_variables(stmt.expr, bound_vars))
                elif isinstance(stmt, LetStmt):
                    if stmt.init:
                        free_vars.update(self.collect_free_variables(stmt.init, bound_vars))
                    bound_vars = bound_vars | {stmt.name}
                elif isinstance(stmt, AssignStmt):
                    free_vars.update(self.collect_free_variables(stmt.value, bound_vars))
                elif isinstance(stmt, ReturnStmt):
                    if stmt.value:
                        free_vars.update(self.collect_free_variables(stmt.value, bound_vars))
                else:
                    # Raw expression (implicit return)
                    free_vars.update(self.collect_free_variables(stmt, bound_vars))
        elif isinstance(expr, MatchExpr):
            free_vars.update(self.collect_free_variables(expr.scrutinee, bound_vars))
            for arm in expr.arms:
                arm_bound = bound_vars.copy()
                if isinstance(arm.pattern, VariantPattern) and arm.pattern.bindings:
                    arm_bound.update(arm.pattern.bindings)
                free_vars.update(self.collect_free_variables(arm.body, arm_bound))

        return free_vars

    def parse_lambda_expr(self) -> ClosureExpr:
        """Parse lambda expression: lambda(params) -> type { body }"""
        self.expect(TokenType.LAMBDA)
        self.expect(TokenType.LPAREN)

        # Parse parameters (all have type annotations)
        params = []
        while not self.match(TokenType.RPAREN):
            name = self.expect(TokenType.IDENT).value
            self.expect(TokenType.COLON)
            param_type = self.parse_type()
            params.append(Param(name, param_type))

            if self.match(TokenType.COMMA):
                self.advance()

        self.expect(TokenType.RPAREN)

        # Parse return type annotation
        ret_type = None
        if self.match(TokenType.ARROW):
            self.advance()
            ret_type = self.parse_type()

        # Parse body (block or expression)
        if self.match(TokenType.LBRACE):
            body = self.parse_block()
        else:
            body = self.parse_expr()

        # Infer captures from free variables in body
        param_names = {p.name for p in params}
        free_vars = self.collect_free_variables(body, param_names)
        captures = sorted(free_vars)  # Sort for deterministic output

        return ClosureExpr(params, ret_type, captures, body)

    def parse_pattern(self) -> Pattern:
        tok = self.peek()
        if tok.type == TokenType.IDENT:
            # Variant pattern or wildcard
            name = self.advance().value
            if name == "_":
                return WildcardPattern()
            # Check for tagged union field bindings: VariantName(x, y, ...)
            bindings = None
            if self.match(TokenType.LPAREN):
                self.advance()
                bindings = []
                while not self.match(TokenType.RPAREN):
                    binding_name = self.expect(TokenType.IDENT).value
                    bindings.append(binding_name)
                    if self.match(TokenType.COMMA):
                        self.advance()
                self.expect(TokenType.RPAREN)
            return VariantPattern(name, bindings)
        elif tok.type == TokenType.NUMBER:
            # Literal pattern
            val = self.advance().value
            return LiteralPattern(IntLiteral(val))
        elif tok.type == TokenType.TRUE:
            self.advance()
            return LiteralPattern(BoolLiteral(True))
        elif tok.type == TokenType.FALSE:
            self.advance()
            return LiteralPattern(BoolLiteral(False))
        else:
            source_range = SourceRange.at(tok.line, tok.column)
            raise ParseError(f"Expected pattern, got {tok.type.name}", source_range)

    def parse_params(self, allow_type_vars: bool = False) -> List[Param]:
        params, _ = self.parse_params_variadic(allow_type_vars)
        return params

    def parse_params_variadic(self, allow_type_vars: bool = False) -> (List[Param], bool):
        params = []
        variadic = False
        if self.match(TokenType.RPAREN):
            return params, variadic
        while True:
            if self.match(TokenType.ELLIPSIS):
                self.advance()
                variadic = True
                break
            name = self.expect(TokenType.IDENT).value
            self.expect(TokenType.COLON)
            ptype = self.parse_type(allow_type_vars)
            params.append(Param(name, ptype))
            if self.match(TokenType.COMMA):
                self.advance()
            else:
                break
        return params, variadic

    def parse_block(self) -> Block:
        """Parse block with implicit returns (last expr without semicolon)."""
        self.expect(TokenType.LBRACE)
        stmts = []
        while not self.match(TokenType.RBRACE):
            if self.match(TokenType.LET):
                stmts.append(self.parse_let_stmt())
            elif self.match(TokenType.IF):
                stmts.append(self.parse_if_stmt())
            elif self.match(TokenType.WHILE):
                stmts.append(self.parse_while_stmt())
            elif self.match(TokenType.FOR):
                stmts.append(self.parse_for_stmt())
            elif self.match(TokenType.RETURN):
                stmts.append(self.parse_return_stmt())
            elif self.match(TokenType.BREAK):
                self.advance()
                self.expect(TokenType.SEMICOLON)
                stmts.append(BreakStmt())
            elif self.match(TokenType.CONTINUE):
                self.advance()
                self.expect(TokenType.SEMICOLON)
                stmts.append(ContinueStmt())
            else:
                # Expression statement - might be implicit return if last
                expr = self.parse_expr()
                if self.match(TokenType.SEMICOLON):
                    # Explicit semicolon - this is a statement
                    self.advance()
                    stmts.append(ExprStmt(expr))
                elif self.match(TokenType.RBRACE):
                    # No semicolon before closing brace - implicit return expression
                    stmts.append(expr)
                    break
                else:
                    # No semicolon and not at closing brace - error
                    self.expect(TokenType.SEMICOLON)
        self.expect(TokenType.RBRACE)
        return Block(stmts)

    def parse_stmt(self) -> Any:
        if self.match(TokenType.LET):
            return self.parse_let_stmt()
        elif self.match(TokenType.IF):
            return self.parse_if_stmt()
        elif self.match(TokenType.WHILE):
            return self.parse_while_stmt()
        elif self.match(TokenType.FOR):
            return self.parse_for_stmt()
        elif self.match(TokenType.RETURN):
            return self.parse_return_stmt()
        elif self.match(TokenType.BREAK):
            self.advance()
            self.expect(TokenType.SEMICOLON)
            return BreakStmt()
        elif self.match(TokenType.CONTINUE):
            self.advance()
            self.expect(TokenType.SEMICOLON)
            return ContinueStmt()
        else:
            return self.parse_expr_or_assign_stmt()

    def parse_let_stmt(self) -> LetStmt:
        start_tok = self.peek()
        self.expect(TokenType.LET)
        name = self.expect(TokenType.IDENT).value
        typ: Optional[JType] = None
        if self.match(TokenType.COLON):
            self.advance()
            typ = self.parse_type()
        self.expect(TokenType.EQ)
        init = self.parse_expr()
        end_tok = self.peek()
        self.expect(TokenType.SEMICOLON)
        location = self.make_location(start_tok, end_tok)
        return LetStmt(name, typ, init, location=location)

    def parse_if_stmt(self) -> IfStmt:
        start_tok = self.peek()
        self.expect(TokenType.IF)
        cond = self.parse_expr()
        then_block = self.parse_block()
        else_block = None
        if self.match(TokenType.ELSE):
            self.advance()
            if self.match(TokenType.IF):
                # else-if chaining: wrap the nested if in a Block
                else_block = Block([self.parse_if_stmt()])
            else:
                else_block = self.parse_block()
        location = self.make_location(start_tok)
        return IfStmt(cond, then_block, else_block, location=location)

    def parse_while_stmt(self) -> WhileStmt:
        self.expect(TokenType.WHILE)
        cond = self.parse_expr()
        body = self.parse_block()
        return WhileStmt(cond, body)

    def parse_for_stmt(self) -> ForStmt:
        self.expect(TokenType.FOR)
        self.expect(TokenType.LPAREN)
        init = self.parse_for_init()
        cond = self.parse_expr()
        self.expect(TokenType.SEMICOLON)
        step = self.parse_for_step()
        self.expect(TokenType.RPAREN)
        body = self.parse_block()
        return ForStmt(init, cond, step, body)

    def parse_for_init(self) -> Any:
        if self.match(TokenType.LET):
            self.expect(TokenType.LET)
            name = self.expect(TokenType.IDENT).value
            typ = None
            if self.match(TokenType.COLON):
                self.advance()
                typ = self.parse_type()
            self.expect(TokenType.EQ)
            init = self.parse_expr()
            self.expect(TokenType.SEMICOLON)
            return LetStmt(name, typ, init)
        elif self.match(TokenType.SEMICOLON):
            self.advance()
            return ExprStmt(IntLiteral(0))
        else:
            expr = self.parse_expr()
            self.expect(TokenType.SEMICOLON)
            return ExprStmt(expr)

    def parse_for_step(self) -> Any:
        if self.match(TokenType.RPAREN):
            return IntLiteral(0)
        expr = self.parse_expr()
        if self.match(TokenType.EQ):
            self.advance()
            rhs = self.parse_expr()
            return AssignStmt(expr, rhs)
        return expr

    def parse_return_stmt(self) -> ReturnStmt:
        self.expect(TokenType.RETURN)
        val = None
        if not self.match(TokenType.SEMICOLON):
            val = self.parse_expr()
        self.expect(TokenType.SEMICOLON)
        return ReturnStmt(val)

    def parse_expr_or_assign_stmt(self) -> Any:
        expr = self.parse_expr()
        if self.match(TokenType.EQ):
            self.advance()
            rhs = self.parse_expr()
            self.expect(TokenType.SEMICOLON)
            return AssignStmt(expr, rhs)
        # Match expressions consume their own braces, so no semicolon needed
        if not isinstance(expr, MatchExpr):
            self.expect(TokenType.SEMICOLON)
        return ExprStmt(expr)

    # ---- Expression parsing with precedence climbing ----
    # Precedence (low to high):
    #   || → && → | → ^ → & → == → < → << → + → * → unary

    def parse_expr(self) -> Any:
        return self.parse_or()

    def parse_or(self) -> Any:
        left = self.parse_and()
        while self.match(TokenType.OROR):
            op = self.advance().value
            right = self.parse_and()
            left = BinaryOp(op, left, right)
        return left

    def parse_and(self) -> Any:
        left = self.parse_bitor()
        while self.match(TokenType.ANDAND):
            op = self.advance().value
            right = self.parse_bitor()
            left = BinaryOp(op, left, right)
        return left

    def parse_bitor(self) -> Any:
        left = self.parse_bitxor()
        while self.match(TokenType.PIPE):
            op = self.advance().value
            right = self.parse_bitxor()
            left = BinaryOp(op, left, right)
        return left

    def parse_bitxor(self) -> Any:
        left = self.parse_bitand()
        while self.match(TokenType.CARET):
            op = self.advance().value
            right = self.parse_bitand()
            left = BinaryOp(op, left, right)
        return left

    def parse_bitand(self) -> Any:
        left = self.parse_eq()
        while self.match(TokenType.AMPERSAND):
            op = self.advance().value
            right = self.parse_eq()
            left = BinaryOp(op, left, right)
        return left

    def parse_eq(self) -> Any:
        left = self.parse_rel()
        while self.match(TokenType.EQEQ, TokenType.NEQ):
            op = self.advance().value
            right = self.parse_rel()
            left = BinaryOp(op, left, right)
        return left

    def parse_rel(self) -> Any:
        left = self.parse_shift()
        while self.match(TokenType.LT, TokenType.GT, TokenType.LE, TokenType.GE):
            op = self.advance().value
            right = self.parse_shift()
            left = BinaryOp(op, left, right)
        return left

    def parse_shift(self) -> Any:
        left = self.parse_add()
        while self.match(TokenType.LSHIFT, TokenType.RSHIFT):
            op = self.advance().value
            right = self.parse_add()
            left = BinaryOp(op, left, right)
        return left

    def parse_add(self) -> Any:
        left = self.parse_mul()
        while self.match(TokenType.PLUS, TokenType.MINUS):
            op = self.advance().value
            right = self.parse_mul()
            left = BinaryOp(op, left, right)
        return left

    def parse_mul(self) -> Any:
        left = self.parse_unary()
        while self.match(TokenType.STAR, TokenType.SLASH, TokenType.PERCENT):
            op = self.advance().value
            right = self.parse_unary()
            left = BinaryOp(op, left, right)
        return left

    def parse_unary(self) -> Any:
        if self.match(TokenType.MINUS, TokenType.BANG, TokenType.STAR,
                      TokenType.AMPERSAND, TokenType.TILDE):
            op = self.advance().value
            operand = self.parse_unary()
            if op == "*":
                return DerefExpr(operand)
            if op == "&":
                return AddrOfExpr(operand)
            return UnaryOp(op, operand)
        return self.parse_postfix()

    def parse_postfix(self) -> Any:
        node = self.parse_primary()
        while True:
            if self.match(TokenType.LPAREN):
                self.advance()
                args = self.parse_args()
                self.expect(TokenType.RPAREN)
                if isinstance(node, VarRef):
                    node = CallExpr(node.name, args)
                else:
                    tok = self.peek()
                    source_range = SourceRange.at(tok.line, tok.column)
                    raise ParseError("Cannot call non-identifier", source_range)
            elif self.match(TokenType.LBRACKET):
                self.advance()
                idx = self.parse_expr()
                self.expect(TokenType.RBRACKET)
                node = IndexExpr(node, idx)
            elif self.match(TokenType.DOT):
                self.advance()
                field_name = self.expect(TokenType.IDENT).value
                node = FieldAccessExpr(node, field_name)
            else:
                break
        return node

    def parse_args(self) -> List[Any]:
        args = []
        if self.match(TokenType.RPAREN):
            return args
        while True:
            args.append(self.parse_expr())
            if self.match(TokenType.COMMA):
                self.advance()
            else:
                break
        return args

    def parse_primary(self) -> Any:
        tok = self.peek()
        if tok.type == TokenType.NUMBER:
            self.advance()
            return IntLiteral(tok.value)
        elif tok.type == TokenType.STRING:
            self.advance()
            return StringLiteral(tok.value)
        elif tok.type == TokenType.TRUE:
            self.advance()
            return BoolLiteral(True)
        elif tok.type == TokenType.FALSE:
            self.advance()
            return BoolLiteral(False)
        elif tok.type == TokenType.NULL:
            self.advance()
            return NullLiteral()
        elif tok.type == TokenType.IDENT:
            self.advance()
            return VarRef(tok.value)
        elif tok.type == TokenType.MATCH:
            # Match expression: match expr { pattern => body, ... }
            return self.parse_match_expr()
        elif tok.type == TokenType.LBRACKET:
            # Array literal: [1, 2, 3]
            self.advance()
            elements = []
            if not self.match(TokenType.RBRACKET):
                while True:
                    elements.append(self.parse_expr())
                    if self.match(TokenType.COMMA):
                        self.advance()
                    else:
                        break
            self.expect(TokenType.RBRACKET)
            return ArrayLiteralExpr(elements)
        elif tok.type == TokenType.LPAREN:
            self.advance()
            # Check for cast: ( type ) expr
            saved_pos = self.pos
            try:
                typ = self.parse_type()
                if self.match(TokenType.RPAREN):
                    self.advance()
                    operand = self.parse_unary()
                    return CastExpr(typ, operand)
            except ParseError:
                pass
            self.pos = saved_pos
            expr = self.parse_expr()
            self.expect(TokenType.RPAREN)
            return expr
        elif tok.type == TokenType.PIPE:
            # Closure expression: |params| body
            return self.parse_closure_expr()
        elif tok.type == TokenType.LAMBDA:
            # Lambda expression: lambda(params) -> type { body }
            return self.parse_lambda_expr()
        else:
            source_range = SourceRange.at(tok.line, tok.column)
            raise ParseError(f"Unexpected token {tok.type.name} in expression", source_range)

def parse_source(source: str) -> Program:
    lexer = Lexer(source)
    tokens = lexer.tokenize()
    parser = Parser(tokens)
    return parser.parse()
