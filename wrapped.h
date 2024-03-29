/**
 * The functions contained within die on ANY error, with very few exceptions to
 * that rule.
 *
 * Used for general purpose sys calls, other such wrapped functions may exist
 * elsewhere if they are a bit more significant (e.g. malloc)
 */

#ifndef __SH_WRAPPED_INCLUDED__
#define __SH_WRAPPED_INCLUDED__

#include <signal.h>
#include <sys/types.h>
#include <termios.h>

#include "util.h"

sig_t sh_signal(int signum, sig_t handler);

int sh_setpgid(pid_t pid, pid_t pgid);

pid_t sh_tcgetpgrp(int fd);
int   sh_tcsetpgrp(int fd, pid_t pgrp);
int   sh_tcgetattr(int fd, struct termios *tp);
int   sh_tcsetattr(int fd, int options, const struct termios *tp);

int sh_kill(pid_t pid, int sig);

pid_t sh_fork(void);

noreturn int sh_execvp(const char *file, char *const argv[]);

/* error-checked waitpid wrapper. this function does NOT fail on ECHILD errors.
 */
pid_t sh_waitpid(pid_t pid, int *wstatus, int options);

int sh_open(const char *path, int flags);
int sh_close(int fd);

int sh_dup2(int oldfd, int newfd);

int sh_setenv(char *key, char *value, int overwrite);

#endif
