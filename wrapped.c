#include "wrapped.h"

#include "error.h"
#include "libc.h"
#include "util.h"

sig_t sh_signal(int signum, sig_t handler) {
	sig_t ret = signal(signum, handler);
	if (ret == SIG_ERR) {
		sh_fatal_unix_error("signal");
	}
	return ret;
}

int sh_setpgid(pid_t pid, pid_t pgid) {
	int ret = setpgid(pid, pgid);
	if (ret < 0) {
		sh_fatal_unix_error("setpgid");
	}
	return ret;
}

pid_t sh_tcgetpgrp(int fd) {
	pid_t ret = tcgetpgrp(fd);
	if (ret < 0) {
		sh_fatal_unix_error("tcgetpgrp");
	}
	return ret;
}

int sh_tcsetpgrp(int fd, pid_t pgrp) {
	int ret = tcsetpgrp(fd, pgrp);
	if (ret < 0) {
		sh_fatal_unix_error("tcsetpgrp");
	}
	return ret;
}

int sh_tcgetattr(int fd, struct termios *tp) {
	int ret = tcgetattr(fd, tp);
	if (ret < 0) {
		sh_fatal_unix_error("tcgetattr");
	}
	return ret;
}

int sh_tcsetattr(int fd, int options, const struct termios *tp) {
	int ret = tcsetattr(fd, options, tp);
	if (ret < 0) {
		sh_fatal_unix_error("tcsetattr");
	}
	return ret;
}

int sh_kill(pid_t pid, int sig) {
	int ret = kill(pid, sig);
	if (ret < 0) {
		sh_fatal_unix_error("kill");
	}
	return ret;
}

pid_t sh_fork(void) {
	pid_t ret = fork();
	if (ret < 0) {
		sh_fatal_unix_error("fork");
	}
	return ret;
}

noreturn int sh_execvp(const char *file, char *const argv[]) {
	execvp(file, argv);
	sh_fatal_unix_error("execvp");
}

pid_t sh_waitpid(pid_t pid, int *wstatus, int options) {
	pid_t ret = waitpid(pid, wstatus, options);
	if (ret < 0 && errno != ECHILD) {
		sh_fatal_unix_error("waitpid");
	}
	return ret;
}

int sh_open(const char *path, int flags) {
	int ret = open(path, flags);
	if (ret < 0) {
		sh_fatal_unix_error("open");
	}
	return ret;
}

int sh_close(int fd) {
	int ret = close(fd);
	if (ret < 0) {
		sh_fatal_unix_error("close");
	}
	return ret;
}

int sh_dup2(int oldfd, int newfd) {
	int ret = dup2(oldfd, newfd);
	if (ret < 0) {
		sh_fatal_unix_error("dup2");
	}
	return ret;
}

int sh_setenv(char *key, char *value, int overwrite) {
	int ret = setenv(key, value, overwrite);
	if (ret < 0) {
		sh_fatal_unix_error("setenv");
	}
	return ret;
}
