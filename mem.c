#include "mem.h"

#include "error.h"
#include "libc.h"

void *sh_malloc(size_t size) {
	void *ptr = malloc(size);
	if (!ptr) {
		sh_fatal_error(NULL);
	}
	return ptr;
}

void *sh_realloc(void *ptr, size_t size) {
	void *new_ptr = realloc(ptr, size);
	if (!new_ptr) {
		sh_fatal_error(NULL);
	}
	return new_ptr;
}

void sh_free(void *ptr) {
	free(ptr);
}
