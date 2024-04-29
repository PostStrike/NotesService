#include "Grid.h"

Grid::Grid(const int width, const int height) {
    this->width = width;
    this->height = height;

    create_grid();
}

void Grid::create_grid() {
    int num_rows = height / cell_height;
    int num_cols = width / cell_width;

    grid.assign(num_rows, std::vector<char>(num_cols, 'A'));

    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr = cairo_create(surface);
    draw(cr);
}

void Grid::draw(cairo_t *cr) {
    cairo_surface_t *image_surface = cairo_image_surface_create_from_png("../img/cursor.png");
    
    // Получение размеров изображения
    double width = cairo_image_surface_get_width(image_surface);
    double height = cairo_image_surface_get_height(image_surface);
    
    // Нарисовать изображение на контексте рисования
    cairo_set_source_surface(cr, image_surface, 0, 0);
    cairo_paint(cr);

    // Освобождение ресурсов
    cairo_surface_destroy(image_surface);
}