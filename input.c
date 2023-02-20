#include "input.h"

#include "error.h"
#include "libc.h"

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

		sh_fatal_error(NULL);
	}

	return line;
}
