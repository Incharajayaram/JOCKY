"""
Unit tests for BYOVD driver scoring calculation
"""

import pytest


class DriverTestData:
    @staticmethod
    def create_driver(
        name="test_driver",
        is_signed=True,
        is_microsoft_blocked=False,
        is_on_edr_list=False,
        sig_revoked=False,
        yara_matches=0,
        release_age_days=365,
    ):
        return {
            "name": name,
            "is_signed": is_signed,
            "is_microsoft_blocked": is_microsoft_blocked,
            "is_on_edr_list": is_on_edr_list,
            "sig_revoked": sig_revoked,
            "yara_matches": yara_matches,
            "release_age_days": release_age_days,
        }


class TestEvasionScoreCalculation:
    """Test evasion score calculation"""

    def test_score_clean_driver(self):
        """Test scoring clean, modern driver"""
        driver = DriverTestData.create_driver(
            is_signed=True,
            is_microsoft_blocked=False,
            is_on_edr_list=False,
            sig_revoked=False,
            yara_matches=0,
        )
        score = self._calculate_evasion_score(driver)
        assert score > 80
        assert score <= 100

    def test_score_blocked_driver(self):
        """Test scoring Microsoft-blocked driver"""
        driver = DriverTestData.create_driver(
            is_microsoft_blocked=True,
            is_on_edr_list=True,
            sig_revoked=True,
        )
        score = self._calculate_evasion_score(driver)
        assert score < 50

    def test_score_edr_listed_driver(self):
        """Test scoring EDR-listed driver"""
        driver = DriverTestData.create_driver(is_on_edr_list=True)
        score = self._calculate_evasion_score(driver)
        assert score < 70

    def test_score_yara_matches_impact(self):
        """Test yara matches impact on score"""
        driver_clean = DriverTestData.create_driver(yara_matches=0)
        driver_matched = DriverTestData.create_driver(yara_matches=3)
        score_clean = self._calculate_evasion_score(driver_clean)
        score_matched = self._calculate_evasion_score(driver_matched)
        assert score_clean > score_matched

    def test_score_unsigned_driver(self):
        """Test scoring unsigned driver"""
        driver = DriverTestData.create_driver(is_signed=False)
        score = self._calculate_evasion_score(driver)
        assert score < 75

    @staticmethod
    def _calculate_evasion_score(driver):
        score = 100
        if not driver["is_signed"]:
            score -= 20
        if driver["is_microsoft_blocked"]:
            score -= 35
        if driver["is_on_edr_list"]:
            score -= 25
        if driver["sig_revoked"]:
            score -= 20
        if driver["yara_matches"] > 0:
            score -= min(driver["yara_matches"] * 10, 40)
        return max(score, 0)


class TestPrevalenceScoreCalculation:
    """Test prevalence score calculation"""

    def test_score_new_driver(self):
        """Test scoring newly released driver"""
        driver = DriverTestData.create_driver(release_age_days=30)
        score = self._calculate_prevalence_score(driver)
        assert score > 70

    def test_score_old_driver(self):
        """Test scoring old driver"""
        driver = DriverTestData.create_driver(release_age_days=1825)
        score = self._calculate_prevalence_score(driver)
        assert score < 60

    def test_score_very_old_driver(self):
        """Test scoring very old driver"""
        driver = DriverTestData.create_driver(release_age_days=3650)
        score = self._calculate_prevalence_score(driver)
        assert score < 40

    @staticmethod
    def _calculate_prevalence_score(driver):
        score = 100
        age_days = driver["release_age_days"]
        if age_days < 90:
            score += 20
        elif age_days < 365:
            score += 10
        elif age_days > 1825:
            score -= 30
        return min(max(score, 0), 100)


class TestCapabilityScoreCalculation:
    """Test capability score calculation"""

    def test_score_full_capability_match(self):
        """Test scoring driver with full capabilities"""
        driver_caps = ["read_kernel", "write_kernel", "execute_code"]
        required_caps = ["read_kernel", "write_kernel", "execute_code"]
        score = self._calculate_capability_score(driver_caps, required_caps)
        assert score == 100

    def test_score_partial_capability_match(self):
        """Test scoring driver with partial capabilities"""
        driver_caps = ["read_kernel", "write_kernel"]
        required_caps = ["read_kernel", "write_kernel", "execute_code"]
        score = self._calculate_capability_score(driver_caps, required_caps)
        assert 50 < score < 100

    def test_score_no_capability_match(self):
        """Test scoring driver with no matching capabilities"""
        driver_caps = ["read_file"]
        required_caps = ["read_kernel", "write_kernel"]
        score = self._calculate_capability_score(driver_caps, required_caps)
        assert score == 0

    def test_score_empty_capabilities(self):
        """Test scoring driver with no capabilities"""
        driver_caps = []
        required_caps = ["read_kernel"]
        score = self._calculate_capability_score(driver_caps, required_caps)
        assert score == 0

    def test_score_extra_capabilities_bonus(self):
        """Test bonus for extra capabilities"""
        driver_caps = ["read_kernel", "write_kernel", "execute_code", "bypass_wdfilter"]
        required_caps = ["read_kernel", "write_kernel"]
        score = self._calculate_capability_score(driver_caps, required_caps)
        assert score > 100 or score == 100

    @staticmethod
    def _calculate_capability_score(driver_caps, required_caps):
        if not required_caps:
            return 100
        matched = sum(1 for cap in required_caps if cap in driver_caps)
        return int((matched / len(required_caps)) * 100)


class TestBlocklistScoreCalculation:
    """Test blocklist impact score calculation"""

    def test_score_clean_driver(self):
        """Test scoring clean driver not on blocklists"""
        driver = DriverTestData.create_driver(
            is_microsoft_blocked=False,
            is_on_edr_list=False,
            sig_revoked=False,
            yara_matches=0,
        )
        score = self._calculate_blocklist_score(driver)
        assert score > 80

    def test_score_on_multiple_lists(self):
        """Test scoring driver on multiple blocklists"""
        driver = DriverTestData.create_driver(
            is_microsoft_blocked=True,
            is_on_edr_list=True,
            sig_revoked=True,
        )
        score = self._calculate_blocklist_score(driver)
        assert score < 40

    def test_score_yara_detection_penalty(self):
        """Test yara detection penalty"""
        driver = DriverTestData.create_driver(yara_matches=5)
        score = self._calculate_blocklist_score(driver)
        assert score < 75

    @staticmethod
    def _calculate_blocklist_score(driver):
        score = 100
        if driver["is_microsoft_blocked"]:
            score -= 40
        if driver["is_on_edr_list"]:
            score -= 35
        if driver["sig_revoked"]:
            score -= 30
        if driver["yara_matches"] > 0:
            score -= min(driver["yara_matches"] * 8, 40)
        return max(score, 0)


class TestCompositeScoreCalculation:
    """Test composite score calculation"""

    def test_weighted_score_calculation(self):
        """Test weighted composite score"""
        driver = DriverTestData.create_driver()
        evasion = 85
        prevalence = 75
        capability = 90
        blocklist = 80
        composite = self._calculate_composite_score(evasion, prevalence, capability, blocklist)
        assert 0 <= composite <= 100

    def test_evasion_weight_impact(self):
        """Test evasion score has significant weight"""
        score_high_evasion = self._calculate_composite_score(95, 50, 50, 50)
        score_low_evasion = self._calculate_composite_score(20, 50, 50, 50)
        assert score_high_evasion > score_low_evasion

    def test_blocklist_weight_impact(self):
        """Test blocklist score has impact"""
        score_no_block = self._calculate_composite_score(80, 80, 80, 100)
        score_blocked = self._calculate_composite_score(80, 80, 80, 20)
        assert score_no_block > score_blocked

    def test_all_perfect_scores(self):
        """Test all perfect scores"""
        composite = self._calculate_composite_score(100, 100, 100, 100)
        assert composite == 100

    def test_all_zero_scores(self):
        """Test all zero scores"""
        composite = self._calculate_composite_score(0, 0, 0, 0)
        assert composite == 0

    @staticmethod
    def _calculate_composite_score(evasion, prevalence, capability, blocklist):
        weights = {
            "evasion": 0.40,
            "prevalence": 0.20,
            "capability": 0.20,
            "blocklist": 0.20,
        }
        return int(
            evasion * weights["evasion"]
            + prevalence * weights["prevalence"]
            + capability * weights["capability"]
            + blocklist * weights["blocklist"]
        )


class TestDriverRanking:
    """Test driver ranking logic"""

    def test_ranking_sorts_by_score(self):
        """Test ranking sorts drivers by composite score"""
        drivers = [
            {"name": "driver_a", "score": 75},
            {"name": "driver_b", "score": 95},
            {"name": "driver_c", "score": 60},
        ]
        ranked = self._rank_drivers(drivers)
        scores = [d["score"] for d in ranked]
        assert scores == sorted(scores, reverse=True)

    def test_ranking_filters_low_score(self):
        """Test ranking filters drivers below threshold"""
        drivers = [
            {"name": "driver_a", "score": 95},
            {"name": "driver_b", "score": 15},
            {"name": "driver_c", "score": 85},
        ]
        ranked = self._rank_drivers(drivers, min_score=50)
        assert all(d["score"] >= 50 for d in ranked)

    def test_ranking_respects_limit(self):
        """Test ranking respects result limit"""
        drivers = [
            {"name": f"driver_{i}", "score": 100 - i * 5}
            for i in range(10)
        ]
        ranked = self._rank_drivers(drivers, limit=3)
        assert len(ranked) == 3

    @staticmethod
    def _rank_drivers(drivers, min_score=0, limit=None):
        filtered = [d for d in drivers if d["score"] >= min_score]
        sorted_drivers = sorted(filtered, key=lambda d: d["score"], reverse=True)
        return sorted_drivers[:limit] if limit else sorted_drivers


class TestFallbackChainGeneration:
    """Test fallback chain generation"""

    def test_generate_fallback_chain(self):
        """Test generating fallback chain"""
        drivers = [
            {"name": "driver_a", "score": 95},
            {"name": "driver_b", "score": 87},
            {"name": "driver_c", "score": 78},
            {"name": "driver_d", "score": 65},
        ]
        chain = self._generate_fallback_chain(drivers, chain_size=3)
        assert len(chain) == 3
        assert chain == ["driver_a", "driver_b", "driver_c"]

    def test_fallback_chain_respects_size(self):
        """Test fallback chain respects requested size"""
        drivers = [{"name": f"driver_{i}", "score": 100 - i} for i in range(5)]
        chain = self._generate_fallback_chain(drivers, chain_size=2)
        assert len(chain) == 2

    def test_fallback_chain_smaller_than_available(self):
        """Test fallback chain when fewer drivers available"""
        drivers = [{"name": "driver_a", "score": 95}]
        chain = self._generate_fallback_chain(drivers, chain_size=5)
        assert len(chain) == 1

    @staticmethod
    def _generate_fallback_chain(drivers, chain_size):
        ranked = sorted(drivers, key=lambda d: d["score"], reverse=True)
        return [d["name"] for d in ranked[:chain_size]]


class TestDriverSelectionLogic:
    """Test driver selection logic"""

    def test_select_highest_score(self):
        """Test selecting highest scored driver"""
        drivers = [
            {"name": "driver_a", "score": 95},
            {"name": "driver_b", "score": 87},
            {"name": "driver_c", "score": 78},
        ]
        selected = self._select_driver(drivers)
        assert selected == "driver_a"

    def test_select_with_preference(self):
        """Test selecting preferred driver"""
        drivers = [
            {"name": "driver_a", "score": 95},
            {"name": "driver_b", "score": 87},
        ]
        selected = self._select_driver(drivers, prefer="driver_b")
        assert selected == "driver_b"

    def test_select_prefers_higher_score_if_available(self):
        """Test preferring higher score when preferred unavailable"""
        drivers = [
            {"name": "driver_a", "score": 95},
            {"name": "driver_b", "score": 87},
        ]
        selected = self._select_driver(drivers, prefer="driver_c")
        assert selected == "driver_a"

    @staticmethod
    def _select_driver(drivers, prefer=None):
        if prefer:
            for driver in drivers:
                if driver["name"] == prefer:
                    return driver["name"]
        return max(drivers, key=lambda d: d["score"])["name"]


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
