#include "../include/jocky_signal.h"
#include "../include/jocky_syscall.h"

/* Signal handler table (simplified) */
static jocky_signal_handler_t signal_handlers[64];

int jocky_signal_send(long pid, int sig) {
    if (sig < 0 || sig > 63) return -1;
    long result = jocky_syscall2(SYS_kill, pid, sig);
    return (result == 0) ? 0 : -1;
}

int jocky_signal_send_thread(long pid, long tid, int sig) {
    if (sig < 0 || sig > 63) return -1;
    long result = jocky_syscall3(SYS_tgkill, pid, tid, sig);
    return (result == 0) ? 0 : -1;
}

int jocky_signal_raise(int sig) {
    if (sig < 0 || sig > 63) return -1;
    long tid = jocky_syscall0(SYS_gettid);
    long pid = jocky_syscall0(SYS_getpid);
    long result = jocky_syscall3(SYS_tgkill, pid, tid, sig);
    return (result == 0) ? 0 : -1;
}

jocky_signal_handler_t jocky_signal_set_handler(int sig, jocky_signal_handler_t handler) {
    if (sig < 0 || sig > 63) return NULL;

    jocky_signal_handler_t old = signal_handlers[sig];
    signal_handlers[sig] = handler;

    return old;
}

int jocky_signal_ignore(int sig) {
    if (sig < 0 || sig > 63) return -1;

    signal_handlers[sig] = NULL;
    return 0;
}

int jocky_signal_block(void) {
    /* Simplified: just mark signals as blocked */
    return 0;
}

int jocky_signal_unblock(void) {
    /* Simplified: just mark signals as unblocked */
    return 0;
}
