# JOCKY

A compiled language and toolchain for building payloads that report back to a C2 server, plus a forensic analysis pipeline for investigating Windows and Linux targets.

Live instance: https://jocky.pointblank.club/

---

## What it does

JOCKY has two main pipelines:

**Attacker pipeline** — Write a payload in the JOCKY language, compile it, and it runs on the target. The payload collects data (system info, credentials, network state, files) and sends it back to the C2 server over HTTP. The server logs everything and exposes it through the web dashboard. Obfuscation passes run at compile time so the output binary is harder to detect.

**Forensic pipeline** — Compile a forensic agent that runs on a target and collects artifacts: event logs, registry keys, prefetch files, MFT, network connections, running processes, memory regions. Output is JSON, STIX, Sigma rules, or HTML report. Useful for incident response and post-compromise analysis.

---

## Architecture

```
JOCKY source (.jky)
        |
    Compiler (Python + LLVM)
        |
    Obfuscation passes
        |
    Windows/Linux PE or ELF
        |
    Runs on target
        |
    HTTP POST to C2 server
        |
    Web dashboard (https://jocky.pointblank.club/)
```

The compiler targets `x86_64-w64-windows-gnu` for Windows and `x86_64-linux-gnu` for Linux. It links against a runtime library that handles network, crypto, evasion, and data collection.

---

## Local build

### Requirements

- Python 3.11+
- Docker (for the web stack)
- 10GB+ free disk
- Linux x86_64

### Setup

```bash
git clone https://github.com/Incharajayaram/JOCKY.git
cd JOCKY
pip install -e .
```

### Toolchain

The compiler needs a pre-built clang toolchain with mingw sysroot and static libs (OpenSSL, curl, zlib). Download it:

```bash
./upload_toolchain.sh   # only if you're uploading to GCS
# or pull from GCS if you have access:
gcloud storage cp gs://jocky-toolchain/toolchain.zip /tmp/toolchain.zip
unzip /tmp/toolchain.zip -d .
```

### Compile a payload

```bash
python scripts/compile_pipeline.py examples/research_chain_windows_production.jky \
    --platform windows \
    --preset aggressive
```

Output binary lands in `build/`.

### Run the web stack locally

```bash
# Backend
cd web/backend
pip install -r requirements.txt
uvicorn app:app --host 0.0.0.0 --port 8000

# Frontend (separate terminal)
cd web/frontend
# serve with nginx or any static file server
```

---

## Obfuscation passes

Aggressive preset runs all of these at compile time:

- `string_encrypt` — XOR encrypt string literals
- `constant_obfuscate` — replace constants with computed expressions
- `symbol_obfuscate` — rename functions and globals
- `crypto_hash` — hash-based dispatch tables
- `scf_obfuscate` — split control flow
- `import_obfuscate` — dynamic import resolution
- `strip_signature` — remove PE/ELF signatures
- `virtualize` — lift hot functions to VM bytecode
- `opaque_pred` — insert opaque predicates
- `substitution` — arithmetic substitution
- `boguscf` — bogus control flow
- `flattening` — control flow flattening
- `linear_mba` — mixed boolean arithmetic
- `anti_debug` — timing and debugger detection
- `indirect_call` — indirect call dispatch

---

## C2 telemetry

The payload sends events to `/api/telemetry` on the C2 server. Events include:

- `stage_start` / `stage_complete` — pipeline progress
- `data_collected` — harvested data chunks
- `exfil_success` / `exfil_fail` — upload results
- `evasion_event` — EDR/AV detection attempts
- `error` — runtime errors

The dashboard at https://jocky.pointblank.club/ shows live event streams, exfiltrated data, forensic results, and driver scoring.

---

## Project structure

```
JOCKY/
├── src/jocky/
│   ├── language/       # Parser, type checker, codegen
│   ├── passes/         # Obfuscation passes
│   └── stdlib/         # Standard library
├── src/runtime/
│   ├── windows/        # Windows-specific APIs
│   ├── linux/          # Linux-specific APIs
│   ├── common/         # Shared (crypto, network, compression)
│   ├── evasion/        # EDR evasion, AMSI bypass, unhooking
│   ├── forensics/      # Forensic collection and analysis
│   └── exfil/          # Exfiltration channels
├── web/
│   ├── backend/        # FastAPI C2 server
│   └── frontend/       # Dashboard
├── examples/           # Example .jky programs
├── scripts/            # Compile pipeline and tooling
└── tests/
```

---

## License

For authorized security research and red team use only.
