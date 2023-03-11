#ifndef __SH_BUILTINS_INCLUDED__
#define __SH_BUILTINS_INCLUDED__

typedef int (*builtin_t)(char **);

builtin_t sh_get_builtin(char *name);

#endif
