#include "repl.h"

#include "builtins.h"
#include "config.h"
#include "error.h"
#include "input.h"
#include "job.h"
#include "libc.h"
#include "logger.h"
#include "mem.h"
#include "parser.h"
#include "signal.h"
#include "wrapped.h"

static int sh_launch_job(job_t *job);
static int sh_evaluate(command_t *args);

static int sh_launch_job(job_t *job) {
	int pid = sh_fork();

	if (pid == 0) {
		// child
		if (sh_config.shell_is_interactive) {
			sh_setpgid(0, 0);
			if (!job->cmd->bg) {
				sh_tcsetpgrp(sh_config.shell_terminal,
					     getpid());
			}

			/* child process must NOT inherit the shell's SIG_IGN's
			 */
			sh_sig_dfl_all(SIGINT, SIGQUIT, SIGTSTP, SIGTTIN,
				       SIGTTOU);
		}

		sh_execvp(job->cmd->argv[0], job->cmd->argv);
	} else {
		// parent
		job->pid = pid;
		job->status = RUNNING;

		if (sh_config.shell_is_interactive) {
			/* according to Kerrisk and GNU libc manual, parent
			 * should also attempt to set child's pgid. Here we
			 * allow for EACCES errors, as this occurs if the child
			 * has already set the pgid */
			if (setpgid(pid, 0) < 0 && errno != EACCES) {
				sh_fatal_unix_error(NULL);
			}
			job->pgid = pid;
		} else {
			job->pgid = sh_config.shell_pgid;
		}

		if (!sh_config.shell_is_interactive) {
			sh_wait_for_job(job);
		} else if (!job->cmd->bg) {
			sh_move_job_fg(job, 0);
		} else {
			sh_move_job_bg(job, 0);
		}
	}

	return SH_REPL_CONTINUE;
}

static int sh_evaluate(command_t *cmd) {
	if (cmd->argv[0] == NULL) {
		/* user entered an empty line */
		return SH_REPL_CONTINUE;
	}

	builtin_t builtin = sh_get_builtin(cmd->argv[0]);
	if (builtin) {
		return builtin(cmd->argv);
	}

	job_t *job = sh_new_job(cmd);
	if (!job) {
		/* probably hit max jobs */
		return SH_REPL_CONTINUE;
	}

	return sh_launch_job(job);
}

int sh_repl(void) {
	int status = 0;
	do {
		sh_do_job_notifications();
		sh_print_prompt();
		char	  *line = sh_read_line();
		command_t *cmd = sh_new_command(line);
		status = sh_evaluate(cmd);
	} while (status == SH_REPL_CONTINUE);

	return status;
}
