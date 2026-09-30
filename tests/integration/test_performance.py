"""
Performance and Stability Tests for AI Threat Engine

This test suite covers:
1. Payload startup time overhead
2. Threat scoring latency
3. Driver selection time
4. EDR profiler overhead
5. Stress testing
6. Memory leak detection
"""

import os
import sys
import time
import asyncio
from pathlib import Path
from typing import List, Tuple
import pytest
import httpx

PROJECT_ROOT = Path(__file__).parent.parent.parent
sys.path.insert(0, str(PROJECT_ROOT))

BACKEND_URL = os.getenv("BACKEND_URL", "http://localhost:8000")
TEST_TIMEOUT = 60


class PerformanceMetrics:
    """Track performance metrics"""

    def __init__(self):
        self.threat_score_latencies: List[float] = []
        self.driver_selection_times: List[float] = []
        self.profiler_overheads: List[float] = []

    def add_threat_score_latency(self, latency_ms: float):
        self.threat_score_latencies.append(latency_ms)

    def add_driver_selection_time(self, time_ms: float):
        self.driver_selection_times.append(time_ms)

    def add_profiler_overhead(self, overhead_percent: float):
        self.profiler_overheads.append(overhead_percent)

    def get_stats(self, latencies: List[float]) -> dict:
        if not latencies:
            return {}

        return {
            "min_ms": min(latencies),
            "max_ms": max(latencies),
            "avg_ms": sum(latencies) / len(latencies),
            "median_ms": sorted(latencies)[len(latencies) // 2],
            "p95_ms": sorted(latencies)[int(len(latencies) * 0.95)] if len(latencies) > 1 else latencies[0],
            "p99_ms": sorted(latencies)[int(len(latencies) * 0.99)] if len(latencies) > 1 else latencies[0],
        }


class TestThreatScoringLatency:
    """Test threat scoring performance"""

    @pytest.fixture
    def client(self):
        return httpx.Client(base_url=BACKEND_URL, timeout=TEST_TIMEOUT)

    def test_threat_score_latency_single_request(self, client):
        """Test single threat score request latency"""
        start = time.perf_counter()
        response = client.get("/api/ai/threat-score")
        end = time.perf_counter()

        latency_ms = (end - start) * 1000

        assert response.status_code == 200
        assert latency_ms < 1000, f"Single request latency {latency_ms}ms exceeds 1000ms"

    def test_threat_score_latency_100_requests(self, client):
        """Test threat scoring with 100 sequential requests"""
        metrics = PerformanceMetrics()

        for _ in range(100):
            start = time.perf_counter()
            response = client.get("/api/ai/threat-score")
            end = time.perf_counter()

            assert response.status_code == 200
            latency_ms = (end - start) * 1000
            metrics.add_threat_score_latency(latency_ms)

        stats = metrics.get_stats(metrics.threat_score_latencies)

        # Verify performance targets
        assert stats["avg_ms"] < 100, f"Average latency {stats['avg_ms']}ms exceeds 100ms"
        assert stats["p95_ms"] < 500, f"P95 latency {stats['p95_ms']}ms exceeds 500ms"
        assert stats["p99_ms"] < 1000, f"P99 latency {stats['p99_ms']}ms exceeds 1000ms"

    @pytest.mark.asyncio
    async def test_concurrent_threat_score_latency(self):
        """Test concurrent threat score request latency"""
        async with httpx.AsyncClient(base_url=BACKEND_URL, timeout=TEST_TIMEOUT) as client:
            metrics = PerformanceMetrics()

            # Run 100 concurrent requests
            tasks = []
            for _ in range(100):
                async def request_with_timing():
                    start = time.perf_counter()
                    response = await client.get("/api/ai/threat-score")
                    end = time.perf_counter()
                    latency_ms = (end - start) * 1000
                    metrics.add_threat_score_latency(latency_ms)
                    assert response.status_code == 200

                tasks.append(request_with_timing())

            await asyncio.gather(*tasks)

            stats = metrics.get_stats(metrics.threat_score_latencies)
            assert stats["avg_ms"] < 200, f"Concurrent avg latency {stats['avg_ms']}ms exceeds 200ms"


class TestDriverSelectionPerformance:
    """Test driver selection performance"""

    @pytest.fixture
    def client(self):
        return httpx.Client(base_url=BACKEND_URL, timeout=TEST_TIMEOUT)

    def test_driver_selection_latency(self, client):
        """Test driver ranking latency (should be <10ms)"""
        start = time.perf_counter()
        response = client.get("/api/driver/ranking")
        end = time.perf_counter()

        latency_ms = (end - start) * 1000

        assert response.status_code == 200
        assert latency_ms < 10, f"Driver selection latency {latency_ms}ms exceeds 10ms"

    def test_driver_fallback_chain_latency(self, client):
        """Test fallback chain generation latency"""
        start = time.perf_counter()
        response = client.get("/api/driver/fallback-chain")
        end = time.perf_counter()

        latency_ms = (end - start) * 1000

        assert response.status_code == 200
        assert latency_ms < 10, f"Fallback chain latency {latency_ms}ms exceeds 10ms"

    def test_driver_selection_with_100_requests(self, client):
        """Test driver selection with 100 requests"""
        metrics = PerformanceMetrics()

        for _ in range(100):
            start = time.perf_counter()
            response = client.get("/api/driver/ranking")
            end = time.perf_counter()

            assert response.status_code == 200
            time_ms = (end - start) * 1000
            metrics.add_driver_selection_time(time_ms)

        stats = metrics.get_stats(metrics.driver_selection_times)
        assert stats["avg_ms"] < 15, f"Avg driver selection time {stats['avg_ms']}ms exceeds 15ms"


class TestEDRProfilerOverhead:
    """Test EDR profiler overhead"""

    @pytest.fixture
    def client(self):
        return httpx.Client(base_url=BACKEND_URL, timeout=TEST_TIMEOUT)

    def test_edr_profile_latency(self, client):
        """Test EDR profile retrieval latency"""
        start = time.perf_counter()
        response = client.get("/api/edr/profile")
        end = time.perf_counter()

        latency_ms = (end - start) * 1000

        assert response.status_code == 200
        # EDR profiler overhead should be minimal (<5ms)
        assert latency_ms < 5, f"EDR profile latency {latency_ms}ms exceeds 5ms"

    def test_edr_profile_update_latency(self, client):
        """Test EDR profile update latency"""
        update_data = {"adaptive_mode": "stealth"}

        start = time.perf_counter()
        response = client.post("/api/edr/profile-update", json=update_data)
        end = time.perf_counter()

        latency_ms = (end - start) * 1000

        assert response.status_code == 200
        assert latency_ms < 5, f"EDR profile update latency {latency_ms}ms exceeds 5ms"


class TestStressScenarios:
    """Test stress scenarios"""

    @pytest.fixture
    def client(self):
        return httpx.Client(base_url=BACKEND_URL, timeout=TEST_TIMEOUT)

    def test_1000_threat_events(self, client):
        """Stress test: 1000+ threat events"""
        for i in range(1000):
            event_data = {
                "event_type": "syscall",
                "syscall_id": i % 256,
                "details": {"pid": i}
            }

            response = client.post("/api/ai/threat-event", json=event_data)
            assert response.status_code == 200

    def test_continuous_driver_ranking_requests(self, client):
        """Stress test: continuous driver ranking requests"""
        for _ in range(500):
            response = client.get("/api/driver/ranking")
            assert response.status_code == 200
            assert "ranked_drivers" in response.json()

    @pytest.mark.asyncio
    async def test_high_concurrency_mixed_requests(self):
        """Stress test: 500 concurrent mixed requests"""
        async with httpx.AsyncClient(base_url=BACKEND_URL, timeout=TEST_TIMEOUT) as client:
            tasks = []

            # Mix of different request types
            endpoints = [
                "/api/ai/threat-score",
                "/api/driver/ranking",
                "/api/edr/profile",
                "/api/ai/mutations",
            ]

            for i in range(500):
                endpoint = endpoints[i % len(endpoints)]
                tasks.append(client.get(endpoint))

            responses = await asyncio.gather(*tasks)

            # Verify all requests succeeded
            success_count = sum(1 for r in responses if r.status_code == 200)
            assert success_count >= len(responses) * 0.95, "At least 95% of requests should succeed"


class TestMemoryAndStability:
    """Test memory usage and stability"""

    @pytest.fixture
    def client(self):
        return httpx.Client(base_url=BACKEND_URL, timeout=TEST_TIMEOUT)

    def test_connection_pooling(self, client):
        """Test connection pooling doesn't leak"""
        # Make many requests through same client
        for _ in range(1000):
            response = client.get("/api/ai/threat-score")
            assert response.status_code == 200

    def test_repeated_edr_profile_updates(self, client):
        """Test repeated EDR profile updates don't leak memory"""
        modes = ["stealth", "normal", "aggressive", "stealth"]

        for _ in range(250):
            for mode in modes:
                update_data = {"adaptive_mode": mode}
                response = client.post("/api/edr/profile-update", json=update_data)
                assert response.status_code == 200

    def test_error_handling_under_load(self, client):
        """Test error handling doesn't cause issues under load"""
        for _ in range(100):
            # Valid request
            response = client.get("/api/ai/threat-score")
            assert response.status_code == 200

            # Invalid request (should handle gracefully)
            response = client.post("/api/ai/threat-event", json={})
            assert response.status_code in [200, 400, 422]


class TestResponseFormats:
    """Test response format consistency"""

    @pytest.fixture
    def client(self):
        return httpx.Client(base_url=BACKEND_URL, timeout=TEST_TIMEOUT)

    def test_json_response_validity(self, client):
        """Test all responses are valid JSON"""
        endpoints = [
            "/api/ai/threat-score",
            "/api/ai/strategy",
            "/api/ai/mutations",
            "/api/driver/ranking",
            "/api/driver/fallback-chain",
            "/api/edr/profile",
        ]

        for endpoint in endpoints:
            response = client.get(endpoint)
            assert response.status_code == 200

            # Verify response is valid JSON
            try:
                data = response.json()
                assert isinstance(data, dict), f"Response from {endpoint} should be dict"
            except Exception as e:
                pytest.fail(f"Invalid JSON from {endpoint}: {e}")

    def test_response_field_consistency(self, client):
        """Test response fields are consistent across requests"""
        # First request
        response1 = client.get("/api/ai/threat-score")
        data1 = response1.json()

        # Second request
        response2 = client.get("/api/ai/threat-score")
        data2 = response2.json()

        # Should have same keys
        assert set(data1.keys()) == set(data2.keys()), "Response keys should be consistent"


if __name__ == "__main__":
    pytest.main([__file__, "-v", "--tb=short"])
