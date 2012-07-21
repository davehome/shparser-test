#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shp.h"

static char *
get_input(void *context)
{
	char *tmp;
	shp_context *ctx = context;
	memset(ctx->data, 0, sizeof(char));
	tmp = fgets(ctx->data, BUFSIZ, ctx->fp);
	return (tmp == NULL ? 0 : tmp);
}

int
main(int argc, char **argv)
{
	shp_context ctx;
	shp_scanner scanner;
	shp_status status = 0;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s FILE\n", argv[0]);
		return (EXIT_FAILURE);
	}

	memset(&ctx, 0, sizeof(shp_context));
	ctx.filename = argv[1];
	ctx.fp = fopen(ctx.filename, "r");

	if (!ctx.fp) {
		status = (shp_status)errno;
		fprintf(stdout, "Can't open template:  %s\n", strerror(status));
		return (status);
	}

	shp_init(&scanner, get_input, &ctx);

	while (status == SHP_SCANNING)
		status = shp_scan(&scanner);

	if (status == SHP_ERROR)
		printf("%s\n", scanner.error);

	fclose(ctx.fp);

	return ((int)status - 1);
}
