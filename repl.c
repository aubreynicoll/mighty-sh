#include "repl.h"

#include "error.h"
#include "input.h"
#include "libc.h"
#include "mem.h"
#include "parser.h"

int sh_evaluate(char **tokens) {
	int pid = fork();
	if (pid == 0) {
		// child
		execvp(tokens[0], tokens);
		perror(tokens[0]);  // execvp only returns on error
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

	return 0;
}

int sh_repl(void) {
	int status = 0;
	do {
		sh_print_prompt();
		char  *line = sh_read_line();
		char **tokens = sh_parse_tokens(line);
		status = sh_evaluate(tokens);
		sh_free_all(tokens, line);
	} while (!status);

	return status;
}
