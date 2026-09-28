"""Test suite for mutation testing infrastructure."""

import pytest
import tempfile
from pathlib import Path
import sys

# Add tools directory to path
sys.path.insert(0, str(Path(__file__).parent.parent.parent / "tools"))

from mutator import (
    ArithmeticMutator,
    ComparisonMutator,
    ConstantMutator,
    BoundaryMutator,
    ReturnValueMutator,
    get_all_mutators,
)
from mutation_test import MutationTester, MutationReport


class TestArithmeticMutator:
    """Test arithmetic operator mutations."""

    def test_addition_mutation(self):
        """Test mutation of addition operator."""
        code = "x = 5 + 3"
        mutator = ArithmeticMutator()
        mutants = mutator.mutate(code)

        assert len(mutants) > 0
        # Should have mutations for + to -, *, /
        names = [m.name for m in mutants]
        assert any("arith_" in name for name in names)

    def test_subtraction_mutation(self):
        """Test mutation of subtraction operator."""
        code = "y = 10 - 2"
        mutator = ArithmeticMutator()
        mutants = mutator.mutate(code)

        assert len(mutants) > 0

    def test_multiplication_mutation(self):
        """Test mutation of multiplication operator."""
        code = "z = 4 * 5"
        mutator = ArithmeticMutator()
        mutants = mutator.mutate(code)

        assert len(mutants) > 0

    def test_division_mutation(self):
        """Test mutation of division operator."""
        code = "a = 20 / 4"
        mutator = ArithmeticMutator()
        mutants = mutator.mutate(code)

        assert len(mutants) > 0


class TestComparisonMutator:
    """Test comparison operator mutations."""

    def test_equality_mutation(self):
        """Test mutation of equality operator."""
        code = "if x == 5:"
        mutator = ComparisonMutator()
        mutants = mutator.mutate(code)

        assert len(mutants) > 0

    def test_inequality_mutation(self):
        """Test mutation of inequality operator."""
        code = "if x != 10:"
        mutator = ComparisonMutator()
        mutants = mutator.mutate(code)

        assert len(mutants) > 0

    def test_less_than_mutation(self):
        """Test mutation of less-than operator."""
        code = "while x < 100:"
        mutator = ComparisonMutator()
        mutants = mutator.mutate(code)

        assert len(mutants) > 0

    def test_greater_than_mutation(self):
        """Test mutation of greater-than operator."""
        code = "if value > 0:"
        mutator = ComparisonMutator()
        mutants = mutator.mutate(code)

        assert len(mutants) > 0

    def test_less_equal_mutation(self):
        """Test mutation of less-equal operator."""
        code = "if x <= 50:"
        mutator = ComparisonMutator()
        mutants = mutator.mutate(code)

        assert len(mutants) > 0

    def test_greater_equal_mutation(self):
        """Test mutation of greater-equal operator."""
        code = "if x >= 10:"
        mutator = ComparisonMutator()
        mutants = mutator.mutate(code)

        assert len(mutants) > 0


class TestConstantMutator:
    """Test constant value mutations."""

    def test_constant_increment(self):
        """Test incrementing constant values."""
        code = "x = 5"
        mutator = ConstantMutator()
        mutants = mutator.mutate(code)

        assert len(mutants) > 0
        # Should have mutations 5->4 and 5->6
        names = [m.name for m in mutants]
        assert any("const_" in name for name in names)

    def test_zero_mutation(self):
        """Test mutation of zero constant."""
        code = "if count == 0:"
        mutator = ConstantMutator()
        mutants = mutator.mutate(code)

        assert len(mutants) > 0

    def test_multiple_constants(self):
        """Test mutation of multiple constants."""
        code = "a = 1 + 2 + 3"
        mutator = ConstantMutator()
        mutants = mutator.mutate(code)

        # Should mutate all three constants
        assert len(mutants) >= 6  # At least 2 mutations per constant


class TestBoundaryMutator:
    """Test boundary condition mutations."""

    def test_strict_to_loose_boundary(self):
        """Test < to <= mutation."""
        code = "for i in range(10):"
        mutator = BoundaryMutator()
        mutants = mutator.mutate(code)

        # < or > might be in the code
        assert isinstance(mutants, list)

    def test_boundary_flipping(self):
        """Test flipping boundary operators."""
        code = "while x > 0:"
        mutator = BoundaryMutator()
        mutants = mutator.mutate(code)

        assert len(mutants) > 0


class TestReturnValueMutator:
    """Test return value mutations."""

    def test_boolean_return_mutation(self):
        """Test mutation of boolean returns."""
        code = "return True"
        mutator = ReturnValueMutator()
        mutants = mutator.mutate(code)

        assert len(mutants) == 1
        assert mutants[0].mutated_code == "return False"

    def test_numeric_return_mutation(self):
        """Test mutation of numeric returns."""
        code = "return 42"
        mutator = ReturnValueMutator()
        mutants = mutator.mutate(code)

        assert len(mutants) > 0
        assert any("ret_42" in m.name for m in mutants)

    def test_false_return_mutation(self):
        """Test mutation of false return."""
        code = "return False"
        mutator = ReturnValueMutator()
        mutants = mutator.mutate(code)

        assert len(mutants) == 1
        assert mutants[0].mutated_code == "return True"


class TestMutatorIntegration:
    """Test integrated mutation operator functionality."""

    def test_all_mutators_available(self):
        """Test that all mutators are available."""
        mutators = get_all_mutators()

        assert len(mutators) >= 5
        assert any(isinstance(m, ArithmeticMutator) for m in mutators)
        assert any(isinstance(m, ComparisonMutator) for m in mutators)
        assert any(isinstance(m, ConstantMutator) for m in mutators)
        assert any(isinstance(m, BoundaryMutator) for m in mutators)
        assert any(isinstance(m, ReturnValueMutator) for m in mutators)

    def test_complex_code_mutation(self):
        """Test mutation of complex code."""
        code = """
def calculate(x, y):
    if x > 0:
        return x + y
    else:
        return x - y
        """

        mutators = get_all_mutators()
        all_mutants = []

        for mutator in mutators:
            mutants = mutator.mutate(code)
            all_mutants.extend(mutants)

        # Should generate multiple mutants from multiple operators
        assert len(all_mutants) > 0


class TestMutationReport:
    """Test mutation report generation."""

    def test_report_formatting(self):
        """Test report formatting."""
        code = "x = 1 + 2"
        mutator = ArithmeticMutator()
        mutants = mutator.mutate(code)

        # Create report manually
        report = MutationReport(
            total_mutants=10,
            killed_mutants=9,
            survived_mutants=mutants[:1] if mutants else [],
            results=[]
        )

        assert report.score == 90.0
        formatted = report.format_report()
        assert "Mutation Test Report" in formatted
        assert "90.0%" in formatted

    def test_zero_mutants_score(self):
        """Test score with zero mutants."""
        report = MutationReport(
            total_mutants=0,
            killed_mutants=0,
            survived_mutants=[],
            results=[]
        )

        assert report.score == 100.0


class TestMutationTester:
    """Test mutation test runner."""

    def test_tester_initialization(self):
        """Test MutationTester initialization."""
        with tempfile.TemporaryDirectory() as tmpdir:
            test_file = Path(tmpdir) / "test.py"
            source_file = Path(tmpdir) / "source.py"

            test_file.write_text("# test")
            source_file.write_text("# source")

            tester = MutationTester(str(test_file), str(source_file))

            assert tester.test_file == test_file
            assert tester.source_file == source_file
            assert tester.timeout == 10

    def test_custom_timeout(self):
        """Test custom timeout setting."""
        with tempfile.TemporaryDirectory() as tmpdir:
            test_file = Path(tmpdir) / "test.py"
            source_file = Path(tmpdir) / "source.py"

            test_file.write_text("# test")
            source_file.write_text("# source")

            tester = MutationTester(str(test_file), str(source_file), timeout=30)

            assert tester.timeout == 30


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
