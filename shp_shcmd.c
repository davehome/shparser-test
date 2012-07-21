#include <stdio.h>
#include <string.h>
#include "shp.h"

void
shp_shcmd(shp_scanner *scanner, const char *cmd)
{
	FILE *p;
	char *tmp;

	p = popen(cmd, "r");
	tmp = fgets(scanner->cmdbuf, BUFSIZ, p);
	scanner->cmdbuf[strcspn(tmp, "\n")] = '\0';
	pclose(p);
}
