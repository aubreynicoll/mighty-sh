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

static void sh_evaluate(command_t *cmd);

static void sh_evaluate(command_t *cmd) {
	if (cmd == NULL) {
		/* user entered an empty line or whitespace */
		return;
	}

	/* run as builtin if found */
	builtin_t builtin = sh_get_builtin(cmd->argv[0]);
	if (builtin) {
		builtin(cmd->argv);
		sh_free_command(cmd);
		return;
	}

	/* else create & launch job */
	job_t *job = sh_new_job(cmd);
	if (!job) {
		/* probably hit max jobs */
		return;
	}
	sh_launch_job(job);
}

void sh_repl(void) {
	for (;;) {
		sh_do_job_notifications();
		sh_print_prompt();
		char	  *line = sh_read_line();
		command_t *cmd = sh_new_command(line);
		sh_evaluate(cmd);
	}
}
