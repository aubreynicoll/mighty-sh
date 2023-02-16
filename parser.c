#include "parser.h"

#include "libc.h"
#include "mem.h"

char **sh_parse_tokens(char *line) {
	char **tokens;
	size_t tokens_len = SH_TOK_BUFSIZE;
	char  *token;
	size_t i = 0;

	tokens = sh_malloc(tokens_len * sizeof(char *));

	token = strtok(line, SH_TOK_DELIMITERS);
	while (token) {
		tokens[i++] = token;

		if (i == tokens_len) {
			tokens_len *= 2;
			tokens = sh_realloc(tokens, tokens_len);
		}

		token = strtok(NULL, SH_TOK_DELIMITERS);
	}

	tokens[i] = NULL;

	return tokens;
}
