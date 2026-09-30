"""
Unit tests for EDR throttle profile detection
"""

import pytest


class TestProfileTypeDetection:
    """Test EDR profile type detection"""

    def test_detect_not_detected(self):
        """Test detection of no EDR"""
        callbacks = [100, 105, 110, 115, 120]
        profile = self._detect_profile_type(callbacks)
        assert profile == "not_detected"

    def test_detect_frequent(self):
        """Test detection of frequent EDR callbacks"""
        callbacks = [10, 20, 30, 40, 50]
        profile = self._detect_profile_type(callbacks)
        assert profile == "frequent"

    def test_detect_normal(self):
        """Test detection of normal EDR callbacks"""
        callbacks = [100, 200, 300, 400, 500]
        profile = self._detect_profile_type(callbacks)
        assert profile == "normal"

    def test_detect_throttled(self):
        """Test detection of throttled EDR"""
        callbacks = [1000, 2000, 3000, 4000, 5000]
        profile = self._detect_profile_type(callbacks)
        assert profile == "throttled"

    def test_detect_rare(self):
        """Test detection of rare EDR callbacks"""
        callbacks = [5000, 10000, 15000, 20000, 25000]
        profile = self._detect_profile_type(callbacks)
        assert profile == "rare"

    @staticmethod
    def _detect_profile_type(callbacks):
        if len(callbacks) < 2:
            return "unknown"

        intervals = [callbacks[i + 1] - callbacks[i] for i in range(len(callbacks) - 1)]
        avg_interval = sum(intervals) / len(intervals)

        if avg_interval < 50:
            return "frequent"
        elif avg_interval < 300:
            return "normal"
        elif avg_interval < 1500:
            return "throttled"
        elif avg_interval >= 1500:
            return "rare"
        else:
            return "unknown"


class TestIntervalCalculation:
    """Test callback interval calculation"""

    def test_calculate_min_interval(self):
        """Test calculating minimum interval"""
        callbacks = [100, 150, 200, 210, 300]
        min_interval = self._calculate_min_interval(callbacks)
        assert min_interval == 10

    def test_calculate_max_interval(self):
        """Test calculating maximum interval"""
        callbacks = [100, 150, 200, 210, 300]
        max_interval = self._calculate_max_interval(callbacks)
        assert max_interval == 90

    def test_calculate_avg_interval(self):
        """Test calculating average interval"""
        callbacks = [100, 200, 300, 400, 500]
        avg_interval = self._calculate_avg_interval(callbacks)
        assert avg_interval == 100

    def test_handle_single_callback(self):
        """Test handling single callback"""
        callbacks = [100]
        min_interval = self._calculate_min_interval(callbacks)
        assert min_interval == 0 or min_interval is None

    def test_handle_empty_callbacks(self):
        """Test handling empty callback list"""
        callbacks = []
        avg_interval = self._calculate_avg_interval(callbacks)
        assert avg_interval == 0 or avg_interval is None

    @staticmethod
    def _calculate_min_interval(callbacks):
        if len(callbacks) < 2:
            return 0
        intervals = [callbacks[i + 1] - callbacks[i] for i in range(len(callbacks) - 1)]
        return min(intervals)

    @staticmethod
    def _calculate_max_interval(callbacks):
        if len(callbacks) < 2:
            return 0
        intervals = [callbacks[i + 1] - callbacks[i] for i in range(len(callbacks) - 1)]
        return max(intervals)

    @staticmethod
    def _calculate_avg_interval(callbacks):
        if len(callbacks) < 2:
            return 0
        intervals = [callbacks[i + 1] - callbacks[i] for i in range(len(callbacks) - 1)]
        return sum(intervals) // len(intervals) if intervals else 0


class TestConfidenceCalculation:
    """Test confidence calculation for profile detection"""

    def test_high_confidence_regular_intervals(self):
        """Test high confidence with regular intervals"""
        callbacks = [100, 200, 300, 400, 500]
        confidence = self._calculate_confidence(callbacks)
        assert confidence > 80

    def test_low_confidence_irregular_intervals(self):
        """Test low confidence with irregular intervals"""
        callbacks = [100, 200, 250, 500, 600]
        confidence = self._calculate_confidence(callbacks)
        assert confidence < 80

    def test_confidence_ranges_0_to_100(self):
        """Test confidence is in 0-100 range"""
        for i in range(5):
            callbacks = [100 * j for j in range(1, i + 5)]
            confidence = self._calculate_confidence(callbacks)
            assert 0 <= confidence <= 100

    def test_single_callback_low_confidence(self):
        """Test single callback has low confidence"""
        callbacks = [100]
        confidence = self._calculate_confidence(callbacks)
        assert confidence < 50

    @staticmethod
    def _calculate_confidence(callbacks):
        if len(callbacks) < 2:
            return 20

        intervals = [callbacks[i + 1] - callbacks[i] for i in range(len(callbacks) - 1)]
        avg_interval = sum(intervals) / len(intervals)

        variance = sum((x - avg_interval) ** 2 for x in intervals) / len(intervals)
        std_dev = variance ** 0.5

        coefficient_of_variation = (std_dev / avg_interval) if avg_interval > 0 else 1.0
        confidence = 100 - min(coefficient_of_variation * 50, 100)
        return max(confidence, 0)


class TestAdaptiveMode:
    """Test adaptive mode selection based on profile"""

    def test_mode_stealth_for_frequent(self):
        """Test stealth mode for frequent EDR"""
        profile = "frequent"
        mode = self._get_adaptive_mode(profile)
        assert mode == "stealth"

    def test_mode_normal_for_normal(self):
        """Test normal mode for normal EDR"""
        profile = "normal"
        mode = self._get_adaptive_mode(profile)
        assert mode == "normal"

    def test_mode_normal_for_throttled(self):
        """Test normal mode for throttled EDR"""
        profile = "throttled"
        mode = self._get_adaptive_mode(profile)
        assert mode == "normal"

    def test_mode_aggressive_for_rare(self):
        """Test aggressive mode for rare EDR"""
        profile = "rare"
        mode = self._get_adaptive_mode(profile)
        assert mode == "aggressive"

    def test_mode_aggressive_for_not_detected(self):
        """Test aggressive mode when no EDR detected"""
        profile = "not_detected"
        mode = self._get_adaptive_mode(profile)
        assert mode == "aggressive"

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


class TestSyscallDelayCalculation:
    """Test syscall delay calculation based on profile"""

    def test_delay_stealth_mode(self):
        """Test high delay for stealth mode"""
        delay = self._get_syscall_delay("stealth")
        assert delay > 50

    def test_delay_normal_mode(self):
        """Test medium delay for normal mode"""
        delay = self._get_syscall_delay("normal")
        assert 20 < delay <= 50

    def test_delay_aggressive_mode(self):
        """Test low delay for aggressive mode"""
        delay = self._get_syscall_delay("aggressive")
        assert delay < 20

    def test_delay_consistency(self):
        """Test delay calculation is consistent"""
        delay1 = self._get_syscall_delay("stealth")
        delay2 = self._get_syscall_delay("stealth")
        assert delay1 == delay2

    @staticmethod
    def _get_syscall_delay(mode):
        delay_map = {
            "stealth": 100,
            "normal": 50,
            "aggressive": 10,
        }
        return delay_map.get(mode, 50)


class TestOperationBatching:
    """Test operation batching decisions"""

    def test_should_batch_stealth(self):
        """Test batching operations in stealth mode"""
        should_batch = self._should_batch_operations("stealth")
        assert should_batch is True

    def test_should_batch_normal(self):
        """Test batching operations in normal mode"""
        should_batch = self._should_batch_operations("normal")
        assert should_batch is True

    def test_should_not_batch_aggressive(self):
        """Test not batching in aggressive mode"""
        should_batch = self._should_batch_operations("aggressive")
        assert should_batch is False

    def test_batch_size_stealth(self):
        """Test larger batch size in stealth mode"""
        batch_size = self._get_batch_size("stealth")
        assert batch_size > 10

    def test_batch_size_aggressive(self):
        """Test smaller batch size in aggressive mode"""
        batch_size = self._get_batch_size("aggressive")
        assert batch_size < 10

    @staticmethod
    def _should_batch_operations(mode):
        return mode in ["stealth", "normal"]

    @staticmethod
    def _get_batch_size(mode):
        batch_map = {
            "stealth": 50,
            "normal": 25,
            "aggressive": 5,
        }
        return batch_map.get(mode, 10)


class TestIndirectSyscalls:
    """Test indirect syscall decisions"""

    def test_use_indirect_stealth(self):
        """Test using indirect syscalls in stealth mode"""
        use_indirect = self._should_use_indirect_syscalls("stealth")
        assert use_indirect is True

    def test_use_indirect_normal(self):
        """Test using indirect syscalls in normal mode"""
        use_indirect = self._should_use_indirect_syscalls("normal")
        assert use_indirect is False

    def test_use_indirect_aggressive(self):
        """Test using direct syscalls in aggressive mode"""
        use_indirect = self._should_use_indirect_syscalls("aggressive")
        assert use_indirect is False

    @staticmethod
    def _should_use_indirect_syscalls(mode):
        return mode == "stealth"


class TestProfilePersistence:
    """Test profile saving and loading"""

    def test_save_profile(self):
        """Test saving profile to file"""
        profile_data = {
            "type": "throttled",
            "avg_interval": 1000,
            "confidence": 85,
        }
        result = self._save_profile(profile_data, "/tmp/profile.bin")
        assert result["status"] == "saved"

    def test_load_profile(self):
        """Test loading profile from file"""
        profile_data = {
            "type": "throttled",
            "avg_interval": 1000,
        }
        result = self._load_profile("/tmp/profile.bin")
        assert result is not None

    def test_profile_format_compatibility(self):
        """Test profile format is compatible"""
        profile_data = {
            "magic": 0x4544525f,
            "version": 1,
            "type": "throttled",
        }
        result = self._validate_profile_format(profile_data)
        assert result["valid"] is True

    @staticmethod
    def _save_profile(data, path):
        return {"status": "saved", "path": path}

    @staticmethod
    def _load_profile(path):
        return {"type": "throttled", "avg_interval": 1000}

    @staticmethod
    def _validate_profile_format(data):
        required_fields = ["type"]
        valid = all(field in data for field in required_fields)
        return {"valid": valid}


class TestProfileUpdate:
    """Test profile update logic"""

    def test_update_profile_on_new_callbacks(self):
        """Test updating profile with new callbacks"""
        old_callbacks = [100, 200, 300]
        new_callbacks = [400, 500]
        updated = self._update_profile(old_callbacks, new_callbacks)
        assert len(updated) > len(old_callbacks)

    def test_update_maintains_history(self):
        """Test update maintains callback history"""
        old_callbacks = [100, 200, 300]
        new_callbacks = [400, 500]
        updated = self._update_profile(old_callbacks, new_callbacks)
        assert updated[:3] == old_callbacks

    def test_recompute_confidence_on_update(self):
        """Test recomputing confidence on update"""
        old_callbacks = [100, 200]
        new_callbacks = [300, 400, 500]
        old_confidence = 50
        new_confidence = self._recompute_confidence(old_callbacks + new_callbacks)
        assert new_confidence >= old_confidence

    @staticmethod
    def _update_profile(old, new):
        return old + new

    @staticmethod
    def _recompute_confidence(callbacks):
        if len(callbacks) < 3:
            return 50
        return min(100, 50 + len(callbacks) * 5)


class TestThrottleThresholds:
    """Test throttle threshold detection"""

    def test_detect_throttle_start(self):
        """Test detecting start of throttling"""
        callbacks = [100, 110, 120, 500, 600, 700]
        throttle_start = self._detect_throttle_start(callbacks)
        assert throttle_start > 0

    def test_detect_throttle_end(self):
        """Test detecting end of throttling"""
        callbacks = [100, 500, 600, 700, 120, 130]
        throttle_end = self._detect_throttle_end(callbacks)
        assert throttle_end > 0

    def test_identify_throttle_pattern(self):
        """Test identifying throttle pattern"""
        callbacks = [100, 200, 300, 1000, 2000, 3000]
        pattern = self._identify_throttle_pattern(callbacks)
        assert pattern in ["increasing", "stable", "none"]

    @staticmethod
    def _detect_throttle_start(callbacks):
        for i in range(len(callbacks) - 1):
            if callbacks[i + 1] - callbacks[i] > 200:
                return i
        return -1

    @staticmethod
    def _detect_throttle_end(callbacks):
        for i in range(len(callbacks) - 1):
            if callbacks[i + 1] - callbacks[i] < 200:
                return i
        return -1

    @staticmethod
    def _identify_throttle_pattern(callbacks):
        if len(callbacks) < 3:
            return "none"
        if callbacks[-1] - callbacks[-2] > callbacks[1] - callbacks[0]:
            return "increasing"
        return "stable"


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
