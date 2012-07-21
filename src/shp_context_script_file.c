#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "shp.h"

int
shp_context_script_file(shp_context *ctx, const char *filename)
{
	int rv = 0;

	ctx->filename = filename;
	ctx->fp = fopen(ctx->filename, "r");
	if (errno != 0 || ctx->fp == NULL) {
		rv = errno;
		fprintf(stdout, "Can't open shell script: `%s', %s.\n",
			ctx->filename, strerror(rv));
	}
	return (rv);
}
