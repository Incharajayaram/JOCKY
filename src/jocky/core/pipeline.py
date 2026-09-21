from typing import List
from .stage import Stage
from .context import BuildContext

class Pipeline:
    """Orchestrator: runs stages in order."""
    def __init__(self, stages: List[Stage]):
        self.stages = stages

    def run(self, ctx: BuildContext) -> BuildContext:
        """Run all stages in the pipeline."""
        print("Starting JOCKY Pipeline...")
        
        # Preflight all stages
        for stage in self.stages:
            print(f"[{stage.name}] Running preflight...")
            stage.preflight(ctx)
            
        # Run all stages
        for stage in self.stages:
            print(f"[{stage.name}] Running...")
            ctx = stage.run(ctx)
            
        # Postflight all stages
        for stage in self.stages:
            print(f"[{stage.name}] Running postflight...")
            stage.postflight(ctx)
            
        print("Pipeline finished successfully.")
        return ctx

# Helper function to get the default pipeline (for now empty or dummy)
def get_default_pipeline() -> Pipeline:
    return Pipeline([])
