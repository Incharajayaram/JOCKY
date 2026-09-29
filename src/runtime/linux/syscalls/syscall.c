#include "../include/jocky_syscall.h"

#if defined(__x86_64__)

/* x86_64 Linux syscall calling convention:
 * rax = syscall number
 * rdi, rsi, rdx, r10, r8, r9 = arguments 1-6
 * rcx, r11 = clobbered by syscall instruction
 * return value in rax (negative = -errno)
 */

long jocky_syscall0(long number) {
    long result;
    __asm__ __volatile__(
        "syscall"
        : "=a" (result)
        : "0" (number)
        : "rcx", "r11"
    );
    return result;
}

long jocky_syscall1(long number, long arg1) {
    long result;
    __asm__ __volatile__(
        "syscall"
        : "=a" (result)
        : "0" (number), "D" (arg1)
        : "rcx", "r11"
    );
    return result;
}

long jocky_syscall2(long number, long arg1, long arg2) {
    long result;
    __asm__ __volatile__(
        "syscall"
        : "=a" (result)
        : "0" (number), "D" (arg1), "S" (arg2)
        : "rcx", "r11"
    );
    return result;
}

long jocky_syscall3(long number, long arg1, long arg2, long arg3) {
    long result;
    __asm__ __volatile__(
        "syscall"
        : "=a" (result)
        : "0" (number), "D" (arg1), "S" (arg2), "d" (arg3)
        : "rcx", "r11"
    );
    return result;
}

long jocky_syscall4(long number, long arg1, long arg2, long arg3, long arg4) {
    long result;
    register long r10 __asm__("r10") = arg4;
    __asm__ __volatile__(
        "syscall"
        : "=a" (result)
        : "0" (number), "D" (arg1), "S" (arg2), "d" (arg3), "r" (r10)
        : "rcx", "r11"
    );
    return result;
}

long jocky_syscall5(long number, long arg1, long arg2, long arg3, long arg4, long arg5) {
    long result;
    register long r10 __asm__("r10") = arg4;
    register long r8 __asm__("r8") = arg5;
    __asm__ __volatile__(
        "syscall"
        : "=a" (result)
        : "0" (number), "D" (arg1), "S" (arg2), "d" (arg3), "r" (r10), "r" (r8)
        : "rcx", "r11"
    );
    return result;
}

long jocky_syscall6(long number, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6) {
    long result;
    register long r10 __asm__("r10") = arg4;
    register long r8 __asm__("r8") = arg5;
    register long r9 __asm__("r9") = arg6;
    __asm__ __volatile__(
        "syscall"
        : "=a" (result)
        : "0" (number), "D" (arg1), "S" (arg2), "d" (arg3), "r" (r10), "r" (r8), "r" (r9)
        : "rcx", "r11"
    );
    return result;
}

#endif
