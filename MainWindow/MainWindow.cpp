#include "MainWindow.h"


void MainWindow::create_buttons() { 
    const int padding = 50;

    button_create = gtk_button_new_with_label("Создать заметку");
    button_open = gtk_button_new_with_label("Открыть заметки");
    button_exit = gtk_button_new_with_label("Выйти");
    gtk_widget_set_size_request(button_create, 100, padding);
    gtk_widget_set_size_request(button_open, 100, padding);
    gtk_widget_set_size_request(button_exit, 100, padding);

    GtkWidget *fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed);


    const int box_width = 200;
    const int box_height = 100;

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_fixed_put(GTK_FIXED(fixed), box, (width - box_width) / 2,
                                         (height - box_height - padding * 2) / 2);
    gtk_widget_set_size_request(box, box_width, box_height);
    
    gtk_box_pack_start(GTK_BOX(box), button_open, TRUE, TRUE, 2);
    gtk_box_pack_start(GTK_BOX(box), button_create, TRUE, TRUE, 2);
    gtk_box_pack_start(GTK_BOX(box), button_exit, TRUE, TRUE, 2);

    g_signal_connect(button_open, "clicked", G_CALLBACK(on_button_open_clicked), this);
    g_signal_connect(button_create, "clicked", G_CALLBACK(on_button_create_clicked), this);
    g_signal_connect(button_exit, "clicked", G_CALLBACK(on_button_exit_clicked), this);
}

MainWindow::MainWindow(GtkWidget *window, const int width, const int height) {
    this->width = width;
    this->height = height;
    this->window = window;
    gtk_window_set_title(GTK_WINDOW(this->window), "Main Window");
    
    create_buttons();
}

void MainWindow::show() {
    gtk_widget_show_all(window);
}

void clear_window(gpointer data, ID id) {
    MainWindow *main_window = static_cast<MainWindow *>(data);
    GtkWidget *window = main_window->get_window();

    GList *children = gtk_container_get_children(GTK_CONTAINER(window));    
    for(GList *it = children; it != NULL; it = g_list_next(it)) {
        gtk_widget_destroy(GTK_WIDGET(it->data));
    }
    g_list_free(children);

    main_window->send_response(id);
}

// Обработчики событий для кнопок
void MainWindow::on_button_open_clicked(GtkWidget *widget, gpointer data) {
    g_print("Кнопка \"Открыть заметки\" нажата!\n"); //log

    clear_window(data, 2);
}

void MainWindow::on_button_create_clicked(GtkWidget *widget, gpointer data) {
    g_print("Кнопка \"Создать заметку\" нажата!\n"); //log

    clear_window(data, 1);
}

void MainWindow::on_button_exit_clicked(GtkWidget *widget, gpointer data) {
    g_print("Кнопка \"Выйти\" нажата!\n"); //log

    clear_window(data, 3);

    gtk_main_quit();
}