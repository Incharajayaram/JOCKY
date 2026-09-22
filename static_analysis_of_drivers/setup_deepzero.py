import argparse
import json
import os
import shutil
import subprocess
import sys
import zipfile
from pathlib import Path

GHIDRA_RELEASE = "Ghidra_12.1.4_build"
GHIDRA_URL = ("https://github.com/NationalSecurityAgency/ghidra/releases/"
              f"download/{GHIDRA_RELEASE}/ghidra_12.1.4_PUBLIC_20260921.zip")
DEEPZERO_REPO = "https://github.com/416rehman/DeepZero.git"
ADOPTIUM_API = "https://api.adoptium.net/v3/binary/latest/17/ga/windows/x64/jdk/hotspot/normal/eclipse"

def run(cmd, cwd=None, env=None):
    print(f"[cmd] {' '.join(cmd)}" + (f"  (cwd={cwd})" if cwd else ""))
    return subprocess.run(cmd, cwd=cwd, env=env)

def win_join(*parts):
    return os.path.join(*parts)

def find_existing(dirname_root, hint_name):
    root = Path(dirname_root)
    if root.name.startswith(hint_name):
        return root
    for p in root.iterdir():
        if p.name.startswith(hint_name):
            return p
    return None

def ensure_env_file(env_path, gh, java_home):
    if env_path.exists():
        print(f"[*] {env_path} already exists, keeping it")
        return False
    text = "\n".join([
        "# DeepZero runtime config (created by setup_deepzero.py)",
        "",
        "# any one of these enables LLM assessment (claude-code CLI needs none)",
        "GEMINI_API_KEY=",
        "OPENAI_API_KEY=",
        "ANTHROPIC_API_KEY=",
        "OPENROUTER_API_KEY=",
        "",
    ]) + f"GHIDRA_INSTALL_DIR={gh}\n" + f"JAVA_HOME={java_home}\n"
    env_path.write_text(text, encoding="utf-8")
    print(f"[*] wrote {env_path}")
    return True

def main():
    ap = argparse.ArgumentParser(description="Install DeepZero + its toolchain into a top-level dir")
    ap.add_argument("--root", default="deepzero_toolchain", help="parent directory for everything")
    ap.add_argument("--skip-ghidra", action="store_true", help="skip Ghidra download")
    ap.add_argument("--skip-java", action="store_true", help="skip embedded JDK download")
    ap.add_argument("--skip-semgrep", action="store_true", help="skip semgrep pip install")
    ap.add_argument("--env-path", default=None, help="where to write .env (default <root>/.env)")
    args = ap.parse_args()

    root = Path(args.root).resolve()
    root.mkdir(parents=True, exist_ok=True)

    deepzero_dir = root / "DeepZero"
    venv_dir = root / "venv"
    tool_dir = root / "tools"
    tool_dir.mkdir(parents=True, exist_ok=True)

    python = sys.executable

    if deepzero_dir.exists():
        print(f"[*] DeepZero already at {deepzero_dir}")
    else:
        r = run(["git", "clone", "--depth", "1", DEEPZERO_REPO, str(deepzero_dir)], cwd=str(root))
        if r.returncode != 0:
            sys.exit("[!] git clone failed")

    if not (venv_dir / ("Scripts/python.exe" if os.name == "nt" else "bin/python")).exists():
        print("[*] creating venv")
        run([python, "-m", "venv", str(venv_dir)])
    venv_py = venv_dir / ("Scripts/python.exe" if os.name == "nt" else "bin/python")
    venv_pip = [str(venv_py), "-m", "pip"]
    venv_run = venv_dir / ("Scripts/deepzero.exe" if os.name == "nt" else "bin/deepzero")

    print("[*] installing deepzero[full]")
    r = run(venv_pip + ["install", "-e", ".[full]"], cwd=str(deepzero_dir))
    if r.returncode != 0:
        sys.exit("[!] deepzero install failed")

    if not args.skip_semgrep:
        print("[*] installing semgrep (needed by semgrep_scanner)")
        r = run(venv_pip + ["install", "semgrep"])
        if r.returncode != 0:
            print("[!] semgrep install failed (scanner stage will error later)")

    ghidra_dir = None
    if not args.skip_ghidra:
        print("[*] locating/installing Ghidra")
        existing = find_existing(tool_dir, "ghidra")
        if existing:
            ghidra_dir = existing
            print(f"[*] found {ghidra_dir}")
        else:
            zip_path = tool_dir / "ghidra.zip"
            print(f"[*] downloading {GHIDRA_URL}")
            subprocess.run(["powershell", "-NoProfile", "-Command",
                            f"Invoke-WebRequest -Uri '{GHIDRA_URL}' -OutFile '{zip_path}'"], check=True)
            print("[*] extracting (this takes a while)")
            with zipfile.ZipFile(zip_path) as zf:
                zf.extractall(tool_dir)
            zip_path.unlink(missing_ok=True)
            ghidra_dir = find_existing(tool_dir, "ghidra")
            if not ghidra_dir:
                sys.exit("[!] Ghidra extraction produced no dir")

    java_home = None
    if not args.skip_java:
        print("[*] locating/installing embedded JDK 17")
        existing = find_existing(tool_dir, "jdk")
        if existing:
            java_home = existing
            print(f"[*] found {java_home}")
        else:
            jdk_zip = tool_dir / "jdk.zip"
            print(f"[*] downloading JDK 17 from {ADOPTIUM_API}")
            subprocess.run(["powershell", "-NoProfile", "-Command",
                            f"Invoke-WebRequest -Uri '{ADOPTIUM_API}' -OutFile '{jdk_zip}'"], check=True)
            with zipfile.ZipFile(jdk_zip) as zf:
                zf.extractall(tool_dir)
            jdk_zip.unlink(missing_ok=True)
            java_home = find_existing(tool_dir, "jdk")
            if not java_home:
                sys.exit("[!] JDK extraction produced no dir")

    env_path = Path(args.env_path or root / ".env")
    ensure_env_file(env_path, str(ghidra_dir) if ghidra_dir else "", str(java_home) if java_home else "")

    print("\n[*] setup complete")
    print(f"    deepzero CLI : {venv_run}")
    print(f"    venv python  : {venv_py}")
    print(f"    env file     : {env_path}")
    print(f"    GHIDRA_INSTALL_DIR : {ghidra_dir}")
    print(f"    JAVA_HOME    : {java_home}")
    print("\n    Add an LLM key to the env file (or use Claude Code CLI) before running.")

if __name__ == "__main__":
    main()