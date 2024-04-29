#ifndef CREATEWINDOW_H
#define CREATEWINDOW_H

#include <gtk/gtk.h>
#include <sigc++/sigc++.h>

using ID = int;

class CreateWindow {
public:
    CreateWindow(GtkWidget *, const int, const int);
    void show();
    void send_response(ID);

    sigc::signal<void, int> response_changed;

    GtkWidget* get_window() {
        return window;
    } 

    void set_response(ID new_id) {
        response_changed.emit(new_id);
    }
private:
    int width;
    int height;

    GtkWidget *window = NULL;
};

#endif // CREATEWINDOW_H