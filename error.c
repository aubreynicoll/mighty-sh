#include "error.h"

#include "config.h"
#include "libc.h"
#include "util.h"

void sh_unix_error(char *message) {
	message = message ? message : sh_config.shell_name;
	perror(message);
}

noreturn void sh_fatal_unix_error(char *message) {
	message = message ? message : sh_config.shell_name;
	perror(message);
	exit(EXIT_FAILURE);
}
