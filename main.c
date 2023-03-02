#include "config.h"
#include "repl.h"
#include "util.h"

int main(int argc unused, char **argv) {
	/* initialization */
	sh_config.shell_name = argv[0];

	/* read, evaluate, print loop */
	sh_repl();
}
