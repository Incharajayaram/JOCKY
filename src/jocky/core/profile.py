import yaml
from pathlib import Path
from typing import Dict, Any, Optional

class Profile:
    def __init__(self, data: Dict[str, Any]):
        self.name = data.get("name", "unknown")
        self.description = data.get("description", "")
        self.passes = data.get("passes", [])
        self.polymorphic = data.get("polymorphic", False)
        self.seed = data.get("seed", 0)
        self.optimization_level = data.get("optimization_level", 2)
        self.packing = data.get("packing", {})

    @property
    def packing_enabled(self) -> bool:
        return self.packing.get("enabled", False)

    @property
    def packing_tool(self) -> str:
        return self.packing.get("tool", "upx")

    @property
    def packing_args(self) -> list:
        return self.packing.get("args", [])

def load_profile(name: str, profile_dir: Optional[Path] = None) -> Profile:
    if profile_dir is None:
        profile_dir = Path(__file__).parent.parent / "passes" / "profiles"
    profile_path = profile_dir / f"{name}.yaml"
    if not profile_path.exists():
        raise FileNotFoundError(f"Profile not found: {profile_path}")
    with open(profile_path, "r") as f:
        data = yaml.safe_load(f)
    return Profile(data)

def list_profiles(profile_dir: Optional[Path] = None) -> list:
    if profile_dir is None:
        profile_dir = Path(__file__).parent.parent / "passes" / "profiles"
    profiles = []
    for p in sorted(profile_dir.glob("*.yaml")):
        with open(p, "r") as f:
            data = yaml.safe_load(f)
        profiles.append({
            "name": data.get("name", p.stem),
            "file": p.name,
            "description": data.get("description", ""),
        })
    return profiles
