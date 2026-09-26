# JOCKY Tools

Utility scripts for JOCKY development and analysis.

## Tools

### manifest_generator.py

Auto-extract IOCTLs from Windows driver binaries.

**Quick Start:**
```bash
python manifest_generator.py RTCore64.sys -o manifest.json
```

See [../docs/manifest-generation.md](../docs/manifest-generation.md) for details.

**Features:**
- Multi-backend support (Ghidra, IDA, Capstone)
- IOCTL code discovery
- Capability inference
- PE metadata extraction

**Backends:**
- `ghidra` - Open-source, recommended
- `capstone` - Fast fallback pattern matching
- `ida` - Professional, highest accuracy

### batch_manifest_generator.py

Process multiple drivers in parallel.

**Usage:**
```bash
python batch_manifest_generator.py -i drivers/ -o manifests/ --jobs 8
```

**Features:**
- Concurrent processing
- Summary statistics
- Per-driver output
- LOLDrivers integration

### ghidra_ioctl_extractor.py

Ghidra Python script for headless analysis.

**Integration:**
Place in Ghidra's script directory or use via:
```bash
ghidra_home/support/analyzeHeadless <project> <name> \
  -import driver.sys -postScript ghidra_ioctl_extractor.py
```

### ida_ioctl_extractor.py

IDA Pro Python script (requires IDA Pro 7.0+).

**Integration:**
1. Open driver in IDA Pro
2. File → Script file... → Select this script
3. Results in IDA console and `/tmp/<driver>_manifest.json`

## Installation

### Prerequisites

```bash
# Core dependencies
pip install pefile pyyaml

# For Ghidra support (recommended)
export GHIDRA_INSTALL_DIR=/path/to/ghidra

# For Capstone fallback (optional)
pip install capstone

# For IDA Pro (optional, requires license)
# Copy script to IDA's script directory
```

### Setup

```bash
cd /home/deval/JOCKY
chmod +x tools/*.py
```

## Usage Examples

### Single Driver

```bash
# Basic (auto-detect backend)
python tools/manifest_generator.py RTCore64.sys

# With Ghidra
python tools/manifest_generator.py RTCore64.sys \
  --method ghidra --ghidra /opt/ghidra

# Output formats
python tools/manifest_generator.py RTCore64.sys -f json
python tools/manifest_generator.py RTCore64.sys -f yaml
```

### Batch Processing

```bash
# Process directory
python tools/batch_manifest_generator.py \
  -i pipeline_scripts/drivers_out/ \
  -o manifests/ \
  --method ghidra \
  --jobs 8

# Generate summary
python tools/batch_manifest_generator.py \
  --driver-dir pipeline_scripts/drivers_out/ \
  --summary
```

### LOLDrivers Collection

```bash
# Analyze all LOLDrivers
python tools/batch_manifest_generator.py \
  --driver-dir pipeline_scripts/drivers_out/ \
  -o src/runtime/byovd/drivers/
```

## Output Format

Manifests contain:
- IOCTL codes and device paths
- Extracted capabilities
- PE metadata
- Analysis confidence score
- Inferred function codes

Example:
```json
{
  "name": "RTCore64.sys",
  "sha256": "32e1a8513eee...",
  "arch": "x64",
  "ioctl_map": {
    "map_physical": {
      "code": "0x82000000",
      "method": "METHOD_BUFFERED"
    }
  },
  "capabilities": ["arb_physical_read", "arb_physical_write"],
  "confidence": 0.85
}
```

## Architecture

```
manifest_generator.py (main engine)
├── IOCTLExtractor (base class)
│   ├── GhidraExtractor
│   ├── CapstoneExtractor
│   └── IDAExtractor
├── DriverAnalysis (data model)
└── ManifestGenerator (orchestration)

batch_manifest_generator.py (batch processor)
└── Uses manifest_generator.py
```

## Adding Custom Extractors

Implement the `IOCTLExtractor` interface:

```python
class CustomExtractor(IOCTLExtractor):
    def extract(self, binary_path: str) -> List[IOCTLInfo]:
        # Return list of IOCTLInfo objects
        pass
```

Register in `ManifestGenerator._get_default_extractor()`.

## Performance

| Method | Speed | Accuracy | Notes |
|---|---|---|---|
| Capstone | <1s | 40% | Pattern matching only |
| Ghidra | 5-30s | 70% | Control flow analysis |
| IDA | 10-60s | 92% | Full decompilation |

Batch processing with 8 workers:
- 100 drivers: ~8-10 minutes

## Troubleshooting

### Ghidra Not Found
```bash
export GHIDRA_INSTALL_DIR=/opt/ghidra
# or use --ghidra flag
```

### Import Errors
```bash
pip install pefile pyyaml capstone
```

### Hangs on Batch Processing
```bash
# Reduce parallel workers
--jobs 2
```

## Integration with JOCKY

### Update Driver Manifests

```bash
python tools/batch_manifest_generator.py \
  --driver-dir pipeline_scripts/drivers_out/ \
  -o src/runtime/byovd/drivers/ \
  --summary

# Manually merge new IOCTL codes into driver_manifest.yaml
```

### CI/CD Workflow

```yaml
# .github/workflows/update-drivers.yml
- name: Generate driver manifests
  run: |
    python tools/batch_manifest_generator.py \
      -i drivers/ -o manifests/ \
      --method ghidra --summary
```

## Contributing

When adding new extractors or features:
1. Maintain the `IOCTLExtractor` interface
2. Add unit tests in `tests/test_manifest_generator.py`
3. Update documentation in `docs/manifest-generation.md`
4. Test with known drivers (RTCore64, WinRing0, etc.)

## References

- [JOCKY Manifest Generation Guide](/docs/manifest-generation.md)
- [BYOVD Driver Configuration](/docs/driver-configuration.md)
- [Ghidra Python API](https://ghidra.re/)
- [IDA Pro SDK](https://www.hex-rays.com/products/ida/)
- [LOLDrivers Database](https://loldrivers.io/)

## License

Part of JOCKY. Use for authorized security research only.
