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

static int sh_evaluate(command_t *cmd);

static int sh_evaluate(command_t *cmd) {
	if (cmd->argv[0] == NULL) {
		/* user entered an empty line */
		return SH_REPL_CONTINUE;
	}

	/* run as builtin if found */
	builtin_t builtin = sh_get_builtin(cmd->argv[0]);
	if (builtin) {
		return builtin(cmd->argv);
	}

	/* else create & launch job */
	job_t *job = sh_new_job(cmd);
	if (!job) {
		/* probably hit max jobs */
		return SH_REPL_CONTINUE;
	}
	sh_launch_job(job);

	return SH_REPL_CONTINUE;
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
