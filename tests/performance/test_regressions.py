"""Performance regression tests for JOCKY compiler."""

import subprocess
import json
from pathlib import Path
import pytest


class PerformanceRegression:
    """Track and verify build performance across changes."""

    BASELINE_DIR = Path(__file__).parent / "baselines"
    REGRESSION_THRESHOLD = 0.10  # 10% variance allowed
    MIN_RUNS = 2  # Minimum runs to establish baseline

    def __init__(self):
        self.BASELINE_DIR.mkdir(exist_ok=True)

    def _get_baseline_path(self, example_name: str) -> Path:
        """Get baseline file path for an example."""
        return self.BASELINE_DIR / f"{example_name}.json"

    def _run_build(self, source_file: Path, profile: str = "none") -> float:
        """Run a single build and return elapsed time in seconds."""
        try:
            result = subprocess.run(
                ["jocky", "build", str(source_file), "-p", profile, "-o", "/tmp/jocky_perf_test"],
                capture_output=True,
                text=True,
                timeout=300,
            )
        except FileNotFoundError:
            pytest.skip("jocky compiler not found (expected in CI/CD environment with compiled binaries)")

        if result.returncode != 0:
            pytest.fail(f"Build failed: {result.stderr}")
        return 0.0

    def _measure_build(self, source_file: Path, profile: str = "none", iterations: int = 3) -> dict:
        """Measure build performance across multiple iterations."""
        import time

        times = []
        for _ in range(iterations):
            start = time.perf_counter()
            self._run_build(source_file, profile)
            elapsed = time.perf_counter() - start
            times.append(elapsed)

        return {
            "average": sum(times) / len(times),
            "min": min(times),
            "max": max(times),
            "times": times,
            "iterations": iterations,
        }

    def check_regression(self, example_name: str, source_file: Path, profile: str = "none"):
        """Check if current build performance regresses from baseline."""
        baseline_path = self._get_baseline_path(example_name)

        if not baseline_path.exists():
            # First run - establish baseline
            baseline = self._measure_build(source_file, profile, iterations=self.MIN_RUNS)
            baseline_path.write_text(json.dumps(baseline, indent=2))
            pytest.skip(f"Baseline established for {example_name}")

        # Load baseline
        baseline = json.loads(baseline_path.read_text())
        baseline_time = baseline["average"]

        # Measure current performance
        current = self._measure_build(source_file, profile, iterations=2)
        current_time = current["average"]

        # Check for regressions
        regression_pct = (current_time - baseline_time) / baseline_time
        if regression_pct > self.REGRESSION_THRESHOLD:
            pytest.fail(
                f"Performance regression detected in {example_name}:\n"
                f"  Baseline: {baseline_time:.3f}s\n"
                f"  Current:  {current_time:.3f}s\n"
                f"  Regression: {regression_pct*100:.1f}%"
            )


@pytest.fixture
def perf_tester():
    """Fixture providing performance regression tester."""
    return PerformanceRegression()


def test_basic_example_build(perf_tester):
    """Test build performance for basic example doesn't regress."""
    example_file = Path(__file__).parent.parent.parent / "examples" / "basic.jky"
    if not example_file.exists():
        pytest.skip("basic.jky example not found")

    perf_tester.check_regression("basic_example", example_file)


def test_pattern_matching_build(perf_tester):
    """Test build performance for pattern matching example."""
    example_file = Path(__file__).parent.parent.parent / "examples" / "pattern_matching.jky"
    if not example_file.exists():
        pytest.skip("pattern_matching.jky example not found")

    perf_tester.check_regression("pattern_matching_example", example_file)


def test_enum_matching_build(perf_tester):
    """Test build performance for enum matching example."""
    example_file = Path(__file__).parent.parent.parent / "examples" / "enum_matching.jky"
    if not example_file.exists():
        pytest.skip("enum_matching.jky example not found")

    perf_tester.check_regression("enum_matching_example", example_file)


@pytest.mark.parametrize("profile", ["none", "light", "standard"])
def test_profile_build_times(perf_tester, profile):
    """Test build times for different obfuscation profiles."""
    example_file = Path(__file__).parent.parent.parent / "examples" / "basic.jky"
    if not example_file.exists():
        pytest.skip("basic.jky example not found")

    perf_tester.check_regression(f"basic_example_{profile}", example_file, profile=profile)
