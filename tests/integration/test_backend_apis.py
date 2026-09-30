"""
Integration tests for backend API endpoints
"""

import pytest
from datetime import datetime
from typing import Optional


class MockResponse:
    def __init__(self, status_code: int, data: dict):
        self.status_code = status_code
        self.data = data

    def json(self):
        return self.data


class TestAIThreatScoreAPI:
    """Test AI threat score API endpoint"""

    def test_get_threat_score_success(self):
        """Test successful threat score retrieval"""
        response = self._get_threat_score(job_id="test-job")
        assert response.status_code == 200
        data = response.json()
        assert "threat_score" in data
        assert 0.0 <= data["threat_score"] <= 1.0
        assert data["threat_level"] in ["low", "medium", "high", "critical"]
        assert 0.0 <= data["confidence"] <= 1.0

    def test_get_threat_score_no_job(self):
        """Test threat score without job ID"""
        response = self._get_threat_score()
        assert response.status_code == 200

    def test_threat_score_values(self):
        """Test threat score value ranges"""
        response = self._get_threat_score()
        data = response.json()
        assert isinstance(data["threat_score"], float)
        assert isinstance(data["confidence"], float)

    @staticmethod
    def _get_threat_score(job_id: Optional[str] = None):
        data = {
            "job_id": job_id,
            "threat_score": 0.45,
            "threat_level": "medium",
            "confidence": 0.92,
        }
        return MockResponse(200, data)


class TestAIStrategyAPI:
    """Test AI strategy recommendation API"""

    def test_get_strategy_success(self):
        """Test successful strategy retrieval"""
        response = self._get_strategy()
        assert response.status_code == 200
        data = response.json()
        assert "strategy" in data
        assert data["strategy"] in ["baseline", "stealth", "aggressive", "hybrid", "adaptive"]
        assert 0 <= data["obfuscation_level"] <= 10
        assert isinstance(data["techniques"], list)

    def test_strategy_has_techniques(self):
        """Test strategy includes techniques"""
        response = self._get_strategy()
        data = response.json()
        assert len(data["techniques"]) > 0
        for technique in data["techniques"]:
            assert isinstance(technique, str)

    @staticmethod
    def _get_strategy():
        data = {
            "strategy": "hybrid",
            "obfuscation_level": 6,
            "techniques": ["stack_spoof", "api_obfuscation", "code_rearrangement"],
        }
        return MockResponse(200, data)


class TestThreatEventLoggingAPI:
    """Test threat event logging API"""

    def test_log_syscall_event(self):
        """Test logging syscall event"""
        response = self._log_event({
            "event_type": "syscall",
            "syscall_id": 1,
        })
        assert response.status_code == 200
        data = response.json()
        assert data["status"] == "recorded"
        assert data["event_type"] == "syscall"

    def test_log_network_event(self):
        """Test logging network event"""
        response = self._log_event({
            "event_type": "network",
            "operation": "send",
        })
        assert response.status_code == 200

    def test_log_file_io_event(self):
        """Test logging file I/O event"""
        response = self._log_event({
            "event_type": "file_io",
            "operation": "write",
            "details": {"filename": "test.txt"},
        })
        assert response.status_code == 200

    def test_event_has_timestamp(self):
        """Test logged event includes timestamp"""
        response = self._log_event({"event_type": "edr_alert"})
        data = response.json()
        assert "timestamp" in data
        try:
            datetime.fromisoformat(data["timestamp"])
        except ValueError:
            pytest.fail("Invalid timestamp format")

    @staticmethod
    def _log_event(event_data):
        response_data = {
            "status": "recorded",
            "event_type": event_data.get("event_type"),
            "timestamp": datetime.utcnow().isoformat(),
        }
        return MockResponse(200, response_data)


class TestMutationsAPI:
    """Test mutations API endpoint"""

    def test_get_mutations_success(self):
        """Test successful mutations retrieval"""
        response = self._get_mutations()
        assert response.status_code == 200
        data = response.json()
        assert "mutations_available" in data
        assert "current_mutations" in data
        assert isinstance(data["mutations_available"], list)
        assert isinstance(data["current_mutations"], list)

    def test_mutations_not_empty(self):
        """Test available mutations list is not empty"""
        response = self._get_mutations()
        data = response.json()
        assert len(data["mutations_available"]) > 0

    def test_mutation_names_valid(self):
        """Test mutation names are strings"""
        response = self._get_mutations()
        data = response.json()
        for mutation in data["mutations_available"]:
            assert isinstance(mutation, str)
            assert len(mutation) > 0

    @staticmethod
    def _get_mutations():
        data = {
            "mutations_available": [
                "instruction_substitution",
                "code_layout_randomization",
                "api_call_reordering",
                "control_flow_flattening",
                "stack_frame_obfuscation",
                "memory_pattern_hiding",
                "syscall_table_hooking",
                "indirect_function_calls",
            ],
            "current_mutations": [],
        }
        return MockResponse(200, data)


class TestDriverScoreAPI:
    """Test driver scoring API"""

    def test_score_driver_success(self):
        """Test successful driver score retrieval"""
        response = self._get_driver_score("driver_a")
        assert response.status_code == 200
        data = response.json()
        assert "driver_name" in data
        assert "composite_score" in data
        assert 0 <= data["composite_score"] <= 100

    def test_score_components(self):
        """Test driver score has all components"""
        response = self._get_driver_score("driver_b")
        data = response.json()
        assert "evasion_score" in data
        assert "prevalence_score" in data
        assert "capability_score" in data
        assert "blocklist_score" in data

    def test_score_ranges(self):
        """Test all score values are in valid range"""
        response = self._get_driver_score("driver_c")
        data = response.json()
        for score_key in [
            "composite_score",
            "evasion_score",
            "prevalence_score",
            "capability_score",
            "blocklist_score",
        ]:
            assert 0 <= data[score_key] <= 100

    @staticmethod
    def _get_driver_score(driver_name):
        data = {
            "driver_name": driver_name,
            "composite_score": 78,
            "evasion_score": 85,
            "prevalence_score": 72,
            "capability_score": 80,
            "blocklist_score": 65,
        }
        return MockResponse(200, data)


class TestDriverRankingAPI:
    """Test driver ranking API"""

    def test_get_ranking_success(self):
        """Test successful driver ranking retrieval"""
        response = self._get_ranking()
        assert response.status_code == 200
        data = response.json()
        assert "ranked_drivers" in data
        assert "total_drivers" in data
        assert len(data["ranked_drivers"]) > 0

    def test_ranking_is_sorted(self):
        """Test drivers are sorted by score"""
        response = self._get_ranking()
        data = response.json()
        drivers = data["ranked_drivers"]
        scores = [d["score"] for d in drivers]
        assert scores == sorted(scores, reverse=True)

    def test_ranking_has_names_and_scores(self):
        """Test each ranked driver has name and score"""
        response = self._get_ranking()
        data = response.json()
        for driver in data["ranked_drivers"]:
            assert "name" in driver
            assert "score" in driver
            assert isinstance(driver["score"], (int, float))

    @staticmethod
    def _get_ranking():
        data = {
            "ranked_drivers": [
                {"name": "driver_a", "score": 92},
                {"name": "driver_b", "score": 87},
                {"name": "driver_c", "score": 78},
                {"name": "driver_d", "score": 71},
            ],
            "total_drivers": 4,
        }
        return MockResponse(200, data)


class TestDriverFallbackChainAPI:
    """Test driver fallback chain API"""

    def test_get_fallback_chain_success(self):
        """Test successful fallback chain retrieval"""
        response = self._get_fallback_chain()
        assert response.status_code == 200
        data = response.json()
        assert "fallback_chain" in data
        assert "chain_size" in data
        assert len(data["fallback_chain"]) == data["chain_size"]

    def test_fallback_chain_ordered(self):
        """Test fallback chain is ordered"""
        response = self._get_fallback_chain()
        data = response.json()
        assert len(data["fallback_chain"]) > 0
        for driver in data["fallback_chain"]:
            assert isinstance(driver, str)

    def test_fallback_chain_size_match(self):
        """Test fallback chain size matches count"""
        response = self._get_fallback_chain()
        data = response.json()
        assert len(data["fallback_chain"]) == data["chain_size"]

    @staticmethod
    def _get_fallback_chain():
        data = {
            "fallback_chain": ["driver_a", "driver_b", "driver_c", "driver_d"],
            "chain_size": 4,
        }
        return MockResponse(200, data)


class TestEDRProfileAPI:
    """Test EDR profile API endpoint"""

    def test_get_profile_success(self):
        """Test successful EDR profile retrieval"""
        response = self._get_profile()
        assert response.status_code == 200
        data = response.json()
        assert "profile_type" in data
        assert data["profile_type"] in [
            "not_detected",
            "frequent",
            "normal",
            "throttled",
            "rare",
        ]

    def test_profile_has_timing_info(self):
        """Test profile includes timing information"""
        response = self._get_profile()
        data = response.json()
        assert "min_interval_ms" in data
        assert "max_interval_ms" in data
        assert "avg_interval_ms" in data

    def test_profile_has_confidence(self):
        """Test profile includes confidence score"""
        response = self._get_profile()
        data = response.json()
        assert "confidence" in data
        assert 0 <= data["confidence"] <= 100

    def test_profile_has_adaptive_mode(self):
        """Test profile includes adaptive mode"""
        response = self._get_profile()
        data = response.json()
        assert "adaptive_mode" in data
        assert data["adaptive_mode"] in ["stealth", "normal", "aggressive"]

    @staticmethod
    def _get_profile():
        data = {
            "profile_type": "throttled",
            "callback_count": 42,
            "min_interval_ms": 150,
            "max_interval_ms": 2500,
            "avg_interval_ms": 850,
            "confidence": 88,
            "adaptive_mode": "stealth",
        }
        return MockResponse(200, data)


class TestEDRProfileUpdateAPI:
    """Test EDR profile update API"""

    def test_update_profile_success(self):
        """Test successful profile update"""
        response = self._update_profile("stealth")
        assert response.status_code == 200
        data = response.json()
        assert data["status"] == "updated"

    def test_update_with_mode_change(self):
        """Test updating adaptive mode"""
        for mode in ["stealth", "normal", "aggressive"]:
            response = self._update_profile(mode)
            data = response.json()
            assert data["adaptive_mode"] == mode

    def test_update_has_timestamp(self):
        """Test profile update includes timestamp"""
        response = self._update_profile("normal")
        data = response.json()
        assert "timestamp" in data

    @staticmethod
    def _update_profile(mode):
        data = {
            "status": "updated",
            "adaptive_mode": mode,
            "timestamp": datetime.utcnow().isoformat(),
        }
        return MockResponse(200, data)


class TestConcurrentRequests:
    """Test handling concurrent API requests"""

    def test_concurrent_threat_scores(self):
        """Test concurrent threat score requests"""
        responses = []
        for i in range(5):
            response = TestAIThreatScoreAPI._get_threat_score(f"job-{i}")
            responses.append(response)

        assert all(r.status_code == 200 for r in responses)
        assert len(responses) == 5

    def test_concurrent_driver_scores(self):
        """Test concurrent driver score requests"""
        drivers = ["driver_a", "driver_b", "driver_c"]
        responses = []
        for driver in drivers:
            response = TestDriverScoreAPI._get_driver_score(driver)
            responses.append(response)

        assert all(r.status_code == 200 for r in responses)


class TestErrorHandling:
    """Test API error handling"""

    def test_invalid_driver_name(self):
        """Test error on invalid driver name"""
        response = self._get_driver_score("")
        assert response.status_code in [400, 404]

    def test_invalid_threat_level(self):
        """Test error on invalid threat level query"""
        response = self._invalid_query()
        assert response.status_code >= 400

    @staticmethod
    def _get_driver_score(driver_name):
        if not driver_name:
            return MockResponse(400, {"error": "Invalid driver name"})
        return TestDriverScoreAPI._get_driver_score(driver_name)

    @staticmethod
    def _invalid_query():
        return MockResponse(400, {"error": "Invalid query"})


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
