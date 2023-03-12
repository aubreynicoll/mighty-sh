#include "repl.h"

#include "builtins.h"
#include "config.h"
#include "error.h"
#include "input.h"
#include "libc.h"
#include "logger.h"
#include "mem.h"
#include "parser.h"
#include "signal.h"
#include "wrapped.h"

static int sh_launch_job(char **args);
static int sh_evaluate(char **args);

static int sh_launch_job(char **args) {
	int pid = sh_fork();

	if (pid == 0) {
		// child
		if (sh_config.shell_is_interactive) {
			sh_setpgid(0, 0);
			sh_tcsetpgrp(sh_config.shell_terminal, getpid());

			/* child process must NOT inherit the shell's SIG_IGN's
			 */
			sh_sig_dfl_all(SIGINT, SIGQUIT, SIGTSTP, SIGTTIN,
				       SIGTTOU, SIGCHLD);
		}

		sh_execvp(args[0], args);
	} else {
		// parent

		/* according to Kerrisk and GNU libc manual, parent should also
		 * attempt to set child's pgid & set as foreground process of
		 * terminal. Here we allow for EACCES errors, as this
		 * occurs if the child has already set the pgid */
		if (sh_config.shell_is_interactive) {
			if (setpgid(pid, 0) < 0 && errno != EACCES) {
				sh_fatal_unix_error(NULL);
			}

			sh_tcsetpgrp(sh_config.shell_terminal, pid);
		}

		int wstatus;

		/* the following loop doesn't yet handle ctrl-z (SIGTSTP) very
		   well, and needs another process to send it SIGCONT.
		 */
		do {
			sh_waitpid(pid, &wstatus, WUNTRACED | WCONTINUED);
		} while (!(WIFEXITED(wstatus) || WIFSIGNALED(wstatus)));

		if (sh_config.shell_is_interactive) {
			sh_tcsetpgrp(sh_config.shell_terminal,
				     sh_config.shell_pgid);
		}
	}

	return SH_REPL_CONTINUE;
}

static int sh_evaluate(char **args) {
	if (args[0] == NULL) {
		return SH_REPL_CONTINUE;
	}

	builtin_t builtin = sh_get_builtin(args[0]);
	if (builtin) {
		return builtin(args);
	}
	return sh_launch_job(args);
}

int sh_repl(void) {
	int status = 0;
	do {
		sh_print_prompt();
		char  *line = sh_read_line();
		char **tokens = sh_parse_tokens(line);
		status = sh_evaluate(tokens);
		sh_free_all(tokens, line);
	} while (status == SH_REPL_CONTINUE);

	return status;
}
