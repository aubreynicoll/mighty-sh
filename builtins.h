#ifndef __SH_BUILTINS_INCLUDED__
#define __SH_BUILTINS_INCLUDED__

typedef void (*builtin_t)(char **);

builtin_t sh_get_builtin(char *name);

#endif
