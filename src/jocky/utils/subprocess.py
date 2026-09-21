import subprocess
import sys
from pathlib import Path
from rich.console import Console

_console = Console()

def run_cmd(cmd: list, step_name: str):
    cmd_str = " ".join(str(c) for c in cmd)
    _console.print(f"  [dim]▸ {step_name}[/dim]")
    _console.print(f"    [dim]{cmd_str}[/dim]")
    try:
        result = subprocess.run(cmd, check=True, capture_output=True, text=True)
        if result.stdout:
            for line in result.stdout.strip().splitlines():
                _console.print(f"    [dim]{line}[/dim]")
    except subprocess.CalledProcessError as e:
        _console.print(f"  [bold red]✗ {step_name} failed[/bold red]")
        if e.stdout:
            for line in e.stdout.strip().splitlines():
                _console.print(f"    [dim]{line}[/dim]")
        if e.stderr:
            for line in e.stderr.strip().splitlines():
                _console.print(f"    [bold red]{line}[/bold red]")
        raise RuntimeError(f"Command failed: {cmd_str}") from e
    except FileNotFoundError as e:
        _console.print(f"  [bold red]✗ Command not found: {cmd[0]}[/bold red]")
        raise
