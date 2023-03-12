#ifndef __SH_ERROR_INCLUDED__
#define __SH_ERROR_INCLUDED__

#include <stdlib.h>

#include "util.h"

#define sh_error(...) \
	{ fprintf(stderr, __VA_ARGS__); }

#define sh_fatal_error(...)                   \
	{                                     \
		fprintf(stderr, __VA_ARGS__); \
		exit(EXIT_FAILURE);           \
	}

void	      sh_unix_error(char *message);
noreturn void sh_fatal_unix_error(char *message);

#endif
