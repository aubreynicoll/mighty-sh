#include "config.h"

#include "error.h"
#include "libc.h"
#include "signal.h"
#include "util.h"
#include "wrapped.h"

config_t sh_config;

void sh_init_config(int argc unused, char **argv) {
	sh_config.shell_name = argv[0];

	/* handle input file */
	if (argc > 1) {
		/* if a filename has been passed, we redirect STDIN to read from
		 * that file */
		char *path = argv[1];
		int   fd = sh_open(path, O_RDONLY);
		sh_dup2(fd, STDIN_FILENO);
		sh_close(fd);
	}

	sh_config.shell_pgid = getpgrp(); /* assigned pgid */
	sh_config.shell_terminal = STDIN_FILENO;
	sh_config.shell_is_interactive = isatty(sh_config.shell_terminal);

	if (sh_config.shell_is_interactive) {
		/* stop self until sh pgid is foreground pgid */
		while (sh_tcgetpgrp(sh_config.shell_terminal) !=
		       sh_config.shell_pgid) {
			sh_kill(-sh_config.shell_pgid, SIGTTIN);
		}

		/* sh ignores job control signals */
		sh_sig_ign_all(SIGINT, SIGQUIT, SIGTSTP, SIGTTIN, SIGTTOU);

		/* move sh process to its own pgid */
		sh_setpgid(0, 0);
		sh_config.shell_pgid = getpid();

		/* take control of terminal & save terminal modes */
		sh_tcsetpgrp(sh_config.shell_terminal, sh_config.shell_pgid);
		sh_tcgetattr(sh_config.shell_terminal, &sh_config.shell_tmodes);
	}
}
