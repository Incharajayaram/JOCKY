from pathlib import Path
from typing import List, Iterator
import shutil

def ensure_dir(path: Path) -> Path:
    path.mkdir(parents=True, exist_ok=True)
    return path

def clean_dir(path: Path) -> Path:
    if path.exists():
        shutil.rmtree(path)
    path.mkdir(parents=True, exist_ok=True)
    return path

def find_files(root: Path, pattern: str = "*") -> Iterator[Path]:
    yield from root.rglob(pattern)
