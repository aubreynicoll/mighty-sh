typedef struct config_t config_t;
struct config_t {
	char *shell_name;
};

extern config_t sh_config;

void sh_init_config(int argc, char **argv);
