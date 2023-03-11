#ifndef __SH_CONFIG_INCLUDED__
#define __SH_CONFIG_INCLUDED__

typedef struct config_t config_t;
struct config_t {
	char *shell_name;
};

extern config_t sh_config;

void sh_init_config(int argc, char **argv);

#endif
