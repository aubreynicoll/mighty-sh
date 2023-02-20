#include "main.h"

#include "repl.h"
#include "util.h"

config_t sh_config;

int main(int argc unused, char **argv) {
	/* initialization */
	sh_config.shell_name = argv[0];

	/* read, evaluate, print loop */
	sh_repl();
}
