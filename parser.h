#ifndef __SH_PARSER_INCLUDED__
#define __SH_PARSER_INCLUDED__

typedef struct command_t {
	/* the original line, used for displaying to the user */
	char  *cmd;
	/* a copy of the line used for tokenizing, which is mangled and useless
	 * on its own */
	char  *dup;
	/* NULL-terminated array of token pointers, used for evaluating the
	 * command */
	char **argv;
} command_t;

/**
 * Assumes ownership of its arguments.
 *
 * This function returns a malloc'd command_t that must be freed by the
 * accompanying function.
 */
command_t *sh_new_command(char *line);

/**
 * used to free the command_t created by the accompanying function.
 */
void sh_free_command(command_t *cmd);

#endif
