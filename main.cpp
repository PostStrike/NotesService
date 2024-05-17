#include <iostream>

#include "MainWindow/MainWindow.h"
#include "CreateWindow/CreateWindow.h"
#include "MouseEvents.h"

const int width = 800;
const int height = 600; 

const int MainWindowId = 0;
const int CreateWindowId = 1;

struct State {
    int id = 0;
    std::string name = "None";
};

State state;

GtkWidget *window;

template<typename T>
void show_window();
void loop(ID);
void get_response(ID);


int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), width, height);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    g_signal_connect(window, "button-press-event", G_CALLBACK(on_button_press_event), &state.name);
    gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK);

    // Фон окна
    GtkCssProvider *provider = gtk_css_provider_new();
    GError *error = nullptr;
    gtk_css_provider_load_from_path(provider, "../styles/window.css", &error);
    if (error != nullptr) {
        g_printerr("Ошибка загрузки CSS: %s\n", error->message);
        g_error_free(error);
        return 0;
    }

    GtkStyleContext *context = gtk_widget_get_style_context(window);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_object_unref(provider);

    // Мой цикл
    loop(0);

    // Запуск главного цикла GTK
    gtk_main();


    return 0;
}

template<typename T>
void show_window() {
    static T w(window, width, height);
    w.show();
    w.response_changed.connect(sigc::ptr_fun(&get_response));
}

void loop(ID process) {
    switch (process) {
    
    case MainWindowId:
        state.id = MainWindowId;
        state.name = "MainWindow"; 
        show_window<MainWindow>();
        return;

    case CreateWindowId:
        state.id = CreateWindowId;
        state.name = "CreateWindow"; 
        show_window<CreateWindow>();        
        return;

    default:
        return;
    
    }
}

void get_response(ID response) {
    std::cout << response << std::endl;
    loop(response);
}