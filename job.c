#include "job.h"

#include "config.h"
#include "error.h"
#include "libc.h"
#include "logger.h"
#include "parser.h"
#include "signal.h"
#include "wrapped.h"

#define MAXJOBS 16

static job_t  jobs[MAXJOBS];
static size_t jobs_size;

static void  sh_check_bg_jobs(void);
static int   sh_mark_job_status(job_t *job, int wstatus);
static void  sh_continue_job(job_t *job);
static char *sh_job_status_str(job_t *job);

job_t *sh_new_job(command_t *cmd) {
	if (jobs_size == MAXJOBS) {
		sh_error("%s: max jobs reached\n", sh_config.shell_name);
		return NULL;
	}

	job_t *job = &jobs[jobs_size];
	*job = (job_t){.cmd = cmd, .tmodes = sh_config.shell_tmodes};

	++jobs_size;

	return job;
}

void sh_free_job(job_t *job) {
	if (jobs_size == 0) {
		sh_fatal_error("%s: attempted to free non-existent job\n",
			       sh_config.shell_name);
	}

	sh_free_command(job->cmd);

	--jobs_size;

	if (job != &jobs[jobs_size]) {
		*job = jobs[jobs_size];
	}

	/* default initialize static memory, ISO C 6.7.10 (22) */
	jobs[jobs_size] = (job_t){};
}

job_t *sh_find_job_pid(pid_t pid) {
	if (pid < 1) {
		return NULL;
	}

	for (size_t i = 0; i < jobs_size; ++i) {
		if (jobs[i].pid == pid) {
			return &jobs[i];
		}
	}

	return NULL;
}

void sh_launch_job(job_t *job) {
	if (job->status != NOT_STARTED) {
		sh_fatal_error("%s: job already launched\n",
			       sh_config.shell_name);
	}

	pid_t pid = sh_fork();
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
}

void sh_wait_for_job(job_t *job) {
	int wstatus;
	sh_waitpid(job->pid, &wstatus, WUNTRACED);
	sh_mark_job_status(job, wstatus);
}

void sh_print_job_info(job_t *job) {
	printf("[%ld] %s %s", (long)job->pid, sh_job_status_str(job),
	       job->cmd->line);
	fflush(stdout);
}

void sh_print_jobs(void) {
	for (size_t i = 0; i < jobs_size; ++i) {
		sh_print_job_info(&jobs[i]);
	}
}

void sh_do_job_notifications(void) {
	sh_check_bg_jobs();

	for (size_t i = 0; i < jobs_size; ++i) {
		job_t *job = &jobs[i];

		if (job->status == EXITED) {
			sh_print_job_info(job);
			sh_free_job(job);

			/* back up because sh_free_job() copies last element to
			 * freed spot */
			--i;
		} else if (job->status == STOPPED && !job->notified) {
			// print stopped
			job->notified = 1;
			sh_print_job_info(job);
		}
	}
}

void sh_move_job_bg(job_t *job, int continue_job) {
	if (continue_job) {
		sh_continue_job(job);
	}

	sh_print_job_info(job);
}

void sh_move_job_fg(job_t *job, int continue_job) {
	/* job controls terminal*/
	sh_tcsetpgrp(sh_config.shell_terminal, job->pgid);

	if (continue_job) {
		/* restore terminal modes when continuing job */
		sh_tcsetattr(sh_config.shell_terminal, TCSADRAIN, &job->tmodes);
		sh_continue_job(job);
		sh_print_job_info(job);
	}

	/* wait for STOPPED or EXITED */
	sh_wait_for_job(job);

	/* shell controls terminal */
	sh_tcsetpgrp(sh_config.shell_terminal, sh_config.shell_pgid);

	/* save job tmodes and restore shell tmodes */
	sh_tcgetattr(sh_config.shell_terminal, &job->tmodes);
	sh_tcsetattr(sh_config.shell_terminal, TCSADRAIN,
		     &sh_config.shell_tmodes);
}

/**
 * exhaustively check background jobs until no processes
 * ready to report
 */
static void sh_check_bg_jobs(void) {
	int   wstatus;
	pid_t pid;

	do {
		pid = sh_waitpid(WAIT_ANY, &wstatus, WUNTRACED | WNOHANG);
		if (pid < 1) {
			/* no reporting processes or ECHILD */
			break;
		}
	} while (!sh_mark_job_status(sh_find_job_pid(pid), wstatus));
}

/**
 * Called in conjuction with waitpid, the status passed to this function should
 * be the returned wait_status from waitpid
 */
static int sh_mark_job_status(job_t *job, int wstatus) {
	if (job == NULL) {
		return -1;
	}

	if (WIFSTOPPED(wstatus)) {
		job->status = STOPPED;
	} else {
		job->status = EXITED;
	}

	return 0;
}

static void sh_continue_job(job_t *job) {
	sh_kill(-job->pgid, SIGCONT);
	job->status = RUNNING;
	job->notified = 0;
}

static char *sh_job_status_str(job_t *job) {
	switch (job->status) {
		case NOT_STARTED:
			return "not started";
		case RUNNING:
			return "running";
		case STOPPED:
			return "stopped";
		case EXITED:
			return "exited";
		default:
			return "unknown";
	}
}
