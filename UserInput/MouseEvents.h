#include <gtk/gtk.h>
#include <string>
#include <iostream>

#pragma "once"

struct Coords {
    int x = 0;
    int y = 0;
};

void window_clicked(GdkEventButton*, gpointer);