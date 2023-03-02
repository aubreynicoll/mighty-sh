#include "error.h"

#include "config.h"
#include "libc.h"

void sh_error(char *message) {
	message = message ? message : sh_config.shell_name;
	perror(message);
}

void sh_fatal_error(char *message) {
	message = message ? message : sh_config.shell_name;
	perror(message);
	exit(EXIT_FAILURE);
}
