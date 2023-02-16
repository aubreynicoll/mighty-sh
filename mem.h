#include <stddef.h>

#define sh_free_all(...)                                        \
	{                                                       \
		int    stopper;                                 \
		void **vec = (void *[]){__VA_ARGS__, &stopper}; \
		for (int i = 0; vec[i] != &stopper; ++i) {      \
			sh_free(vec[i]);                        \
		}                                               \
	}

void *sh_malloc(size_t size);
void *sh_realloc(void *ptr, size_t size);
void  sh_free(void *ptr);
