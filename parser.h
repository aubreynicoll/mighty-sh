#ifndef __SH_PARSER_INCLUDED__
#define __SH_PARSER_INCLUDED__

#define SH_TOK_BUFSIZE	  16
#define SH_TOK_DELIMITERS " \n\t\f\r\v"

char **sh_parse_tokens(char *line);

#endif
