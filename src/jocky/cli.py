import sys
import shutil
import subprocess
from pathlib import Path
from typing import Optional

import click
from rich.console import Console
from rich.table import Table
from rich.panel import Panel
from rich.text import Text
from rich import box

from jocky.core.context import BuildContext
from jocky.core.pipeline import Pipeline
from jocky.core.profile import load_profile, list_profiles
from jocky.stages.parse import ParseStage
from jocky.stages.lower_ir import LowerIRStage
from jocky.stages.mlir_obfuscate import MLIRObfuscateStage
from jocky.stages.ir_obfuscate import IRObfuscateStage
from jocky.stages.link import LinkStage
from jocky.stages.pack import PackStage
from jocky.passes.registry import PASS_REGISTRY

console = Console()

def get_pipeline() -> Pipeline:
    return Pipeline([
        ParseStage(),
        LowerIRStage(),
        MLIRObfuscateStage(),
        IRObfuscateStage(),
        LinkStage(),
        PackStage(),
    ])

# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def resolve_profile(name: Optional[str]):
    if name:
        return load_profile(name)
    try:
        return load_profile("standard")
    except FileNotFoundError:
        return None

def header(title: str):
    console.print()
    console.rule(f"[bold cyan]{title}[/bold cyan]")

def success(msg: str):
    console.print(f"[bold green]✓[/bold green] {msg}")

def error(msg: str):
    console.print(f"[bold red]✗[/bold red] {msg}")

def warn(msg: str):
    console.print(f"[bold yellow]⚠[/bold yellow] {msg}")

def info_line(label: str, value: str):
    console.print(f"  [dim]{label}[/dim] {value}")

# ---------------------------------------------------------------------------
# CLI Group
# ---------------------------------------------------------------------------

@click.group(invoke_without_command=False)
@click.version_option(version="0.1.0", prog_name="jocky")
def cli():
    """JOCKY — Compiler & Obfuscation Pipeline

    Build native executables from JOCKY source with integrated LLVM/MLIR
    obfuscation passes. Run ./jocky COMMAND --help for details.
    """
    pass

# ---------------------------------------------------------------------------
# build
# ---------------------------------------------------------------------------

@cli.command()
@click.argument("file", type=click.Path(exists=True, dir_okay=False))
@click.option("--profile", "-p", default=None, help="Obfuscation profile (none, light, standard, aggressive, paranoid)")
@click.option("--output", "-o", default=None, help="Output executable path")
@click.option("--keep-intermediates", is_flag=True, help="Keep intermediate build files")
def build(file: str, profile: Optional[str], output: Optional[str], keep_intermediates: bool):
    """Build a JOCKY source file into a native executable."""
    input_path = Path(file)
    build_dir = input_path.parent / ".jocky-build"
    build_dir.mkdir(exist_ok=True)

    prof = resolve_profile(profile)
    config = {}
    if output:
        config["output"] = output

    ctx = BuildContext(
        input_file=input_path,
        profile_name=profile or (prof.name if prof else "none"),
        output_dir=build_dir,
        config=config,
    )
    ctx.state["profile"] = prof

    header("JOCKY Build")
    console.print(f"  [dim]Source[/dim]     {input_path}")
    console.print(f"  [dim]Profile[/dim]    {ctx.profile_name}")
    console.print(f"  [dim]Build dir[/dim]  {build_dir}")
    console.print()

    pipeline = get_pipeline()
    try:
        ctx = pipeline.run(ctx)
    except Exception as e:
        console.print()
        error(f"Build failed: {e}")
        raise click.ClickException(str(e))

    exe = ctx.state.get("executable")
    console.print()
    if exe:
        success(f"Build succeeded → [bold white]{exe}[/bold white]")
    else:
        warn("Build finished but no executable was produced")

    if not keep_intermediates:
        for stage_dir in build_dir.iterdir():
            if stage_dir.is_dir():
                shutil.rmtree(stage_dir)

# ---------------------------------------------------------------------------
# run
# ---------------------------------------------------------------------------

@cli.command(context_settings=dict(ignore_unknown_options=True))
@click.argument("file", type=click.Path(exists=True, dir_okay=False))
@click.option("--profile", "-p", default=None, help="Obfuscation profile")
@click.argument("run_args", nargs=-1, type=click.UNPROCESSED)
def run(file: str, profile: Optional[str], run_args: tuple):
    """Build and immediately run a JOCKY executable."""
    input_path = Path(file)
    build_dir = input_path.parent / ".jocky-build"

    # Reuse build logic
    prof = resolve_profile(profile)
    config = {}
    ctx = BuildContext(
        input_file=input_path,
        profile_name=profile or (prof.name if prof else "none"),
        output_dir=build_dir,
        config=config,
    )
    ctx.state["profile"] = prof

    header("JOCKY Run")
    console.print(f"  [dim]Source[/dim]  {input_path}")
    console.print(f"  [dim]Profile[/dim]  {ctx.profile_name}")
    console.print()

    pipeline = get_pipeline()
    try:
        ctx = pipeline.run(ctx)
    except Exception as e:
        console.print()
        error(f"Build failed: {e}")
        raise click.ClickException(str(e))

    exe = ctx.state.get("executable")
    if not exe or not exe.exists():
        error("Executable not found after build")
        raise click.ClickException("Executable missing")

    console.print()
    success(f"Build succeeded → [bold white]{exe}[/bold white]")
    console.print()
    console.rule("[bold green]Program Output[/bold green]")

    result = subprocess.run([str(exe)] + list(run_args))
    sys.exit(result.returncode)

# ---------------------------------------------------------------------------
# verify
# ---------------------------------------------------------------------------

@cli.command()
@click.argument("file", type=click.Path(exists=True, dir_okay=False))
def verify(file: str):
    """Parse and type-check a JOCKY file without compiling."""
    input_path = Path(file)
    src = input_path.read_text()

    header("JOCKY Verify")
    console.print(f"  [dim]File[/dim]  {input_path}")
    console.print()

    from jocky.language.lexer import Lexer
    from jocky.language.parser import Parser
    from jocky.language.checker import TypeChecker

    with console.status("[bold cyan]Lexing...[/bold cyan]", spinner="dots"):
        lexer = Lexer(src)
        tokens = lexer.tokenize()
    success(f"Lexer produced {len(tokens)} tokens")

    with console.status("[bold cyan]Parsing...[/bold cyan]", spinner="dots"):
        parser = Parser(tokens)
        ast = parser.parse()
    decl_count = len(ast.decls)
    success(f"Parser produced AST with {decl_count} top-level declaration(s)")

    with console.status("[bold cyan]Type-checking...[/bold cyan]", spinner="dots"):
        checker = TypeChecker()
        checker.check(ast)
    success("Type-check passed")

    console.print()
    console.print(Panel(
        f"[bold green]{input_path.name}[/bold green] is syntactically and type-correct.",
        title="Verification Result",
        border_style="green"
    ))

# ---------------------------------------------------------------------------
# info
# ---------------------------------------------------------------------------

@cli.command()
@click.argument("file", type=click.Path(exists=True, dir_okay=False))
def info(file: str):
    """Show pipeline info for a JOCKY file."""
    input_path = Path(file)

    header("JOCKY Info")

    stages = Table(box=box.ROUNDED, show_header=True, header_style="bold magenta")
    stages.add_column("#", style="dim", justify="right")
    stages.add_column("Stage")
    stages.add_column("Description")

    descriptions = [
        ("parse", "Lex, parse, and type-check JOCKY source"),
        ("lower_ir", "Generate textual LLVM IR from AST"),
        ("mlir_obfuscate", "Run MLIR passes (string-encrypt, constant-obfuscate, ...)"),
        ("ir_obfuscate", "Run LLVM IR obfuscation passes via opt"),
        ("link", "Compile bitcode to object and link executable"),
        ("pack", "Optional UPX packing"),
    ]
    for i, (name, desc) in enumerate(descriptions, 1):
        stages.add_row(str(i), f"[bold]{name}[/bold]", desc)

    console.print(stages)
    console.print()
    console.print(f"  [dim]File[/dim]    {input_path}")
    console.print(f"  [dim]Size[/dim]    {input_path.stat().st_size} bytes")

# ---------------------------------------------------------------------------
# list-profiles
# ---------------------------------------------------------------------------

@cli.command("list-profiles")
def list_profiles_cmd():
    """List available obfuscation profiles."""
    profiles = list_profiles()

    table = Table(title="Obfuscation Profiles", box=box.ROUNDED, show_header=True, header_style="bold magenta")
    table.add_column("Name", style="bold cyan")
    table.add_column("Description")
    table.add_column("File", style="dim")

    for p in profiles:
        table.add_row(p["name"], p["description"], p["file"])

    console.print()
    console.print(table)
    console.print()
    console.print("[dim]Use --profile NAME with build/run to select a profile.[/dim]")

# ---------------------------------------------------------------------------
# list-passes
# ---------------------------------------------------------------------------

@cli.command("list-passes")
def list_passes_cmd():
    """List available obfuscation passes."""
    table = Table(title="Obfuscation Passes", box=box.ROUNDED, show_header=True, header_style="bold magenta")
    table.add_column("Profile Name", style="bold cyan")
    table.add_column("opt/mlir Flag", style="bold green")
    table.add_column("Category", style="dim")

    from jocky.passes.registry import FUNCTION_PASSES, MODULE_PASSES, MLIR_PASSES

    for name, flag in sorted(PASS_REGISTRY.items()):
        if flag in FUNCTION_PASSES:
            cat = "LLVM function"
        elif flag in MODULE_PASSES:
            cat = "LLVM module"
        elif flag in MLIR_PASSES:
            cat = "MLIR"
        else:
            cat = "other"
        table.add_row(name, flag, cat)

    console.print()
    console.print(table)

# ---------------------------------------------------------------------------
# clean
# ---------------------------------------------------------------------------

@cli.command()
@click.option("--all", "all_flag", is_flag=True, help="Remove all build artifacts including cache")
def clean(all_flag: bool):
    """Clean build artifacts from the current directory."""
    header("JOCKY Clean")
    removed = 0
    for build_dir in Path(".").rglob(".jocky-build"):
        shutil.rmtree(build_dir)
        console.print(f"  [red]✗[/red] Removed {build_dir}")
        removed += 1
    if removed:
        success(f"Cleaned {removed} build directory(s)")
    else:
        console.print("  [dim]Nothing to clean.[/dim]")

# ---------------------------------------------------------------------------
# Entry point
# ---------------------------------------------------------------------------

def main():
    cli()

if __name__ == "__main__":
    main()
