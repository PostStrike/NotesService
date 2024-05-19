#include "Grid.h"

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


Grid::Grid(GtkWidget* window, GtkWidget* box, const int width, const int height, const int font_size) {
    this->window = window;
    this->width = width;
    this->height = height;
    this->font_size = font_size;
    this->box = box;

    create_grid();
}

void Grid::create_grid() {
    num_rows = height / font_rectangles[font_size].second - 1;
    num_cols = width / font_rectangles[font_size].first;

    grid.assign(num_rows, std::vector<char>(num_cols, 0));
}

void Grid::draw(int key_code) {
    std::cout << key_code << std::endl;
    if(grid[cursor.x][cursor.y]) {
        return;
    }
    grid[cursor.x][cursor.y] = key_code;

    std::string file_path = "../img/symbol_images_" + std::to_string(font_size) + "/" + std::to_string(key_code) + ".png";

    GtkWidget* image = gtk_image_new_from_file(file_path.c_str());  
    GtkWidget* fix = gtk_fixed_new();

    int x = cursor.x * font_rectangles[font_size].first;
    int y = cursor.y * font_rectangles[font_size].second;

    gtk_fixed_put(GTK_FIXED(fix), image, x, y);
    
    gtk_overlay_add_overlay(GTK_OVERLAY(box), fix);

    gtk_widget_show(fix); 
    gtk_widget_show(image); 
}

void Grid::draw_cursor() {
    std::string file_path = "../img/symbol_images_" + std::to_string(font_size)
                                                          + "/cursor_text.png";

    if(cursor.container == nullptr) {
        cursor.child = gtk_image_new_from_file(file_path.c_str());
        cursor.container = gtk_fixed_new();
        
        gtk_fixed_put(GTK_FIXED(cursor.container), cursor.child, 
                    cursor.x * font_rectangles[font_size].first - font_rectangles[font_size].first / 2, 
                    cursor.y * font_rectangles[font_size].second);

        gtk_overlay_add_overlay(GTK_OVERLAY(box), cursor.container);
        gtk_widget_show(box); 
        gtk_widget_show(cursor.container); 
        gtk_widget_show(cursor.child); 
        return;
    }

    gtk_fixed_move(GTK_FIXED(cursor.container), cursor.child, 
                  cursor.x * font_rectangles[font_size].first - font_rectangles[font_size].first / 2, 
                  cursor.y * font_rectangles[font_size].second);

    gtk_widget_show(cursor.container);
}   

void Grid::delete_cursor() {
    if(cursor.container == nullptr) return;

    gtk_widget_hide(cursor.container);
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
    int new_x = binsearch(0, num_cols, font_rectangles[font_size].first, x);
    int new_y = binsearch(0, num_rows, font_rectangles[font_size].second, y);

    return {new_x, new_y};
}