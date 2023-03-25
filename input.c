#include "input.h"

#include "config.h"
#include "error.h"
#include "libc.h"

#define SH_LINE_PROMPT "mighty-shell> "

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
		/* perform normal exit on EOF */
		if (feof(stdin)) {
			if (sh_config.shell_is_interactive) {
				printf("\nexit\n");
			}
			exit(EXIT_SUCCESS);
		}

		/* a real error occurred */
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
