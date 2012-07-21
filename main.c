#include <stdio.h>
#include <string.h>
#include "shp.h"


int
main(int argc, char **argv)
{
	int rv;
	shp_context ctx;
	shp_cb cb;
	shp_scanner scanner;
	shp_status status = 0;

	memset(&ctx, 0, sizeof(shp_context));

	if ((rv = shp_getopt(&ctx, argc, argv)) != 0)
		return (rv);

	cb.scan = shp_file_input;
	shp_init(&scanner, &cb, &ctx);

	while (status == SHP_SCANNING)
		status = shp_scan(&scanner);

	if (status == SHP_ERROR) {
		fflush(NULL);
		fprintf(stderr, "%s\n", scanner.error);
	}

	fclose(ctx.fp);

	return ((int)status - 1);
}
