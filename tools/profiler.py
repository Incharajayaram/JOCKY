#!/usr/bin/env python3
"""Performance profiler for JOCKY compiler."""

import time
import json
import sys
from pathlib import Path
from typing import Dict, List
import subprocess
import tempfile

import click
from rich.console import Console
from rich.table import Table
from rich import box

console = Console()


class CompilerProfiler:
    """Profile JOCKY compiler performance."""

    def __init__(self):
        self.results: Dict[str, Dict] = {}
        self.timing_data: Dict[str, List[float]] = {}

    def profile_build(self, source_file: Path, profile_name: str, iterations: int = 3) -> Dict:
        """Profile a single build."""
        times = []

        console.print(f"[cyan]Profiling[/cyan] {source_file.name} with profile [bold]{profile_name}[/bold]...")

        for i in range(iterations):
            with tempfile.TemporaryDirectory() as tmpdir:
                output = Path(tmpdir) / "output"

                start = time.perf_counter()
                result = subprocess.run(
                    ["jocky", "build", str(source_file), "-p", profile_name, "-o", str(output)],
                    capture_output=True,
                    text=True
                )
                elapsed = time.perf_counter() - start

                if result.returncode != 0:
                    console.print(f"[red]Build failed:[/red] {result.stderr}")
                    return None

                times.append(elapsed)
                console.print(f"  [dim]Run {i+1}/{iterations}:[/dim] {elapsed:.3f}s")

        avg = sum(times) / len(times)
        min_time = min(times)
        max_time = max(times)

        return {
            "average": avg,
            "min": min_time,
            "max": max_time,
            "times": times,
            "iterations": iterations,
        }

    def profile_all_profiles(self, source_file: Path, iterations: int = 3) -> Dict[str, Dict]:
        """Profile all available obfuscation profiles."""
        profiles = ["none", "light", "standard", "aggressive", "paranoid"]
        results = {}

        console.print(f"[bold cyan]Profiling all obfuscation profiles[/bold cyan]")
        console.print(f"  Source: {source_file}")
        console.print(f"  Iterations: {iterations}")
        console.print()

        for profile in profiles:
            result = self.profile_build(source_file, profile, iterations)
            if result:
                results[profile] = result
            console.print()

        return results

    def profile_stages(self, source_file: Path) -> Dict[str, float]:
        """Profile individual compilation stages."""
        stages = ["parse", "lower-ir", "mlir-obfuscate", "ir-obfuscate", "link", "pack"]
        results = {}

        console.print(f"[bold cyan]Profiling compilation stages[/bold cyan]")

        for stage in stages:
            # This would require modifications to the jocky CLI to support stage profiling
            # For now, we'll just show the concept
            console.print(f"  [dim]{stage}:[/dim] [yellow](requires CLI extension)[/yellow]")

        return results

    def generate_report(self, results: Dict[str, Dict]):
        """Generate performance report."""
        table = Table(
            title="JOCKY Compiler Performance Report",
            box=box.ROUNDED,
            show_header=True,
            header_style="bold magenta"
        )
        table.add_column("Profile", style="bold cyan")
        table.add_column("Average (s)", justify="right", style="green")
        table.add_column("Min (s)", justify="right", style="dim")
        table.add_column("Max (s)", justify="right", style="dim")
        table.add_column("Overhead", justify="right")

        # Calculate baseline (none profile)
        baseline = results.get("none", {}).get("average", 0)

        for profile, data in sorted(results.items()):
            avg = data["average"]
            min_t = data["min"]
            max_t = data["max"]

            if baseline > 0 and profile != "none":
                overhead = f"{((avg - baseline) / baseline * 100):.1f}%"
            else:
                overhead = "—"

            table.add_row(
                profile,
                f"{avg:.3f}",
                f"{min_t:.3f}",
                f"{max_t:.3f}",
                overhead
            )

        console.print()
        console.print(table)
        console.print()

    def save_results(self, results: Dict[str, Dict], output_file: Path):
        """Save results to JSON."""
        with open(output_file, 'w') as f:
            json.dump(results, f, indent=2)
        console.print(f"[green]Results saved to[/green] {output_file}")

    def compare_results(self, file1: Path, file2: Path):
        """Compare two profiling results."""
        with open(file1) as f:
            results1 = json.load(f)
        with open(file2) as f:
            results2 = json.load(f)

        table = Table(
            title="Performance Comparison",
            box=box.ROUNDED,
            show_header=True,
            header_style="bold magenta"
        )
        table.add_column("Profile", style="bold cyan")
        table.add_column("Before (s)", justify="right")
        table.add_column("After (s)", justify="right")
        table.add_column("Change", justify="right")

        for profile in sorted(results1.keys()):
            if profile not in results2:
                continue

            before = results1[profile]["average"]
            after = results2[profile]["average"]
            change_pct = ((after - before) / before * 100) if before > 0 else 0

            change_str = f"{change_pct:+.1f}%"
            if change_pct < 0:
                change_str = f"[green]{change_str}[/green]"
            elif change_pct > 0:
                change_str = f"[red]{change_str}[/red]"

            table.add_row(profile, f"{before:.3f}", f"{after:.3f}", change_str)

        console.print()
        console.print(table)
        console.print()


@click.group()
def cli():
    """JOCKY compiler performance profiler."""
    pass


@cli.command()
@click.argument("source_file", type=click.Path(exists=True, dir_okay=False))
@click.option("-p", "--profile", default=None, help="Specific profile to test")
@click.option("-i", "--iterations", default=3, type=int, help="Number of iterations")
@click.option("-o", "--output", type=click.Path(), help="Save results to JSON file")
def profile(source_file: str, profile: str, iterations: int, output: str):
    """Profile JOCKY compiler performance."""
    source_path = Path(source_file)
    profiler = CompilerProfiler()

    if profile:
        results = {profile: profiler.profile_build(source_path, profile, iterations)}
    else:
        results = profiler.profile_all_profiles(source_path, iterations)

    profiler.generate_report(results)

    if output:
        profiler.save_results(results, Path(output))


@cli.command()
@click.argument("file1", type=click.Path(exists=True, dir_okay=False))
@click.argument("file2", type=click.Path(exists=True, dir_okay=False))
def compare(file1: str, file2: str):
    """Compare two profiling result files."""
    profiler = CompilerProfiler()
    profiler.compare_results(Path(file1), Path(file2))


@cli.command()
@click.argument("source_file", type=click.Path(exists=True, dir_okay=False))
def stages(source_file: str):
    """Profile individual compilation stages."""
    source_path = Path(source_file)
    profiler = CompilerProfiler()

    profiler.profile_stages(source_path)
    console.print("[yellow]Stage profiling requires CLI extension to measure individual stages.[/yellow]")


if __name__ == '__main__':
    cli()
