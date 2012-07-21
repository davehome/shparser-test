#include "shp.h"
	
const char *
shp_str_state(shp_state *s)
{
	switch (*s) {
		case STATE_GENERAL: return stringify(character or something);
		case STATE_COMMENT: return stringify(comment... rofl:);
		case STATE_IDENTIFIER: return stringify(identifier definition);
		case STATE_VALUE: return stringify(value... could not parse);
		case STATE_REFERENCE: return stringify(reference);
		case STATE_SHELLCMD: return stringify(shell command);
		default: return stringify(syntax in general);
	}
}
