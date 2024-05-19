#include "CreateWindow.h"

CreateWindow::CreateWindow(GtkWidget *window, const int width, const int height){
    this->width = width;
    this->height = height;
    this->window = window;
    gtk_window_set_title(GTK_WINDOW(this->window), "Create Window");
}


void draw_loop(Grid *grid) {
    while(true) {
        if (grid->t % 6 == 0) {
            grid->draw_cursor();
        } else if (grid->t % 6 == 3) {
            grid->delete_cursor();
        }
        grid->t++;


        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

static gboolean on_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data) {
    Grid* grid = static_cast<Grid*>(data);

    int x = static_cast<int>(event->x);
    int y = static_cast<int>(event->y);

    std::pair<int, int> new_coords = grid->nearest_cell(x, y);

    grid->cursor.x = new_coords.first;
    grid->cursor.y = new_coords.second;
    grid->t = 6;

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

static gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer data) {
    int key_code = event->keyval;
    Grid *grid = static_cast<Grid*>(data);
    
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

    return FALSE;  
}

void CreateWindow::show() {
    GtkWidget* box = gtk_overlay_new();
    gtk_container_add(GTK_CONTAINER(window), box);

    //отрисовка сетки
    grid = new Grid(window, box, width, height, 20);
    std::thread* draw_thread = new std::thread(draw_loop, grid);
    draw_thread->detach();

    //отслеживание мышки
    g_signal_connect(window, "button-press-event", G_CALLBACK(on_button_press_event), grid);
    gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK);

    //смена курсора мышки
    g_signal_connect(window, "enter-notify-event", G_CALLBACK(on_enter_notify), NULL);
    g_signal_connect(window, "realize", G_CALLBACK(on_realize), NULL);

    //обработка нажатия кнопок
    g_signal_connect(G_OBJECT(window), "key-press-event", G_CALLBACK(on_key_press), grid);
}