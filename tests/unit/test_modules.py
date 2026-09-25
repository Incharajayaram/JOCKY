import pytest
import tempfile
from pathlib import Path
from jocky.core.modules import (
    Module, Symbol, ModuleRegistry, ModuleLoader,
    get_registry, get_loader, reset_modules
)
from jocky.language.ast import ModulePath, UseStmt


class TestModulePath:
    def test_module_path_creation(self):
        path = ModulePath(["math", "algebra", "solve"])
        assert path.components == ["math", "algebra", "solve"]
        assert str(path) == "math::algebra::solve"

    def test_module_path_from_string(self):
        path = ModulePath.from_string("foo::bar::baz")
        assert path.components == ["foo", "bar", "baz"]

    def test_module_path_single_component(self):
        path = ModulePath(["math"])
        assert str(path) == "math"


class TestUseStmt:
    def test_use_stmt_specific_symbol(self):
        path = ModulePath(["math", "algebra", "solve"])
        use = UseStmt(path, all=False)

        assert use.symbol == "solve"
        assert use.module_path.components == ["math", "algebra"]
        assert use.all is False

    def test_use_stmt_wildcard(self):
        path = ModulePath(["math", "algebra"])
        use = UseStmt(path, all=True)

        assert use.symbol is None
        assert use.module_path == path
        assert use.all is True

    def test_use_stmt_root_level(self):
        path = ModulePath(["foo"])
        use = UseStmt(path, all=False)

        assert use.symbol == "foo"


class TestSymbol:
    def test_symbol_creation(self):
        sym = Symbol("add", type_info="fn(i32, i32) -> i32", public=True)
        assert sym.name == "add"
        assert sym.public is True

    def test_symbol_private(self):
        sym = Symbol("helper", type_info="fn() -> void", public=False)
        assert sym.public is False


class TestModule:
    def test_module_creation(self):
        mod = Module("math")
        assert mod.name == "math"
        assert len(mod.symbols) == 0

    def test_add_symbol(self):
        mod = Module("math")
        mod.add_symbol("add", "fn(i32, i32) -> i32", public=True)

        assert "add" in mod.symbols
        assert mod.get_symbol("add") is not None
        assert mod.get_symbol("add").name == "add"

    def test_symbol_visibility(self):
        mod = Module("math")
        mod.add_symbol("public_fn", "fn() -> void", public=True)
        mod.add_symbol("private_fn", "fn() -> void", public=False)

        pub = mod.get_symbol("public_fn")
        priv = mod.get_symbol("private_fn")

        assert pub.public is True
        assert priv.public is False

    def test_add_submodule(self):
        parent = Module("graphics")
        child = Module("2d")

        parent.add_submodule("2d", child)

        assert "2d" in parent.submodules
        assert child.parent is parent

    def test_add_import(self):
        mod = Module("main")
        imported = Module("math")

        mod.add_import("math", imported)

        assert "math" in mod.imports
        assert mod.imports["math"] is imported

    def test_get_symbol_recursive_direct(self):
        mod = Module("math")
        mod.add_symbol("add", "fn(i32, i32) -> i32", public=True)

        sym, path = mod.get_symbol_recursive(["add"])
        assert sym is not None
        assert sym.name == "add"

    def test_get_symbol_recursive_submodule(self):
        parent = Module("graphics")
        child = Module("2d")
        child.add_symbol("draw_line", "fn() -> void", public=True)

        parent.add_submodule("2d", child)

        sym, path = parent.get_symbol_recursive(["2d", "draw_line"])
        assert sym is not None
        assert sym.name == "draw_line"

    def test_get_symbol_recursive_not_found(self):
        mod = Module("math")
        mod.add_symbol("add", "fn() -> void")

        sym, path = mod.get_symbol_recursive(["subtract"])
        assert sym is None

    def test_module_string_representation(self):
        mod = Module("utils")
        assert str(mod) == "Module(utils)"


class TestModuleRegistry:
    def test_registry_creation(self):
        registry = ModuleRegistry()
        assert len(registry.modules) == 0

    def test_register_module(self):
        registry = ModuleRegistry()
        mod = Module("math")

        registry.register_module(mod)

        assert registry.get_module("math") is mod

    def test_register_multiple_modules(self):
        registry = ModuleRegistry()
        mod1 = Module("math")
        mod2 = Module("utils")

        registry.register_module(mod1)
        registry.register_module(mod2)

        assert len(registry.modules) == 2
        assert registry.get_module("math") is mod1
        assert registry.get_module("utils") is mod2

    def test_list_modules(self):
        registry = ModuleRegistry()
        mod1 = Module("math")
        mod2 = Module("utils")

        registry.register_module(mod1)
        registry.register_module(mod2)

        modules = registry.list_modules()
        assert len(modules) == 2
        assert mod1 in modules
        assert mod2 in modules

    def test_get_module_from_file(self):
        registry = ModuleRegistry()
        mod = Module("math")
        path = Path("math.jky")

        registry.register_module(mod, path)

        assert registry.get_module_from_file(path) is mod


class TestModuleLoader:
    @pytest.fixture
    def temp_project(self):
        with tempfile.TemporaryDirectory() as tmpdir:
            base = Path(tmpdir)

            # Create math.jky
            (base / "math.jky").write_text("fn add(a: i32, b: i32) -> i32 { return a + b; }")

            # Create utils.jky
            (base / "utils.jky").write_text("fn helper() { }")

            # Create math/algebra/ directory module
            (base / "math").mkdir()
            (base / "math" / "mod.jky").write_text("// Algebra module")
            (base / "math" / "algebra.jky").write_text("fn solve() { }")

            yield base

    def test_loader_creation(self, temp_project):
        loader = ModuleLoader(temp_project)
        assert loader.base_dir == temp_project

    def test_discover_modules(self, temp_project):
        loader = ModuleLoader(temp_project)
        modules = loader.discover_modules()

        # Should discover math.jky and utils.jky
        assert len(modules) >= 2

        module_names = list(modules.values())
        assert "math" in module_names
        assert "utils" in module_names

    def test_load_file(self, temp_project):
        loader = ModuleLoader(temp_project)
        path = temp_project / "math.jky"

        source = loader.load_file(path)

        assert "add" in source
        assert source == "fn add(a: i32, b: i32) -> i32 { return a + b; }"

    def test_load_file_cached(self, temp_project):
        loader = ModuleLoader(temp_project)
        path = temp_project / "math.jky"

        source1 = loader.load_file(path)
        source2 = loader.load_file(path)

        assert source1 is source2  # Same object (cached)

    def test_compute_checksum(self, temp_project):
        loader = ModuleLoader(temp_project)
        source1 = "fn test() { }"
        source2 = "fn test() { }"
        source3 = "fn different() { }"

        check1 = loader.compute_checksum(source1)
        check2 = loader.compute_checksum(source2)
        check3 = loader.compute_checksum(source3)

        assert check1 == check2
        assert check1 != check3
        assert len(check1) == 64  # SHA256 hex

    def test_create_module_from_file(self, temp_project):
        loader = ModuleLoader(temp_project)
        path = temp_project / "math.jky"

        mod = loader.create_module_from_file(path, "math")

        assert mod.name == "math"
        assert mod.path == path
        assert len(mod.source) > 0
        assert len(mod.checksum) == 64

    def test_load_project(self, temp_project):
        loader = ModuleLoader(temp_project)
        registry = loader.load_project()

        assert registry.get_module("math") is not None
        assert registry.get_module("utils") is not None
        assert len(registry.list_modules()) >= 2

    def test_module_checksum_changes_with_source(self, temp_project):
        loader = ModuleLoader(temp_project)
        path = temp_project / "math.jky"

        mod1 = loader.create_module_from_file(path, "math")
        check1 = mod1.checksum

        # Simulate file change
        reset_modules()
        loader2 = ModuleLoader(temp_project)
        (temp_project / "math.jky").write_text("fn different() { }")
        loader2.loaded_files.clear()

        mod2 = loader2.create_module_from_file(path, "math")
        check2 = mod2.checksum

        assert check1 != check2


class TestGlobalModuleFunctions:
    def test_get_registry_singleton(self):
        reset_modules()
        reg1 = get_registry()
        reg2 = get_registry()
        assert reg1 is reg2

    def test_get_loader_singleton(self):
        reset_modules()
        loader1 = get_loader()
        loader2 = get_loader()
        assert loader1 is loader2

    def test_reset_modules(self):
        reset_modules()
        reg1 = get_registry()

        reset_modules()
        reg2 = get_registry()

        assert reg1 is not reg2


class TestModuleIntegration:
    def test_complex_module_structure(self):
        # Build a complex module hierarchy
        root = Module("project")

        math_mod = Module("math")
        algebra = Module("algebra")
        algebra.add_symbol("solve", "fn(eq: string) -> i32")
        math_mod.add_submodule("algebra", algebra)
        root.add_submodule("math", math_mod)

        utils_mod = Module("utils")
        utils_mod.add_symbol("format", "fn(s: string) -> string")
        root.add_submodule("utils", utils_mod)

        # Test recursive access
        sym, path = root.get_symbol_recursive(["math", "algebra", "solve"])
        assert sym is not None
        assert sym.name == "solve"

        sym, path = root.get_symbol_recursive(["utils", "format"])
        assert sym is not None
        assert sym.name == "format"

    def test_module_with_imports(self):
        main = Module("main")
        math = Module("math")
        math.add_symbol("add", "fn(i32, i32) -> i32")

        main.add_import("math", math)

        assert "math" in main.imports
        assert main.imports["math"].get_symbol("add") is not None


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
