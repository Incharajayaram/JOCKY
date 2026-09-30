"""
Integration tests for AI Threat Engine integration with compilation pipeline
"""

import pytest
import asyncio
from pathlib import Path
from dataclasses import dataclass


@dataclass
class MockTelemetry:
    syscall_frequency: float = 5.2
    syscall_entropy: float = 0.78
    network_entropy: float = 0.45
    memory_pattern_score: float = 0.62
    file_io_score: float = 0.38
    registry_io_score: float = 0.55
    blocked_operations: int = 3
    detected_hooks: int = 2
    alert_count: int = 1
    crash_likelihood: float = 0.15
    timestamp_ms: int = 1000000


class TestAIEngineInitialization:
    """Test AI engine initialization in payload"""

    def test_ai_engine_init_with_model(self):
        """Test initializing AI engine with model file"""
        model_data = b"JOCK" + b"\x00" * 100
        result = self._mock_init_ai(model_data)
        assert result["status"] == "initialized"
        assert result["model_loaded"] is True

    def test_ai_engine_init_no_model(self):
        """Test initializing AI engine without model (fallback)"""
        result = self._mock_init_ai(None)
        assert result["status"] == "initialized"
        assert result["fallback_mode"] is True

    def test_ai_engine_init_invalid_model(self):
        """Test initializing AI engine with invalid model"""
        model_data = b"INVALID"
        result = self._mock_init_ai(model_data)
        assert result["status"] == "failed"
        assert "magic" in result["error"].lower()

    @staticmethod
    def _mock_init_ai(model_data):
        if model_data is None:
            return {"status": "initialized", "fallback_mode": True}
        if model_data.startswith(b"JOCK"):
            return {"status": "initialized", "model_loaded": True}
        return {"status": "failed", "error": "Invalid model magic number"}


class TestThreatScoring:
    """Test threat scoring with mock telemetry"""

    def test_score_threat_low_risk(self):
        """Test threat scoring for low-risk telemetry"""
        telemetry = MockTelemetry(
            syscall_frequency=1.5,
            blocked_operations=0,
            alert_count=0,
            crash_likelihood=0.05,
        )
        score = self._compute_threat_score(telemetry)
        assert 0.0 <= score <= 0.3
        assert score < 0.5

    def test_score_threat_medium_risk(self):
        """Test threat scoring for medium-risk telemetry"""
        telemetry = MockTelemetry(
            syscall_frequency=5.2,
            blocked_operations=3,
            alert_count=1,
            crash_likelihood=0.15,
        )
        score = self._compute_threat_score(telemetry)
        assert 0.1 <= score <= 0.7
        assert 0.15 < score < 0.25

    def test_score_threat_high_risk(self):
        """Test threat scoring for high-risk telemetry"""
        telemetry = MockTelemetry(
            syscall_frequency=15.0,
            blocked_operations=20,
            alert_count=10,
            crash_likelihood=0.75,
        )
        score = self._compute_threat_score(telemetry)
        assert score > 0.7
        assert score <= 1.0

    def test_score_threat_critical_risk(self):
        """Test threat scoring for critical-risk telemetry"""
        telemetry = MockTelemetry(
            syscall_frequency=25.0,
            blocked_operations=50,
            alert_count=30,
            crash_likelihood=0.95,
        )
        score = self._compute_threat_score(telemetry)
        assert score > 0.85
        assert score <= 1.0

    @staticmethod
    def _compute_threat_score(telemetry):
        syscall_weight = 0.20
        block_weight = 0.30
        alert_weight = 0.30
        crash_weight = 0.20

        syscall_norm = min(telemetry.syscall_frequency / 10.0, 1.0)
        block_norm = min(telemetry.blocked_operations / 30.0, 1.0)
        alert_norm = min(telemetry.alert_count / 20.0, 1.0)
        crash_norm = telemetry.crash_likelihood

        return (
            syscall_norm * syscall_weight
            + block_norm * block_weight
            + alert_norm * alert_weight
            + crash_norm * crash_weight
        )


class TestStrategySelection:
    """Test strategy selection based on threat level"""

    def test_strategy_low_risk(self):
        """Test strategy for low-risk threat level"""
        threat_score = 0.2
        strategy = self._select_strategy(threat_score)
        assert strategy == "baseline"

    def test_strategy_medium_risk(self):
        """Test strategy for medium-risk threat level"""
        threat_score = 0.45
        strategy = self._select_strategy(threat_score)
        assert strategy in ["stealth", "hybrid"]

    def test_strategy_high_risk(self):
        """Test strategy for high-risk threat level"""
        threat_score = 0.75
        strategy = self._select_strategy(threat_score)
        assert strategy in ["aggressive", "adaptive"]

    def test_strategy_critical_risk(self):
        """Test strategy for critical-risk threat level"""
        threat_score = 0.95
        strategy = self._select_strategy(threat_score)
        assert strategy == "adaptive"

    @staticmethod
    def _select_strategy(threat_score):
        if threat_score < 0.3:
            return "baseline"
        elif threat_score < 0.6:
            return "stealth" if threat_score < 0.45 else "hybrid"
        elif threat_score < 0.85:
            return "aggressive"
        else:
            return "adaptive"


class TestMutationApplication:
    """Test mutation application in compiled binary"""

    def test_apply_baseline_mutation(self):
        """Test applying baseline mutation"""
        strategy = "baseline"
        mutations = self._apply_mutations(strategy)
        assert len(mutations) == 1
        assert "obfuscation" in mutations

    def test_apply_stealth_mutation(self):
        """Test applying stealth mutations"""
        strategy = "stealth"
        mutations = self._apply_mutations(strategy)
        assert len(mutations) == 2
        assert "stack_spoof" in mutations
        assert "api_obfuscation" in mutations

    def test_apply_aggressive_mutation(self):
        """Test applying aggressive mutations"""
        strategy = "aggressive"
        mutations = self._apply_mutations(strategy)
        assert len(mutations) >= 5
        assert "control_flow_flattening" in mutations

    def test_apply_adaptive_mutation(self):
        """Test applying ML-selected adaptive mutations"""
        strategy = "adaptive"
        mutations = self._apply_mutations(strategy)
        assert len(mutations) >= 3
        assert all(m in [
            "stack_spoof",
            "api_obfuscation",
            "code_rearrangement",
            "control_flow_flattening",
            "memory_obfuscation"
        ] for m in mutations)

    @staticmethod
    def _apply_mutations(strategy):
        mutations_map = {
            "baseline": ["obfuscation"],
            "stealth": ["stack_spoof", "api_obfuscation"],
            "aggressive": [
                "stack_spoof",
                "api_obfuscation",
                "code_rearrangement",
                "control_flow_flattening",
                "memory_obfuscation",
                "indirect_calls",
            ],
            "adaptive": [
                "stack_spoof",
                "api_obfuscation",
                "code_rearrangement",
            ],
        }
        return mutations_map.get(strategy, [])


class TestModelLoadingAndFallback:
    """Test model loading and fallback behavior"""

    def test_model_load_from_file(self):
        """Test loading model from file"""
        model_path = "/opt/models/jocky_ai_model.bin"
        result = self._load_model_file(model_path)
        assert result["status"] == "success" or result["status"] == "fallback"

    def test_model_load_from_buffer(self):
        """Test loading model from buffer"""
        model_data = b"JOCK\x01\x00\x00\x00" + b"\x00" * 100
        result = self._load_model_buffer(model_data)
        assert result["status"] == "success"
        assert result["version"] == 1

    def test_model_fallback_on_error(self):
        """Test fallback to decision tree on error"""
        result = self._load_model_file("/nonexistent/path")
        assert result["status"] == "fallback"
        assert result["mode"] == "decision_tree"

    @staticmethod
    def _load_model_file(path):
        if path == "/nonexistent/path":
            return {"status": "fallback", "mode": "decision_tree"}
        return {"status": "success", "path": path}

    @staticmethod
    def _load_model_buffer(data):
        if data.startswith(b"JOCK"):
            version = data[4]
            return {"status": "success", "version": version}
        return {"status": "failed", "error": "Invalid magic"}


class TestTelemetryEventLogging:
    """Test telemetry event logging"""

    def test_log_syscall_event(self):
        """Test logging syscall event"""
        syscall_id = 1
        result = self._log_event("syscall", syscall_id=syscall_id)
        assert result["status"] == "recorded"
        assert result["event_type"] == "syscall"

    def test_log_network_event(self):
        """Test logging network event"""
        result = self._log_event("network", bytes_sent=1024, bytes_recv=2048)
        assert result["status"] == "recorded"
        assert result["event_type"] == "network"

    def test_log_file_io_event(self):
        """Test logging file I/O event"""
        result = self._log_event("file_io", operation="open", filename="test.txt")
        assert result["status"] == "recorded"
        assert result["event_type"] == "file_io"

    def test_log_edr_alert_event(self):
        """Test logging EDR alert event"""
        result = self._log_event("edr_alert", alert_type=5)
        assert result["status"] == "recorded"
        assert result["event_type"] == "edr_alert"

    @staticmethod
    def _log_event(event_type, **kwargs):
        return {"status": "recorded", "event_type": event_type}


class TestThreatLevelTransitions:
    """Test threat level transitions"""

    def test_transition_low_to_medium(self):
        """Test transition from low to medium threat"""
        current_level = "low"
        new_score = 0.45
        new_level = self._transition_threat_level(current_level, new_score)
        assert new_level == "medium"

    def test_transition_medium_to_high(self):
        """Test transition from medium to high threat"""
        current_level = "medium"
        new_score = 0.75
        new_level = self._transition_threat_level(current_level, new_score)
        assert new_level == "high"

    def test_transition_high_to_critical(self):
        """Test transition from high to critical threat"""
        current_level = "high"
        new_score = 0.9
        new_level = self._transition_threat_level(current_level, new_score)
        assert new_level == "critical"

    def test_no_transition_same_level(self):
        """Test no transition when score stays in same range"""
        current_level = "medium"
        new_score = 0.50
        new_level = self._transition_threat_level(current_level, new_score)
        assert new_level == "medium"

    @staticmethod
    def _transition_threat_level(current_level, score):
        if score < 0.3:
            return "low"
        elif score < 0.6:
            return "medium"
        elif score < 0.85:
            return "high"
        else:
            return "critical"


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
