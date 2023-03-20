#include "builtins.h"

#include "error.h"
#include "input.h"
#include "job.h"
#include "libc.h"
#include "repl.h"
#include "util.h"

typedef struct builtin_desc_s builtin_desc_t;
struct builtin_desc_s {
	char	 *name;
	builtin_t f;
};

static int sh_cd(char **args);
static int sh_jobs(char **args);
static int sh_fg(char **args);
static int sh_bg(char **args);
static int sh_help(char **args);
static int sh_exit(char **args);

static builtin_desc_t builtins[] = {
    {.name = "cd", .f = &sh_cd},     {.name = "jobs", .f = &sh_jobs},
    {.name = "fg", .f = &sh_fg},     {.name = "bg", .f = &sh_bg},
    {.name = "help", .f = &sh_help}, {.name = "exit", .f = &sh_exit},
    {.name = NULL, .f = NULL}};

static int sh_cd(char **args) {
	char *dir = args[1];
	if (!dir) {
		dir = getenv("HOME");
	}

	int status = chdir(dir);
	if (status < 0) {
		sh_unix_error("cd");
	}

	return SH_REPL_CONTINUE;
}

static int sh_jobs(char **args unused) {
	sh_print_jobs();

	return SH_REPL_CONTINUE;
}

static int sh_fg(char **args) {
	if (!args[1]) {
		sh_error("usage: %s <pid>\n", args[0]);
		return SH_REPL_CONTINUE;
	}

	LongResult result = sh_get_long(args[1]);
	if (result.err) {
		sh_error("%s: could not parse pid\n", args[0]);
		return SH_REPL_CONTINUE;
	}
	pid_t pid = result.ok;

	job_t *job = sh_find_job_pid(pid);
	if (!job) {
		sh_error("%s: job not found\n", args[0]);
		return SH_REPL_CONTINUE;
	}

	sh_move_job_fg(job, 1);

	return SH_REPL_CONTINUE;
}

static int sh_bg(char **args) {
	if (!args[1]) {
		sh_error("usage: %s <pid>\n", args[0]);
		return SH_REPL_CONTINUE;
	}

	LongResult result = sh_get_long(args[1]);
	if (result.err) {
		sh_error("%s: could not parse pid\n", args[0]);
		return SH_REPL_CONTINUE;
	}
	pid_t pid = result.ok;

	job_t *job = sh_find_job_pid(pid);
	if (!job) {
		sh_error("%s: job not found\n", args[0]);
		return SH_REPL_CONTINUE;
	}

	sh_move_job_bg(job, 1);

	return SH_REPL_CONTINUE;
}

static int sh_help(char **args unused) {
	printf(
	    "Mighty Shell! by Aubrey Nicoll\n"
	    "\n"
	    "Builtins:\n"
	    "cd <directory> - change directory\n"
	    "help - you already figured out this one!\n"
	    "exit - quit the shell\n"
	    "\n"
	    "Please see the man pages for information about non-builtin commands.\n");
	return SH_REPL_CONTINUE;
}

static int sh_exit(char **args unused) {
	return SH_REPL_EXIT;
}

builtin_t sh_get_builtin(char *name) {
	for (int i = 0; builtins[i].name; ++i) {
		if (!strcmp(name, builtins[i].name)) {
			return builtins[i].f;
		}
	}
	return NULL;
}
