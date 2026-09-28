#!/bin/bash
# JOCKY ML Model Setup Script
# Downloads and configures Phi-3-mini quantized model for threat assessment
# Authorized: Red Hat + IIT Bombay Cyber Security Team

set -e

MODEL_DIR="/opt/models"
MODEL_NAME="phi3_evasion"
MODEL_FILE="phi3_evasion.gguf"
MODEL_URL="https://huggingface.co/microsoft/Phi-3-mini-4k-instruct-gguf/resolve/main/Phi-3-mini-4k-instruct-Q4_K_M.gguf"

echo "======================================================================"
echo "JOCKY ML Model Setup - Phi-3-mini Quantized"
echo "======================================================================"
echo ""

# Step 1: Create model directory
echo "[*] Creating model directory..."
mkdir -p "$MODEL_DIR"
chmod 755 "$MODEL_DIR"
echo "[+] Directory: $MODEL_DIR"
echo ""

# Step 2: Download model
echo "[*] Downloading Phi-3-mini (4-bit quantized)..."
echo "    This is ~2.4GB, may take a few minutes..."
echo ""

if command -v wget &> /dev/null; then
    wget -c "$MODEL_URL" -O "$MODEL_DIR/$MODEL_FILE" \
        --no-check-certificate \
        --show-progress
elif command -v curl &> /dev/null; then
    curl -L -C - "$MODEL_URL" -o "$MODEL_DIR/$MODEL_FILE" \
        --progress-bar
else
    echo "[!] ERROR: Neither wget nor curl found"
    exit 1
fi

echo ""
echo "[+] Model downloaded"
echo ""

# Step 3: Verify download
echo "[*] Verifying download..."
MODEL_SIZE=$(du -h "$MODEL_DIR/$MODEL_FILE" | cut -f1)
echo "    File size: $MODEL_SIZE"

# Calculate and verify checksum (if available)
if [ -f "$MODEL_DIR/$MODEL_FILE.sha256" ]; then
    sha256sum -c "$MODEL_DIR/$MODEL_FILE.sha256"
    echo "[+] Checksum verified"
else
    echo "[*] No checksum file available (continuing anyway)"
fi
echo ""

# Step 4: Create model config
echo "[*] Creating model configuration..."
cat > "$MODEL_DIR/config.json" << 'EOF'
{
  "model_name": "phi-3-mini-4k-instruct-gguf",
  "model_file": "phi3_evasion.gguf",
  "size_mb": 2400,
  "quantization": "Q4_K_M",
  "context_length": 4096,
  "max_batch_size": 4,
  "inference_timeout_ms": 100,
  "purpose": "Threat assessment and evasion strategy selection",
  "capabilities": [
    "threat_scoring",
    "risk_classification",
    "strategy_recommendation",
    "anomaly_detection"
  ],
  "parameters": {
    "temperature": 0.3,
    "top_p": 0.9,
    "top_k": 40,
    "repeat_penalty": 1.1
  }
}
EOF

echo "[+] Config created: $MODEL_DIR/config.json"
echo ""

# Step 5: Create training data template
echo "[*] Creating training data template..."
cat > "$MODEL_DIR/training_data.json" << 'EOF'
{
  "threat_patterns": [
    {
      "telemetry": {
        "syscall_frequency": 2500,
        "network_entropy": 7.8,
        "memory_pattern_score": 0.92,
        "file_io_score": 0.78,
        "blocked_operations": 15,
        "alert_count": 8,
        "crash_likelihood": 0.85
      },
      "threat_level": "CRITICAL",
      "recommended_strategy": "AI_ADAPTIVE",
      "confidence": 0.95,
      "explanation": "High syscall frequency + network anomaly + multiple blocks = Active EDR detection"
    },
    {
      "telemetry": {
        "syscall_frequency": 450,
        "network_entropy": 4.2,
        "memory_pattern_score": 0.45,
        "file_io_score": 0.35,
        "blocked_operations": 3,
        "alert_count": 2,
        "crash_likelihood": 0.15
      },
      "threat_level": "HIGH",
      "recommended_strategy": "AGGRESSIVE",
      "confidence": 0.87,
      "explanation": "Elevated syscall activity + some blocking detected = EDR actively monitoring"
    },
    {
      "telemetry": {
        "syscall_frequency": 120,
        "network_entropy": 3.1,
        "memory_pattern_score": 0.22,
        "file_io_score": 0.18,
        "blocked_operations": 0,
        "alert_count": 0,
        "crash_likelihood": 0.05
      },
      "threat_level": "MEDIUM",
      "recommended_strategy": "HYBRID",
      "confidence": 0.79,
      "explanation": "Normal activity levels, some OS monitoring possible"
    },
    {
      "telemetry": {
        "syscall_frequency": 45,
        "network_entropy": 2.1,
        "memory_pattern_score": 0.12,
        "file_io_score": 0.05,
        "blocked_operations": 0,
        "alert_count": 0,
        "crash_likelihood": 0.02
      },
      "threat_level": "LOW",
      "recommended_strategy": "STEALTH",
      "confidence": 0.92,
      "explanation": "Minimal activity, baseline system - stealth approach sufficient"
    }
  ]
}
EOF

echo "[+] Training data template created: $MODEL_DIR/training_data.json"
echo ""

# Step 6: Create inference script
echo "[*] Creating inference wrapper script..."
cat > "$MODEL_DIR/run_inference.py" << 'EOF'
#!/usr/bin/env python3
"""
JOCKY ML Inference Wrapper
Loads Phi-3-mini and runs threat assessment
"""

import json
import sys
from pathlib import Path

try:
    from ctransformers import AutoModelForCausalLM
except ImportError:
    print("[!] ERROR: ctransformers not installed")
    print("    Install with: pip install ctransformers")
    sys.exit(1)

def load_model(model_path):
    """Load quantized model"""
    print(f"[*] Loading model: {model_path}")

    model = AutoModelForCausalLM.from_pretrained(
        model_path,
        model_type="phi",
        gpu_layers=10,  # Use 10 GPU layers if available
        temperature=0.3,
        top_p=0.9,
        top_k=40,
        repeat_penalty=1.1,
    )

    print("[+] Model loaded successfully")
    return model

def assess_threat(model, telemetry):
    """Run threat assessment on telemetry"""

    prompt = f"""Given this system telemetry, assess the threat level and recommend an evasion strategy.

Telemetry:
- Syscall frequency: {telemetry.get('syscall_frequency', 0)}/sec
- Network entropy: {telemetry.get('network_entropy', 0):.1f}
- Memory pattern score: {telemetry.get('memory_pattern_score', 0):.2f}
- File I/O score: {telemetry.get('file_io_score', 0):.2f}
- Blocked operations: {telemetry.get('blocked_operations', 0)}
- EDR alerts: {telemetry.get('alert_count', 0)}
- Crash likelihood: {telemetry.get('crash_likelihood', 0):.2f}

Response format:
THREAT_LEVEL: [LOW|MEDIUM|HIGH|CRITICAL]
STRATEGY: [STEALTH|HYBRID|AGGRESSIVE|AI_ADAPTIVE]
CONFIDENCE: [0.0-1.0]
REASONING: [brief explanation]
"""

    result = model(prompt, max_new_tokens=200)
    return result

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: run_inference.py <model_path> [telemetry_json]")
        sys.exit(1)

    model_path = sys.argv[1]

    model = load_model(model_path)

    # Example telemetry
    telemetry = {
        "syscall_frequency": 500,
        "network_entropy": 5.2,
        "memory_pattern_score": 0.55,
        "file_io_score": 0.45,
        "blocked_operations": 5,
        "alert_count": 2,
        "crash_likelihood": 0.25
    }

    print("\n[*] Running threat assessment...")
    result = assess_threat(model, telemetry)
    print(f"\n[+] Model output:\n{result}")
EOF

chmod +x "$MODEL_DIR/run_inference.py"
echo "[+] Inference script created: $MODEL_DIR/run_inference.py"
echo ""

# Step 7: Create JOCKY integration header
echo "[*] Creating JOCKY integration header..."
cat > "$MODEL_DIR/jocky_ml_config.h" << 'EOF'
#ifndef JOCKY_ML_CONFIG_H
#define JOCKY_ML_CONFIG_H

/* JOCKY ML Model Configuration
 * Phi-3-mini-4k-instruct (Q4_K_M quantization)
 * 2.4GB, ~100ms inference time
 */

#define JOCKY_ML_MODEL_PATH "/opt/models/phi3_evasion.gguf"
#define JOCKY_ML_CONFIG_PATH "/opt/models/config.json"
#define JOCKY_ML_MAX_INFERENCE_MS 100
#define JOCKY_ML_CONTEXT_LENGTH 4096
#define JOCKY_ML_MODEL_SIZE_MB 2400

typedef enum {
    JOCKY_ML_THREAT_LOW = 0,
    JOCKY_ML_THREAT_MEDIUM = 1,
    JOCKY_ML_THREAT_HIGH = 2,
    JOCKY_ML_THREAT_CRITICAL = 3,
} JOCKY_ML_THREAT_LEVEL;

typedef struct {
    float threat_score;
    JOCKY_ML_THREAT_LEVEL threat_level;
    uint32_t recommended_strategy;
    float confidence;
    char reasoning[256];
} JOCKY_ML_ASSESSMENT;

/* Initialize ML model */
int jocky_ml_init(void);

/* Run threat assessment on telemetry */
int jocky_ml_assess_threat(
    const void* telemetry_data,
    JOCKY_ML_ASSESSMENT* out_assessment);

/* Shutdown ML model */
int jocky_ml_shutdown(void);

#endif
EOF

echo "[+] JOCKY integration header created: $MODEL_DIR/jocky_ml_config.h"
echo ""

# Step 8: Dependencies
echo "[*] ML Model Dependencies"
echo "    Python packages required:"
echo "    - ctransformers (GGUF model inference)"
echo "    - torch (optional, for GPU acceleration)"
echo ""
echo "    Install with:"
echo "    pip install ctransformers torch"
echo ""

# Step 9: Verification
echo "[*] Verifying setup..."
echo ""

if [ -f "$MODEL_DIR/$MODEL_FILE" ]; then
    echo "[+] Model file exists"
    echo "    Path: $MODEL_DIR/$MODEL_FILE"
    echo "    Size: $(du -h "$MODEL_DIR/$MODEL_FILE" | cut -f1)"
else
    echo "[!] Model file not found"
    exit 1
fi

if [ -f "$MODEL_DIR/config.json" ]; then
    echo "[+] Config file exists"
fi

if [ -f "$MODEL_DIR/training_data.json" ]; then
    echo "[+] Training data template exists"
fi

if [ -x "$MODEL_DIR/run_inference.py" ]; then
    echo "[+] Inference script is executable"
fi

echo ""
echo "======================================================================"
echo "ML Model Setup Complete"
echo "======================================================================"
echo ""
echo "Next steps:"
echo "1. Install Python dependencies:"
echo "   pip install ctransformers torch"
echo ""
echo "2. Test model inference:"
echo "   python3 $MODEL_DIR/run_inference.py $MODEL_DIR/$MODEL_FILE"
echo ""
echo "3. Integrate into JOCKY:"
echo "   - Include $MODEL_DIR/jocky_ml_config.h in build"
echo "   - Link with ctransformers library"
echo "   - Call jocky_ml_init() during startup"
echo ""
echo "Model ready for JOCKY research chain v2"
echo ""
