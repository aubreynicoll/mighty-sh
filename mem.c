#include "mem.h"

#include "config.h"
#include "error.h"
#include "libc.h"

char *sh_strdup(const char *str) {
	char *dup = strdup(str);
	if (!dup) {
		sh_fatal_unix_error(NULL);
	}
	return dup;
}

void *sh_malloc(size_t n, size_t size) {
	if (size && SIZE_MAX / size < n) {
		sh_fatal_error("%s: overflowed during allocation\n",
			       sh_config.shell_name);
	}

	void *ptr = malloc(n * size);
	if (!ptr) {
		sh_fatal_unix_error(NULL);
	}
	return ptr;
}

void *sh_realloc(void *ptr, size_t n, size_t size) {
	if (size && SIZE_MAX / size < n) {
		sh_fatal_error("%s: overflowed during allocation\n",
			       sh_config.shell_name);
	}

	void *new_ptr = realloc(ptr, n * size);
	if (!new_ptr) {
		sh_fatal_unix_error(NULL);
	}
	return new_ptr;
}

void sh_free(void *ptr) {
	free(ptr);
}
