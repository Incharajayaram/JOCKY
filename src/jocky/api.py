"""Programmatic Python API for the JOCKY compiler."""
from dataclasses import dataclass, field
from pathlib import Path
from typing import Optional, Dict, Any, List
import tempfile

@dataclass
class CompileResult:
    success: bool
    executable: Optional[Path] = None
    errors: List[str] = field(default_factory=list)
    warnings: List[str] = field(default_factory=list)
    ir_path: Optional[Path] = None  # path to generated LLVM IR file (if available)


def compile(
    source: Optional[str] = None,
    file: Optional[Path] = None,
    profile: str = "none",
    target: str = "native",
    output_dir: Optional[Path] = None,
    no_prelude: bool = False,
    extra_config: Optional[Dict[str, Any]] = None,
) -> CompileResult:
    """Compile a Jocky program.

    Args:
        source:     Jocky source code as a string (mutually exclusive with file).
        file:       Path to a .jky source file (mutually exclusive with source).
        profile:    Obfuscation profile name ("none", "light", "standard", "aggressive").
        target:     Target OS — "native", "windows", or "linux".
        output_dir: Directory for build artefacts and the final executable.
                    Defaults to a temporary directory (not cleaned up on success).
        no_prelude: Skip auto-injection of the standard prelude.
        extra_config: Additional key/value pairs forwarded to BuildContext.config.

    Returns:
        CompileResult with .success, .executable, .errors, and .ir_path.
    """
    from jocky.core.context import BuildContext
    from jocky.core.pipeline import Pipeline
    from jocky.core.profile import load_profile
    from jocky.stages.parse import ParseStage
    from jocky.stages.lower_ir import LowerIRStage
    from jocky.stages.mlir_obfuscate import MLIRObfuscateStage
    from jocky.stages.ir_obfuscate import IRObfuscateStage
    from jocky.stages.link import LinkStage
    from jocky.stages.pack import PackStage

    if source is None and file is None:
        return CompileResult(success=False, errors=["Either 'source' or 'file' must be provided"])
    if source is not None and file is not None:
        return CompileResult(success=False, errors=["Provide either 'source' or 'file', not both"])

    if output_dir is None:
        output_dir = Path(tempfile.mkdtemp(prefix="jocky_build_"))

    # Write source string to a temp file if needed
    if source is not None:
        src_file = output_dir / "_source.jky"
        src_file.parent.mkdir(parents=True, exist_ok=True)
        src_file.write_text(source)
        input_file = src_file
    else:
        input_file = Path(file)

    try:
        prof = load_profile(profile)
    except Exception:
        prof = None

    config: Dict[str, Any] = {
        "target": target,
        "no_prelude": no_prelude,
    }
    if extra_config:
        config.update(extra_config)

    ctx = BuildContext(
        input_file=input_file,
        profile_name=profile,
        output_dir=output_dir,
        config=config,
    )
    ctx.state["profile"] = prof

    pipeline = Pipeline([
        ParseStage(),
        LowerIRStage(),
        MLIRObfuscateStage(),
        IRObfuscateStage(),
        LinkStage(),
        PackStage(),
    ])

    try:
        ctx = pipeline.run(ctx)
    except Exception as e:
        ir_path = ctx.state.get("llvm_ir")
        return CompileResult(
            success=False,
            errors=[str(e)],
            ir_path=Path(ir_path) if ir_path else None,
        )

    executable = ctx.state.get("executable")
    ir_path = ctx.state.get("llvm_ir")
    return CompileResult(
        success=True,
        executable=Path(executable) if executable else None,
        ir_path=Path(ir_path) if ir_path else None,
    )
