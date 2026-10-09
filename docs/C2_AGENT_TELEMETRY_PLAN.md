# C2 Agent Telemetry & Real-Time Dashboard Plan

## Overview
Replace manual monitoring with a custom telemetry agent that reports VM execution progress to a centralized C2 dashboard in real-time. Similar to Wazuh but purpose-built for JOCKY operational visibility.

## Architecture

```
┌─────────────────┐
│   JOCKY Binary  │
│    (in VM)      │
└────────┬────────┘
         │
         ▼
┌─────────────────────────────────┐
│  Custom Telemetry Agent         │
│  - Phase progress tracking      │
│  - EDR evasion status           │
│  - Data collected metrics       │
│  - Exfil channels status        │
│  - BYOVD driver attempts        │
│  - Forensic analysis results    │
└────────┬────────────────────────┘
         │ HTTP POST / WebSocket
         ▼
┌─────────────────────────────────┐
│  C2 Backend (app.py)            │
│  - Collect telemetry            │
│  - Store in SQLite/JSON         │
│  - WebSocket broadcast           │
│  - Historical logs              │
└────────┬────────────────────────┘
         │
         ▼
┌─────────────────────────────────┐
│  C2 Dashboard (Frontend)        │
│  - Live phase progress          │
│  - Driver load attempts         │
│  - Exfil success/failure        │
│  - Forensic results             │
│  - Timeline view                │
│  - Real-time log stream         │
└─────────────────────────────────┘
```

## Implementation Phases

### Phase 1: Custom Telemetry Agent (Python)
**Goal**: Lightweight agent running on VM to capture and report chain execution events

**File**: `src/runtime/telemetry_agent.py` (or as standalone .exe compiled with PyInstaller)

**Events to capture**:
```python
{
    "timestamp": "2026-10-10T00:01:23.456Z",
    "chain_id": "research_chain_v4",
    "event_type": "phase_start|phase_complete|driver_attempt|data_collected|exfil_sent",
    "phase": "0|1|2|3|4|5|6|7|8|9|10|11|12",
    "phase_name": "EDR Evasion|C2 Bootstrap|...",
    "status": "success|failure|pending",
    "details": {
        "driver_name": "rtkiow10x64.sys",  # for driver_attempt
        "device_path": "\\.\RTCore64",
        "error_code": 2,
        "data_bytes": 576,  # for data_collected
        "channel": "cdn|dns|discord",  # for exfil_sent
    },
    "metrics": {
        "memory_used_mb": 45,
        "privilege_level": "admin",
        "edr_techniques_deployed": 11,
    }
}
```

**Communication**:
- POST to `http://C2_SERVER:8443/api/telemetry/report`
- Retry logic with exponential backoff
- Batch events (10s window) to reduce overhead

### Phase 2: C2 Backend Enhancement (app.py)
**Goal**: Collect, store, and broadcast telemetry in real-time

**New endpoints**:
```python
POST /api/telemetry/report         # Receive agent events
GET  /api/telemetry/chain/{id}     # Get full chain execution timeline
WS   /ws/telemetry/{chain_id}      # WebSocket live stream
GET  /api/telemetry/summary        # Dashboard summary stats
```

**Storage**: 
- SQLite table: `telemetry_events` (timestamp, chain_id, event_type, phase, status, details_json)
- In-memory cache for last 10 minutes (WebSocket subscriptions)

**Features**:
- Auto-cleanup old events (>24h)
- Real-time WebSocket broadcast to all connected dashboards
- Chain timeline reconstruction

### Phase 3: Dashboard Frontend (React/Vue)
**Goal**: Real-time visualization of chain execution

**Views**:
1. **Live Feed** - Real-time event stream (newest first)
   - Phase progress (0-12 with progress bar)
   - Each driver attempt with success/failure
   - Exfil channel confirmations
   - Colored status badges (success=green, failure=red, pending=yellow)

2. **Timeline** - Gantt-like view showing when each phase ran
   - Duration of each phase
   - Parallel events (exfil channels, multiple driver attempts)
   - Hover for details

3. **Metrics** - Summary stats
   - EDR techniques deployed: 11/15
   - Drivers successful: 1/62
   - Data collected: 576 bytes
   - Exfil success rate: 3/5 channels
   - Execution time: 2m 34s

4. **Forensic Results** - Structured tree view
   - Event logs collected
   - Registry hives
   - MFT analysis
   - Jump lists

**Technology**: 
- Backend: FastAPI WebSocket handler (already in app.py)
- Frontend: Single-page React/Vue app served from `/dashboard`
- Real-time via WebSocket, fallback to polling

### Phase 4: Integration with JOCKY Chain
**Goal**: Embed telemetry reporting into the chain itself

**Approach 1**: Wrap chain with telemetry shim
- Python wrapper script runs binary and captures stdout
- Parse console output for phase markers
- Send events to C2

**Approach 2**: Add telemetry calls to chain code
- JOCKY builtin: `jocky_telemetry_report(event_type, phase, details)`
- Calls C2 endpoint directly from chain
- Requires HTTP client in runtime (already available)

**Recommendation**: Start with Approach 1 (non-invasive), migrate to Approach 2 (native)

## Milestones

| Milestone | Effort | Priority |
|-----------|--------|----------|
| Telemetry agent (Python) | 2-3h | High |
| Backend endpoints + WebSocket | 2-3h | High |
| Simple dashboard (HTML/JS) | 2-3h | Medium |
| Full React dashboard | 4-6h | Medium |
| Chain integration | 1-2h | High |

## Non-Blocking Issues

1. **Privacy**: Telemetry stays within C2 (192.168.56.1:8443), no external leakage
2. **Overhead**: Events batched every 10s, <1MB/hour network usage
3. **Failure gracefully**: If C2 unreachable, chain continues normally
4. **Stealth**: Telemetry agent runs as separate process; can be toggled off

## Success Criteria

- ✅ Real-time phase progress visible in dashboard
- ✅ Each driver attempt logged with success/failure reason
- ✅ Exfil channel status (which succeeded, which failed)
- ✅ Full chain execution timeline reconstructable
- ✅ <500ms latency from event on VM to dashboard update
- ✅ No impact on chain execution if C2 unreachable

## Next Steps

1. Build telemetry agent (capture stdout, POST events)
2. Add WebSocket endpoint to app.py
3. Build simple HTML dashboard (no build tools)
4. Test with current research_chain_windows_production.exe
5. Consider JOCKY native integration (builtins)
