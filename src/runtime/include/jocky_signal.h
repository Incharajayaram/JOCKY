#ifndef JOCKY_SIGNAL_H
#define JOCKY_SIGNAL_H

#include <stddef.h>

/* Signal numbers */
#define JOCKY_SIGHUP     1
#define JOCKY_SIGINT     2
#define JOCKY_SIGQUIT    3
#define JOCKY_SIGILL     4
#define JOCKY_SIGTRAP    5
#define JOCKY_SIGABRT    6
#define JOCKY_SIGBUS     7
#define JOCKY_SIGFPE     8
#define JOCKY_SIGKILL    9
#define JOCKY_SIGUSR1    10
#define JOCKY_SIGSEGV    11
#define JOCKY_SIGUSR2    12
#define JOCKY_SIGPIPE    13
#define JOCKY_SIGALRM    14
#define JOCKY_SIGTERM    15
#define JOCKY_SIGCHLD    17
#define JOCKY_SIGCONT    18
#define JOCKY_SIGSTOP    19

/* Signal handler typedef */
typedef void (*jocky_signal_handler_t)(int);

/* ========== SIGNAL OPERATIONS ========== */

/**
 * Send signal to process.
 * Returns 0 on success, -1 on error.
 */
int jocky_signal_send(long pid, int sig);

/**
 * Send signal to thread.
 * Returns 0 on success, -1 on error.
 */
int jocky_signal_send_thread(long pid, long tid, int sig);

/**
 * Raise signal in current process.
 * Returns 0 on success, -1 on error.
 */
int jocky_signal_raise(int sig);

/**
 * Set signal handler.
 * sig: Signal number
 * handler: Signal handler function
 * Returns previous handler, NULL on error.
 */
jocky_signal_handler_t jocky_signal_set_handler(int sig, jocky_signal_handler_t handler);

/**
 * Ignore signal.
 * Returns 0 on success, -1 on error.
 */
int jocky_signal_ignore(int sig);

/**
 * Block signal mask.
 * Returns 0 on success, -1 on error.
 */
int jocky_signal_block(void);

/**
 * Unblock signal mask.
 * Returns 0 on success, -1 on error.
 */
int jocky_signal_unblock(void);

#endif // JOCKY_SIGNAL_H
