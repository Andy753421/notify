#define _DEFAULT_SOURCE 1

#include <u.h>
#include <libc.h>
#include <thread.h>
#include <plumb.h>

#include "main.h"
#include "srv.h"
#include "gui.h"

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

static void srv_main(void *arg)
{
	Plumbmsg  *msg;
	Plumbattr *attr;
	static char buf[5120];
	const char *head, *urgency, *expire, *icon, *category, *hint;

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
		sprint(buf, "%s:\n%.*s", head, msg->ndata, msg->data);
		gui_show(buf);
	}
}

/**********
 * Server *
 **********/

void srv_init(int argc, char **argv)
{
	//debug("srv_init");
	notifyfd = plumbopen("notify", OREAD);
}

void srv_run(void)
{
	//debug("srv_run");
	proccreate(srv_main, NULL, 0);
}
