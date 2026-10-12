from jocky.core.stage import Stage
from jocky.core.context import BuildContext
from jocky.utils.subprocess import run_cmd
import shutil
from pathlib import Path
from rich.console import Console

_console = Console()

def _find_tool(name: str) -> str | None:
    found = shutil.which(name)
    if found:
        return found
    toolchain_path = Path(__file__).resolve().parents[3] / "toolchain" / "bin" / name
    if toolchain_path.is_file():
        return str(toolchain_path)
    return None

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

        tool_path = _find_tool(profile.packing_tool)
        if not tool_path:
            _console.print(f"  [bold yellow]⚠[/bold yellow] Packing tool '{profile.packing_tool}' not found, skipping.")
            return ctx

        args = [tool_path] + profile.packing_args + [str(exe)]
        run_cmd(args, "Packing executable")
        return ctx
