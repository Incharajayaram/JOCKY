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
@click.option("--target", "-t", default="native", help="Target platform (linux, windows, or both)")
@click.option("--keep-intermediates", is_flag=True, help="Keep intermediate build files")
@click.option("--no-prelude", is_flag=True, help="Skip auto-injection of the standard prelude")
def build(file: str, profile: Optional[str], output: Optional[str], target: str, keep_intermediates: bool, no_prelude: bool):
    """Build a JOCKY source file into a native executable (Linux, Windows, or both)."""
    input_path = Path(file)
    build_dir = input_path.parent / ".jocky-build"
    build_dir.mkdir(exist_ok=True)

    prof = resolve_profile(profile)
    targets = ["linux", "windows"] if target.lower() == "both" else [target]

    for tgt in targets:
        config = {"target": tgt, "no_prelude": no_prelude}
        if output:
            config["output"] = output if len(targets) == 1 else f"{output}_{tgt}"

        ctx = BuildContext(
            input_file=input_path,
            profile_name=profile or (prof.name if prof else "none"),
            output_dir=build_dir,
            config=config,
        )
        ctx.state["profile"] = prof

        header(f"JOCKY Build ({tgt})")
        console.print(f"  [dim]Source[/dim]     {input_path}")
        console.print(f"  [dim]Profile[/dim]    {ctx.profile_name}")
        console.print(f"  [dim]Target[/dim]     {tgt}")
        console.print(f"  [dim]Build dir[/dim]  {build_dir}")
        console.print()

        pipeline = get_pipeline()
        try:
            ctx = pipeline.run(ctx)
        except Exception as e:
            console.print()
            error(f"Build failed for {tgt}: {e}")
            raise click.ClickException(str(e))

        exe = ctx.state.get("executable")
        console.print()
        if exe:
            success(f"Build succeeded [{tgt}] → [bold white]{exe}[/bold white]")
        else:
            warn(f"Build finished for {tgt} but no executable was produced")

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

    # Include prelude if not disabled
    prelude_path = Path(__file__).parent / "stdlib" / "prelude.jky"
    if prelude_path.exists():
        prelude_src = prelude_path.read_text()
        src = prelude_src + "\n" + src

    header("JOCKY Verify")
    console.print(f"  [dim]File[/dim]  {input_path}")
    console.print()

    from jocky.language.lexer import Lexer
    from jocky.language.parser import Parser
    from jocky.language.resolver import ModuleResolver
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

    with console.status("[bold cyan]Resolving modules...[/bold cyan]", spinner="dots"):
        resolver = ModuleResolver(input_path.parent)
        ast = resolver.resolve_program(ast)
    success(f"Resolved to {len(ast.decls)} declaration(s)")

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
# repl
# ---------------------------------------------------------------------------

@cli.command()
@click.option('-f', '--file', type=click.Path(), help='Load file at startup')
def repl(file: str):
    """Start JOCKY interactive REPL for experimentation."""
    from jocky.repl import JockyREPL

    try:
        repl_instance = JockyREPL()

        if file:
            repl_instance.onecmd(f"load {file}")

        repl_instance.cmdloop()
    except KeyboardInterrupt:
        console.print("\n[yellow]Interrupted[/yellow]")
        raise click.Abort()

# ---------------------------------------------------------------------------
# mutate
# ---------------------------------------------------------------------------

@cli.command()
@click.option("--test", required=True, type=click.Path(exists=True), help="Test file to run")
@click.option("--source", required=True, type=click.Path(exists=True), help="Source file to mutate")
@click.option("--timeout", type=int, default=10, help="Timeout per test run (seconds)")
@click.option("--export", type=click.Path(), help="Export results to JSON file")
@click.option("--quiet", is_flag=True, help="Suppress verbose output")
def mutate(test: str, source: str, timeout: int, export: Optional[str], quiet: bool):
    """Run mutation testing to measure test quality."""
    try:
        from tools.mutation_test import MutationTester
    except ImportError:
        error("Mutation testing tools not available. Install with: pip install -e .")
        raise click.Abort()

    if not quiet:
        header("Mutation Testing")
        info_line("Test file:", test)
        info_line("Source file:", source)
        info_line("Timeout:", f"{timeout}s")
        console.print()

    try:
        tester = MutationTester(test, source, timeout=timeout)
        report_text = tester.run_and_report()

        console.print(report_text)

        if export:
            tester.export_json(export)
            success(f"Results exported to {export}")
    except Exception as e:
        error(f"Mutation testing failed: {e}")
        raise click.Abort()

# ---------------------------------------------------------------------------
# debug
# ---------------------------------------------------------------------------

@cli.command()
@click.argument("file", type=click.Path(exists=True, dir_okay=False))
@click.option("--breakpoint", "-b", multiple=True, help="Set breakpoint at location (file:line or function)")
@click.option("--lldb", is_flag=True, help="Use LLDB instead of GDB")
@click.argument("run_args", nargs=-1, type=click.UNPROCESSED)
def debug(file: str, breakpoint: tuple, lldb: bool, run_args: tuple):
    """Debug a compiled JOCKY executable with GDB or LLDB."""
    try:
        import sys
        sys.path.insert(0, str(Path(__file__).parent.parent.parent / "tools"))
        from debugger import JockyDebugger
    except ImportError:
        error("Debugger tools not available. Install with: pip install -e .")
        raise click.Abort()

    executable = Path(file)
    if not executable.exists():
        error(f"Executable not found: {file}")
        raise click.Abort()

    debugger_type = "lldb" if lldb else "gdb"

    header("JOCKY Debugger")
    info_line("Executable:", str(executable))
    info_line("Debugger:", debugger_type)
    console.print()

    try:
        with JockyDebugger(str(executable), debugger_type) as dbg:
            # Set breakpoints
            for bp in breakpoint:
                console.print(f"  Setting breakpoint at [cyan]{bp}[/cyan]")
                dbg.set_breakpoint(bp)

            # Run program
            console.print(f"  Starting [bold]{executable.name}[/bold]...")
            dbg.run(list(run_args) if run_args else None)
            console.print()

            # Interactive debugging loop
            console.print("[dim]Type 'help' for commands, 'quit' to exit[/dim]")
            while True:
                try:
                    cmd = console.input("[bold cyan](jocky-dbg)[/bold cyan] ").strip()
                    if not cmd:
                        continue

                    if cmd == "quit" or cmd == "exit":
                        break
                    elif cmd == "help":
                        _print_debug_help()
                    elif cmd == "continue" or cmd == "c":
                        dbg.continue_execution()
                        console.print("[dim]Continuing...[/dim]")
                    elif cmd == "step" or cmd == "s":
                        dbg.step()
                        console.print("[dim]Stepped[/dim]")
                    elif cmd == "next" or cmd == "n":
                        dbg.next()
                        console.print("[dim]Next[/dim]")
                    elif cmd == "stepi" or cmd == "si":
                        dbg.step_instruction()
                        console.print("[dim]Instruction stepped[/dim]")
                    elif cmd == "backtrace" or cmd == "bt":
                        output = dbg.backtrace()
                        if output:
                            console.print(output)
                    elif cmd == "registers" or cmd == "reg":
                        output = dbg.print_registers()
                        if output:
                            console.print(output)
                    elif cmd.startswith("print "):
                        var = cmd[6:].strip()
                        output = dbg.print_variable(var)
                        if output:
                            console.print(output)
                    elif cmd.startswith("break "):
                        location = cmd[6:].strip()
                        dbg.set_breakpoint(location)
                        console.print(f"[green]Breakpoint set at {location}[/green]")
                    else:
                        console.print("[yellow]Unknown command. Type 'help' for help.[/yellow]")
                except KeyboardInterrupt:
                    console.print("\n[yellow]Interrupted[/yellow]")
                    break
                except EOFError:
                    break

    except FileNotFoundError as e:
        error(str(e))
        raise click.Abort()
    except RuntimeError as e:
        error(str(e))
        raise click.Abort()


def _print_debug_help():
    """Print debug command help."""
    help_text = """
[bold cyan]JOCKY Debugger Commands:[/bold cyan]

  [bold]c, continue[/bold]       Continue execution
  [bold]s, step[/bold]          Step one source line
  [bold]n, next[/bold]          Step over (next function)
  [bold]si, stepi[/bold]        Step one machine instruction
  [bold]bt, backtrace[/bold]    Print call stack
  [bold]reg, registers[/bold]   Print CPU registers
  [bold]print VAR[/bold]        Print variable value
  [bold]break LOCATION[/bold]   Set breakpoint (file:line or function)
  [bold]help[/bold]             Show this help
  [bold]quit, exit[/bold]       Exit debugger
"""
    console.print(help_text)

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
