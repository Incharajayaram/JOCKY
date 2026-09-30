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
from datetime import datetime as dt
from metrics import get_metric


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

    # Categorize passes: some must be top-level, some go in function() wrapper
    llvm_function_passes = []
    llvm_toplevel_passes = []

    for p in LLVM_PASSES:
        enabled = llvm_cfg.get(p["id"], p["default"])
        if enabled:
            # These passes must be top-level, not wrapped in function()
            if p["flag"] in ("strip-signature", "virtualize", "anti-debug", "indirect-call"):
                llvm_toplevel_passes.append(p["flag"])
            else:
                llvm_function_passes.append(p["flag"])

    # Build pass string: strip-signature,virtualize,function(...),anti-debug,indirect-call
    llvm_pass_str = ""
    parts = []

    # Add strip-signature first if present
    if "strip-signature" in llvm_toplevel_passes:
        parts.append("strip-signature")
        llvm_toplevel_passes.remove("strip-signature")

    # Add virtualize before function wrapper if present
    if "virtualize" in llvm_toplevel_passes:
        parts.append("virtualize")
        llvm_toplevel_passes.remove("virtualize")

    # Add function passes
    if llvm_function_passes:
        parts.append(f"function({','.join(llvm_function_passes)})")

    # Add remaining top-level passes (anti-debug, indirect-call)
    parts.extend(llvm_toplevel_passes)

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
    job_id: str = None,
):
    build_dir = None
    source_dir = None
    try:
        job.status = JobStatus.RUNNING
        job.progress = 5
        job.logs.append("[PIPELINE] Starting compilation")
        if notify_callback:
            await notify_callback(job)

        # Use project-relative directories (Docker-mountable)
        project_root = Path(__file__).resolve().parent.parent.parent
        build_base = project_root / "build"
        build_base.mkdir(exist_ok=True)
        build_dir = build_base / f"jocky_build_{uuid.uuid4().hex[:8]}"
        build_dir.mkdir(parents=True, exist_ok=True)

        # Use project-relative source directory instead of /tmp
        src_base = project_root / "build" / "sources"
        src_base.mkdir(exist_ok=True)
        source_dir = src_base / f"jocky_src_{uuid.uuid4().hex[:8]}"
        source_dir.mkdir(parents=True, exist_ok=True)
        source_path = source_dir / "source.jky"
        source_path.write_text(source)

        job.progress = 10
        job.logs.append(f"[PIPELINE] Platform: {job.platform}")
        job.logs.append(f"[PIPELINE] Obfuscation preset: {preset}")
        if notify_callback:
            await notify_callback(job)

        obf_cfg = _build_obfuscation_config(obfuscation)

        # Get compile script (already have project_root from build_dir setup)
        compile_script = project_root / "scripts" / "compile_pipeline.py"

        # Use local compilation only (Docker disabled for development)
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
                # Use = syntax to avoid argparse issues with values starting with --
                cmd.append(f"--mlir-passes={','.join(obf_cfg['mlir_flags'])}")
            if obf_cfg.get("llvm_passes"):
                cmd.append(f"--llvm-passes={obf_cfg['llvm_passes']}")
        else:
            job.progress = 15
            job.logs.append("[PIPELINE] Using local compile pipeline (Docker not available)")
            if notify_callback:
                await notify_callback(job)

            cmd = [
                "python3",
                str(compile_script),
                str(source_path),
                str(build_dir),
                "--platform", job.platform,
                "--preset", preset,
            ]

            if obf_cfg.get("mlir_flags"):
                # Use = syntax to avoid argparse issues with values starting with --
                cmd.append(f"--mlir-passes={','.join(obf_cfg['mlir_flags'])}")
            if obf_cfg.get("llvm_passes"):
                cmd.append(f"--llvm-passes={obf_cfg['llvm_passes']}")

        # Set up environment with proper paths
        env = os.environ.copy()
        env["PYTHONPATH"] = str(project_root / "src")
        env["TOOLCHAIN_PATH"] = str(project_root / "toolchain")
        toolchain_lib = str(project_root / "toolchain" / "lib")
        env["LD_LIBRARY_PATH"] = f"{toolchain_lib}:{env.get('LD_LIBRARY_PATH', '')}"

        job.logs.append(f"[PIPELINE] LD_LIBRARY_PATH={env.get('LD_LIBRARY_PATH', 'NOT SET')}")

        # Try Docker with automatic local fallback on mount failures
        docker_failed_mount = False
        if use_docker:
            try:
                process = await asyncio.create_subprocess_exec(
                    *cmd,
                    stdout=asyncio.subprocess.PIPE,
                    stderr=asyncio.subprocess.STDOUT,
                    cwd=str(project_root) if not use_docker else None,
                    env=env,
                )

                docker_output = []
                while True:
                    line = await process.stdout.readline()
                    if not line:
                        break
                    decoded = line.decode("utf-8", errors="replace").rstrip()
                    docker_output.append(decoded)
                    job.logs.append(decoded)

                await process.wait()

                # Check if Docker failed due to mount issues
                docker_stderr = "\n".join(docker_output)
                if process.returncode != 0 and ("not shared from the host" in docker_stderr or "mounts denied" in docker_stderr):
                    docker_failed_mount = True
                    job.logs.append("[PIPELINE] Docker mount failed, falling back to local compilation")
                    job.progress = 15  # Reset to retry with local
                elif process.returncode != 0:
                    # Docker failed for other reasons, don't retry with local
                    use_docker = False  # Prevent status check
                else:
                    # Docker succeeded
                    use_docker = False  # Mark completion
            except Exception as e:
                job.logs.append(f"[PIPELINE] Docker execution failed: {str(e)}")
                docker_failed_mount = True
                job.progress = 15

        # If Docker mount failed or wasn't available, use local compilation
        if not use_docker or docker_failed_mount:
            if docker_failed_mount:
                # Rebuild command for local compilation
                cmd = [
                    "python3",
                    str(compile_script),
                    str(source_path),
                    str(build_dir),
                    "--platform", job.platform,
                    "--preset", preset,
                ]

                if obf_cfg.get("mlir_flags"):
                    cmd.extend(["--mlir-passes", ",".join(obf_cfg["mlir_flags"])])
                if obf_cfg.get("llvm_passes"):
                    cmd.extend(["--llvm-passes", obf_cfg["llvm_passes"]])

                job.logs.append("[PIPELINE] Starting local compilation")
                if notify_callback:
                    await notify_callback(job)

            process = await asyncio.create_subprocess_exec(
                *cmd,
                stdout=asyncio.subprocess.PIPE,
                stderr=asyncio.subprocess.STDOUT,
                cwd=str(project_root),
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
            # Save output to persistent location, cleanup temp build_dir
            artifacts_dir = project_root / "build" / "artifacts"
            artifacts_dir.mkdir(parents=True, exist_ok=True)

            persistent_output = artifacts_dir / f"{job.job_id}_{output_name}"
            shutil.copy2(output_file, persistent_output)

            job.status = JobStatus.COMPLETED
            job.progress = 100
            job.output_path = str(persistent_output)
            job.output_name = output_name
            file_size = persistent_output.stat().st_size
            size_mb = file_size / (1024 * 1024)
            job.logs.append(f"[PIPELINE] Build complete: {output_name} ({size_mb:.2f} MB)")

            # Finalize metrics
            if job_id:
                metric = get_metric(job_id)
                if metric:
                    metric.finalize(dt.utcnow(), file_size, True)
        else:
            job.status = JobStatus.FAILED
            job.progress = 100
            job.logs.append("[PIPELINE] Build produced no output binary")

            # Finalize metrics as failed
            if job_id:
                metric = get_metric(job_id)
                if metric:
                    metric.finalize(dt.utcnow(), 0, False)

        if notify_callback:
            await notify_callback(job)

    except Exception as e:
        job.status = JobStatus.FAILED
        job.progress = 100
        job.logs.append(f"[PIPELINE] Error: {str(e)}")

        # Finalize metrics on exception
        if job_id:
            metric = get_metric(job_id)
            if metric:
                metric.finalize(dt.utcnow(), 0, False)

        if notify_callback:
            await notify_callback(job)

    finally:
        if source_dir:
            shutil.rmtree(source_dir, ignore_errors=True)
        if build_dir:
            shutil.rmtree(build_dir, ignore_errors=True)
