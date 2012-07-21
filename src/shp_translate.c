#include "shp.h"

shp_chars
shp_translate (char c)
{
	switch (c) {
	case '#' : return CHAR_HASH;
	case ' ' : return CHAR_SPACE;
	case '\t': return CHAR_TAB;
	case '\n': return CHAR_NEWLINE;
	case '=' : return CHAR_EQUALS;
	case '$' : return CHAR_DOLLAR;
	case '{' : return CHAR_OBRACE;
	case '}' : return CHAR_EBRACE;
	case '(' : return CHAR_OPAREN;
	case ')' : return CHAR_EPAREN;
	case '\'': return CHAR_SQUOTE;
	case '"' : return CHAR_DQUOTE;
	/*case '|' : return CHAR_PIPE;
	case '&' : return CHAR_AMP;*/
	default  : return CHAR_GENERAL;
	}
}
