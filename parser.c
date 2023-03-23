#include "parser.h"

#include "libc.h"
#include "mem.h"

#define SH_TOK_BUFSIZE	  16
#define SH_TOK_DELIMITERS " \n\t\f\r\v"

command_t *sh_new_command(char *line) {
	size_t tokens_len = SH_TOK_BUFSIZE;
	size_t i = 0;

	command_t *cmd = sh_malloc(1, sizeof(command_t));
	cmd->line = line;
	cmd->dup = sh_strdup(cmd->line);
	cmd->argv = sh_malloc(tokens_len, sizeof(char *));
	cmd->bg = 0;

	char *token = strtok(cmd->dup, SH_TOK_DELIMITERS);
	while (token) {
		cmd->argv[i++] = token;

		if (i == tokens_len) {
			tokens_len *= 2;
			cmd->argv =
			    sh_realloc(cmd->argv, tokens_len, sizeof(char *));
		}

		token = strtok(NULL, SH_TOK_DELIMITERS);
	}

	if (i == 0) {
		/* no tokens: cleanup and return NULL*/
		sh_free_command(cmd);
		return NULL;
	}

	if (!strcmp("&", cmd->argv[i - 1])) {
		cmd->bg = 1;
		--i; /* overwrite "&" token with NULL */
	}

	cmd->argv[i] = NULL;

	return cmd;
}

void sh_free_command(command_t *cmd) {
	sh_free_all(cmd->line, cmd->dup, cmd->argv, cmd);
}
