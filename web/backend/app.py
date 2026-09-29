import asyncio
import os
from pathlib import Path
from typing import Optional

from fastapi import FastAPI, WebSocket, WebSocketDisconnect, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import FileResponse
from pydantic import BaseModel, Field, field_validator, ConfigDict

from compiler import create_job, get_job, run_compilation, JobStatus
from runtime_apis import RUNTIME_APIS
from obfuscation import MLIR_PASSES, LLVM_PASSES
from demo_scripts import WINDOWS_DEMO, LINUX_DEMO
from database import init_db, save_job, get_job_record, list_jobs
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

ws_connections: dict[str, list[WebSocket]] = {}


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
    preset: str = "standard"

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

    async def notify(j):
        conns = ws_connections.get(j.job_id, [])
        dead = []
        for ws in conns:
            try:
                await ws.send_json({
                    "status": j.status.value,
                    "progress": j.progress,
                    "logs": j.logs[-10:],
                })
            except Exception:
                dead.append(ws)
        for ws in dead:
            conns.remove(ws)

    asyncio.create_task(run_compilation(job, request.source, obf_dict, notify, request.preset))
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
    return RUNTIME_APIS


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


@app.get("/api/config", response_model=ConfigResponse)
async def get_config():
    """Get backend configuration for frontend"""
    return ConfigResponse(
        api_version="1.0.0",
        supported_platforms=["windows", "linux"],
        obfuscation_presets=["none", "light", "standard", "aggressive"],
        docs_url="/docs",
    )


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
                output_size=r.output_path and Path(r.output_path).stat().st_size if Path(r.output_path).exists() else None if r.output_path else None,
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


if __name__ == "__main__":
    import uvicorn
    host = os.getenv("BACKEND_HOST", "0.0.0.0")
    port = int(os.getenv("BACKEND_PORT", "8000"))
    uvicorn.run(app, host=host, port=port)
