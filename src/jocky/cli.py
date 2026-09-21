import argparse
import sys
from typing import Optional

def main(args: Optional[list[str]] = None):
    parser = argparse.ArgumentParser(prog="jocky", description="JOCKY Compiler Pipeline")
    subparsers = parser.add_subparsers(dest="command", required=True)

    # build command
    build_parser = subparsers.add_parser("build", help="Build a JOCKY file")
    build_parser.add_argument("file", help="Input .jky file")
    build_parser.add_argument("--profile", help="Build profile to use")
    build_parser.add_argument("--output", help="Output path")
    build_parser.add_argument("--keep-intermediates", action="store_true", help="Keep intermediate files")

    # run command
    run_parser = subparsers.add_parser("run", help="Run a JOCKY file")
    run_parser.add_argument("file", help="Input .jky file")
    run_parser.add_argument("--profile", help="Build profile to use")
    run_parser.add_argument("args", nargs=argparse.REMAINDER, help="Arguments for the executable")

    # clean command
    clean_parser = subparsers.add_parser("clean", help="Clean build artifacts")
    clean_parser.add_argument("--all", action="store_true", help="Clean all artifacts including cache")

    # info command
    info_parser = subparsers.add_parser("info", help="Show pipeline stages + profile")
    info_parser.add_argument("file", help="Input .jky file")

    # list-profiles command
    subparsers.add_parser("list-profiles", help="List available profiles")

    # list-passes command
    subparsers.add_parser("list-passes", help="List available obfuscation passes")

    # verify command
    verify_parser = subparsers.add_parser("verify", help="Dry-run preflight only")
    verify_parser.add_argument("file", help="Input .jky file")

    parsed_args = parser.parse_args(args)

    # Dispatch logic would go here. For now, we just print the parsed command.
    print(f"JOCKY CLI: {parsed_args.command}")
    
    if parsed_args.command == "build":
        print(f"Building {parsed_args.file} with profile {parsed_args.profile}")
        # from jocky.core.pipeline import build_pipeline
        # build_pipeline(parsed_args.file, parsed_args.profile)
    elif parsed_args.command == "verify":
        print(f"Verifying {parsed_args.file}")
        
if __name__ == "__main__":
    main()
