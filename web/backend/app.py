import asyncio
import os
from pathlib import Path

from fastapi import FastAPI, WebSocket, WebSocketDisconnect, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import FileResponse
from pydantic import BaseModel, Field

from compiler import create_job, get_job, run_compilation, JobStatus
from runtime_apis import RUNTIME_APIS
from obfuscation import MLIR_PASSES, LLVM_PASSES
from demo_scripts import WINDOWS_DEMO, LINUX_DEMO


app = FastAPI(
    title="JOCKY Compiler API",
    version="1.0.0",
    docs_url="/docs",
    redoc_url="/redoc",
)

# CORS configuration - support localhost for dev, but allow production origins via env var
allowed_origins = os.getenv("CORS_ORIGINS", "http://localhost:3000,http://localhost:5173").split(",")
app.add_middleware(
    CORSMiddleware,
    allow_origins=allowed_origins,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

ws_connections: dict[str, list[WebSocket]] = {}


class MLIRConfig(BaseModel):
    string_encrypt: bool = True
    constant_obfuscate: bool = True
    symbol_obfuscate: bool = True


class LLVMConfig(BaseModel):
    boguscf: bool = True
    flattening: bool = True
    substitution: bool = True
    split: bool = True
    indirect_call: bool = True
    strip_signature: bool = True


class ObfuscationConfig(BaseModel):
    mlir: MLIRConfig = Field(default_factory=MLIRConfig)
    llvm: LLVMConfig = Field(default_factory=LLVMConfig)


class CompileRequest(BaseModel):
    source: str
    platform: str = "windows"
    obfuscation: ObfuscationConfig = Field(default_factory=ObfuscationConfig)
    preset: str = "standard"


class CompileResponse(BaseModel):
    job_id: str


class StatusResponse(BaseModel):
    status: str
    logs: list[str]
    progress: int


@app.post("/api/compile", response_model=CompileResponse)
async def compile_source(request: CompileRequest):
    if request.platform not in ("windows", "linux"):
        raise HTTPException(status_code=400, detail="Platform must be 'windows' or 'linux'")
    if request.preset not in ("none", "light", "standard", "aggressive"):
        raise HTTPException(status_code=400, detail="Preset must be 'none', 'light', 'standard', or 'aggressive'")
    if not request.source.strip():
        raise HTTPException(status_code=400, detail="Source code cannot be empty")

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


@app.get("/api/config")
async def get_config():
    """Get backend configuration for frontend"""
    return {
        "api_version": "1.0.0",
        "supported_platforms": ["windows", "linux"],
        "obfuscation_presets": ["none", "light", "standard", "aggressive"],
        "docs_url": "/docs",
    }


if __name__ == "__main__":
    import uvicorn
    host = os.getenv("BACKEND_HOST", "0.0.0.0")
    port = int(os.getenv("BACKEND_PORT", "8000"))
    uvicorn.run(app, host=host, port=port)
