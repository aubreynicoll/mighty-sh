#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define SH_LINE_PROMPT	  "mighty-shell> "
#define SH_TOK_BUFSIZE	  1024
#define SH_TOK_DELIMITERS " \n\t\f\r\v"

void   sh_fatal(void);
void   sh_print_prompt(void);
char  *sh_read_line(void);
char **sh_parse_tokens(char *line);
int    sh_evaluate(char **tokens);
int    sh_repl(void);

void sh_fatal(void) {
	perror("mighty-sh");
	exit(EXIT_FAILURE);
}

void sh_print_prompt(void) {
	printf(SH_LINE_PROMPT);
	fflush(stdout);
}

char *sh_read_line(void) {
	char  *line = NULL;
	size_t len;
	int    status;

	status = getline(&line, &len, stdin);
	if (status == -1) {
		if (feof(stdin)) {
			printf("\nexit\n");
			exit(EXIT_SUCCESS);
		}

		sh_fatal();
	}

	return line;
}

char **sh_parse_tokens(char *line) {
	char **tokens;
	size_t tokens_len = SH_TOK_BUFSIZE;
	char  *token;
	size_t i = 0;

	tokens = malloc(tokens_len * sizeof(char *));
	if (!tokens) {
		sh_fatal();
	}

	token = strtok(line, SH_TOK_DELIMITERS);
	while (token) {
		tokens[i++] = token;

		if (i == tokens_len) {
			tokens_len *= 2;
			tokens = realloc(tokens, tokens_len);
			if (!tokens) {
				sh_fatal();
			}
		}

		token = strtok(NULL, SH_TOK_DELIMITERS);
	}

	tokens[i] = NULL;

	return tokens;
}

int sh_evaluate(char **tokens) {
	int pid = fork();
	if (pid == 0) {
		// child
		execvp(tokens[0], tokens);
		perror(tokens[0]);  // execvp only returns on error
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		// error
		sh_fatal();
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
		free(tokens);
		free(line);
	} while (!status);

	return status;
}

int main(void) {
	sh_repl();
}
