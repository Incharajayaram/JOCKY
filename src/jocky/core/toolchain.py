"""Toolchain discovery for JOCKY.

Finds the LLVM obfuscation toolchain via (in order of priority):
1. JOCKY_LLVM_TOOLCHAIN environment variable
2. jocky.yaml config file (toolchain.llvm_dir key)
3. PATH search for clang + opt + mlir-opt
4. Common install locations
"""

import os
import shutil
from pathlib import Path
from typing import Optional

try:
    import yaml
    _HAS_YAML = True
except ImportError:
    _HAS_YAML = False

class ToolchainError(Exception):
    pass

class Toolchain:
    def __init__(self, root: Path):
        self.root = Path(root)
        self.bin = self.root / "bin"
        self.lib = self.root / "lib"
        self._validate()

    def _validate(self):
        if not self.root.exists():
            raise ToolchainError(f"Toolchain root does not exist: {self.root}")
        if not self.bin.exists():
            raise ToolchainError(f"Toolchain bin directory missing: {self.bin}")

    def clang(self) -> Path:
        return self._find("clang")

    def opt(self) -> Path:
        return self._find("opt")

    def mlir_opt(self) -> Path:
        return self._find("mlir-opt")

    def mlir_translate(self) -> Path:
        return self._find("mlir-translate")

    def llvm_plugin(self) -> Path:
        p = self.lib / "LLVMObfuscationPlugin.so"
        if not p.exists():
            p = self.lib / "LLVMObfuscationPlugin.dylib"
        if not p.exists():
            p = self.lib / "LLVMObfuscationPlugin.dll"
        return p

    def mlir_plugin(self) -> Path:
        p = self.lib / "MLIRObfuscationPlugin.so"
        if not p.exists():
            p = self.lib / "MLIRObfuscationPlugin.dylib"
        if not p.exists():
            p = self.lib / "MLIRObfuscationPlugin.dll"
        return p

    def _find(self, name: str) -> Path:
        # Prefer toolchain bin
        candidate = self.bin / name
        if candidate.exists():
            return candidate
        # Fall back to PATH
        found = shutil.which(name)
        if found:
            return Path(found)
        raise ToolchainError(f"Tool binary not found: {name}")

    def __repr__(self):
        return f"Toolchain({self.root})"


def _find_in_config() -> Optional[Path]:
    """Look for toolchain path in jocky.yaml config files."""
    if not _HAS_YAML:
        return None
    search_paths = [
        Path.cwd() / "jocky.yaml",
        Path.home() / ".config" / "jocky" / "config.yaml",
        Path.home() / ".jocky.yaml",
    ]
    for p in search_paths:
        if p.exists():
            try:
                with open(p, "r") as f:
                    data = yaml.safe_load(f)
                if data and "toolchain" in data:
                    tc = data["toolchain"]
                    if isinstance(tc, str):
                        return Path(tc)
                    if isinstance(tc, dict) and "llvm_dir" in tc:
                        return Path(tc["llvm_dir"])
            except Exception:
                continue
    return None


def _find_in_path() -> Optional[Path]:
    """If clang, opt, and mlir-opt are all on PATH, infer the prefix."""
    clang = shutil.which("clang")
    opt = shutil.which("opt")
    mlir_opt = shutil.which("mlir-opt")
    if clang and opt and mlir_opt:
        # All three must share the same parent bin directory
        clang_p = Path(clang).resolve().parent
        opt_p = Path(opt).resolve().parent
        mlir_p = Path(mlir_opt).resolve().parent
        if clang_p == opt_p == mlir_p:
            # Verify the plugin exists too
            lib_dir = clang_p.parent / "lib"
            plugin = lib_dir / "LLVMObfuscationPlugin.so"
            if plugin.exists() or (lib_dir / "MLIRObfuscationPlugin.so").exists():
                return clang_p.parent
    return None


def _find_in_common_locations() -> Optional[Path]:
    """Search common install prefixes."""
    candidates = [
        Path.home() / "projects" / "llvm-obfuscation-tools-linux-x86_64",
        Path.home() / "llvm-obfuscation-tools-linux-x86_64",
        Path("/usr/local/llvm-obfuscation"),
        Path("/opt/llvm-obfuscation"),
        Path("/opt/llvm-obfuscation-tools"),
        Path.cwd().parent.parent / "llvm-obfuscation-tools-linux-x86_64",
    ]
    for c in candidates:
        if (c / "bin" / "clang").exists() and (c / "lib" / "LLVMObfuscationPlugin.so").exists():
            return c
    return None


def discover_toolchain() -> Toolchain:
    """Discover the LLVM obfuscation toolchain.

    Priority:
    1. JOCKY_LLVM_TOOLCHAIN environment variable
    2. jocky.yaml config file (toolchain.llvm_dir)
    3. PATH search (clang + opt + mlir-opt in same bin/)
    4. Common install locations

    Raises ToolchainError if no toolchain is found.
    """
    # 1. Environment variable
    env_dir = os.environ.get("JOCKY_LLVM_TOOLCHAIN")
    if env_dir:
        return Toolchain(Path(env_dir))

    # 2. Config file
    config_dir = _find_in_config()
    if config_dir:
        return Toolchain(config_dir)

    # 3. PATH search
    path_dir = _find_in_path()
    if path_dir:
        return Toolchain(path_dir)

    # 4. Common locations
    common_dir = _find_in_common_locations()
    if common_dir:
        return Toolchain(common_dir)

    raise ToolchainError(
        "Could not find LLVM obfuscation toolchain.\n"
        "Set JOCKY_LLVM_TOOLCHAIN to the directory containing bin/ and lib/,\n"
        "or create a jocky.yaml with:\n"
        "  toolchain:\n"
        "    llvm_dir: /path/to/llvm-obfuscation-tools\n"
        "or ensure clang, opt, and mlir-opt are on your PATH."
    )
