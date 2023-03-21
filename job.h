#ifndef __SH_JOB_INCLUDED__
#define __SH_JOB_INCLUDED__

#include <sys/types.h>
#include <termios.h>

#include "parser.h"

enum job_status { NOT_STARTED, RUNNING, STOPPED, EXITED };

typedef struct job_t {
	pid_t		pid;
	pid_t		pgid;
	command_t      *cmd;
	enum job_status status;
	int		notified;
	struct termios	tmodes;
} job_t;

/**
 * Assumes ownership of its arguments.
 *
 * returns a pointer to the job with the given cmd. The job is NOT_STARTED and
 * other fields such as .pid have not been set
 */
job_t *sh_new_job(command_t *cmd);

/**
 * This function frees the job_t returned by sh_new_job()
 */
void sh_free_job(job_t *job);

/**
 * find a job by pid
 */
job_t *sh_find_job_pid(pid_t pid);

/**
 * wait for the job until stopped or exited
 */
void sh_wait_for_job(job_t *job);

void sh_print_job_info(job_t *job);

void sh_print_jobs(void);

void sh_do_job_notifications(void);

void sh_move_job_bg(job_t *job, int continue_job);

void sh_move_job_fg(job_t *job, int continue_job);

#endif
