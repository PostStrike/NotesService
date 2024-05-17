#include <gtk/gtk.h>
#include <string>
#include <iostream>

#pragma "once"

struct Coords {
    int x = 0;
    int y = 0;
};

static gboolean on_button_press_event(GtkWidget*, GdkEventButton*, gpointer);