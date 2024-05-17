#include "MouseEvents.h"


gboolean on_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data) {
    std::string state = *static_cast<std::string*>(data); std::cout << state.c_str();
    if (state.c_str() == "CreateWindow" && event->type == GDK_BUTTON_PRESS) {
        g_print("Mouse click at coordinates: (%.2d, %.2d)\n", static_cast<int>(event->x), static_cast<int>(event->y));
    }

    return FALSE; 
}