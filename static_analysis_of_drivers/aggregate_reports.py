import argparse
import csv
import json
import sys
from datetime import datetime, timezone
from pathlib import Path

def find_newest_run(base: Path):
    if (base / "run.json").exists():
        return base
    cands = [d for d in base.iterdir() if d.is_dir() and (d / "run.json").exists()]
    if not cands:
        return None
    return max(cands, key=lambda d: (d / "run.json").stat().st_mtime)

def read_json(path: Path):
    if not path.exists():
        return None
    try:
        return json.loads(path.read_text(encoding="utf-8"))
    except (json.JSONDecodeError, OSError):
        return None

def flatten_stage_data(stage: dict):
    out = {}
    for k, v in (stage.get("data") or {}).items():
        if isinstance(v, (dict, list)):
            v = json.dumps(v, default=str)
        out[f"stage:{k}"] = v
    return out

def main():
    ap = argparse.ArgumentParser(description="Aggregate a deepzero run dir into one CSV + JSON")
    ap.add_argument("--base-work-dir", default=None,
                    help="dir containing corpus subdirs (e.g. <abs>/loldrivers_corpus); newest run dir is used")
    ap.add_argument("--run-dir", default=None, help="exact run directory (overrides --base-work-dir)")
    ap.add_argument("--out", default="summary", help="output stem (summary.csv / summary.json)")
    args = ap.parse_args()

    if args.run_dir:
        run_dir = Path(args.run_dir).resolve()
    elif args.base_work_dir:
        run_dir = find_newest_run(Path(args.base_work_dir).resolve())
    else:
        sys.exit("pass --run-dir or --base-work-dir")

    if run_dir is None or not run_dir.exists():
        sys.exit(f"no run directory found under {args.base_work_dir or args.run_dir}")

    print(f"[*] run dir: {run_dir}")

    run = read_json(run_dir / "run.json") or {}
    manifest = read_json(run_dir / "run_manifest.json") or {}
    inv_path = run_dir / "report" / "inventory.csv"
    findings_path = run_dir / "report" / "findings.jsonl"
    report_json = read_json(run_dir / "report" / "report.json")

    rows = []
    samples_dir = run_dir / "samples"
    if samples_dir.is_dir():
        for sdir in sorted(samples_dir.iterdir()):
            if not sdir.is_dir():
                continue
            st = read_json(sdir / "state.json")
            if st is None:
                continue
            row = {
                "sample_id": st.get("sample_id", sdir.name),
                "filename": st.get("filename", ""),
                "sha256": st.get("sha256", ""),
                "source_path": st.get("source_path", ""),
                "verdict": st.get("verdict", ""),
                "current_stage": st.get("current_stage", ""),
                "error": st.get("error", ""),
            }
            history = st.get("history") or {}
            for name, stage in history.items():
                status = stage.get("status", "")
                row[f"{name}:status"] = status
                flat = flatten_stage_data(stage)
                for k, v in flat.items():
                    row[f"{name}:{k}"] = v
                if stage.get("error"):
                    row[f"{name}:error"] = stage.get("error")
                if stage.get("skip_reason"):
                    row[f"{name}:skip_reason"] = stage.get("skip_reason")
                artifacts = stage.get("artifacts") or {}
                for k, v in artifacts.items():
                    row[f"{name}:art:{k}"] = v
            rows.append(row)

    findings = []
    if findings_path.exists():
        for line in findings_path.read_text(encoding="utf-8").splitlines():
            line = line.strip()
            if not line:
                continue
            try:
                findings.append(json.loads(line))
            except json.JSONDecodeError:
                continue

    inventory = []
    if inv_path.exists():
        with inv_path.open(newline="", encoding="utf-8") as fh:
            inventory = list(csv.DictReader(fh))

    buckets = (report_json or {}).get("buckets") or {}
    vulnerable = buckets.get("vulnerable")
    if vulnerable is None:
        vulnerable = sum(1 for r in rows
                         if str(r.get("assess:classification", "")).lower() == "vulnerable")

    out_path = Path(args.out)
    summary = {
        "generated_at": datetime.now(timezone.utc).isoformat(),
        "run_dir": str(run_dir),
        "run": run,
        "manifest": manifest,
        "report": report_json,
        "totals": {
            "samples": len(rows),
            "findings": len(findings),
            "vulnerable": vulnerable,
            "buckets": buckets,
        },
        "rows": rows,
        "findings": findings,
        "inventory": inventory,
        "findings_count": len(findings),
    }
    summary["totals"]["completed"] = sum(1 for r in rows if r.get("verdict") == "completed")
    summary["totals"]["filtered"] = sum(1 for r in rows if r.get("verdict") == "filtered")
    summary["totals"]["failed"] = sum(1 for r in rows if r.get("verdict") == "failed")

    json_out = out_path.with_suffix(".json") if out_path.suffix else Path(f"{out_path}.json")
    json_out.write_text(json.dumps(summary, indent=2, default=str), encoding="utf-8")
    print(f"[*] wrote {json_out}")

    if rows:
        csv_out = json_out.with_suffix(".csv")
        fieldnames = []
        for r in rows:
            for k in r:
                if k not in fieldnames:
                    fieldnames.append(k)
        with csv_out.open("w", newline="", encoding="utf-8") as fh:
            writer = csv.DictWriter(fh, fieldnames=fieldnames, extrasaction="ignore")
            writer.writeheader()
            writer.writerows(rows)
        print(f"[*] wrote {csv_out}")

    print(f"[*] {summary['totals']['samples']} samples, {len(findings)} findings, "
          f"{summary['totals']['vulnerable']} vulnerable")

if __name__ == "__main__":
    main()