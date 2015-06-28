#include <gtk/gtk.h>

#include "main.h"
#include "srv.h"
#include "gui.h"

/***********
 * Globals *
 ***********/

static GtkWidget *messages;

/*************
 * Callbacks *
 *************/

static gboolean on_keypress(GtkWidget *widget, GdkEventKey *event, gpointer _)
{
	switch (event->keyval) {
		case 'q':
			gtk_main_quit();
			return TRUE;
	}
	return FALSE;
}

static gboolean on_destroy(GtkWidget *widget, gpointer _)
{
	gtk_main_quit();
	return TRUE;
}

static gboolean on_message(gpointer _msg)
{
	const char *msg = _msg;

	/* Create message button */
	GtkWidget *button = gtk_button_new();
	GtkWidget *label  = gtk_label_new(msg);

	gtk_widget_set_halign(label, GTK_ALIGN_START);
	gtk_widget_set_valign(label, GTK_ALIGN_START);
	gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
	gtk_label_set_line_wrap_mode(GTK_LABEL(label), PANGO_WRAP_WORD);
	gtk_container_add(GTK_CONTAINER(button), label);

	/* Add it to the message list */
	gtk_box_pack_start(GTK_BOX(messages), button, FALSE, FALSE, 0);
	gtk_box_reorder_child(GTK_BOX(messages), button, 0);
	gtk_widget_show_all(button);

	return FALSE;
}

/*****************
 * GUI Functions *
 *****************/

void gui_show(const char *msg)
{
	g_idle_add(on_message, (gpointer)msg);
}

void gui_init(int argc, char **argv)
{
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
	gtk_main();
}
