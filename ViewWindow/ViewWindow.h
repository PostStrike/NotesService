#ifndef VIEWWINDOW_H
#define VIEWWINDOW_H

#include <gtk/gtk.h>
#include <sigc++/sigc++.h>
#include <dirent.h>

#include "Grid.h"

using ID = int;

class ViewWindow {
public:
    ViewWindow(GtkWidget *, const int, const int);
    void show();
    void show_grid();

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
};

#ifndef KEYDATA_H
#define KEYDATA_H

struct KeyData {
    Grid* grid;
    ViewWindow* main_window;
};

#endif // KEYDATA_H

struct GridData {
    const char* filename;
    GtkWidget* window;
};

#endif // VIEWWINDOW_H