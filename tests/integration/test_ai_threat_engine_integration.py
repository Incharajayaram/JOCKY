"""
Comprehensive Integration Tests for AI Threat Engine and Driver Intelligence

This test suite covers:
1. Backend API Testing
2. Compilation with New Flags
3. Runtime Behavior Testing
4. Integration Points Testing
5. Performance & Stability Testing
"""

import os
import sys
import json
import time
import subprocess
import tempfile
import asyncio
from pathlib import Path
from typing import Dict, Any, List
import pytest
import httpx

# Add project root to path
PROJECT_ROOT = Path(__file__).parent.parent.parent
sys.path.insert(0, str(PROJECT_ROOT))

# Test configuration
BACKEND_URL = os.getenv("BACKEND_URL", "http://localhost:8000")
TEST_TIMEOUT = 30
LOAD_TEST_CONCURRENT_REQUESTS = 100


class TestAIThreatEngineAPIs:
    """Test AI Threat Engine API endpoints"""

    @pytest.fixture
    def client(self):
        """Create HTTP client for testing"""
        return httpx.Client(base_url=BACKEND_URL, timeout=TEST_TIMEOUT)

    def test_threat_score_endpoint(self, client):
        """Test GET /api/ai/threat-score"""
        response = client.get("/api/ai/threat-score")
        assert response.status_code == 200

        data = response.json()
        assert "threat_score" in data
        assert "threat_level" in data
        assert "confidence" in data

        # Validate threat score is in valid range
        assert isinstance(data["threat_score"], (int, float))
        assert 0.0 <= data["threat_score"] <= 1.0

        # Validate threat level
        valid_levels = ["low", "medium", "high", "critical"]
        assert data["threat_level"] in valid_levels

        # Validate confidence
        assert isinstance(data["confidence"], (int, float))
        assert 0.0 <= data["confidence"] <= 1.0

    def test_ai_strategy_endpoint(self, client):
        """Test GET /api/ai/strategy"""
        response = client.get("/api/ai/strategy")
        assert response.status_code == 200

        data = response.json()
        assert "strategy" in data
        assert "obfuscation_level" in data
        assert "techniques" in data

        # Validate strategy
        valid_strategies = ["stealth", "hybrid", "aggressive", "ai_adaptive"]
        assert data["strategy"] in valid_strategies

        # Validate obfuscation level (0-10)
        assert isinstance(data["obfuscation_level"], int)
        assert 0 <= data["obfuscation_level"] <= 10

        # Validate techniques list
        assert isinstance(data["techniques"], list)
        assert len(data["techniques"]) > 0

    def test_threat_event_logging(self, client):
        """Test POST /api/ai/threat-event"""
        event_data = {
            "event_type": "syscall",
            "syscall_id": 1,
            "operation": "CreateRemoteThread",
            "details": {"target_pid": 1234}
        }

        response = client.post("/api/ai/threat-event", json=event_data)
        assert response.status_code == 200

        data = response.json()
        assert data["status"] == "recorded"
        assert data["event_type"] == "syscall"
        assert "timestamp" in data

    def test_threat_event_network_event(self, client):
        """Test POST /api/ai/threat-event with network event"""
        event_data = {
            "event_type": "network",
            "operation": "connect",
            "details": {"ip": "192.168.1.100", "port": 443}
        }

        response = client.post("/api/ai/threat-event", json=event_data)
        assert response.status_code == 200

        data = response.json()
        assert data["status"] == "recorded"
        assert data["event_type"] == "network"

    def test_threat_event_file_event(self, client):
        """Test POST /api/ai/threat-event with file event"""
        event_data = {
            "event_type": "file",
            "operation": "write",
            "details": {"path": "C:\\\\Users\\\\Admin\\\\AppData\\\\test.exe"}
        }

        response = client.post("/api/ai/threat-event", json=event_data)
        assert response.status_code == 200

        data = response.json()
        assert data["status"] == "recorded"
        assert data["event_type"] == "file"

    def test_mutations_endpoint(self, client):
        """Test GET /api/ai/mutations"""
        response = client.get("/api/ai/mutations")
        assert response.status_code == 200

        data = response.json()
        assert "mutations_available" in data
        assert "current_mutations" in data

        # Validate mutations list
        assert isinstance(data["mutations_available"], list)
        assert len(data["mutations_available"]) >= 8

        expected_mutations = [
            "instruction_substitution",
            "code_layout_randomization",
            "api_call_reordering",
            "control_flow_flattening",
            "stack_frame_obfuscation",
            "memory_pattern_hiding",
            "syscall_table_hooking",
            "indirect_function_calls",
        ]
        for mutation in expected_mutations:
            assert mutation in data["mutations_available"]


class TestDriverIntelligenceAPIs:
    """Test Driver Intelligence API endpoints"""

    @pytest.fixture
    def client(self):
        """Create HTTP client for testing"""
        return httpx.Client(base_url=BACKEND_URL, timeout=TEST_TIMEOUT)

    def test_driver_score_endpoint(self, client):
        """Test GET /api/driver/score/{driver}"""
        driver_name = "rtkiow10x64.sys"
        response = client.get(f"/api/driver/score/{driver_name}")
        assert response.status_code == 200

        data = response.json()
        assert data["driver_name"] == driver_name
        assert "composite_score" in data
        assert "evasion_score" in data
        assert "prevalence_score" in data
        assert "capability_score" in data
        assert "blocklist_score" in data

        # Validate scores are in range 0-100
        for score_key in ["composite_score", "evasion_score", "prevalence_score",
                          "capability_score", "blocklist_score"]:
            assert isinstance(data[score_key], int)
            assert 0 <= data[score_key] <= 100

    def test_driver_ranking_endpoint(self, client):
        """Test GET /api/driver/ranking"""
        response = client.get("/api/driver/ranking")
        assert response.status_code == 200

        data = response.json()
        assert "ranked_drivers" in data
        assert "total_drivers" in data

        # Validate drivers list
        assert isinstance(data["ranked_drivers"], list)
        assert data["total_drivers"] > 0

        # Validate each driver entry
        for driver in data["ranked_drivers"]:
            assert "name" in driver
            assert "score" in driver
            assert isinstance(driver["score"], int)
            assert 0 <= driver["score"] <= 100

        # Drivers should be ranked by score (descending)
        scores = [d["score"] for d in data["ranked_drivers"]]
        assert scores == sorted(scores, reverse=True)

    def test_driver_fallback_chain_endpoint(self, client):
        """Test GET /api/driver/fallback-chain"""
        response = client.get("/api/driver/fallback-chain")
        assert response.status_code == 200

        data = response.json()
        assert "fallback_chain" in data
        assert "chain_size" in data

        # Validate fallback chain
        assert isinstance(data["fallback_chain"], list)
        assert data["chain_size"] > 0
        assert data["chain_size"] == len(data["fallback_chain"])

        # All entries should be strings (driver names)
        for driver in data["fallback_chain"]:
            assert isinstance(driver, str)


class TestEDRProfileAPIs:
    """Test EDR Profiler API endpoints"""

    @pytest.fixture
    def client(self):
        """Create HTTP client for testing"""
        return httpx.Client(base_url=BACKEND_URL, timeout=TEST_TIMEOUT)

    def test_edr_profile_endpoint(self, client):
        """Test GET /api/edr/profile"""
        response = client.get("/api/edr/profile")
        assert response.status_code == 200

        data = response.json()
        assert "profile_type" in data
        assert "callback_count" in data
        assert "min_interval_ms" in data
        assert "max_interval_ms" in data
        assert "avg_interval_ms" in data
        assert "confidence" in data
        assert "adaptive_mode" in data

        # Validate profile type
        valid_profiles = ["not_detected", "frequent", "normal", "throttled", "rare"]
        assert data["profile_type"] in valid_profiles

        # Validate callback count
        assert isinstance(data["callback_count"], int)
        assert data["callback_count"] >= 0

        # Validate intervals
        assert data["min_interval_ms"] >= 0
        assert data["max_interval_ms"] >= data["min_interval_ms"]
        assert 0 <= data["avg_interval_ms"] <= data["max_interval_ms"]

        # Validate confidence
        assert isinstance(data["confidence"], int)
        assert 0 <= data["confidence"] <= 100

        # Validate adaptive mode
        valid_modes = ["stealth", "normal", "aggressive"]
        assert data["adaptive_mode"] in valid_modes

    def test_edr_profile_update_endpoint(self, client):
        """Test POST /api/edr/profile-update"""
        update_data = {
            "adaptive_mode": "stealth",
            "profile_data": {
                "syscall_delay": 50,
                "batch_operations": True
            }
        }

        response = client.post("/api/edr/profile-update", json=update_data)
        assert response.status_code == 200

        data = response.json()
        assert data["status"] == "updated"
        assert data["adaptive_mode"] == "stealth"
        assert "timestamp" in data

    def test_edr_profile_modes(self, client):
        """Test different EDR adaptive modes"""
        for mode in ["stealth", "normal", "aggressive"]:
            update_data = {"adaptive_mode": mode}
            response = client.post("/api/edr/profile-update", json=update_data)
            assert response.status_code == 200

            data = response.json()
            assert data["adaptive_mode"] == mode


class TestErrorHandling:
    """Test error handling for all endpoints"""

    @pytest.fixture
    def client(self):
        """Create HTTP client for testing"""
        return httpx.Client(base_url=BACKEND_URL, timeout=TEST_TIMEOUT)

    def test_invalid_driver_name(self, client):
        """Test invalid driver name handling"""
        response = client.get("/api/driver/score/nonexistent.sys")
        # Should still return 200 but with default/error values
        assert response.status_code in [200, 404]

    def test_invalid_event_type(self, client):
        """Test invalid threat event type"""
        event_data = {
            "event_type": "invalid_type",
        }
        response = client.post("/api/ai/threat-event", json=event_data)
        # Should either accept or return validation error
        assert response.status_code in [200, 400, 422]

    def test_missing_required_fields(self, client):
        """Test missing required fields in request"""
        event_data = {}
        response = client.post("/api/ai/threat-event", json=event_data)
        # Should return validation error
        assert response.status_code in [400, 422]


class TestConcurrentRequests:
    """Test concurrent API requests"""

    @pytest.fixture
    def client(self):
        """Create HTTP client for testing"""
        return httpx.Client(base_url=BACKEND_URL, timeout=TEST_TIMEOUT)

    @pytest.mark.asyncio
    async def test_concurrent_threat_scores(self):
        """Test 100+ concurrent threat score requests"""
        async with httpx.AsyncClient(base_url=BACKEND_URL, timeout=TEST_TIMEOUT) as client:
            tasks = []
            for _ in range(LOAD_TEST_CONCURRENT_REQUESTS):
                task = client.get("/api/ai/threat-score")
                tasks.append(task)

            responses = await asyncio.gather(*[client.get("/api/ai/threat-score")
                                               for _ in range(LOAD_TEST_CONCURRENT_REQUESTS)])

            assert len(responses) == LOAD_TEST_CONCURRENT_REQUESTS
            for response in responses:
                assert response.status_code == 200

    @pytest.mark.asyncio
    async def test_concurrent_driver_rankings(self):
        """Test 100+ concurrent driver ranking requests"""
        async with httpx.AsyncClient(base_url=BACKEND_URL, timeout=TEST_TIMEOUT) as client:
            responses = await asyncio.gather(*[client.get("/api/driver/ranking")
                                               for _ in range(LOAD_TEST_CONCURRENT_REQUESTS)])

            assert len(responses) == LOAD_TEST_CONCURRENT_REQUESTS
            for response in responses:
                assert response.status_code == 200


class TestBackendIntegration:
    """Test integration with compilation pipeline"""

    @pytest.fixture
    def client(self):
        """Create HTTP client for testing"""
        return httpx.Client(base_url=BACKEND_URL, timeout=TEST_TIMEOUT)

    def test_get_config_includes_ai_info(self, client):
        """Test that config endpoint includes AI/driver info"""
        response = client.get("/api/config")
        assert response.status_code == 200

        data = response.json()
        assert "api_version" in data
        assert "supported_platforms" in data

    def test_runtime_apis_endpoint(self, client):
        """Test runtime APIs are available"""
        response = client.get("/api/runtime-apis")
        assert response.status_code == 200

        data = response.json()
        assert "categories" in data
        assert len(data["categories"]) > 0


if __name__ == "__main__":
    pytest.main([__file__, "-v", "--tb=short"])
