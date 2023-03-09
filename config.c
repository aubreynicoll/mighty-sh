#include "config.h"

#include "libc.h"
#include "util.h"

config_t sh_config;

void sh_init_config(int argc unused, char **argv) {
	sh_config.shell_name = argv[0];
}
