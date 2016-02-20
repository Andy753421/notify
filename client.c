#define _DEFAULT_SOURCE 1

#include <u.h>
#include <libc.h>
#include <bio.h>
#include <thread.h>
#include <plumb.h>

#include "main.h"
#include "client.h"

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

static Biobuf bin;
static Biobuf bout;

static int notifyfd;

/**********
 * Server *
 **********/

void client_init(void)
{
	debug("client: init");
	Binit(&bin,  0, OREAD);
	Binit(&bout, 1, OWRITE);
	notifyfd = plumbopen("notify", OWRITE);
}

void client_send(char *mesg)
{
	Plumbmsg msg;

	msg.dst   = "notify";
	msg.src   = "notify";
	msg.type  = "text";
	msg.data  = mesg ? strdup(mesg) :
		    Brdstr(&bin, '\n', 1);
	msg.ndata = strlen(msg.data);

	plumbsend(notifyfd, &msg);

	free(msg.data);
}
