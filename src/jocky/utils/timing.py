import time
from typing import Optional

class Timer:
    """Context manager that measures elapsed wall time."""

    def __init__(self, label: str = ""):
        self.label = label
        self.elapsed: float = 0.0
        self._start: Optional[float] = None

    def __enter__(self) -> "Timer":
        self._start = time.perf_counter()
        return self

    def __exit__(self, *_):
        self.elapsed = time.perf_counter() - self._start

    def __str__(self) -> str:
        return f"{self.label}: {self.elapsed * 1000:.1f}ms" if self.label else f"{self.elapsed * 1000:.1f}ms"
