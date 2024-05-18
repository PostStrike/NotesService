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


Grid::Grid(const int width, const int height, const int font_size) {
    this->width = width;
    this->height = height;
    this->font_size = font_size;

    create_grid();
}

void Grid::create_grid() {
    num_rows = height / font_rectangles[font_size].second;
    num_cols = width / font_rectangles[font_size].first;

    grid.assign(num_rows, std::vector<char>(num_cols, ' '));
}

/*
void Grid::draw(cairo_t *cr, char sym) {
    std::string file_path = "../img/symbol_images_" + std::to_string(font_size)
                                  + "/" + std::to_string(static_cast<int>(sym)) + ".png";
    std::cout << file_path << std::endl;
    cairo_surface_t *image_surface = cairo_image_surface_create_from_png(file_path.c_str());

    cairo_set_source_surface(cr, image_surface, cursor_position.first, cursor_position.second);
    cairo_paint(cr);

    // Очищение памяти
    cairo_surface_destroy(image_surface);
}
*/

void Grid::draw_cursor(GtkWidget* window) {
    std::string file_path = "../img/symbol_images_" + std::to_string(font_size)
                                                          + "/cursor_text.png";

    if(cursor.container == nullptr) {
        cursor.child = gtk_image_new_from_file (file_path.c_str());
        cursor.container = gtk_fixed_new();
        gtk_fixed_put(GTK_FIXED(cursor.container), cursor.child, cursor.x * font_rectangles[font_size].first, 
                                                                cursor.y * font_rectangles[font_size].second);
        gtk_container_add(GTK_CONTAINER(window), cursor.container);

        gtk_widget_show_all(window);

        return;
    }

    gtk_fixed_move(GTK_FIXED(cursor.container), cursor.child, cursor.x * font_rectangles[font_size].first, 
                                                              cursor.y * font_rectangles[font_size].second);
    gtk_widget_show(cursor.container);
}   

void Grid::delete_cursor(GtkWidget* window) {
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