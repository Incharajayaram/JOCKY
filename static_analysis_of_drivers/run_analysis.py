import argparse
import hashlib
import os
import re
import shutil
import subprocess
import sys
from pathlib import Path

THIS_DIR = Path(__file__).resolve().parent
PIPELINE_NAME = "loldrivers_corpus"
PIPELINE_SRC = THIS_DIR / "pipelines" / PIPELINE_NAME

def corpus_segment(target: Path) -> str:
    resolved = target.expanduser().resolve()
    key_src = str(resolved).lower() if os.name == "nt" else str(resolved)
    digest = hashlib.sha256(key_src.encode("utf-8")).hexdigest()[:8]
    base = resolved.name or "root"
    base = re.sub(r"[^A-Za-z0-9._-]", "_", base)[:48].strip("._-") or "corpus"
    return f"{base}-{digest}"

def load_env(path: Path):
    if not path.exists():
        return
    for line in path.read_text(encoding="utf-8").splitlines():
        line = line.strip()
        if not line or line.startswith("#") or "=" not in line:
            continue
        k, _, v = line.partition("=")
        k = k.strip()
        v = v.strip().strip('"').strip("'")
        os.environ.setdefault(k, v)

def run(cmd, cwd=None, check=True):
    print(f"[cmd] {' '.join(str(c) for c in cmd)}" + (f"  (cwd={cwd})" if cwd else ""))
    r = subprocess.run([str(c) for c in cmd], cwd=cwd)
    if check and r.returncode != 0:
        sys.exit(f"[!] command failed: exit {r.returncode}")
    return r

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("corpus", help="directory of .sys drivers (e.g. drivers_out)")
    ap.add_argument("--no-download", action="store_true", help="skip sample download step")
    ap.add_argument("--toolchain", default=None,
                    help="deepzero toolchain dir (default deepzero_toolchain)")
    ap.add_argument("--work-dir", default=None,
                    help="base work dir; run land in <it>/loldrivers_corpus/<corpus-key>")
    ap.add_argument("--model", default=None, help="llm model override for the run")
    ap.add_argument("--interactive-model", default="openai/gpt-4o",
                    help="model for interactive repl (default openai/gpt-4o)")
    ap.add_argument("--no-report", action="store_true", help="skip report + aggregate")
    ap.add_argument("--interactive", action="store_true", help="drop into deepzero interactive after run")
    ap.add_argument("--run-only", action="store_true", help="assume setup/download already done")
    ap.add_argument("--compile", action="store_true",
                    help="run deepzero validate on the installed pipeline and exit")
    args = ap.parse_args()

    corpus = Path(args.corpus).resolve()
    downloading = not args.run_only and not args.no_download
    if not corpus.is_dir():
        if downloading:
            corpus.mkdir(parents=True, exist_ok=True)
        else:
            sys.exit(f"corpus dir not found: {corpus}")

    toolchain = Path(args.toolchain or THIS_DIR / "deepzero_toolchain").resolve()
    deepzero_root = toolchain / "DeepZero"
    venv = toolchain / "venv"
    venv_py = venv / ("Scripts/python.exe" if os.name == "nt" else "bin/python")
    deepzero_cli = venv / ("Scripts/deepzero.exe" if os.name == "nt" else "bin/deepzero")

    if not args.run_only:
        if not args.no_download:
            print("== step 1: download samples ==")
            run([sys.executable, str(THIS_DIR / "download_drivers.py"),
                 "--out", str(corpus)], cwd=THIS_DIR)
        print("== step 2: ensure DeepZero toolchain ==")
        run([sys.executable, str(THIS_DIR / "setup_deepzero.py"),
             "--root", str(toolchain)], cwd=THIS_DIR)

    if not venv_py.exists():
        sys.exit(f"venv missing: {venv_py} - run without --run-only")
    if not deepzero_root.exists():
        sys.exit(f"DeepZero missing: {deepzero_root} - run without --run-only")

    env = dict(os.environ)
    load_env(toolchain / ".env")

    pipe_target = deepzero_root / "pipelines" / PIPELINE_NAME / "pipeline.yaml"
    if PIPELINE_SRC.exists():
        pipe_target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(PIPELINE_SRC / "pipeline.yaml", pipe_target)
    else:
        sys.exit(f"pipeline source missing: {PIPELINE_SRC}")

    if args.compile:
        print("== pipeline validate ==")
        r = subprocess.run([str(deepzero_cli), "validate", str(pipe_target)], cwd=str(deepzero_root), env=env)
        sys.exit(r.returncode)

    base_work = Path(args.work_dir or THIS_DIR / "work").resolve()
    run_dir = base_work / PIPELINE_NAME / corpus_segment(corpus)

    print(f"== run ==")
    print(f"   pipeline: {PIPELINE_NAME}")
    print(f"   corpus  : {corpus}")
    print(f"   run dir : {run_dir}")
    cmd = [str(deepzero_cli), "run", str(corpus), "-p", str(pipe_target),
           "-w", str(base_work)]
    if args.model:
        cmd += ["-m", args.model]
    run(cmd, cwd=str(deepzero_root), env=env)

    if args.no_report:
        return

    print("== status ==")
    run([str(deepzero_cli), "status", "-w", str(run_dir)], cwd=str(deepzero_root), env=env)

    print("== report ==")
    r = subprocess.run([str(deepzero_cli), "report", "-w", str(run_dir)],
                       cwd=str(deepzero_root), env=env, capture_output=True, text=True)
    print(r.stdout)
    if r.returncode != 0:
        print(r.stderr, file=sys.stderr)
    else:
        m = re.search(r"report written to\s+(\S+)", r.stdout)
        if m:
            print(f"   open: {m.group(1)}")

    print("== aggregate ==")
    agg_out = THIS_DIR / "summary"
    run([sys.executable, str(THIS_DIR / "aggregate_reports.py"),
         "--run-dir", str(run_dir), "--out", str(agg_out)], cwd=THIS_DIR)

    if args.interactive:
        print("== interactive ==")
        subprocess.run([str(deepzero_cli), "interactive", "-w", str(run_dir),
                        "-m", args.interactive_model], cwd=str(deepzero_root), env=env)

    print("\n[done]")

if __name__ == "__main__":
    main()