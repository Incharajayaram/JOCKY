#!/usr/bin/env python3
"""
JOCKY Lightweight AI Model Generator

Generates a compact quantized decision tree model for threat assessment.
Model is designed to be embedded in the executable for offline inference.

Output: Binary model file suitable for jocky_ai_init()
"""

import struct
import sys
from pathlib import Path

# Model magic signature
JOCKY_AI_MODEL_MAGIC = 0x4A4F434B  # "JOCK"
JOCKY_AI_MODEL_VERSION = 1

class JockyAIModel:
    def __init__(self):
        self.magic = JOCKY_AI_MODEL_MAGIC
        self.version = JOCKY_AI_MODEL_VERSION
        self.input_size = 10  # 10 telemetry features
        self.output_size = 4  # 4 risk levels
        self.model_type = 0   # Decision tree
        self.quantization_bits = 8  # 8-bit quantization

        # Simplified decision tree weights (quantized to uint8)
        # These represent decision thresholds and branch weights
        self.weights = self._generate_decision_tree_weights()
        self.bias = self._generate_bias()

    def _generate_decision_tree_weights(self):
        """Generate quantized decision tree weights"""
        weights = bytearray()

        # Decision tree nodes (simplified representation)
        # Node 0: Check blocked_operations threshold
        weights.append(8)      # Threshold: 8 blocked ops
        weights.append(255)    # High criticality weight
        weights.append(200)    # Medium weight

        # Node 1: Check syscall frequency
        weights.append(100)    # Threshold: 100 syscalls/sec
        weights.append(180)    # Frequency weight

        # Node 2: Check alert count
        weights.append(4)      # Threshold: 4 alerts
        weights.append(200)    # Alert weight

        # Node 3: Check network entropy
        weights.append(50)     # Threshold: 5.0 entropy
        weights.append(150)    # Network weight

        # Node 4: Check memory pattern score
        weights.append(128)    # Threshold: 0.5 (scaled to 128)
        weights.append(100)    # Memory weight

        # Node 5: Check file I/O score
        weights.append(75)     # Threshold: 0.3 (scaled)
        weights.append(80)     # File I/O weight

        # Node 6: Check crash likelihood
        weights.append(100)    # Threshold: 0.4 (scaled)
        weights.append(90)     # Crash weight

        # Node 7: Check registry operations
        weights.append(50)     # Threshold: 0.2 (scaled)
        weights.append(60)     # Registry weight

        # Final leaf nodes (risk classification)
        # LOW risk (sum < 50)
        weights.append(0)      # Risk level: LOW

        # MEDIUM risk (50-120)
        weights.append(1)      # Risk level: MEDIUM

        # HIGH risk (120-200)
        weights.append(2)      # Risk level: HIGH

        # CRITICAL risk (200+)
        weights.append(3)      # Risk level: CRITICAL

        return bytes(weights)

    def _generate_bias(self):
        """Generate bias terms (thresholds)"""
        bias = bytearray()

        # Bias for each decision node (affects threshold sensitivity)
        for _ in range(8):
            bias.append(5)  # Small bias for stability

        # Risk level biases
        bias.append(0)      # LOW bias
        bias.append(0)      # MEDIUM bias
        bias.append(10)     # HIGH bias (more conservative)
        bias.append(20)     # CRITICAL bias (most conservative)

        return bytes(bias)

    def serialize(self, output_path):
        """Serialize model to binary file"""
        with open(output_path, 'wb') as f:
            # Header
            f.write(struct.pack('<I', self.magic))
            f.write(struct.pack('<I', self.version))
            f.write(struct.pack('<I', self.input_size))
            f.write(struct.pack('<I', self.output_size))
            f.write(struct.pack('<I', len(self.weights)))
            f.write(struct.pack('<I', len(self.bias)))
            f.write(struct.pack('<B', self.model_type))
            f.write(struct.pack('<B', self.quantization_bits))
            f.write(struct.pack('<H', 0))  # Reserved

            # Weights
            f.write(self.weights)

            # Bias
            f.write(self.bias)

        print(f"[+] Model serialized: {output_path}")
        print(f"    Magic: 0x{self.magic:08x}")
        print(f"    Version: {self.version}")
        print(f"    Input features: {self.input_size}")
        print(f"    Output classes: {self.output_size}")
        print(f"    Weights: {len(self.weights)} bytes")
        print(f"    Bias: {len(self.bias)} bytes")
        print(f"    Model type: Decision Tree")
        print(f"    Quantization: {self.quantization_bits}-bit")

        total_size = (
            4 + 4 + 4 + 4 + 4 + 4 + 1 + 1 + 2 +  # Header
            len(self.weights) + len(self.bias)
        )
        print(f"    Total size: {total_size} bytes")

    def generate_c_header(self, output_path):
        """Generate C header with embedded model data"""
        with open(output_path, 'w') as f:
            f.write("/* Auto-generated JOCKY AI Model */\n")
            f.write("#ifndef JOCKY_AI_MODEL_DATA_H\n")
            f.write("#define JOCKY_AI_MODEL_DATA_H\n\n")
            f.write("#include <stdint.h>\n\n")
            f.write("/* Embedded AI threat assessment model */\n")
            f.write("static const uint8_t jocky_ai_embedded_model[] = {\n")

            # Serialize inline
            data = bytearray()
            data.extend(struct.pack('<I', self.magic))
            data.extend(struct.pack('<I', self.version))
            data.extend(struct.pack('<I', self.input_size))
            data.extend(struct.pack('<I', self.output_size))
            data.extend(struct.pack('<I', len(self.weights)))
            data.extend(struct.pack('<I', len(self.bias)))
            data.extend(struct.pack('<B', self.model_type))
            data.extend(struct.pack('<B', self.quantization_bits))
            data.extend(struct.pack('<H', 0))
            data.extend(self.weights)
            data.extend(self.bias)

            # Write as hex values
            for i, byte in enumerate(data):
                if i % 16 == 0:
                    f.write("    ")
                f.write(f"0x{byte:02x}")
                if i < len(data) - 1:
                    f.write(", ")
                if (i + 1) % 16 == 0:
                    f.write("\n")

            f.write("\n};\n\n")
            f.write(f"static const size_t jocky_ai_embedded_model_size = {len(data)};\n\n")
            f.write("#endif /* JOCKY_AI_MODEL_DATA_H */\n")

        print(f"[+] C header generated: {output_path}")

def main():
    output_dir = Path(__file__).parent.parent / "models"
    output_dir.mkdir(exist_ok=True)

    print("=" * 70)
    print("JOCKY Lightweight AI Model Generator")
    print("=" * 70)
    print()

    # Generate model
    model = JockyAIModel()

    # Save binary model
    model_path = output_dir / "jocky_ai_model.bin"
    model.serialize(str(model_path))
    print()

    # Generate C header for embedding
    header_path = output_dir / "jocky_ai_model.h"
    model.generate_c_header(str(header_path))
    print()

    print("[+] Model files ready for integration")
    print(f"    Binary: {model_path}")
    print(f"    Header: {header_path}")
    print()
    print("Usage:")
    print("  1. Load from file: jocky_ai_load_model_file(\"/opt/models/jocky_ai_model.bin\");")
    print("  2. Embed in code: Include jocky_ai_model.h and use jocky_ai_load_model_buffer()")
    print()

if __name__ == "__main__":
    main()
