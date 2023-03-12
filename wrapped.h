/**
 * The functions contained within die on ANY error
 *
 * Used for general purpose sys calls, other such wrapped functions may exist
 * elsewhere if they are a bit more signicant (e.g. malloc)
 */

#ifndef __SH_WRAPPED_INCLUDED__
#define __SH_WRAPPED_INCLUDED__

#include <signal.h>
#include <sys/types.h>

sig_t sh_signal(int signum, sig_t handler);

int sh_setpgid(pid_t pid, pid_t pgid);

pid_t sh_tcgetpgrp(int fd);
int   sh_tcsetpgrp(int fd, pid_t pgrp);

int sh_kill(pid_t pid, int sig);

pid_t sh_fork(void);

int sh_execvp(const char *file, char *const argv[]);

pid_t sh_waitpid(pid_t pid, int *wstatus, int options);

#endif
