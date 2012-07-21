#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shp.h"

shp_status
shp_scan(shp_scanner *scanner)
{
	shp_context *ctx;
	shp_state *state;
	shp_state *next_state;
	shp_chars category;
	size_t selector;
	const char *tmp;
	const char *data;
	char c;

	ctx = scanner->ctx;
	data = scanner->scan_func(ctx);
	if (data == 0) return (SHP_DONE);

	state = &scanner->state;
	next_state = &scanner->next_state;

	for (ctx->pos = 0; (c = *data++) != 0; ctx->pos++) {
		category = shp_translate(c);
		selector = 1000 * *state + category;
		switch (selector) {
		case 1000 * STATE_GENERAL + CHAR_GENERAL:
			*next_state = STATE_IDENTIFIER;
			--ctx->pos; --data;
			break;
		case 1000 * STATE_GENERAL + CHAR_HASH:
		case 1000 * STATE_IDENTIFIER + CHAR_HASH:
			*state = STATE_IDENTIFIER;
			ctx->line++;
			return (SHP_SCANNING);
		case 1000 * STATE_IDENTIFIER + CHAR_NEWLINE:
			*next_state = STATE_IDENTIFIER;
			break;
		case 1000 * STATE_IDENTIFIER + CHAR_GENERAL:
			*next_state = STATE_IDENTIFIER;
			ctx->key[ctx->klen++] = c;
			putchar(c);
			break;
		case 1000 * STATE_IDENTIFIER + CHAR_EQUALS:
			*next_state = STATE_VALUE;
			ctx->key[ctx->klen++] = '\0';
			putchar(c);
			break;
		case 1000 * STATE_REFERENCE + CHAR_OPAREN:
			*next_state = STATE_SHELLCMD;
			ctx->commanding = true;
			break;
		case 1000 * STATE_REFERENCE + CHAR_OBRACE:
			*next_state = STATE_REFERENCE;
			break;
		case 1000 * STATE_REFERENCE + CHAR_GENERAL:
			*next_state = STATE_REFERENCE;
			ctx->ref[ctx->rlen++] = c;
			break;
		case 1000 * STATE_REFERENCE + CHAR_EBRACE:
			*next_state = (ctx->commanding)
				? STATE_SHELLCMD
				: STATE_VALUE;
			ctx->ref[ctx->rlen++] = '\0';
			if ((tmp = getenv(ctx->ref)) == NULL) tmp = "";
			strncpy(ctx->ref, tmp, strlen(tmp));
			ctx->ref[strlen(tmp)] = '\0';
			ctx->rlen = strlen(ctx->ref);
			if (ctx->commanding) {
				strncpy(&ctx->cmd[ctx->clen], ctx->ref,
					ctx->rlen);
				ctx->clen += ctx->rlen;
			} else {
				strncpy(&ctx->val[ctx->vlen], ctx->ref,
					ctx->rlen);
				ctx->vlen += ctx->rlen;
				printf("%s", ctx->ref);
			}
			ctx->rlen = 0;
			break;
		case 1000 * STATE_SHELLCMD + CHAR_GENERAL:
		case 1000 * STATE_SHELLCMD + CHAR_SPACE:
		case 1000 * STATE_SHELLCMD + CHAR_TAB:
		case 1000 * STATE_SHELLCMD + CHAR_SQUOTE:
		case 1000 * STATE_SHELLCMD + CHAR_DQUOTE:
			*next_state = STATE_SHELLCMD;
			ctx->cmd[ctx->clen++] = c;
			break;
		case 1000 * STATE_SHELLCMD + CHAR_DOLLAR:
			*next_state = STATE_REFERENCE;
			break;
		case 1000 * STATE_SHELLCMD + CHAR_EPAREN:
			*next_state = STATE_VALUE;
			ctx->cmd[ctx->clen++] = '\0';
			shp_shcmd(scanner, ctx->cmd);
			strncpy(&ctx->val[ctx->vlen], scanner->cmdbuf,
				strlen(scanner->cmdbuf));
			ctx->vlen += strlen(scanner->cmdbuf);
			printf("%s", scanner->cmdbuf);
			ctx->commanding = false;
			ctx->clen = 0;
			break;
		case 1000 * STATE_VALUE + CHAR_SQUOTE:
		case 1000 * STATE_VALUE + CHAR_DQUOTE:
			*next_state = STATE_VALUE;
			ctx->quoting = !ctx->quoting;
			putchar(c);
			break;
		case 1000 * STATE_VALUE + CHAR_DOLLAR:
			*next_state = STATE_REFERENCE;
			break;
		case 1000 * STATE_VALUE + CHAR_OPAREN:
		case 1000 * STATE_VALUE + CHAR_EPAREN:
		case 1000 * STATE_VALUE + CHAR_GENERAL:
			*next_state = STATE_VALUE;
			ctx->val[ctx->vlen++] = c;
			putchar(c);
			break;
		case 1000 * STATE_VALUE + CHAR_SPACE:
			*next_state = STATE_VALUE;
			ctx->val[ctx->vlen++] = c;
			putchar(c);
			break;
		case 1000 * STATE_VALUE + CHAR_TAB:
			*next_state = STATE_VALUE;
			ctx->val[ctx->vlen++] = c;
			putchar(c);
			break;
		case 1000 * STATE_VALUE + CHAR_EQUALS:
			*next_state = STATE_VALUE;
			ctx->val[ctx->vlen++] = c;
			putchar(c);
			break;
		case 1000 * STATE_VALUE + CHAR_NEWLINE:
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
			sprintf(scanner->error,
				"\nError at %s:%d:%d: Bad %s %c",
				ctx->filename, ctx->line, ctx->pos + 1,
				shp_str_state(state), c);
			return (SHP_ERROR);
		}
		*state = *next_state;
	}
	ctx->line++;
	return (SHP_SCANNING);
}
