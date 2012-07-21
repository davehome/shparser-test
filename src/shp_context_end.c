#include <stdio.h>
#include "shp.h"

void
shp_context_end(shp_context *ctx)
{
	fclose(ctx->fp);
}
