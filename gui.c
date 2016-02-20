#include <gtk/gtk.h>

#include "main.h"
#include "gui.h"

/***********
 * Globals *
 ***********/

static GtkWidget *messages;

/***********
 * Helpers *
 ***********/

struct {
	const gchar *ptrn;
	const gchar *repl;
	GRegex *re;
} regexes[] = {
	{"&lt;",  "<",  NULL},
	{"&gt;",  ">",  NULL},
	{"&amp;", "&",  NULL},
	{NULL,    NULL, NULL}, // markup_escape here
	{"https?://\\S+", "<a href=\"\\0\">\\0</a>", NULL},
};

gchar *escape(const gchar *_str)
{
	if (!_str || !_str[0])
		return NULL;
	char *str = g_strdup(_str);
	for (int i = 0; i < G_N_ELEMENTS(regexes); i++) {
		gchar *tmp;
		if (regexes[i].ptrn && !regexes[i].re)
			regexes[i].re = g_regex_new(regexes[i].ptrn, 0, 0, NULL);
		if (regexes[i].ptrn)
			tmp = g_regex_replace(regexes[i].re, str, -1, 0, regexes[i].repl, 0, NULL);
		else
			tmp = g_markup_escape_text(str, -1);
		g_free(str);
		str = tmp;
	}
	return str;
}

static void clear(void)
{
	GList *old = gtk_container_get_children(GTK_CONTAINER(messages));
	for (GList *cur = old; cur; cur = cur->next)
		gtk_container_remove(GTK_CONTAINER(messages), cur->data);
	g_list_free(old);
}

/*************
 * Callbacks *
 *************/

static gboolean on_keypress(GtkWidget *widget, GdkEventKey *event, gpointer _)
{
	switch (event->keyval) {
		case 'q':
			gtk_main_quit();
			return TRUE;
		case 'l':
			if (event->state & GDK_CONTROL_MASK)
				clear();
			return TRUE;
	}
	return FALSE;
}

static gboolean on_destroy(GtkWidget *widget, gpointer _)
{
	gtk_main_quit();
	return TRUE;
}

static gboolean on_message(gpointer _markup)
{
	char *markup = _markup;

	/* Create message button */
	GtkWidget *button = gtk_button_new();
	GtkWidget *label  = gtk_label_new(markup);

	gtk_widget_set_halign(label, GTK_ALIGN_START);
	gtk_widget_set_valign(label, GTK_ALIGN_START);
	//gtk_label_set_xalign(label, GTK_ALIGN_START);
	//gtk_label_set_yalign(label, GTK_ALIGN_START);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
	gtk_label_set_line_wrap_mode(GTK_LABEL(label), PANGO_WRAP_WORD);
	gtk_container_add(GTK_CONTAINER(button), label);

	/* Add it to the message list */
	gtk_box_pack_start(GTK_BOX(messages), button, FALSE, FALSE, 0);
	gtk_box_reorder_child(GTK_BOX(messages), button, 0);
	gtk_widget_show_all(button);

	g_free(markup);

	debug("gui: on_message");

	return FALSE;
}

/*****************
 * GUI Functions *
 *****************/

void gui_show(notify_t *msg)
{
	debug("gui: show");

	/* Create message markup */
	time_t age = 0;
	gchar *head_str = escape(msg->head);
	gchar *text_str = escape(msg->text);
	gchar *time_str = g_strdup_printf("-%ld:%02ld:%02ld", age/(60*60), (age/60)%60, age%60);
	gchar *markup  = g_strdup_printf(
			"<span font_weight=\"bold\" font=\"Mono 6\">(%s)</span> "
			"<span font_weight=\"bold\" font=\"Sans 6\">%s</span>: "
			"%s", time_str, head_str, text_str);
	g_free(time_str);
	g_free(head_str);
	g_free(text_str);

	/* Show it in the main thread */
	g_idle_add(on_message, markup);
}

void gui_init(int argc, char **argv)
{
	debug("gui: init");

	gtk_init(&argc, &argv);

	GtkWidget *window   = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget *scroll   = gtk_scrolled_window_new(NULL, NULL);
	GtkWidget *viewport = gtk_viewport_new(NULL, NULL);

	messages = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);

	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
			GTK_POLICY_AUTOMATIC,  GTK_POLICY_AUTOMATIC);

	g_signal_connect(window, "key-press-event", G_CALLBACK(on_keypress), NULL);
	g_signal_connect(window, "destroy",         G_CALLBACK(on_destroy),  NULL);

	gtk_container_add(GTK_CONTAINER(viewport), messages);
	gtk_container_add(GTK_CONTAINER(scroll),   viewport);
	gtk_container_add(GTK_CONTAINER(window),   scroll);
	gtk_widget_show_all(window);
}

void gui_run(void)
{
	debug("gui: run");

	gtk_main();
}
