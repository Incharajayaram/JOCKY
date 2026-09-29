#!/usr/bin/env python3
"""Interactive REPL for JOCKY language experimentation."""

import cmd
import sys
import tempfile
import subprocess
from pathlib import Path
from typing import Optional, List

import click
from rich.console import Console
from rich.syntax import Syntax
from rich.panel import Panel
from rich.table import Table

from .language.lexer import Lexer
from .language.parser import Parser
from .language.checker import Checker
from .language.codegen import Codegen

console = Console()


class JockyREPL(cmd.Cmd):
    """Interactive JOCKY language REPL."""

    intro = """
╭──────────────────────────────────────╮
│  JOCKY Interactive REPL              │
│  Type 'help' for commands            │
│  Type 'exit' or Ctrl-D to quit       │
╰──────────────────────────────────────╯
"""
    prompt = "jocky> "

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.buffer = ""
        self.in_multiline = False
        self.functions: dict = {}
        self.variables: dict = {}
        self.history: List[str] = []

    def default(self, line: str):
        """Handle JOCKY code input."""
        if not line.strip():
            return

        # Handle multiline input
        if line.endswith("\\"):
            self.buffer += line[:-1] + "\n"
            self.in_multiline = True
            self.prompt = "...> "
            return

        if self.in_multiline:
            self.buffer += line + "\n"
            if line.strip() == "":
                # Empty line ends multiline
                line = self.buffer
                self.buffer = ""
                self.in_multiline = False
                self.prompt = "jocky> "
            else:
                return

        self.history.append(line)
        self._eval_code(line)

    def _eval_code(self, code: str):
        """Evaluate JOCKY code."""
        try:
            # Lex and parse
            lexer = Lexer(code)
            tokens = lexer.tokenize()

            parser = Parser(tokens)
            ast = parser.parse()

            if not ast:
                console.print("[yellow]No valid code to evaluate[/]")
                return

            # Type check
            checker = Checker()
            checked_ast = checker.check(ast)

            # Show AST
            console.print(f"[cyan]AST:[/] {ast}")

            # Codegen
            codegen = Codegen()
            ir = codegen.codegen(checked_ast)

            console.print(Panel(
                Syntax(ir, "llvm", theme="monokai", line_numbers=True),
                title="LLVM IR",
                expand=False
            ))

        except Exception as e:
            console.print(f"[red]Error:[/] {e}")

    def do_vars(self, _):
        """Show current variables."""
        if not self.variables:
            console.print("[yellow]No variables defined[/]")
            return

        table = Table(title="Variables")
        table.add_column("Name", style="cyan")
        table.add_column("Type", style="magenta")
        table.add_column("Value", style="green")

        for name, (typ, val) in self.variables.items():
            table.add_row(name, typ, str(val))

        console.print(table)

    def do_funcs(self, _):
        """Show defined functions."""
        if not self.functions:
            console.print("[yellow]No functions defined[/]")
            return

        table = Table(title="Functions")
        table.add_column("Name", style="cyan")
        table.add_column("Signature", style="magenta")

        for name, sig in self.functions.items():
            table.add_row(name, sig)

        console.print(table)

    def do_help(self, arg):
        """Show help."""
        if not arg:
            console.print(Panel("""
[cyan]JOCKY REPL Commands:[/]

[bold]eval[/]
  Evaluate JOCKY code (default action)

[bold]vars[/]
  Show all variables

[bold]funcs[/]
  Show all functions

[bold]history[/]
  Show command history

[bold]clear[/]
  Clear all variables and functions

[bold]load <file>[/]
  Load JOCKY file

[bold]compile <output>[/]
  Compile current buffer to executable

[bold]profile <name>[/]
  Set obfuscation profile (none, light, standard, aggressive)

[bold]exit[/]
  Exit REPL (Ctrl-D also works)
            """, title="Help"))
        else:
            super().do_help(arg)

    def do_history(self, _):
        """Show command history."""
        if not self.history:
            console.print("[yellow]No history[/]")
            return

        for i, cmd_line in enumerate(self.history, 1):
            console.print(f"[cyan]{i}[/] {cmd_line}")

    def do_clear(self, _):
        """Clear all state."""
        self.variables.clear()
        self.functions.clear()
        self.buffer = ""
        console.print("[green]State cleared[/]")

    def do_load(self, filename: str):
        """Load JOCKY file."""
        if not filename:
            console.print("[red]Usage: load <filename>[/]")
            return

        path = Path(filename)
        if not path.exists():
            console.print(f"[red]File not found: {filename}[/]")
            return

        try:
            with open(path) as f:
                code = f.read()
            self._eval_code(code)
            console.print(f"[green]Loaded {filename}[/]")
        except Exception as e:
            console.print(f"[red]Error loading file:[/] {e}")

    def do_compile(self, output: str):
        """Compile current code."""
        if not output:
            console.print("[red]Usage: compile <output_path>[/]")
            return

        if not self.buffer and not self.history:
            console.print("[yellow]No code to compile[/]")
            return

        code = self.buffer or "\n".join(self.history)

        with tempfile.NamedTemporaryFile(mode='w', suffix='.jky', delete=False) as f:
            f.write(code)
            temp_file = f.name

        try:
            # Use jocky CLI to compile
            result = subprocess.run(
                ["jocky", "build", temp_file, "-o", output, "-p", "standard"],
                capture_output=True,
                text=True
            )

            if result.returncode == 0:
                console.print(f"[green]✓ Compiled to {output}[/]")
            else:
                console.print(f"[red]Compilation failed:[/]\n{result.stderr}")

        finally:
            Path(temp_file).unlink(missing_ok=True)

    def do_exit(self, _):
        """Exit REPL."""
        console.print("[cyan]Goodbye![/]")
        return True

    def emptyline(self):
        """Don't repeat last command on empty input."""
        pass

    def do_EOF(self, _):
        """Handle Ctrl-D."""
        print()
        return self.do_exit("")


@click.command()
@click.option('-f', '--file', type=click.Path(), help='Load file at startup')
def repl(file: Optional[str]):
    """Start JOCKY interactive REPL."""
    try:
        repl_instance = JockyREPL()

        if file:
            repl_instance.onecmd(f"load {file}")

        repl_instance.cmdloop()
    except KeyboardInterrupt:
        console.print("\n[yellow]Interrupted[/]")
        sys.exit(0)


if __name__ == '__main__':
    repl()
