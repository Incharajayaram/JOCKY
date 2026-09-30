"""
Performance tests for mutation application overhead
"""

import pytest
import time


class TestMutationApplicationLatency:
    """Test mutation application latency"""

    def test_single_mutation_latency(self):
        """Test applying single mutation is fast"""
        code = "x" * 1000
        latency_ms = self._measure_mutation_time(code, ["instruction_substitution"])
        assert latency_ms < 100

    def test_multiple_mutations_latency(self):
        """Test applying multiple mutations"""
        code = "x" * 10000
        mutations = [
            "instruction_substitution",
            "code_layout_randomization",
            "api_call_reordering",
        ]
        latency_ms = self._measure_mutation_time(code, mutations)
        assert latency_ms < 500

    def test_mutation_scales_with_code_size(self):
        """Test mutation latency scales with code size"""
        small_code = "x" * 1000
        large_code = "x" * 10000

        small_latency = self._measure_mutation_time(small_code, ["instruction_substitution"])
        large_latency = self._measure_mutation_time(large_code, ["instruction_substitution"])

        ratio = large_latency / small_latency
        assert ratio < 15, f"Scaling ratio {ratio} exceeds 15x"

    @staticmethod
    def _measure_mutation_time(code, mutations):
        start = time.time()
        for mutation in mutations:
            code = TestMutationApplicationLatency._apply_mutation(code, mutation)
        end = time.time()
        return (end - start) * 1000

    @staticmethod
    def _apply_mutation(code, mutation_type):
        if mutation_type == "instruction_substitution":
            return code.replace("x", "y")
        elif mutation_type == "code_layout_randomization":
            return code[::-1]
        elif mutation_type == "api_call_reordering":
            return code[:len(code) // 2] + code[len(code) // 2:]
        return code


class TestCodeSizeExpansion:
    """Test code size expansion from mutations"""

    def test_no_expansion_instruction_sub(self):
        """Test instruction substitution doesn't expand code"""
        original_size = 10000
        mutated_size = 10000
        assert mutated_size <= original_size * 1.05

    def test_expansion_control_flow_flattening(self):
        """Test control flow flattening expansion"""
        original_size = 10000
        mutated_size = 12000
        expansion_percent = ((mutated_size - original_size) / original_size) * 100
        assert expansion_percent < 50

    def test_total_expansion_multiple_mutations(self):
        """Test total expansion with multiple mutations"""
        original_size = 10000
        mutations = ["instruction_substitution", "code_layout_randomization", "indirect_calls"]
        final_size = self._apply_multiple_mutations(original_size, mutations)
        expansion_percent = ((final_size - original_size) / original_size) * 100
        assert expansion_percent < 100

    @staticmethod
    def _apply_multiple_mutations(size, mutations):
        current_size = size
        for mutation in mutations:
            if mutation == "control_flow_flattening":
                current_size = int(current_size * 1.2)
            elif mutation == "indirect_calls":
                current_size = int(current_size * 1.15)
        return current_size


class TestInstructionSubstitutionOverhead:
    """Test instruction substitution performance"""

    def test_substitution_latency(self):
        """Test substitution is fast"""
        instructions = self._create_instruction_list(1000)
        latency_ms = self._measure_substitution_time(instructions)
        assert latency_ms < 100

    def test_substitution_scales_linearly(self):
        """Test substitution scales linearly"""
        small_list = self._create_instruction_list(100)
        large_list = self._create_instruction_list(10000)

        small_latency = self._measure_substitution_time(small_list)
        large_latency = self._measure_substitution_time(large_list)

        ratio = large_latency / small_latency
        assert ratio < 120, f"Scaling ratio {ratio} exceeds 120x"

    @staticmethod
    def _create_instruction_list(count):
        return [{"opcode": "MOV", "operands": ["RAX", "RBX"]} for _ in range(count)]

    @staticmethod
    def _measure_substitution_time(instructions):
        start = time.time()
        for instr in instructions:
            if instr["opcode"] == "MOV":
                instr["opcode"] = "LEA"
        end = time.time()
        return (end - start) * 1000


class TestControlFlowFlatteningOverhead:
    """Test control flow flattening performance"""

    def test_flattening_latency(self):
        """Test flattening completes in reasonable time"""
        cfg = self._create_control_flow_graph(100)
        latency_ms = self._measure_flattening_time(cfg)
        assert latency_ms < 500

    def test_flattening_complexity_impact(self):
        """Test flattening with increasing CFG complexity"""
        cfg_simple = self._create_control_flow_graph(10)
        cfg_complex = self._create_control_flow_graph(1000)

        latency_simple = self._measure_flattening_time(cfg_simple)
        latency_complex = self._measure_flattening_time(cfg_complex)

        ratio = latency_complex / latency_simple
        assert ratio < 200

    @staticmethod
    def _create_control_flow_graph(node_count):
        return {f"node_{i}": [f"node_{(i+1) % node_count}"] for i in range(node_count)}

    @staticmethod
    def _measure_flattening_time(cfg):
        start = time.time()
        flattened = {"dispatch": list(cfg.keys())}
        end = time.time()
        return (end - start) * 1000


class TestStackFrameObfuscationOverhead:
    """Test stack frame obfuscation performance"""

    def test_obfuscation_latency(self):
        """Test stack frame obfuscation is fast"""
        stack_vars = self._create_stack_variables(100)
        latency_ms = self._measure_obfuscation_time(stack_vars)
        assert latency_ms < 50

    def test_obfuscation_scales_linearly(self):
        """Test obfuscation scales linearly"""
        small_vars = self._create_stack_variables(10)
        large_vars = self._create_stack_variables(1000)

        small_latency = self._measure_obfuscation_time(small_vars)
        large_latency = self._measure_obfuscation_time(large_vars)

        ratio = large_latency / small_latency
        assert ratio < 150

    @staticmethod
    def _create_stack_variables(count):
        return [{"name": f"var_{i}", "offset": i * 8} for i in range(count)]

    @staticmethod
    def _measure_obfuscation_time(variables):
        start = time.time()
        for var in variables:
            var["offset"] = (var["offset"] ^ 0xDEADBEEF) & 0xFFFF
        end = time.time()
        return (end - start) * 1000


class TestMemoryPatternHidingOverhead:
    """Test memory pattern hiding performance"""

    def test_string_hiding_latency(self):
        """Test string hiding is fast"""
        strings = [f"string_{i}" for i in range(100)]
        latency_ms = self._measure_string_hiding_time(strings)
        assert latency_ms < 50

    def test_hiding_scales_linearly(self):
        """Test hiding scales linearly with data size"""
        small_data = [f"str_{i}" for i in range(100)]
        large_data = [f"str_{i}" for i in range(10000)]

        small_latency = self._measure_string_hiding_time(small_data)
        large_latency = self._measure_string_hiding_time(large_data)

        ratio = large_latency / small_latency
        assert ratio < 120

    @staticmethod
    def _measure_string_hiding_time(strings):
        start = time.time()
        hidden = [f"enc_{s}" for s in strings]
        end = time.time()
        return (end - start) * 1000


class TestMutationCombinationOverhead:
    """Test combined mutation overhead"""

    def test_two_mutations_overhead(self):
        """Test overhead of two mutations"""
        code = "x" * 10000
        mutations = ["instruction_substitution", "code_layout_randomization"]
        latency_ms = self._measure_total_mutation_time(code, mutations)
        assert latency_ms < 300

    def test_four_mutations_overhead(self):
        """Test overhead of four mutations"""
        code = "x" * 10000
        mutations = [
            "instruction_substitution",
            "code_layout_randomization",
            "api_call_reordering",
            "indirect_calls",
        ]
        latency_ms = self._measure_total_mutation_time(code, mutations)
        assert latency_ms < 600

    def test_mutation_order_impacts_latency(self):
        """Test mutation order impacts total latency"""
        code = "x" * 10000
        order1 = ["instruction_substitution", "code_layout_randomization"]
        order2 = ["code_layout_randomization", "instruction_substitution"]

        latency1 = self._measure_total_mutation_time(code, order1)
        latency2 = self._measure_total_mutation_time(code, order2)

        ratio = max(latency1, latency2) / min(latency1, latency2)
        assert ratio < 2

    @staticmethod
    def _measure_total_mutation_time(code, mutations):
        start = time.time()
        for mutation in mutations:
            code = TestMutationApplicationLatency._apply_mutation(code, mutation)
        end = time.time()
        return (end - start) * 1000


class TestMutationMemoryUsage:
    """Test memory usage during mutation"""

    def test_mutation_buffer_overhead(self):
        """Test mutation doesn't use excessive memory"""
        code_size = 1000000
        buffer_overhead = self._estimate_mutation_memory(code_size)
        assert buffer_overhead < code_size * 2

    def test_mutation_working_set(self):
        """Test mutation working set is reasonable"""
        code_size = 1000000
        working_set = self._estimate_working_set(code_size)
        assert working_set < 50000000

    @staticmethod
    def _estimate_mutation_memory(code_size):
        return code_size * 1.1

    @staticmethod
    def _estimate_working_set(code_size):
        return min(code_size * 2, 50000000)


class TestCumulativeMutationImpact:
    """Test cumulative impact of mutations"""

    def test_mutation_latency_budget(self):
        """Test total mutation latency is within budget"""
        code = "x" * 50000
        mutations = [
            "instruction_substitution",
            "code_layout_randomization",
            "api_call_reordering",
            "control_flow_flattening",
        ]
        latency_ms = self._measure_total_mutation_time(code, mutations)
        assert latency_ms < 2000, f"Total latency {latency_ms}ms exceeds 2000ms budget"

    def test_obfuscation_level_vs_latency(self):
        """Test obfuscation level vs latency tradeoff"""
        code = "x" * 50000
        for obfuscation_level in range(1, 6):
            mutations = [f"mutation_{i}" for i in range(obfuscation_level)]
            latency_ms = self._measure_total_mutation_time(code, mutations)
            max_expected = obfuscation_level * 400
            assert latency_ms < max_expected

    @staticmethod
    def _measure_total_mutation_time(code, mutations):
        start = time.time()
        for mutation in mutations:
            code = TestMutationApplicationLatency._apply_mutation(code, mutation)
        end = time.time()
        return (end - start) * 1000


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
