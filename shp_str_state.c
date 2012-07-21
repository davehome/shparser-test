#include "shp.h"
	
const char *
shp_str_state(shp_state *s)
{
	switch (*s) {
		case STATE_GENERAL: return stringify(character);
		case STATE_COMMENT: return stringify(comment);
		case STATE_IDENTIFIER: return stringify(identifier);
		case STATE_VALUE: return stringify(value);
		case STATE_SQUOTE: return stringify(use of single quote);
		case STATE_DQUOTE: return stringify(use of double quote);
		case STATE_REFERENCE: return stringify(reference);
		case STATE_SHELLCMD: return stringify(shell Command);
		default: return stringify(syntax);
	}
}
