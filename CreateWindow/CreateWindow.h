#ifndef CREATEWINDOW_H
#define CREATEWINDOW_H

#include <gtk/gtk.h>
#include <sigc++/sigc++.h>

#include "Grid.h"

using ID = int;

class CreateWindow {
public:
    CreateWindow(GtkWidget *, const int, const int);
    void show();

    sigc::signal<void, int> response_changed;

    GtkWidget* get_window() {
        return window;
    } 

    void send_response(ID id) {
        response_changed.emit(id);
    }
private:
    int width;
    int height;

    GtkWidget *window = NULL;
    Grid* grid = nullptr;
};

#endif // CREATEWINDOW_H