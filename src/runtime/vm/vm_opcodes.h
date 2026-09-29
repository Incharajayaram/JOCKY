#ifndef VM_OPCODES_H
#define VM_OPCODES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    VM_NOP = 0x00,

    VM_PUSH_IMM8 = 0x01,
    VM_PUSH_IMM16 = 0x02,
    VM_PUSH_IMM32 = 0x03,
    VM_PUSH_IMM64 = 0x04,
    VM_PUSH_REG = 0x05,

    VM_POP_REG = 0x06,
    VM_POP_DISCARD = 0x07,

    VM_MOV_REG = 0x08,
    VM_SWAP = 0x09,
    VM_DUP = 0x0A,

    VM_ADD = 0x10,
    VM_SUB = 0x11,
    VM_MUL = 0x12,
    VM_DIV = 0x13,
    VM_UDIV = 0x14,
    VM_MOD = 0x15,
    VM_UMOD = 0x16,

    VM_AND = 0x20,
    VM_OR = 0x21,
    VM_XOR = 0x22,
    VM_NOT = 0x23,
    VM_SHL = 0x24,
    VM_SHR = 0x25,
    VM_SAR = 0x26,

    VM_CMP = 0x30,
    VM_CMP_EQ = 0x31,
    VM_CMP_NE = 0x32,
    VM_CMP_LT = 0x33,
    VM_CMP_LE = 0x34,
    VM_CMP_GT = 0x35,
    VM_CMP_GE = 0x36,
    VM_CMP_ULT = 0x37,
    VM_CMP_ULE = 0x38,
    VM_CMP_UGT = 0x39,
    VM_CMP_UGE = 0x3A,

    VM_LOAD8 = 0x40,
    VM_LOAD16 = 0x41,
    VM_LOAD32 = 0x42,
    VM_LOAD64 = 0x43,
    VM_STORE8 = 0x44,
    VM_STORE16 = 0x45,
    VM_STORE32 = 0x46,
    VM_STORE64 = 0x47,

    VM_JMP = 0x50,
    VM_JZ = 0x51,
    VM_JNZ = 0x52,
    VM_JLT = 0x53,
    VM_JLE = 0x54,
    VM_JGT = 0x55,
    VM_JGE = 0x56,

    VM_CALL = 0x60,
    VM_RET = 0x61,
    VM_HALT = 0x62,

    VM_SYSCALL = 0x70,

} vm_opcode_t;

#define VM_MAX_REGS 256
#define VM_MAX_STACK 8192

typedef struct {
    uint64_t registers[VM_MAX_REGS];
    uint64_t stack[VM_MAX_STACK];
    uint32_t sp;
    uint32_t pc;
    uint8_t *bytecode;
    uint32_t bytecode_len;
    uint64_t *call_stack;
    uint32_t call_sp;
    void *external_funcs[256];
    uint32_t num_external;
    uint8_t flags;
} jocky_vm_t;

#define VM_FLAG_ZERO 0x01
#define VM_FLAG_SIGN 0x02
#define VM_FLAG_OVERFLOW 0x04
#define VM_FLAG_CARRY 0x08

#ifdef __cplusplus
}
#endif

#endif
