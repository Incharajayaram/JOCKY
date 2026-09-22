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

LOLDRIVERS_URL = "https://www.loldrivers.io/api/drivers.json"
MB_URL = "https://mb-api.abuse.ch/api/v1/"
MB_QUERY_LIMIT = 1000

def load_catalog(catalog_path, refresh):
    if catalog_path.exists() and not refresh:
        return json.loads(catalog_path.read_text(encoding="utf-8"))
    print(f"[*] fetching loldrivers catalog: {LOLDRIVERS_URL}")
    r = requests.get(LOLDRIVERS_URL, timeout=60)
    r.raise_for_status()
    data = r.json()
    if not catalog_path.parent.exists():
        catalog_path.parent.mkdir(parents=True, exist_ok=True)
    catalog_path.write_text(json.dumps(data, indent=1), encoding="utf-8")
    return data

def load_unblocked_ids(path):
    ids = set()
    if not path.exists():
        return ids
    for line in path.read_text(encoding="utf-8").splitlines():
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        first = line.split("|")[0].strip()
        if first:
            ids.add(first)
    return ids

def select_samples(catalog, unblocked_ids, all_samples):
    selected = {}
    for d in catalog:
        lolid = d.get("Id")
        if unblocked_ids and lolid not in unblocked_ids:
            continue
        category = d.get("Category", "")
        mitre = d.get("MitreID", "") or ""
        tags = ",".join(d.get("Tags") or [])
        for s in d.get("KnownVulnerableSamples") or []:
            sha256 = s.get("SHA256")
            if not sha256 or sha256 == "-":
                continue
            filename = s.get("Filename") or f"{sha256}.sys"
            selected[sha256] = {
                "sha256": sha256,
                "filename": filename,
                "lolid": lolid,
                "drivername": d.get("Name") or d.get("Filename") or "",
                "category": category,
                "mitre_ids": mitre,
                "tags": tags,
                "publisher": s.get("Publisher") or "",
                "full_metadata": s,
            }
    if not all_samples and unblocked_ids:
        pass
    return selected

def api_key_from(env_var):
    key = os.environ.get(env_var)
    if not key:
        print(f"[!] No MalwareBazaar API key. Set env var {env_var}.")
        print("    Register for a free key at https://bazaar.abuse.ch/")
        write_hash_list = input("    Write only the SHA256 list without downloading? [y/N]: ").strip().lower()
        if write_hash_list in ("y", "yes"):
            return None
        sys.exit(2)
    return key

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

def extract_sys(zip_bytes, sha256, out_dir):
    with zipfile.ZipFile(io.BytesIO(zip_bytes)) as zf:
        names = zf.namelist()
        for name in names:
            lower = name.lower()
            sys_name = None
            if lower.endswith(".sys"):
                sys_name = name
            if sys_name:
                data = zf.read(sys_name)
                return sys_name, data
    return None, None

def save_sample(out_dir, zip_bytes, sha256, folder):
    sys_name, data = extract_sys(zip_bytes, sha256, out_dir)
    if data is None:
        return None
    if folder == "rawzip":
        dest = out_dir / f"{sha256}.zip"
        dest.write_bytes(zip_bytes)
        return dest
    if folder == "hashed":
        dest = out_dir / f"{sha256}.sys"
        dest.write_bytes(data)
        return dest
    safe = "".join(c for c in sys_name if c.isalnum() or c in "._-") or f"{sha256}.sys"
    dest = out_dir / safe
    dest.write_bytes(data)
    return dest

def main():
    ap = argparse.ArgumentParser(description="Download LOLDrivers samples from MalwareBazaar")
    ap.add_argument("--catalog", default="loldrivers.json", help="path to loldrivers catalog (fetched if missing)")
    ap.add_argument("--refresh-catalog", action="store_true", help="re-fetch catalog from loldrivers.io")
    ap.add_argument("--unblocked", default="unblocked_loldrivers.md",
                    help="filter to these LolDrivers ID lines (default unblocked_loldrivers.md)")
    ap.add_argument("--no-filter", action="store_true", help="download all samples, ignore --unblocked")
    ap.add_argument("--out", default="drivers_out", help="output directory")
    ap.add_argument("--folder", choices=["orig", "hashed", "rawzip"], default="orig",
                    help="orig=keep zip filename, hashed=<sha256>.sys, rawzip=save zip files")
    ap.add_argument("--only-list", action="store_true", help="write sha256_list.txt and exit (no downloads)")
    ap.add_argument("--api-key", default=None, help=f"MalwareBazaar API key (or {os.environ.get('MALWAREBAZAAR_API_KEY') and 'set MALWAREBAZAAR_API_KEY env' or 'env var MALWAREBAZAAR_API_KEY'})")
    args = ap.parse_args()

    cwd = Path.cwd()
    catalog = load_catalog(cwd / args.catalog, args.refresh_catalog)
    unblocked = set() if args.no_filter else load_unblocked_ids(cwd / args.unblocked)
    selected = select_samples(catalog, unblocked, args.no_filter)
    if not selected:
        sys.exit(f"[!] no samples selected (unblocked list empty or mismatch: {args.unblocked})")

    print(f"[*] {len(selected)} samples selected from {len(catalog)} catalog entries")

    out_dir = cwd / args.out
    out_dir.mkdir(parents=True, exist_ok=True)

    if args.only_list:
        list_path = cwd / "sha256_list.txt"
        list_path.write_text("\n".join(sorted(selected)) + "\n", encoding="utf-8")
        print(f"[*] wrote {list_path} with {len(selected)} hashes")
        return

    api_key = args.api_key or os.environ.get("MALWAREBAZAAR_API_KEY")
    if not api_key:
        print("[!] No API key found; writing hash list only.")
        list_path = cwd / "sha256_list.txt"
        list_path.write_text("\n".join(sorted(selected)) + "\n", encoding="utf-8")
        print(f"[*] wrote {list_path}")
        return

    session = requests.Session()
    manifest = []
    n_ok = n_missing = n_skip = n_err = 0
    count = 0
    for sha256 in sorted(selected):
        info = selected[sha256]
        dest = out_dir / (info["filename"] if args.folder == "orig" else f"{sha256}.sys")
        if args.folder != "rawzip" and dest.exists():
            print(f"  [-] skip (exists): {info['filename']}")
            n_skip += 1
            manifest.append({"sha256": sha256, "status": "skipped", "path": str(dest)})
            continue
        count += 1
        if count % MB_QUERY_LIMIT == 0:
            print(f"[*] sleep 60s after {MB_QUERY_LIMIT} queries")
            time.sleep(60)
        try:
            content, status = mb_download(sha256, api_key, session)
            if content is None:
                print(f"  [-] missing on MalwareBazaar: {info['filename']} ({status})")
                n_missing += 1
                manifest.append({"sha256": sha256, "status": "missing", "path": None})
                continue
            saved = save_sample(out_dir, content, sha256, args.folder if args.folder != "orig" else "orig")
            if saved is None:
                print(f"  [-] no .sys inside archive: {info['filename']}")
                n_missing += 1
                manifest.append({"sha256": sha256, "status": "no_sys", "path": None})
                continue
            digest = hashlib.sha256(saved.read_bytes()).hexdigest()
            if digest.lower() != sha256.lower():
                print(f"  [!] hash mismatch {info['filename']}: got {digest}")
            print(f"  [+] {info['filename']}")
            n_ok += 1
            manifest.append({"sha256": sha256, "filename": info["filename"], "path": str(saved),
                             "lolid": info["lolid"], "drivername": info["drivername"],
                             "category": info["category"], "mitre_ids": info["mitre_ids"],
                             "tags": info["tags"], "publisher": info["publisher"], "status": "ok"})
        except Exception as e:
            print(f"  [!] error {sha256}: {e}")
            n_err += 1
            manifest.append({"sha256": sha256, "status": "error", "error": str(e), "path": None})

    manifest_path = out_dir / "_manifest.json"
    payload = {
        "generated_at": datetime.now(timezone.utc).isoformat(),
        "catalog": str(cwd / args.catalog),
        "total_selected": len(selected),
        "downloaded": n_ok,
        "missing": n_missing,
        "skipped": n_skip,
        "errors": n_err,
        "samples": manifest,
    }
    manifest_path.write_text(json.dumps(payload, indent=2), encoding="utf-8")
    print(f"\n[*] done: {n_ok} ok, {n_missing} missing, {n_skip} skipped, {n_err} errors")
    print(f"[*] manifest: {manifest_path}")

if __name__ == "__main__":
    main()