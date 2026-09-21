from jocky.core.stage import Stage
from jocky.core.context import BuildContext
from jocky.utils.subprocess import run_cmd
import shutil
from rich.console import Console

_console = Console()

class PackStage(Stage):
    @property
    def name(self) -> str:
        return "pack"

    def run(self, ctx: BuildContext) -> BuildContext:
        profile = ctx.state.get("profile")
        if not profile or not profile.packing_enabled:
            return ctx

        exe = ctx.state.get("executable")
        if not exe:
            return ctx

        tool = profile.packing_tool
        if not shutil.which(tool):
            _console.print(f"  [bold yellow]⚠[/bold yellow] Packing tool '{tool}' not found, skipping.")
            return ctx

        args = [tool] + profile.packing_args + [str(exe)]
        run_cmd(args, "Packing executable")
        return ctx
