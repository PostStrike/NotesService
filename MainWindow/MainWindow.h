#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtk/gtk.h>
#include <sigc++/sigc++.h>

using ID = int;

class MainWindow {
public:
    MainWindow(GtkWidget *, const int, const int);
    void show();
    void create_buttons();

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
    GtkWidget *button_open = NULL;
    GtkWidget *button_create = NULL;
    GtkWidget *button_exit = NULL;
    
    // Callback-функции для обработки нажатий на кнопки
    static void on_button_open_clicked(GtkWidget *widget, gpointer data);
    static void on_button_create_clicked(GtkWidget *widget, gpointer data);
    static void on_button_exit_clicked(GtkWidget *widget, gpointer data);
};

#endif // MAINWINDOW_H