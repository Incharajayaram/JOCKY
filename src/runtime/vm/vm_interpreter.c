/*
 * JOCKY Bytecode VM Interpreter
 * Stack-based VM with 256 registers and 40+ instructions
 */

#include "vm_opcodes.h"
#include "jocky_vm.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static jocky_vm_t *g_vm = NULL;

void jocky_vm_init(void) {
    if (g_vm) return;
    g_vm = (jocky_vm_t *)malloc(sizeof(jocky_vm_t));
    if (!g_vm) return;

    memset(g_vm, 0, sizeof(jocky_vm_t));
    g_vm->call_stack = (uint64_t *)malloc(4096 * sizeof(uint64_t));
    g_vm->sp = 0;
    g_vm->pc = 0;
    g_vm->call_sp = 0;
}

void jocky_vm_destroy(void) {
    if (!g_vm) return;
    if (g_vm->call_stack) {
        free(g_vm->call_stack);
    }
    if (g_vm->bytecode && g_vm->bytecode != NULL) {
        free(g_vm->bytecode);
    }
    free(g_vm);
    g_vm = NULL;
}

void jocky_vm_load(uint8_t *bytecode, size_t size) {
    if (!g_vm) jocky_vm_init();
    if (!g_vm) return;

    if (g_vm->bytecode && g_vm->bytecode != bytecode) {
        free(g_vm->bytecode);
    }

    g_vm->bytecode = (uint8_t *)malloc(size);
    if (!g_vm->bytecode) return;

    memcpy(g_vm->bytecode, bytecode, size);
    g_vm->bytecode_len = size;
    g_vm->pc = 0;
}

void jocky_vm_register_external(uint32_t index, void *func) {
    if (!g_vm) jocky_vm_init();
    if (!g_vm || index >= 256) return;

    g_vm->external_funcs[index] = func;
    if (index >= g_vm->num_external) {
        g_vm->num_external = index + 1;
    }
}

static inline uint8_t vm_read_u8(void) {
    if (g_vm->pc >= g_vm->bytecode_len) return 0;
    return g_vm->bytecode[g_vm->pc++];
}

static inline uint16_t vm_read_u16(void) {
    if (g_vm->pc + 2 > g_vm->bytecode_len) return 0;
    uint16_t val = *(uint16_t *)(g_vm->bytecode + g_vm->pc);
    g_vm->pc += 2;
    return val;
}

static inline uint32_t vm_read_u32(void) {
    if (g_vm->pc + 4 > g_vm->bytecode_len) return 0;
    uint32_t val = *(uint32_t *)(g_vm->bytecode + g_vm->pc);
    g_vm->pc += 4;
    return val;
}

static inline uint64_t vm_read_u64(void) {
    if (g_vm->pc + 8 > g_vm->bytecode_len) return 0;
    uint64_t val = *(uint64_t *)(g_vm->bytecode + g_vm->pc);
    g_vm->pc += 8;
    return val;
}

static inline int vm_stack_push(uint64_t val) {
    if (g_vm->sp >= VM_MAX_STACK) return -1;
    g_vm->stack[g_vm->sp++] = val;
    return 0;
}

static inline uint64_t vm_stack_pop(void) {
    if (g_vm->sp == 0) return 0;
    return g_vm->stack[--g_vm->sp];
}

static inline uint64_t vm_stack_peek(void) {
    if (g_vm->sp == 0) return 0;
    return g_vm->stack[g_vm->sp - 1];
}

static inline void vm_set_flags(uint64_t val) {
    g_vm->flags = 0;
    if (val == 0) g_vm->flags |= VM_FLAG_ZERO;
    if ((int64_t)val < 0) g_vm->flags |= VM_FLAG_SIGN;
}

uint64_t jocky_vm_execute(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
    if (!g_vm) jocky_vm_init();
    if (!g_vm || !g_vm->bytecode) return 0;

    g_vm->registers[0] = arg1;
    g_vm->registers[1] = arg2;
    g_vm->registers[2] = arg3;
    g_vm->registers[3] = arg4;
    g_vm->pc = 0;
    g_vm->sp = 0;
    g_vm->call_sp = 0;

    while (g_vm->pc < g_vm->bytecode_len) {
        uint8_t opcode = vm_read_u8();

        switch (opcode) {
            case VM_NOP:
                break;

            case VM_PUSH_IMM8: {
                uint8_t imm = vm_read_u8();
                vm_stack_push(imm);
                break;
            }

            case VM_PUSH_IMM16: {
                uint16_t imm = vm_read_u16();
                vm_stack_push(imm);
                break;
            }

            case VM_PUSH_IMM32: {
                uint32_t imm = vm_read_u32();
                vm_stack_push(imm);
                break;
            }

            case VM_PUSH_IMM64: {
                uint64_t imm = vm_read_u64();
                vm_stack_push(imm);
                break;
            }

            case VM_PUSH_REG: {
                uint8_t reg = vm_read_u8();
                if (reg < VM_MAX_REGS) {
                    vm_stack_push(g_vm->registers[reg]);
                }
                break;
            }

            case VM_POP_REG: {
                uint8_t reg = vm_read_u8();
                if (reg < VM_MAX_REGS) {
                    g_vm->registers[reg] = vm_stack_pop();
                }
                break;
            }

            case VM_POP_DISCARD:
                vm_stack_pop();
                break;

            case VM_MOV_REG: {
                uint8_t dst = vm_read_u8();
                uint8_t src = vm_read_u8();
                if (dst < VM_MAX_REGS && src < VM_MAX_REGS) {
                    g_vm->registers[dst] = g_vm->registers[src];
                }
                break;
            }

            case VM_SWAP: {
                if (g_vm->sp >= 2) {
                    uint64_t tmp = g_vm->stack[g_vm->sp - 1];
                    g_vm->stack[g_vm->sp - 1] = g_vm->stack[g_vm->sp - 2];
                    g_vm->stack[g_vm->sp - 2] = tmp;
                }
                break;
            }

            case VM_DUP: {
                if (g_vm->sp > 0 && g_vm->sp < VM_MAX_STACK) {
                    g_vm->stack[g_vm->sp] = g_vm->stack[g_vm->sp - 1];
                    g_vm->sp++;
                }
                break;
            }

            case VM_ADD: {
                uint64_t b = vm_stack_pop();
                uint64_t a = vm_stack_pop();
                vm_stack_push(a + b);
                break;
            }

            case VM_SUB: {
                uint64_t b = vm_stack_pop();
                uint64_t a = vm_stack_pop();
                vm_stack_push(a - b);
                break;
            }

            case VM_MUL: {
                uint64_t b = vm_stack_pop();
                uint64_t a = vm_stack_pop();
                vm_stack_push(a * b);
                break;
            }

            case VM_DIV: {
                uint64_t b = vm_stack_pop();
                uint64_t a = vm_stack_pop();
                if (b == 0) return 0;
                vm_stack_push((int64_t)a / (int64_t)b);
                break;
            }

            case VM_UDIV: {
                uint64_t b = vm_stack_pop();
                uint64_t a = vm_stack_pop();
                if (b == 0) return 0;
                vm_stack_push(a / b);
                break;
            }

            case VM_MOD: {
                uint64_t b = vm_stack_pop();
                uint64_t a = vm_stack_pop();
                if (b == 0) return 0;
                vm_stack_push((int64_t)a % (int64_t)b);
                break;
            }

            case VM_UMOD: {
                uint64_t b = vm_stack_pop();
                uint64_t a = vm_stack_pop();
                if (b == 0) return 0;
                vm_stack_push(a % b);
                break;
            }

            case VM_AND: {
                uint64_t b = vm_stack_pop();
                uint64_t a = vm_stack_pop();
                vm_stack_push(a & b);
                break;
            }

            case VM_OR: {
                uint64_t b = vm_stack_pop();
                uint64_t a = vm_stack_pop();
                vm_stack_push(a | b);
                break;
            }

            case VM_XOR: {
                uint64_t b = vm_stack_pop();
                uint64_t a = vm_stack_pop();
                vm_stack_push(a ^ b);
                break;
            }

            case VM_NOT: {
                uint64_t a = vm_stack_pop();
                vm_stack_push(~a);
                break;
            }

            case VM_SHL: {
                uint64_t b = vm_stack_pop();
                uint64_t a = vm_stack_pop();
                vm_stack_push(a << (b & 0x3F));
                break;
            }

            case VM_SHR: {
                uint64_t b = vm_stack_pop();
                uint64_t a = vm_stack_pop();
                vm_stack_push(a >> (b & 0x3F));
                break;
            }

            case VM_SAR: {
                uint64_t b = vm_stack_pop();
                int64_t a = (int64_t)vm_stack_pop();
                vm_stack_push((uint64_t)(a >> (b & 0x3F)));
                break;
            }

            case VM_CMP: {
                uint64_t b = vm_stack_pop();
                uint64_t a = vm_stack_pop();
                vm_set_flags(a - b);
                break;
            }

            case VM_CMP_EQ: {
                uint64_t b = vm_stack_pop();
                uint64_t a = vm_stack_pop();
                vm_stack_push((a == b) ? 1 : 0);
                break;
            }

            case VM_CMP_NE: {
                uint64_t b = vm_stack_pop();
                uint64_t a = vm_stack_pop();
                vm_stack_push((a != b) ? 1 : 0);
                break;
            }

            case VM_CMP_LT: {
                uint64_t b = vm_stack_pop();
                int64_t a = (int64_t)vm_stack_pop();
                vm_stack_push((a < (int64_t)b) ? 1 : 0);
                break;
            }

            case VM_CMP_LE: {
                uint64_t b = vm_stack_pop();
                int64_t a = (int64_t)vm_stack_pop();
                vm_stack_push((a <= (int64_t)b) ? 1 : 0);
                break;
            }

            case VM_CMP_GT: {
                uint64_t b = vm_stack_pop();
                int64_t a = (int64_t)vm_stack_pop();
                vm_stack_push((a > (int64_t)b) ? 1 : 0);
                break;
            }

            case VM_CMP_GE: {
                uint64_t b = vm_stack_pop();
                int64_t a = (int64_t)vm_stack_pop();
                vm_stack_push((a >= (int64_t)b) ? 1 : 0);
                break;
            }

            case VM_CMP_ULT: {
                uint64_t b = vm_stack_pop();
                uint64_t a = vm_stack_pop();
                vm_stack_push((a < b) ? 1 : 0);
                break;
            }

            case VM_CMP_ULE: {
                uint64_t b = vm_stack_pop();
                uint64_t a = vm_stack_pop();
                vm_stack_push((a <= b) ? 1 : 0);
                break;
            }

            case VM_CMP_UGT: {
                uint64_t b = vm_stack_pop();
                uint64_t a = vm_stack_pop();
                vm_stack_push((a > b) ? 1 : 0);
                break;
            }

            case VM_CMP_UGE: {
                uint64_t b = vm_stack_pop();
                uint64_t a = vm_stack_pop();
                vm_stack_push((a >= b) ? 1 : 0);
                break;
            }

            case VM_LOAD8: {
                uint64_t addr = vm_stack_pop();
                uint8_t val = *(uint8_t *)addr;
                vm_stack_push(val);
                break;
            }

            case VM_LOAD16: {
                uint64_t addr = vm_stack_pop();
                uint16_t val = *(uint16_t *)addr;
                vm_stack_push(val);
                break;
            }

            case VM_LOAD32: {
                uint64_t addr = vm_stack_pop();
                uint32_t val = *(uint32_t *)addr;
                vm_stack_push(val);
                break;
            }

            case VM_LOAD64: {
                uint64_t addr = vm_stack_pop();
                uint64_t val = *(uint64_t *)addr;
                vm_stack_push(val);
                break;
            }

            case VM_STORE8: {
                uint64_t val = vm_stack_pop();
                uint64_t addr = vm_stack_pop();
                *(uint8_t *)addr = (uint8_t)val;
                break;
            }

            case VM_STORE16: {
                uint64_t val = vm_stack_pop();
                uint64_t addr = vm_stack_pop();
                *(uint16_t *)addr = (uint16_t)val;
                break;
            }

            case VM_STORE32: {
                uint64_t val = vm_stack_pop();
                uint64_t addr = vm_stack_pop();
                *(uint32_t *)addr = (uint32_t)val;
                break;
            }

            case VM_STORE64: {
                uint64_t val = vm_stack_pop();
                uint64_t addr = vm_stack_pop();
                *(uint64_t *)addr = val;
                break;
            }

            case VM_JMP: {
                uint32_t target = vm_read_u32();
                g_vm->pc = target;
                break;
            }

            case VM_JZ: {
                uint32_t target = vm_read_u32();
                uint64_t cond = vm_stack_pop();
                if (cond == 0) g_vm->pc = target;
                break;
            }

            case VM_JNZ: {
                uint32_t target = vm_read_u32();
                uint64_t cond = vm_stack_pop();
                if (cond != 0) g_vm->pc = target;
                break;
            }

            case VM_JLT: {
                uint32_t target = vm_read_u32();
                if (g_vm->flags & VM_FLAG_SIGN) g_vm->pc = target;
                break;
            }

            case VM_JLE: {
                uint32_t target = vm_read_u32();
                if ((g_vm->flags & VM_FLAG_SIGN) || (g_vm->flags & VM_FLAG_ZERO)) g_vm->pc = target;
                break;
            }

            case VM_JGT: {
                uint32_t target = vm_read_u32();
                if (!(g_vm->flags & VM_FLAG_SIGN) && !(g_vm->flags & VM_FLAG_ZERO)) g_vm->pc = target;
                break;
            }

            case VM_JGE: {
                uint32_t target = vm_read_u32();
                if (!(g_vm->flags & VM_FLAG_SIGN)) g_vm->pc = target;
                break;
            }

            case VM_CALL: {
                uint8_t func_idx = vm_read_u8();
                if (func_idx >= g_vm->num_external) return 0;
                if (g_vm->call_sp >= 4096) return 0;

                g_vm->call_stack[g_vm->call_sp++] = g_vm->pc;

                typedef uint64_t (*vm_func_t)(uint64_t, uint64_t, uint64_t, uint64_t);
                vm_func_t func = (vm_func_t)g_vm->external_funcs[func_idx];
                uint64_t result = func(g_vm->registers[0], g_vm->registers[1],
                                      g_vm->registers[2], g_vm->registers[3]);
                g_vm->registers[0] = result;
                break;
            }

            case VM_RET:
                if (g_vm->call_sp > 0) {
                    g_vm->pc = g_vm->call_stack[--g_vm->call_sp];
                } else {
                    return g_vm->registers[0];
                }
                break;

            case VM_HALT:
                return g_vm->registers[0];

            case VM_SYSCALL: {
                uint8_t syscall_id = vm_read_u8();
                break;
            }

            default:
                return 0;
        }
    }

    return g_vm->registers[0];
}

uint64_t jocky_vm_get_register(uint32_t reg_idx) {
    if (!g_vm || reg_idx >= VM_MAX_REGS) return 0;
    return g_vm->registers[reg_idx];
}

void jocky_vm_set_register(uint32_t reg_idx, uint64_t value) {
    if (!g_vm || reg_idx >= VM_MAX_REGS) return;
    g_vm->registers[reg_idx] = value;
}

void *jocky_vm_compile_function(void *func_ptr, size_t *out_size) {
    *out_size = 0;
    return NULL;
}
