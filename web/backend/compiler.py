import asyncio
import json
import os
import shutil
import subprocess
import tempfile
import uuid
from dataclasses import dataclass, field
from enum import Enum
from pathlib import Path
from typing import Optional

from obfuscation import MLIR_PASSES, LLVM_PASSES


class JobStatus(str, Enum):
    PENDING = "pending"
    RUNNING = "running"
    COMPLETED = "completed"
    FAILED = "failed"


@dataclass
class CompileJob:
    job_id: str
    status: JobStatus = JobStatus.PENDING
    logs: list[str] = field(default_factory=list)
    progress: int = 0
    platform: str = "windows"
    output_path: Optional[str] = None
    output_name: Optional[str] = None


jobs: dict[str, CompileJob] = {}


def create_job(platform: str) -> CompileJob:
    job_id = str(uuid.uuid4())
    job = CompileJob(job_id=job_id, platform=platform)
    jobs[job_id] = job
    return job


def get_job(job_id: str) -> Optional[CompileJob]:
    return jobs.get(job_id)


def _build_obfuscation_config(obfuscation: dict) -> dict:
    mlir_cfg = obfuscation.get("mlir", {})
    llvm_cfg = obfuscation.get("llvm", {})

    mlir_flags = []
    for p in MLIR_PASSES:
        enabled = mlir_cfg.get(p["id"], p["default"])
        if enabled:
            mlir_flags.append(p["flag"])

    llvm_function_passes = []
    llvm_module_passes = []
    for p in LLVM_PASSES:
        enabled = llvm_cfg.get(p["id"], p["default"])
        if enabled:
            if p["flag"] in ("indirect-call", "strip-signature"):
                llvm_module_passes.append(p["flag"])
            else:
                llvm_function_passes.append(p["flag"])

    llvm_pass_str = ""
    parts = []
    if llvm_function_passes:
        parts.append(f"function({','.join(llvm_function_passes)})")
    if llvm_module_passes:
        parts.append(f"module({','.join(llvm_module_passes)})")
    if parts:
        llvm_pass_str = ",".join(parts)

    return {
        "mlir_flags": mlir_flags,
        "llvm_passes": llvm_pass_str,
    }


async def run_compilation(
    job: CompileJob,
    source: str,
    obfuscation: dict,
    notify_callback=None,
    preset: str = "standard",
):
    build_dir = None
    source_dir = None
    try:
        job.status = JobStatus.RUNNING
        job.progress = 5
        job.logs.append("[PIPELINE] Starting compilation")
        if notify_callback:
            await notify_callback(job)

        build_dir = tempfile.mkdtemp(prefix="jocky_build_")
        source_dir = tempfile.mkdtemp(prefix="jocky_src_")
        source_path = Path(source_dir) / "source.jky"
        source_path.write_text(source)

        job.progress = 10
        job.logs.append(f"[PIPELINE] Platform: {job.platform}")
        job.logs.append(f"[PIPELINE] Obfuscation preset: {preset}")
        if notify_callback:
            await notify_callback(job)

        obf_cfg = _build_obfuscation_config(obfuscation)

        # Get project root (backend is at web/backend, scripts is at root/scripts)
        project_root = Path(__file__).resolve().parent.parent.parent
        compile_script = project_root / "scripts" / "compile_pipeline.py"

        # Try Docker first, fall back to local compilation
        use_docker = False
        try:
            result = subprocess.run(["docker", "ps"], capture_output=True, timeout=5)
            if result.returncode == 0:
                use_docker = True
                # Verify jocky-compiler image exists
                result = subprocess.run(["docker", "images", "-q", "jocky-compiler:latest"],
                                       capture_output=True, timeout=5)
                use_docker = bool(result.stdout.strip())
        except (FileNotFoundError, subprocess.TimeoutExpired):
            use_docker = False

        if use_docker:
            job.progress = 15
            job.logs.append("[PIPELINE] Launching Docker container")
            if notify_callback:
                await notify_callback(job)

            cmd = [
                "docker", "run", "--rm",
                "-v", f"{build_dir}:/workspace/build",
                "-v", f"{source_path}:/workspace/jocky/source.jky:ro",
                "jocky-compiler:latest",
                "python3", "scripts/compile_pipeline.py",
                "/workspace/jocky/source.jky",
                "/workspace/build",
                "--platform", job.platform,
                "--preset", preset,
            ]

            if obf_cfg.get("mlir_flags"):
                cmd.extend(["--mlir-passes", ",".join(obf_cfg["mlir_flags"])])
            if obf_cfg.get("llvm_passes"):
                cmd.extend(["--llvm-passes", obf_cfg["llvm_passes"]])
        else:
            job.progress = 15
            job.logs.append("[PIPELINE] Using local compile pipeline (Docker not available)")
            if notify_callback:
                await notify_callback(job)

            cmd = [
                "python3",
                str(compile_script),
                str(source_path),
                build_dir,
                "--platform", job.platform,
                "--preset", preset,
            ]

            if obf_cfg.get("mlir_flags"):
                cmd.extend(["--mlir-passes", ",".join(obf_cfg["mlir_flags"])])
            if obf_cfg.get("llvm_passes"):
                cmd.extend(["--llvm-passes", obf_cfg["llvm_passes"]])

        # Set up environment with proper paths
        env = os.environ.copy()
        env["PYTHONPATH"] = str(project_root / "src")
        env["TOOLCHAIN_PATH"] = str(project_root / "toolchain")

        process = await asyncio.create_subprocess_exec(
            *cmd,
            stdout=asyncio.subprocess.PIPE,
            stderr=asyncio.subprocess.STDOUT,
            cwd=str(project_root) if not use_docker else None,
            env=env,
        )

        stage_progress = {
            "PARSE": 30,
            "CodeGen": 45,
            "MLIR": 55,
            "LLVM-OBF": 65,
            "COMPILE": 75,
            "RUNTIME": 82,
            "LINK": 90,
        }

        while True:
            line = await process.stdout.readline()
            if not line:
                break
            decoded = line.decode("utf-8", errors="replace").rstrip()
            job.logs.append(decoded)

            for stage, progress in stage_progress.items():
                if f"[{stage}]" in decoded:
                    job.progress = progress
                    break

            if notify_callback:
                await notify_callback(job)

        await process.wait()

        if process.returncode != 0:
            job.status = JobStatus.FAILED
            job.progress = 100
            job.logs.append(f"[PIPELINE] Build failed with exit code {process.returncode}")
            if notify_callback:
                await notify_callback(job)
            return

        if job.platform == "windows":
            output_file = Path(build_dir) / "source.exe"
            output_name = "source.exe"
        else:
            output_file = Path(build_dir) / "source"
            output_name = "source"

        if output_file.exists():
            job.status = JobStatus.COMPLETED
            job.progress = 100
            job.output_path = str(output_file)
            job.output_name = output_name
            file_size = output_file.stat().st_size
            size_mb = file_size / (1024 * 1024)
            job.logs.append(f"[PIPELINE] Build complete: {output_name} ({size_mb:.2f} MB)")
        else:
            job.status = JobStatus.FAILED
            job.progress = 100
            job.logs.append("[PIPELINE] Build produced no output binary")

        if notify_callback:
            await notify_callback(job)

    except Exception as e:
        job.status = JobStatus.FAILED
        job.progress = 100
        job.logs.append(f"[PIPELINE] Error: {str(e)}")
        if notify_callback:
            await notify_callback(job)

    finally:
        if source_dir:
            shutil.rmtree(source_dir, ignore_errors=True)
