#!/usr/bin/env python3
"""JOCKY debugger interface for GDB/LLDB.

Provides Python interface to control debugging of JOCKY compiled programs.
"""

import subprocess
import re
import sys
from typing import List, Optional, Tuple
from pathlib import Path


class JockyDebugger:
    """Interface to GDB/LLDB for debugging JOCKY programs."""

    def __init__(self, executable: str, debugger: str = "gdb"):
        """Initialize debugger.

        Args:
            executable: Path to compiled JOCKY executable
            debugger: "gdb" or "lldb"
        """
        self.executable = Path(executable)
        self.debugger_type = debugger.lower()
        self.process = None
        self.breakpoints: List[str] = []
        self.current_location = None

        if not self.executable.exists():
            raise FileNotFoundError(f"Executable not found: {executable}")

        if self.debugger_type not in ("gdb", "lldb"):
            raise ValueError(f"Unsupported debugger: {debugger}")

    def start(self, args: Optional[List[str]] = None):
        """Start program under debugger.

        Args:
            args: Command-line arguments to pass to program
        """
        if self.process:
            return

        cmd = self._build_debugger_command(args or [])
        try:
            self.process = subprocess.Popen(
                cmd,
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                text=True,
                bufsize=1
            )
        except FileNotFoundError:
            raise RuntimeError(f"Debugger not found: {self.debugger_type}")

    def _build_debugger_command(self, args: List[str]) -> List[str]:
        """Build debugger command line."""
        if self.debugger_type == "gdb":
            return [
                "gdb",
                "-q",
                "-batch",
                "-ex", f"file {self.executable}",
                *(["-ex", f"set args {' '.join(args)}"] if args else [])
            ]
        else:  # lldb
            return [
                "lldb",
                "-b",
                str(self.executable)
            ]

    def set_breakpoint(self, location: str) -> bool:
        """Set breakpoint.

        Args:
            location: Function name, file:line, or address

        Returns:
            True if breakpoint was set successfully
        """
        if not self.process:
            self.start()

        cmd = self._format_command("break", location)
        return self._execute_command(cmd)

    def remove_breakpoint(self, location: str) -> bool:
        """Remove breakpoint.

        Args:
            location: Same format as set_breakpoint

        Returns:
            True if breakpoint was removed
        """
        if not self.process:
            return False

        cmd = self._format_command("delete", location)
        return self._execute_command(cmd)

    def run(self, args: Optional[List[str]] = None) -> bool:
        """Run program.

        Args:
            args: Command-line arguments

        Returns:
            True if program ran successfully
        """
        if not self.process:
            self.start(args)
            return True

        return self._execute_command("run")

    def continue_execution(self) -> bool:
        """Continue from breakpoint."""
        if not self.process:
            return False
        return self._execute_command("continue")

    def step(self) -> bool:
        """Step one source line."""
        if not self.process:
            return False
        return self._execute_command("step")

    def next(self) -> bool:
        """Step over one source line."""
        if not self.process:
            return False
        return self._execute_command("next")

    def step_instruction(self) -> bool:
        """Step one machine instruction."""
        if not self.process:
            return False
        return self._execute_command("stepi")

    def backtrace(self) -> Optional[str]:
        """Print call stack."""
        if not self.process:
            return None
        return self._execute_command("backtrace", capture_output=True)

    def print_variable(self, var_name: str) -> Optional[str]:
        """Print variable value.

        Args:
            var_name: Variable name to print

        Returns:
            Variable value or None
        """
        if not self.process:
            return None
        return self._execute_command(f"print {var_name}", capture_output=True)

    def print_registers(self) -> Optional[str]:
        """Print CPU registers."""
        if not self.process:
            return None
        return self._execute_command("info registers", capture_output=True)

    def print_memory(self, address: str, count: int = 16) -> Optional[str]:
        """Print memory contents.

        Args:
            address: Memory address to examine
            count: Number of bytes to print

        Returns:
            Memory dump or None
        """
        if not self.process:
            return None
        return self._execute_command(
            f"x/{count}bx {address}",
            capture_output=True
        )

    def get_location(self) -> Optional[Tuple[str, int]]:
        """Get current source location (file, line).

        Returns:
            Tuple of (filename, line_number) or None
        """
        output = self.backtrace()
        if not output:
            return None

        # Parse first line of backtrace to get location
        for line in output.split("\n"):
            match = re.search(r"at (.+):(\d+)", line)
            if match:
                return (match.group(1), int(match.group(2)))
        return None

    def quit(self):
        """Terminate debugger."""
        if self.process:
            try:
                self.process.terminate()
                self.process.wait(timeout=2)
            except subprocess.TimeoutExpired:
                self.process.kill()
            self.process = None

    def _format_command(self, cmd: str, arg: str) -> str:
        """Format debugger command."""
        if self.debugger_type == "gdb":
            if cmd == "break":
                return f"break {arg}"
            elif cmd == "delete":
                return f"delete {arg}"
            elif cmd == "continue":
                return "continue"
            elif cmd == "step":
                return "step"
            elif cmd == "next":
                return "next"
            elif cmd == "stepi":
                return "stepi"
            else:
                return cmd
        else:  # lldb
            if cmd == "break":
                return f"breakpoint set -n {arg}"
            elif cmd == "delete":
                return f"breakpoint delete {arg}"
            elif cmd == "continue":
                return "continue"
            elif cmd == "step":
                return "thread step-in"
            elif cmd == "next":
                return "thread step-over"
            elif cmd == "stepi":
                return "thread step-inst"
            else:
                return cmd

    def _execute_command(self, cmd: str, capture_output: bool = False) -> bool:
        """Execute a debugger command.

        Args:
            cmd: Command to execute
            capture_output: If True, return command output instead of success status

        Returns:
            Boolean success status or output string if capture_output=True
        """
        if not self.process:
            return False if not capture_output else None

        try:
            self.process.stdin.write(cmd + "\n")
            self.process.stdin.flush()

            if capture_output:
                output = []
                while True:
                    line = self.process.stdout.readline()
                    if not line or line.startswith("(gdb)") or line.startswith("(lldb)"):
                        break
                    output.append(line)
                return "\n".join(output)
            else:
                # Wait for prompt
                for _ in range(100):
                    line = self.process.stdout.readline()
                    if "(gdb)" in line or "(lldb)" in line:
                        return True
                return False
        except Exception as e:
            print(f"Error executing command: {e}", file=sys.stderr)
            return False if not capture_output else None

    def __enter__(self):
        """Context manager support."""
        self.start()
        return self

    def __exit__(self, *args):
        """Context manager cleanup."""
        self.quit()


def main():
    """CLI for JOCKY debugger."""
    import argparse

    parser = argparse.ArgumentParser(
        description="Debug JOCKY compiled programs"
    )
    parser.add_argument("executable", help="Path to executable")
    parser.add_argument(
        "--gdb", action="store_const", const="gdb", dest="debugger",
        help="Use GDB (default)"
    )
    parser.add_argument(
        "--lldb", action="store_const", const="lldb", dest="debugger",
        help="Use LLDB"
    )
    parser.add_argument(
        "-b", "--breakpoint", action="append", dest="breakpoints",
        help="Set breakpoint at location (can be used multiple times)"
    )
    parser.add_argument(
        "args", nargs="*", help="Arguments to pass to executable"
    )

    args = parser.parse_args()
    args.debugger = args.debugger or "gdb"

    try:
        with JockyDebugger(args.executable, args.debugger) as dbg:
            # Set breakpoints
            for bp in args.breakpoints or []:
                print(f"Setting breakpoint at {bp}")
                dbg.set_breakpoint(bp)

            # Run program
            print(f"Running {args.executable}")
            dbg.run(args.args or None)

            # Interactive debugging loop
            while True:
                try:
                    cmd = input("(jocky-dbg) ").strip()
                    if not cmd:
                        continue

                    if cmd == "quit" or cmd == "exit":
                        break
                    elif cmd == "continue" or cmd == "c":
                        dbg.continue_execution()
                    elif cmd == "step" or cmd == "s":
                        dbg.step()
                    elif cmd == "next" or cmd == "n":
                        dbg.next()
                    elif cmd == "stepi" or cmd == "si":
                        dbg.step_instruction()
                    elif cmd == "backtrace" or cmd == "bt":
                        output = dbg.backtrace()
                        print(output)
                    elif cmd == "registers" or cmd == "reg":
                        output = dbg.print_registers()
                        print(output)
                    elif cmd.startswith("print "):
                        var = cmd[6:].strip()
                        output = dbg.print_variable(var)
                        print(output)
                    elif cmd.startswith("break "):
                        location = cmd[6:].strip()
                        dbg.set_breakpoint(location)
                    else:
                        print("Unknown command")
                except KeyboardInterrupt:
                    print("\nInterrupted")
                    break
                except EOFError:
                    break

    except FileNotFoundError as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)
    except RuntimeError as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
