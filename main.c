#include "config.h"
#include "repl.h"
#include "util.h"

int main(int argc, char **argv) {
	/* initialization */
	sh_init_config(argc, argv);

	/* read, evaluate, print loop */
	sh_repl();
}
