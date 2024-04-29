#include "CreateWindow.h"


CreateWindow::CreateWindow(GtkWidget *window, const int width, const int height){
    this->width = width;
    this->height = height;
    this->window = window;
    gtk_window_set_title(GTK_WINDOW(this->window), "Create Window");
}

void CreateWindow::show() {
    gtk_widget_show_all(window);
}