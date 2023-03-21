#ifndef __SH_UTIL_INCLUDED__
#define __SH_UTIL_INCLUDED__

#ifdef __GNUC__
#define unused	 __attribute__((unused))
#define noreturn __attribute__((noreturn))
#else
#define unused
#define noreturn
#endif

#endif
