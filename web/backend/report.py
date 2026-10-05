import asyncio
import re
from typing import Optional

import httpx

from obfuscation import MLIR_PASSES, LLVM_PASSES
from runtime_apis import RUNTIME_APIS

# Build flat lookup: fn_name -> {name, description, category, platform}
_API_INDEX: list[dict] = []
for _cat in RUNTIME_APIS:
    for _api in _cat.get("apis", []):
        _API_INDEX.append({
            "fn": _api["name"],
            "name": _api["name"],
            "description": _api.get("description", ""),
            "category": _cat.get("category", "Unknown"),
            "platform": _cat.get("platform", "all"),
        })

_CATEGORY_BEHAVIORS: dict[str, str] = {
    "Anti-Analysis": "detects and evades sandboxes, debuggers, and analysis environments",
    "BYOVD": "exploits a signed vulnerable driver for kernel-level code execution",
    "Evasion": "applies runtime evasion techniques to avoid AV/EDR detection",
    "Crypto": "performs cryptographic operations (hashing, encryption, key derivation)",
    "Exfiltration": "exfiltrates collected data to a remote endpoint or C2 server",
    "Network": "establishes network connections for C2 communication and data transfer",
    "Process": "spawns, injects into, or hijacks running processes",
    "Kernel": "executes kernel-level operations via direct syscalls",
    "Forensics": "performs forensic-grade analysis and cleanup of the host environment",
    "Sandbox": "spawns isolated sandbox environments for secondary payload execution",
    "Audit": "records audit trails of all performed operations",
    "Plugin": "loads and executes additional plugin modules at runtime",
    "AI/ML Runtime": "uses on-device ML models for adaptive evasion behaviour",
}

DOGBOLT_BASE = "https://dogbolt.org/api/binaries/"


def extract_used_apis(source: str) -> list[dict]:
    seen: set[str] = set()
    result = []
    for entry in _API_INDEX:
        fn = entry["fn"]
        if fn in seen:
            continue
        if re.search(rf'\b{re.escape(fn)}\s*\(', source):
            seen.add(fn)
            result.append({
                "name": entry["name"],
                "description": entry["description"],
                "category": entry["category"],
                "platform": entry["platform"],
            })
    return result


def get_enabled_passes(obfuscation_config: dict) -> dict:
    mlir_cfg = obfuscation_config.get("mlir", {})
    llvm_cfg = obfuscation_config.get("llvm", {})
    return {
        "mlir": [
            {"id": p["id"], "name": p["name"], "description": p["description"]}
            for p in MLIR_PASSES
            if mlir_cfg.get(p["id"], p["default"])
        ],
        "llvm": [
            {"id": p["id"], "name": p["name"], "description": p["description"]}
            for p in LLVM_PASSES
            if llvm_cfg.get(p["id"], p["default"])
        ],
    }


def generate_behavior_summary(platform: str, apis: list[dict], passes: dict) -> str:
    categories = {a["category"] for a in apis}
    behaviors = [_CATEGORY_BEHAVIORS[c] for c in sorted(categories) if c in _CATEGORY_BEHAVIORS]

    mlir_count = len(passes.get("mlir", []))
    llvm_count = len(passes.get("llvm", []))
    pass_parts = []
    if mlir_count:
        pass_parts.append(f"{mlir_count} MLIR obfuscation pass{'es' if mlir_count > 1 else ''}")
    if llvm_count:
        pass_parts.append(f"{llvm_count} LLVM obfuscation pass{'es' if llvm_count > 1 else ''}")

    obf_text = (
        f"The binary has been hardened with {' and '.join(pass_parts)}, "
        "making static analysis, signature matching, and dynamic tracing significantly harder."
        if pass_parts else "No obfuscation passes were applied to this binary."
    )

    if behaviors:
        behavior_lines = "\n".join(f"  • {b.capitalize()}" for b in behaviors)
        behavior_text = (
            f"On a real {platform} system, this binary will:\n{behavior_lines}\n\n"
            "The binary is designed to operate stealthily — anti-analysis checks run first, "
            "and all sensitive operations are gated behind environment validation."
        )
    else:
        behavior_text = (
            f"The {platform} binary performs operations with no detected JOCKY runtime API usage. "
            "It may rely entirely on custom or inline logic."
        )

    return f"{behavior_text}\n\n{obf_text}"


async def upload_to_dogbolt(binary_path: str) -> Optional[str]:
    try:
        async with httpx.AsyncClient(timeout=60) as client:
            with open(binary_path, "rb") as f:
                resp = await client.post(
                    DOGBOLT_BASE,
                    files={"file": ("binary", f, "application/octet-stream")},
                )
            if resp.status_code in (200, 201):
                return resp.json().get("id")
    except Exception:
        pass
    return None


async def poll_ghidra(binary_id: str, max_seconds: int = 180) -> Optional[str]:
    url = f"{DOGBOLT_BASE}{binary_id}/decompilations/"
    checks = max(max_seconds // 8, 1)
    try:
        async with httpx.AsyncClient(timeout=30) as client:
            for _ in range(checks):
                await asyncio.sleep(8)
                resp = await client.get(url)
                if resp.status_code != 200:
                    break
                for item in resp.json().get("results", []):
                    if "Ghidra" in item.get("decompiler", {}).get("name", ""):
                        output = item.get("output")
                        if output:
                            return output
    except Exception:
        pass
    return None


async def run_dogbolt_analysis(job) -> None:
    """Upload binary to dogbolt and store Ghidra decompilation on the job."""
    if not job.output_path:
        job.decompilation_status = "failed"
        return

    job.decompilation_status = "running"
    binary_id = await upload_to_dogbolt(job.output_path)
    if not binary_id:
        job.decompilation_status = "failed"
        return

    job.dogbolt_id = binary_id
    result = await poll_ghidra(binary_id)
    if result:
        job.decompilation = result
        job.decompilation_status = "done"
    else:
        job.decompilation_status = "failed"
