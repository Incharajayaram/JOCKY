import pytest
from jocky.language.parser import parse_source, ParseError
from jocky.language.ast import (
    FuncDecl, StructDef, EnumDef, FFIDecl, Attribute, JType
)
from jocky.language.checker import TypeChecker, TypeError as JockyTypeError


class TestAttributesParser:
    def test_parse_function_with_inline_attribute(self):
        src = '''#[inline]
fn add(a: i32, b: i32) -> i32 {
    return a + b;
}
'''
        prog = parse_source(src)
        assert len(prog.decls) == 1
        func = prog.decls[0]
        assert isinstance(func, FuncDecl)
        assert func.name == "add"
        assert len(func.attributes) == 1
        assert func.attributes[0].name == "inline"
        assert func.attributes[0].args == []

    def test_parse_function_with_inline_never_attribute(self):
        src = '''#[inline(never)]
fn foo() -> i32 {
    return 0;
}
'''
        prog = parse_source(src)
        func = prog.decls[0]
        assert len(func.attributes) == 1
        assert func.attributes[0].name == "inline"
        assert func.attributes[0].args == ["never"]

    def test_parse_function_with_inline_always_attribute(self):
        src = '''#[inline(always)]
fn bar() -> i32 {
    return 42;
}
'''
        prog = parse_source(src)
        func = prog.decls[0]
        assert len(func.attributes) == 1
        assert func.attributes[0].name == "inline"
        assert func.attributes[0].args == ["always"]

    def test_parse_function_with_no_mangle_attribute(self):
        src = '''#[no_mangle]
fn exported_func() -> i32 {
    return 0;
}
'''
        prog = parse_source(src)
        func = prog.decls[0]
        assert len(func.attributes) == 1
        assert func.attributes[0].name == "no_mangle"
        assert func.attributes[0].args == []

    def test_parse_function_with_multiple_attributes(self):
        src = '''#[inline]
#[no_mangle]
#[cold]
fn cold_exported() -> i32 {
    return 0;
}
'''
        prog = parse_source(src)
        func = prog.decls[0]
        assert len(func.attributes) == 3
        assert func.attributes[0].name == "inline"
        assert func.attributes[1].name == "no_mangle"
        assert func.attributes[2].name == "cold"

    def test_parse_function_with_obfuscate_attribute(self):
        src = '''#[obfuscate(aggressive)]
fn sensitive_func() -> i32 {
    return 0;
}
'''
        prog = parse_source(src)
        func = prog.decls[0]
        assert len(func.attributes) == 1
        assert func.attributes[0].name == "obfuscate"
        assert func.attributes[0].args == ["aggressive"]

    def test_parse_struct_with_packed_attribute(self):
        src = '''#[packed]
struct Point {
    x: i32;
    y: i32;
};
'''
        prog = parse_source(src)
        struct = prog.decls[0]
        assert isinstance(struct, StructDef)
        assert struct.name == "Point"
        assert len(struct.attributes) == 1
        assert struct.attributes[0].name == "packed"

    def test_parse_struct_with_repr_attribute(self):
        src = '''#[repr(u32)]
struct Status {
    code: i32;
};
'''
        prog = parse_source(src)
        struct = prog.decls[0]
        assert len(struct.attributes) == 1
        assert struct.attributes[0].name == "repr"
        assert struct.attributes[0].args == ["u32"]

    def test_parse_enum_with_repr_attribute(self):
        src = '''#[repr(i32)]
enum Color {
    Red = 0,
    Green = 1,
    Blue = 2,
};
'''
        prog = parse_source(src)
        enum = prog.decls[0]
        assert isinstance(enum, EnumDef)
        assert enum.name == "Color"
        assert len(enum.attributes) == 1
        assert enum.attributes[0].name == "repr"
        assert enum.attributes[0].args == ["i32"]

    def test_parse_ffi_with_no_mangle_attribute(self):
        src = '''#[no_mangle]
ffi printf(fmt: string) -> i32;
'''
        prog = parse_source(src)
        ffi = prog.decls[0]
        assert isinstance(ffi, FFIDecl)
        assert ffi.name == "printf"
        assert len(ffi.attributes) == 1
        assert ffi.attributes[0].name == "no_mangle"

    def test_parse_function_without_attributes(self):
        src = '''fn simple() -> i32 {
    return 0;
}
'''
        prog = parse_source(src)
        func = prog.decls[0]
        assert len(func.attributes) == 0

    def test_parse_struct_without_attributes(self):
        src = '''struct Data {
    value: i32;
};
'''
        prog = parse_source(src)
        struct = prog.decls[0]
        assert len(struct.attributes) == 0

    def test_parse_attribute_with_string_argument(self):
        src = '''#[test("foo")]
fn annotated() -> i32 {
    return 0;
}
'''
        prog = parse_source(src)
        func = prog.decls[0]
        assert len(func.attributes) == 1
        assert func.attributes[0].args == ["foo"]

    def test_parse_attribute_with_numeric_argument(self):
        src = '''#[packed(8)]
struct Aligned {
    x: i32;
};
'''
        prog = parse_source(src)
        struct = prog.decls[0]
        assert len(struct.attributes) == 1
        assert struct.attributes[0].args == [8]


class TestAttributesChecker:
    def test_validate_inline_attribute_on_function(self):
        src = '''#[inline]
fn add(a: i32, b: i32) -> i32 {
    return a + b;
}
'''
        prog = parse_source(src)
        checker = TypeChecker()
        checker.check(prog)

    def test_validate_invalid_attribute_on_function(self):
        src = '''#[packed]
fn invalid() -> i32 {
    return 0;
}
'''
        prog = parse_source(src)
        checker = TypeChecker()
        with pytest.raises(JockyTypeError):
            checker.check(prog)

    def test_validate_packed_attribute_on_struct(self):
        src = '''#[packed]
struct Data {
    x: i32;
};
'''
        prog = parse_source(src)
        checker = TypeChecker()
        checker.check(prog)

    def test_validate_invalid_attribute_on_struct(self):
        src = '''#[inline]
struct Data {
    x: i32;
};
'''
        prog = parse_source(src)
        checker = TypeChecker()
        with pytest.raises(JockyTypeError):
            checker.check(prog)

    def test_validate_inline_never_argument(self):
        src = '''#[inline(never)]
fn foo() -> i32 {
    return 0;
}
'''
        prog = parse_source(src)
        checker = TypeChecker()
        checker.check(prog)

    def test_validate_inline_always_argument(self):
        src = '''#[inline(always)]
fn foo() -> i32 {
    return 0;
}
'''
        prog = parse_source(src)
        checker = TypeChecker()
        checker.check(prog)

    def test_validate_invalid_inline_argument(self):
        src = '''#[inline(maybe)]
fn foo() -> i32 {
    return 0;
}
'''
        prog = parse_source(src)
        checker = TypeChecker()
        with pytest.raises(JockyTypeError):
            checker.check(prog)

    def test_validate_obfuscate_standard(self):
        src = '''#[obfuscate(standard)]
fn secret() -> i32 {
    return 0;
}
'''
        prog = parse_source(src)
        checker = TypeChecker()
        checker.check(prog)

    def test_validate_obfuscate_aggressive(self):
        src = '''#[obfuscate(aggressive)]
fn secret() -> i32 {
    return 0;
}
'''
        prog = parse_source(src)
        checker = TypeChecker()
        checker.check(prog)

    def test_validate_invalid_obfuscate_argument(self):
        src = '''#[obfuscate(maximum)]
fn secret() -> i32 {
    return 0;
}
'''
        prog = parse_source(src)
        checker = TypeChecker()
        with pytest.raises(JockyTypeError):
            checker.check(prog)

    def test_validate_repr_on_enum(self):
        src = '''#[repr(i32)]
enum Status {
    OK = 0,
    Error = 1,
};
'''
        prog = parse_source(src)
        checker = TypeChecker()
        checker.check(prog)

    def test_validate_packed_with_alignment(self):
        src = '''#[packed(4)]
struct Data {
    x: i32;
};
'''
        prog = parse_source(src)
        checker = TypeChecker()
        checker.check(prog)

    def test_validate_invalid_packed_alignment(self):
        src = '''#[packed(invalid)]
struct Data {
    x: i32;
};
'''
        prog = parse_source(src)
        checker = TypeChecker()
        with pytest.raises(JockyTypeError):
            checker.check(prog)

    def test_validate_no_mangle_on_ffi(self):
        src = '''#[no_mangle]
ffi malloc(size: i64) -> i32;
'''
        prog = parse_source(src)
        checker = TypeChecker()
        checker.check(prog)

    def test_validate_invalid_attribute_on_ffi(self):
        src = '''#[cold]
ffi puts(s: string) -> i32;
'''
        prog = parse_source(src)
        checker = TypeChecker()
        with pytest.raises(JockyTypeError):
            checker.check(prog)

    def test_validate_cold_attribute_on_function(self):
        src = '''#[cold]
fn error_handler() -> i32 {
    return -1;
}
'''
        prog = parse_source(src)
        checker = TypeChecker()
        checker.check(prog)

    def test_validate_hot_attribute_on_function(self):
        src = '''#[hot]
fn inner_loop() -> i32 {
    return 0;
}
'''
        prog = parse_source(src)
        checker = TypeChecker()
        checker.check(prog)


class TestAttributesCodeGen:
    def test_codegen_function_with_inline_attribute(self):
        from jocky.language.codegen import CodeGen

        src = '''#[inline]
fn add(a: i32, b: i32) -> i32 {
    return a + b;
}
'''
        prog = parse_source(src)
        checker = TypeChecker()
        checker.check(prog)

        codegen = CodeGen()
        output = codegen.gen(prog)
        assert "attributes: [inline]" in output
        assert "@add" in output

    def test_codegen_function_with_multiple_attributes(self):
        from jocky.language.codegen import CodeGen

        src = '''#[inline(never)]
#[cold]
fn rare_path() -> i32 {
    return 0;
}
'''
        prog = parse_source(src)
        checker = TypeChecker()
        checker.check(prog)

        codegen = CodeGen()
        output = codegen.gen(prog)
        assert "attributes: [inline(never), cold]" in output

    def test_codegen_struct_with_packed_attribute(self):
        from jocky.language.codegen import CodeGen

        src = '''#[packed]
struct Compact {
    x: i32;
    y: i32;
};
'''
        prog = parse_source(src)
        checker = TypeChecker()
        checker.check(prog)

        codegen = CodeGen()
        output = codegen.gen(prog)
        assert "struct attributes: [packed]" in output
        assert "%Compact = type" in output

    def test_codegen_function_without_attributes(self):
        from jocky.language.codegen import CodeGen

        src = '''fn simple() -> i32 {
    return 42;
}
'''
        prog = parse_source(src)
        checker = TypeChecker()
        checker.check(prog)

        codegen = CodeGen()
        output = codegen.gen(prog)
        assert "attributes:" not in output
        assert "@simple" in output


class TestAttributesIntegration:
    def test_attributes_with_function_call(self):
        src = '''#[hot]
fn frequently_called(x: i32) -> i32 {
    return x * 2;
}

fn main() -> i32 {
    return frequently_called(10);
}
'''
        prog = parse_source(src)
        checker = TypeChecker()
        checker.check(prog)

    def test_multiple_functions_with_different_attributes(self):
        src = '''#[inline]
fn fast() -> i32 {
    return 1;
}

#[cold]
fn error() -> i32 {
    return -1;
}

#[obfuscate(standard)]
fn secret() -> i32 {
    return 42;
}

fn main() -> i32 {
    return fast() + error() + secret();
}
'''
        prog = parse_source(src)
        checker = TypeChecker()
        checker.check(prog)

    def test_struct_and_enum_with_attributes(self):
        src = '''#[repr(u32)]
enum Status {
    OK = 0,
    Error = 1,
};

#[packed]
struct Result {
    code: Status;
    value: i32;
};

fn get_status() -> Status {
    let code: Status = OK;
    return code;
}
'''
        prog = parse_source(src)
        checker = TypeChecker()
        checker.check(prog)
