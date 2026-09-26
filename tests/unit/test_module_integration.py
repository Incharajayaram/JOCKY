import pytest
from jocky.language.parser import parse_source
from jocky.language.checker import TypeChecker, TypeError
from jocky.core.modules import ModuleRegistry, reset_modules
from jocky.language.ast import ModDecl, UseStmt, FuncDecl


class TestModuleTypeChecking:
    def test_simple_module_declaration(self):
        """Test parsing and type checking a simple module."""
        reset_modules()
        src = """
mod math {
    fn add(a: i32, b: i32) -> i32 {
        return a + b;
    }
}

fn main() -> i32 {
    return 0;
}
"""
        prog = parse_source(src)
        assert any(isinstance(d, ModDecl) for d in prog.decls)

        checker = TypeChecker()
        checker.check(prog)

        # Module should be registered
        assert checker.module_registry.get_module("math") is not None

    def test_use_statement_parsing(self):
        """Test parsing use statements."""
        reset_modules()
        src = """
mod math {
    fn add(a: i32, b: i32) -> i32 {
        return a + b;
    }
}

use math::add;

fn main() -> i32 {
    return 0;
}
"""
        prog = parse_source(src)
        assert any(isinstance(d, UseStmt) for d in prog.decls)

    def test_module_with_use_statement(self):
        """Test module declaration and use statement together."""
        reset_modules()
        src = """
mod math {
    fn add(a: i32, b: i32) -> i32 {
        return a + b;
    }
}

use math::add;

fn main() -> i32 {
    return add(5, 3);
}
"""
        prog = parse_source(src)
        checker = TypeChecker()

        # Should type check without errors
        checker.check(prog)

    def test_module_symbol_registration(self):
        """Test that module symbols are properly registered."""
        reset_modules()
        src = """
mod math {
    fn add(a: i32, b: i32) -> i32 {
        return a + b;
    }

    fn multiply(a: i32, b: i32) -> i32 {
        return a * b;
    }
}

fn main() -> i32 {
    return 0;
}
"""
        prog = parse_source(src)
        checker = TypeChecker()
        checker.check(prog)

        # Check that module and symbols were registered
        math_mod = checker.module_registry.get_module("math")
        assert math_mod is not None
        assert "add" in math_mod.symbols
        assert "multiply" in math_mod.symbols

    def test_use_all_symbols(self):
        """Test importing all symbols with wildcard."""
        reset_modules()
        src = """
mod utils {
    fn helper1() -> i32 { return 1; }
    fn helper2() -> i32 { return 2; }
}

use utils::*;

fn main() -> i32 {
    return helper1();
}
"""
        prog = parse_source(src)
        checker = TypeChecker()
        checker.check(prog)

        # All symbols from utils should be visible
        assert "helper1" in checker.visible_symbols
        assert "helper2" in checker.visible_symbols

    def test_nested_module_paths(self):
        """Test parsing nested module paths in use statements."""
        reset_modules()
        src = """
mod graphics {
    mod drawing {
        fn draw_line() -> i32 { return 0; }
    }
}

fn main() -> i32 {
    return 0;
}
"""
        prog = parse_source(src)
        assert len(prog.decls) >= 1

        # Should parse without errors
        checker = TypeChecker()
        checker.check(prog)

    def test_multiple_modules(self):
        """Test multiple module declarations."""
        reset_modules()
        src = """
mod math {
    fn add(a: i32, b: i32) -> i32 {
        return a + b;
    }
}

mod utils {
    fn print_num(x: i32) -> void { }
}

fn main() -> i32 {
    return 0;
}
"""
        prog = parse_source(src)
        checker = TypeChecker()
        checker.check(prog)

        # Both modules should be registered
        assert checker.module_registry.get_module("math") is not None
        assert checker.module_registry.get_module("utils") is not None

    def test_use_specific_symbol(self):
        """Test importing a specific symbol."""
        reset_modules()
        src = """
mod math {
    fn add(a: i32, b: i32) -> i32 {
        return a + b;
    }

    fn multiply(a: i32, b: i32) -> i32 {
        return a * b;
    }
}

use math::add;

fn main() -> i32 {
    return add(2, 3);
}
"""
        prog = parse_source(src)
        checker = TypeChecker()
        checker.check(prog)

        # Only 'add' should be imported
        assert "add" in checker.visible_symbols
        assert "multiply" not in checker.visible_symbols

    def test_module_with_struct(self):
        """Test module containing struct definitions."""
        reset_modules()
        src = """
mod geometry {
    struct Point {
        x: i32;
        y: i32
    };
}

fn main() -> i32 {
    return 0;
}
"""
        prog = parse_source(src)
        checker = TypeChecker()
        checker.check(prog)

        # Struct should be registered in module
        geom_mod = checker.module_registry.get_module("geometry")
        assert geom_mod is not None

    def test_module_with_enum(self):
        """Test module containing enum definitions."""
        reset_modules()
        src = """
mod types {
    enum Status { OK = 0, Error = 1 };
}

fn main() -> i32 {
    return 0;
}
"""
        prog = parse_source(src)
        checker = TypeChecker()
        checker.check(prog)

        # Enum should be registered in module
        types_mod = checker.module_registry.get_module("types")
        assert types_mod is not None


class TestModuleErrors:
    def test_use_nonexistent_module_error(self):
        """Test error when importing from non-existent module."""
        reset_modules()
        src = """
use nonexistent::foo;

fn main() -> i32 {
    return 0;
}
"""
        prog = parse_source(src)
        checker = TypeChecker()

        with pytest.raises(TypeError) as exc_info:
            checker.check(prog)

        assert "Cannot find module" in str(exc_info.value)

    def test_use_nonexistent_symbol_error(self):
        """Test error when importing non-existent symbol."""
        reset_modules()
        src = """
mod math {
    fn add(a: i32, b: i32) -> i32 {
        return a + b;
    }
}

use math::nonexistent;

fn main() -> i32 {
    return 0;
}
"""
        prog = parse_source(src)
        checker = TypeChecker()

        with pytest.raises(TypeError) as exc_info:
            checker.check(prog)

        assert "Cannot find symbol" in str(exc_info.value)


class TestModuleIntegration:
    def test_multi_module_compilation(self):
        """Test compiling a program with multiple modules."""
        reset_modules()
        src = """
mod math {
    fn add(a: i32, b: i32) -> i32 {
        return a + b;
    }
}

mod utils {
    fn double(x: i32) -> i32 {
        return x + x;
    }
}

use math::add;
use utils::double;

fn main() -> i32 {
    let result: i32 = add(5, 3);
    return double(result);
}
"""
        prog = parse_source(src)
        checker = TypeChecker()

        # Should type check successfully
        checker.check(prog)

    def test_module_registry_persistence(self):
        """Test that modules persist in registry across type checks."""
        reset_modules()
        src1 = """
mod math {
    fn add(a: i32, b: i32) -> i32 {
        return a + b;
    }
}

fn main() -> i32 {
    return 0;
}
"""
        prog1 = parse_source(src1)
        checker1 = TypeChecker()
        checker1.check(prog1)

        # Create another checker with the same registry
        registry = checker1.module_registry
        src2 = """
use math::add;

fn main() -> i32 {
    return add(1, 2);
}
"""
        prog2 = parse_source(src2)
        checker2 = TypeChecker(module_registry=registry)

        # Should find math module from first type check
        math_mod = checker2.module_registry.get_module("math")
        assert math_mod is not None


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
