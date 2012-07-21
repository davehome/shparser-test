#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shp.h"

shp_status
shp_scan(shp_scanner *scanner)
{
	shp_context *ctx;
	shp_cb *cb;
	shp_state *state;
	shp_state *next_state;
	shp_chars category;
	size_t selector;
	const char *tmp;
	const char *data;
	char c;

	ctx = scanner->ctx;
	cb = scanner->cb;
	data = cb->scan(ctx);
	if (data == 0) return (SHP_DONE);

	state = &scanner->state;
	next_state = &scanner->next_state;

	for (ctx->pos = 0; (c = *data++) != 0; ctx->pos++) {
		category = shp_translate(c);
		selector = *state | category;
		switch (selector) {
		case STATE_GENERAL | CHAR_DOT:
		case STATE_IDENTIFIER | CHAR_DOT:
			*next_state = STATE_SOURCED;
			break;
		case STATE_SOURCED | CHAR_SPACE:
			*next_state = STATE_SOURCED;
			break;
		case STATE_SOURCED | CHAR_GENERAL:
			*next_state = STATE_SOURCED;
			ctx->src[ctx->slen++] = c;
			break;
		case STATE_SOURCED | CHAR_DOLLAR:
			*next_state = STATE_REFERENCE;
			ctx->sourcing = true;
			break;
		case STATE_SOURCED | CHAR_NEWLINE:
			*next_state = STATE_IDENTIFIER;
			ctx->sourcing = false;
			if (shp_scan_sourced(ctx->src) != SHP_DONE) {
				sprintf(scanner->error,
					"Error in external script: `%s'",
					ctx->src);
				return (SHP_ERROR);
			}
			ctx->slen = 0;
			break;
		case STATE_GENERAL | CHAR_GENERAL:
			*next_state = STATE_IDENTIFIER;
			--ctx->pos; --data;
			break;
		case STATE_GENERAL | CHAR_HASH:
		case STATE_IDENTIFIER | CHAR_HASH:
			*state = STATE_IDENTIFIER;
			ctx->line++;
			return (SHP_SCANNING);
		case STATE_IDENTIFIER | CHAR_NEWLINE:
			*next_state = STATE_IDENTIFIER;
			break;
		case STATE_IDENTIFIER | CHAR_GENERAL:
			*next_state = STATE_IDENTIFIER;
			ctx->key[ctx->klen++] = c;
			putchar(c);
			break;
		case STATE_IDENTIFIER | CHAR_EQUALS:
			*next_state = STATE_VALUE;
			ctx->key[ctx->klen++] = '\0';
			putchar(c);
			break;
		case STATE_REFERENCE | CHAR_OPAREN:
			*next_state = STATE_SHELLCMD;
			ctx->commanding = true;
			break;
		case STATE_REFERENCE | CHAR_OBRACE:
			*next_state = STATE_REFERENCE;
			break;
		case STATE_REFERENCE | CHAR_GENERAL:
			*next_state = STATE_REFERENCE;
			ctx->ref[ctx->rlen++] = c;
			break;
		case STATE_REFERENCE | CHAR_EBRACE:
			if (ctx->commanding)
				*next_state = STATE_SHELLCMD;
			else if (ctx->sourcing)
				*next_state = STATE_SOURCED;
			else
				*next_state = STATE_VALUE;
			ctx->ref[ctx->rlen++] = '\0';
			if ((tmp = getenv(ctx->ref)) == NULL) tmp = "";
			strncpy(ctx->ref, tmp, strlen(tmp));
			ctx->ref[strlen(tmp)] = '\0';
			ctx->rlen = strlen(ctx->ref);
			if (ctx->commanding) {
				strncpy(&ctx->cmd[ctx->clen], ctx->ref,
					ctx->rlen);
				ctx->clen += ctx->rlen;
			} else if (ctx->sourcing) {
				strncpy(&ctx->src[ctx->slen], ctx->ref,
					ctx->rlen);
				ctx->slen += ctx->rlen;
			} else {
				strncpy(&ctx->val[ctx->vlen], ctx->ref,
					ctx->rlen);
				ctx->vlen += ctx->rlen;
				printf("%s", ctx->ref);
			}
			ctx->rlen = 0;
			break;
		case STATE_SHELLCMD | CHAR_GENERAL:
		case STATE_SHELLCMD | CHAR_SPACE:
		case STATE_SHELLCMD | CHAR_TAB:
		case STATE_SHELLCMD | CHAR_DOT:
		case STATE_SHELLCMD | CHAR_SQUOTE:
		case STATE_SHELLCMD | CHAR_DQUOTE:
			*next_state = STATE_SHELLCMD;
			ctx->cmd[ctx->clen++] = c;
			break;
		case STATE_SHELLCMD | CHAR_DOLLAR:
			*next_state = STATE_REFERENCE;
			break;
		case STATE_SHELLCMD | CHAR_EPAREN:
			*next_state = STATE_VALUE;
			ctx->cmd[ctx->clen++] = '\0';
			if (shp_shcmd(scanner, ctx->cmd) == SHP_ERROR)
				return (SHP_ERROR);
			strncpy(&ctx->val[ctx->vlen], scanner->cmdbuf,
				strlen(scanner->cmdbuf));
			ctx->vlen += strlen(scanner->cmdbuf);
			printf("%s", scanner->cmdbuf);
			ctx->commanding = false;
			ctx->clen = 0;
			break;
		case STATE_VALUE | CHAR_SQUOTE:
		case STATE_VALUE | CHAR_DQUOTE:
			*next_state = STATE_VALUE;
			ctx->quoting = !ctx->quoting;
			putchar(c);
			break;
		case STATE_VALUE | CHAR_DOLLAR:
			*next_state = STATE_REFERENCE;
			break;
		case STATE_VALUE | CHAR_OPAREN:
		case STATE_VALUE | CHAR_EPAREN:
		case STATE_VALUE | CHAR_DOT:
		case STATE_VALUE | CHAR_GENERAL:
			*next_state = STATE_VALUE;
			ctx->val[ctx->vlen++] = c;
			putchar(c);
			break;
		case STATE_VALUE | CHAR_SPACE:
			*next_state = STATE_VALUE;
			ctx->val[ctx->vlen++] = c;
			putchar(c);
			break;
		case STATE_VALUE | CHAR_TAB:
			*next_state = STATE_VALUE;
			ctx->val[ctx->vlen++] = c;
			putchar(c);
			break;
		case STATE_VALUE | CHAR_EQUALS:
			*next_state = STATE_VALUE;
			ctx->val[ctx->vlen++] = c;
			putchar(c);
			break;
		case STATE_VALUE | CHAR_NEWLINE:
			if (ctx->quoting) {
				*next_state = STATE_VALUE;
				ctx->val[ctx->vlen++] = c;
			} else {
				*next_state = STATE_IDENTIFIER;
				ctx->val[ctx->vlen++] = '\0';
				setenv(ctx->key, ctx->val, 1);
				ctx->klen = 0; ctx->vlen = 0;
			}
			putchar(c);
			break;
		default:
			sprintf(scanner->error, "\nError: %s:%d:%d: Bad %s.",
				ctx->filename, ctx->line, ctx->pos + 1,
				shp_str_state(state));
			return (SHP_ERROR);
		}
		*state = *next_state;
	}
	ctx->line++;
	return (SHP_SCANNING);
}
