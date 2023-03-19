#include "job.h"

#include <string.h>
#include <sys/wait.h>

#include "config.h"
#include "error.h"
#include "parser.h"

#define MAXJOBS 16

static job_t  jobs[MAXJOBS];
static size_t jobs_size;

job_t *sh_new_job(command_t *cmd) {
	if (jobs_size == MAXJOBS) {
		sh_error("%s: max jobs reached\n", sh_config.shell_name);
		return NULL;
	}

	job_t *job = &jobs[jobs_size];
	job->cmd = cmd;

	++jobs_size;

	return job;
}

void sh_free_job(job_t *job) {
	sh_free_command(job->cmd);

	--jobs_size;

	if (job != &jobs[jobs_size]) {
		*job = jobs[jobs_size];
	}

	memset(&jobs[jobs_size], 0, sizeof(job_t));
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

int sh_update_job_status(job_t *job, int wstatus) {
	if (job == NULL) {
		return -1;
	}

	if (WIFSTOPPED(wstatus)) {
		job->status = STOPPED;
	} else if (WIFEXITED(wstatus)) {
		job->status = EXITED;
	}

	return 0;
}

void sh_wait_for_job(job_t *job) {
	int wstatus;

	do {
		waitpid(job->pid, &wstatus, 0);
	} while (!sh_update_job_status(job, wstatus) &&
		 !(job->status == STOPPED || job->status == EXITED));
}

void sh_check_bg_jobs(void) {
	if (!jobs_size) {
		return;
	}

	int   wstatus;
	pid_t pid;

	do {
		pid = waitpid(WAIT_ANY, &wstatus, WNOHANG);
		if (!pid) {
			/* no reporting processes */
			break;
		}
	} while (!sh_update_job_status(sh_find_job_pid(pid), wstatus));
}
