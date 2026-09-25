import argparse
import hashlib
import io
import json
import os
import sys
import time
import zipfile
from datetime import datetime, timezone
from pathlib import Path

try:
    import requests
except ImportError:
    sys.exit("Missing dependency: pip install requests")

MB_URL = "https://mb-api.abuse.ch/api/v1/"
MB_QUERY_LIMIT = 1000

def mb_download(sha256, api_key, session):
    r = session.post(MB_URL, data={"query": "get_file", "sha256_hash": sha256},
                     headers={"Auth-Key": api_key}, timeout=60)
    ctype = r.headers.get("Content-Type", "")
    if r.status_code == 401:
        raise RuntimeError("MalwareBazaar authentication failed (bad/expired API key)")
    if "application/json" in ctype or r.text.lstrip().startswith("{"):
        try:
            j = r.json()
            if j.get("query_status") == "file_not_found":
                return None, "file_not_found"
            if j.get("query_status") != "ok":
                return None, j.get("query_status", "unknown")
        except Exception:
            pass
    if "application/zip" not in ctype and not r.content[:2] == b"PK":
        return None, "unexpected content-type"
    return r.content, "ok"

def extract_sys(zip_bytes, sha256):
    with zipfile.ZipFile(io.BytesIO(zip_bytes)) as zf:
        for name in zf.namelist():
            if name.lower().endswith(".sys"):
                return name, zf.read(name)
    return None, None

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--max-downloads", type=int, default=50, help="max drivers to download")
    ap.add_argument("--out", default="drivers_out", help="output directory")
    ap.add_argument("--delay", type=float, default=0.3, help="seconds between requests")
    args = ap.parse_args()

    api_key = os.environ.get("MALWAREBAZAAR_API_KEY")
    if not api_key:
        sys.exit("[!] Set MALWAREBAZAAR_API_KEY env var")

    hashes_file = Path("sha256_list.txt")
    if not hashes_file.exists():
        sys.exit("[!] sha256_list.txt not found. Run download_drivers.py --only-list first.")

    hashes = [h.strip() for h in hashes_file.read_text().splitlines() if h.strip()]
    out_dir = Path(args.out)
    out_dir.mkdir(parents=True, exist_ok=True)

    session = requests.Session()
    n_ok = n_missing = n_skip = 0
    manifest = []

    for sha256 in hashes:
        if n_ok >= args.max_downloads:
            print(f"[*] reached max downloads ({args.max_downloads}), stopping")
            break

        dest = out_dir / f"{sha256}.sys"
        if dest.exists():
            print(f"  [-] skip (exists): {sha256}")
            n_skip += 1
            manifest.append({"sha256": sha256, "status": "skipped", "path": str(dest)})
            continue

        try:
            content, status = mb_download(sha256, api_key, session)
            if content is None:
                print(f"  [-] missing: {sha256} ({status})")
                n_missing += 1
                manifest.append({"sha256": sha256, "status": "missing"})
                continue

            sys_name, data = extract_sys(content, sha256)
            if data is None:
                print(f"  [-] no .sys in zip: {sha256}")
                n_missing += 1
                manifest.append({"sha256": sha256, "status": "no_sys"})
                continue

            dest.write_bytes(data)
            digest = hashlib.sha256(data).hexdigest()
            if digest.lower() != sha256.lower():
                print(f"  [!] hash mismatch: got {digest}")
            print(f"  [+] downloaded {sha256} ({len(data)} bytes)")
            n_ok += 1
            manifest.append({"sha256": sha256, "status": "ok", "path": str(dest), "size": len(data)})

        except Exception as e:
            print(f"  [!] error {sha256}: {e}")
            manifest.append({"sha256": sha256, "status": "error", "error": str(e)})

        time.sleep(args.delay)

    manifest_path = out_dir / "_manifest.json"
    payload = {
        "generated_at": datetime.now(timezone.utc).isoformat(),
        "total_selected": len(hashes),
        "downloaded": n_ok,
        "missing": n_missing,
        "skipped": n_skip,
        "samples": manifest,
    }
    manifest_path.write_text(json.dumps(payload, indent=2), encoding="utf-8")
    print(f"\n[*] done: {n_ok} ok, {n_missing} missing, {n_skip} skipped")
    print(f"[*] manifest: {manifest_path}")

if __name__ == "__main__":
    main()
