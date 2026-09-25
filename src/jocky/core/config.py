from pathlib import Path
from typing import Any, Dict, Optional
import json

_DEFAULT_CONFIG: Dict[str, Any] = {
    "profile": "release",
    "target": "native",
    "no_prelude": False,
    "no_runtime": False,
    "obfuscate": True,
}

def load_config(path: Optional[Path] = None) -> Dict[str, Any]:
    """Load build config from a JSON file, falling back to defaults."""
    cfg = dict(_DEFAULT_CONFIG)
    if path is None:
        candidates = [Path("jocky.json"), Path(".jocky.json")]
        for c in candidates:
            if c.exists():
                path = c
                break
    if path is not None and path.exists():
        with path.open() as f:
            overrides = json.load(f)
        cfg.update(overrides)
    return cfg
