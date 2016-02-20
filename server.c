#define _DEFAULT_SOURCE 1

#include <u.h>
#include <libc.h>
#include <thread.h>
#include <plumb.h>

#include "main.h"
#include "server.h"

/*
 * typedef struct {
 * 	char      *name;
 * 	char      *value;
 * 	Plumbattr *next;
 * } Plumbattr;
 *
 * typedef struct {
 * 	char      *src;
 * 	char      *dst;
 * 	char      *wdir;
 * 	char      *type;
 * 	Plumbattr *attr;
 * 	int       ndata;
 * 	char      *data;
 * } Plumbmsg;
 */

static int notifyfd;

/*************
 * Main loop *
 *************/

static void server_main(void *arg)
{
	debug("server: main");

	notify_t notify;
	Plumbmsg  *msg;
	Plumbattr *attr;
	const char *head, *urgency, *expire, *icon, *category, *hint;

	while (1) {
		while ((msg = plumbrecv(notifyfd))) {
			head = urgency = expire = icon = category = hint = "";
			for (attr = msg->attr; attr; attr = attr->next) {
				if      (!strcmp(attr->name, "head"))     head     = attr->value;
				//else if (!strcmp(attr->name, "urgency"))  urgency  = attr->value;
				//else if (!strcmp(attr->name, "expire"))   expire   = attr->value;
				//else if (!strcmp(attr->name, "icon"))     icon     = attr->value;
				//else if (!strcmp(attr->name, "category")) category = attr->value;
				//else if (!strcmp(attr->name, "hint"))     hint     = attr->value;
				else debug("unknown attr: %s -> %s", attr->name, attr->value);
			}
			char *text = strndup(msg->data, msg->ndata);
			notify.head = head;
			notify.text = text;
			show(&notify);
			free(text);
		}
		debug("server: disconnected");
		plumbunmount();
		do {
			sleep(1000);
			debug("server: retrying");
			notifyfd = plumbopen("notify", OREAD);
		} while (notifyfd < 0);
	}
}

/**********
 * Server *
 **********/

void server_init(void)
{
	debug("server: init");
	notifyfd = plumbopen("notify", OREAD);
	if (notifyfd < 0)
		error("unable to connect to plumber");
}

void server_run(void)
{
	debug("server: run");
	proccreate(server_main, NULL, 0);
}
