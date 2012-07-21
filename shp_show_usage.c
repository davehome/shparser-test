#include <stdio.h>
#include <stdlib.h>
#include "shp.h"

#define SHP_USAGE_FMT							\
"Usage:\n"								\
"  %s [-h] [FILE|[--]]\n\n"						\
" Option(s):\n"								\
"  -h		Show this message.\n\n"					\
" Argument(s):\n"							\
"  FILE		Read shell script from a file on disk.\n"		\
"  --		Read shell script from standard input.\n"		\
"		(note: this argument is optional)\n\n"

int
shp_show_usage(const char *progname)
{
	fprintf(stderr, SHP_USAGE_FMT, progname);
	return (EXIT_FAILURE);
}
