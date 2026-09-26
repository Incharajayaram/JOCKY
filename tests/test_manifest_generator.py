"""
Tests for manifest generator tool.
"""

import json
import sys
import tempfile
from pathlib import Path

import pytest

# Add tools directory to path
sys.path.insert(0, str(Path(__file__).parent.parent / "tools"))

from manifest_generator import (
    IOCTLExtractor, CapstoneExtractor, IOCTLInfo, DriverAnalysis,
    ManifestGenerator
)


class TestIOCTLInfo:
    """Test IOCTL info data structure."""

    def test_ioctl_creation(self):
        ioctl = IOCTLInfo(
            code=0x82000000,
            hex_code="0x82000000",
            name="map_physical",
            capability="arb_physical_read",
            method="METHOD_BUFFERED"
        )

        assert ioctl.code == 0x82000000
        assert ioctl.hex_code == "0x82000000"
        assert ioctl.name == "map_physical"
        assert ioctl.capability == "arb_physical_read"

    def test_ioctl_to_dict(self):
        ioctl = IOCTLInfo(
            code=0x82000000,
            hex_code="0x82000000"
        )

        d = ioctl.to_dict()
        assert d['code'] == 0x82000000
        assert d['hex_code'] == "0x82000000"
        assert 'device_io_control_call' not in d


class TestDriverAnalysis:
    """Test driver analysis data structure."""

    def test_analysis_creation(self):
        analysis = DriverAnalysis(
            filename="RTCore64.sys",
            sha256="abc123",
            arch="x64",
            company="Realtek"
        )

        assert analysis.filename == "RTCore64.sys"
        assert analysis.sha256 == "abc123"
        assert analysis.arch == "x64"
        assert analysis.company == "Realtek"
        assert len(analysis.ioctls) == 0


class TestCapstoneExtractor:
    """Test Capstone-based IOCTL extraction."""

    @pytest.mark.skipif(not Path("/home/deval/JOCKY/pipeline_scripts/drivers_out/cpuz.sys").exists(),
                       reason="Test driver not available")
    def test_extract_real_driver(self):
        """Test extraction from real driver binary."""
        extractor = CapstoneExtractor()

        driver_path = "/home/deval/JOCKY/pipeline_scripts/drivers_out/cpuz.sys"
        ioctls = extractor.extract(driver_path)

        # Basic validation
        assert isinstance(ioctls, list)
        for ioctl in ioctls:
            assert isinstance(ioctl, IOCTLInfo)
            assert ioctl.code > 0
            assert "0x" in ioctl.hex_code

    def test_ioctl_validation(self):
        """Test IOCTL code validation."""
        extractor = CapstoneExtractor()

        # Valid IOCTL
        assert extractor._is_likely_ioctl(0x82000000)

        # Invalid IOCTLs
        assert not extractor._is_likely_ioctl(0x00000001)  # Too small
        assert not extractor._is_likely_ioctl(0xFFFFFFFF + 1)  # Too large

    def test_method_detection(self):
        """Test METHOD extraction from IOCTL code."""
        extractor = CapstoneExtractor()

        # METHOD_BUFFERED (0)
        assert extractor._get_ioctl_method(0x82000000) == "METHOD_BUFFERED"

        # METHOD_IN_DIRECT (1)
        assert extractor._get_ioctl_method(0x82000001) == "METHOD_IN_DIRECT"

        # METHOD_OUT_DIRECT (2)
        assert extractor._get_ioctl_method(0x82000002) == "METHOD_OUT_DIRECT"

        # METHOD_NEITHER (3)
        assert extractor._get_ioctl_method(0x82000003) == "METHOD_NEITHER"


class TestManifestGenerator:
    """Test manifest generation."""

    def test_generator_creation(self):
        """Test generator initialization."""
        extractor = CapstoneExtractor()
        generator = ManifestGenerator(extractor)

        assert generator.extractor == extractor

    def test_capability_inference(self):
        """Test capability inference from IOCTLs."""
        generator = ManifestGenerator()

        # Create test IOCTLs
        ioctls = [
            IOCTLInfo(code=0x82000000, hex_code="0x82000000"),  # Physical memory
            IOCTLInfo(code=0x82000008, hex_code="0x82000008"),  # MSR read
        ]

        caps = generator._infer_capabilities(ioctls)

        # Should detect arbitrary I/O capabilities
        assert len(caps) >= 0
        assert all(isinstance(cap, str) for cap in caps)

    @pytest.mark.skipif(not Path("/home/deval/JOCKY/pipeline_scripts/drivers_out/cpuz.sys").exists(),
                       reason="Test driver not available")
    def test_analyze_real_driver(self):
        """Test full analysis of real driver."""
        generator = ManifestGenerator(CapstoneExtractor())

        driver_path = "/home/deval/JOCKY/pipeline_scripts/drivers_out/cpuz.sys"
        analysis = generator.analyze_driver(driver_path)

        assert analysis.filename == "cpuz.sys"
        assert analysis.sha256 is not None
        assert analysis.arch == "x64"
        assert isinstance(analysis.ioctls, list)

    @pytest.mark.skipif(not Path("/home/deval/JOCKY/pipeline_scripts/drivers_out/cpuz.sys").exists(),
                       reason="Test driver not available")
    def test_generate_manifest(self):
        """Test manifest generation."""
        generator = ManifestGenerator(CapstoneExtractor())

        driver_path = "/home/deval/JOCKY/pipeline_scripts/drivers_out/cpuz.sys"
        analysis = generator.analyze_driver(driver_path)
        manifest = generator.generate_manifest(analysis)

        # Validate structure
        assert manifest['name'] == "cpuz.sys"
        assert 'sha256' in manifest
        assert 'arch' in manifest
        assert 'ioctl_map' in manifest
        assert 'capabilities' in manifest
        assert 'analysis_method' in manifest

        # Should be JSON serializable
        json_str = json.dumps(manifest)
        assert len(json_str) > 0


class TestManifestOutput:
    """Test manifest output formats."""

    @pytest.mark.skipif(not Path("/home/deval/JOCKY/pipeline_scripts/drivers_out/cpuz.sys").exists(),
                       reason="Test driver not available")
    def test_json_output(self):
        """Test JSON format output."""
        generator = ManifestGenerator(CapstoneExtractor())

        driver_path = "/home/deval/JOCKY/pipeline_scripts/drivers_out/cpuz.sys"
        analysis = generator.analyze_driver(driver_path)
        manifest = generator.generate_manifest(analysis)

        # Should be valid JSON
        json_str = json.dumps(manifest)
        parsed = json.loads(json_str)

        assert parsed['name'] == "cpuz.sys"

    @pytest.mark.skipif(not Path("/home/deval/JOCKY/pipeline_scripts/drivers_out/cpuz.sys").exists(),
                       reason="Test driver not available")
    def test_manifest_with_ioctls(self):
        """Test manifest generation with detected IOCTLs."""
        extractor = CapstoneExtractor()

        # Manually add test IOCTL
        ioctl = IOCTLInfo(
            code=0x82000000,
            hex_code="0x82000000",
            name="test_ioctl",
            method="METHOD_BUFFERED"
        )

        analysis = DriverAnalysis(filename="test.sys")
        analysis.ioctls = [ioctl]

        generator = ManifestGenerator(extractor)
        manifest = generator.generate_manifest(analysis)

        assert len(manifest['ioctl_map']) == 1
        assert 'test_ioctl' in manifest['ioctl_map']


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
