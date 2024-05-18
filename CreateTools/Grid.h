#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <chrono>
#include <thread>

#include <gtk/gtk.h>

#include "Cursor.h"

#ifndef CREATETOOLS_H
#define CREATETOOLS_H

class Grid {
public:
    Grid(const int, const int, const int);

    void draw(cairo_t*, char);
    void draw_cursor(GtkWidget*);
    void delete_cursor(GtkWidget*);
    std::pair<int, int> nearest_cell(int, int);

    // Время существования сетки
    int t = 0;

    //курсор
    Cursor cursor;

    static std::map<int, std::pair<int, int>> font_rectangles;
private:
    // Переменные для области отрисовки
    int width;
    int height;
    
    // Переменные для элементов сетки
    int font_size;
    int num_rows;
    int num_cols;

    std::vector<std::vector<char>> grid;

    void create_grid();
};

#endif // CREATETOOLS_H