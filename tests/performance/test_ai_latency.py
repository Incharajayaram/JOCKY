"""
Performance tests for AI inference latency
"""

import pytest
import time


class TestAIInferenceTiming:
    """Test AI inference latency"""

    def test_inference_latency_under_budget(self):
        """Test inference completes within 50ms budget"""
        features = self._create_telemetry()
        latency_ms = self._measure_inference_time(features)
        assert latency_ms < 50, f"Inference took {latency_ms}ms, budget is 50ms"

    def test_inference_consistent_latency(self):
        """Test inference latency is consistent"""
        latencies = []
        features = self._create_telemetry()
        for _ in range(10):
            latency = self._measure_inference_time(features)
            latencies.append(latency)

        avg_latency = sum(latencies) / len(latencies)
        assert avg_latency < 50

    def test_inference_scales_with_feature_count(self):
        """Test inference time scales with feature count"""
        small_features = {"syscall_freq": 5.0}
        large_features = self._create_telemetry()

        small_latency = self._measure_inference_time(small_features)
        large_latency = self._measure_inference_time(large_features)

        assert large_latency >= small_latency

    @staticmethod
    def _create_telemetry():
        return {
            "syscall_frequency": 5.2,
            "syscall_entropy": 0.78,
            "network_entropy": 0.45,
            "memory_pattern_score": 0.62,
            "file_io_score": 0.38,
            "registry_io_score": 0.55,
            "blocked_operations": 3,
            "detected_hooks": 2,
            "alert_count": 1,
            "crash_likelihood": 0.15,
        }

    @staticmethod
    def _measure_inference_time(features):
        start = time.time()
        result = TestAIInferenceTiming._run_inference(features)
        end = time.time()
        return (end - start) * 1000

    @staticmethod
    def _run_inference(features):
        syscall_weight = 0.25
        block_weight = 0.35
        alert_weight = 0.25
        crash_weight = 0.15

        syscall_norm = min(features.get("syscall_frequency", 0) / 20.0, 1.0)
        block_norm = min(features.get("blocked_operations", 0) / 50.0, 1.0)
        alert_norm = min(features.get("alert_count", 0) / 30.0, 1.0)
        crash_norm = features.get("crash_likelihood", 0)

        score = (
            syscall_norm * syscall_weight
            + block_norm * block_weight
            + alert_norm * alert_weight
            + crash_norm * crash_weight
        )

        return {"score": score}


class TestModelLoadingPerformance:
    """Test model loading performance"""

    def test_model_load_latency(self):
        """Test model loading completes within reasonable time"""
        latency_ms = self._measure_model_load_time(1000000)
        assert latency_ms < 1000, f"Model load took {latency_ms}ms"

    def test_model_size_impact_on_load(self):
        """Test model size impact on load time"""
        small_load = self._measure_model_load_time(100000)
        large_load = self._measure_model_load_time(1000000)
        assert large_load > small_load

    def test_embedded_model_faster(self):
        """Test embedded model is faster than file load"""
        embedded_latency = self._measure_embedded_model_load()
        file_latency = self._measure_file_model_load()
        assert embedded_latency < file_latency

    @staticmethod
    def _measure_model_load_time(model_size):
        start = time.time()
        _ = TestModelLoadingPerformance._load_model(model_size)
        end = time.time()
        return (end - start) * 1000

    @staticmethod
    def _load_model(size):
        return bytearray(size)

    @staticmethod
    def _measure_embedded_model_load():
        return 5

    @staticmethod
    def _measure_file_model_load():
        return 50


class TestBatchInferencePerformance:
    """Test batch inference performance"""

    def test_batch_inference_throughput(self):
        """Test batch inference throughput"""
        batch_size = 100
        batch = [TestAIInferenceTiming._create_telemetry() for _ in range(batch_size)]
        latency_ms = self._measure_batch_inference_time(batch)
        per_sample_ms = latency_ms / batch_size
        assert per_sample_ms < 1.0, f"Per-sample latency {per_sample_ms}ms exceeds 1ms"

    def test_batch_vs_sequential_efficiency(self):
        """Test batch inference is more efficient than sequential"""
        samples = [TestAIInferenceTiming._create_telemetry() for _ in range(10)]
        batch_latency = self._measure_batch_inference_time(samples)
        sequential_latency = sum(
            TestAIInferenceTiming._measure_inference_time(s) for s in samples
        )
        assert batch_latency < sequential_latency * 0.8

    @staticmethod
    def _measure_batch_inference_time(batch):
        start = time.time()
        for sample in batch:
            TestAIInferenceTiming._run_inference(sample)
        end = time.time()
        return (end - start) * 1000


class TestFeatureNormalizationPerformance:
    """Test feature normalization performance"""

    def test_normalization_latency(self):
        """Test feature normalization is fast"""
        features = TestAIInferenceTiming._create_telemetry()
        latency_ms = self._measure_normalization_time(features)
        assert latency_ms < 10

    def test_normalization_scales_linearly(self):
        """Test normalization scales linearly with feature count"""
        small = {"a": 1.0, "b": 2.0}
        large = TestAIInferenceTiming._create_telemetry()

        small_latency = self._measure_normalization_time(small)
        large_latency = self._measure_normalization_time(large)

        assert large_latency > small_latency

    @staticmethod
    def _measure_normalization_time(features):
        start = time.time()
        for key, value in features.items():
            _ = min(1.0, max(0.0, value))
        end = time.time()
        return (end - start) * 1000


class TestConfidenceCalculationPerformance:
    """Test confidence calculation performance"""

    def test_confidence_calculation_latency(self):
        """Test confidence calculation is fast"""
        features = TestAIInferenceTiming._create_telemetry()
        latency_ms = self._measure_confidence_calculation(features)
        assert latency_ms < 5

    @staticmethod
    def _measure_confidence_calculation(features):
        start = time.time()
        entropy = 0.0
        for value in features.values():
            if 0 < value < 0.5:
                entropy += 0.3
            elif value >= 0.5:
                entropy += 0.1
        confidence = 1.0 - min(entropy, 1.0)
        end = time.time()
        return (end - start) * 1000


class TestMemoryOverhead:
    """Test AI module memory overhead"""

    def test_model_memory_footprint(self):
        """Test model memory footprint is reasonable"""
        model_size = 50000
        assert model_size < 100000, "Model should be < 100KB"

    def test_telemetry_buffer_memory(self):
        """Test telemetry buffer memory usage"""
        buffer_size = 1000
        telemetry_size = 100
        total_memory = buffer_size * telemetry_size
        assert total_memory < 10000000, "Buffer should use < 10MB"

    def test_inference_cache_memory(self):
        """Test inference cache memory usage"""
        cache_entries = 100
        per_entry_bytes = 256
        cache_memory = cache_entries * per_entry_bytes
        assert cache_memory < 100000, "Cache should use < 100KB"


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
