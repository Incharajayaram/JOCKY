"""Mutation testing framework for measuring test quality."""

import subprocess
import tempfile
import shutil
from pathlib import Path
from typing import List, Tuple, Optional
from dataclasses import dataclass
import json

from mutator import get_all_mutators, Mutant


@dataclass
class MutationResult:
    """Result of a single mutation test."""
    mutant: Mutant
    killed: bool
    test_output: str


@dataclass
class MutationReport:
    """Complete mutation testing report."""
    total_mutants: int
    killed_mutants: int
    survived_mutants: List[Mutant]
    results: List[MutationResult]

    @property
    def score(self) -> float:
        """Return mutation score as percentage."""
        if self.total_mutants == 0:
            return 100.0
        return (self.killed_mutants / self.total_mutants) * 100

    def format_report(self) -> str:
        """Format report for display."""
        lines = [
            "╭─────────────────────────────────╮",
            "│   Mutation Test Report          │",
            "├─────────────────────────────────┤",
            f"│ Total Mutants: {self.total_mutants:<17}│",
            f"│ Killed: {self.killed_mutants:<24}│",
            f"│ Survived: {len(self.survived_mutants):<21}│",
            f"│ Score: {self.score:.1f}%{' ' * 22}│",
            "└─────────────────────────────────┘",
        ]

        if self.survived_mutants:
            lines.extend([
                "",
                "Survived mutants (not caught by tests):",
                "─" * 40,
            ])
            for mutant in self.survived_mutants[:10]:  # Show first 10
                lines.append(f"  • Line {mutant.line}: {mutant.description}")
            if len(self.survived_mutants) > 10:
                lines.append(f"  ... and {len(self.survived_mutants) - 10} more")

        return "\n".join(lines)


class MutationTester:
    """Run mutation tests to measure test quality."""

    def __init__(self, test_file: str, source_file: str, timeout: int = 10):
        """
        Initialize mutation tester.

        Args:
            test_file: Path to test file to run
            source_file: Path to source file to mutate
            timeout: Timeout per test run in seconds
        """
        self.test_file = Path(test_file)
        self.source_file = Path(source_file)
        self.timeout = timeout
        self.results: List[MutationResult] = []

    def run(self) -> MutationReport:
        """Run full mutation testing suite."""
        # Read source code
        with open(self.source_file) as f:
            source_code = f.read()

        # Get all mutators
        mutators = get_all_mutators()
        all_mutants: List[Mutant] = []

        # Generate mutants from all operators
        for mutator in mutators:
            mutants = mutator.mutate(source_code)
            all_mutants.extend(mutants)

        # Run tests against each mutant
        killed_count = 0
        survived_mutants = []

        for i, mutant in enumerate(all_mutants):
            print(f"\rTesting mutant {i + 1}/{len(all_mutants)}", end='', flush=True)

            is_killed = self._test_mutant(mutant)
            if is_killed:
                killed_count += 1
            else:
                survived_mutants.append(mutant)

            self.results.append(MutationResult(
                mutant=mutant,
                killed=is_killed,
                test_output=""
            ))

        print()  # Newline after progress

        return MutationReport(
            total_mutants=len(all_mutants),
            killed_mutants=killed_count,
            survived_mutants=survived_mutants,
            results=self.results
        )

    def _test_mutant(self, mutant: Mutant) -> bool:
        """Test if a mutant is killed (caught) by tests.

        Returns:
            True if mutant is killed (tests fail with mutation)
            False if mutant survives (tests still pass)
        """
        with tempfile.TemporaryDirectory() as tmpdir:
            tmpdir_path = Path(tmpdir)

            # Copy the source file with mutation
            mutant_source = tmpdir_path / self.source_file.name
            with open(mutant_source, 'w') as f:
                f.write(mutant.mutated_code)

            # Copy test file
            test_copy = tmpdir_path / self.test_file.name
            shutil.copy(self.test_file, test_copy)

            # Run pytest on the mutant
            try:
                result = subprocess.run(
                    ['python', '-m', 'pytest', str(test_copy), '-q'],
                    cwd=tmpdir_path,
                    capture_output=True,
                    timeout=self.timeout,
                    text=True
                )

                # If tests fail (return code != 0), mutant is killed
                return result.returncode != 0
            except subprocess.TimeoutExpired:
                # Timeout means we assume mutant is killed (tests are slow)
                return True
            except Exception:
                # Error running tests - assume not killed to be conservative
                return False

    def run_and_report(self) -> str:
        """Run mutation tests and return formatted report."""
        report = self.run()
        return report.format_report()

    def export_json(self, output_file: str):
        """Export results to JSON."""
        report = MutationReport(
            total_mutants=len(self.results),
            killed_mutants=sum(1 for r in self.results if r.killed),
            survived_mutants=[r.mutant for r in self.results if not r.killed],
            results=self.results
        )

        data = {
            'total_mutants': report.total_mutants,
            'killed_mutants': report.killed_mutants,
            'survived_mutants': len(report.survived_mutants),
            'score': report.score,
            'survived': [
                {
                    'name': m.name,
                    'line': m.line,
                    'description': m.description,
                }
                for m in report.survived_mutants
            ]
        }

        with open(output_file, 'w') as f:
            json.dump(data, f, indent=2)


def main():
    """CLI entry point."""
    import argparse

    parser = argparse.ArgumentParser(description='Mutation testing framework')
    parser.add_argument('--test', required=True, help='Test file to run')
    parser.add_argument('--source', required=True, help='Source file to mutate')
    parser.add_argument('--timeout', type=int, default=10, help='Test timeout in seconds')
    parser.add_argument('--export', help='Export results to JSON file')
    parser.add_argument('--quiet', action='store_true', help='Suppress output')

    args = parser.parse_args()

    tester = MutationTester(args.test, args.source, timeout=args.timeout)

    if not args.quiet:
        print(f"Running mutation tests...")
        print(f"  Test file: {args.test}")
        print(f"  Source file: {args.source}")
        print()

    report_text = tester.run_and_report()

    if not args.quiet:
        print(report_text)

    if args.export:
        tester.export_json(args.export)
        if not args.quiet:
            print(f"\nResults exported to {args.export}")


if __name__ == '__main__':
    main()
