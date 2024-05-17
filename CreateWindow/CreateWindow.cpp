#include "CreateWindow.h"

CreateWindow::CreateWindow(GtkWidget *window, const int width, const int height){
    this->width = width;
    this->height = height;
    this->window = window;
    gtk_window_set_title(GTK_WINDOW(this->window), "Create Window");
}

void draw_loop(GtkWidget* window, Grid *grid) {
    while(true) {
        GtkWidget *drawing_area = gtk_drawing_area_new();
        gtk_container_add(GTK_CONTAINER(window), drawing_area);

        g_signal_connect(drawing_area, "draw", G_CALLBACK(draw_callback), grid);
        grid->t++;

        gtk_widget_show_all(window);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        gtk_container_remove(GTK_CONTAINER(window), drawing_area);
        if(GTK_IS_WIDGET(drawing_area)) {
            gtk_widget_destroy(drawing_area);
        }
    }
}

void CreateWindow::show() {
    //отрисовка сетки
    grid = new Grid(width, height, 30);
    std::thread* draw_thread = new std::thread(draw_loop, window, grid);
    draw_thread->detach();
}