#include <stdio.h>
#include <stdlib.h>
#include "shp.h"

shp_status
shp_scan_sourced(const char *filename)
{
	int rv;
	shp_scanner sc;
	shp_context ctx;
	shp_cb cb;
	shp_status status = 0;

	if ((rv = shp_context_script_file(&ctx, filename)) != 0)
		exit(rv);

	shp_init(&sc, &cb, &ctx);

	while (status == SHP_SCANNING)
		status = shp_scan(&sc);

	if (status == SHP_ERROR) {
		fflush(NULL);
		fprintf(stderr, "%s\n", sc.error);
	}

	shp_context_end(&ctx);

	return status;
}
