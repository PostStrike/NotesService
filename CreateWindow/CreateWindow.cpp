#include "CreateWindow.h"

#include "Grid.h"


CreateWindow::CreateWindow(GtkWidget *window, const int width, const int height){
    this->width = width;
    this->height = height;
    this->window = window;
    gtk_window_set_title(GTK_WINDOW(this->window), "Create Window");
}

void CreateWindow::show() {
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), drawing_area);

    //отрисовка сетки
    Grid* grid = new Grid(width, height, 20);
    g_signal_connect(drawing_area, "draw", G_CALLBACK(on_draw), grid);

    gtk_widget_show_all(window);
}