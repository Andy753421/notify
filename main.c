#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "main.h"
#include "srv.h"
#include "gui.h"

void debug(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	fprintf(stderr, "debug: ");
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	va_end(ap);
}

void threadmain(int argc, char **argv)
{
	srv_init(argc, argv);
	gui_init(argc, argv);

	srv_run();
	gui_run();

	exit(0);
}
