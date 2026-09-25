# The Complete Presentation Architecture

Let me walk you through this end-to-end: what the control panel is, how to connect to the VM, what the demo looks like, and how to build it.

---

## Part 1: The Core Idea — What You're Actually Building

You're building a **web-based mission control** for your offensive framework. The judge opens a browser, sees a dashboard, and watches the entire JOCKY pipeline execute in real time on a real Windows VM with a real EDR.

There are three layers:

```
┌──────────────────────────────────────────────────────────┐
│  LAYER 1: THE DASHBOARD (Browser)                        │
│  - Driver arsenal view                                   │
│  - Compiler panel with live logs                         │
│  - Deployment panel                                      │
│  - Embedded VM screen (noVNC)                            │
│  - EDR status feed                                       │
└──────────────────────────────────────────────────────────┘
                          ↕ (WebSocket + REST)
┌──────────────────────────────────────────────────────────┐
│  LAYER 2: THE BACKEND (Python/Node)                      │
│  - Hypervisor control (start/stop/snapshot)              │
│  - VM command execution (WinRM/SSH)                      │
│  - Compiler invocation                                   │
│  - Log streaming                                         │
│  - Result collection                                     │
└──────────────────────────────────────────────────────────┘
                          ↕
┌──────────────────────────────────────────────────────────┐
│  LAYER 3: THE VM (Windows 11 + EDR)                      │
│  - Runs the target environment                           │
│  - EDR installed and monitoring                          │
│  - VNC server for visual access                          │
│  - WinRM/SSH for remote commands                         │
└──────────────────────────────────────────────────────────┘
```

**The key insight:** You're not building a simulator. You're building a **remote control** for a real VM.

---

## Part 2: How the VM Integration Actually Works

There are four options, from simplest to most impressive. Pick based on time.

### Option A: Screenshot Polling (Simplest)
- Backend sends commands to the VM via WinRM.
- After each command, backend takes a screenshot via PowerShell and sends it to the dashboard.
- The dashboard shows a slideshow of screenshots.

**Pros:** Easy to build.  
**Cons:** Feels static. Not real-time.  
**Time:** Half a day.

### Option B: Embedded VNC (Recommended)
- VM runs a VNC server (TightVNC or UltraVNC).
- Dashboard embeds **noVNC** — a pure JavaScript VNC client.
- The judge sees the VM's screen live inside the dashboard.

**Pros:** Real-time, interactive, impressive.  
**Cons:** Slightly more setup.  
**Time:** 1 day.

### Option C: Hypervisor API (Advanced)
- Use VMware Workstation API (`vmrun`), Hyper-V WMI, or VirtualBox SDK.
- Start, stop, snapshot, and revert VMs from the dashboard.
- Combine with noVNC for visual.

**Pros:** Full control from the dashboard.  
**Cons:** Hypervisor-specific code.  
**Time:** 2–3 days.

### Option D: Hybrid (Best)
- **noVNC for visual** (real-time VM screen in the dashboard).
- **WinRM/SSH for control** (send commands to the VM).
- **Hypervisor CLI for lifecycle** (start/stop/snapshot via `vmrun` or `powershell`).

**Pros:** Best of everything.  
**Cons:** More moving parts.  
**Time:** 2–3 days.

**Recommendation:** Go with **Option D**, but if time is short, **Option B** is enough.

---

## Part 3: The Control Panel — What It Looks Like

Here's a concrete design. Six panels, all on one page.

```
┌─────────────────────────────────────────────────────────────────────┐
│  JOCKY MISSION CONTROL                          [Target: WIN11-VM] │
├──────────────────┬──────────────────┬───────────────────────────────┤
│                  │                  │                               │
│  DRIVER ARSENAL  │  COMPILER        │   LIVE VM (noVNC)             │
│                  │                  │                               │
│  ✅ rtkio64.sys  │  Source:         │   ┌─────────────────────┐     │
│     MmMapIoSpace │  script.jky      │   │                     │     │
│                  │                  │   │   Windows 11 VM     │     │
│  ✅ cpuz141.sys  │  [Compile ▶]     │   │   Screen here       │     │
│     MmMapIoSpace │                  │   │                     │     │
│     PCI config   │  Log:            │   │                     │     │
│                  │  > Parsing...    │   └─────────────────────┘     │
│  ✅ NTIOLib.sys  │  > OLLVM pass... │                               │
│     MmMapIoSpace │  > String enc... │   [Snapshot] [Revert]        │
│                  │  > Binary built  │                               │
│  ✅ 0eab16c7.sys │    ✅ done       │                               │
│     PCI config   │                  │                               │
│                  │  Binary hash:    │                               │
│  ❌ RTCore64.sys │  a3f2...         │                               │
│     (blocked)    │                  │                               │
│                  │  [Deploy ▶]      │                               │
├──────────────────┴──────────────────┼───────────────────────────────┤
│                                     │                               │
│  EDR STATUS                         │   RESULTS                     │
│                                     │                               │
│  CrowdStrike Falcon                 │   Driver loaded:    ✅        │
│  Status: 🟢 Active                  │   Callbacks removed: ✅        │
│  Alerts: 0                          │   PPL stripped:     ✅        │
│  Last check: 2s ago                 │   Payload executed: ✅        │
│                                     │   Exfil sent:       ✅        │
│  (Same panel before JOCKY: 47       │   Cleanup:          ✅        │
│   alerts during standard payload)   │                               │
│                                     │   Time: 4.2s                  │
└─────────────────────────────────────┴───────────────────────────────┘
```

### Panel Details

**1. Driver Arsenal**
- Lists all analyzed drivers from your manifests.
- Shows capabilities (MmMapIoSpace, PCI config, process kill, etc.).
- Shows blocklist status (✅ unblocked / ❌ blocked).
- Shows whether the driver was dynamically validated (✅ tested).

**2. Compiler**
- Text area for the JOCKY source (or a dropdown to load pre-written scripts).
- "Compile" button.
- Real-time log output showing each obfuscation pass as it runs.
- Output binary hash (changes on every compile due to polymorphism).

**3. Live VM (noVNC)**
- Embedded noVNC iframe showing the VM's screen.
- Buttons: "Snapshot" and "Revert" to manage VM state.

**4. EDR Status**
- Live feed of the EDR's status (alerts, detections).
- This is the "wow" panel: it stays silent during the JOCKY run.

**5. Results**
- Checklist of what JOCKY did: driver loaded, callbacks removed, PPL stripped, payload executed, exfil sent, cleanup done.
- Time taken.

**6. Deployment**
- "Deploy" button that triggers the full pipeline.
- Progress bar showing which stage is running.

---

## Part 4: The Demo Script (5 Minutes)

Here's the exact sequence for the live demo. Rehearse this 5 times.

**Minute 1: Context**
- Slide: "Modern EDRs detect standard malware in milliseconds."
- Show a standard MSVC payload on the VM. EDR blocks it. Alert fires.
- Show the EDR dashboard: 47 alerts.

**Minute 2: The Driver Arsenal**
- Switch to the control panel.
- Show the driver arsenal: 25+ analyzed drivers, 5 unblocked, all with confirmed primitives.
- Mention: "We analyzed these with DeepZero, tested them dynamically, and catalogued their IOCTLs."

**Minute 3: Compilation**
- Load a JOCKY script from the dropdown.
- Click "Compile."
- Watch the log: parsing → LLVM IR → OLLVM pass → string encryption → packing.
- Show the binary hash changing on each compile.
- Mention: "No signature exists for this binary. VirusTotal shows 0/68."

**Minute 4: Deployment**
- Click "Deploy."
- Watch the noVNC panel:
  - Driver loads (visible in the VM).
  - Runtime executes.
  - Payload runs.
- Watch the Results panel fill in:
  - Callbacks removed ✅
  - PPL stripped ✅
  - Payload executed ✅
  - Exfil sent ✅

**Minute 5: The Reveal**
- Switch to the EDR dashboard.
- **0 alerts.**
- Show the before/after: 47 alerts → 0 alerts.
- Show the exfil dashboard: data arrived.
- Show the VM's event logs: cleared.
- Closing: "This is JOCKY. Real language. Real bypass. Real results."

---

## Part 5: The Tech Stack

Here's a concrete stack you can build in 5–7 days.

### Backend
- **Python 3.11 + Flask** for the REST API.
- **Flask-SocketIO** for real-time log streaming.
- **pywinrm** for sending commands to the Windows VM.
- **subprocess** for invoking the compiler and hypervisor CLI.

### Frontend
- **Single HTML file** with vanilla JS (or React if you prefer).
- **noVNC** embedded as an iframe (`<iframe src="http://vm:6080/vnc.html">`).
- **Chart.js** for the EDR alert graph (before/after).
- **WebSocket** for live logs.

### VM Setup
- **Hypervisor:** VMware Workstation (easiest) or Hyper-V.
- **Guest:** Windows 11 24H2, 4 GB RAM, 60 GB disk.
- **EDR:** CrowdStrike Falcon trial (15 days free) or Microsoft Defender for Endpoint trial.
- **Remote access:**
  - Enable WinRM: `Enable-PSRemoting -Force`
  - Install TightVNC, set password, start service.
- **VM state:** Disable HVCI, disable blocklist, enable test signing.
- **Snapshot:** Take a clean snapshot before each demo.

### The Integration Glue
A single Python file that:
1. Provides REST endpoints for the dashboard.
2. Invokes `vmrun` for snapshot/revert.
3. Invokes the JOCKY compiler for `.jky` → `.exe`.
4. Uses WinRM to copy the binary to the VM and run it.
5. Streams logs back to the dashboard via WebSocket.
6. Polls the EDR status via WinRM (e.g., reading EDR log files).

---

## Part 6: Practical Details You'll Forget Until It's Too Late

### NoVNC Setup
- Install noVNC on the host: `git clone https://github.com/novnc/noVNC`
- TightVNC on the VM should listen on port 5900.
- Use `websockify` to bridge VNC to WebSocket: `websockify 6080 localhost:5900`
- Embed in dashboard: `<iframe src="http://host:6080/vnc.html?autoconnect=1&password=...">`

### EDR Choice
- **CrowdStrike Falcon** has a 15-day free trial. Best for the demo because it's the EDR from your research.
- **Microsoft Defender for Endpoint** has a 90-day trial. Easier to install.
- **SentinelOne** has a free trial. Also good.
- **If none work:** Use Windows Defender with a custom detection rule that triggers on standard payloads. Less impressive but workable.

### WinRM
- Enable on the VM: `Enable-PSRemoting -Force`
- Set up TrustedHosts on the host: `Set-Item WSMan:\localhost\Client\TrustedHosts -Value "vm-ip"`
- Test: `python -c "import winrm; s = winrm.Session('http://vm-ip:5985/wsman', auth=('user','pass')); print(s.run_ps('whoami').std_out)"`

### Compiler Invocation
- The dashboard backend runs: `subprocess.run(['jocky', 'compile', 'script.jky', '-o', 'payload.exe'])`
- Stream stdout/stderr to the dashboard via WebSocket.

### Deployment
- Copy the binary to the VM: `winrm` file transfer or `smbclient`.
- Run it: `s.run_ps('C:\\Users\\user\\payload.exe')`
- Collect output: `s.run_ps('type C:\\Users\\user\\output.log')`

### Snapshot/Revert
- VMware: `vmrun snapshot "path.vmx" "clean"` and `vmrun revertToSnapshot "path.vmx" "clean"`
- Hyper-V: `Checkpoint-VM` and `Restore-VMSnapshot`
- VirtualBox: `VBoxManage snapshot <vm> take <name>` and `VBoxManage snapshot <vm> restore <name>`

### Backup Plan
- **Record the entire demo as a video.** If the live demo fails, play the video.
- Have a second VM ready with the snapshot.
- Have a pre-compiled binary ready in case the compiler fails.

---

## Part 7: The Slides (Supporting the Demo)

Your slide deck should be minimal — the demo is the star. Use slides only for context and results.

| Slide | Content |
|-------|---------|
| 1 | Title: "JOCKY — A Next‑Gen Programming Language for EDR Evasion" |
| 2 | Problem: Modern EDR detects MSVC/GCC artifacts in milliseconds |
| 3 | Approach: New language + BYOVD arsenal + LLVM obfuscation |
| 4 | Architecture: Pipeline diagram (source → compiler → runtime → VM) |
| 5 | Driver Arsenal: How we analyze and catalog drivers |
| 6 | **Live Demo** (switch to dashboard) |
| 7 | Results: VirusTotal comparison, EDR alerts, timing |
| 8 | Limitations: HVCI, hypervisor EDRs, MFT artifacts |
| 9 | Future work: Real hardware, more drivers, AI‑driven analysis |
| 10 | Conclusion |

---

## Part 8: Timeline (5 Days)

| Day | Task |
|-----|------|
| **Day 1** | Set up Windows VM, install EDR, enable WinRM + VNC, take clean snapshot |
| **Day 2** | Build backend: Flask API, WinRM integration, compiler invocation, WebSocket logs |
| **Day 3** | Build frontend: dashboard HTML/JS, noVNC embed, driver arsenal panel |
| **Day 4** | Integrate everything; run end‑to‑end demo; fix bugs |
| **Day 5** | Record backup video; rehearse demo 5 times; finalize slides |

---

## Part 9: What the Judges Will Remember

1. **The live VM in the browser.** They'll remember watching a real Windows VM get bypassed without leaving the dashboard.
2. **The EDR staying silent.** Going from 47 alerts to 0 alerts is visceral.
3. **The binary hash changing.** Polymorphism is abstract until they see the hash change on every compile.
4. **The driver arsenal.** Showing 25 analyzed drivers, 5 unblocked, all validated, tells a story of rigor.
5. **The narrative.** "We didn't simulate this. We built it."

---

## Summary

| Component | What It Is | Time |
|-----------|-----------|------|
| **Backend** | Flask API + WebSocket + WinRM + subprocess | 1.5 days |
| **Frontend** | HTML/JS dashboard with noVNC embed | 1.5 days |
| **VM setup** | Windows 11 + EDR + WinRM + VNC | 1 day |
| **Integration** | Compiler invocation, deployment scripts, log streaming | 0.5 day |
| **Rehearsal** | 5 run‑throughs + backup video | 0.5 day |
| **Total** | | **5 days** |

**The dashboard is the product. The VM is the proof. The demo is the pitch.**

Build the dashboard. Connect it to the VM. Rehearse. Ship.