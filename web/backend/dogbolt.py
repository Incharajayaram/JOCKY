import gzip
import json
import re
from typing import Optional

import httpx
from fastapi import APIRouter, File, HTTPException, UploadFile

router = APIRouter(prefix="/api/dogbolt", tags=["dogbolt"])

DOGBOLT_BASE = "https://dogbolt.org"
MAX_BINARY_SIZE = 2 * 1024 * 1024

_DECOMPILERS_FALLBACK = [
    {"name": "Ghidra", "version": "11.0.3"},
    {"name": "Hex-Rays", "version": "8.4.0"},
    {"name": "BinaryNinja", "version": "4.0.5265"},
    {"name": "angr", "version": "9.2.118"},
    {"name": "Reko", "version": "0.11.5.0"},
    {"name": "Snowman", "version": "0.1.4"},
    {"name": "dewolf", "version": "2024.3"},
]

# POST /api/binaries/ is AllowAny in dogbolt's DRF views — no CSRF or auth needed.
# GET /api/binaries/{id}/decompilations/ and rerun are also AllowAny.
# Only GET /api/binaries/ (list all) requires IsWorkerOrAdmin — we never call that.


@router.post("/upload")
async def upload_binary(file: UploadFile = File(...)):
    data = await file.read()
    if len(data) > MAX_BINARY_SIZE:
        size_mb = len(data) / (1024 * 1024)
        raise HTTPException(413, f"File too large: {size_mb:.2f} MB — Dogbolt limit is 2 MB")

    async with httpx.AsyncClient(timeout=30, follow_redirects=True) as client:
        resp = await client.post(
            f"{DOGBOLT_BASE}/api/binaries/",
            files={"file": (file.filename or "binary", data, file.content_type or "application/octet-stream")},
        )
    if not resp.is_success:
        raise HTTPException(resp.status_code, f"Dogbolt upload failed ({resp.status_code}): {resp.text[:300]}")
    return resp.json()


async def _fetch_all_decompilations(binary_id: str) -> list[dict]:
    results: list[dict] = []
    url: Optional[str] = f"{DOGBOLT_BASE}/api/binaries/{binary_id}/decompilations/"
    async with httpx.AsyncClient(timeout=30, follow_redirects=True) as client:
        while url:
            resp = await client.get(url)
            if not resp.is_success:
                raise HTTPException(resp.status_code, f"Dogbolt status error: {resp.text[:200]}")
            body = resp.json()
            results.extend(body.get("results", []))
            url = body.get("next")
    return results


@router.get("/status/{binary_id}")
async def get_status(binary_id: str):
    results = await _fetch_all_decompilations(binary_id)
    return {"results": results, "next": None}


@router.get("/download/{binary_id}/{decompilation_id}")
async def download_decompilation(binary_id: str, decompilation_id: str):
    results = await _fetch_all_decompilations(binary_id)
    entry = next((r for r in results if str(r.get("id")) == decompilation_id), None)
    if not entry:
        raise HTTPException(404, "Decompilation not found")

    download_url: Optional[str] = entry.get("download_url")
    if not download_url:
        raise HTTPException(404, "No download URL available yet")

    async with httpx.AsyncClient(timeout=30, follow_redirects=True) as client:
        resp = await client.get(download_url)
        raw = resp.content

    try:
        code = gzip.decompress(raw).decode("utf-8", errors="replace")
    except Exception:
        code = raw.decode("utf-8", errors="replace")

    if not code.strip():
        raise HTTPException(422, "Empty decompile result")

    return {"code": code, "decompiler": entry.get("decompiler", {})}


@router.get("/decompilers")
async def list_decompilers():
    try:
        async with httpx.AsyncClient(timeout=15, follow_redirects=True) as client:
            resp = await client.get(f"{DOGBOLT_BASE}/")
        match = re.search(
            r'<script[^>]+id=["\']decompilers_json["\'][^>]*>(.*?)</script>',
            resp.text,
            re.DOTALL,
        )
        if match:
            decompilers = json.loads(match.group(1).strip())
            return {"decompilers": decompilers, "count": len(decompilers)}
    except Exception:
        pass
    return {"decompilers": _DECOMPILERS_FALLBACK, "count": len(_DECOMPILERS_FALLBACK)}


@router.post("/rerun/{binary_id}/{decompilation_id}")
async def rerun_decompilation(binary_id: str, decompilation_id: str):
    async with httpx.AsyncClient(timeout=15, follow_redirects=True) as client:
        resp = await client.post(
            f"{DOGBOLT_BASE}/api/binaries/{binary_id}/decompilations/{decompilation_id}/rerun/",
        )
    if not resp.is_success:
        raise HTTPException(resp.status_code, f"Rerun failed: {resp.text[:200]}")
    return {"status": "ok"}
