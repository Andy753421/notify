#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <getopt.h>

#include "main.h"
#include "client.h"
#include "server.h"
#include "text.h"
#include "gui.h"

/* Config vars */
static int   opt_send  = 0;
static char *opt_mesg  = NULL;
static int   opt_recv  = 0;
static int   opt_text  = 0;
static int   opt_gui   = 0;
static int   opt_debug = 0;

/* Local Data */
static struct option long_options[] = {
	{ "send",  2, NULL, 's' },
	{ "recv",  0, NULL, 'r' },
	{ "text",  0, NULL, 't' },
	{ "gui",   0, NULL, 'g' },
	{ "debug", 0, NULL, 'd' },
	{ "help",  0, NULL, 'h' },
	{ NULL,    0, NULL,  0  },
};

/* Helper functions */
static void usage(void)
{
	printf("Usage:\n");
	printf("  notify [OPTIONS...]\n");
	printf("\n");
	printf("Options:\n");
	printf("  -s, --send=[MSG]  Sent notification to plumber\n");
	printf("  -r, --recv        Receive notifications from plumber\n");
	printf("  -t, --text        Log text notifications to stdout\n");
	printf("  -g, --gui         Start GTK notifications window\n");
	printf("  -d, --debug       Print debug message to stdout\n");
	printf("  -h, --help        Print usage information\n");
}

static void parse(int argc, char **argv)
{
	while (1) {
		int c = getopt_long(argc, argv, "srtgdh", long_options, NULL);
		if (c == -1)
			break;
		switch (c) {
			case 's':
				opt_send  = 1;
				opt_mesg  = optarg;
				break;
			case 'r':
				opt_recv  = 1;
				break;
			case 't':
				opt_text  = 1;
				break;
			case 'g':
				opt_gui   = 1;
				break;
			case 'd':
				opt_debug = 1;
				break;
			case 'h':
				usage(); exit(0);
			default:
				usage(); exit(1);
		}
	}
}

/* Main functions */
void show(notify_t *msg)
{
	if (opt_gui)
		gui_show(msg);
	if (opt_text)
		text_show(msg);
}

void debug(const char *fmt, ...)
{
	va_list ap;
	if (opt_debug) {
		va_start(ap, fmt);
		fprintf(stderr, "# ");
		vfprintf(stderr, fmt, ap);
		fprintf(stderr, "\n");
		va_end(ap);
	}
}

void error(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	fprintf(stderr, "Error: ");
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	va_end(ap);
	exit(-1);
}

void threadmain(int argc, char **argv)
{
	parse(argc, argv);

	if (opt_send) {
		client_init();
		client_send(opt_mesg);
	}
	if (opt_recv) {
		server_init();
		server_run();
	}

	if (opt_text) {
		text_init();
	}
	if (opt_gui) {
		gui_init(argc, argv);
		gui_run();
	} else if (opt_text) {
		while (1)
			sleep(1);
	}

	exit(0);
}
