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

    Object::space = (int)font_rectangles[font_size].first;
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

void Grid::draw(int key_code) {
    std::string file_path = "../img/symbol_images_" + std::to_string(font_size) + "/" + std::to_string(key_code) + ".png";
    std::string space_path = "symbol_images_" + std::to_string(font_size) + "/" + std::to_string(key_code) + ".png";
    
    int real_size = spaces[space_path].first + spaces[space_path].second;
    int x = cursor.x;
    int y = cursor.y;

    Letter obj;
    obj.child = gtk_image_new_from_file(file_path.c_str());  
    obj.container = gtk_fixed_new();
    obj.size = real_size;
    obj.x = x;
    obj.y = y;

    grid[y].push_back(obj);

    gtk_fixed_put(GTK_FIXED(grid[y][grid[y].size() - 1].container), grid[y][grid[y].size() - 1].child, x, y);
    
    gtk_overlay_add_overlay(GTK_OVERLAY(box), grid[y][grid[y].size() - 1].container);

    gtk_widget_show(grid[y][grid[y].size() - 1].container); 
    gtk_widget_show(grid[y][grid[y].size() - 1].child); 
    
    cursor.x = cursor.x + real_size;
    cursor.move();
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
    cursor.move_backward(grid);
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
    for(const Letter& el : grid[new_y]) {
        if(new_x > el.x && new_x <= el.x + el.size) {
            new_x = el.x + el.size;
        }
    }

    return {new_x, new_y};
}