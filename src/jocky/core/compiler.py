"""Compilation pipeline with incremental caching support."""

from typing import Optional, Tuple
from pathlib import Path
from jocky.language.lexer import Lexer
from jocky.language.parser import Parser, parse_source
from jocky.language.checker import TypeChecker
from jocky.language.codegen import CodeGen
from jocky.language.ast import Program
from jocky.core.cache import CacheManager, get_cache


class CachingCompiler:
    """Compiler wrapper that adds incremental caching to the pipeline."""

    def __init__(self, cache_manager: Optional[CacheManager] = None, use_cache: bool = True, target_platform: str = "windows"):
        self.cache = cache_manager or get_cache()
        self.use_cache = use_cache
        self.target_platform = target_platform

    def lex_with_cache(self, source: str, file_path: Optional[str] = None) -> Tuple[list, bool]:
        """
        Tokenize source, using cache if available.

        Returns:
            Tuple of (tokens, from_cache)
        """
        if not self.use_cache or not file_path:
            lexer = Lexer(source)
            return lexer.tokenize(), False

        # Try to load from cache
        cached_tokens = self.cache.load_cache(file_path, "lexer")
        if cached_tokens is not None:
            return cached_tokens, True

        # Not cached, tokenize
        lexer = Lexer(source)
        tokens = lexer.tokenize()

        # Save to cache
        self.cache.save_cache(file_path, "lexer", tokens)
        return tokens, False

    def parse_with_cache(self, tokens: list, file_path: Optional[str] = None) -> Tuple[Program, bool]:
        """
        Parse tokens to AST, using cache if available.

        Returns:
            Tuple of (ast, from_cache)
        """
        if not self.use_cache or not file_path:
            parser = Parser(tokens)
            return parser.parse(), False

        # Try to load from cache
        cached_ast = self.cache.load_cache(file_path, "parser")
        if cached_ast is not None:
            return cached_ast, True

        # Not cached, parse
        parser = Parser(tokens)
        ast = parser.parse()

        # Save to cache
        self.cache.save_cache(file_path, "parser", ast)
        return ast, False

    def check_with_cache(self, ast: Program, file_path: Optional[str] = None) -> Tuple[TypeChecker, bool]:
        """
        Type check AST, using cache if available.

        Returns:
            Tuple of (type_checker, from_cache)
        """
        if not self.use_cache or not file_path:
            checker = TypeChecker()
            checker.check(ast)
            return checker, False

        # Try to load cached type info
        cached_checker = self.cache.load_cache(file_path, "checker")
        if cached_checker is not None:
            return cached_checker, True

        # Not cached, type check
        checker = TypeChecker()
        checker.check(ast)

        # Save to cache
        self.cache.save_cache(file_path, "checker", checker)
        return checker, False

    def codegen_with_cache(self, ast: Program, checker: TypeChecker, file_path: Optional[str] = None) -> Tuple[str, bool]:
        """
        Generate LLVM IR, using cache if available.

        Returns:
            Tuple of (ir, from_cache)
        """
        if not self.use_cache or not file_path:
            codegen = CodeGen(target_platform=self.target_platform)
            # Pre-populate codegen with type info
            codegen.structs = checker.structs
            codegen.enums = checker.enums
            codegen.functions = checker.functions
            return codegen.gen(ast), False

        # Try to load from cache
        cached_ir = self.cache.load_cache(file_path, "codegen")
        if cached_ir is not None:
            return cached_ir, True

        # Not cached, generate IR
        codegen = CodeGen(target_platform=self.target_platform)
        codegen.structs = checker.structs
        codegen.enums = checker.enums
        codegen.functions = checker.functions
        ir = codegen.gen(ast)

        # Save to cache
        self.cache.save_cache(file_path, "codegen", ir)
        return ir, False

    def compile_file_with_cache(self, file_path: str) -> Tuple[str, dict]:
        """
        Compile a file with full caching support.

        Returns:
            Tuple of (ir, stats)
        """
        with open(file_path, "r", encoding="utf-8") as f:
            source = f.read()

        stats = {"cache_hits": 0, "cache_misses": 0}

        # Lexing
        tokens, lexer_cached = self.lex_with_cache(source, file_path)
        stats["cache_hits" if lexer_cached else "cache_misses"] += 1

        # Parsing
        ast, parser_cached = self.parse_with_cache(tokens, file_path)
        stats["cache_hits" if parser_cached else "cache_misses"] += 1

        # Type checking
        checker, checker_cached = self.check_with_cache(ast, file_path)
        stats["cache_hits" if checker_cached else "cache_misses"] += 1

        # Code generation
        ir, codegen_cached = self.codegen_with_cache(ast, checker, file_path)
        stats["cache_hits" if codegen_cached else "cache_misses"] += 1

        return ir, stats

    def clear_cache(self, stage: Optional[str] = None) -> bool:
        """Clear cache for a specific stage or all stages."""
        if stage:
            return self.cache.clear_stage_cache(stage)
        return self.cache.clear_all_cache()

    def get_cache_stats(self) -> dict:
        """Get cache statistics."""
        return self.cache.get_stats()

    def print_cache_stats(self):
        """Print formatted cache statistics."""
        self.cache.print_stats()


# Global compiler instance
_compiler_instance: Optional[CachingCompiler] = None


def get_compiler(use_cache: bool = True) -> CachingCompiler:
    """Get or create global compiler instance."""
    global _compiler_instance
    if _compiler_instance is None:
        _compiler_instance = CachingCompiler(use_cache=use_cache)
    return _compiler_instance


def init_compiler(cache_manager: Optional[CacheManager] = None, use_cache: bool = True) -> CachingCompiler:
    """Initialize global compiler instance."""
    global _compiler_instance
    _compiler_instance = CachingCompiler(cache_manager, use_cache)
    return _compiler_instance
