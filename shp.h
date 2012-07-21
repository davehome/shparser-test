#ifndef SHP_H
#define SHP_H

#include "shp_context.h"
#include "shp_chars.h"

#define stringify( name ) # name

typedef struct _shp_scanner_t shp_scanner;

typedef char *(*shp_scan_func_t)(void *);

typedef enum {
	SHP_SCANNING,
	SHP_DONE,
	SHP_ERROR
} shp_status;

typedef enum {
	STATE_GENERAL,
	STATE_COMMENT,
	STATE_IDENTIFIER,
	STATE_VALUE,
	STATE_SQUOTE,
	STATE_DQUOTE,
	STATE_REFERENCE,
	STATE_SHELLCMD
} shp_state;

struct _shp_scanner_t {
	shp_scan_func_t scan_func;
	shp_context *ctx;
	shp_state state;
	shp_state next_state;
	char error[BUFSIZ];
	char cmdbuf[BUFSIZ];
};

void shp_init(shp_scanner *, shp_scan_func_t, shp_context *);
shp_status shp_scan(shp_scanner *);
shp_chars shp_translate(char) __attribute__ ((const));
const char *shp_str_state(shp_state *) __attribute__ ((pure));
void shp_shcmd(shp_scanner *, const char *);

#endif /* SHP_H */
