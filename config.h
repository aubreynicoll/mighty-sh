#ifndef __SH_CONFIG_INCLUDED__
#define __SH_CONFIG_INCLUDED__

#include <sys/types.h>
#include <termios.h>

typedef struct config_t config_t;
struct config_t {
	char	      *shell_name;
	pid_t	       shell_pgid;
	struct termios shell_tmodes;
	int	       shell_terminal;
	int	       shell_is_interactive;
};

extern config_t sh_config;

void sh_init_config(int argc, char **argv);

#endif
