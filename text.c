#include <stdio.h>

#include "main.h"
#include "text.h"

/*****************
 * Text Functions *
 *****************/

void text_show(notify_t *msg)
{
	debug("text: show");
	printf("%s: %s\n", msg->head, msg->text);
	fflush(stdout);
}

void text_init(void)
{
	debug("text: init");
}
