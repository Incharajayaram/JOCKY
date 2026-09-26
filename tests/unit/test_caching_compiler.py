import pytest
import tempfile
from pathlib import Path
from jocky.core.compiler import CachingCompiler, get_compiler, init_compiler
from jocky.core.cache import CacheManager
from jocky.language.parser import parse_source
from jocky.language.checker import TypeChecker
from jocky.language.codegen import CodeGen


class TestCachingCompiler:
    @pytest.fixture
    def temp_cache(self):
        with tempfile.TemporaryDirectory() as tmpdir:
            cache = CacheManager(cache_dir=tmpdir)
            yield cache

    @pytest.fixture
    def compiler(self, temp_cache):
        return CachingCompiler(cache_manager=temp_cache, use_cache=True)

    @pytest.fixture
    def test_file(self):
        with tempfile.NamedTemporaryFile(mode="w", suffix=".jky", delete=False) as f:
            f.write("fn main() -> i32 { return 0; }")
            f.flush()
            yield f.name
        Path(f.name).unlink()

    def test_compiler_init(self, compiler):
        assert compiler is not None
        assert compiler.use_cache is True
        assert compiler.cache is not None

    def test_lex_with_cache_first_time(self, compiler, test_file):
        with open(test_file) as f:
            source = f.read()

        tokens, from_cache = compiler.lex_with_cache(source, test_file)
        assert tokens is not None
        assert from_cache is False
        assert len(tokens) > 0

    def test_lex_with_cache_hit(self, compiler, test_file):
        with open(test_file) as f:
            source = f.read()

        # First call - cache miss
        tokens1, from_cache1 = compiler.lex_with_cache(source, test_file)
        assert from_cache1 is False

        # Second call - should hit cache
        tokens2, from_cache2 = compiler.lex_with_cache(source, test_file)
        assert from_cache2 is True
        assert len(tokens1) == len(tokens2)

    def test_parse_with_cache_first_time(self, compiler, test_file):
        with open(test_file) as f:
            source = f.read()

        tokens, _ = compiler.lex_with_cache(source, test_file)
        ast, from_cache = compiler.parse_with_cache(tokens, test_file)

        assert ast is not None
        assert from_cache is False

    def test_parse_with_cache_hit(self, compiler, test_file):
        with open(test_file) as f:
            source = f.read()

        tokens, _ = compiler.lex_with_cache(source, test_file)

        # First parse
        ast1, from_cache1 = compiler.parse_with_cache(tokens, test_file)
        assert from_cache1 is False

        # Second parse with same tokens
        ast2, from_cache2 = compiler.parse_with_cache(tokens, test_file)
        assert from_cache2 is True

    def test_check_with_cache(self, compiler, test_file):
        with open(test_file) as f:
            source = f.read()

        tokens, _ = compiler.lex_with_cache(source, test_file)
        ast, _ = compiler.parse_with_cache(tokens, test_file)

        # First check
        checker1, from_cache1 = compiler.check_with_cache(ast, test_file)
        assert from_cache1 is False
        assert checker1.functions is not None

        # Second check
        checker2, from_cache2 = compiler.check_with_cache(ast, test_file)
        assert from_cache2 is True

    def test_codegen_with_cache(self, compiler, test_file):
        with open(test_file) as f:
            source = f.read()

        tokens, _ = compiler.lex_with_cache(source, test_file)
        ast, _ = compiler.parse_with_cache(tokens, test_file)
        checker, _ = compiler.check_with_cache(ast, test_file)

        # First codegen
        ir1, from_cache1 = compiler.codegen_with_cache(ast, checker, test_file)
        assert from_cache1 is False
        assert len(ir1) > 0

        # Second codegen
        ir2, from_cache2 = compiler.codegen_with_cache(ast, checker, test_file)
        assert from_cache2 is True
        assert ir1 == ir2

    def test_compile_file_with_cache(self, compiler, test_file):
        # First compilation
        ir1, stats1 = compiler.compile_file_with_cache(test_file)
        assert ir1 is not None
        assert len(ir1) > 0
        assert stats1["cache_misses"] == 4  # All stages miss first time
        assert stats1["cache_hits"] == 0

        # Second compilation - should hit all caches
        ir2, stats2 = compiler.compile_file_with_cache(test_file)
        assert ir1 == ir2
        assert stats2["cache_hits"] == 4  # All stages hit
        assert stats2["cache_misses"] == 0

    def test_cache_invalidation_on_source_change(self, compiler, test_file):
        # First compilation
        ir1, stats1 = compiler.compile_file_with_cache(test_file)
        assert stats1["cache_misses"] == 4

        # Modify file
        with open(test_file, "w") as f:
            f.write("fn main() -> i32 { return 42; }")

        # Second compilation - cache should be invalid
        ir2, stats2 = compiler.compile_file_with_cache(test_file)
        assert ir1 != ir2
        assert stats2["cache_misses"] == 4  # All miss due to source change

    def test_no_cache_mode(self, temp_cache, test_file):
        compiler = CachingCompiler(cache_manager=temp_cache, use_cache=False)

        with open(test_file) as f:
            source = f.read()

        # First call
        tokens1, from_cache1 = compiler.lex_with_cache(source, test_file)
        assert from_cache1 is False

        # Second call should also miss (cache disabled)
        tokens2, from_cache2 = compiler.lex_with_cache(source, test_file)
        assert from_cache2 is False

    def test_cache_stats(self, compiler, test_file):
        with open(test_file) as f:
            source = f.read()

        # Compile twice
        compiler.compile_file_with_cache(test_file)
        compiler.compile_file_with_cache(test_file)

        stats = compiler.get_cache_stats()
        assert stats["hits"] == 4
        assert stats["misses"] == 4
        assert "hit_rate" in stats

    def test_clear_cache(self, compiler, test_file):
        with open(test_file) as f:
            source = f.read()

        # Compile to populate cache
        tokens, _ = compiler.lex_with_cache(source, test_file)

        # Clear lexer cache
        compiler.clear_cache("lexer")

        # Next lex should miss
        tokens2, from_cache = compiler.lex_with_cache(source, test_file)
        assert from_cache is False

    def test_global_compiler_instance(self):
        compiler1 = get_compiler()
        compiler2 = get_compiler()
        assert compiler1 is compiler2

    def test_init_compiler_global(self):
        with tempfile.TemporaryDirectory() as tmpdir:
            cache = CacheManager(cache_dir=tmpdir)
            compiler = init_compiler(cache_manager=cache)
            assert compiler is get_compiler()


class TestCachingCompilerIntegration:
    def test_full_pipeline_with_complex_program(self):
        with tempfile.TemporaryDirectory() as tmpdir:
            with tempfile.NamedTemporaryFile(mode="w", suffix=".jky", delete=False, dir=tmpdir) as f:
                f.write("""
enum Status { OK = 0, Error = 1 };

fn check(code: i32) -> i32 {
    match code {
        0 => 100,
        1 => 200,
        _ => 999
    }
}

fn main() -> i32 {
    let s: Status = OK;
    return check(0);
}
""")
                f.flush()

                cache = CacheManager(cache_dir=tmpdir)
                compiler = CachingCompiler(cache_manager=cache, use_cache=True)

                # Compile multiple times
                ir1, stats1 = compiler.compile_file_with_cache(f.name)
                ir2, stats2 = compiler.compile_file_with_cache(f.name)

                # First should miss all, second should hit all
                assert stats1["cache_misses"] == 4
                assert stats2["cache_hits"] == 4
                assert ir1 == ir2

                Path(f.name).unlink()

    def test_compiler_with_type_errors(self):
        with tempfile.TemporaryDirectory() as tmpdir:
            with tempfile.NamedTemporaryFile(mode="w", suffix=".jky", delete=False, dir=tmpdir) as f:
                f.write("fn main() { let x: i32 = true; }")  # Type error
                f.flush()

                cache = CacheManager(cache_dir=tmpdir)
                compiler = CachingCompiler(cache_manager=cache, use_cache=True)

                # Should raise TypeError
                with pytest.raises(Exception):
                    compiler.compile_file_with_cache(f.name)

                Path(f.name).unlink()


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
