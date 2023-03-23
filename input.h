#ifndef __SH_INPUT_INCLUDED__
#define __SH_INPUT_INCLUDED__

/**
 * Due to lack of good in-band signalling when dealing with signed integers,
 * sh_get_long() returns type LongResult. If the function succeeded, .err will
 * be set to zero, and .ok will contain the parsed value.
 */
typedef struct LongResult {
	int  err;
	long ok;
} LongResult;

LongResult sh_get_long(char *str);
void	   sh_print_prompt(void);
char	  *sh_read_line(void);

#endif
