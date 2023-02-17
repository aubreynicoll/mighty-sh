typedef int (*builtin_t)(char **);

builtin_t sh_get_builtin(char *name);
