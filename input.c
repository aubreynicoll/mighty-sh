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

		sh_fatal_unix_error(NULL);
	}

	return line;
}

LongResult sh_get_long(char *str) {
	char *endptr;

	errno = 0;

	long value = strtol(str, &endptr, 0);

	if (errno != 0 || str == endptr) {
		return (LongResult){.err = 1};
	}

	return (LongResult){.ok = value};
}
