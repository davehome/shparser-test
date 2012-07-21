#ifndef SHP_H
#define SHP_H

#include "shp_context.h"
#include "shp_chars.h"

#define stringify( name ) # name

typedef struct _shp_scanner_t shp_scanner;
typedef struct _shp_cb_t shp_cb;

typedef char *(*shp_scan_func_t)(void *);

typedef enum {
	SHP_SCANNING,
	SHP_DONE,
	SHP_ERROR
} shp_status;

typedef enum {
	STATE_GENERAL		= 0x100,
	STATE_COMMENT		= 0x200,
	STATE_IDENTIFIER	= 0x300,
	STATE_VALUE		= 0x400,
	STATE_REFERENCE		= 0x500,
	STATE_SHELLCMD		= 0x600
} shp_state;

struct _shp_cb_t {
	shp_scan_func_t scan;

};

struct _shp_scanner_t {
	shp_context *ctx;
	shp_cb *cb;
	shp_state state;
	shp_state next_state;
	char error[BUFSIZ];
	char cmdbuf[BUFSIZ];
};

int shp_getopt(shp_context *, int, char **);
int shp_show_usage(const char *);
void shp_init(shp_scanner *, shp_cb *, shp_context *);
char *shp_file_input(void *);
shp_status shp_scan(shp_scanner *);
shp_chars shp_translate(char) __attribute__ ((const));
const char *shp_str_state(shp_state *) __attribute__ ((pure));
shp_status shp_shcmd(shp_scanner *, const char *);

#endif /* SHP_H */
