#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shp.h"

shp_status
shp_shcmd(shp_scanner *scanner, const char *cmd)
{
	shp_context *ctx = scanner->ctx;
	FILE *stream;
	size_t read;
	int rv, err;

	err = 0;
	fflush(stderr);
	stream = popen(cmd, "r");
	read = fread(scanner->cmdbuf, sizeof(char), BUFSIZ, stream);
	rv = pclose(stream);

	if (err != 0 || rv != 0 || read == 0) {
		err = errno;
		sprintf(scanner->error, "\nError: %s:%d:%d: %s `%s' %s.",
			ctx->filename, ctx->line, ctx->pos - strlen(cmd) + 1,
			"Executing shell command", cmd, "failed");
		if (err != 0) fprintf(stderr, "%s\n", strerror(err));
		return (SHP_ERROR);
	}
	if (scanner->cmdbuf[read-1] == '\n')
		scanner->cmdbuf[read-1] = '\0';
	/*scanner->cmdbuf[strcspn(scanner->cmdbuf, "\n")] = '\0';*/
	return (SHP_SCANNING);
}
