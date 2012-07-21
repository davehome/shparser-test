#include <stdio.h>
#include <string.h>
#include "shp.h"

char *
shp_file_input(void *context)
{
	char *tmp;
	shp_context *ctx = context;
	memset(ctx->data, 0, sizeof(char));
	tmp = fgets(ctx->data, BUFSIZ, ctx->fp);
	return (tmp == NULL ? 0 : tmp);
}
