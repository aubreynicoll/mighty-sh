#ifndef __SH_LOGGER_INCLUDED__
#define __SH_LOGGER_INCLUDED__

#ifdef DEBUG_MODE
#define sh_debug_print(...) \
	{ printf(__VA_ARGS__); }
#else
#define sh_debug_print(...)
#endif

#endif
