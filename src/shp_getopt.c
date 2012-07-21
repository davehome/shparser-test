#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "shp.h"

int
shp_getopt(shp_context *ctx, int argc, char **argv)
{
	int opt, rv;
	const char *progname;

	progname = argv[0];

	while ((opt = getopt(argc, argv, "h")) != -1) {
		switch (opt) {
			case 'h':
				return shp_show_usage(progname);
				break;
			default:
				return shp_show_usage(progname);
				break;
		}
	}

	argc -= optind;
	argv += optind;

	if (argc == 0) {
		ctx->filename = "(stdin)";
		ctx->fp = stdin;
	} else {
		ctx->filename = argv[0];
		ctx->fp = fopen(ctx->filename, "r");
		if (errno != 0 || ctx->fp == NULL) {
			rv = errno;
			fprintf(stdout, "Can't open template: `%s', %s.\n",
				ctx->filename, strerror(rv));
			return (rv);
		}
	}

	return 0;
}
