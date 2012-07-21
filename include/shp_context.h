#ifndef SHP_CONTEXT_H
#define SHP_CONTEXT_H

#include <stdbool.h>
#include <stdio.h>

typedef struct _shp_context_t shp_context;

struct _shp_context_t {
	char data[BUFSIZ];
	char key[BUFSIZ];
	char val[BUFSIZ];
	char ref[BUFSIZ];
	char cmd[BUFSIZ];
	char src[BUFSIZ];
	char version[BUFSIZ];
	const char *filename;
	FILE *fp;
	size_t line, pos, klen, vlen, rlen, clen, slen;
	bool quoting;
	bool commanding;
	bool sourcing;
};

#endif /* SHP_CONTEXT_H */
