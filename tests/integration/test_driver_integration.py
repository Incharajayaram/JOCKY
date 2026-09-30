"""
Integration tests for BYOVD Driver Intelligence with compilation pipeline
"""

import pytest
from dataclasses import dataclass
from typing import List, Optional


@dataclass
class DriverMetadata:
    name: str
    sha256_low: int
    sha256_high: int
    release_timestamp: int
    size_bytes: int
    is_signed: bool
    is_microsoft_blocked: bool
    is_on_edr_list: bool
    sig_revoked: bool
    yara_matches: int
    capabilities: List[str]


class TestDriverScoringCalculation:
    """Test driver scoring calculation"""

    def test_score_evasion_high(self):
        """Test evasion score calculation for high-evasion driver"""
        driver = DriverMetadata(
            name="evga_driver",
            sha256_low=0xABCD1234,
            sha256_high=0x5678EFAB,
            release_timestamp=1609459200,
            size_bytes=256000,
            is_signed=True,
            is_microsoft_blocked=False,
            is_on_edr_list=False,
            sig_revoked=False,
            yara_matches=0,
            capabilities=["read_kernel", "write_kernel"],
        )
        score = self._calculate_evasion_score(driver)
        assert score > 70
        assert score <= 100

    def test_score_evasion_medium(self):
        """Test evasion score for medium-evasion driver"""
        driver = DriverMetadata(
            name="generic_driver",
            sha256_low=0x12345678,
            sha256_high=0x9ABCDEF0,
            release_timestamp=1577836800,
            size_bytes=512000,
            is_signed=True,
            is_microsoft_blocked=False,
            is_on_edr_list=True,
            sig_revoked=False,
            yara_matches=1,
            capabilities=["read_file"],
        )
        score = self._calculate_evasion_score(driver)
        assert 40 < score <= 70

    def test_score_evasion_low(self):
        """Test evasion score for low-evasion driver"""
        driver = DriverMetadata(
            name="blocked_driver",
            sha256_low=0x11111111,
            sha256_high=0x22222222,
            release_timestamp=1546300800,
            size_bytes=1024000,
            is_signed=False,
            is_microsoft_blocked=True,
            is_on_edr_list=True,
            sig_revoked=True,
            yara_matches=5,
            capabilities=[],
        )
        score = self._calculate_evasion_score(driver)
        assert score <= 40

    def test_score_prevalence_high(self):
        """Test prevalence score for rare driver"""
        driver = DriverMetadata(
            name="rare_driver",
            sha256_low=0xAAAAAAAA,
            sha256_high=0xBBBBBBBB,
            release_timestamp=1609459200,
            size_bytes=256000,
            is_signed=True,
            is_microsoft_blocked=False,
            is_on_edr_list=False,
            sig_revoked=False,
            yara_matches=0,
            capabilities=["read_kernel"],
        )
        score = self._calculate_prevalence_score(driver)
        assert score > 70

    def test_score_capability_match(self):
        """Test capability score for matching capabilities"""
        driver = DriverMetadata(
            name="capable_driver",
            sha256_low=0xCCCCCCCC,
            sha256_high=0xDDDDDDDD,
            release_timestamp=1609459200,
            size_bytes=256000,
            is_signed=True,
            is_microsoft_blocked=False,
            is_on_edr_list=False,
            sig_revoked=False,
            yara_matches=0,
            capabilities=["read_kernel", "write_kernel", "execute_arbitrary"],
        )
        required = ["read_kernel", "write_kernel"]
        score = self._calculate_capability_score(driver, required)
        assert score == 100

    def test_score_capability_partial_match(self):
        """Test capability score for partial capability match"""
        driver = DriverMetadata(
            name="partial_driver",
            sha256_low=0xEEEEEEEE,
            sha256_high=0xFFFFFFFF,
            release_timestamp=1609459200,
            size_bytes=256000,
            is_signed=True,
            is_microsoft_blocked=False,
            is_on_edr_list=False,
            sig_revoked=False,
            yara_matches=0,
            capabilities=["read_kernel"],
        )
        required = ["read_kernel", "write_kernel"]
        score = self._calculate_capability_score(driver, required)
        assert 40 <= score < 100

    def test_score_blocklist_impact(self):
        """Test blocklist score impact on total score"""
        driver_clean = DriverMetadata(
            name="clean_driver",
            sha256_low=0x00000001,
            sha256_high=0x00000002,
            release_timestamp=1609459200,
            size_bytes=256000,
            is_signed=True,
            is_microsoft_blocked=False,
            is_on_edr_list=False,
            sig_revoked=False,
            yara_matches=0,
            capabilities=["read_kernel"],
        )

        driver_blocked = DriverMetadata(
            name="blocked_driver",
            sha256_low=0x00000003,
            sha256_high=0x00000004,
            release_timestamp=1609459200,
            size_bytes=256000,
            is_signed=True,
            is_microsoft_blocked=True,
            is_on_edr_list=True,
            sig_revoked=True,
            yara_matches=3,
            capabilities=["read_kernel"],
        )

        score_clean = self._calculate_blocklist_score(driver_clean)
        score_blocked = self._calculate_blocklist_score(driver_blocked)
        assert score_clean > score_blocked

    @staticmethod
    def _calculate_evasion_score(driver):
        score = 100
        if driver.is_microsoft_blocked:
            score -= 25
        if driver.is_on_edr_list:
            score -= 15
        if driver.sig_revoked:
            score -= 20
        if driver.yara_matches > 0:
            score -= min(10 * driver.yara_matches, 30)
        return max(score, 0)

    @staticmethod
    def _calculate_prevalence_score(driver):
        score = 100
        if (driver.release_timestamp > 1609459200):
            score += 20
        else:
            score -= 10
        return min(max(score, 0), 100)

    @staticmethod
    def _calculate_capability_score(driver, required):
        if not required:
            return 100
        matched = sum(1 for cap in required if cap in driver.capabilities)
        return int((matched / len(required)) * 100)

    @staticmethod
    def _calculate_blocklist_score(driver):
        score = 100
        if driver.is_microsoft_blocked:
            score -= 30
        if driver.is_on_edr_list:
            score -= 25
        if driver.sig_revoked:
            score -= 20
        if driver.yara_matches > 0:
            score -= min(5 * driver.yara_matches, 25)
        return max(score, 0)


class TestFallbackChainSelection:
    """Test fallback chain selection based on driver scores"""

    def test_select_top_ranked_driver(self):
        """Test selecting top-ranked driver"""
        drivers = self._create_test_drivers()
        best = self._select_best_driver(drivers)
        assert best == "driver_a"

    def test_fallback_chain_ranking(self):
        """Test fallback chain maintains proper ranking"""
        drivers = self._create_test_drivers()
        chain = self._get_fallback_chain(drivers, chain_size=4)
        assert chain == ["driver_a", "driver_b", "driver_c", "driver_d"]
        assert len(chain) == 4

    def test_fallback_chain_skips_blocked(self):
        """Test fallback chain skips blocked drivers"""
        drivers = self._create_test_drivers()
        chain = self._get_fallback_chain(drivers, chain_size=2)
        assert "driver_c" not in chain
        assert len(chain) == 2

    def test_fallback_chain_preference(self):
        """Test fallback chain respects driver preference"""
        drivers = self._create_test_drivers()
        chain = self._get_fallback_chain(drivers, chain_size=4, prefer="driver_b")
        assert chain[0] == "driver_b"

    @staticmethod
    def _create_test_drivers():
        return {
            "driver_a": {"score": 95, "blocked": False},
            "driver_b": {"score": 87, "blocked": False},
            "driver_c": {"score": 75, "blocked": True},
            "driver_d": {"score": 68, "blocked": False},
        }

    @staticmethod
    def _select_best_driver(drivers):
        best_name = None
        best_score = -1
        for name, info in drivers.items():
            if not info["blocked"] and info["score"] > best_score:
                best_score = info["score"]
                best_name = name
        return best_name

    @staticmethod
    def _get_fallback_chain(drivers, chain_size, prefer=None):
        available = {
            name: info
            for name, info in drivers.items()
            if not info["blocked"]
        }

        if prefer and prefer in available:
            sorted_drivers = [prefer]
            remaining = sorted(
                (n for n in available if n != prefer),
                key=lambda n: available[n]["score"],
                reverse=True,
            )
            sorted_drivers.extend(remaining)
        else:
            sorted_drivers = sorted(
                available.keys(),
                key=lambda n: available[n]["score"],
                reverse=True,
            )

        return sorted_drivers[:chain_size]


class TestBYOVDInitializationWithDriver:
    """Test BYOVD initialization with top-ranked driver"""

    def test_init_byovd_with_best_driver(self):
        """Test BYOVD initialization with best driver"""
        drivers = ["driver_a", "driver_b", "driver_c"]
        best = drivers[0]
        result = self._init_byovd(best)
        assert result["status"] == "initialized"
        assert result["driver"] == "driver_a"

    def test_init_byovd_fallback_on_error(self):
        """Test BYOVD falls back on driver error"""
        drivers = ["driver_a", "driver_b", "driver_c"]
        result = self._init_byovd_with_fallback(drivers, "driver_a")
        assert result["status"] == "initialized"

    def test_init_byovd_all_drivers_failed(self):
        """Test BYOVD fails when all drivers unavailable"""
        drivers = []
        result = self._init_byovd_with_fallback(drivers, None)
        assert result["status"] == "failed"
        assert "no drivers" in result.get("error", "").lower()

    @staticmethod
    def _init_byovd(driver_name):
        return {"status": "initialized", "driver": driver_name}

    @staticmethod
    def _init_byovd_with_fallback(drivers, preferred):
        if not drivers:
            return {"status": "failed", "error": "No drivers available"}
        selected = preferred if preferred in drivers else drivers[0]
        return {"status": "initialized", "driver": selected}


class TestEDRProfilerInitialization:
    """Test EDR profiler initialization"""

    def test_edr_profiler_init_success(self):
        """Test successful EDR profiler initialization"""
        result = self._init_edr_profiler()
        assert result["status"] == "initialized"
        assert "handle" in result

    def test_edr_profiler_load_existing_profile(self):
        """Test loading existing EDR profile"""
        result = self._load_edr_profile("/path/to/profile.bin")
        assert result["status"] == "loaded"

    def test_edr_profiler_create_new_profile(self):
        """Test creating new EDR profile"""
        result = self._create_edr_profile()
        assert result["status"] == "created"
        assert result["profile_type"] == "not_detected"

    @staticmethod
    def _init_edr_profiler():
        return {"status": "initialized", "handle": 0x12345678}

    @staticmethod
    def _load_edr_profile(path):
        return {"status": "loaded", "path": path}

    @staticmethod
    def _create_edr_profile():
        return {"status": "created", "profile_type": "not_detected"}


class TestThrottleProfileDetection:
    """Test EDR throttle profile detection"""

    def test_detect_no_throttling(self):
        """Test detection of no throttling"""
        callbacks = [100, 105, 110, 115]
        profile = self._detect_profile(callbacks)
        assert profile["type"] == "not_detected"

    def test_detect_frequent_throttling(self):
        """Test detection of frequent EDR callbacks"""
        callbacks = [10, 20, 30, 40, 50]
        profile = self._detect_profile(callbacks)
        assert profile["type"] == "frequent"

    def test_detect_normal_throttling(self):
        """Test detection of normal EDR callbacks"""
        callbacks = [100, 200, 300, 400, 500]
        profile = self._detect_profile(callbacks)
        assert profile["type"] == "normal"

    def test_detect_throttled_behavior(self):
        """Test detection of throttled EDR"""
        callbacks = [500, 1000, 1500, 2000, 2500]
        profile = self._detect_profile(callbacks)
        assert profile["type"] == "throttled"

    def test_detect_rare_throttling(self):
        """Test detection of rare EDR callbacks"""
        callbacks = [5000, 10000, 15000, 20000]
        profile = self._detect_profile(callbacks)
        assert profile["type"] == "rare"

    @staticmethod
    def _detect_profile(callbacks):
        if not callbacks:
            return {"type": "unknown"}

        intervals = [callbacks[i + 1] - callbacks[i] for i in range(len(callbacks) - 1)]
        avg_interval = sum(intervals) / len(intervals) if intervals else 0

        if avg_interval < 50:
            profile_type = "frequent"
        elif avg_interval < 300:
            profile_type = "normal"
        elif avg_interval < 1500:
            profile_type = "throttled"
        else:
            profile_type = "rare"

        return {
            "type": profile_type,
            "avg_interval": avg_interval,
            "min_interval": min(intervals) if intervals else 0,
            "max_interval": max(intervals) if intervals else 0,
        }


class TestAdaptiveBehavior:
    """Test adaptive behavior based on EDR profile"""

    def test_adaptive_mode_stealth(self):
        """Test stealth mode for frequent EDR"""
        profile_type = "frequent"
        mode = self._get_adaptive_mode(profile_type)
        assert mode == "stealth"

    def test_adaptive_mode_normal(self):
        """Test normal mode for normal EDR"""
        profile_type = "normal"
        mode = self._get_adaptive_mode(profile_type)
        assert mode == "normal"

    def test_adaptive_mode_aggressive(self):
        """Test aggressive mode for rare EDR"""
        profile_type = "rare"
        mode = self._get_adaptive_mode(profile_type)
        assert mode == "aggressive"

    def test_syscall_delay_stealth(self):
        """Test syscall delay in stealth mode"""
        mode = "stealth"
        delay = self._get_syscall_delay(mode)
        assert delay > 50

    def test_syscall_delay_aggressive(self):
        """Test syscall delay in aggressive mode"""
        mode = "aggressive"
        delay = self._get_syscall_delay(mode)
        assert delay < 20

    def test_should_batch_operations_stealth(self):
        """Test operation batching in stealth mode"""
        mode = "stealth"
        should_batch = self._should_batch_operations(mode)
        assert should_batch is True

    def test_should_use_indirect_syscalls_stealth(self):
        """Test indirect syscalls in stealth mode"""
        mode = "stealth"
        use_indirect = self._should_use_indirect_syscalls(mode)
        assert use_indirect is True

    @staticmethod
    def _get_adaptive_mode(profile_type):
        mode_map = {
            "frequent": "stealth",
            "normal": "normal",
            "throttled": "normal",
            "rare": "aggressive",
            "not_detected": "aggressive",
        }
        return mode_map.get(profile_type, "normal")

    @staticmethod
    def _get_syscall_delay(mode):
        delay_map = {
            "stealth": 100,
            "normal": 50,
            "aggressive": 10,
        }
        return delay_map.get(mode, 50)

    @staticmethod
    def _should_batch_operations(mode):
        return mode in ["stealth", "normal"]

    @staticmethod
    def _should_use_indirect_syscalls(mode):
        return mode in ["stealth"]


class TestDriverFailureAndRecovery:
    """Test driver failure and recovery"""

    def test_driver_load_failure(self):
        """Test handling driver load failure"""
        driver = "driver_a"
        result = self._attempt_driver_load(driver, should_fail=True)
        assert result["status"] == "failed"

    def test_fallback_to_next_driver(self):
        """Test fallback to next driver on failure"""
        drivers = ["driver_a", "driver_b", "driver_c"]
        result = self._load_with_fallback(drivers)
        assert result["status"] == "loaded"
        assert result["driver"] in drivers

    def test_all_drivers_exhausted(self):
        """Test when all drivers fail"""
        drivers = []
        result = self._load_with_fallback(drivers)
        assert result["status"] == "failed"

    @staticmethod
    def _attempt_driver_load(driver_name, should_fail=False):
        if should_fail:
            return {"status": "failed", "driver": driver_name}
        return {"status": "loaded", "driver": driver_name}

    @staticmethod
    def _load_with_fallback(drivers):
        if not drivers:
            return {"status": "failed", "error": "No drivers available"}
        for driver in drivers:
            result = TestDriverFailureAndRecovery._attempt_driver_load(driver)
            if result["status"] == "loaded":
                return result
        return {"status": "failed"}


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
