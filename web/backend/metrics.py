from datetime import datetime
from typing import Optional
from dataclasses import dataclass, asdict


@dataclass
class CompilationMetrics:
    job_id: str
    platform: str
    preset: str
    start_time: datetime
    end_time: Optional[datetime] = None
    source_size: int = 0
    output_size: int = 0
    compilation_time_ms: int = 0
    mlir_passes_enabled: list[str] = None
    llvm_passes_enabled: list[str] = None
    success: bool = False

    def __post_init__(self):
        if self.mlir_passes_enabled is None:
            self.mlir_passes_enabled = []
        if self.llvm_passes_enabled is None:
            self.llvm_passes_enabled = []

    def finalize(self, end_time: datetime, output_size: int, success: bool):
        self.end_time = end_time
        self.output_size = output_size
        self.success = success
        self.compilation_time_ms = int((end_time - self.start_time).total_seconds() * 1000)

    def to_dict(self):
        return asdict(self)


metrics_store: dict[str, CompilationMetrics] = {}


def create_metric(job_id: str, platform: str, preset: str, source_size: int, mlir_passes: list[str], llvm_passes: list[str]) -> CompilationMetrics:
    metric = CompilationMetrics(
        job_id=job_id,
        platform=platform,
        preset=preset,
        start_time=datetime.utcnow(),
        source_size=source_size,
        mlir_passes_enabled=mlir_passes,
        llvm_passes_enabled=llvm_passes,
    )
    metrics_store[job_id] = metric
    return metric


def get_metric(job_id: str) -> Optional[CompilationMetrics]:
    return metrics_store.get(job_id)


def get_all_metrics() -> dict[str, CompilationMetrics]:
    return metrics_store
