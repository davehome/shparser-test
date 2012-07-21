#ifndef SHP_CHARS_H
#define SHP_CHARS_H

typedef enum {
	CHAR_GENERAL,
	CHAR_HASH = '#',
	CHAR_SPACE = ' ',
	CHAR_TAB = '\t',
	CHAR_NEWLINE = '\n',
	CHAR_EQUALS = '=',
	CHAR_DOLLAR = '$',
	CHAR_OBRACE = '{',
	CHAR_EBRACE = '}',
	CHAR_OPAREN = '(',
	CHAR_EPAREN = ')',
	CHAR_SQUOTE = '\'',
	CHAR_DQUOTE = '"',
	CHAR_PIPE = '|',
	CHAR_AMP = '&',
	CHAR_DOT = '.'
} shp_chars;

#endif /* SHP_CHARS_H */
