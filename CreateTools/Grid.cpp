#include "Grid.h"

#include "spaces.h"

std::map<int, std::pair<int, int>> Grid::font_rectangles = {
    {10, {10, 14}},
    {20, {18, 26}},
    {30, {30, 42}},
    {40, {40, 56}},
    {50, {50, 70}},
    {60, {60, 84}},
    {70, {70, 98}},
    {80, {80, 112}}
};

std::map<int, std::pair<int, int>> Object::font_rectangles = {
    {10, {10, 14}},
    {20, {18, 26}},
    {30, {30, 42}},
    {40, {40, 56}},
    {50, {50, 70}},
    {60, {60, 84}},
    {70, {70, 98}},
    {80, {80, 112}}
};

int Object::space = 0;
int Object::num_cols = 0;
int Object::num_rows = 0;
int Object::font_size = 0;
GtkWidget* Object::window = nullptr;
GtkWidget* Object::box = nullptr;

Grid::Grid(GtkWidget* window, GtkWidget* box, const int width, const int height, const int font_size) {
    this->window = window;
    this->width = width;
    this->height = height;
    this->font_size = font_size;
    this->box = box;

    Object::space = (int)(font_rectangles[font_size].first / 2);
    Object::font_size = font_size;
    Object::window = window;
    Object::box = box;

    create_grid();
}

void Grid::create_grid() {
    num_rows = height / font_rectangles[font_size].second - 1;
    num_cols = width / font_rectangles[font_size].first;

    Object::num_cols = num_cols;
    Object::num_rows = num_rows;
}

Grid::~Grid() {
    for(int i = 0; i < grid.size(); ++i) {
        gtk_container_remove(GTK_CONTAINER(box), grid[i]->container);
        delete grid[i];
    }
    gtk_container_remove(GTK_CONTAINER(box), cursor.container);
    gtk_container_remove(GTK_CONTAINER(window), box);
}

void Grid::draw(int key_code) {
    std::string file_path = "../img/symbol_images_" + std::to_string(font_size) + "/" + std::to_string(key_code) + ".png";
    std::string space_path = "symbol_images_" + std::to_string(font_size) + "/" + std::to_string(key_code) + ".png";
    
    int real_size = spaces[space_path].first + spaces[space_path].second;
    int x = cursor.x; 
    int y = cursor.y;

    Letter* obj = new Letter;
    obj->child = gtk_image_new_from_file(file_path.c_str());  
    obj->container = gtk_fixed_new();
    obj->size = real_size;
    obj->x = x;
    obj->y = y;
    obj->sym = key_code;

    cursor.move_letters(grid, obj->size);
    show_all();

    grid.push_back(obj);
    objects.push_back(obj);

    gtk_fixed_put(GTK_FIXED(grid.back()->container), grid.back()->child, x, y);
    
    gtk_overlay_add_overlay(GTK_OVERLAY(box), grid.back()->container);

    gtk_widget_show(grid.back()->container); 
    gtk_widget_show(grid.back()->child); 
    
    cursor.x = cursor.x + real_size;
    cursor.move();
}

void Grid::draw_const(int key_code, int x, int y) {
    std::string file_path = "../img/symbol_images_" + std::to_string(font_size) + "/" + std::to_string(key_code) + ".png";
    std::string space_path = "symbol_images_" + std::to_string(font_size) + "/" + std::to_string(key_code) + ".png";
    
    int real_size = spaces[space_path].first + spaces[space_path].second;

    Letter* obj = new Letter;
    obj->child = gtk_image_new_from_file(file_path.c_str());  
    obj->container = gtk_fixed_new();
    obj->size = real_size;
    obj->x = x;
    obj->y = y;
    obj->sym = key_code;

    grid.push_back(obj);
    objects.push_back(obj);

    gtk_fixed_put(GTK_FIXED(grid.back()->container), grid.back()->child, x, y);
    
    gtk_overlay_add_overlay(GTK_OVERLAY(box), grid.back()->container);

    gtk_widget_show(grid.back()->container); 
    gtk_widget_show(grid.back()->child); 
}

void Grid::draw_cursor() {
    std::string file_path = "../img/symbol_images_" + std::to_string(font_size)
                                                          + "/cursor_text.png";

    if(cursor.container == nullptr) {
        cursor.child = gtk_image_new_from_file(file_path.c_str());
        cursor.container = gtk_fixed_new();
        
        gtk_fixed_put(GTK_FIXED(cursor.container), cursor.child, cursor.x - font_rectangles[font_size].first / 2, cursor.y);
        gtk_overlay_add_overlay(GTK_OVERLAY(box), cursor.container);
        gtk_widget_show(box); 
        gtk_widget_show(cursor.container); 
        gtk_widget_show(cursor.child); 

        objects.push_back(&cursor);

        return;
    }

    cursor.move();
}   

void Grid::delete_cursor() {
    if(cursor.container == nullptr) return;

    gtk_widget_hide(cursor.container);
}

void Grid::to_new_row() {
    cursor.enter();
}

void Grid::space() {
    cursor.move_forward(grid);
}

void Grid::backspace() {
    cursor.move_backward(grid, objects);
}

void Grid::left_arrow() { 
    cursor.left_arrow(grid);
}

void Grid::right_arrow() {
    cursor.right_arrow(grid);
}

void Grid::down_arrow() {
    cursor.down_arrow();
}

void Grid::up_arrow() {
    cursor.up_arrow();
}

void Grid::show_all() {
    for(Object* obj : objects) {
        obj->move();
    }
}

int binsearch(int l, int r, int d, int x) {
    int copy_r = r;

    while(l < r) {
        int mid = (l + r) / 2;
        int dist = (copy_r - mid) * d;

        if(x >= dist) {
            r = mid;
        }
        else {
            l = mid + 1;
        }
    }

    return copy_r - l;
}

std::pair<int, int> Grid::nearest_cell(int x, int y) {
    int new_y = binsearch(0, num_rows, font_rectangles[font_size].second, y) * font_rectangles[font_size].second;

    int new_x = x;
    for(const Letter* el : grid) {
        if(new_x > el->x && new_x <= el->x + el->size) {
            new_x = el->x;
        }
    }

    return {new_x, new_y};
}

void save_grid_to_file(std::string& filename, Grid* grid) {
    filename = "../notes/" + filename + ".txt";
    std::ofstream file(filename);

    file << grid->get_grid().size() << "\n";
    for(const Letter* obj : grid->get_grid()) {
        file << obj->x << " " << obj->y << " " << obj->sym << "\n";
    }

    file.close();
    printf("Data saved to %s\n", filename.c_str());
}

static void on_save_button_clicked(GtkWidget *button, gpointer data) {
    const gchar *name = gtk_entry_get_text(GTK_ENTRY(data));
    std::string filename(name);

    Grid* grid = static_cast<Grid*>(g_object_get_data(G_OBJECT(button), "grid"));

    save_grid_to_file(filename, grid);
    gtk_widget_destroy(gtk_widget_get_toplevel(button));
}

void show_input_dialog(Grid* grid) {
    GtkWidget *dialog, *entry, *button, *box;

    dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(dialog), "Save Grid");
    gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 100);
    gtk_container_set_border_width(GTK_CONTAINER(dialog), 10);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(dialog), box);

    entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box), entry, TRUE, TRUE, 0);

    button = gtk_button_new_with_label("Save");
    gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 0);

    g_object_set_data(G_OBJECT(button), "grid", grid);

    g_signal_connect(button, "clicked", G_CALLBACK(on_save_button_clicked), entry);

    gtk_widget_show_all(dialog);
}

void Grid::save() {
    show_input_dialog(this);
}
