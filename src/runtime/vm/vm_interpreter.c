/*
 * JOCKY Custom VM Interpreter
 * Executes custom bytecode for virtualized functions
 */

#include "jocky_vm.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/* VM Instruction Set */
typedef enum {
    VM_PUSH_IMM = 0x01,
    VM_PUSH_REG = 0x02,
    VM_POP_REG  = 0x03,
    VM_ADD      = 0x04,
    VM_SUB      = 0x05,
    VM_MUL      = 0x06,
    VM_DIV      = 0x07,
    VM_AND      = 0x08,
    VM_OR       = 0x09,
    VM_XOR      = 0x0A,
    VM_SHL      = 0x0B,
    VM_SHR      = 0x0C,
    VM_CALL     = 0x0D,
    VM_RET      = 0x0E,
    VM_JMP      = 0x0F,
    VM_JZ       = 0x10,
    VM_JNZ      = 0x11,
    VM_LOAD     = 0x12,
    VM_STORE    = 0x13,
    VM_CMP      = 0x14,
    VM_NOP      = 0xFF,
} vm_opcode_t;

typedef struct {
    uint64_t registers[16];
    uint64_t stack[1024];
    uint32_t sp;
    uint64_t ip;
    uint8_t* bytecode;
    size_t bytecode_size;
    uint64_t* call_stack;
    uint32_t call_sp;
    void* external_funcs[256];
    uint32_t num_external;
} vm_context_t;

static vm_context_t g_vm;

void jocky_vm_init(void) {
    memset(&g_vm, 0, sizeof(g_vm));
    g_vm.sp = 0;
    g_vm.ip = 0;
    g_vm.call_sp = 0;
}

void jocky_vm_load(uint8_t* bytecode, size_t size) {
    g_vm.bytecode = bytecode;
    g_vm.bytecode_size = size;
    g_vm.ip = 0;
}

void jocky_vm_register_external(uint32_t index, void* func) {
    if (index < 256) {
        g_vm.external_funcs[index] = func;
        if (index >= g_vm.num_external) {
            g_vm.num_external = index + 1;
        }
    }
}

uint64_t jocky_vm_execute(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
    g_vm.registers[0] = arg1;
    g_vm.registers[1] = arg2;
    g_vm.registers[2] = arg3;
    g_vm.registers[3] = arg4;
    
    while (g_vm.ip < g_vm.bytecode_size) {
        uint8_t opcode = g_vm.bytecode[g_vm.ip++];
        
        switch (opcode) {
            case 0xFF: break;
            case 0x01: {
                if (g_vm.ip + 8 > g_vm.bytecode_size) return 0;
                uint64_t imm = *(uint64_t*)(g_vm.bytecode + g_vm.ip);
                g_vm.ip += 8;
                if (g_vm.sp >= 1024) return 0;
                g_vm.stack[g_vm.sp++] = imm;
                break;
            }
            case 0x02: {
                if (g_vm.ip >= g_vm.bytecode_size) return 0;
                uint8_t reg = g_vm.bytecode[g_vm.ip++];
                if (reg >= 16) return 0;
                if (g_vm.sp >= 1024) return 0;
                g_vm.stack[g_vm.sp++] = g_vm.registers[reg];
                break;
            }
            case 0x03: {
                if (g_vm.ip >= g_vm.bytecode_size) return 0;
                uint8_t reg = g_vm.bytecode[g_vm.ip++];
                if (reg >= 16 || g_vm.sp == 0) return 0;
                g_vm.registers[reg] = g_vm.stack[--g_vm.sp];
                break;
            }
            case 0x04: {
                if (g_vm.sp < 2) return 0;
                uint64_t b = g_vm.stack[--g_vm.sp];
                uint64_t a = g_vm.stack[--g_vm.sp];
                g_vm.stack[g_vm.sp++] = a + b;
                break;
            }
            case 0x05: {
                if (g_vm.sp < 2) return 0;
                uint64_t b = g_vm.stack[--g_vm.sp];
                uint64_t a = g_vm.stack[--g_vm.sp];
                g_vm.stack[g_vm.sp++] = a - b;
                break;
            }
            case 0x06: {
                if (g_vm.sp < 2) return 0;
                uint64_t b = g_vm.stack[--g_vm.sp];
                uint64_t a = g_vm.stack[--g_vm.sp];
                g_vm.stack[g_vm.sp++] = a * b;
                break;
            }
            case 0x07: {
                if (g_vm.sp < 2) return 0;
                uint64_t b = g_vm.stack[--g_vm.sp];
                uint64_t a = g_vm.stack[--g_vm.sp];
                if (b == 0) return 0;
                g_vm.stack[g_vm.sp++] = a / b;
                break;
            }
            case 0x08: {
                if (g_vm.sp < 2) return 0;
                uint64_t b = g_vm.stack[--g_vm.sp];
                uint64_t a = g_vm.stack[--g_vm.sp];
                g_vm.stack[g_vm.sp++] = a & b;
                break;
            }
            case 0x09: {
                if (g_vm.sp < 2) return 0;
                uint64_t b = g_vm.stack[--g_vm.sp];
                uint64_t a = g_vm.stack[--g_vm.sp];
                g_vm.stack[g_vm.sp++] = a | b;
                break;
            }
            case 0x0A: {
                if (g_vm.sp < 2) return 0;
                uint64_t b = g_vm.stack[--g_vm.sp];
                uint64_t a = g_vm.stack[--g_vm.sp];
                g_vm.stack[g_vm.sp++] = a ^ b;
                break;
            }
            case 0x0B: {
                if (g_vm.sp < 2) return 0;
                uint64_t b = g_vm.stack[--g_vm.sp];
                uint64_t a = g_vm.stack[--g_vm.sp];
                g_vm.stack[g_vm.sp++] = a << b;
                break;
            }
            case 0x0C: {
                if (g_vm.sp < 2) return 0;
                uint64_t b = g_vm.stack[--g_vm.sp];
                uint64_t a = g_vm.stack[--g_vm.sp];
                g_vm.stack[g_vm.sp++] = a >> b;
                break;
            }
            case 0x0D: {
                if (g_vm.ip >= g_vm.bytecode_size) return 0;
                uint8_t func_idx = g_vm.bytecode[g_vm.ip++];
                if (func_idx >= g_vm.num_external) return 0;
                if (g_vm.call_sp >= 256) return 0;
                g_vm.call_stack[g_vm.call_sp++] = g_vm.ip;
                typedef uint64_t (*vm_func_t)(uint64_t, uint64_t, uint64_t, uint64_t);
                vm_func_t func = (uint64_t(*)(uint64_t,uint64_t,uint64_t,uint64_t))g_vm.external_funcs[func_idx];
                uint64_t result = func(g_vm.registers[0], g_vm.registers[1], g_vm.registers[2], g_vm.registers[3]);
                g_vm.registers[0] = result;
                break;
            }
            case 0x0E: {
                if (g_vm.call_sp == 0) return g_vm.registers[0];
                g_vm.ip = g_vm.call_stack[--g_vm.call_sp];
                break;
            }
            case 0x0F: {
                if (g_vm.ip + 4 > g_vm.bytecode_size) return 0;
                uint32_t target = *(uint32_t*)(g_vm.bytecode + g_vm.ip);
                g_vm.ip = target;
                break;
            }
            case 0x10: {
                if (g_vm.ip + 4 > g_vm.bytecode_size) return 0;
                uint32_t target = *(uint32_t*)(g_vm.bytecode + g_vm.ip);
                g_vm.ip += 4;
                if (g_vm.sp == 0) return 0;
                if (g_vm.stack[g_vm.sp - 1] == 0) g_vm.ip = target;
                g_vm.sp--;
                break;
            }
            case 0x11: {
                if (g_vm.ip + 4 > g_vm.bytecode_size) return 0;
                uint32_t target = *(uint32_t*)(g_vm.bytecode + g_vm.ip);
                g_vm.ip += 4;
                if (g_vm.sp == 0) return 0;
                if (g_vm.stack[g_vm.sp - 1] != 0) g_vm.ip = target;
                g_vm.sp--;
                break;
            }
            case 0x12: {
                if (g_vm.sp == 0) return 0;
                uint64_t addr = g_vm.stack[--g_vm.sp];
                if (g_vm.sp >= 1024) return 0;
                g_vm.stack[g_vm.sp++] = *(uint64_t*)addr;
                break;
            }
            case 0x13: {
                if (g_vm.sp < 2) return 0;
                uint64_t value = g_vm.stack[--g_vm.sp];
                uint64_t addr = g_vm.stack[--g_vm.sp];
                *(uint64_t*)addr = value;
                break;
            }
            case 0x14: {
                if (g_vm.sp < 2) return 0;
                uint64_t b = g_vm.stack[--g_vm.sp];
                uint64_t a = g_vm.stack[--g_vm.sp];
                g_vm.stack[g_vm.sp++] = (a == b) ? 1 : 0;
                break;
            }
            default: return 0;
        }
    }
    return g_vm.registers[0];
}

void* jocky_vm_compile_function(void* func_ptr, size_t* out_size) {
    *out_size = 0;
    return NULL;
}
