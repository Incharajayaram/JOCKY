import pytest
import tempfile
import json
from pathlib import Path
from jocky.core.cache import CacheManager, get_cache, init_cache


class TestCacheEntry:
    def test_cache_entry_to_dict(self):
        from jocky.core.cache import CacheEntry

        entry = CacheEntry(
            path="test.jky",
            content_hash="abc123",
            timestamp=1234567890.0,
            metadata={"stage": "lexer"}
        )
        d = entry.to_dict()
        assert d["path"] == "test.jky"
        assert d["content_hash"] == "abc123"

    def test_cache_entry_from_dict(self):
        from jocky.core.cache import CacheEntry

        d = {
            "path": "test.jky",
            "content_hash": "abc123",
            "timestamp": 1234567890.0,
            "metadata": {"stage": "lexer"}
        }
        entry = CacheEntry.from_dict(d)
        assert entry.path == "test.jky"
        assert entry.content_hash == "abc123"


class TestCacheManager:
    @pytest.fixture
    def temp_cache(self):
        with tempfile.TemporaryDirectory() as tmpdir:
            cache = CacheManager(cache_dir=tmpdir)
            yield cache

    @pytest.fixture
    def test_file(self):
        with tempfile.NamedTemporaryFile(mode="w", suffix=".jky", delete=False) as f:
            f.write("fn test() -> i32 { return 0; }")
            f.flush()
            yield f.name
        Path(f.name).unlink()

    def test_cache_manager_init(self, temp_cache):
        assert temp_cache.cache_dir.exists()
        assert (temp_cache.cache_dir / "lexer").exists()
        assert (temp_cache.cache_dir / "parser").exists()

    def test_content_hash(self, temp_cache):
        content = "test content"
        hash1 = temp_cache._content_hash(content)
        hash2 = temp_cache._content_hash(content)
        assert hash1 == hash2
        assert len(hash1) == 64  # SHA256 hex is 64 chars

    def test_file_hash(self, temp_cache, test_file):
        hash1 = temp_cache._file_hash(test_file)
        hash2 = temp_cache._file_hash(test_file)
        assert hash1 == hash2
        assert len(hash1) == 64

    def test_cache_key_generation(self, temp_cache):
        key = temp_cache._cache_key("test.jky", "lexer")
        assert "lexer" in key
        assert "test.jky" in key

    def test_cache_path_generation(self, temp_cache):
        path = temp_cache.get_cache_path("test.jky", "lexer")
        assert path.name.endswith(".pkl")
        assert "lexer" in str(path)

    def test_meta_path_generation(self, temp_cache):
        path = temp_cache.get_meta_path("test.jky", "lexer")
        assert path.name.endswith(".meta.json")
        assert "lexer" in str(path)

    def test_is_cached_not_exists(self, temp_cache, test_file):
        assert not temp_cache.is_cached(test_file, "lexer")

    def test_save_and_load_cache(self, temp_cache, test_file):
        data = {"tokens": [1, 2, 3, 4, 5]}

        # Save cache
        result = temp_cache.save_cache(test_file, "lexer", data)
        assert result is True

        # Verify cache exists
        assert temp_cache.is_cached(test_file, "lexer")

        # Load cache
        loaded = temp_cache.load_cache(test_file, "lexer")
        assert loaded == data

    def test_cache_invalidation_on_file_change(self, temp_cache, test_file):
        data = {"tokens": [1, 2, 3]}

        # Save cache
        temp_cache.save_cache(test_file, "lexer", data)
        assert temp_cache.is_cached(test_file, "lexer")

        # Modify file
        with open(test_file, "w") as f:
            f.write("fn new() -> i32 { return 42; }")

        # Cache should be invalidated
        assert not temp_cache.is_cached(test_file, "lexer")

    def test_cache_stats(self, temp_cache, test_file):
        data = {"test": "data"}

        # Cache miss
        temp_cache.load_cache(test_file, "lexer")
        assert temp_cache.stats["misses"] == 1

        # Save and hit
        temp_cache.save_cache(test_file, "lexer", data)
        temp_cache.load_cache(test_file, "lexer")
        assert temp_cache.stats["hits"] == 1

    def test_get_stats(self, temp_cache, test_file):
        data = {"test": "data"}
        temp_cache.save_cache(test_file, "lexer", data)
        temp_cache.load_cache(test_file, "lexer")  # Hit
        temp_cache.load_cache("nonexistent.jky", "lexer")  # Miss

        stats = temp_cache.get_stats()
        assert stats["hits"] >= 1
        assert stats["misses"] >= 1
        assert "hit_rate" in stats

    def test_clear_stage_cache(self, temp_cache, test_file):
        data = {"tokens": [1, 2, 3]}
        temp_cache.save_cache(test_file, "lexer", data)

        assert temp_cache.is_cached(test_file, "lexer")

        # Clear lexer cache
        result = temp_cache.clear_stage_cache("lexer")
        assert result is True
        assert not temp_cache.is_cached(test_file, "lexer")

    def test_clear_all_cache(self, temp_cache, test_file):
        data = {"tokens": [1, 2, 3]}
        temp_cache.save_cache(test_file, "lexer", data)
        temp_cache.save_cache(test_file, "parser", data)

        assert temp_cache.is_cached(test_file, "lexer")
        assert temp_cache.is_cached(test_file, "parser")

        # Clear all
        result = temp_cache.clear_all_cache()
        assert result is True
        assert not temp_cache.is_cached(test_file, "lexer")
        assert not temp_cache.is_cached(test_file, "parser")

    def test_invalidate_file(self, temp_cache, test_file):
        data = {"tokens": [1, 2, 3]}
        temp_cache.save_cache(test_file, "lexer", data)
        temp_cache.save_cache(test_file, "parser", data)

        # Invalidate
        result = temp_cache.invalidate_file(test_file)
        assert result is True
        assert not temp_cache.is_cached(test_file, "lexer")
        assert not temp_cache.is_cached(test_file, "parser")

    def test_multiple_stages_independent(self, temp_cache, test_file):
        lexer_data = {"stage": "lexer"}
        parser_data = {"stage": "parser"}

        temp_cache.save_cache(test_file, "lexer", lexer_data)
        temp_cache.save_cache(test_file, "parser", parser_data)

        loaded_lexer = temp_cache.load_cache(test_file, "lexer")
        loaded_parser = temp_cache.load_cache(test_file, "parser")

        assert loaded_lexer == lexer_data
        assert loaded_parser == parser_data

    def test_cache_persistence(self, test_file):
        with tempfile.TemporaryDirectory() as tmpdir:
            cache1 = CacheManager(cache_dir=tmpdir)
            data = {"test": "data"}
            cache1.save_cache(test_file, "lexer", data)

            # Create new cache instance (simulates new process)
            cache2 = CacheManager(cache_dir=tmpdir)
            loaded = cache2.load_cache(test_file, "lexer")

            assert loaded == data

    def test_get_cache_global(self):
        cache1 = get_cache()
        cache2 = get_cache()
        assert cache1 is cache2

    def test_init_cache_global(self):
        with tempfile.TemporaryDirectory() as tmpdir:
            cache = init_cache(cache_dir=tmpdir)
            assert cache is get_cache()


class TestCacheIntegration:
    def test_cache_with_large_data(self):
        with tempfile.TemporaryDirectory() as tmpdir:
            with tempfile.NamedTemporaryFile(mode="w", suffix=".jky", delete=False) as f:
                f.write("large content " * 1000)
                f.flush()

                cache = CacheManager(cache_dir=tmpdir)
                large_data = {"tokens": list(range(10000))}

                # Save and load
                cache.save_cache(f.name, "lexer", large_data)
                loaded = cache.load_cache(f.name, "lexer")

                assert loaded == large_data
                Path(f.name).unlink()

    def test_cache_concurrent_stages(self):
        with tempfile.TemporaryDirectory() as tmpdir:
            with tempfile.NamedTemporaryFile(mode="w", suffix=".jky", delete=False) as f:
                f.write("fn test() {}")
                f.flush()

                cache = CacheManager(cache_dir=tmpdir)

                # Save across all stages
                for stage in cache.stages:
                    data = {"stage": stage, "data": "value"}
                    cache.save_cache(f.name, stage, data)

                # All should be cached
                for stage in cache.stages:
                    assert cache.is_cached(f.name, stage)

                Path(f.name).unlink()


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
