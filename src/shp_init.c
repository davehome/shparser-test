#include <string.h>
#include "shp.h"

void
shp_init(shp_scanner *scanner, shp_cb *cb, shp_context *ctx)
{
	scanner = memset(scanner, 0, sizeof(shp_scanner));
	scanner->ctx = ctx;
	scanner->cb = cb; /* gets rid of useless gcc warning */
	scanner->state = STATE_GENERAL;
	scanner->next_state = STATE_GENERAL;
	memset(scanner->ctx->key, 0, BUFSIZ);
	memset(scanner->ctx->val, 0, BUFSIZ);
	memset(scanner->ctx->ref, 0, BUFSIZ);
	memset(scanner->ctx->cmd, 0, BUFSIZ);
	scanner->ctx->klen = 0;
	scanner->ctx->vlen = 0;
	scanner->ctx->rlen = 0;
	scanner->ctx->clen = 0;
	scanner->ctx->line = 1;
	scanner->ctx->quoting = false;
	scanner->ctx->commanding = false;
}
