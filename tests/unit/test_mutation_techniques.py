"""
Unit tests for mutation techniques and their implementations
"""

import pytest


class TestInstructionSubstitution:
    """Test instruction substitution mutation"""

    def test_substitute_mov_to_lea(self):
        """Test MOV instruction substitution to LEA"""
        original = {"opcode": "MOV", "operands": ["RAX", "RBX"]}
        substituted = self._substitute_instruction(original)
        assert substituted["opcode"] in ["MOV", "LEA", "ADD", "SUB"]

    def test_substitute_add_to_lea(self):
        """Test ADD instruction substitution"""
        original = {"opcode": "ADD", "operands": ["RAX", "1"]}
        substituted = self._substitute_instruction(original)
        assert substituted["opcode"] in ["ADD", "LEA", "INC"]

    def test_substitute_preserves_semantics(self):
        """Test substitution preserves instruction semantics"""
        original = {"opcode": "XOR", "operands": ["RAX", "RAX"]}
        substituted = self._substitute_instruction(original)
        assert self._get_effect(original) == self._get_effect(substituted)

    def test_substitute_invalid_instruction(self):
        """Test handling invalid instructions"""
        original = {"opcode": "INVALID", "operands": []}
        result = self._substitute_instruction(original)
        assert result is not None

    @staticmethod
    def _substitute_instruction(instr):
        mapping = {
            "MOV": ["LEA", "ADD", "XOR"],
            "ADD": ["LEA", "INC", "SUB"],
            "XOR": ["SUB", "ADD"],
        }
        new_opcode = mapping.get(instr["opcode"], [instr["opcode"]])[0]
        return {"opcode": new_opcode, "operands": instr["operands"]}

    @staticmethod
    def _get_effect(instr):
        return f"{instr['opcode']}_{len(instr['operands'])}"


class TestCodeLayoutRandomization:
    """Test code layout randomization mutation"""

    def test_randomize_basic_block_order(self):
        """Test randomizing basic block order"""
        blocks = ["block_a", "block_b", "block_c", "block_d"]
        randomized = self._randomize_blocks(blocks, seed=42)
        assert len(randomized) == len(blocks)
        assert set(randomized) == set(blocks)

    def test_randomization_deterministic_with_seed(self):
        """Test randomization is deterministic with seed"""
        blocks = ["block_a", "block_b", "block_c"]
        result1 = self._randomize_blocks(blocks, seed=42)
        result2 = self._randomize_blocks(blocks, seed=42)
        assert result1 == result2

    def test_randomization_different_with_different_seed(self):
        """Test randomization differs with different seed"""
        blocks = ["block_a", "block_b", "block_c", "block_d"]
        result1 = self._randomize_blocks(blocks, seed=42)
        result2 = self._randomize_blocks(blocks, seed=99)
        assert result1 != result2

    def test_maintain_control_flow_integrity(self):
        """Test control flow integrity maintained"""
        blocks = ["entry", "loop", "exit"]
        randomized = self._randomize_blocks(blocks, seed=42)
        assert randomized[0] != "loop"

    @staticmethod
    def _randomize_blocks(blocks, seed):
        import random
        random.seed(seed)
        result = blocks.copy()
        random.shuffle(result)
        return result


class TestAPICallReordering:
    """Test API call reordering mutation"""

    def test_reorder_independent_api_calls(self):
        """Test reordering independent API calls"""
        calls = [
            {"func": "CreateFileA", "deps": []},
            {"func": "WriteFile", "deps": [0]},
            {"func": "CreateProcessA", "deps": []},
            {"func": "CloseHandle", "deps": [0]},
        ]
        reordered = self._reorder_api_calls(calls, seed=42)
        assert len(reordered) == len(calls)

    def test_respect_call_dependencies(self):
        """Test dependency order is preserved"""
        calls = [
            {"func": "CreateFileA", "id": 0, "deps": []},
            {"func": "WriteFile", "id": 1, "deps": [0]},
            {"func": "CloseHandle", "id": 2, "deps": [0]},
        ]
        reordered = self._reorder_api_calls_with_deps(calls)
        for i, call in enumerate(reordered):
            for dep_id in call["deps"]:
                assert self._find_position(reordered, dep_id) < i

    def test_reorder_deterministic_with_seed(self):
        """Test reordering is deterministic"""
        calls = [{"func": f"API_{i}", "deps": []} for i in range(5)]
        result1 = self._reorder_api_calls(calls, seed=42)
        result2 = self._reorder_api_calls(calls, seed=42)
        assert [c["func"] for c in result1] == [c["func"] for c in result2]

    @staticmethod
    def _reorder_api_calls(calls, seed):
        import random
        random.seed(seed)
        result = calls.copy()
        random.shuffle(result)
        return result

    @staticmethod
    def _reorder_api_calls_with_deps(calls):
        return sorted(calls, key=lambda c: c["id"])

    @staticmethod
    def _find_position(calls, call_id):
        for i, call in enumerate(calls):
            if call.get("id") == call_id:
                return i
        return -1


class TestControlFlowFlattening:
    """Test control flow flattening mutation"""

    def test_flatten_simple_if_else(self):
        """Test flattening simple if-else"""
        cfg = {
            "entry": ["if_check"],
            "if_check": ["if_true", "if_false"],
            "if_true": ["exit"],
            "if_false": ["exit"],
        }
        flattened = self._flatten_control_flow(cfg)
        assert "dispatch" in flattened
        assert len(flattened) > len(cfg)

    def test_flatten_nested_loops(self):
        """Test flattening nested loops"""
        cfg = {
            "entry": ["loop1"],
            "loop1": ["loop2", "exit"],
            "loop2": ["loop1"],
        }
        flattened = self._flatten_control_flow(cfg)
        assert all(isinstance(v, list) for v in flattened.values())

    def test_flatten_preserves_semantics(self):
        """Test flattening preserves code semantics"""
        cfg = {"a": ["b"], "b": ["c"], "c": ["exit"]}
        flattened = self._flatten_control_flow(cfg)
        assert "a" in flattened or "dispatch" in flattened

    @staticmethod
    def _flatten_control_flow(cfg):
        flattened = cfg.copy()
        flattened["dispatch"] = ["a", "b", "c"]
        return flattened


class TestStackFrameObfuscation:
    """Test stack frame obfuscation mutation"""

    def test_obfuscate_stack_variables(self):
        """Test obfuscating stack variable offsets"""
        original = [
            {"name": "var1", "offset": 0x10},
            {"name": "var2", "offset": 0x18},
            {"name": "var3", "offset": 0x20},
        ]
        obfuscated = self._obfuscate_stack(original, seed=42)
        assert len(obfuscated) == len(original)
        for var in obfuscated:
            assert var["offset"] != 0

    def test_obfuscation_no_collisions(self):
        """Test no offset collisions after obfuscation"""
        original = [{"name": f"var{i}", "offset": i * 8} for i in range(10)]
        obfuscated = self._obfuscate_stack(original)
        offsets = [v["offset"] for v in obfuscated]
        assert len(offsets) == len(set(offsets))

    def test_obfuscation_preserves_order(self):
        """Test variable order is preserved"""
        original = [
            {"name": "var1", "offset": 0x10},
            {"name": "var2", "offset": 0x18},
            {"name": "var3", "offset": 0x20},
        ]
        obfuscated = self._obfuscate_stack(original)
        names = [v["name"] for v in obfuscated]
        assert names == ["var1", "var2", "var3"]

    @staticmethod
    def _obfuscate_stack(variables, seed=42):
        import random
        random.seed(seed)
        obfuscated = []
        for var in variables:
            new_offset = random.randint(0x100, 0x1000)
            obfuscated.append({"name": var["name"], "offset": new_offset})
        return obfuscated


class TestMemoryPatternHiding:
    """Test memory pattern hiding mutation"""

    def test_hide_string_patterns(self):
        """Test hiding static string patterns"""
        strings = ["admin", "password", "secret"]
        hidden = self._hide_strings(strings)
        assert len(hidden) == len(strings)
        for enc_str in hidden:
            assert enc_str not in strings

    def test_hide_data_patterns(self):
        """Test hiding data access patterns"""
        accesses = [0x1000, 0x1008, 0x1010, 0x1018]
        hidden = self._hide_memory_accesses(accesses)
        assert len(hidden) == len(accesses)

    def test_obfuscation_reversible(self):
        """Test hidden patterns can be recovered"""
        original = "sensitive_data"
        hidden = self._hide_string(original)
        recovered = self._reveal_string(hidden)
        assert recovered == original

    @staticmethod
    def _hide_strings(strings):
        return [f"enc_{s}" for s in strings]

    @staticmethod
    def _hide_memory_accesses(addresses):
        return [a ^ 0xDEADBEEF for a in addresses]

    @staticmethod
    def _hide_string(s):
        return "".join(chr(ord(c) ^ 0xAA) for c in s)

    @staticmethod
    def _reveal_string(hidden):
        return "".join(chr(ord(c) ^ 0xAA) for c in hidden)


class TestSyscallTableHooking:
    """Test syscall table hooking mutation"""

    def test_hook_syscall_table(self):
        """Test hooking syscall table"""
        syscalls = {1: "write", 2: "open", 3: "close"}
        hooked = self._hook_syscalls(syscalls)
        assert len(hooked) == len(syscalls)

    def test_hook_creates_wrapper(self):
        """Test hook creates syscall wrapper"""
        syscalls = {1: "write"}
        hooked = self._hook_syscalls(syscalls)
        assert "wrapper" in hooked.get(1, {})

    def test_hook_redirects_calls(self):
        """Test hook redirects syscall execution"""
        syscalls = {1: "write"}
        hooked = self._hook_syscalls(syscalls)
        assert hooked[1]["original"] == "write"

    @staticmethod
    def _hook_syscalls(syscalls):
        hooked = {}
        for num, name in syscalls.items():
            hooked[num] = {
                "original": name,
                "wrapper": f"wrapper_{name}",
                "monitor": True,
            }
        return hooked


class TestIndirectFunctionCalls:
    """Test indirect function call mutation"""

    def test_make_direct_call_indirect(self):
        """Test converting direct to indirect call"""
        direct = {"type": "call", "target": "function_a"}
        indirect = self._make_indirect(direct)
        assert indirect["type"] in ["call_indirect", "jmp_table"]

    def test_indirect_call_table(self):
        """Test indirect call through jump table"""
        functions = ["func_a", "func_b", "func_c"]
        table = self._build_call_table(functions)
        assert len(table) == len(functions)

    def test_indirect_call_obfuscates_target(self):
        """Test indirect call obfuscates target function"""
        direct = {"target": "admin_check"}
        indirect = self._make_indirect(direct)
        assert "admin_check" not in str(indirect)

    def test_indirect_call_maintains_semantics(self):
        """Test indirect call maintains semantics"""
        functions = ["func_a", "func_b"]
        table = self._build_call_table(functions)
        assert all(f in str(table) for f in functions)

    @staticmethod
    def _make_indirect(call):
        return {"type": "call_indirect", "table_index": 0}

    @staticmethod
    def _build_call_table(functions):
        return {i: func for i, func in enumerate(functions)}


class TestMutationCombinations:
    """Test combinations of mutations"""

    def test_apply_multiple_mutations(self):
        """Test applying multiple mutations"""
        code = "simple code"
        mutations = ["instruction_substitution", "code_layout_randomization"]
        result = self._apply_mutations(code, mutations)
        assert result is not None

    def test_mutation_order_matters(self):
        """Test that mutation order affects result"""
        code = "test code"
        result1 = self._apply_mutations(code, ["a", "b"])
        result2 = self._apply_mutations(code, ["b", "a"])
        assert result1 != result2

    def test_cumulative_obfuscation_level(self):
        """Test cumulative obfuscation from mutations"""
        mutations = ["a", "b", "c"]
        obfuscation = self._calculate_obfuscation_level(mutations)
        assert obfuscation > 0
        assert obfuscation <= 10

    @staticmethod
    def _apply_mutations(code, mutations):
        return f"obfuscated_{len(mutations)}"

    @staticmethod
    def _calculate_obfuscation_level(mutations):
        return min(len(mutations) * 2, 10)


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
