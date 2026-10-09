# C2 Telemetry API Documentation

This document describes the real-time telemetry collection and streaming infrastructure for the JOCKY C2 backend.

## Overview

The telemetry system allows deployed agents and binaries to report execution events in real-time, which are:
- Persisted to SQLite database (`telemetry_events` table)
- Cached in-memory for fast retrieval (last 100 events per chain)
- Broadcast to connected WebSocket clients for live monitoring
- Automatically cleaned up after 24 hours

## Database Schema

```sql
CREATE TABLE telemetry_events (
    id INTEGER PRIMARY KEY,
    timestamp TEXT,                    -- ISO 8601 timestamp from agent
    chain_id TEXT,                     -- Execution chain identifier
    event_type TEXT,                   -- Event classification
    phase INTEGER,                     -- Operational phase (1-N)
    status TEXT,                       -- Event status (running, success, failed, etc.)
    details_json TEXT,                 -- JSON blob for event-specific data
    received_at DATETIME DEFAULT CURRENT_TIMESTAMP  -- Server receipt time
)
```

Indexes on: `chain_id`, `event_type`, `timestamp`, `received_at`

## Telemetry Events

Common event types and their meaning:

| Event Type | Phase | Status | Details | Example |
|------------|-------|--------|---------|---------|
| `phase_start` | Y | running | `{"module": "reconnaissance"}` | Start of phase 1 |
| `phase_complete` | N | success | `{"duration_ms": 1250}` | Phase 2 completed |
| `driver_attempt` | Y | running | `{"driver": "ntfs_driver.sys", "method": "exploitX"}` | Trying a driver |
| `driver_success` | Y | success | `{"driver": "ntfs_driver.sys", "handle": "0x1234"}` | Driver loaded |
| `driver_failed` | Y | failed | `{"driver": "foo.sys", "error": "signature invalid"}` | Driver blocked |
| `data_collected` | Y | success | `{"bytes": 5242880, "source": "C:\Users\Admin\Desktop"}` | 5 MB collected |
| `exfil_attempt` | Y | running | `{"channel": "dns", "destination": "attacker.com"}` | Starting exfil |
| `exfil_success` | Y | success | `{"channel": "https", "bytes_sent": 10485760}` | Data exfiltrated |
| `exfil_failed` | Y | failed | `{"channel": "dns", "error": "timeout"}` | Exfil blocked |
| `detection_evasion` | Y | success | `{"technique": "amsi_bypass", "method": "winapi_hooking"}` | EDR evaded |
| `error` | Y | failed | `{"component": "driver_loader", "error": "access denied"}` | Runtime error |

## API Endpoints

### 1. Report Telemetry Event

**POST** `/api/telemetry/report`

Submit a telemetry event from a deployed agent/binary.

#### Request

```json
{
  "timestamp": "2024-10-10T12:34:56.789Z",
  "chain_id": "chain_abc123def456",
  "event_type": "phase_start",
  "phase": 1,
  "status": "running",
  "details": {
    "module": "reconnaissance",
    "target_scope": "enterprise_network"
  }
}
```

#### Response

```json
{
  "id": 1,
  "timestamp": "2024-10-10T12:34:56.789Z",
  "chain_id": "chain_abc123def456",
  "event_type": "phase_start",
  "phase": 1,
  "status": "running",
  "details_json": "{\"module\": \"reconnaissance\", \"target_scope\": \"enterprise_network\"}",
  "received_at": "2024-10-10T12:34:57.123Z"
}
```

#### cURL Example

```bash
curl -X POST http://localhost:8000/api/telemetry/report \
  -H "Content-Type: application/json" \
  -d '{
    "timestamp": "2024-10-10T12:34:56Z",
    "chain_id": "chain_abc123",
    "event_type": "phase_start",
    "phase": 1,
    "status": "running",
    "details": {"module": "reconnaissance"}
  }'
```

### 2. Get Chain Timeline

**GET** `/api/telemetry/chain/{chain_id}`

Retrieve full execution timeline for a chain (all events in chronological order).

#### Query Parameters

- `limit` (int, default=50): Maximum number of events to return

#### Response

```json
{
  "chain_id": "chain_abc123",
  "events": [
    {
      "id": 1,
      "timestamp": "2024-10-10T12:34:56.123Z",
      "chain_id": "chain_abc123",
      "event_type": "phase_start",
      "phase": 1,
      "status": "running",
      "details_json": null,
      "received_at": "2024-10-10T12:34:57.456Z"
    },
    {
      "id": 2,
      "timestamp": "2024-10-10T12:35:00.456Z",
      "chain_id": "chain_abc123",
      "event_type": "data_collected",
      "phase": 1,
      "status": "success",
      "details_json": "{\"bytes\": 1048576}",
      "received_at": "2024-10-10T12:35:01.789Z"
    }
  ],
  "total_events": 2
}
```

#### cURL Example

```bash
curl http://localhost:8000/api/telemetry/chain/chain_abc123?limit=100
```

### 3. Get Chain Summary Statistics

**GET** `/api/telemetry/summary`

Get aggregated summary statistics for a chain's execution.

#### Query Parameters

- `chain_id` (string, required): Chain identifier

#### Response

```json
{
  "chain_id": "chain_abc123",
  "current_phase": 3,
  "phases_completed": 2,
  "drivers_attempted": 5,
  "drivers_successful": 4,
  "data_collected_bytes": 52428800,
  "exfil_success_rate": 100.0,
  "execution_time_seconds": 3600,
  "first_event_timestamp": "2024-10-10T12:00:00.000Z",
  "last_event_timestamp": "2024-10-10T13:00:00.000Z"
}
```

#### Calculated Fields

- **current_phase**: Highest phase number seen in `phase_start` events
- **phases_completed**: Count of `phase_complete` events
- **drivers_attempted**: Count of `driver_attempt` events
- **drivers_successful**: Count of `driver_success` events
- **data_collected_bytes**: Sum of `bytes` from all `data_collected` events
- **exfil_success_rate**: Percentage of successful exfil attempts
- **execution_time_seconds**: Duration from first to last event

#### cURL Example

```bash
curl "http://localhost:8000/api/telemetry/summary?chain_id=chain_abc123"
```

### 4. WebSocket Live Stream

**WS** `/ws/telemetry/{chain_id}`

Connect to receive live telemetry updates for a chain. On connection, the server sends:
1. Historical events (last 50 from cache/DB)
2. New events in real-time as they arrive

#### Initial Message (Connection)

```json
{
  "type": "history",
  "events": [
    {
      "id": 1,
      "timestamp": "2024-10-10T12:34:56.123Z",
      "chain_id": "chain_abc123",
      "event_type": "phase_start",
      "phase": 1,
      "status": "running",
      "details_json": null,
      "received_at": "2024-10-10T12:34:57.456Z"
    }
  ],
  "count": 1
}
```

#### Incoming Events

```json
{
  "event": {
    "id": 2,
    "timestamp": "2024-10-10T12:35:00.456Z",
    "chain_id": "chain_abc123",
    "event_type": "data_collected",
    "phase": 1,
    "status": "success",
    "details_json": "{\"bytes\": 1048576}",
    "received_at": "2024-10-10T12:35:01.789Z"
  }
}
```

#### JavaScript Example

```javascript
const chain_id = "chain_abc123";
const ws = new WebSocket(`ws://localhost:8000/ws/telemetry/${chain_id}`);

ws.onopen = () => {
  console.log("Connected to telemetry stream");
};

ws.onmessage = (msg) => {
  const data = JSON.parse(msg.data);
  
  if (data.type === "history") {
    console.log(`Received ${data.count} historical events`);
    data.events.forEach(event => {
      console.log(`  [${event.event_type}] ${event.status}`);
    });
  } else if (data.event) {
    const event = data.event;
    console.log(`[${event.timestamp}] ${event.event_type}: ${event.status}`);
  }
};

ws.onerror = (error) => {
  console.error("WebSocket error:", error);
};

ws.onclose = () => {
  console.log("Disconnected from telemetry stream");
};
```

#### HTML5 Example (React)

```jsx
import { useEffect, useState } from 'react';

export function TelemetryMonitor({ chainId }) {
  const [events, setEvents] = useState([]);
  const [summary, setSummary] = useState(null);

  useEffect(() => {
    const ws = new WebSocket(`ws://localhost:8000/ws/telemetry/${chainId}`);

    ws.onmessage = async (msg) => {
      const data = JSON.parse(msg.data);
      
      if (data.type === "history") {
        setEvents(data.events);
      } else if (data.event) {
        setEvents(prev => [...prev, data.event]);
        
        // Refresh summary on new events
        const response = await fetch(
          `http://localhost:8000/api/telemetry/summary?chain_id=${chainId}`
        );
        setSummary(await response.json());
      }
    };

    return () => ws.close();
  }, [chainId]);

  return (
    <div>
      <h2>Chain: {chainId}</h2>
      {summary && (
        <div>
          <p>Phase: {summary.current_phase}</p>
          <p>Drivers: {summary.drivers_successful}/{summary.drivers_attempted}</p>
          <p>Data: {(summary.data_collected_bytes / 1024 / 1024).toFixed(2)} MB</p>
        </div>
      )}
      <ul>
        {events.map(e => (
          <li key={e.id}>
            {e.timestamp}: {e.event_type} ({e.status})
          </li>
        ))}
      </ul>
    </div>
  );
}
```

#### Python Example

```python
import asyncio
import json
import websockets

async def monitor_chain(chain_id):
    uri = f"ws://localhost:8000/ws/telemetry/{chain_id}"
    async with websockets.connect(uri) as ws:
        while True:
            msg = await ws.recv()
            data = json.loads(msg)
            
            if data.get("type") == "history":
                print(f"Received {data['count']} historical events")
            elif data.get("event"):
                event = data["event"]
                print(f"[{event['timestamp']}] {event['event_type']}: {event['status']}")

# Usage
asyncio.run(monitor_chain("chain_abc123"))
```

## In-Memory Cache

For performance, the last 100 events per chain are kept in memory (Python `deque`):

- On POST `/api/telemetry/report`: Event is added to cache
- On WebSocket connection: If cache has events, they're sent immediately (faster than DB query)
- On `GET /api/telemetry/chain/{chain_id}`: Falls back to DB if cache is empty
- Cache is cleared for a chain when WebSocket connections close

## Automatic Cleanup

A background task runs every hour to delete events older than 24 hours from the database. This keeps the database size manageable while allowing real-time streaming for active chains.

## Integration Example

### Agent-side (deployed binary)

```c
// Simple telemetry reporter in C
#include <curl/curl.h>
#include <time.h>
#include <json-c/json.h>

void report_event(const char *chain_id, const char *event_type, int phase, const char *status, json_object *details) {
    time_t now = time(NULL);
    struct tm *tm_info = gmtime(&now);
    char timestamp[30];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", tm_info);

    json_object *payload = json_object_new_object();
    json_object_object_add(payload, "timestamp", json_object_new_string(timestamp));
    json_object_object_add(payload, "chain_id", json_object_new_string(chain_id));
    json_object_object_add(payload, "event_type", json_object_new_string(event_type));
    if (phase > 0) json_object_object_add(payload, "phase", json_object_new_int(phase));
    json_object_object_add(payload, "status", json_object_new_string(status));
    if (details) json_object_object_add(payload, "details", details);

    const char *json_str = json_object_to_json_string(payload);
    
    CURL *curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://c2.local:8000/api/telemetry/report");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_str);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    
    json_object_put(payload);
}
```

### Dashboard-side (web frontend)

```javascript
class TelemetryDashboard {
  constructor(containerId) {
    this.container = document.getElementById(containerId);
    this.chains = new Map();
  }

  subscribeToChain(chainId) {
    const ws = new WebSocket(`ws://localhost:8000/ws/telemetry/${chainId}`);
    
    ws.onmessage = async (msg) => {
      const data = JSON.parse(msg.data);
      
      if (data.event) {
        this.updateChainUI(chainId, data.event);
      }
    };

    this.chains.set(chainId, ws);
  }

  async updateChainUI(chainId, event) {
    const summary = await fetch(
      `http://localhost:8000/api/telemetry/summary?chain_id=${chainId}`
    ).then(r => r.json());

    this.container.innerHTML += `
      <div class="chain-event">
        <strong>${event.event_type}</strong>: ${event.status}
        <br/>Phase ${summary.current_phase}: ${summary.drivers_successful}/${summary.drivers_attempted} drivers
        <br/>Data: ${(summary.data_collected_bytes / 1024 / 1024).toFixed(1)} MB
      </div>
    `;
  }
}
```

## Error Handling

All endpoints return standard HTTP status codes:

- `200 OK`: Successful request
- `404 Not Found`: Chain ID doesn't exist
- `422 Unprocessable Entity`: Invalid request payload
- `500 Internal Server Error`: Database or server error

## Rate Limiting

None implemented by default. For production, add:
- Per-chain_id rate limits (100 events/min)
- Per-IP rate limits
- Total database size caps
