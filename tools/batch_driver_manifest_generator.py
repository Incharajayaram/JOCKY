#!/usr/bin/env python3
"""
JOCKY Batch Driver Manifest Generator
Extracts IOCTL codes from unblocked drivers in automated batch processing
Authorized: Red Hat + IIT Bombay Cyber Security Team

Input: Directory of driver files (.sys, .ko)
Output: Extended driver manifest with IOCTL signatures
"""

import os
import re
import json
import struct
import argparse
from pathlib import Path
from typing import List, Dict, Tuple, Optional
from dataclasses import dataclass, asdict
import logging

logging.basicConfig(
    level=logging.INFO,
    format='[%(levelname)s] %(message)s'
)
logger = logging.getLogger(__name__)

@dataclass
class DriverInfo:
    name: str
    path: str
    device_path: str
    ioctl_base: int
    ioctl_codes: List[int]
    reliability_score: float
    evasion_potential: str  # KERNEL_ACCESS, MEMORY_READ, REGISTER_MODIFY
    supported_os: List[str]
    discovered_date: str

class IOCTLExtractor:
    """Extract IOCTL codes from driver binaries"""

    # Common IOCTL patterns in Windows drivers
    IOCTL_PATTERNS = [
        b'\x48\x89\x45',  # mov [rax], rcx
        b'\x0f\xb7',      # movzx
        b'\x41\x57',      # push r15
        b'\xff\x15',      # call qword
    ]

    # Known IOCTL base prefixes
    KNOWN_IOCTL_BASES = {
        0x22: 'FILE_DEVICE_DISK_FILE_SYSTEM',
        0x80: 'FILE_DEVICE_UNKNOWN',
        0x81: 'AMD_DEVICE',
        0x82: 'REALTEK_DEVICE',
        0x84: 'MSI_DEVICE',
        0x85: 'ENE_DEVICE',
        0x88: 'INTEL_DEVICE',
    }

    def __init__(self, binary_path: str):
        self.binary_path = binary_path
        self.binary_data = None
        self.ioctl_codes = set()
        self.device_name = None

    def load_binary(self) -> bool:
        """Load driver binary into memory"""
        try:
            with open(self.binary_path, 'rb') as f:
                self.binary_data = f.read()
            logger.info(f"Loaded {self.binary_path} ({len(self.binary_data)} bytes)")
            return True
        except Exception as e:
            logger.error(f"Failed to load {self.binary_path}: {e}")
            return False

    def extract_ioctl_codes(self) -> List[int]:
        """Extract IOCTL codes from binary"""
        if not self.binary_data:
            return []

        ioctl_list = []

        # Search for IOCTL patterns
        # IOCTLs are typically encoded as:
        # CTL_CODE = (Device << 16) | (Access << 14) | (Function << 2) | Method

        for i in range(len(self.binary_data) - 4):
            # Look for 4-byte IOCTL-like values
            value = struct.unpack('<I', self.binary_data[i:i+4])[0]

            # Check if it matches IOCTL pattern
            # Typical range: 0x80000000 to 0xFFFFFFFF
            if self._is_valid_ioctl(value):
                ioctl_list.append(value)

        # Deduplicate and sort
        self.ioctl_codes = sorted(set(ioctl_list))
        return self.ioctl_codes

    def _is_valid_ioctl(self, value: int) -> bool:
        """Check if value matches IOCTL pattern"""
        # IOCTL values are typically:
        # - In range 0x80000000 to 0xFFFFFFFF (kernel mode)
        # - Have specific structure: (Device << 16) | (Access << 14) | (Function << 2) | Method

        if value < 0x80000000 or value > 0xFFFFFFFF:
            return False

        # Check device field (bits 16-31)
        device = (value >> 16) & 0xFFFF

        # Known device codes
        if device in self.KNOWN_IOCTL_BASES or device in range(0x20, 0xFF):
            return True

        return False

    def extract_device_name(self) -> str:
        """Extract device name from binary strings"""
        if not self.binary_data:
            return "Unknown"

        # Search for common device name patterns
        patterns = [
            b'\\Device\\',
            b'\\DosDevices\\',
            b'\\\\.\\',
            b'DeviceName',
        ]

        for pattern in patterns:
            idx = self.binary_data.find(pattern)
            if idx != -1:
                try:
                    # Extract null-terminated string
                    end = self.binary_data.find(b'\x00', idx)
                    if end != -1:
                        device_name = self.binary_data[idx:end].decode('utf-8', errors='ignore')
                        return device_name
                except:
                    pass

        return Path(self.binary_path).stem

    def calculate_reliability(self) -> float:
        """Score driver reliability (0.0 to 1.0)"""
        score = 0.5  # baseline

        # Higher score if we found IOCTL codes
        if self.ioctl_codes:
            score += min(0.3, len(self.ioctl_codes) * 0.01)

        # Check file size (larger = more likely real driver)
        if self.binary_data:
            size = len(self.binary_data)
            if size > 50000:  # > 50KB
                score += 0.15
            if size > 500000:  # > 500KB
                score += 0.05

        return min(1.0, score)

class ManifestGenerator:
    """Generate extended driver manifest"""

    # Known unblocked drivers (first 9)
    PRIMARY_DRIVERS = [
        {
            'name': 'rtkiow10x64.sys',
            'device_path': '\\\\.\\\RTCore64',
            'ioctl_base': 0x82000000,
            'evasion': 'KERNEL_ACCESS'
        },
        {
            'name': 'rtkiow8x64.sys',
            'device_path': '\\\\.\\\RTCore64',
            'ioctl_base': 0x82000000,
            'evasion': 'KERNEL_ACCESS'
        },
        {
            'name': 'AMDRyzenMasterDriver.sys',
            'device_path': '\\\\.\\\AMDRyzenMasterDriver',
            'ioctl_base': 0x81000000,
            'evasion': 'KERNEL_ACCESS'
        },
        {
            'name': 'nvflsh64.sys',
            'device_path': '\\\\.\\\nvflsh64',
            'ioctl_base': 0x80002000,
            'evasion': 'MEMORY_READ'
        },
        {
            'name': 'speedfan.sys',
            'device_path': '\\\\.\\\speedfan',
            'ioctl_base': 0x80002000,
            'evasion': 'MEMORY_READ'
        },
        {
            'name': 'ene.sys',
            'device_path': '\\\\.\\\EneIo',
            'ioctl_base': 0x85000000,
            'evasion': 'REGISTER_MODIFY'
        },
        {
            'name': 'iQVW64.SYS',
            'device_path': '\\\\.\\\Nal',
            'ioctl_base': 0x80802000,
            'evasion': 'KERNEL_ACCESS'
        },
        {
            'name': 'UCOREW64.SYS',
            'device_path': '\\\\.\\\Global\\',
            'ioctl_base': 0x88000000,
            'evasion': 'KERNEL_ACCESS'
        },
        {
            'name': 'NTIOLib.sys',
            'device_path': '\\\\.\\\NTIOLib',
            'ioctl_base': 0x84002000,
            'evasion': 'REGISTER_MODIFY'
        },
    ]

    def __init__(self):
        self.drivers: List[DriverInfo] = []

    def add_from_directory(self, driver_dir: str) -> int:
        """Scan directory for drivers and extract IOCTL codes"""
        added = 0

        driver_path = Path(driver_dir)
        if not driver_path.is_dir():
            logger.error(f"Directory not found: {driver_dir}")
            return 0

        # Find all driver files
        driver_files = list(driver_path.glob('**/*.sys')) + list(driver_path.glob('**/*.ko'))

        logger.info(f"Found {len(driver_files)} driver files")

        for driver_file in driver_files:
            logger.info(f"Processing: {driver_file.name}")

            # Extract IOCTL codes
            extractor = IOCTLExtractor(str(driver_file))
            if not extractor.load_binary():
                continue

            ioctl_codes = extractor.extract_ioctl_codes()
            device_name = extractor.extract_device_name()
            reliability = extractor.calculate_reliability()

            # Determine IOCTL base from codes
            ioctl_base = self._infer_ioctl_base(ioctl_codes) or 0x80000000

            # Create driver info
            driver = DriverInfo(
                name=driver_file.name,
                path=str(driver_file),
                device_path=device_name,
                ioctl_base=ioctl_base,
                ioctl_codes=ioctl_codes[:5],  # Store first 5 codes
                reliability_score=reliability,
                evasion_potential='KERNEL_ACCESS',
                supported_os=['Windows 10', 'Windows 11'],
                discovered_date='2026-09-28'
            )

            self.drivers.append(driver)
            added += 1

            logger.info(f"  ✓ Extracted {len(ioctl_codes)} IOCTL codes (reliability: {reliability:.2f})")

        return added

    def _infer_ioctl_base(self, ioctl_codes: List[int]) -> Optional[int]:
        """Infer IOCTL base from extracted codes"""
        if not ioctl_codes:
            return None

        # IOCTL base is typically in high 16 bits
        bases = set()
        for code in ioctl_codes:
            base = code & 0xFFFF0000
            if base >= 0x80000000:
                bases.add(base)

        if bases:
            return max(bases)  # Use most common base

        return None

    def add_known_drivers(self):
        """Add known unblocked drivers as baseline"""
        logger.info(f"Adding {len(self.PRIMARY_DRIVERS)} known primary drivers")

        for known in self.PRIMARY_DRIVERS:
            driver = DriverInfo(
                name=known['name'],
                path=f"/drivers/{known['name']}",
                device_path=known['device_path'],
                ioctl_base=known['ioctl_base'],
                ioctl_codes=[known['ioctl_base']],
                reliability_score=0.95,  # Known to work
                evasion_potential=known['evasion'],
                supported_os=['Windows 10', 'Windows 11'],
                discovered_date='2026-09-28'
            )
            self.drivers.append(driver)

    def sort_by_reliability(self):
        """Sort drivers by reliability score (highest first)"""
        self.drivers.sort(key=lambda d: d.reliability_score, reverse=True)

    def export_json(self, output_path: str):
        """Export manifest as JSON"""
        manifest = {
            'version': '1.0',
            'generated_date': '2026-09-28',
            'total_drivers': len(self.drivers),
            'drivers': [asdict(d) for d in self.drivers]
        }

        with open(output_path, 'w') as f:
            json.dump(manifest, f, indent=2)

        logger.info(f"Manifest exported to {output_path}")

    def export_c_header(self, output_path: str):
        """Export manifest as C header file"""
        with open(output_path, 'w') as f:
            f.write('/*\n')
            f.write(' * JOCKY Extended Driver Manifest\n')
            f.write(' * Auto-generated from batch analysis\n')
            f.write(' * Generated: 2026-09-28\n')
            f.write(' */\n\n')
            f.write('#ifndef JOCKY_EXTENDED_DRIVER_MANIFEST_H\n')
            f.write('#define JOCKY_EXTENDED_DRIVER_MANIFEST_H\n\n')

            f.write(f'#define JOCKY_DRIVER_COUNT {len(self.drivers)}\n\n')

            f.write('typedef struct {\n')
            f.write('    const char* name;\n')
            f.write('    const char* device_path;\n')
            f.write('    uint32_t ioctl_base;\n')
            f.write('    float reliability_score;\n')
            f.write('} JOCKY_DRIVER_ENTRY;\n\n')

            f.write('static const JOCKY_DRIVER_ENTRY jocky_drivers[] = {\n')

            for driver in self.drivers[:20]:  # Limit to top 20 for example
                f.write(f'    {{"{driver.name}", "{driver.device_path}", ')
                f.write(f'0x{driver.ioctl_base:08x}, {driver.reliability_score:.2f}}},\n')

            f.write('};\n\n')
            f.write('#endif\n')

        logger.info(f"C header exported to {output_path}")

    def print_summary(self):
        """Print summary statistics"""
        print("\n" + "="*70)
        print("JOCKY Extended Driver Manifest Summary")
        print("="*70)
        print(f"\nTotal drivers analyzed: {len(self.drivers)}")

        # Reliability distribution
        high_reliability = sum(1 for d in self.drivers if d.reliability_score >= 0.8)
        med_reliability = sum(1 for d in self.drivers if 0.5 <= d.reliability_score < 0.8)
        low_reliability = sum(1 for d in self.drivers if d.reliability_score < 0.5)

        print(f"\nReliability Distribution:")
        print(f"  High (>0.8):   {high_reliability}")
        print(f"  Medium (0.5-0.8): {med_reliability}")
        print(f"  Low (<0.5):    {low_reliability}")

        # Evasion potential
        kernel_access = sum(1 for d in self.drivers if d.evasion_potential == 'KERNEL_ACCESS')
        memory_read = sum(1 for d in self.drivers if d.evasion_potential == 'MEMORY_READ')
        register_mod = sum(1 for d in self.drivers if d.evasion_potential == 'REGISTER_MODIFY')

        print(f"\nEvasion Potential:")
        print(f"  Kernel Access:   {kernel_access}")
        print(f"  Memory Read:     {memory_read}")
        print(f"  Register Modify: {register_mod}")

        # Top drivers
        print(f"\nTop 5 Drivers by Reliability:")
        for i, driver in enumerate(self.drivers[:5], 1):
            print(f"  {i}. {driver.name} ({driver.reliability_score:.2f})")

        print("\n" + "="*70 + "\n")

def main():
    parser = argparse.ArgumentParser(
        description='JOCKY Batch Driver Manifest Generator'
    )
    parser.add_argument('driver_dir', nargs='?', default='/home/incharanew/Downloads/drivers_out',
                       help='Directory containing driver files')
    parser.add_argument('--output-json', default='extended_driver_manifest.json',
                       help='Output JSON manifest')
    parser.add_argument('--output-header', default='jocky_extended_drivers.h',
                       help='Output C header file')

    args = parser.parse_args()

    print("\n" + "="*70)
    print("JOCKY Batch Driver Manifest Generator")
    print("="*70 + "\n")

    # Create generator
    generator = ManifestGenerator()

    # Add known primary drivers
    generator.add_known_drivers()

    # Try to scan for additional drivers
    if os.path.isdir(args.driver_dir):
        logger.info(f"Scanning {args.driver_dir} for drivers...")
        added = generator.add_from_directory(args.driver_dir)
        logger.info(f"Added {added} drivers from directory")
    else:
        logger.warning(f"Driver directory not found: {args.driver_dir}")
        logger.info("Using primary drivers only")

    # Sort by reliability
    generator.sort_by_reliability()

    # Export manifests
    generator.export_json(args.output_json)
    generator.export_c_header(args.output_header)

    # Print summary
    generator.print_summary()

    print(f"[+] Manifest generation complete")
    print(f"    JSON: {args.output_json}")
    print(f"    Header: {args.output_header}\n")

if __name__ == '__main__':
    main()
