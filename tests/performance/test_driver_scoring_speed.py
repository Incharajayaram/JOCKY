"""
Performance tests for driver scoring calculation speed
"""

import pytest
import time


class TestDriverScoringLatency:
    """Test driver scoring calculation latency"""

    def test_single_driver_score_latency(self):
        """Test scoring single driver is fast"""
        driver = self._create_driver()
        latency_ms = self._measure_score_time(driver)
        assert latency_ms < 10, f"Scoring took {latency_ms}ms"

    def test_score_multiple_drivers(self):
        """Test scoring multiple drivers"""
        drivers = [self._create_driver(f"driver_{i}") for i in range(10)]
        latency_ms = self._measure_score_batch_time(drivers)
        avg_per_driver = latency_ms / len(drivers)
        assert avg_per_driver < 2, f"Average latency {avg_per_driver}ms exceeds 2ms"

    def test_score_consistency(self):
        """Test scoring is consistent"""
        driver = self._create_driver()
        score1 = self._calculate_score(driver)
        score2 = self._calculate_score(driver)
        assert score1 == score2

    @staticmethod
    def _create_driver(name="test_driver"):
        return {
            "name": name,
            "is_signed": True,
            "is_microsoft_blocked": False,
            "is_on_edr_list": False,
            "sig_revoked": False,
            "yara_matches": 0,
            "release_age_days": 365,
        }

    @staticmethod
    def _measure_score_time(driver):
        start = time.time()
        _ = TestDriverScoringLatency._calculate_score(driver)
        end = time.time()
        return (end - start) * 1000

    @staticmethod
    def _measure_score_batch_time(drivers):
        start = time.time()
        for driver in drivers:
            _ = TestDriverScoringLatency._calculate_score(driver)
        end = time.time()
        return (end - start) * 1000

    @staticmethod
    def _calculate_score(driver):
        evasion = 100
        if not driver["is_signed"]:
            evasion -= 20
        if driver["is_microsoft_blocked"]:
            evasion -= 35

        prevalence = 100 - (driver["release_age_days"] / 10)

        capability = 50

        blocklist = 100
        if driver["is_on_edr_list"]:
            blocklist -= 25

        composite = int(
            evasion * 0.40 + prevalence * 0.20 + capability * 0.20 + blocklist * 0.20
        )
        return composite


class TestRankingPerformance:
    """Test driver ranking performance"""

    def test_rank_10_drivers(self):
        """Test ranking 10 drivers"""
        drivers = [self._create_driver(f"driver_{i}") for i in range(10)]
        latency_ms = self._measure_ranking_time(drivers)
        assert latency_ms < 50

    def test_rank_100_drivers(self):
        """Test ranking 100 drivers"""
        drivers = [self._create_driver(f"driver_{i}") for i in range(100)]
        latency_ms = self._measure_ranking_time(drivers)
        assert latency_ms < 500

    def test_ranking_scales_reasonably(self):
        """Test ranking scales reasonably"""
        drivers_10 = [self._create_driver(f"d{i}") for i in range(10)]
        drivers_100 = [self._create_driver(f"d{i}") for i in range(100)]

        latency_10 = self._measure_ranking_time(drivers_10)
        latency_100 = self._measure_ranking_time(drivers_100)

        ratio = latency_100 / latency_10
        assert ratio < 15, f"Scaling ratio {ratio} exceeds 15x"

    @staticmethod
    def _create_driver(name="test_driver"):
        return {
            "name": name,
            "is_signed": True,
            "is_microsoft_blocked": False,
            "is_on_edr_list": False,
            "sig_revoked": False,
            "yara_matches": 0,
            "release_age_days": 365,
            "score": 75,
        }

    @staticmethod
    def _measure_ranking_time(drivers):
        start = time.time()
        sorted_drivers = sorted(drivers, key=lambda d: d.get("score", 0), reverse=True)
        end = time.time()
        return (end - start) * 1000


class TestFallbackChainGeneration:
    """Test fallback chain generation performance"""

    def test_generate_chain_from_10_drivers(self):
        """Test generating chain from 10 drivers"""
        drivers = [self._create_scored_driver(f"driver_{i}", 100 - i * 5) for i in range(10)]
        latency_ms = self._measure_chain_generation(drivers, 5)
        assert latency_ms < 20

    def test_generate_chain_from_100_drivers(self):
        """Test generating chain from 100 drivers"""
        drivers = [
            self._create_scored_driver(f"driver_{i}", max(0, 100 - i))
            for i in range(100)
        ]
        latency_ms = self._measure_chain_generation(drivers, 10)
        assert latency_ms < 100

    @staticmethod
    def _create_scored_driver(name, score):
        return {"name": name, "score": score}

    @staticmethod
    def _measure_chain_generation(drivers, chain_size):
        start = time.time()
        sorted_drivers = sorted(drivers, key=lambda d: d["score"], reverse=True)
        chain = [d["name"] for d in sorted_drivers[:chain_size]]
        end = time.time()
        return (end - start) * 1000


class TestCapabilityMatching:
    """Test capability matching performance"""

    def test_match_single_driver_capabilities(self):
        """Test matching single driver capabilities"""
        driver_caps = ["cap1", "cap2", "cap3", "cap4", "cap5"]
        required_caps = ["cap1", "cap3"]
        latency_ms = self._measure_capability_match_time(driver_caps, required_caps)
        assert latency_ms < 1

    def test_match_large_capability_set(self):
        """Test matching large capability set"""
        driver_caps = [f"cap_{i}" for i in range(50)]
        required_caps = [f"cap_{i}" for i in range(25)]
        latency_ms = self._measure_capability_match_time(driver_caps, required_caps)
        assert latency_ms < 5

    def test_match_across_many_drivers(self):
        """Test capability matching across many drivers"""
        drivers = [
            {"capabilities": [f"cap_{i % 5}" for i in range(10)]}
            for _ in range(100)
        ]
        required = ["cap_1", "cap_3"]
        latency_ms = self._measure_multi_driver_match(drivers, required)
        assert latency_ms < 100

    @staticmethod
    def _measure_capability_match_time(driver_caps, required_caps):
        start = time.time()
        matched = sum(1 for cap in required_caps if cap in driver_caps)
        score = (matched / len(required_caps)) * 100 if required_caps else 100
        end = time.time()
        return (end - start) * 1000

    @staticmethod
    def _measure_multi_driver_match(drivers, required_caps):
        start = time.time()
        for driver in drivers:
            matched = sum(1 for cap in required_caps if cap in driver["capabilities"])
            _ = (matched / len(required_caps)) * 100 if required_caps else 100
        end = time.time()
        return (end - start) * 1000


class TestScoreCaching:
    """Test score caching performance"""

    def test_cached_score_retrieval(self):
        """Test cached score retrieval is fast"""
        driver = {"name": "driver_a"}
        score_cache = {driver["name"]: 85}

        latency_ms = self._measure_cache_lookup(score_cache, driver["name"])
        assert latency_ms < 1

    def test_cache_miss_overhead(self):
        """Test cache miss overhead"""
        score_cache = {f"driver_{i}": 50 + i for i in range(10)}
        latency_ms = self._measure_cache_lookup(score_cache, "driver_not_found")
        assert latency_ms < 2

    def test_cache_size_doesnt_impact_lookup(self):
        """Test cache size doesn't impact lookup"""
        small_cache = {f"driver_{i}": 50 for i in range(10)}
        large_cache = {f"driver_{i}": 50 for i in range(1000)}

        small_latency = self._measure_cache_lookup(small_cache, "driver_5")
        large_latency = self._measure_cache_lookup(large_cache, "driver_500")

        assert large_latency < small_latency * 2

    @staticmethod
    def _measure_cache_lookup(cache, key):
        start = time.time()
        _ = cache.get(key, None)
        end = time.time()
        return (end - start) * 1000


class TestMemoryUsage:
    """Test memory usage for driver scoring"""

    def test_score_data_structure_size(self):
        """Test driver score data structure is efficient"""
        driver = {
            "name": "test",
            "score": 75,
            "evasion": 80,
            "prevalence": 70,
            "capability": 75,
            "blocklist": 70,
        }
        memory_bytes = len(str(driver).encode())
        assert memory_bytes < 500

    def test_score_cache_memory(self):
        """Test score cache memory usage"""
        cache = {f"driver_{i}": {"score": 50 + i % 50, "components": [80, 70, 75, 65]} for i in range(1000)}
        memory_kb = len(str(cache).encode()) / 1024
        assert memory_kb < 1000


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
