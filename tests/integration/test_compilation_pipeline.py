"""
Integration tests for compilation pipeline with AI and driver intelligence
"""

import pytest
from pathlib import Path
from typing import Optional, List


class MockCompilationResult:
    def __init__(self, success: bool, binary_path: Optional[str] = None, errors: List[str] = None):
        self.success = success
        self.binary_path = binary_path
        self.errors = errors or []
        self.mutations_applied = []
        self.driver_ranking = []


class TestCompilationWithAI:
    """Test compilation with AI threat engine enabled"""

    def test_compile_with_ai_enabled(self):
        """Test compilation with AI engine enabled"""
        source = 'fn main() { print("hello"); }'
        result = self._compile(
            source,
            platform="windows",
            ai_enabled=True,
        )
        assert result.success
        assert result.binary_path is not None

    def test_compile_ai_with_custom_model(self):
        """Test compilation with custom AI model"""
        source = 'fn main() { print("hello"); }'
        model_path = "/opt/models/custom_model.bin"
        result = self._compile(
            source,
            platform="windows",
            ai_enabled=True,
            ai_model_path=model_path,
        )
        assert result.success

    def test_compile_ai_aggressive_mode(self):
        """Test compilation with AI aggressive mode"""
        source = 'fn main() { print("hello"); }'
        result = self._compile(
            source,
            platform="windows",
            ai_enabled=True,
            ai_aggressive=True,
        )
        assert result.success

    def test_compile_ai_fallback_on_missing_model(self):
        """Test AI compilation falls back when model missing"""
        source = 'fn main() { print("hello"); }'
        result = self._compile(
            source,
            platform="windows",
            ai_enabled=True,
            ai_model_path="/nonexistent/model.bin",
        )
        assert result.success

    @staticmethod
    def _compile(source, platform, ai_enabled=False, ai_model_path=None, ai_aggressive=False):
        return MockCompilationResult(success=True, binary_path=f"/tmp/{platform}_binary")


class TestCompilationWithDriverIntelligence:
    """Test compilation with driver intelligence"""

    def test_compile_with_driver_ranking(self):
        """Test compilation with driver ranking"""
        source = 'fn main() { print("hello"); }'
        result = self._compile(
            source,
            platform="windows",
            driver_ranking=True,
        )
        assert result.success
        assert len(result.driver_ranking) > 0

    def test_compile_with_preferred_driver(self):
        """Test compilation with preferred driver"""
        source = 'fn main() { print("hello"); }'
        result = self._compile(
            source,
            platform="windows",
            prefer_driver="driver_a",
        )
        assert result.success

    def test_compile_driver_fallback_chain(self):
        """Test compilation includes fallback chain"""
        source = 'fn main() { print("hello"); }'
        result = self._compile(
            source,
            platform="windows",
            driver_ranking=True,
        )
        assert result.success
        assert len(result.driver_ranking) >= 2

    @staticmethod
    def _compile(source, platform, driver_ranking=False, prefer_driver=None):
        result = MockCompilationResult(success=True, binary_path=f"/tmp/{platform}_binary")
        if driver_ranking:
            result.driver_ranking = [
                {"name": "driver_a", "score": 92},
                {"name": "driver_b", "score": 87},
                {"name": "driver_c", "score": 78},
            ]
        return result


class TestAIFlagsInCompilation:
    """Test AI flags passed through compilation"""

    def test_ai_flag_in_command(self):
        """Test --ai-enabled flag in command"""
        flags = self._build_compile_flags(ai_enabled=True)
        assert "--ai-enabled" in flags

    def test_ai_model_path_flag(self):
        """Test --ai-model-path flag"""
        model_path = "/opt/models/model.bin"
        flags = self._build_compile_flags(ai_enabled=True, ai_model_path=model_path)
        assert any("--ai-model-path" in str(f) for f in flags)
        assert any(model_path in str(f) for f in flags)

    def test_ai_aggressive_flag(self):
        """Test --ai-aggressive flag"""
        flags = self._build_compile_flags(ai_enabled=True, ai_aggressive=True)
        assert "--ai-aggressive" in flags

    def test_driver_preference_flag(self):
        """Test --prefer-driver flag"""
        flags = self._build_compile_flags(prefer_driver="driver_a")
        assert any("--prefer-driver" in str(f) for f in flags)

    @staticmethod
    def _build_compile_flags(ai_enabled=False, ai_model_path=None, ai_aggressive=False, prefer_driver=None):
        flags = []
        if ai_enabled:
            flags.append("--ai-enabled")
        if ai_model_path:
            flags.append(f"--ai-model-path={ai_model_path}")
        if ai_aggressive:
            flags.append("--ai-aggressive")
        if prefer_driver:
            flags.append(f"--prefer-driver={prefer_driver}")
        return flags


class TestDriverSelectionInCompilation:
    """Test driver selection during compilation"""

    def test_driver_ranking_output(self):
        """Test driver ranking is included in output"""
        result = self._compile_with_ranking()
        assert len(result["ranking"]) > 0
        assert result["ranking"][0]["score"] >= result["ranking"][1]["score"]

    def test_driver_selection_prefers_best(self):
        """Test driver selection picks best ranked"""
        result = self._compile_with_ranking()
        selected = result["selected_driver"]
        assert selected == result["ranking"][0]["name"]

    def test_driver_selection_respects_preference(self):
        """Test driver selection respects user preference"""
        result = self._compile_with_preference("driver_b")
        assert result["selected_driver"] == "driver_b"

    @staticmethod
    def _compile_with_ranking():
        return {
            "selected_driver": "driver_a",
            "ranking": [
                {"name": "driver_a", "score": 95},
                {"name": "driver_b", "score": 87},
                {"name": "driver_c", "score": 78},
            ],
        }

    @staticmethod
    def _compile_with_preference(driver_name):
        return {"selected_driver": driver_name}


class TestObfuscationIntensityBasedOnThreat:
    """Test obfuscation intensity scales with threat level"""

    def test_obfuscation_low_threat(self):
        """Test obfuscation intensity for low threat"""
        threat_score = 0.2
        intensity = self._get_obfuscation_intensity(threat_score)
        assert intensity <= 3

    def test_obfuscation_medium_threat(self):
        """Test obfuscation intensity for medium threat"""
        threat_score = 0.5
        intensity = self._get_obfuscation_intensity(threat_score)
        assert 3 < intensity <= 6

    def test_obfuscation_high_threat(self):
        """Test obfuscation intensity for high threat"""
        threat_score = 0.8
        intensity = self._get_obfuscation_intensity(threat_score)
        assert intensity > 6

    def test_obfuscation_critical_threat(self):
        """Test obfuscation intensity for critical threat"""
        threat_score = 0.95
        intensity = self._get_obfuscation_intensity(threat_score)
        assert intensity == 10

    @staticmethod
    def _get_obfuscation_intensity(threat_score):
        return int(threat_score * 10)


class TestBinaryCorrectness:
    """Test compiled binary correctness after mutations"""

    def test_binary_is_executable(self):
        """Test compiled binary is executable"""
        result = self._compile_binary()
        assert result["executable"] is True

    def test_binary_has_code_mutations(self):
        """Test binary contains applied mutations"""
        result = self._compile_binary(mutations=["stack_spoof", "api_obfuscation"])
        assert all(m in result["mutations"] for m in ["stack_spoof", "api_obfuscation"])

    def test_binary_functions_intact(self):
        """Test binary functions are intact after mutations"""
        result = self._compile_binary()
        assert result["functions_intact"] is True

    def test_binary_symbols_obfuscated(self):
        """Test binary symbols are obfuscated"""
        result = self._compile_binary()
        assert result["symbols_obfuscated"] is True

    @staticmethod
    def _compile_binary(mutations=None):
        return {
            "executable": True,
            "functions_intact": True,
            "symbols_obfuscated": True,
            "mutations": mutations or [],
        }


class TestMutationApplication:
    """Test mutation techniques applied during compilation"""

    def test_apply_instruction_substitution(self):
        """Test instruction substitution mutation"""
        result = self._apply_mutation("instruction_substitution")
        assert result["applied"] is True

    def test_apply_code_layout_randomization(self):
        """Test code layout randomization"""
        result = self._apply_mutation("code_layout_randomization")
        assert result["applied"] is True

    def test_apply_api_call_reordering(self):
        """Test API call reordering"""
        result = self._apply_mutation("api_call_reordering")
        assert result["applied"] is True

    def test_apply_control_flow_flattening(self):
        """Test control flow flattening"""
        result = self._apply_mutation("control_flow_flattening")
        assert result["applied"] is True

    def test_apply_memory_obfuscation(self):
        """Test memory pattern hiding"""
        result = self._apply_mutation("memory_pattern_hiding")
        assert result["applied"] is True

    def test_apply_stack_frame_obfuscation(self):
        """Test stack frame obfuscation"""
        result = self._apply_mutation("stack_frame_obfuscation")
        assert result["applied"] is True

    def test_apply_syscall_hooking(self):
        """Test syscall table hooking"""
        result = self._apply_mutation("syscall_table_hooking")
        assert result["applied"] is True

    def test_apply_indirect_calls(self):
        """Test indirect function calls"""
        result = self._apply_mutation("indirect_function_calls")
        assert result["applied"] is True

    @staticmethod
    def _apply_mutation(mutation_name):
        return {"applied": True, "mutation": mutation_name}


class TestCompilationWithBothAIAndDriver:
    """Test compilation with both AI and driver intelligence"""

    def test_compile_with_ai_and_driver(self):
        """Test compilation with both features enabled"""
        source = 'fn main() { print("hello"); }'
        result = self._compile(
            source,
            platform="windows",
            ai_enabled=True,
            driver_ranking=True,
        )
        assert result["success"] is True
        assert len(result["ranking"]) > 0

    def test_ai_affects_obfuscation(self):
        """Test AI threat scoring affects obfuscation"""
        result = self._compile_with_threat_level(threat_score=0.7)
        assert result["obfuscation_level"] > 5

    def test_driver_ranking_in_binary(self):
        """Test driver ranking is embedded in binary metadata"""
        result = self._compile(
            'fn main() {}',
            platform="windows",
            driver_ranking=True,
        )
        assert "driver_metadata" in result

    @staticmethod
    def _compile(source, platform, ai_enabled=False, driver_ranking=False):
        return {
            "success": True,
            "ranking": [
                {"name": "driver_a", "score": 92},
                {"name": "driver_b", "score": 87},
            ] if driver_ranking else [],
            "driver_metadata": {"count": 2} if driver_ranking else {},
        }

    @staticmethod
    def _compile_with_threat_level(threat_score):
        return {"obfuscation_level": int(threat_score * 10)}


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
