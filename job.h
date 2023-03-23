#ifndef __SH_JOB_INCLUDED__
#define __SH_JOB_INCLUDED__

#include <sys/types.h>
#include <termios.h>

#include "parser.h"

enum job_status { NOT_STARTED, RUNNING, STOPPED, EXITED };

typedef struct job_t {
	pid_t		pid;
	pid_t		pgid;
	/* the parsed command. job_t assumes ownership of this and will attempt
	 * to free this pointer during its cleanup routine */
	command_t      *cmd;
	enum job_status status;
	/* when the user has been notified of a stopped job, this should be set
	 * to 1, else 0 */
	int		notified;
	/* if the job alters the terminal modes, they will be saved here when it
	 * gets moved to the background from the foreground */
	struct termios	tmodes;
} job_t;

/**
 * Assumes ownership of its arguments. cmd must be malloc'd, as sh_free_job()
 * will attempt to free this pointer
 *
 * returns an unmalloc'd pointer to the job with the given cmd. The job is
 * NOT_STARTED and other fields such as .pid have not been set. The job can be
 * launched using sh_launch_job().
 */
job_t *sh_new_job(command_t *cmd);

/**
 * This function frees the job_t returned by sh_new_job().
 */
void sh_free_job(job_t *job);

/**
 * find a job by pid
 */
job_t *sh_find_job_pid(pid_t pid);

/* launches a job by calling fork() + execvp(). This function blocks until
 * SIGCHLD if the job is a foreground job. */
void sh_launch_job(job_t *job);

/**
 * block until job is STOPPED or EXITED.
 */
void sh_wait_for_job(job_t *job);

/* print the pid, status, and command issued for the job */
void sh_print_job_info(job_t *job);

/* call sh_print_job_info() for all jobs */
void sh_print_jobs(void);

/* This function checks the status of all jobs and notifies the user of any
 * stopped or exited jobs. Exited jobs are freed using sh_free_job(). */
void sh_do_job_notifications(void);

/* send a job to the background */
void sh_move_job_bg(job_t *job, int continue_job);

/* handle job as a foreground job, giving it the terminal and blocking until it
 * is EXITED or STOPPED. Once unblocked, it saves the job's terminal modes and
 * reclaims the terminal for the shell */
void sh_move_job_fg(job_t *job, int continue_job);

#endif
