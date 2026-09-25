"""Module resolver for JOCKY.

Resolves `use` statements to actual module files and merges them into the AST.
Modules are searched in:
1. stdlib directory (jocky/stdlib/)
2. Project-local modules directory (./jocky_modules/)
3. User modules directory (~/.jocky/modules/)
"""

from pathlib import Path
from typing import List, Optional, Dict, Set
from .ast import Program, UseStmt, FFIDecl
from .lexer import Lexer
from .parser import Parser

class ModuleResolver:
    def __init__(self, project_root: Optional[Path] = None):
        self.project_root = project_root or Path.cwd()
        # Find stdlib directory
        self.stdlib_paths = self._find_stdlib_paths()
        self._resolved: Dict[str, Program] = {}
        self._resolving: Set[str] = set()

    def _find_stdlib_paths(self) -> List[Path]:
        """Find all possible stdlib directories."""
        candidates = [
            Path(__file__).parent.parent / "stdlib",  # src/jocky/stdlib
            self.project_root / "jocky" / "stdlib",
            self.project_root / "stdlib",
            Path.home() / ".jocky" / "stdlib",
        ]
        return [p for p in candidates if p.exists()]

    def resolve_module(self, module_path: str) -> Optional[Program]:
        """Resolve a module path to an AST program.

        Args:
            module_path: e.g., "jocky.linux.modules"

        Returns:
            Parsed Program from the module, or None if not found.
        """
        if module_path in self._resolved:
            return self._resolved[module_path]

        if module_path in self._resolving:
            raise RuntimeError(f"Circular dependency: {module_path}")

        self._resolving.add(module_path)

        try:
            # Try each stdlib path
            for stdlib_path in self.stdlib_paths:
                module_file = self._find_module_file(stdlib_path, module_path)
                if module_file:
                    src = module_file.read_text()
                    lexer = Lexer(src)
                    tokens = lexer.tokenize()
                    parser = Parser(tokens)
                    program = parser.parse()
                    self._resolved[module_path] = program
                    return program

            raise RuntimeError(f"Module not found: {module_path}")

        finally:
            self._resolving.discard(module_path)

    def _find_module_file(self, root: Path, module_path: str) -> Optional[Path]:
        """Find a module file for the given path.

        Converts "jocky.linux.modules" -> "jocky/linux/modules.jky"
        """
        parts = module_path.split(".")
        # Try as a file: jocky/linux/modules.jky
        module_file = root.joinpath(*parts).with_suffix(".jky")
        if module_file.exists():
            return module_file

        return None

    def resolve_program(self, program: Program) -> Program:
        """Resolve all use statements in a program and merge modules.

        Returns a new Program with UseStmt nodes removed and module contents merged.
        """
        use_stmts: List[UseStmt] = []
        other_decls = []

        # Separate use statements from other declarations
        for decl in program.decls:
            if isinstance(decl, UseStmt):
                use_stmts.append(decl)
            else:
                other_decls.append(decl)

        # Resolve each module
        all_decls = []
        for use_stmt in use_stmts:
            module_prog = self.resolve_module(use_stmt.module_path)
            if module_prog:
                all_decls.extend(module_prog.decls)

        # Add non-use declarations
        all_decls.extend(other_decls)

        return Program(all_decls)
