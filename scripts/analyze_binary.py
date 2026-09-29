#!/usr/bin/env python3
"""Ghidra headless binary analysis for JOCKY-compiled executables.

Runs Ghidra in headless mode to analyze the PE executable and outputs:
  - Function list with sizes
  - Import table (FFI calls)
  - String references
  - Entry point analysis
  - Obfuscation metrics (basic block count, control flow complexity)
"""
import sys
import os
import subprocess
import tempfile
import time
from pathlib import Path


def log(msg):
    ts = time.strftime("%H:%M:%S")
    print(f"[{ts}] [GHIDRA] {msg}", flush=True)


GHIDRA_SCRIPT = '''
// @category JOCKY
// @description Analyze JOCKY-compiled binary

import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.data.*;
import ghidra.program.model.mem.*;
import ghidra.program.model.block.*;
import java.util.*;

public class JockyAnalysis extends GhidraScript {
    @Override
    public void run() throws Exception {
        println("========================================");
        println("JOCKY Binary Analysis Report");
        println("========================================");

        Program program = currentProgram;
        println("Binary: " + program.getName());
        println("Format: " + program.getExecutableFormat());
        println("Architecture: " + program.getLanguage().getProcessor());
        println("Image Base: " + program.getImageBase());
        println("");

        // Functions
        FunctionManager fm = program.getFunctionManager();
        int funcCount = fm.getFunctionCount();
        println("--- Functions (" + funcCount + ") ---");
        FunctionIterator funcs = fm.getFunctions(true);
        int jockyFuncs = 0;
        while (funcs.hasNext()) {
            Function f = funcs.next();
            long size = f.getBody().getNumAddresses();
            String name = f.getName();
            boolean isExternal = f.isExternal();
            if (!isExternal) {
                println("  " + name + " @ " + f.getEntryPoint() + " (" + size + " bytes)");
                if (name.startsWith("jocky_") || name.equals("main") ||
                    name.contains("load_byovd") || name.contains("exfil") ||
                    name.contains("forensic") || name.contains("assess") ||
                    name.contains("encrypt") || name.contains("cleanup")) {
                    jockyFuncs++;
                }
            }
        }
        println("JOCKY-specific functions: " + jockyFuncs);
        println("");

        // Imports
        println("--- Imports ---");
        SymbolTable st = program.getSymbolTable();
        SymbolIterator extSyms = st.getExternalSymbols();
        int importCount = 0;
        while (extSyms.hasNext()) {
            Symbol s = extSyms.next();
            println("  " + s.getName() + " (from " + s.getParentNamespace().getName() + ")");
            importCount++;
        }
        println("Total imports: " + importCount);
        println("");

        // Strings
        println("--- Strings (JOCKY-related) ---");
        DataIterator dataIter = program.getListing().getDefinedData(true);
        int strCount = 0;
        while (dataIter.hasNext()) {
            Data data = dataIter.next();
            if (data.getDataType() instanceof StringDataType ||
                data.getDataType().getName().contains("string")) {
                String val = data.getDefaultValueRepresentation();
                if (val != null && val.length() > 2) {
                    String lower = val.toLowerCase();
                    if (lower.contains("jocky") || lower.contains("byovd") ||
                        lower.contains("driver") || lower.contains("exfil") ||
                        lower.contains("evasion") || lower.contains("research") ||
                        lower.contains("cdn") || lower.contains("dns") ||
                        lower.contains("discord")) {
                        println("  " + data.getAddress() + ": " + val);
                    }
                    strCount++;
                }
            }
        }
        println("Total strings found: " + strCount);
        println("");

        // Basic block analysis
        println("--- Control Flow Complexity ---");
        BasicBlockModel bbModel = new BasicBlockModel(program);
        funcs = fm.getFunctions(true);
        int totalBlocks = 0;
        int maxBlocks = 0;
        String maxFunc = "";
        while (funcs.hasNext()) {
            Function f = funcs.next();
            if (f.isExternal()) continue;
            CodeBlockIterator blocks = bbModel.getCodeBlocksContaining(f.getBody(), monitor);
            int blockCount = 0;
            while (blocks.hasNext()) {
                blocks.next();
                blockCount++;
            }
            totalBlocks += blockCount;
            if (blockCount > maxBlocks) {
                maxBlocks = blockCount;
                maxFunc = f.getName();
            }
        }
        println("Total basic blocks: " + totalBlocks);
        println("Most complex function: " + maxFunc + " (" + maxBlocks + " blocks)");
        println("Average blocks/function: " + (funcCount > 0 ? totalBlocks / funcCount : 0));
        println("");

        // Entry point
        println("--- Entry Point ---");
        Function entry = fm.getFunctionAt(program.getImageBase().add(
            program.getSymbolTable().getLabelHistory().length > 0 ? 0 : 0));
        Function mainFunc = null;
        funcs = fm.getFunctions(true);
        while (funcs.hasNext()) {
            Function f = funcs.next();
            if (f.getName().equals("main")) {
                mainFunc = f;
                break;
            }
        }
        if (mainFunc != null) {
            println("main() found at: " + mainFunc.getEntryPoint());
            println("main() size: " + mainFunc.getBody().getNumAddresses() + " bytes");
        }

        println("");
        println("========================================");
        println("Analysis complete.");
        println("========================================");
    }
}
'''


def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <binary.exe> [ghidra_path]")
        sys.exit(1)

    binary = Path(sys.argv[1])
    ghidra_path = Path(sys.argv[2]) if len(sys.argv) > 2 else Path("/opt/ghidra")

    if not binary.exists():
        log(f"Binary not found: {binary}")
        sys.exit(1)

    headless = ghidra_path / "support" / "analyzeHeadless"
    if not headless.exists():
        log(f"Ghidra headless not found at {headless}")
        log("Falling back to objdump analysis")
        fallback_analysis(binary)
        return

    log(f"Analyzing: {binary}")
    log(f"Size: {binary.stat().st_size} bytes")

    with tempfile.TemporaryDirectory() as tmpdir:
        tmpdir = Path(tmpdir)
        script_path = tmpdir / "JockyAnalysis.java"
        script_path.write_text(GHIDRA_SCRIPT)
        project_dir = tmpdir / "project"
        project_dir.mkdir()

        cmd = [
            str(headless),
            str(project_dir), "JockyAnalysis",
            "-import", str(binary),
            "-postScript", str(script_path),
            "-scriptPath", str(tmpdir),
            "-deleteProject",
        ]
        log("Running Ghidra headless analysis...")
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=300)

        for line in result.stdout.split("\n"):
            if line.strip() and not line.startswith("INFO"):
                print(line)

        if result.returncode != 0:
            log("Ghidra analysis had errors, showing stderr")
            for line in result.stderr.split("\n")[-20:]:
                if line.strip():
                    log(f"  {line}")


def fallback_analysis(binary):
    """Basic analysis using objdump when Ghidra is not available."""
    log("Running objdump-based analysis")
    print()

    result = subprocess.run(["file", str(binary)], capture_output=True, text=True)
    print(f"File type: {result.stdout.strip()}")
    print()

    result = subprocess.run(
        ["x86_64-w64-mingw32-objdump", "-f", str(binary)],
        capture_output=True, text=True
    )
    if result.returncode == 0:
        print("--- Binary Header ---")
        print(result.stdout)

    result = subprocess.run(
        ["x86_64-w64-mingw32-objdump", "-t", str(binary)],
        capture_output=True, text=True
    )
    if result.returncode == 0:
        lines = result.stdout.strip().split("\n")
        funcs = [l for l in lines if " F " in l or " f " in l]
        print(f"--- Symbols ({len(funcs)} functions) ---")
        for f in funcs[:50]:
            print(f"  {f}")
        if len(funcs) > 50:
            print(f"  ... and {len(funcs) - 50} more")
        print()

    result = subprocess.run(
        ["x86_64-w64-mingw32-objdump", "-p", str(binary)],
        capture_output=True, text=True
    )
    if result.returncode == 0:
        in_imports = False
        print("--- PE Import Table ---")
        for line in result.stdout.split("\n"):
            if "DLL Name:" in line or "Import" in line:
                in_imports = True
            if in_imports and line.strip():
                print(f"  {line.strip()}")

    result = subprocess.run(
        ["strings", "-n", "8", str(binary)],
        capture_output=True, text=True
    )
    if result.returncode == 0:
        all_strings = result.stdout.strip().split("\n")
        print(f"\n--- Strings ({len(all_strings)} total, showing relevant) ---")
        for s in all_strings:
            sl = s.lower()
            if any(k in sl for k in ["jocky", "byovd", "driver", "exfil", "evasion",
                                      "research", "cdn", "dns", "discord", "chain",
                                      "audit", "cleanup", "forensic", "kernel"]):
                print(f"  {s}")


if __name__ == "__main__":
    main()
