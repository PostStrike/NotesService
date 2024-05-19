#include "Grid.h"

std::map<int, std::pair<int, int>> Grid::font_rectangles = {
    {10, {10, 14}},
    {20, {20, 28}},
    {30, {30, 42}},
    {40, {40, 56}},
    {50, {50, 70}},
    {60, {60, 84}},
    {70, {70, 98}},
    {80, {80, 112}}
};


Grid::Grid(const int width, const int height, const int font_size, GtkWidget* window) {
    this->width = width;
    this->height = height;
    this->font_size = font_size;

    box = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), box);

    create_grid();
}

Grid::~Grid() {
}

void Grid::create_grid() {
    num_rows = height / font_rectangles[font_size].second;
    num_cols = width / font_rectangles[font_size].first;

    grid.assign(num_rows, std::vector<Fixed>(num_cols));
}

void Grid::draw_cursor(GtkWidget* window) {
    std::string file_path = "../img/symbol_images_" + std::to_string(font_size)
                                                          + "/cursor_text.png";

    if(cursor.container == nullptr) {
        cursor.child = gtk_image_new_from_file(file_path.c_str());
        cursor.container = gtk_fixed_new();
        gtk_fixed_put(GTK_FIXED(cursor.container), cursor.child, cursor.x * font_rectangles[font_size].first, 
                                                                cursor.y * font_rectangles[font_size].second);
        gtk_fixed_put(GTK_FIXED(box), cursor.container, 0, 0);
        gtk_widget_show_all(window);

        return;
    }

    gtk_fixed_move(GTK_FIXED(cursor.container), cursor.child, cursor.x * font_rectangles[font_size].first, 
                                                              cursor.y * font_rectangles[font_size].second);
    gtk_widget_show(cursor.container);
    printf("x = %d, y = %d, \n", cursor.x, cursor.y);
}   

void Grid::draw_symbol(GtkWidget* window, int symbol_code) {
    if(grid[cursor.x][cursor.y].defined) {
        gtk_container_remove(GTK_CONTAINER(box), grid[cursor.x][cursor.y].parent);
    }
    std::string file_path = "../img/symbol_images_" + std::to_string(font_size)
                                                          + "/" + std::to_string(symbol_code) + ".png";
    Fixed fix;
    fix.child = gtk_image_new_from_file(file_path.c_str());
    fix.parent = gtk_fixed_new();
    fix.defined = true;
    grid[cursor.x][cursor.y] = fix;
    gtk_fixed_put(GTK_FIXED(grid[cursor.x][cursor.y].parent), grid[cursor.x][cursor.y].child, 0, 0);
    
    gtk_fixed_put(GTK_FIXED(box), grid[cursor.x][cursor.y].parent, cursor.x * font_rectangles[font_size].first, 
                  cursor.y * font_rectangles[font_size].second);
    gtk_widget_show(grid[cursor.x][cursor.y].parent); 
}

void Grid::delete_cursor(GtkWidget* window) {
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