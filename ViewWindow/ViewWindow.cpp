#include "ViewWindow.h"

extern std::vector<gulong> handlers;
Grid* p = nullptr;

void clear_window(ViewWindow* main_window, int id) {
    GtkWidget *window = main_window->get_window();

    //очищаем окно
    GList *children = gtk_container_get_children(GTK_CONTAINER(window));    
    for(GList *it = children; it != NULL; it = g_list_next(it)) {
        gtk_widget_destroy(GTK_WIDGET(it->data));
    }
    g_list_free(children);

    //удаляем обработчики
    for (auto handler : handlers) {
        g_signal_handler_disconnect(window, handler);
    }
    handlers.clear();

    main_window->send_response(id);
}

static gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer data) {
    KeyData* cur_data = static_cast<KeyData*>(data);
    Grid* grid = static_cast<Grid*>(cur_data->grid);

    switch (event->keyval) {
        case GDK_KEY_Escape:
            clear_window(cur_data->main_window, 0);
            delete cur_data;
            return TRUE;
        case GDK_KEY_BackSpace:
            if(p != nullptr) {
                delete p;
                p = nullptr;
            }
            cur_data->main_window->show();
            break;
        default:
            break;
    }

    return FALSE;  
}

ViewWindow::ViewWindow(GtkWidget *window, const int width, const int height){
    this->width = width;
    this->height = height;
    this->window = window;
    gtk_window_set_title(GTK_WINDOW(this->window), "View Window");

    KeyData* data = new KeyData{nullptr, this};

    //обработчик нажатия кнопок
    gulong key_press_handler = g_signal_connect(G_OBJECT(window), "key-press-event", G_CALLBACK(on_key_press), data);
    handlers.push_back(key_press_handler);
}

std::vector<std::string> get_saved_files(const std::string& directory) {
    std::vector<std::string> files;
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(directory.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string file_name = ent->d_name;
            if (file_name != "." && file_name != "..") {
                files.push_back(file_name);
            }
        }
        closedir(dir);
    } 

    return files;
}

GtkWidget* button_box = nullptr;

static void on_file_button_clicked(GtkWidget *button, gpointer data) {
    GridData* grid_data = static_cast<GridData*>(data);
    const char* filename = grid_data->filename;
    GtkWidget* window = grid_data->window;
    std::cout << "Button clicked for file: " << filename << std::endl;

    GtkWidget* box = gtk_overlay_new();
    gtk_container_remove(GTK_CONTAINER(window), button_box);
    gtk_container_add(GTK_CONTAINER(window), box);

    Grid* grid = new Grid(window, box, 800, 600, 20);
    p = grid;
    std::string filepath = filename;
    filepath = "../notes/" + filepath;
    std::cout << filepath << std::endl;
    std::ifstream file(filepath);

    int size; file >> size;
    while(size--) {
        int x, y; char key_code; file >> x >> y >> key_code;
        grid->draw_const(key_code, x, y);
    }
    gtk_widget_show_all(window);
}

void ViewWindow::show() {
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "../styles/buttons.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(provider);

    button_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), button_box);

    std::vector<std::string> files = get_saved_files("../notes");
    for (const std::string& file : files) {
        GtkWidget *button = gtk_button_new_with_label(file.c_str());
        gtk_widget_set_name(button, "button-custom");
        GridData* grid_data = new GridData{g_strdup(file.c_str()), window}; 
        g_signal_connect(button, "clicked", G_CALLBACK(on_file_button_clicked), grid_data);
        gtk_box_pack_start(GTK_BOX(button_box), button, FALSE, FALSE, 0);
    }

    gtk_widget_show_all(window);
}