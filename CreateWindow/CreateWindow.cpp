#include "CreateWindow.h"

bool stop = false;
std::thread *draw_thread;
std::vector<gulong> handlers;

CreateWindow::CreateWindow(GtkWidget *window, const int width, const int height){
    this->width = width;
    this->height = height;
    this->window = window;
    gtk_window_set_title(GTK_WINDOW(this->window), "Create Window");
}

void draw_loop(Grid *grid) {
    while(true) {
        if(stop) {
            return;
        }  
        if (grid->t % 6 == 0) {
            grid->draw_cursor();
        } else if (grid->t % 6 == 3) {
            grid->delete_cursor();
        }
        grid->t++;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

static gboolean on_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data) {
    if(stop) return TRUE;
    Grid* grid = static_cast<Grid*>(data);

    int x = static_cast<int>(event->x);
    int y = static_cast<int>(event->y);

    std::pair<int, int> new_coords = grid->nearest_cell(x, y);

    grid->cursor.x = new_coords.first;
    grid->cursor.y = new_coords.second;
    grid->t = 3;

    return FALSE; 
}

static gboolean on_enter_notify(GtkWidget *widget, GdkEventCrossing *event, gpointer data) {
    GdkWindow *window = gtk_widget_get_window(widget);
    GdkCursor *cursor = gdk_cursor_new_from_name(gdk_display_get_default(), "text");
    gdk_window_set_cursor(window, cursor);
    g_object_unref(cursor);
    return TRUE;
}

static void on_realize(GtkWidget *widget, gpointer data) {
    gtk_widget_set_events(widget, GDK_ENTER_NOTIFY_MASK);
}

void clear_window(CreateWindow* main_window, ID id) {
    GtkWidget *window = main_window->get_window();
    stop = true;
    if (draw_thread && draw_thread->joinable()) {
        draw_thread->join();
    }
    delete draw_thread;
    draw_thread = nullptr;

    //очищаем содержимое окна
    GList *children = gtk_container_get_children(GTK_CONTAINER(window));    
    for(GList *it = children; it != NULL; it = g_list_next(it)) {
        gtk_widget_destroy(GTK_WIDGET(it->data));
    }
    g_list_free(children);

    //отключаем обработчики
    for (auto handler : handlers) {
        g_signal_handler_disconnect(window, handler);
    }
    handlers.clear();

    main_window->send_response(id);
}

static gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer data) {
    int key_code = event->keyval;

    KeyData* cur_data = static_cast<KeyData*>(data);
    Grid* grid = static_cast<Grid*>(cur_data->grid);

    bool flag = false;
    if(key_code >= 'a' && key_code <= 'z') flag = true; 
    if(key_code >= 'A' && key_code <= 'Z') flag = true;
    std::string special_alph = ",.{}[]''<>+=-:;";
    if(special_alph.find(key_code) != std::string::npos) {
        flag = true;
    }

    if(flag) {
        grid->draw(key_code);
    }

    switch (event->keyval) {
        case GDK_KEY_Escape:
            stop = true;
            delete grid;
            clear_window(cur_data->main_window, 0);
            delete cur_data;
            return TRUE;
        case GDK_KEY_Return:
            grid->to_new_row();
            grid->show_all();
            break;
        case GDK_KEY_space:
            grid->space();
            grid->show_all();
            break;
        case GDK_KEY_BackSpace:
            grid->backspace();
            grid->show_all();
            break;
        case GDK_KEY_Left:
            grid->left_arrow();
            grid->show_all();
            break;
        case GDK_KEY_Right:
            grid->right_arrow();
            grid->show_all();
            break;
        case GDK_KEY_Down: 
            grid->down_arrow();
            grid->show_all();
            break;
        case GDK_KEY_Up: 
            grid->up_arrow();
            grid->show_all();
            break;
        default:
            break;
    }

    return FALSE;  
}

void CreateWindow::show() {
    GtkWidget* box = gtk_overlay_new();
    gtk_container_add(GTK_CONTAINER(window), box);

    grid = new Grid(window, box, width, height, 20);
    stop = false; 
    draw_thread = new std::thread(draw_loop, grid);

    //отслеживание мышки
    gulong button_press_handler = g_signal_connect(window, "button-press-event", G_CALLBACK(on_button_press_event), grid);
    handlers.push_back(button_press_handler);

    gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK);

    //смена курсора мышки
    gulong enter_notify_handler = g_signal_connect(window, "enter-notify-event", G_CALLBACK(on_enter_notify), NULL);
    handlers.push_back(enter_notify_handler);

    gulong realize_handler = g_signal_connect(window, "realize", G_CALLBACK(on_realize), NULL);
    handlers.push_back(realize_handler);

    KeyData* data = new KeyData{grid, this};

    //обработка нажатия кнопок
    gulong key_press_handler = g_signal_connect(G_OBJECT(window), "key-press-event", G_CALLBACK(on_key_press), data);
    handlers.push_back(key_press_handler);
}