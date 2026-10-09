#!/bin/bash

# Telemetry API Test Suite
# Tests all telemetry endpoints with realistic scenarios

set -e

BASE_URL="http://localhost:8000"
CHAIN_ID="chain_$(date +%s)"

echo "=========================================="
echo "JOCKY C2 Telemetry API Test Suite"
echo "=========================================="
echo "Chain ID: $CHAIN_ID"
echo ""

# Test 1: Report phase start event
echo "[1/5] Reporting phase start event..."
curl -s -X POST "$BASE_URL/api/telemetry/report" \
  -H "Content-Type: application/json" \
  -d "{
    \"timestamp\": \"$(date -u +%Y-%m-%dT%H:%M:%SZ)\",
    \"chain_id\": \"$CHAIN_ID\",
    \"event_type\": \"phase_start\",
    \"phase\": 1,
    \"status\": \"running\",
    \"details\": {\"module\": \"reconnaissance\", \"target\": \"192.168.1.0/24\"}
  }" | python3 -m json.tool
echo ""

# Simulate some delay
sleep 1

# Test 2: Report data collected event
echo "[2/5] Reporting data collected event..."
curl -s -X POST "$BASE_URL/api/telemetry/report" \
  -H "Content-Type: application/json" \
  -d "{
    \"timestamp\": \"$(date -u +%Y-%m-%dT%H:%M:%SZ)\",
    \"chain_id\": \"$CHAIN_ID\",
    \"event_type\": \"data_collected\",
    \"phase\": 1,
    \"status\": \"success\",
    \"details\": {\"bytes\": 5242880, \"source\": \"C:\\\\Users\\\\Admin\\\\.ssh\", \"files\": 42}
  }" | python3 -m json.tool
echo ""

sleep 1

# Test 3: Report driver attempt
echo "[3/5] Reporting driver attempt event..."
curl -s -X POST "$BASE_URL/api/telemetry/report" \
  -H "Content-Type: application/json" \
  -d "{
    \"timestamp\": \"$(date -u +%Y-%m-%dT%H:%M:%SZ)\",
    \"chain_id\": \"$CHAIN_ID\",
    \"event_type\": \"driver_attempt\",
    \"phase\": 2,
    \"status\": \"running\",
    \"details\": {\"driver\": \"ntfs_driver.sys\", \"method\": \"signature_bypass\"}
  }" | python3 -m json.tool
echo ""

sleep 1

# Test 4: Get chain timeline
echo "[4/5] Getting chain timeline..."
curl -s -X GET "$BASE_URL/api/telemetry/chain/$CHAIN_ID?limit=50" | python3 -m json.tool
echo ""

# Test 5: Get chain summary
echo "[5/5] Getting chain summary statistics..."
curl -s -X GET "$BASE_URL/api/telemetry/summary?chain_id=$CHAIN_ID" | python3 -m json.tool
echo ""

echo "=========================================="
echo "Test suite complete!"
echo "=========================================="
echo ""
echo "Example WebSocket subscription:"
echo "  wscat -c ws://localhost:8000/ws/telemetry/$CHAIN_ID"
echo ""
echo "To submit more events from agent:"
echo "  curl -X POST http://localhost:8000/api/telemetry/report \\"
echo "    -H 'Content-Type: application/json' \\"
echo "    -d '{\"timestamp\": \"...\", \"chain_id\": \"$CHAIN_ID\", \"event_type\": \"...\", ...}'"
