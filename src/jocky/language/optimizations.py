"""Performance optimization utilities for JOCKY compiler pipeline.

Provides caching, lookup tables, and buffering mechanisms to reduce
compilation time across lexer, parser, type checker, and codegen stages.
"""

from typing import Dict, Tuple, Optional, Any, Set
from functools import lru_cache
import array


class CharacterClassLookup:
    """Fast character classification using lookup tables instead of string checks."""

    def __init__(self):
        self.whitespace = bytearray(256)
        self.digit = bytearray(256)
        self.hex_digit = bytearray(256)
        self.alpha = bytearray(256)
        self.alnum = bytearray(256)
        self.identifier_start = bytearray(256)
        self.identifier_cont = bytearray(256)

        for i in range(256):
            ch = chr(i)
            self.whitespace[i] = 1 if ch in ' \t\r\n' else 0
            self.digit[i] = 1 if ch.isdigit() else 0
            self.hex_digit[i] = 1 if ch in '0123456789abcdefABCDEF' else 0
            self.alpha[i] = 1 if ch.isalpha() else 0
            self.alnum[i] = 1 if ch.isalnum() else 0
            self.identifier_start[i] = 1 if ch.isalpha() or ch == '_' else 0
            self.identifier_cont[i] = 1 if ch.isalnum() or ch == '_' else 0

    def is_whitespace(self, ch: str) -> bool:
        return self.whitespace[ord(ch[0])] if ch else False

    def is_digit(self, ch: str) -> bool:
        return self.digit[ord(ch[0])] if ch else False

    def is_hex_digit(self, ch: str) -> bool:
        return self.hex_digit[ord(ch[0])] if ch else False

    def is_alpha(self, ch: str) -> bool:
        return self.alpha[ord(ch[0])] if ch else False

    def is_alnum(self, ch: str) -> bool:
        return self.alnum[ord(ch[0])] if ch else False

    def is_identifier_start(self, ch: str) -> bool:
        return self.identifier_start[ord(ch[0])] if ch else False

    def is_identifier_cont(self, ch: str) -> bool:
        return self.identifier_cont[ord(ch[0])] if ch else False


class TokenPositionCache:
    """Cache token position calculations to avoid redundant work."""

    def __init__(self):
        self.positions: Dict[int, Tuple[int, int]] = {}

    def cache_position(self, token_index: int, line: int, column: int):
        self.positions[token_index] = (line, column)

    def get_position(self, token_index: int) -> Optional[Tuple[int, int]]:
        return self.positions.get(token_index)


class LookaheadCache:
    """Cache parser lookahead results for frequently parsed patterns."""

    def __init__(self, max_size: int = 10000):
        self.cache: Dict[Tuple[int, int], Any] = {}
        self.max_size = max_size

    def get(self, pos: int, lookahead_depth: int) -> Optional[Any]:
        key = (pos, lookahead_depth)
        return self.cache.get(key)

    def set(self, pos: int, lookahead_depth: int, result: Any):
        if len(self.cache) < self.max_size:
            key = (pos, lookahead_depth)
            self.cache[key] = result

    def clear(self):
        self.cache.clear()


class TypeUnificationCache:
    """Cache type unification results to avoid redundant type checking."""

    def __init__(self):
        self.cache: Dict[Tuple[str, str], bool] = {}
        self.hits = 0
        self.misses = 0

    def get(self, type1: str, type2: str) -> Optional[bool]:
        key = (type1, type2) if type1 <= type2 else (type2, type1)
        result = self.cache.get(key)
        if result is not None:
            self.hits += 1
        return result

    def set(self, type1: str, type2: str, result: bool):
        key = (type1, type2) if type1 <= type2 else (type2, type1)
        self.cache[key] = result

    def stats(self) -> Dict[str, int]:
        total = self.hits + self.misses
        hit_rate = (self.hits / total * 100) if total > 0 else 0
        return {
            'hits': self.hits,
            'misses': self.misses,
            'total': total,
            'hit_rate_percent': hit_rate,
            'cache_size': len(self.cache)
        }


class FastSymbolTable:
    """Symbol table with O(1) lookup and fast iteration."""

    def __init__(self):
        self.symbols: Dict[str, Any] = {}
        self.scopes: list[Dict[str, Any]] = [{}]

    def push_scope(self):
        self.scopes.append({})

    def pop_scope(self):
        if len(self.scopes) > 1:
            self.scopes.pop()

    def define(self, name: str, value: Any):
        self.scopes[-1][name] = value

    def lookup(self, name: str) -> Optional[Any]:
        for scope in reversed(self.scopes):
            if name in scope:
                return scope[name]
        return None

    def lookup_local(self, name: str) -> Optional[Any]:
        return self.scopes[-1].get(name)

    def all_symbols(self) -> Dict[str, Any]:
        result = {}
        for scope in self.scopes:
            result.update(scope)
        return result


class IREmissionBuffer:
    """Buffer IR emissions for batch writing, reducing system calls."""

    def __init__(self, buffer_size: int = 16384):
        self.buffer: list[str] = []
        self.buffer_size = buffer_size
        self.output: list[str] = []
        self.size = 0

    def emit(self, line: str):
        self.buffer.append(line)
        self.size += len(line) + 1  # +1 for newline

        if self.size > self.buffer_size:
            self.flush()

    def emit_multiple(self, lines: list[str]):
        for line in lines:
            self.emit(line)

    def flush(self):
        if self.buffer:
            self.output.extend(self.buffer)
            self.buffer.clear()
            self.size = 0

    def get_all(self) -> list[str]:
        self.flush()
        return self.output

    def clear(self):
        self.buffer.clear()
        self.output.clear()
        self.size = 0


class FunctionDeduplicator:
    """Detect and deduplicate identical generated functions."""

    def __init__(self):
        self.function_hashes: Dict[str, int] = {}
        self.hash_to_name: Dict[int, str] = {}
        self.duplicates: Set[str] = set()

    def register_function(self, name: str, code_hash: int):
        self.function_hashes[name] = code_hash

        if code_hash in self.hash_to_name:
            self.duplicates.add(name)
        else:
            self.hash_to_name[code_hash] = name

    def is_duplicate(self, name: str) -> bool:
        return name in self.duplicates

    def get_canonical_name(self, name: str) -> Optional[str]:
        if name not in self.function_hashes:
            return None

        code_hash = self.function_hashes[name]
        return self.hash_to_name.get(code_hash)


class LLVMTypeCache:
    """Cache LLVM type string generation to avoid recomputation."""

    def __init__(self):
        self.type_cache: Dict[str, str] = {}
        self.hits = 0
        self.misses = 0

    def get_llvm_type(self, type_name: str) -> Optional[str]:
        result = self.type_cache.get(type_name)
        if result is not None:
            self.hits += 1
        else:
            self.misses += 1
        return result

    def cache_type(self, type_name: str, llvm_type: str):
        self.type_cache[type_name] = llvm_type

    def stats(self) -> Dict[str, Any]:
        total = self.hits + self.misses
        hit_rate = (self.hits / total * 100) if total > 0 else 0
        return {
            'hits': self.hits,
            'misses': self.misses,
            'total': total,
            'hit_rate_percent': hit_rate,
            'cache_size': len(self.type_cache)
        }


class FastMemoryPool:
    """Object pool for frequently allocated small strings and buffers."""

    def __init__(self, initial_pool_size: int = 1000):
        self.string_pool: list[bytearray] = []
        self.buffer_pool: list[bytearray] = []
        self.initial_size = initial_pool_size

        for _ in range(initial_pool_size // 10):
            self.string_pool.append(bytearray(256))
        for _ in range(initial_pool_size // 10):
            self.buffer_pool.append(bytearray(4096))

    def get_string_buffer(self) -> bytearray:
        if self.string_pool:
            buf = self.string_pool.pop()
            buf.clear()
            return buf
        return bytearray(256)

    def return_string_buffer(self, buf: bytearray):
        if len(self.string_pool) < self.initial_size // 10:
            buf.clear()
            self.string_pool.append(buf)

    def get_buffer(self) -> bytearray:
        if self.buffer_pool:
            buf = self.buffer_pool.pop()
            buf.clear()
            return buf
        return bytearray(4096)

    def return_buffer(self, buf: bytearray):
        if len(self.buffer_pool) < self.initial_size // 10:
            buf.clear()
            self.buffer_pool.append(buf)


class BytecodeCompressionOptimizer:
    """Compress bytecode to reduce generated code size."""

    @staticmethod
    def compress_opcodes(opcodes: list[int]) -> bytes:
        """Simple compression: run-length encode repeated opcodes."""
        if not opcodes:
            return b''

        result = bytearray()
        i = 0

        while i < len(opcodes):
            opcode = opcodes[i]
            count = 1

            while i + count < len(opcodes) and opcodes[i + count] == opcode and count < 255:
                count += 1

            if count > 2:
                result.append(0xFF)  # Compression marker
                result.append(opcode)
                result.append(count)
                i += count
            else:
                for _ in range(count):
                    result.append(opcode)
                i += count

        return bytes(result)

    @staticmethod
    def decompress_opcodes(compressed: bytes) -> list[int]:
        """Decompress run-length encoded bytecode."""
        opcodes = []
        i = 0

        while i < len(compressed):
            if compressed[i] == 0xFF and i + 2 < len(compressed):
                opcode = compressed[i + 1]
                count = compressed[i + 2]
                opcodes.extend([opcode] * count)
                i += 3
            else:
                opcodes.append(compressed[i])
                i += 1

        return opcodes
