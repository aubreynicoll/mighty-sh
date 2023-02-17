#include "builtins.h"

#include "libc.h"
#include "repl.h"

typedef struct builtin_desc_s builtin_desc_t;
struct builtin_desc_s {
	char	 *name;
	builtin_t f;
};

static int sh_cd(char **args);
static int sh_help(char **args);
static int sh_exit(char **args);

static builtin_desc_t builtins[] = {{.name = "cd", .f = &sh_cd},
				    {.name = "help", .f = &sh_help},
				    {.name = "exit", .f = &sh_exit},
				    {.name = NULL, .f = NULL}};

static int sh_cd(char **args) {
	char *dir = args[1];
	if (!dir) {
		dir = getenv("HOME");
	}

	int status = chdir(dir);
	if (status < 0) {
		perror("cd");
	}

	return SH_REPL_CONTINUE;
}

static int sh_help(char **args) {
	printf(
	    "Mighty Shell! by Aubrey Nicoll\n"
	    "\n"
	    "Builtins:\n"
	    "cd <directory> - change directory\n"
	    "help - you already figured out this one!\n"
	    "exit - quit the shell\n"
	    "\n"
	    "Please see the man pages for information about non-builtin commands.\n");
	return SH_REPL_CONTINUE;
}

static int sh_exit(char **args) {
	return SH_REPL_EXIT;
}

builtin_t sh_get_builtin(char *name) {
	for (int i = 0; builtins[i].name; ++i) {
		if (!strcmp(name, builtins[i].name)) {
			return builtins[i].f;
		}
	}
	return NULL;
}
