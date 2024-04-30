#include "Grid.h"

Grid::Grid(const int width, const int height, const int font_size) {
    this->width = width;
    this->height = height;
    this->font_size = font_size;

    create_grid();
}

void Grid::create_grid() {
    int num_rows = height;
    int num_cols = width;

    grid.assign(num_rows, std::vector<char>(num_cols, 'A'));
}

void Grid::draw(cairo_t *cr, char sym) {
    std::string file_path = "../img/symbol_images_" + std::to_string(font_size)
                                  + "/" + std::to_string(static_cast<int>(sym)) + ".png";
    std::cout << file_path << std::endl;
    cairo_surface_t *image_surface = cairo_image_surface_create_from_png(file_path.c_str());

    cairo_set_source_surface(cr, image_surface, 0, 0);
    cairo_paint(cr);

    // Очищение памяти
    cairo_surface_destroy(image_surface);
}

void Grid::draw_cursor(cairo_t *cr) {
    std::string file_path = "../img/symbol_images_" + std::to_string(font_size)
                                                          + "/cursor_text.png";
    cairo_surface_t *cursor_surface = cairo_image_surface_create_from_png(file_path.c_str());

    cairo_set_source_surface(cr, cursor_surface, 0, 0);
    cairo_paint(cr);

    // Очищение памяти
    cairo_surface_destroy(cursor_surface);
}