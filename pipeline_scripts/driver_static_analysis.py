#!/usr/bin/env python3
"""
Static analysis script for Windows kernel drivers (.sys).
Analyzes PE headers, sections, imports, exports, signatures, strings, and
flags suspicious capabilities (RWX, dangerous imports, known-vulnerable patterns).
"""
import sys
import os
import json
import math
import hashlib
from pathlib import Path
from datetime import datetime, timezone

try:
    import pefile
except ImportError:
    sys.exit("Missing pefile. Install: pip install pefile")

try:
    from capstone import Cs, CS_ARCH_X86, CS_MODE_64, CS_MODE_32
except ImportError:
    Cs = None

# Suspicious imports that indicate vulnerable capabilities
DANGEROUS_IMPORTS = {
    # Memory manipulation
    "MmMapIoSpace", "MmMapIoSpaceEx", "MmUnmapIoSpace",
    "MmMapLockedPages", "MmMapLockedPagesSpecifyCache",
    "MmAllocateContiguousMemory", "MmAllocateContiguousMemorySpecifyCache",
    "MmGetPhysicalAddress", "MmGetSystemRoutineAddress",
    "ZwMapViewOfSection", "NtMapViewOfSection",
    # Physical memory RW
    "MmCopyMemory", "RtlCopyMemory", "memcpy",
    # Process/thread manipulation
    "ZwOpenProcess", "NtOpenProcess", "PsGetCurrentProcessId",
    "PsLookupProcessByProcessId", "KeAttachProcess", "KeStackAttachProcess",
    "ZwAllocateVirtualMemory", "NtAllocateVirtualMemory",
    "ZwWriteVirtualMemory", "NtWriteVirtualMemory",
    "ZwProtectVirtualMemory", "NtProtectVirtualMemory",
    # Registry
    "ZwCreateKey", "ZwSetValueKey", "ZwDeleteKey",
    # IOCTL handler
    "IoCreateDevice", "IoCreateSymbolicLink", "IoDeleteDevice",
    "IoCompleteRequest", "IofCompleteRequest",
    # Port I/O
    "READ_PORT_UCHAR", "READ_PORT_USHORT", "READ_PORT_ULONG",
    "WRITE_PORT_UCHAR", "WRITE_PORT_USHORT", "WRITE_PORT_ULONG",
    "READ_PORT_BUFFER_UCHAR", "WRITE_PORT_BUFFER_UCHAR",
    # MSR / CPU control
    "__readmsr", "__writemsr", "__rdtsc",
    # Other dangerous
    "ExAllocatePool", "ExAllocatePoolWithTag", "ExFreePool",
    "ExAllocatePool2", "ExAllocatePool3",
}

SUSPICIOUS_SECTION_NAMES = {".vmp0", "vmp0", ".upx", "UPX0", "UPX1", ".aspack",
                             ".petite", ".themida", ".enigma", ".mnbvcx", ".xyz"}

def sha256_file(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        while chunk := f.read(65536):
            h.update(chunk)
    return h.hexdigest()

def entropy(data):
    if not data:
        return 0.0
    e = 0.0
    for x in range(256):
        p = data.count(bytes([x])) / len(data)
        if p > 0:
            e -= p * math.log2(p)
    return e

def get_arch(pe):
    machine = pe.FILE_HEADER.Machine
    if machine == pefile.MACHINE_TYPE["IMAGE_FILE_MACHINE_AMD64"]:
        return "x64"
    if machine == pefile.MACHINE_TYPE["IMAGE_FILE_MACHINE_I386"]:
        return "x86"
    if machine == pefile.MACHINE_TYPE["IMAGE_FILE_MACHINE_ARM64"]:
        return "ARM64"
    return f"0x{machine:04X}"

def has_signature(pe):
    try:
        return pe.OPTIONAL_HEADER.DATA_DIRECTORY[pefile.DIRECTORY_ENTRY["IMAGE_DIRECTORY_ENTRY_SECURITY"]].VirtualAddress != 0
    except Exception:
        return False

def analyze_driver(path):
    result = {
        "path": str(path),
        "filename": path.name,
        "sha256": sha256_file(path),
        "size": path.stat().st_size,
        "error": None,
        "warnings": [],
        "score": 0,
    }

    try:
        pe = pefile.PE(str(path), fast_load=True)
    except Exception as e:
        result["error"] = f"PE parse failed: {e}"
        return result

    # Basic PE info
    result["arch"] = get_arch(pe)
    result["subsystem"] = "native" if pe.OPTIONAL_HEADER.Subsystem == 1 else f"subsystem_{pe.OPTIONAL_HEADER.Subsystem}"
    result["image_base"] = hex(pe.OPTIONAL_HEADER.ImageBase)
    result["entry_point"] = hex(pe.OPTIONAL_HEADER.AddressOfEntryPoint)
    result["is_signed"] = has_signature(pe)

    # Timestamp
    ts = pe.FILE_HEADER.TimeDateStamp
    try:
        dt = datetime.fromtimestamp(ts, tz=timezone.utc)
        result["timestamp"] = dt.isoformat()
    except Exception:
        result["timestamp"] = f"invalid ({ts})"

    # Imphash
    try:
        result["imphash"] = pe.get_imphash()
    except Exception:
        result["imphash"] = None

    # Sections
    sections = []
    for sec in pe.sections:
        name = sec.Name.rstrip(b"\x00").decode("latin-1", errors="replace")
        data = sec.get_data()
        ent = entropy(data)
        chars = sec.Characteristics
        is_exec = bool(chars & 0x20000000)
        is_writable = bool(chars & 0x80000000)
        is_readable = bool(chars & 0x40000000)
        sections.append({
            "name": name,
            "virtual_address": hex(sec.VirtualAddress),
            "virtual_size": sec.Misc_VirtualSize,
            "raw_size": sec.SizeOfRawData,
            "entropy": round(ent, 2),
            "exec": is_exec,
            "write": is_writable,
            "read": is_readable,
        })
        if name.lower() in {s.lower() for s in SUSPICIOUS_SECTION_NAMES}:
            result["warnings"].append(f"suspicious section name: {name}")
            result["score"] += 1
        if is_exec and is_writable:
            result["warnings"].append(f"RWX section: {name}")
            result["score"] += 2
        if ent > 7.0:
            result["warnings"].append(f"high entropy section {name}: {ent:.2f}")
            result["score"] += 1
    result["sections"] = sections

    # Imports
    dangerous = []
    try:
        pe.parse_data_directories(directories=[pefile.DIRECTORY_ENTRY["IMAGE_DIRECTORY_ENTRY_IMPORT"]])
        if hasattr(pe, "DIRECTORY_ENTRY_IMPORT"):
            for entry in pe.DIRECTORY_ENTRY_IMPORT:
                dll = entry.dll.decode("utf-8", errors="replace") if entry.dll else "?"
                for imp in entry.imports:
                    name = imp.name.decode("utf-8", errors="replace") if imp.name else f"ordinal_{imp.ordinal}"
                    if name in DANGEROUS_IMPORTS:
                        dangerous.append({"dll": dll, "name": name})
    except Exception:
        pass
    result["dangerous_imports"] = dangerous
    result["dangerous_import_count"] = len(dangerous)
    result["score"] += len(dangerous) * 2

    # Exports
    exports = []
    try:
        pe.parse_data_directories(directories=[pefile.DIRECTORY_ENTRY["IMAGE_DIRECTORY_ENTRY_EXPORT"]])
        if hasattr(pe, "DIRECTORY_ENTRY_EXPORT"):
            for exp in pe.DIRECTORY_ENTRY_EXPORT.symbols:
                name = exp.name.decode("utf-8", errors="replace") if exp.name else f"ordinal_{exp.ordinal}"
                exports.append(name)
    except Exception:
        pass
    result["exports"] = exports
    result["export_count"] = len(exports)

    # Interesting strings (ASCII printable >= 4 chars)
    strings_found = []
    try:
        with open(path, "rb") as f:
            data = f.read()
        import re
        for match in re.finditer(rb"[\x20-\x7e]{4,}", data):
            s = match.group().decode("ascii")
            lower = s.lower()
            if any(k in lower for k in ["ioctl", "device", "physical", "memory", "map", "write", "read",
                                         "process", "thread", "token", "privilege", "debug", "msr", "port",
                                         "registry", "key", "rootkit", "bypass", "kill", "protect"]):
                strings_found.append(s)
    except Exception:
        pass
    result["interesting_strings"] = strings_found[:20]  # cap at 20
    result["interesting_string_count"] = len(strings_found)

    # Brief disassembly of entry point if capstone available
    if Cs and pe.OPTIONAL_HEADER.AddressOfEntryPoint:
        try:
            mode = CS_MODE_64 if result["arch"] == "x64" else CS_MODE_32
            md = Cs(CS_ARCH_X86, mode)
            ep_rva = pe.OPTIONAL_HEADER.AddressOfEntryPoint
            ep_data = pe.get_data(ep_rva, 64)
            disasm = []
            for insn in md.disasm(ep_data, pe.OPTIONAL_HEADER.ImageBase + ep_rva):
                disasm.append(f"0x{insn.address:x}: {insn.mnemonic} {insn.op_str}")
                if len(disasm) >= 8:
                    break
            result["entry_disasm"] = disasm
        except Exception:
            result["entry_disasm"] = []
    else:
        result["entry_disasm"] = []

    pe.close()
    return result

def main():
    import argparse
    ap = argparse.ArgumentParser()
    ap.add_argument("input", help="directory of .sys files or a single .sys file")
    ap.add_argument("--out", default="driver_analysis_report.json", help="output JSON report")
    ap.add_argument("--markdown", default="driver_analysis_report.md", help="output Markdown summary")
    args = ap.parse_args()

    target = Path(args.input)
    if target.is_dir():
        files = sorted([f for f in target.iterdir() if f.suffix.lower() == ".sys" or f.stat().st_size < 50*1024*1024])
    else:
        files = [target]

    results = []
    print(f"[*] analyzing {len(files)} files...")
    for i, f in enumerate(files, 1):
        print(f"  [{i}/{len(files)}] {f.name}")
        r = analyze_driver(f)
        results.append(r)

    # Sort by score descending
    results.sort(key=lambda x: x.get("score", 0), reverse=True)

    # Write JSON
    Path(args.out).write_text(json.dumps(results, indent=2), encoding="utf-8")
    print(f"[*] JSON report: {args.out}")

    # Write Markdown
    lines = ["# Driver Static Analysis Report\n",
             f"Generated: {datetime.now(timezone.utc).isoformat()}\n",
             f"Total samples: {len(results)}\n\n"]

    lines.append("## Summary by Risk Score\n\n")
    lines.append("| Rank | File | Arch | Signed | Dangerous Imports | Score | Warnings |\n")
    lines.append("|------|------|------|--------|-------------------|-------|----------|\n")
    for idx, r in enumerate(results, 1):
        warns = "; ".join(r["warnings"]) if r["warnings"] else "-"
        warns_short = warns[:60] + "..." if len(warns) > 60 else warns
        signed = "Yes" if r.get("is_signed") else "No"
        lines.append(f"| {idx} | {r['filename']} | {r.get('arch','?')} | {signed} | {r.get('dangerous_import_count',0)} | {r.get('score',0)} | {warns_short} |\n")

    for r in results:
        lines.append(f"\n---\n\n## {r['filename']}\n\n")
        if r.get("error"):
            lines.append(f"**ERROR:** {r['error']}\n\n")
            continue
        lines.append(f"- **SHA256:** `{r['sha256']}`\n")
        lines.append(f"- **Size:** {r['size']:,} bytes\n")
        lines.append(f"- **Arch:** {r.get('arch','?')}\n")
        lines.append(f"- **Signed:** {'Yes' if r.get('is_signed') else 'No'}\n")
        lines.append(f"- **Timestamp:** {r.get('timestamp','?')}\n")
        lines.append(f"- **Imphash:** `{r.get('imphash') or 'N/A'}`\n")
        lines.append(f"- **Score:** {r.get('score',0)}\n")

        if r.get("warnings"):
            lines.append("\n**Warnings:**\n")
            for w in r["warnings"]:
                lines.append(f"- {w}\n")

        if r.get("dangerous_imports"):
            lines.append("\n**Dangerous Imports:**\n")
            for imp in r["dangerous_imports"]:
                lines.append(f"- `{imp['name']}` from `{imp['dll']}`\n")

        if r.get("sections"):
            lines.append("\n**Sections:**\n")
            lines.append("| Name | VA | Virtual Size | Raw Size | Entropy | R | W | X |\n")
            lines.append("|------|-----|-------------|----------|---------|---|---|---|\n")
            for sec in r["sections"]:
                lines.append(f"| {sec['name']} | {sec['virtual_address']} | {sec['virtual_size']:,} | {sec['raw_size']:,} | {sec['entropy']} | {'R' if sec['read'] else '-'} | {'W' if sec['write'] else '-'} | {'X' if sec['exec'] else '-'} |\n")

        if r.get("exports"):
            lines.append(f"\n**Exports ({r.get('export_count',0)}):**\n")
            for exp in r["exports"][:10]:
                lines.append(f"- `{exp}`\n")
            if len(r["exports"]) > 10:
                lines.append(f"- ... and {len(r['exports']) - 10} more\n")

        if r.get("interesting_strings"):
            lines.append("\n**Interesting Strings:**\n")
            for s in r["interesting_strings"][:10]:
                lines.append(f"- `{s}`\n")

        if r.get("entry_disasm"):
            lines.append("\n**Entry Point Disassembly:**\n```asm\n")
            for d in r["entry_disasm"]:
                lines.append(f"{d}\n")
            lines.append("```\n")

    lines.append("\n---\n\n*Generated by driver_static_analysis.py*\n")
    Path(args.markdown).write_text("".join(lines), encoding="utf-8")
    print(f"[*] Markdown report: {args.markdown}")

    # Top 10 by score
    print("\n[*] Top 10 highest-risk drivers:")
    for i, r in enumerate(results[:10], 1):
        warns = ", ".join(r["warnings"]) if r["warnings"] else "clean"
        print(f"  {i}. {r['filename']} (score={r['score']}, imports={r.get('dangerous_import_count',0)}) - {warns}")

if __name__ == "__main__":
    main()
