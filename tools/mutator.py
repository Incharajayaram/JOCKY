"""Mutation testing operators for measuring test quality."""

import ast
import re
from typing import List, Tuple, Optional
from dataclasses import dataclass


@dataclass
class Mutant:
    """Represents a single code mutation."""
    name: str
    mutated_code: str
    original_code: str
    line: int
    description: str


class MutationOperator:
    """Base class for mutation operators."""

    def mutate(self, code: str) -> List[Mutant]:
        """Generate mutants of the given code."""
        raise NotImplementedError


class ArithmeticMutator(MutationOperator):
    """Mutate arithmetic operators: + ↔ -, * ↔ /, etc."""

    MUTATIONS = {
        '+': ['-', '*', '/'],
        '-': ['+', '*', '/'],
        '*': ['+', '-', '/'],
        '/': ['+', '-', '*'],
        '%': ['+', '-'],
    }

    def mutate(self, code: str) -> List[Mutant]:
        """Generate arithmetic mutations."""
        mutants = []

        # Find all arithmetic operators in the code
        for op, replacements in self.MUTATIONS.items():
            # Escape special regex characters
            escaped_op = re.escape(op)
            # Match the operator with word boundaries or spaces
            pattern = rf'([^"\']*?)(\s{escaped_op}\s|\s{escaped_op}(?!\=))'

            for match in re.finditer(pattern, code):
                if match:
                    start = match.start(2)
                    end = match.end(2)
                    for new_op in replacements:
                        mutated = code[:start] + f' {new_op} ' + code[end:]
                        mutants.append(Mutant(
                            name=f"arith_{op}_to_{new_op}",
                            mutated_code=mutated,
                            original_code=code,
                            line=code[:start].count('\n') + 1,
                            description=f"Changed arithmetic operator {op} to {new_op}"
                        ))

        return mutants


class ComparisonMutator(MutationOperator):
    """Mutate comparisons: == ↔ !=, < ↔ >, etc."""

    MUTATIONS = {
        '==': ['!=', '<', '>'],
        '!=': ['=='],
        '<': ['>', '<=', '>='],
        '>': ['<', '<=', '>='],
        '<=': ['<', '>='],
        '>=': ['>'],
    }

    def mutate(self, code: str) -> List[Mutant]:
        """Generate comparison mutations."""
        mutants = []

        for op, replacements in self.MUTATIONS.items():
            # Match the comparison operator (not in strings)
            escaped_op = re.escape(op)
            pattern = rf'([^"\']*?)({escaped_op})'

            for match in re.finditer(pattern, code):
                if match:
                    start = match.start(2)
                    end = match.end(2)

                    # Skip if in string or comment
                    if self._is_in_string_or_comment(code, start):
                        continue

                    for new_op in replacements:
                        mutated = code[:start] + new_op + code[end:]
                        mutants.append(Mutant(
                            name=f"cmp_{op}_to_{new_op}",
                            mutated_code=mutated,
                            original_code=code,
                            line=code[:start].count('\n') + 1,
                            description=f"Changed comparison {op} to {new_op}"
                        ))

        return mutants

    def _is_in_string_or_comment(self, code: str, pos: int) -> bool:
        """Check if position is inside a string or comment."""
        # Count quotes before position
        before = code[:pos]
        single_quotes = before.count("'") - before.count("\\'")
        double_quotes = before.count('"') - before.count('\\"')

        # Check for comments
        if '//' in before.split('\n')[-1]:
            return True

        return (single_quotes % 2 == 1) or (double_quotes % 2 == 1)


class ConstantMutator(MutationOperator):
    """Mutate integer constants: 0 → 1, x → x+1, etc."""

    def mutate(self, code: str) -> List[Mutant]:
        """Generate constant mutations."""
        mutants = []

        # Find all integer literals not in strings
        pattern = r'\b(\d+)\b'

        for match in re.finditer(pattern, code):
            val_str = match.group(1)
            val = int(val_str)
            start = match.start(1)

            # Skip if in string
            if self._is_in_string(code, start):
                continue

            # Generate mutations: +1, -1
            for delta in [-1, 1]:
                new_val = val + delta
                if new_val >= 0:  # Keep non-negative
                    mutated = code[:start] + str(new_val) + code[match.end(1):]
                    mutants.append(Mutant(
                        name=f"const_{val}_to_{new_val}",
                        mutated_code=mutated,
                        original_code=code,
                        line=code[:start].count('\n') + 1,
                        description=f"Changed constant {val} to {new_val}"
                    ))

        return mutants

    def _is_in_string(self, code: str, pos: int) -> bool:
        """Check if position is inside a string."""
        before = code[:pos]
        single_quotes = before.count("'") - before.count("\\'")
        double_quotes = before.count('"') - before.count('\\"')
        return (single_quotes % 2 == 1) or (double_quotes % 2 == 1)


class BoundaryMutator(MutationOperator):
    """Mutate loop/condition boundaries."""

    def mutate(self, code: str) -> List[Mutant]:
        """Generate boundary mutations."""
        mutants = []

        # Replace < with <=, > with >=
        boundary_pairs = [
            ('<', '<='),
            ('>', '>='),
            ('<=', '<'),
            ('>=', '>'),
        ]

        for old, new in boundary_pairs:
            escaped = re.escape(old)
            for match in re.finditer(escaped, code):
                pos = match.start()
                if not self._is_in_string_or_comment(code, pos):
                    mutated = code[:pos] + new + code[pos + len(old):]
                    mutants.append(Mutant(
                        name=f"boundary_{old}_to_{new}",
                        mutated_code=mutated,
                        original_code=code,
                        line=code[:pos].count('\n') + 1,
                        description=f"Changed boundary {old} to {new}"
                    ))

        return mutants

    def _is_in_string_or_comment(self, code: str, pos: int) -> bool:
        """Check if position is inside a string or comment."""
        before = code[:pos]
        single_quotes = before.count("'") - before.count("\\'")
        double_quotes = before.count('"') - before.count('\\"')

        if '//' in before.split('\n')[-1]:
            return True

        return (single_quotes % 2 == 1) or (double_quotes % 2 == 1)


class ReturnValueMutator(MutationOperator):
    """Mutate return values: return x -> return x+1, return true -> return false."""

    def mutate(self, code: str) -> List[Mutant]:
        """Generate return value mutations."""
        mutants = []

        # Find return statements - match various types of values
        pattern = r'return\s+(True|False|[a-zA-Z_]\w*|\d+)'

        for match in re.finditer(pattern, code):
            value = match.group(1)
            start = match.start(1)
            end = match.end(1)

            # Generate mutations
            if value == 'True':
                new_value = 'False'
                mutated = code[:start] + new_value + code[end:]
                mutants.append(Mutant(
                    name=f"ret_{value}_to_{new_value}",
                    mutated_code=mutated,
                    original_code=code,
                    line=code[:start].count('\n') + 1,
                    description=f"Changed return value {value} to {new_value}"
                ))
            elif value == 'False':
                new_value = 'True'
                mutated = code[:start] + new_value + code[end:]
                mutants.append(Mutant(
                    name=f"ret_{value}_to_{new_value}",
                    mutated_code=mutated,
                    original_code=code,
                    line=code[:start].count('\n') + 1,
                    description=f"Changed return value {value} to {new_value}"
                ))
            elif value.isdigit():
                # For numeric returns, try incrementing
                val = int(value)
                new_val = val + 1
                mutated = code[:start] + str(new_val) + code[end:]
                mutants.append(Mutant(
                    name=f"ret_{val}_to_{new_val}",
                    mutated_code=mutated,
                    original_code=code,
                    line=code[:start].count('\n') + 1,
                    description=f"Changed return value {val} to {new_val}"
                ))

        return mutants


def get_all_mutators() -> List[MutationOperator]:
    """Get all available mutation operators."""
    return [
        ArithmeticMutator(),
        ComparisonMutator(),
        ConstantMutator(),
        BoundaryMutator(),
        ReturnValueMutator(),
    ]
