#include "builtins.h"

#include "error.h"
#include "input.h"
#include "job.h"
#include "libc.h"
#include "mem.h"
#include "repl.h"
#include "util.h"
#include "wrapped.h"

#define SH_KEYVAL_DELIMS "="

typedef struct builtin_desc_s builtin_desc_t;
struct builtin_desc_s {
	char	 *name;
	builtin_t f;
};

static void	     sh_cd(char **args);
static void	     sh_export(char **args);
static void	     sh_jobs(char **args);
static void	     sh_fg(char **args);
static void	     sh_bg(char **args);
static void	     sh_help(char **args);
noreturn static void sh_exit(char **args);

static builtin_desc_t builtins[] = {
    {.name = "cd", .f = &sh_cd},     {.name = "export", .f = &sh_export},
    {.name = "jobs", .f = &sh_jobs}, {.name = "fg", .f = &sh_fg},
    {.name = "bg", .f = &sh_bg},     {.name = "help", .f = &sh_help},
    {.name = "exit", .f = &sh_exit}, {.name = NULL, .f = NULL}};

static void sh_cd(char **args) {
	char *dir = args[1];
	if (!dir) {
		dir = getenv("HOME");
	}

	int status = chdir(dir);
	if (status < 0) {
		sh_unix_error("cd");
	}
}

static void sh_export(char **args) {
	for (char **var = &args[1]; *var; ++var) {
		/* copy so we don't mangle args */
		char *dup = sh_strdup(*var);

		/* extract key & value*/
		char *key = strtok(dup, SH_KEYVAL_DELIMS);
		char *value = strtok(NULL, SH_KEYVAL_DELIMS);

		/* if value is NULL, set to empty string */
		value = value ? value : "";

		sh_setenv(key, value, 1);

		/* free copied string*/
		sh_free(dup);
	}
}

static void sh_jobs(char **args unused) {
	sh_print_jobs();
}

static void sh_fg(char **args) {
	if (!args[1]) {
		sh_error("usage: %s <pid>\n", args[0]);
		return;
	}

	LongResult result = sh_get_long(args[1]);
	if (result.err) {
		sh_error("%s: could not parse pid\n", args[0]);
		return;
	}
	pid_t pid = result.ok;

	job_t *job = sh_find_job_pid(pid);
	if (!job) {
		sh_error("%s: job not found\n", args[0]);
		return;
	}

	sh_move_job_fg(job, 1);
}

static void sh_bg(char **args) {
	if (!args[1]) {
		sh_error("usage: %s <pid>\n", args[0]);
		return;
	}

	LongResult result = sh_get_long(args[1]);
	if (result.err) {
		sh_error("%s: could not parse pid\n", args[0]);
		return;
	}
	pid_t pid = result.ok;

	job_t *job = sh_find_job_pid(pid);
	if (!job) {
		sh_error("%s: job not found\n", args[0]);
		return;
	}

	sh_move_job_bg(job, 1);
}

static void sh_help(char **args unused) {
	printf(
	    "Mighty Shell! by Aubrey Nicoll\n"
	    "\n"
	    "Builtins:\n"
	    "cd <directory> - change directory\n"
	    "help - you already figured out this one!\n"
	    "exit - quit the shell\n"
	    "\n"
	    "Please see the man pages for information about non-builtin commands.\n");
}

noreturn static void sh_exit(char **args unused) {
	exit(EXIT_SUCCESS);
}

builtin_t sh_get_builtin(char *name) {
	for (int i = 0; builtins[i].name; ++i) {
		if (!strcmp(name, builtins[i].name)) {
			return builtins[i].f;
		}
	}
	return NULL;
}
