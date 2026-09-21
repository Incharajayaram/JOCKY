import subprocess
import sys
from pathlib import Path

JOCKY_DIR = Path(__file__).parent.parent.parent
EXAMPLES_DIR = JOCKY_DIR / "examples" / "hello-world"

def test_hello_world_build_and_run():
    main_jky = EXAMPLES_DIR / "main.jky"
    assert main_jky.exists()

    # Clean previous build
    build_dir = EXAMPLES_DIR / ".jocky-build"
    if build_dir.exists():
        import shutil
        shutil.rmtree(build_dir)

    result = subprocess.run(
        [sys.executable, "-m", "jocky.cli", "build", str(main_jky), "--profile", "none"],
        cwd=JOCKY_DIR,
        capture_output=True,
        text=True,
        env={"PYTHONPATH": str(JOCKY_DIR / "src")},
    )
    print(result.stdout)
    print(result.stderr, file=sys.stderr)
    assert result.returncode == 0, f"Build failed: {result.stderr}"

    exe = build_dir / "main"
    assert exe.exists(), f"Executable not found at {exe}"

    run_result = subprocess.run([str(exe)], capture_output=True, text=True)
    assert run_result.returncode == 0
    assert "Hello, JOCKY!" in run_result.stdout
