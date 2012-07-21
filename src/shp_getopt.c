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

	rv = 0;
	progname = argv[0];

	while ((opt = getopt(argc, argv, "h")) != -1) {
		switch (opt) {
		case 'h':
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
		rv = shp_context_script_file(ctx, argv[0]);
	}

	return rv;
}
