from typing import List, Optional
from rich.console import Console
from rich.status import Status
from .stage import Stage
from .context import BuildContext

class Pipeline:
    """Orchestrator: runs stages in order with rich console output."""
    def __init__(self, stages: List[Stage], console: Optional[Console] = None):
        self.stages = stages
        self.console = console or Console()

    def run(self, ctx: BuildContext) -> BuildContext:
        total = len(self.stages)
        for i, stage in enumerate(self.stages, 1):
            with self.console.status(
                f"[bold cyan][{i}/{total}][/bold cyan] {stage.name} ...",
                spinner="dots"
            ) as status:
                stage.preflight(ctx)
                ctx = stage.run(ctx)
                stage.postflight(ctx)
                status.update(
                    f"[bold green]✓[/bold green] [bold cyan][{i}/{total}][/bold cyan] {stage.name}"
                )
        return ctx

def get_default_pipeline() -> Pipeline:
    return Pipeline([])
