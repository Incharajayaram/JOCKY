#!/usr/bin/env python3
"""
JOCKY Manifest Generator - Auto-extract IOCTLs from driver binaries

This tool analyzes PE driver files and generates JOCKY manifest YAML/JSON
containing IOCTL definitions, device paths, and capabilities.

Supports:
  - Ghidra headless mode (primary)
  - IDA Pro Python API (via separate script)
  - Capstone-based fallback disassembly
"""

import argparse
import json
import os
import re
import struct
import subprocess
import sys
from collections import defaultdict
from dataclasses import dataclass, field, asdict
from pathlib import Path
from typing import Dict, List, Optional, Tuple, Any

try:
    import capstone
    HAS_CAPSTONE = True
except ImportError:
    HAS_CAPSTONE = False


@dataclass
class IOCTLInfo:
    """Represents a discovered IOCTL and its metadata."""
    code: int
    hex_code: str
    name: Optional[str] = None
    capability: Optional[str] = None
    method: str = "METHOD_BUFFERED"
    access: str = "FILE_ANY_ACCESS"
    input_size: Optional[int] = None
    output_size: Optional[int] = None
    input_layout: Dict[str, Any] = field(default_factory=dict)
    output_layout: Dict[str, Any] = field(default_factory=dict)
    device_io_control_call: Optional[str] = None

    def to_dict(self) -> Dict:
        """Convert to dictionary for serialization."""
        d = asdict(self)
        d.pop('device_io_control_call', None)
        if not d['input_layout']:
            d.pop('input_layout')
        if not d['output_layout']:
            d.pop('output_layout')
        return d


@dataclass
class DriverAnalysis:
    """Results from analyzing a driver binary."""
    filename: str
    sha256: Optional[str] = None
    arch: str = "x64"
    signed: bool = False
    company: Optional[str] = None
    description: Optional[str] = None
    ioctls: List[IOCTLInfo] = field(default_factory=list)
    device_paths: Dict[str, Any] = field(default_factory=dict)
    capabilities: List[str] = field(default_factory=list)
    analysis_method: str = "unknown"
    confidence: float = 0.5


class IOCTLExtractor:
    """Base class for IOCTL extraction strategies."""

    def extract(self, binary_path: str) -> List[IOCTLInfo]:
        """Extract IOCTLs from a driver binary."""
        raise NotImplementedError


class CapstoneExtractor(IOCTLExtractor):
    """Extract IOCTLs using Capstone disassembly (fallback)."""

    def __init__(self, arch: str = "x64"):
        if not HAS_CAPSTONE:
            raise ImportError("capstone library required for Capstone extraction")

        self.arch = arch
        self.md = capstone.Cs(capstone.CS_ARCH_X86,
                             capstone.CS_MODE_64 if arch == "x64" else capstone.CS_MODE_32)
        self.md.detail = True

    def extract(self, binary_path: str) -> List[IOCTLInfo]:
        """Extract IOCTL codes from .text section."""
        ioctls = []

        try:
            with open(binary_path, 'rb') as f:
                dos_header = f.read(2)
                if dos_header != b'MZ':
                    return ioctls

                f.seek(0x3c)
                pe_offset = struct.unpack('<I', f.read(4))[0]

                f.seek(pe_offset)
                pe_sig = f.read(4)
                if pe_sig != b'PE\x00\x00':
                    return ioctls

                num_sections = struct.unpack('<H', f.read(2))[0]
                f.seek(pe_offset + 20 + 8)  # Skip to sections

                text_section = None
                for _ in range(num_sections):
                    sect_name = f.read(8).rstrip(b'\x00')
                    sect_vsize = struct.unpack('<I', f.read(4))[0]
                    sect_vaddr = struct.unpack('<I', f.read(4))[0]
                    sect_size = struct.unpack('<I', f.read(4))[0]
                    sect_ptr = struct.unpack('<I', f.read(4))[0]

                    f.read(16)  # Skip reloc, etc

                    if sect_name == b'.text':
                        text_section = (sect_ptr, sect_size, sect_vaddr)
                        break

                if not text_section:
                    return ioctls

                ptr, size, vaddr = text_section
                f.seek(ptr)
                code = f.read(size)

        except Exception as e:
            print(f"[!] Failed to read binary: {e}")
            return ioctls

        # Search for IOCTL patterns: CTL_CODE macros or direct values
        # Pattern: mov reg, 0x?????? (where ????? matches CTL_CODE format)
        ioctls = self._find_ioctl_patterns(code)

        return ioctls

    def _find_ioctl_patterns(self, code: bytes) -> List[IOCTLInfo]:
        """Find IOCTL codes in raw binary."""
        ioctls = []
        ioctl_set = set()

        # Simple pattern matching for IOCTL-like values
        # IOCTLs typically follow CTL_CODE: ((DeviceType << 16) | (Function << 2) | Method)
        for i in range(0, len(code) - 4, 2):
            value = struct.unpack('<I', code[i:i+4])[0]

            # CTL_CODE format validation
            if self._is_likely_ioctl(value):
                if value not in ioctl_set:
                    ioctl_set.add(value)
                    ioctl = IOCTLInfo(
                        code=value,
                        hex_code=f"0x{value:08x}",
                        method=self._get_ioctl_method(value),
                        access="FILE_ANY_ACCESS"
                    )
                    ioctls.append(ioctl)

        return sorted(ioctls, key=lambda x: x.code)

    def _is_likely_ioctl(self, value: int) -> bool:
        """Check if value matches IOCTL structure."""
        if value < 0x20000:
            return False
        if value > 0xFFFFFFFF:
            return False

        method = value & 0x3
        func = (value >> 2) & 0xFFF
        device = (value >> 16) & 0xFFFF

        return 0 <= device <= 0xFFFF and 0 <= func <= 0xFFF and 0 <= method <= 3

    def _get_ioctl_method(self, value: int) -> str:
        """Extract METHOD from IOCTL code."""
        method = value & 0x3
        methods = {
            0: "METHOD_BUFFERED",
            1: "METHOD_IN_DIRECT",
            2: "METHOD_OUT_DIRECT",
            3: "METHOD_NEITHER"
        }
        return methods.get(method, "METHOD_BUFFERED")


class GhidraExtractor(IOCTLExtractor):
    """Extract IOCTLs using Ghidra headless mode."""

    def __init__(self, ghidra_path: Optional[str] = None):
        self.ghidra_path = ghidra_path or os.environ.get('GHIDRA_INSTALL_DIR')
        if not self.ghidra_path:
            raise ValueError("GHIDRA_INSTALL_DIR not set and ghidra_path not provided")

        self.headless_script = self._find_headless()

    def _find_headless(self) -> str:
        """Locate Ghidra headless analyzer."""
        candidates = [
            Path(self.ghidra_path) / "support" / "analyzeHeadless",
            Path(self.ghidra_path) / "support" / "analyzeHeadless.bat",
            Path(self.ghidra_path) / "bin" / "analyzeHeadless",
        ]
        for candidate in candidates:
            if candidate.exists():
                return str(candidate)
        raise FileNotFoundError("Could not find Ghidra analyzeHeadless")

    def extract(self, binary_path: str) -> List[IOCTLInfo]:
        """Use Ghidra to extract IOCTL codes."""
        # Create temporary Ghidra script
        script = self._generate_ghidra_script()
        script_path = Path(binary_path).parent / "ioctl_extractor.py"
        script_path.write_text(script)

        try:
            return self._run_ghidra_analysis(binary_path, str(script_path))
        finally:
            script_path.unlink(missing_ok=True)

    def _generate_ghidra_script(self) -> str:
        """Generate Ghidra Python script for IOCTL extraction."""
        return '''
# Ghidra IOCTL Extractor Script
# @author JOCKY Manifest Generator
# @category Search.InstructionPattern
# @keybinding
# @toolbar

import json
from ghidra.program.model.address import AddressSet

def find_ioctls():
    """Find IOCTL codes in the binary."""
    ioctls = []
    seen = set()

    # Search for MOV instructions with large immediates
    listing = currentProgram.getListing()

    for instruction in listing.getInstructions(True):
        mnemonic = instruction.getMnemonicString()

        # Look for mov with immediate values (potential IOCTL codes)
        if mnemonic in ['mov', 'movzx', 'lea']:
            operands = instruction.getDefaultOperands()
            for op in operands:
                if op.isImmediate():
                    value = instruction.getOperandValue(1) & 0xFFFFFFFF

                    # Basic IOCTL validation
                    if is_likely_ioctl(value) and value not in seen:
                        seen.add(value)
                        ioctls.append({
                            'code': value,
                            'hex': '0x{:08x}'.format(value),
                            'address': str(instruction.getAddress())
                        })

    return ioctls

def is_likely_ioctl(value):
    """Check if value looks like an IOCTL code."""
    if value < 0x20000 or value > 0xFFFFFFFF:
        return False
    method = value & 0x3
    func = (value >> 2) & 0xFFF
    device = (value >> 16) & 0xFFFF
    return 0 <= device <= 0xFFFF and 0 <= func <= 0xFFF

# Main
ioctls = find_ioctls()
print(json.dumps(ioctls))
'''

    def _run_ghidra_analysis(self, binary_path: str, script_path: str) -> List[IOCTLInfo]:
        """Run Ghidra headless analysis."""
        try:
            result = subprocess.run([
                self.headless_script,
                "/tmp/jocky_ghidra_project",
                "jocky_analysis",
                "-import", binary_path,
                "-scriptPath", str(Path(script_path).parent),
                "-postScript", Path(script_path).name,
                "-deleteProject"
            ], capture_output=True, text=True, timeout=30)

            # Parse JSON output from script
            ioctls = []
            for line in result.stdout.split('\n'):
                if line.startswith('['):
                    try:
                        data = json.loads(line)
                        for item in data:
                            ioctl = IOCTLInfo(
                                code=item['code'],
                                hex_code=item['hex'],
                                device_io_control_call=item.get('address')
                            )
                            ioctls.append(ioctl)
                    except json.JSONDecodeError:
                        pass

            return ioctls

        except subprocess.TimeoutExpired:
            print("[!] Ghidra analysis timed out")
            return []
        except Exception as e:
            print(f"[!] Ghidra analysis failed: {e}")
            return []


class ManifestGenerator:
    """Generate JOCKY manifests from driver analysis."""

    def __init__(self, extractor: Optional[IOCTLExtractor] = None):
        self.extractor = extractor or self._get_default_extractor()

    def _get_default_extractor(self) -> IOCTLExtractor:
        """Get best available extractor."""
        try:
            return GhidraExtractor()
        except (ValueError, FileNotFoundError):
            if HAS_CAPSTONE:
                return CapstoneExtractor()
            raise RuntimeError("No extractor available (need Ghidra or capstone)")

    def analyze_driver(self, binary_path: str) -> DriverAnalysis:
        """Analyze a driver binary and extract IOCTLs."""
        path = Path(binary_path)

        analysis = DriverAnalysis(
            filename=path.name,
            sha256=self._compute_sha256(binary_path),
            analysis_method=type(self.extractor).__name__
        )

        # Extract IOCTLs
        analysis.ioctls = self.extractor.extract(binary_path)

        # Infer capabilities from IOCTLs
        analysis.capabilities = self._infer_capabilities(analysis.ioctls)

        # Try to extract PE metadata
        self._extract_pe_metadata(binary_path, analysis)

        return analysis

    def _compute_sha256(self, path: str) -> Optional[str]:
        """Compute SHA256 of file."""
        try:
            import hashlib
            with open(path, 'rb') as f:
                return hashlib.sha256(f.read()).hexdigest()
        except Exception:
            return None

    def _extract_pe_metadata(self, path: str, analysis: DriverAnalysis) -> None:
        """Extract company/description from PE headers."""
        try:
            from pefile import PE
            pe = PE(path)

            if hasattr(pe, 'VS_FIXEDFILEINFO') and pe.VS_FIXEDFILEINFO:
                info = pe.VS_FIXEDFILEINFO[0]
                # Extract strings from version info
                if hasattr(pe, 'FileInfo'):
                    for entry in pe.FileInfo:
                        if hasattr(entry, 'StringTable'):
                            for st in entry.StringTable:
                                if hasattr(st, 'entries'):
                                    entries = st.entries
                                    analysis.company = entries.get('CompanyName', '').decode('utf-16-le', errors='ignore').rstrip('\0')
                                    analysis.description = entries.get('FileDescription', '').decode('utf-16-le', errors='ignore').rstrip('\0')
        except ImportError:
            pass
        except Exception:
            pass

    def _infer_capabilities(self, ioctls: List[IOCTLInfo]) -> List[str]:
        """Infer driver capabilities from IOCTL patterns."""
        capabilities = set()

        if not ioctls:
            return []

        # Common IOCTL ranges and their capabilities
        ioctl_patterns = {
            (0x80000000, 0x80010000): "arb_physical_read",
            (0x90000000, 0x90010000): "arb_physical_write",
            (0xA0000000, 0xA0010000): "msr_read",
            (0xB0000000, 0xB0010000): "msr_write",
            (0xC0000000, 0xC0010000): "port_io",
        }

        for ioctl in ioctls:
            code = ioctl.code
            for (start, end), cap in ioctl_patterns.items():
                if start <= code < end:
                    capabilities.add(cap)

        return sorted(list(capabilities))

    def generate_manifest(self, analysis: DriverAnalysis) -> Dict[str, Any]:
        """Generate manifest JSON from analysis."""
        ioctl_map = {}

        for i, ioctl in enumerate(analysis.ioctls):
            name = ioctl.name or f"ioctl_{i}"
            ioctl_map[name] = ioctl.to_dict()

        return {
            "name": analysis.filename,
            "sha256": analysis.sha256,
            "arch": analysis.arch,
            "signed": analysis.signed,
            "company": analysis.company,
            "description": analysis.description,
            "evasion_score": 5.0,
            "device_paths": analysis.device_paths,
            "service_name": analysis.filename.replace('.sys', ''),
            "init_sequence": ["create_service", "start_service", "open_device"],
            "capabilities": analysis.capabilities,
            "ioctl_map": ioctl_map,
            "analysis_method": analysis.analysis_method,
            "confidence": analysis.confidence,
            "tested": False,
            "source": "manifest_generator"
        }


def main():
    parser = argparse.ArgumentParser(
        description="JOCKY Manifest Generator - Extract IOCTLs from drivers"
    )
    parser.add_argument("driver", help="Driver binary path")
    parser.add_argument("-o", "--output", help="Output manifest path (JSON)")
    parser.add_argument("-f", "--format", default="json", choices=["json", "yaml"],
                       help="Output format")
    parser.add_argument("--ghidra", help="Path to Ghidra install")
    parser.add_argument("--method", default="auto", choices=["auto", "ghidra", "capstone"],
                       help="Extraction method")

    args = parser.parse_args()

    # Select extractor
    if args.method == "ghidra" or (args.method == "auto" and args.ghidra):
        try:
            extractor = GhidraExtractor(args.ghidra)
        except (ValueError, FileNotFoundError) as e:
            print(f"[!] Ghidra not available: {e}")
            if args.method == "ghidra":
                sys.exit(1)
            extractor = None
    else:
        extractor = None

    if not extractor and args.method in ["auto", "capstone"]:
        try:
            extractor = CapstoneExtractor()
        except ImportError:
            print("[!] Capstone not available, install with: pip install capstone")
            sys.exit(1)

    if not extractor:
        print("[!] No extractor available")
        sys.exit(1)

    # Generate manifest
    print(f"[*] Analyzing {args.driver}...")
    generator = ManifestGenerator(extractor)
    analysis = generator.analyze_driver(args.driver)

    print(f"[+] Found {len(analysis.ioctls)} IOCTLs")
    print(f"[+] Capabilities: {', '.join(analysis.capabilities) or '(none inferred)'}")

    manifest = generator.generate_manifest(analysis)

    # Output
    if args.format == "yaml":
        try:
            import yaml
            output = yaml.dump(manifest, default_flow_style=False, sort_keys=False)
        except ImportError:
            print("[!] PyYAML not installed, outputting JSON instead")
            output = json.dumps(manifest, indent=2)
    else:
        output = json.dumps(manifest, indent=2)

    if args.output:
        Path(args.output).write_text(output)
        print(f"[*] Saved to {args.output}")
    else:
        print(output)


if __name__ == "__main__":
    main()
