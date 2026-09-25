"""Module system for multi-file compilation."""

from pathlib import Path
from typing import Dict, List, Optional, Any, Tuple
from dataclasses import dataclass, field
import hashlib


@dataclass
class Symbol:
    """Represents a symbol (function, struct, etc.) in a module."""
    name: str
    type_info: Any  # Will be set by type checker
    public: bool = True


@dataclass
class Module:
    """Represents a compiled module."""
    name: str
    path: Optional[Path] = None
    symbols: Dict[str, Symbol] = field(default_factory=dict)
    submodules: Dict[str, 'Module'] = field(default_factory=dict)
    imports: Dict[str, 'Module'] = field(default_factory=dict)
    ast_items: List[Any] = field(default_factory=list)
    source: str = ""
    checksum: str = ""
    parent: Optional['Module'] = None

    def add_symbol(self, name: str, type_info: Any, public: bool = True):
        """Register a symbol in this module."""
        self.symbols[name] = Symbol(name, type_info, public)

    def add_submodule(self, name: str, module: 'Module'):
        """Register a submodule."""
        module.parent = self
        self.submodules[name] = module

    def add_import(self, alias: str, module: 'Module'):
        """Register an imported module with an alias."""
        self.imports[alias] = module

    def get_symbol(self, name: str) -> Optional[Symbol]:
        """Get a symbol from this module."""
        return self.symbols.get(name)

    def get_symbol_recursive(self, path_components: List[str]) -> Optional[Tuple[Symbol, str]]:
        """
        Get a symbol using a path like [module, submodule, symbol].
        Returns (Symbol, full_path) or (None, None).
        """
        if not path_components:
            return None, None

        current = self
        for component in path_components[:-1]:
            if component in current.submodules:
                current = current.submodules[component]
            elif component in current.imports:
                current = current.imports[component]
            else:
                return None, None

        symbol_name = path_components[-1]
        symbol = current.get_symbol(symbol_name)

        if symbol:
            return symbol, f"{current.name}::{symbol_name}"
        return None, None

    def __str__(self):
        return f"Module({self.name})"


class ModuleRegistry:
    """Central registry for all loaded modules."""

    def __init__(self):
        self.modules: Dict[str, Module] = {}
        self.root_module = Module("root")
        self.file_to_module: Dict[Path, Module] = {}

    def register_module(self, module: Module, path: Optional[Path] = None):
        """Register a module in the registry."""
        self.modules[module.name] = module
        if path:
            self.file_to_module[path] = module

    def get_module(self, name: str) -> Optional[Module]:
        """Get a module by name."""
        return self.modules.get(name)

    def get_module_from_file(self, path: Path) -> Optional[Module]:
        """Get a module by its source file path."""
        return self.file_to_module.get(path)

    def list_modules(self) -> List[Module]:
        """List all registered modules."""
        return list(self.modules.values())

    def __str__(self):
        return f"ModuleRegistry({len(self.modules)} modules)"


class ModuleLoader:
    """Loads and resolves modules from files and AST."""

    def __init__(self, base_dir: Path = Path(".")):
        self.base_dir = base_dir
        self.registry = ModuleRegistry()
        self.loaded_files: Dict[Path, str] = {}

    def discover_modules(self) -> Dict[Path, str]:
        """
        Discover all .jky files in the project.
        Returns mapping of file path to module name.
        """
        modules = {}
        for jky_file in self.base_dir.rglob("*.jky"):
            # Skip hidden directories and build directories
            if any(part.startswith(".") for part in jky_file.parts):
                continue
            if ".jocky-build" in str(jky_file):
                continue

            # Determine module name from file path
            rel_path = jky_file.relative_to(self.base_dir)

            if jky_file.name == "mod.jky":
                # Directory module
                module_name = str(rel_path.parent).replace("/", "::")
            else:
                # File module
                module_name = str(rel_path.with_suffix("")).replace("/", "::")

            modules[jky_file] = module_name

        return modules

    def load_file(self, path: Path) -> str:
        """Load source code from a file."""
        if path in self.loaded_files:
            return self.loaded_files[path]

        with open(path, "r", encoding="utf-8") as f:
            source = f.read()

        self.loaded_files[path] = source
        return source

    def compute_checksum(self, source: str) -> str:
        """Compute SHA256 checksum of source code."""
        return hashlib.sha256(source.encode()).hexdigest()

    def create_module_from_file(self, file_path: Path, module_name: str) -> Module:
        """Create a Module object from a file."""
        source = self.load_file(file_path)
        checksum = self.compute_checksum(source)

        module = Module(
            name=module_name,
            path=file_path,
            source=source,
            checksum=checksum,
        )

        self.registry.register_module(module, file_path)
        return module

    def load_project(self) -> ModuleRegistry:
        """Load all modules in the project."""
        modules = self.discover_modules()

        for file_path, module_name in sorted(modules.items()):
            self.create_module_from_file(file_path, module_name)

        return self.registry

    def get_module(self, name: str) -> Optional[Module]:
        """Get a loaded module by name."""
        return self.registry.get_module(name)


# Global module registry
_registry: Optional[ModuleRegistry] = None
_loader: Optional[ModuleLoader] = None


def get_registry() -> ModuleRegistry:
    """Get the global module registry."""
    global _registry
    if _registry is None:
        _registry = ModuleRegistry()
    return _registry


def get_loader(base_dir: Path = Path(".")) -> ModuleLoader:
    """Get or create the global module loader."""
    global _loader
    if _loader is None:
        _loader = ModuleLoader(base_dir)
    return _loader


def reset_modules():
    """Reset global module state (for testing)."""
    global _registry, _loader
    _registry = None
    _loader = None
