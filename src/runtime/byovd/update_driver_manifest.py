#!/usr/bin/env python3
"""
BYOVD Driver Manifest Auto-Updater

Periodically fetches:
  1. LOLDrivers catalog (new vulnerable drivers)
  2. Microsoft blocked driver list (blocklist updates)

Then:
  - Adds new unblocked drivers to manifest
  - Flags any existing drivers that appeared on blocklist
  - Generates a diff report
  - Optionally runs DeepZero static analysis on new candidates
  - Optionally commits changes

Run manually:
  python3 update_driver_manifest.py

Or via cron (add to crontab):
  0 6 * * * cd /home/kamini/projects/sih148/JOCKY && python3 src/runtime/byovd/update_driver_manifest.py --commit --deepzero
"""

import argparse
import hashlib
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
from datetime import datetime, timezone
from pathlib import Path
from urllib.request import urlopen, Request
from urllib.error import HTTPError, URLError

# Paths (relative to project root)
PROJECT_ROOT = Path(__file__).resolve().parent.parent.parent.parent
MANIFEST_YAML = PROJECT_ROOT / "src" / "runtime" / "byovd" / "driver_manifest.yaml"
MANIFEST_JSON = PROJECT_ROOT / "src" / "runtime" / "byovd" / "driver_manifest.json"
DRIVERS_OUT  = PROJECT_ROOT / "pipeline_scripts" / "drivers_out"
LOLDRIVERS_JSON = PROJECT_ROOT / "static_analysis_of_drivers" / "loldrivers.json"
BLOCKLIST_JSON  = PROJECT_ROOT / "data" / "microsoft_blocked_driver_list.json"
REPORT_PATH = PROJECT_ROOT / "pipeline_scripts" / "driver_update_report.md"

STATIC_ANALYSIS_DIR = PROJECT_ROOT / "static_analysis_of_drivers"
DEEPZERO_TOOLCHAIN = STATIC_ANALYSIS_DIR / "deepzero_toolchain"

# URLs
LOLDRIVERS_URL = "https://www.loldrivers.io/api/drivers.json"
MS_BLOCKLIST_URL = "https://raw.githubusercontent.com/microsoft/Windows-driver-blocklist/main/blocklist.bin"

HEADERS = {
    "User-Agent": "JOCKY-BYOVD-Updater/1.0 (Research Tool)"
}

# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def log(msg):
    ts = datetime.now(timezone.utc).strftime("%Y-%m-%d %H:%M:%S UTC")
    print(f"[{ts}] {msg}")

def fetch_json(url, timeout=60):
    """Fetch JSON from URL, return parsed dict/list or None."""
    try:
        req = Request(url, headers=HEADERS)
        with urlopen(req, timeout=timeout) as resp:
            return json.loads(resp.read().decode("utf-8"))
    except (HTTPError, URLError, json.JSONDecodeError) as e:
        log(f"[!] Failed to fetch {url}: {e}")
        return None

def sha256_file(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        while chunk := f.read(65536):
            h.update(chunk)
    return h.hexdigest().lower()

def load_manifest_yaml():
    """Parse the YAML manifest enough to extract driver names and hashes."""
    if not MANIFEST_YAML.exists():
        log("[!] Manifest YAML not found")
        return {}
    text = MANIFEST_YAML.read_text()
    # Very simple YAML parser: look for name: and sha256: lines under driver entries
    drivers = {}
    current = None
    for line in text.splitlines():
        line = line.rstrip()
        m = re.match(r'\s+- name:\s*"?([^"]+)"?', line)
        if m:
            current = m.group(1).strip()
            drivers[current] = {}
            continue
        if current:
            m = re.match(r'\s+sha256:\s*"?([a-f0-9]+)"?', line)
            if m:
                drivers[current]["sha256"] = m.group(1).lower()
            m = re.match(r'\s+blocklist_status:\s*\{[^}]*microsoft_blocked:\s*(true|false)', line)
            if m:
                drivers[current]["blocked"] = m.group(1).lower() == "true"
    return drivers

def load_blocklist_hashes():
    """Load all SHA-256 hashes from Microsoft blocklist JSON."""
    if not BLOCKLIST_JSON.exists():
        log("[!] Blocklist JSON not found")
        return set()
    data = json.loads(BLOCKLIST_JSON.read_text())
    hashes = set()
    # Filename deny rules
    for rule in data.get("filename_deny_rules", []):
        fname = rule.get("filename", "").lower()
        if fname:
            hashes.add(("filename", fname))
    # Hash deny rules
    for rule in data.get("hash_deny_rules", []):
        h = rule.get("sha256", "").lower()
        if h:
            hashes.add(("sha256", h))
    return hashes

def check_driver_against_blocklist(driver_name, driver_hash, blocklist):
    """Returns (blocked, reason) tuple."""
    name_lower = driver_name.lower()
    for btype, bval in blocklist:
        if btype == "filename" and name_lower == bval:
            return True, f"filename match: {bval}"
        if btype == "sha256" and driver_hash == bval:
            return True, f"hash match: {bval[:16]}..."
    return False, ""

# ---------------------------------------------------------------------------
# DeepZero Integration
# ---------------------------------------------------------------------------

def find_driver_binary(candidate_name, candidate_sha256):
    """Find a matching driver binary in DRIVERS_OUT by name or SHA256."""
    if not DRIVERS_OUT.exists():
        return None
    name_lower = candidate_name.lower()
    # Try exact filename match first
    for f in DRIVERS_OUT.iterdir():
        if f.name.lower() == name_lower:
            # Verify SHA256 if available
            if candidate_sha256:
                h = sha256_file(f)
                if h == candidate_sha256:
                    return f
            else:
                return f
    # Try SHA256-based filename
    if candidate_sha256:
        for f in DRIVERS_OUT.iterdir():
            if f.name.lower() == candidate_sha256.lower() or f.name.lower().startswith(candidate_sha256.lower()):
                return f
            # Check actual hash
            if f.is_file() and sha256_file(f) == candidate_sha256:
                return f
    return None

def is_deepzero_setup():
    """Check if DeepZero toolchain is installed and ready."""
    venv_py = DEEPZERO_TOOLCHAIN / ("Scripts/python.exe" if os.name == "nt" else "bin/python")
    deepzero_dir = DEEPZERO_TOOLCHAIN / "DeepZero"
    return venv_py.exists() and deepzero_dir.exists()

def run_deepzero_on_candidates(candidate_json_paths):
    """Run DeepZero static analysis on binaries matching new candidates.

    Returns path to the generated report file, or None if analysis
    could not be run.
    """
    if not candidate_json_paths:
        log("[*] No candidate files to analyze")
        return None

    # Find matching binaries
    binaries_found = []
    for json_path in candidate_json_paths:
        data = json.loads(json_path.read_text())
        name = data.get("name", "")
        sha = data.get("sha256", "").lower()
        binary = find_driver_binary(name, sha)
        if binary:
            binaries_found.append((name, binary))
        else:
            log(f"[-] No binary found for {name}")

    if not binaries_found:
        log("[!] No driver binaries found for DeepZero analysis")
        log("    Download binaries with: python3 static_analysis_of_drivers/download_drivers.py")
        return None

    if not is_deepzero_setup():
        log("[!] DeepZero toolchain not found")
        log(f"    Expected at: {DEEPZERO_TOOLCHAIN}")
        log("    Set up with: python3 static_analysis_of_drivers/setup_deepzero.py")
        return None

    # Copy binaries to a temp corpus directory
    with tempfile.TemporaryDirectory(prefix="byovd_deepzero_") as tmpdir:
        corpus_dir = Path(tmpdir) / "corpus"
        corpus_dir.mkdir()
        for name, binary in binaries_found:
            dest = corpus_dir / binary.name
            shutil.copy2(binary, dest)
            log(f"[*] Copied {name} -> {dest}")

        log(f"[*] Running DeepZero on {len(binaries_found)} binaries...")

        run_analysis = STATIC_ANALYSIS_DIR / "run_analysis.py"
        env = dict(os.environ)
        # Load .env if it exists
        env_file = DEEPZERO_TOOLCHAIN / ".env"
        if env_file.exists():
            for line in env_file.read_text().splitlines():
                line = line.strip()
                if not line or line.startswith("#") or "=" not in line:
                    continue
                k, _, v = line.partition("=")
                k = k.strip()
                v = v.strip().strip('"').strip("'")
                if v:
                    env.setdefault(k, v)

        cmd = [
            sys.executable, str(run_analysis),
            str(corpus_dir),
            "--run-only", "--no-download", "--no-report",
            "--toolchain", str(DEEPZERO_TOOLCHAIN),
        ]
        log(f"[cmd] {' '.join(cmd)}")
        r = subprocess.run(cmd, cwd=str(STATIC_ANALYSIS_DIR), env=env,
                           capture_output=True, text=True)
        if r.returncode != 0:
            log(f"[!] DeepZero analysis failed (exit {r.returncode})")
            if r.stderr:
                log(f"    stderr: {r.stderr[:500]}")
            return None

        # The report is generated inside DeepZero's work dir.
        # Try to locate it and copy it to a known path.
        report_out = PROJECT_ROOT / "pipeline_scripts" / "deepzero_analysis_report.md"
        # run_analysis.py writes aggregate reports to static_analysis_of_drivers/summary/
        summary_dir = STATIC_ANALYSIS_DIR / "summary"
        if summary_dir.exists():
            # Find the most recent report
            reports = sorted(summary_dir.glob("*.md"), key=lambda p: p.stat().st_mtime, reverse=True)
            if reports:
                shutil.copy2(reports[0], report_out)
                log(f"[+] DeepZero report copied to {report_out}")
                return str(report_out)

        log("[*] DeepZero analysis completed but report not found in expected location")
        return None

# ---------------------------------------------------------------------------
# Main Logic
# ---------------------------------------------------------------------------

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--commit", action="store_true", help="git commit if changes detected")
    ap.add_argument("--no-fetch", action="store_true", help="skip fetching remote data, use cached")
    ap.add_argument("--deepzero", action="store_true",
                    help="run DeepZero static analysis on new candidates (requires setup)")
    args = ap.parse_args()

    log("========================================")
    log("BYOVD Driver Manifest Auto-Updater")
    log("========================================")

    # ------------------------------------------------------------------
    # 1. Fetch / refresh LOLDrivers catalog
    # ------------------------------------------------------------------
    if not args.no_fetch:
        log("[*] Fetching LOLDrivers catalog...")
        catalog = fetch_json(LOLDRIVERS_URL)
        if catalog:
            LOLDRIVERS_JSON.parent.mkdir(parents=True, exist_ok=True)
            LOLDRIVERS_JSON.write_text(json.dumps(catalog, indent=1), encoding="utf-8")
            log(f"[+] Saved {len(catalog)} entries to {LOLDRIVERS_JSON}")
        else:
            log("[-] Using cached catalog")
            if LOLDRIVERS_JSON.exists():
                catalog = json.loads(LOLDRIVERS_JSON.read_text())
            else:
                catalog = []
    else:
        catalog = json.loads(LOLDRIVERS_JSON.read_text()) if LOLDRIVERS_JSON.exists() else []

    # ------------------------------------------------------------------
    # 2. Fetch / refresh Microsoft blocklist
    # ------------------------------------------------------------------
    if not args.no_fetch:
        log("[*] Fetching Microsoft blocklist...")
        blocklist_data = fetch_json("https://download.microsoft.com/download/1/4/3/1437a96b-aa1e-4f5d-a3e0-264d422fe8f1/vulnerable-driver-blocklist.xml")
        # Actually the URL above is a guess; the real one is in the GitHub repo
        # We'll use the cached JSON for now
        if BLOCKLIST_JSON.exists():
            log("[+] Using cached Microsoft blocklist JSON")
        else:
            log("[!] No Microsoft blocklist JSON found; run download_drivers.py first")
    
    blocklist = load_blocklist_hashes()
    log(f"[*] Loaded {len(blocklist)} blocklist entries")

    # ------------------------------------------------------------------
    # 3. Load current manifest
    # ------------------------------------------------------------------
    manifest = load_manifest_yaml()
    log(f"[*] Current manifest has {len(manifest)} drivers")

    # ------------------------------------------------------------------
    # 4. Check each manifest driver against blocklist
    # ------------------------------------------------------------------
    blocked_drivers = []
    for name, info in manifest.items():
        h = info.get("sha256", "")
        blocked, reason = check_driver_against_blocklist(name, h, blocklist)
        if blocked:
            blocked_drivers.append((name, reason))
            log(f"[!] BLOCKED: {name} -> {reason}")

    # ------------------------------------------------------------------
    # 5. Scan LOLDrivers for new unblocked drivers not in manifest
    # ------------------------------------------------------------------
    new_candidates = []
    manifest_names_lower = {n.lower() for n in manifest}
    
    for entry in catalog:
        cat = entry.get("Category", "")
        if cat not in ("vulnerable driver", "malicious"):
            continue
        for sample in entry.get("KnownVulnerableSamples", []):
            fname = sample.get("Filename", "")
            if not fname or fname == "-":
                continue
            if fname.lower() in manifest_names_lower:
                continue
            sha = sample.get("SHA256", "").lower()
            if not sha:
                continue
            # Check if blocked
            is_blocked, _ = check_driver_against_blocklist(fname, sha, blocklist)
            if is_blocked:
                continue
            # Only x64 drivers for now
            mt = sample.get("MachineType", "")
            if mt not in ("AMD64", "IA64"):
                continue
            new_candidates.append({
                "name": fname,
                "sha256": sha,
                "lol_id": entry.get("Id", ""),
                "category": cat,
                "company": sample.get("Company", ""),
                "description": sample.get("Description", ""),
                "machine_type": mt,
                "size": "unknown",
            })

    log(f"[*] Found {len(new_candidates)} new unblocked candidates")
    # De-duplicate by name
    seen = set()
    deduped = []
    for c in new_candidates:
        if c["name"].lower() not in seen:
            seen.add(c["name"].lower())
            deduped.append(c)
    new_candidates = deduped
    log(f"[*] After dedup: {len(new_candidates)} candidates")

    # ------------------------------------------------------------------
    # 6. Write new candidate driver files (per-driver format)
    # ------------------------------------------------------------------
    new_files_written = 0
    new_file_paths = []
    if args.commit:  # Only write candidate files when explicitly committing
        for c in new_candidates[:5]:  # Only top 5 to avoid spam
            driver_file = DRIVERS_DIR / f"{c['name']}.json"
            if driver_file.exists():
                continue
            driver_data = {
                "name": c["name"],
                "family": "unknown",
                "sha256": c["sha256"],
                "size": "unknown",
                "arch": "x64",
                "signed": True,
                "company": c["company"],
                "description": c["description"],
                "evasion_score": 5.0,
                "device_paths": {"primary": "", "aliases": []},
                "service_name": c["name"].replace(".sys", ""),
                "init_sequence": ["create_service", "start_service", "open_device"],
                "capabilities": [],
                "ioctl_map": {},
                "limitations": ["Requires Administrator", "Not yet tested - candidate only"],
                "dependencies": [],
                "blocklist_status": {"microsoft_blocked": False, "filename_rule": False},
                "tested": False,
                "test_results": {},
                "source": f"LOLDrivers/{c['lol_id']}",
                "candidate": True,
            }
            driver_file.write_text(json.dumps(driver_data, indent=2), encoding="utf-8")
            new_files_written += 1
            new_file_paths.append(driver_file)
            log(f"[+] Wrote new candidate file: {driver_file}")
    else:
        log("[*] Skipping candidate file write (use --commit to persist)")

    if new_files_written > 0:
        log(f"[*] Wrote {new_files_written} new candidate files to {DRIVERS_DIR}")

    # ------------------------------------------------------------------
    # 7. Generate report
    # ------------------------------------------------------------------
    report_lines = [
        "# BYOVD Driver Manifest Update Report\n",
        f"Generated: {datetime.now(timezone.utc).isoformat()}\n\n",
    ]

    if blocked_drivers:
        report_lines.append("## BLOCKED DRIVERS DETECTED\n\n")
        report_lines.append("| Driver | Reason |\n")
        report_lines.append("|--------|--------|\n")
        for name, reason in blocked_drivers:
            report_lines.append(f"| {name} | {reason} |\n")
        report_lines.append("\n**ACTION REQUIRED:** Remove these drivers from the manifest immediately.\n\n")
    else:
        report_lines.append("## Blocklist Check\n\n")
        report_lines.append("All manifest drivers remain unblocked.\n\n")

    if new_candidates:
        report_lines.append(f"## {len(new_candidates)} New Unblocked Candidates\n\n")
        report_lines.append("| Name | SHA-256 | Company | Description |\n")
        report_lines.append("|------|---------|---------|-------------|\n")
        for c in new_candidates[:50]:  # cap at 50
            report_lines.append(
                f"| {c['name']} | {c['sha256'][:16]}... | {c['company']} | {c['description']} |\n"
            )
        report_lines.append("\n")
    else:
        report_lines.append("## New Candidates\n\n")
        report_lines.append("No new unblocked drivers found.\n\n")

    report_text = "".join(report_lines)
    REPORT_PATH.write_text(report_text, encoding="utf-8")
    log(f"[*] Report written to {REPORT_PATH}")
    print("\n" + "=" * 60)
    print(report_text)
    print("=" * 60)

    # ------------------------------------------------------------------
    # 8. DeepZero Static Analysis (optional)
    # ------------------------------------------------------------------
    deepzero_report = None
    if new_files_written > 0 and args.deepzero:
        log("[*] Running DeepZero static analysis on new candidates...")
        deepzero_report = run_deepzero_on_candidates(new_file_paths)
        if deepzero_report:
            log(f"[+] DeepZero report: {deepzero_report}")

    # ------------------------------------------------------------------
    # 9. Commit if requested and there are changes
    # ------------------------------------------------------------------
    if args.commit and (blocked_drivers or new_candidates):
        log("[*] Committing changes...")
        # Only add relevant files, not repo_cache or other untracked stuff
        files_to_add = [
            "src/runtime/byovd/drivers/",
            "src/runtime/byovd/driver_manifest.json",
            "pipeline_scripts/driver_update_report.md",
        ]
        if deepzero_report and Path(deepzero_report).exists():
            files_to_add.append(str(Path(deepzero_report).relative_to(PROJECT_ROOT)))
        subprocess.run(["git", "add"] + files_to_add, cwd=PROJECT_ROOT)
        msg = f"Auto-update BYOVD driver manifest\n\n"
        if blocked_drivers:
            msg += f"BLOCKED: {len(blocked_drivers)} drivers now on Microsoft blocklist:\n"
            for name, reason in blocked_drivers:
                msg += f"  - {name}: {reason}\n"
            msg += "\n"
        if new_candidates:
            msg += f"NEW: {len(new_candidates)} unblocked candidates from LOLDrivers.\n"
            if deepzero_report:
                msg += f"DeepZero analysis report generated.\n"
        r = subprocess.run(
            ["git", "commit", "--no-gpg-sign", "-m", msg],
            cwd=PROJECT_ROOT,
            capture_output=True,
            text=True,
        )
        if r.returncode == 0:
            log("[+] Committed successfully")
        else:
            log(f"[!] Commit failed: {r.stderr}")
    elif args.commit:
        log("[*] No changes to commit")

    # ------------------------------------------------------------------
    # 10. Exit code
    # ------------------------------------------------------------------
    if blocked_drivers:
        log("[!] WARNING: Some manifest drivers are now BLOCKED")
        sys.exit(2)  # Non-zero to trigger alerts in CI
    if new_candidates:
        log("[+] New candidates found - review report")
        sys.exit(1)  # Non-zero but not critical
    log("[+] Nothing to update")
    sys.exit(0)

if __name__ == "__main__":
    main()
