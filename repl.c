#include "repl.h"

#include "builtins.h"
#include "error.h"
#include "input.h"
#include "libc.h"
#include "mem.h"
#include "parser.h"

static sh_exec(char **args);
static sh_evaluate(char **args);

static sh_exec(char **args) {
	int pid = fork();
	if (pid == 0) {
		// child
		execvp(args[0], args);
		perror(args[0]);  // execvp only returns on error
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		// error
		sh_fatal_error();
	} else {
		// parent
		int wstatus;
		do {
			waitpid(pid, &wstatus, WUNTRACED);
		} while (!(WIFEXITED(wstatus) || WIFSIGNALED(wstatus)));
	}

	return SH_REPL_CONTINUE;
}

static int sh_evaluate(char **args) {
	builtin_t builtin = sh_get_builtin(args[0]);
	if (builtin) {
		return builtin(args);
	}
	return sh_exec(args);
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
