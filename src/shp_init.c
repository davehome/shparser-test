#include <string.h>
#include "shp.h"

void
shp_init(shp_scanner *scanner, shp_cb *cb, shp_context *ctx)
{
	memset(ctx->key, 0, BUFSIZ);
	memset(ctx->val, 0, BUFSIZ);
	memset(ctx->ref, 0, BUFSIZ);
	memset(ctx->cmd, 0, BUFSIZ);
	memset(ctx->src, 0, BUFSIZ);

	ctx->klen = ctx->vlen = ctx->rlen = ctx->clen = ctx->slen = 0;
	ctx->quoting = ctx->commanding = ctx->sourcing = false;
	ctx->line = 1;

	cb->scan = shp_file_input;

	scanner = memset(scanner, 0, sizeof(shp_scanner));
	scanner->state = STATE_GENERAL;
	scanner->next_state = STATE_GENERAL;
	scanner->ctx = ctx;
	scanner->cb = cb;
}
