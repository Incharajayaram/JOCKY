"""Comprehensive performance benchmarking for JOCKY compiler optimizations.

Measures compilation speed, runtime efficiency, and memory usage improvements
across lexer, parser, type checker, and codegen stages.
"""

import time
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent.parent / "src"))

from jocky.language.lexer import Lexer
from jocky.language.parser import Parser
from jocky.language.checker import TypeChecker
from jocky.language.codegen import CodeGen
from jocky.language.optimizations import (
    CharacterClassLookup, TypeUnificationCache,
    LLVMTypeCache, IREmissionBuffer
)


class BenchmarkSuite:
    def __init__(self):
        self.results = {}
        self.test_programs = self._create_test_programs()

    def _create_test_programs(self):
        return {
            'simple': '''
fn add(x: i32, y: i32) -> i32 {
    return x + y;
}

fn main() {
    let result: i32 = add(5, 3);
    return result;
}
''',
            'medium': '''
fn multiply_and_add(x: i32, y: i32, z: i32) -> i32 {
    let prod: i32 = x * y;
    return prod + z;
}

fn loop_sum() -> i32 {
    let r1: i32 = 10;
    let r2: i32 = 20;
    let r3: i32 = 30;
    return r1 + r2 + r3;
}

fn main() {
    let result: i32 = multiply_and_add(3, 4, 5);
    let total: i32 = loop_sum();
    return result + total;
}
''',
            'complex': '''
fn compute_square(x: i32) -> i32 {
    return x * x;
}

fn compute_cube(x: i32) -> i32 {
    return x * x * x;
}

fn compute_fourth(x: i32) -> i32 {
    return x * x * x * x;
}

fn compute_fifth(x: i32) -> i32 {
    return x * x * x * x * x;
}

fn complex_chain(a: i32, b: i32, c: i32) -> i32 {
    let r1: i32 = compute_square(a);
    let r2: i32 = compute_cube(b);
    let r3: i32 = compute_fourth(c);
    let r4: i32 = compute_fifth(a);
    return r1 + r2 + r3 + r4;
}

fn main() {
    let result: i32 = complex_chain(2, 3, 4);
    return result;
}
'''
        }

    def benchmark_lexer(self, source: str, iterations: int = 100) -> dict:
        start = time.perf_counter()

        for _ in range(iterations):
            lexer = Lexer(source)
            tokens = lexer.tokenize()

        elapsed = time.perf_counter() - start

        return {
            'time_total_s': elapsed,
            'time_per_iter_ms': (elapsed / iterations) * 1000,
            'tokens_per_second': (len(tokens) * iterations) / elapsed
        }

    def benchmark_parser(self, source: str, iterations: int = 100) -> dict:
        lexer = Lexer(source)
        tokens = lexer.tokenize()

        start = time.perf_counter()

        for _ in range(iterations):
            parser = Parser(tokens, "<bench>")
            ast = parser.parse()

        elapsed = time.perf_counter() - start

        return {
            'time_total_s': elapsed,
            'time_per_iter_ms': (elapsed / iterations) * 1000,
            'ast_nodes_per_second': (len(source) * iterations) / elapsed
        }

    def benchmark_type_checker(self, source: str, iterations: int = 100) -> dict:
        lexer = Lexer(source)
        tokens = lexer.tokenize()
        parser = Parser(tokens, "<bench>")
        ast = parser.parse()

        start = time.perf_counter()

        for _ in range(iterations):
            checker = TypeChecker()
            try:
                checker.check(ast)
            except:
                pass

        elapsed = time.perf_counter() - start
        unif_stats = checker.unification_cache.stats()

        return {
            'time_total_s': elapsed,
            'time_per_iter_ms': (elapsed / iterations) * 1000,
            'unification_hits': unif_stats['hits'],
            'unification_misses': unif_stats['misses'],
            'unification_cache_size': unif_stats['cache_size'],
            'unification_hit_rate': unif_stats['hit_rate_percent']
        }

    def benchmark_codegen(self, source: str, iterations: int = 100) -> dict:
        lexer = Lexer(source)
        tokens = lexer.tokenize()
        parser = Parser(tokens, "<bench>")
        ast = parser.parse()
        checker = TypeChecker()
        try:
            checker.check(ast)
        except:
            pass

        start = time.perf_counter()

        for _ in range(iterations):
            codegen = CodeGen()
            codegen.monomorphic_instances = checker.monomorphic_instances
            try:
                ir = codegen.gen(ast)
            except:
                pass

        elapsed = time.perf_counter() - start
        type_stats = codegen.type_cache.stats()

        return {
            'time_total_s': elapsed,
            'time_per_iter_ms': (elapsed / iterations) * 1000,
            'ir_lines_per_second': (len(ir.split('\n')) * iterations) / elapsed,
            'type_cache_hits': type_stats['hits'],
            'type_cache_misses': type_stats['misses'],
            'type_cache_size': type_stats['cache_size'],
            'type_cache_hit_rate': type_stats['hit_rate_percent']
        }

    def benchmark_full_pipeline(self, source: str, iterations: int = 10) -> dict:
        start = time.perf_counter()

        for _ in range(iterations):
            lexer = Lexer(source)
            tokens = lexer.tokenize()
            parser = Parser(tokens, "<bench>")
            ast = parser.parse()
            checker = TypeChecker()
            try:
                checker.check(ast)
            except:
                pass
            codegen = CodeGen()
            codegen.monomorphic_instances = checker.monomorphic_instances
            try:
                ir = codegen.gen(ast)
            except:
                pass

        elapsed = time.perf_counter() - start

        return {
            'time_total_s': elapsed,
            'time_per_iter_ms': (elapsed / iterations) * 1000,
            'programs_per_second': iterations / elapsed
        }

    def benchmark_character_lookup(self, iterations: int = 1000000) -> dict:
        lookup = CharacterClassLookup()
        test_str = "abcdefghijklmnopqrstuvwxyz0123456789 \t\n"

        start = time.perf_counter()

        for _ in range(iterations):
            for ch in test_str:
                lookup.is_identifier_cont(ch)

        elapsed = time.perf_counter() - start

        return {
            'time_total_s': elapsed,
            'time_per_iter_us': (elapsed / iterations) * 1e6,
            'lookups_per_second': (iterations * len(test_str)) / elapsed
        }

    def run_all(self):
        print("=" * 80)
        print("JOCKY COMPILER PERFORMANCE BENCHMARKS")
        print("=" * 80)

        for prog_name, prog_source in self.test_programs.items():
            print(f"\n{prog_name.upper()} PROGRAM ({len(prog_source)} chars)")
            print("-" * 80)

            try:
                lexer_results = self.benchmark_lexer(prog_source)
                print(f"Lexer:")
                print(f"  {lexer_results['time_per_iter_ms']:.3f} ms/iter")
                print(f"  {lexer_results['tokens_per_second']:.0f} tokens/sec")

                parser_results = self.benchmark_parser(prog_source)
                print(f"\nParser:")
                print(f"  {parser_results['time_per_iter_ms']:.3f} ms/iter")
                print(f"  {parser_results['ast_nodes_per_second']:.0f} chars/sec")

                checker_results = self.benchmark_type_checker(prog_source)
                print(f"\nType Checker:")
                print(f"  {checker_results['time_per_iter_ms']:.3f} ms/iter")
                print(f"  Cache hits: {checker_results['unification_hits']}")
                print(f"  Hit rate: {checker_results['unification_hit_rate']:.1f}%")

                codegen_results = self.benchmark_codegen(prog_source)
                print(f"\nCodegen:")
                print(f"  {codegen_results['time_per_iter_ms']:.3f} ms/iter")
                print(f"  Type cache hit rate: {codegen_results['type_cache_hit_rate']:.1f}%")

                pipeline_results = self.benchmark_full_pipeline(prog_source)
                print(f"\nFull Pipeline:")
                print(f"  {pipeline_results['time_per_iter_ms']:.3f} ms/iter")
                print(f"  {pipeline_results['programs_per_second']:.2f} programs/sec")

            except Exception as e:
                print(f"Error benchmarking {prog_name}: {e}")

        print("\n" + "=" * 80)
        print("CHARACTER LOOKUP OPTIMIZATION")
        print("-" * 80)

        lookup_results = self.benchmark_character_lookup()
        print(f"Character classification (1M iterations):")
        print(f"  {lookup_results['time_per_iter_us']:.3f} µs/char")
        print(f"  {lookup_results['lookups_per_second']:.0f} lookups/sec")

        print("\n" + "=" * 80)


if __name__ == "__main__":
    suite = BenchmarkSuite()
    suite.run_all()
