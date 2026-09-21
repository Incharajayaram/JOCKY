from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Dict

@dataclass
class BuildContext:
    """
    Carries paths, config, and intermediate artifact references between stages.
    """
    input_file: Path
    profile_name: str
    output_dir: Path
    config: Dict[str, Any] = field(default_factory=dict)
    state: Dict[str, Any] = field(default_factory=dict)
    
    def get_stage_output_dir(self, stage_name: str) -> Path:
        """Get the directory where a specific stage should write its intermediate files."""
        stage_dir = self.output_dir / stage_name
        stage_dir.mkdir(parents=True, exist_ok=True)
        return stage_dir
