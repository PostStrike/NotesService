#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <chrono>
#include <thread>

#include <gtk/gtk.h>

#ifndef CREATETOOLS_H
#define CREATETOOLS_H

class Grid {
public:
    Grid(const int, const int, const int);

    void draw(cairo_t *, char);
    void draw_cursor(cairo_t *);
    void delete_cursor(cairo_t *);

    // Время существования сетки
    int t = 0;
private:
    // Переменные для области отрисовки
    int width;
    int height;
    
    // Переменные для элементов сетки
    int font_size;
    int num_rows;
    int num_cols;

    std::map<int, std::pair<int, int>> FontRectangles {
        {10, {10, 14}},
        {20, {20, 28}},
        {30, {30, 42}},
        {40, {40, 56}},
        {50, {50, 70}},
        {60, {60, 84}},
        {70, {70, 98}},
        {80, {80, 112}}
    };

    std::vector<std::vector<char>> grid;

    void create_grid();
};

inline gboolean draw_callback(GtkWidget *widget, cairo_t *cr, gpointer data) {
    Grid *grid = static_cast<Grid*>(data);
    
    if(grid->t % 2 == 0) {
        grid->draw_cursor(cr);
    }
    else {
        grid->delete_cursor(cr);
    }

    return TRUE;
}

#endif // CREATETOOLS_H