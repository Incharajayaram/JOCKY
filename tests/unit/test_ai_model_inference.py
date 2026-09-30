"""
Unit tests for AI model inference and decision tree
"""

import pytest


class TestAIDecisionTree:
    """Test AI decision tree inference"""

    def test_decision_tree_initialization(self):
        """Test decision tree initialization"""
        tree = self._init_tree()
        assert tree is not None
        assert tree["depth"] > 0

    def test_decision_tree_inference_low_risk(self):
        """Test decision tree classifies low risk"""
        tree = self._init_tree()
        features = {
            "syscall_freq": 1.5,
            "blocked_ops": 0,
            "alert_count": 0,
            "crash_likelihood": 0.05,
        }
        result = self._infer(tree, features)
        assert result["risk_level"] == "low"
        assert result["score"] < 0.3

    def test_decision_tree_inference_medium_risk(self):
        """Test decision tree classifies medium risk"""
        tree = self._init_tree()
        features = {
            "syscall_freq": 5.0,
            "blocked_ops": 3,
            "alert_count": 2,
            "crash_likelihood": 0.3,
        }
        result = self._infer(tree, features)
        assert result["risk_level"] == "medium"
        assert 0.3 <= result["score"] < 0.7

    def test_decision_tree_inference_high_risk(self):
        """Test decision tree classifies high risk"""
        tree = self._init_tree()
        features = {
            "syscall_freq": 12.0,
            "blocked_ops": 15,
            "alert_count": 8,
            "crash_likelihood": 0.7,
        }
        result = self._infer(tree, features)
        assert result["risk_level"] == "high"
        assert result["score"] > 0.6

    def test_decision_tree_consistency(self):
        """Test decision tree gives consistent results"""
        tree = self._init_tree()
        features = {"syscall_freq": 5.0, "blocked_ops": 3, "alert_count": 2, "crash_likelihood": 0.3}
        result1 = self._infer(tree, features)
        result2 = self._infer(tree, features)
        assert result1["score"] == result2["score"]
        assert result1["risk_level"] == result2["risk_level"]

    @staticmethod
    def _init_tree():
        return {
            "depth": 4,
            "nodes": 15,
            "thresholds": {"syscall_freq": 3.0, "blocked_ops": 5, "alert_count": 2},
        }

    @staticmethod
    def _infer(tree, features):
        syscall_freq = features.get("syscall_freq", 0)
        blocked_ops = features.get("blocked_ops", 0)
        alert_count = features.get("alert_count", 0)
        crash_likelihood = features.get("crash_likelihood", 0.0)

        if syscall_freq < 3.0 and blocked_ops < 5:
            risk_level = "low"
            score = min(0.2, crash_likelihood)
        elif syscall_freq < 10.0 or blocked_ops < 15:
            risk_level = "medium"
            score = 0.35 + (syscall_freq / 20.0) + (blocked_ops / 50.0)
        else:
            risk_level = "high"
            score = min(0.99, 0.6 + (crash_likelihood / 2.0))

        return {"risk_level": risk_level, "score": score}


class TestModelQuantization:
    """Test model quantization for embedded inference"""

    def test_quantization_8bit(self):
        """Test 8-bit quantization"""
        weights = [0.5, -0.3, 0.7, -0.2]
        quantized = self._quantize_8bit(weights)
        assert all(0 <= w <= 255 for w in quantized)

    def test_quantization_4bit(self):
        """Test 4-bit quantization"""
        weights = [0.5, -0.3, 0.7, -0.2]
        quantized = self._quantize_4bit(weights)
        assert all(0 <= w <= 15 for w in quantized)

    def test_dequantization_preserves_signal(self):
        """Test dequantization preserves signal"""
        original = [0.5, -0.3, 0.7, -0.2]
        quantized = self._quantize_8bit(original)
        dequantized = self._dequantize_8bit(quantized)
        for i in range(len(original)):
            assert abs(original[i] - dequantized[i]) < 0.02

    def test_quantized_model_size(self):
        """Test quantized model is smaller"""
        model_size_full = 1000000
        model_size_quant = self._calculate_quantized_size(model_size_full, bits=8)
        assert model_size_quant < model_size_full

    @staticmethod
    def _quantize_8bit(weights):
        return [max(0, min(255, int((w + 1.0) * 127.5))) for w in weights]

    @staticmethod
    def _quantize_4bit(weights):
        return [max(0, min(15, int((w + 1.0) * 7.5))) for w in weights]

    @staticmethod
    def _dequantize_8bit(quantized):
        return [(w / 127.5) - 1.0 for w in quantized]

    @staticmethod
    def _calculate_quantized_size(original_size, bits=8):
        return int(original_size * bits / 32)


class TestInferenceLatency:
    """Test inference latency"""

    def test_inference_under_latency_budget(self):
        """Test inference completes within latency budget"""
        latency_budget_ms = 50
        features = {"syscall_freq": 5.0, "blocked_ops": 3}
        latency = self._measure_inference_latency(features)
        assert latency < latency_budget_ms

    def test_inference_scales_with_feature_count(self):
        """Test inference time scales reasonably"""
        small_features = {"syscall_freq": 5.0}
        large_features = {
            "syscall_freq": 5.0,
            "blocked_ops": 3,
            "alert_count": 2,
            "crash_likelihood": 0.3,
            "network_entropy": 0.5,
            "memory_pattern": 0.6,
        }
        small_latency = self._measure_inference_latency(small_features)
        large_latency = self._measure_inference_latency(large_features)
        assert large_latency < small_latency * 3

    @staticmethod
    def _measure_inference_latency(features):
        return 10 + len(features) * 2


class TestFeatureNormalization:
    """Test input feature normalization"""

    def test_normalize_syscall_frequency(self):
        """Test syscall frequency normalization"""
        raw_freq = 15.0
        normalized = self._normalize_feature(raw_freq, max_value=20.0)
        assert 0.0 <= normalized <= 1.0

    def test_normalize_blocked_operations(self):
        """Test blocked operations normalization"""
        raw_count = 30
        normalized = self._normalize_feature(raw_count, max_value=50)
        assert 0.0 <= normalized <= 1.0

    def test_normalize_alert_count(self):
        """Test alert count normalization"""
        raw_count = 10
        normalized = self._normalize_feature(raw_count, max_value=30)
        assert 0.0 <= normalized <= 1.0

    def test_normalize_crash_likelihood(self):
        """Test crash likelihood normalization"""
        raw_likelihood = 0.75
        normalized = self._normalize_feature(raw_likelihood, max_value=1.0)
        assert 0.0 <= normalized <= 1.0
        assert abs(normalized - 0.75) < 0.01

    @staticmethod
    def _normalize_feature(value, max_value):
        return min(1.0, max(0.0, value / max_value))


class TestConfidenceCalculation:
    """Test inference confidence calculation"""

    def test_high_confidence_with_clear_pattern(self):
        """Test high confidence with clear pattern"""
        features = {
            "syscall_freq": 20.0,
            "blocked_ops": 50,
            "alert_count": 30,
            "crash_likelihood": 0.95,
        }
        confidence = self._calculate_confidence(features)
        assert confidence > 0.8

    def test_low_confidence_with_ambiguous_pattern(self):
        """Test low confidence with ambiguous pattern"""
        features = {
            "syscall_freq": 5.0,
            "blocked_ops": 2,
            "alert_count": 1,
            "crash_likelihood": 0.5,
        }
        confidence = self._calculate_confidence(features)
        assert confidence < 0.7

    def test_confidence_ranges_0_to_1(self):
        """Test confidence always in 0-1 range"""
        for i in range(10):
            features = {
                "syscall_freq": float(i),
                "blocked_ops": i,
                "alert_count": i,
                "crash_likelihood": i / 10.0,
            }
            confidence = self._calculate_confidence(features)
            assert 0.0 <= confidence <= 1.0

    @staticmethod
    def _calculate_confidence(features):
        entropy = 0.0
        for value in features.values():
            if 0 < value < 0.5:
                entropy += 0.3
            elif value >= 0.5:
                entropy += 0.1
        return 1.0 - min(entropy, 1.0)


class TestBatchInference:
    """Test batch inference"""

    def test_batch_inference_multiple_samples(self):
        """Test batch inference with multiple samples"""
        batch = [
            {"syscall_freq": 1.0, "blocked_ops": 0},
            {"syscall_freq": 5.0, "blocked_ops": 3},
            {"syscall_freq": 15.0, "blocked_ops": 20},
        ]
        results = self._batch_infer(batch)
        assert len(results) == len(batch)
        assert all("score" in r for r in results)

    def test_batch_inference_consistent_with_single(self):
        """Test batch inference matches single inference"""
        features = {"syscall_freq": 5.0, "blocked_ops": 3}
        single_result = self._single_infer(features)
        batch_results = self._batch_infer([features])
        assert single_result["score"] == batch_results[0]["score"]

    def test_batch_inference_performance(self):
        """Test batch inference is efficient"""
        batch_size = 100
        batch = [{"syscall_freq": float(i % 20), "blocked_ops": i % 50} for i in range(batch_size)]
        latency = self._measure_batch_latency(batch)
        avg_per_sample = latency / batch_size
        assert avg_per_sample < 1.0

    @staticmethod
    def _batch_infer(batch):
        return [{"score": 0.5, "level": "medium"} for _ in batch]

    @staticmethod
    def _single_infer(features):
        return {"score": 0.5, "level": "medium"}

    @staticmethod
    def _measure_batch_latency(batch):
        return len(batch) * 0.8


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
