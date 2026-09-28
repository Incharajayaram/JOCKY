#!/bin/bash
# Build script for JOCKY Defensive Forensic Engine

set -e

echo "=== Building JOCKY Defensive Forensic Engine ==="

# Source files (removed forensic_plugins.c - merged into forensic_types.h)
SRCS=(
    "src/runtime/forensics/forensic_utils.c"
    "src/runtime/forensics/plugins/collectors/process_collector.c"
    "src/runtime/forensics/plugins/collectors/file_collector.c"
    "src/runtime/forensics/plugins/collectors/network_collector.c"
    "src/runtime/forensics/plugins/collectors/registry_collector.c"
    "src/runtime/forensics/plugins/collectors/memory_collector.c"
    "src/runtime/forensics/plugins/collectors/evtx_collector.c"
    "src/runtime/forensics/plugins/collectors/prefetch_collector.c"
    "src/runtime/forensics/plugins/collectors/mft_collector.c"
    "src/runtime/forensics/plugins/collectors/usnjrnl_collector.c"
    "src/runtime/forensics/plugins/parsers/pe_parser.c"
    "src/runtime/forensics/plugins/parsers/registry_parser.c"
    "src/runtime/forensics/plugins/parsers/evtx_parser.c"
    "src/runtime/forensics/plugins/parsers/prefetch_parser.c"
    "src/runtime/forensics/plugins/parsers/mft_parser.c"
    "src/runtime/forensics/plugins/parsers/network_parser.c"
    "src/runtime/forensics/plugins/parsers/process_parser.c"
    "src/runtime/forensics/plugins/parsers/file_parser.c"
    "src/runtime/forensics/plugins/parsers/dns_parser.c"
    "src/runtime/forensics/plugins/parsers/arp_parser.c"
    "src/runtime/forensics/plugins/parsers/memory_parser.c"
    "src/runtime/forensics/plugins/parsers/elf_parser.c"
    "src/runtime/forensics/plugins/output/json_output.c"
    "src/runtime/forensics/plugins/output/stix_output.c"
    "src/runtime/forensics/plugins/output/sigma_output.c"
    "src/runtime/forensics/plugins/output/html_output.c"
    "src/runtime/forensics/plugins/output/siem_forwarder.c"
    "src/runtime/forensics/engine/provenance.c"
    "src/runtime/forensics/engine/analysis.c"
    "src/runtime/forensics/audit/audit_log.c"
    "src/runtime/forensics/store/evidence_store.c"
    "src/runtime/forensics/control/capabilities.c"
    "src/runtime/forensics/forensic_engine.c"
    "test_forensic_engine.c"
)

INCLUDES="-Isrc/runtime/forensics"

mkdir -p build_forensics

echo "Compiling..."
gcc -std=c99 -O2 -g \
    $INCLUDES \
    "${SRCS[@]}" \
    -o build_forensics/forensic_test \
    -lpthread -lm -lz -lssl -lcrypto

if [ $? -eq 0 ]; then
    echo "Build successful: build_forensics/forensic_test"
else
    echo "Build failed"
    exit 1
fi