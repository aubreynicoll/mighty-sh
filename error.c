#include "error.h"

#include "libc.h"

void sh_fatal_error(void) {
	perror("mighty-sh");
	exit(EXIT_FAILURE);
}
