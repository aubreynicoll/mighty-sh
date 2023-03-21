#ifndef __SH_SIGNAL_INCLUDED__
#define __SH_SIGNAL_INCLUDED__

#include "wrapped.h"

#define sh_sig_ign_all(...)                           \
	{                                             \
		int *sigs = (int[]){__VA_ARGS__, -1}; \
		for (int i = 0; sigs[i] != -1; ++i) { \
			sh_signal(sigs[i], SIG_IGN);  \
		}                                     \
	}

#define sh_sig_dfl_all(...)                           \
	{                                             \
		int *sigs = (int[]){__VA_ARGS__, -1}; \
		for (int i = 0; sigs[i] != -1; ++i) { \
			sh_signal(sigs[i], SIG_DFL);  \
		}                                     \
	}

#endif
