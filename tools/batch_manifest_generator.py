#!/usr/bin/env python3
"""
Batch Manifest Generator for JOCKY

Process multiple drivers and generate manifest files in bulk.

Usage:
  python batch_manifest_generator.py -i drivers/ -o manifests/ --method ghidra
  python batch_manifest_generator.py --driver-dir pipeline_scripts/drivers_out/
"""

import argparse
import concurrent.futures
import json
import sys
from pathlib import Path
from typing import Dict, List, Optional

# Import the main generator
try:
    from manifest_generator import ManifestGenerator, GhidraExtractor, CapstoneExtractor
except ImportError:
    print("[!] manifest_generator.py not found in same directory")
    sys.exit(1)


class BatchProcessor:
    """Process multiple drivers and generate manifests."""

    def __init__(self, method: str = "auto", ghidra_path: Optional[str] = None,
                 max_workers: int = 4, output_format: str = "json"):
        self.method = method
        self.ghidra_path = ghidra_path
        self.max_workers = max_workers
        self.output_format = output_format
        self.results = {}
        self.failed = []

    def get_extractor(self):
        """Get configured extractor."""
        if self.method == "ghidra":
            try:
                return GhidraExtractor(self.ghidra_path)
            except (ValueError, FileNotFoundError) as e:
                print(f"[!] Ghidra not available: {e}")
                return None
        elif self.method == "capstone":
            try:
                return CapstoneExtractor()
            except ImportError:
                print("[!] Capstone not installed")
                return None
        else:  # auto
            try:
                return GhidraExtractor(self.ghidra_path)
            except:
                try:
                    return CapstoneExtractor()
                except:
                    return None

    def process_driver(self, driver_path: Path) -> Optional[Dict]:
        """Process a single driver file."""
        try:
            extractor = self.get_extractor()
            if not extractor:
                self.failed.append((str(driver_path), "No extractor available"))
                return None

            print(f"[*] Processing {driver_path.name}...", end=" ", flush=True)

            generator = ManifestGenerator(extractor)
            analysis = generator.analyze_driver(str(driver_path))
            manifest = generator.generate_manifest(analysis)

            print(f"✓ ({len(analysis.ioctls)} IOCTLs)")
            return {
                'file': driver_path.name,
                'path': str(driver_path),
                'manifest': manifest,
                'status': 'success'
            }

        except Exception as e:
            print(f"✗ ({str(e)[:50]})")
            self.failed.append((str(driver_path), str(e)))
            return None

    def process_drivers(self, driver_paths: List[Path], output_dir: Optional[Path] = None):
        """Process multiple drivers in parallel."""
        print(f"[*] Processing {len(driver_paths)} drivers with method={self.method}...")

        results = []

        with concurrent.futures.ThreadPoolExecutor(max_workers=self.max_workers) as executor:
            futures = {
                executor.submit(self.process_driver, path): path
                for path in driver_paths
            }

            for future in concurrent.futures.as_completed(futures):
                result = future.result()
                if result:
                    results.append(result)

                    # Save individual manifest if output dir specified
                    if output_dir:
                        output_dir.mkdir(parents=True, exist_ok=True)
                        output_file = output_dir / f"{result['file']}.manifest.json"
                        with open(output_file, 'w') as f:
                            json.dump(result['manifest'], f, indent=2)

        return results

    def generate_summary(self, results: List[Dict]) -> Dict:
        """Generate summary of batch processing."""
        capabilities_count = {}
        device_types = set()
        total_ioctls = 0

        for result in results:
            manifest = result['manifest']
            total_ioctls += len(manifest['ioctl_map'])

            for cap in manifest.get('capabilities', []):
                capabilities_count[cap] = capabilities_count.get(cap, 0) + 1

            for ioctl_info in manifest.get('ioctl_map', {}).values():
                device_type = (ioctl_info['code'] >> 16) & 0xFFFF
                device_types.add(f"0x{device_type:04x}")

        return {
            'total_drivers': len(results),
            'total_ioctls': total_ioctls,
            'failed': len(self.failed),
            'capabilities': capabilities_count,
            'device_types': sorted(list(device_types)),
            'common_capabilities': sorted(
                [(cap, count) for cap, count in capabilities_count.items()],
                key=lambda x: x[1],
                reverse=True
            )[:10]
        }


def find_drivers(root_dir: Path) -> List[Path]:
    """Find all driver files in directory."""
    drivers = []

    driver_extensions = {'.sys', '.exe', '.dll'}

    for path in root_dir.rglob('*'):
        if path.is_file() and path.suffix.lower() in driver_extensions:
            # Skip non-PE files (basic check)
            try:
                with open(path, 'rb') as f:
                    header = f.read(2)
                    if header == b'MZ':  # PE header
                        drivers.append(path)
            except:
                pass

    return sorted(drivers)


def main():
    parser = argparse.ArgumentParser(
        description="Batch process drivers and generate JOCKY manifests"
    )
    parser.add_argument("-i", "--input", type=Path, help="Input directory with drivers")
    parser.add_argument("-o", "--output", type=Path, help="Output directory for manifests")
    parser.add_argument("--driver-dir", type=Path, help="Use default JOCKY drivers directory")
    parser.add_argument("--method", default="auto", choices=["auto", "ghidra", "capstone"],
                       help="Extraction method")
    parser.add_argument("--ghidra", help="Path to Ghidra installation")
    parser.add_argument("-j", "--jobs", type=int, default=4, help="Parallel jobs")
    parser.add_argument("--summary", action="store_true", help="Generate summary JSON")
    parser.add_argument("--aggregate", help="Aggregate all manifests into single YAML")

    args = parser.parse_args()

    # Determine input directory
    if args.driver_dir:
        input_dir = args.driver_dir
    elif args.input:
        input_dir = args.input
    else:
        # Try default JOCKY location
        jocky_drivers = Path("/home/deval/JOCKY/pipeline_scripts/drivers_out")
        if jocky_drivers.exists():
            input_dir = jocky_drivers
        else:
            print("[!] No input directory specified")
            parser.print_help()
            sys.exit(1)

    input_dir = Path(input_dir)
    if not input_dir.exists():
        print(f"[!] Input directory not found: {input_dir}")
        sys.exit(1)

    output_dir = Path(args.output) if args.output else input_dir / "manifests"

    # Find drivers
    drivers = find_drivers(input_dir)
    if not drivers:
        print(f"[!] No drivers found in {input_dir}")
        sys.exit(1)

    print(f"[*] Found {len(drivers)} drivers")

    # Process
    processor = BatchProcessor(
        method=args.method,
        ghidra_path=args.ghidra,
        max_workers=args.jobs,
        output_format="json"
    )

    results = processor.process_drivers(drivers, output_dir)

    # Summary
    if results:
        summary = processor.generate_summary(results)

        print(f"\n[+] Processing complete!")
        print(f"    Processed: {summary['total_drivers']} drivers")
        print(f"    Failed: {summary['failed']}")
        print(f"    IOCTLs found: {summary['total_ioctls']}")
        print(f"    Capabilities: {', '.join(dict(summary['common_capabilities']).keys()) or 'none'}")

        if args.summary:
            summary_file = output_dir / "summary.json"
            summary_file.parent.mkdir(parents=True, exist_ok=True)
            with open(summary_file, 'w') as f:
                json.dump(summary, f, indent=2)
            print(f"    Summary: {summary_file}")

        if processor.failed:
            print(f"\n[!] {len(processor.failed)} drivers failed:")
            for path, error in processor.failed[:5]:
                print(f"    - {Path(path).name}: {error[:60]}")
    else:
        print("[!] No drivers successfully processed")
        sys.exit(1)


if __name__ == "__main__":
    main()
