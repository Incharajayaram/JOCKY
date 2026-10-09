# Telemetry Infrastructure Implementation

## Overview

The C2 backend now includes a complete telemetry collection and real-time streaming infrastructure. This document describes the implementation details and architecture.

## Components Added

### 1. Database Layer (`database.py`)

#### New Model
- **TelemetryEvent**: SQLAlchemy ORM model with fields:
  - `id`: Auto-incrementing primary key
  - `chain_id`: Execution chain identifier (indexed)
  - `timestamp`: ISO 8601 timestamp from agent
  - `event_type`: Event classification (indexed)
  - `phase`: Operational phase number
  - `status`: Event status (indexed)
  - `details_json`: JSON details stored as TEXT
  - `received_at`: Server receipt time (indexed, auto)

#### New Functions
```python
save_telemetry_event(db, chain_id, timestamp, event_type, phase, status, details_json)
get_telemetry_events(db, chain_id, limit=50)
get_telemetry_events_range(db, chain_id, start_timestamp, end_timestamp)
cleanup_old_telemetry(db, hours=24)
```

### 2. API Layer (`app.py`)

#### New Pydantic Models
- **TelemetryEventRequest**: Agent telemetry submission format
- **TelemetryEventResponse**: Telemetry event response format
- **TelemetryChainTimeline**: Timeline for a chain (list of events)
- **TelemetrySummary**: Aggregated statistics for a chain

#### New Endpoints

| Method | Path | Purpose |
|--------|------|---------|
| POST | `/api/telemetry/report` | Submit telemetry event |
| GET | `/api/telemetry/chain/{chain_id}` | Get chain timeline |
| GET | `/api/telemetry/summary` | Get chain statistics |
| WS | `/ws/telemetry/{chain_id}` | Live stream telemetry |

#### New Components
- **telemetry_ws_connections**: Dict mapping chain_id → list of WebSocket connections
- **telemetry_cache**: In-memory cache (deque with maxlen=100) per chain
- **telemetry_cleanup_task()**: Background task for old event cleanup
- **broadcast_telemetry_event()**: Helper to send events to all WebSocket clients

### 3. Startup Integration

```python
@app.on_event("startup")
async def startup_event():
    init_db()
    asyncio.create_task(telemetry_cleanup_task())
```

The telemetry cleanup task runs in the background, deleting events older than 24 hours every hour.

## Data Flow

### 1. Agent Reporting Event
```
Agent Binary
    ↓
POST /api/telemetry/report
    ↓
TelemetryEventRequest validation
    ↓
save_telemetry_event() → Database
    ↓
Add to telemetry_cache[chain_id]
    ↓
broadcast_telemetry_event() → WebSocket clients
    ↓
Return TelemetryEventResponse (202 Accepted)
```

### 2. Dashboard Querying Timeline
```
Web Frontend
    ↓
GET /api/telemetry/chain/{chain_id}
    ↓
Query TelemetryEvent table
    ↓
Order by received_at, return chronologically
    ↓
Return TelemetryChainTimeline
```

### 3. Dashboard Getting Statistics
```
Web Frontend
    ↓
GET /api/telemetry/summary?chain_id=...
    ↓
Query all events for chain
    ↓
Aggregate by event_type:
  - Count phase_start → current_phase
  - Count phase_complete → phases_completed
  - Count driver_attempt/success
  - Sum data_collected bytes
  - Calculate exfil success rate
  - Calculate execution time
    ↓
Return TelemetrySummary
```

### 4. Live Streaming via WebSocket
```
Web Frontend
    ↓
Connect WS /ws/telemetry/{chain_id}
    ↓
Send last 50 events as history (from cache if available)
    ↓
Keep connection open
    ↓
On new POST /api/telemetry/report for same chain_id:
  - broadcast_telemetry_event() sends to all connected WebSocket clients
```

## Performance Optimizations

### In-Memory Cache
- Each chain keeps last 100 events in a Python `deque`
- On new event: O(1) append to cache
- On WebSocket connect: O(n) copy of cache to client (n ≤ 100)
- Avoids database round-trip for recent history

### Database Indexes
- `chain_id`: Fast lookups per chain
- `event_type`: Fast filtering by event type
- `timestamp`: Chronological queries
- `received_at`: Auto-cleanup queries

### Automatic Cleanup
- Background task runs hourly
- Deletes events older than 24 hours
- Prevents unbounded database growth
- Configurable retention policy via `cleanup_old_telemetry(db, hours=N)`

## Testing

### Unit Test Example

```python
import pytest
from datetime import datetime

def test_telemetry_event_save(db):
    event = save_telemetry_event(
        db,
        chain_id="test_chain_1",
        timestamp="2024-10-10T12:00:00Z",
        event_type="phase_start",
        phase=1,
        status="running",
        details_json='{"module": "recon"}'
    )
    
    assert event.id is not None
    assert event.chain_id == "test_chain_1"
    assert event.phase == 1

def test_telemetry_cache_insert():
    cache = {}
    cache["chain_1"] = deque(maxlen=100)
    
    event = {"id": 1, "event_type": "phase_start"}
    cache["chain_1"].append(event)
    
    assert len(cache["chain_1"]) == 1
    assert list(cache["chain_1"])[0]["event_type"] == "phase_start"
```

### Integration Test Script

Run the provided test script:

```bash
cd /home/incharanew/JOCKY/web/backend
chmod +x test_telemetry.sh
./test_telemetry.sh
```

This will:
1. Report phase_start event
2. Report data_collected event
3. Report driver_attempt event
4. Query chain timeline
5. Get chain summary statistics

### Manual Testing with cURL

```bash
# 1. Start the backend
cd /home/incharanew/JOCKY/web/backend
python3 -m uvicorn app:app --host 0.0.0.0 --port 8000

# In another terminal:

# 2. Report events
curl -X POST http://localhost:8000/api/telemetry/report \
  -H "Content-Type: application/json" \
  -d '{
    "timestamp": "2024-10-10T12:00:00Z",
    "chain_id": "chain_test_1",
    "event_type": "phase_start",
    "phase": 1,
    "status": "running",
    "details": {"target": "192.168.1.1"}
  }'

# 3. Get chain timeline
curl http://localhost:8000/api/telemetry/chain/chain_test_1

# 4. Get summary
curl "http://localhost:8000/api/telemetry/summary?chain_id=chain_test_1"

# 5. Connect WebSocket (requires wscat)
# npm install -g wscat
wscat -c ws://localhost:8000/ws/telemetry/chain_test_1
```

### Manual Testing with Python

```python
import requests
import json
from datetime import datetime

BASE_URL = "http://localhost:8000"
chain_id = "test_chain_1"

# Report event
resp = requests.post(
    f"{BASE_URL}/api/telemetry/report",
    json={
        "timestamp": datetime.utcnow().isoformat() + "Z",
        "chain_id": chain_id,
        "event_type": "phase_start",
        "phase": 1,
        "status": "running",
        "details": {"module": "reconnaissance"}
    }
)
print("Report response:", resp.json())

# Get timeline
timeline = requests.get(f"{BASE_URL}/api/telemetry/chain/{chain_id}").json()
print(f"Timeline has {timeline['total_events']} events")

# Get summary
summary = requests.get(f"{BASE_URL}/api/telemetry/summary", params={"chain_id": chain_id}).json()
print(f"Summary: phase {summary['current_phase']}, {summary['drivers_successful']} drivers successful")

# WebSocket
import websockets
import asyncio

async def listen():
    async with websockets.connect(f"ws://localhost:8000/ws/telemetry/{chain_id}") as ws:
        # Send new event and listen
        for i in range(3):
            await asyncio.sleep(1)
            resp = requests.post(
                f"{BASE_URL}/api/telemetry/report",
                json={
                    "timestamp": datetime.utcnow().isoformat() + "Z",
                    "chain_id": chain_id,
                    "event_type": f"event_{i}",
                    "phase": 1,
                    "status": "success"
                }
            )
            
            msg = await ws.recv()
            data = json.loads(msg)
            if data.get("event"):
                print(f"Received: {data['event']['event_type']}")

asyncio.run(listen())
```

## Event Type Reference

### Reconnaissance
- `phase_start`: Start of reconnaissance phase
- `target_enum`: Network enumeration attempt
- `user_enum`: User enumeration attempt
- `process_enum`: Running process enumeration
- `data_collected`: Data successfully collected

### Exploitation
- `driver_attempt`: Attempt to load BYOVD driver
- `driver_success`: Driver loaded successfully
- `driver_failed`: Driver load failed
- `exploit_attempt`: Exploitation attempt
- `exploit_success`: Exploitation succeeded

### Exfiltration
- `exfil_attempt`: Data exfiltration attempt
- `exfil_success`: Data successfully exfiltrated
- `exfil_failed`: Exfiltration blocked/failed
- `channel_test`: Testing exfil channel

### Evasion
- `detection_evasion`: Evasion technique applied
- `edr_callback`: EDR callback detected
- `api_hook_bypass`: API hook bypassed
- `amsi_bypass`: AMSI bypassed

### Errors
- `error`: Generic error event
- `timeout`: Operation timeout
- `permission_denied`: Access denied
- `detection`: Detection by security software

## Database Maintenance

### Check Database Size

```bash
sqlite3 /home/incharanew/JOCKY/jocky_jobs.db "SELECT page_count * page_size as size FROM pragma_page_count(), pragma_page_size();"
```

### Manual Cleanup (Optional)

```bash
sqlite3 /home/incharanew/JOCKY/jocky_jobs.db "DELETE FROM telemetry_events WHERE datetime(received_at) < datetime('now', '-24 hours');"
```

### Export Events for Analysis

```bash
sqlite3 /home/incharanew/JOCKY/jocky_jobs.db "SELECT * FROM telemetry_events WHERE chain_id = 'chain_abc123' ORDER BY received_at;" > events.csv
```

## Future Enhancements

1. **Telemetry Filtering**: POST filter query (event_type, phase range, status)
2. **Batch Reporting**: Allow agent to send multiple events in one request
3. **Event Compression**: GZip compress details_json for large payloads
4. **Retention Policies**: Per-chain retention (some chains keep longer)
5. **Telemetry Analytics**: Pre-computed summaries updated on event arrival
6. **Rate Limiting**: Per-chain event limits
7. **Event Signing**: Cryptographic verification of agent events
8. **Encrypted Channel**: TLS for telemetry channel
9. **Backpressure**: Queue management if agent sends events faster than DB writes
10. **Event Replicas**: Replicate to secondary backend for HA

## Architecture Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                     FastAPI Application                      │
├─────────────────────────────────────────────────────────────┤
│                                                               │
│  ┌──────────────────┐         ┌──────────────────────────┐  │
│  │  HTTP Endpoints  │         │   WebSocket Handler      │  │
│  ├──────────────────┤         ├──────────────────────────┤  │
│  │ POST /report     │◄─────┐  │ WS /ws/telemetry/{id}    │  │
│  │ GET  /chain/{id} │      │  │ - Send history on connect│  │
│  │ GET  /summary    │      │  │ - Broadcast new events   │  │
│  └──────────────────┘      │  └──────────────────────────┘  │
│                      ┌─────┴──────────────┐                 │
│  ┌──────────────────┴─────┐    ┌──────────┴──────────────┐  │
│  │  In-Memory Cache       │    │  broadcast_telemetry   │  │
│  │  (chain_id → deque)    │◄───┤  event()               │  │
│  │  (last 100 events)     │    └────────────────────────┘  │
│  └────────────┬───────────┘                                 │
│               │                                               │
│  ┌────────────▼───────────┐                                  │
│  │  Database Layer        │                                  │
│  ├────────────────────────┤                                  │
│  │  SessionLocal factory  │                                  │
│  │  save_telemetry_event()│                                  │
│  │  get_telemetry_events()│                                  │
│  │  cleanup_old_telemetry()                                 │
│  └────────────┬───────────┘                                  │
│               │                                               │
│  ┌────────────▼───────────────────────┐                     │
│  │  SQLite (jocky_jobs.db)            │                     │
│  ├──────────────────────────────────┤                       │
│  │  telemetry_events (table)         │                     │
│  │  - id (PK)                        │                     │
│  │  - chain_id (indexed)             │                     │
│  │  - event_type (indexed)           │                     │
│  │  - timestamp                      │                     │
│  │  - phase                          │                     │
│  │  - status                         │                     │
│  │  - details_json                   │                     │
│  │  - received_at (indexed)          │                     │
│  └───────────────────────────────────┘                     │
│                                                               │
│  ┌────────────────────────────────────┐                     │
│  │  Background Task: telemetry_cleanup │                     │
│  │  - Runs hourly                      │                     │
│  │  - Deletes events > 24h old        │                     │
│  └────────────────────────────────────┘                     │
│                                                               │
└─────────────────────────────────────────────────────────────┘

Agent Binary ──HTTP─→ POST /api/telemetry/report
                        ↓
                     Save to DB
                     Add to Cache
                     Broadcast to WS
                        ↓
Web Dashboard ←─WS─ /ws/telemetry/{chain_id}
    │           (history + stream)
    │
    ├─→ GET /api/telemetry/chain/{chain_id} ──→ Full Timeline
    │
    └─→ GET /api/telemetry/summary ───→ Aggregated Stats
```

## Files Modified/Added

### Modified
- `/home/incharanew/JOCKY/web/backend/database.py`
  - Added TelemetryEvent model
  - Added telemetry database functions

- `/home/incharanew/JOCKY/web/backend/app.py`
  - Added telemetry models
  - Added telemetry endpoints
  - Added WebSocket handler
  - Added in-memory cache
  - Added background cleanup task

### Added
- `/home/incharanew/JOCKY/web/backend/TELEMETRY_API.md` - API documentation
- `/home/incharanew/JOCKY/web/backend/test_telemetry.sh` - Test script
- `/home/incharanew/JOCKY/web/backend/TELEMETRY_IMPLEMENTATION.md` - This file
