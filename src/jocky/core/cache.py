import hashlib
import json
import pickle
import os
from pathlib import Path
from typing import Any, Optional, Dict, Tuple
from dataclasses import dataclass
from datetime import datetime


@dataclass
class CacheEntry:
    path: str
    content_hash: str
    timestamp: float
    metadata: Dict[str, Any]

    def to_dict(self):
        return {
            "path": self.path,
            "content_hash": self.content_hash,
            "timestamp": self.timestamp,
            "metadata": self.metadata,
        }

    @staticmethod
    def from_dict(d):
        return CacheEntry(
            path=d["path"],
            content_hash=d["content_hash"],
            timestamp=d["timestamp"],
            metadata=d["metadata"],
        )


class CacheManager:
    def __init__(self, cache_dir: str = ".jocky-build/.cache"):
        self.cache_dir = Path(cache_dir)
        self.cache_dir.mkdir(parents=True, exist_ok=True)

        self.stages = ["lexer", "parser", "checker", "codegen"]
        for stage in self.stages:
            (self.cache_dir / stage).mkdir(exist_ok=True)

        self.stats = {
            "hits": 0,
            "misses": 0,
            "stores": 0,
        }

    def _content_hash(self, content: str) -> str:
        return hashlib.sha256(content.encode()).hexdigest()

    def _file_hash(self, file_path: str) -> str:
        with open(file_path, "r", encoding="utf-8", errors="ignore") as f:
            return self._content_hash(f.read())

    def _cache_key(self, file_path: str, stage: str) -> str:
        file_name = Path(file_path).name
        return f"{stage}_{file_name}"

    def get_cache_path(self, file_path: str, stage: str, ext: str = ".pkl") -> Path:
        key = self._cache_key(file_path, stage)
        return self.cache_dir / stage / (key + ext)

    def get_meta_path(self, file_path: str, stage: str) -> Path:
        key = self._cache_key(file_path, stage)
        return self.cache_dir / stage / (key + ".meta.json")

    def is_cached(self, file_path: str, stage: str) -> bool:
        cache_path = self.get_cache_path(file_path, stage)
        meta_path = self.get_meta_path(file_path, stage)

        if not cache_path.exists() or not meta_path.exists():
            return False

        try:
            with open(meta_path, "r") as f:
                meta = json.load(f)

            current_hash = self._file_hash(file_path)
            return meta.get("content_hash") == current_hash
        except Exception:
            return False

    def load_cache(self, file_path: str, stage: str) -> Optional[Any]:
        if not self.is_cached(file_path, stage):
            self.stats["misses"] += 1
            return None

        try:
            cache_path = self.get_cache_path(file_path, stage)
            with open(cache_path, "rb") as f:
                data = pickle.load(f)
            self.stats["hits"] += 1
            return data
        except Exception as e:
            print(f"Warning: Failed to load cache for {file_path}: {e}")
            self.stats["misses"] += 1
            return None

    def save_cache(self, file_path: str, stage: str, data: Any) -> bool:
        try:
            cache_path = self.get_cache_path(file_path, stage)
            meta_path = self.get_meta_path(file_path, stage)

            # Save cache data
            with open(cache_path, "wb") as f:
                pickle.dump(data, f)

            # Save metadata
            content_hash = self._file_hash(file_path)
            metadata = {
                "content_hash": content_hash,
                "timestamp": datetime.now().isoformat(),
                "stage": stage,
            }
            with open(meta_path, "w") as f:
                json.dump(metadata, f)

            self.stats["stores"] += 1
            return True
        except Exception as e:
            print(f"Warning: Failed to save cache for {file_path}: {e}")
            return False

    def clear_stage_cache(self, stage: str) -> bool:
        try:
            stage_dir = self.cache_dir / stage
            if stage_dir.exists():
                for file in stage_dir.glob("*"):
                    file.unlink()
            return True
        except Exception as e:
            print(f"Warning: Failed to clear {stage} cache: {e}")
            return False

    def clear_all_cache(self) -> bool:
        try:
            for stage in self.stages:
                self.clear_stage_cache(stage)
            return True
        except Exception as e:
            print(f"Warning: Failed to clear cache: {e}")
            return False

    def invalidate_file(self, file_path: str) -> bool:
        try:
            for stage in self.stages:
                cache_path = self.get_cache_path(file_path, stage)
                meta_path = self.get_meta_path(file_path, stage)

                if cache_path.exists():
                    cache_path.unlink()
                if meta_path.exists():
                    meta_path.unlink()
            return True
        except Exception as e:
            print(f"Warning: Failed to invalidate cache for {file_path}: {e}")
            return False

    def get_stats(self) -> Dict[str, Any]:
        total = self.stats["hits"] + self.stats["misses"]
        hit_rate = (self.stats["hits"] / total * 100) if total > 0 else 0

        return {
            "hits": self.stats["hits"],
            "misses": self.stats["misses"],
            "stores": self.stats["stores"],
            "total_lookups": total,
            "hit_rate": f"{hit_rate:.1f}%",
        }

    def print_stats(self):
        stats = self.get_stats()
        print(f"\n🚀 Cache Statistics:")
        print(f"  Hits:         {stats['hits']}")
        print(f"  Misses:       {stats['misses']}")
        print(f"  Hit Rate:     {stats['hit_rate']}")
        print(f"  Cached Items: {stats['stores']}")


# Global cache instance
_cache_instance: Optional[CacheManager] = None


def get_cache() -> CacheManager:
    global _cache_instance
    if _cache_instance is None:
        _cache_instance = CacheManager()
    return _cache_instance


def init_cache(cache_dir: str = ".jocky-build/.cache") -> CacheManager:
    global _cache_instance
    _cache_instance = CacheManager(cache_dir)
    return _cache_instance
