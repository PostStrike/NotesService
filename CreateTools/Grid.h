#include <vector>
#include <string>
#include <iostream>

#include <gtk/gtk.h>

#ifndef CREATETOOLS_H
#define CREATETOOLS_H

class Grid {
public:
    Grid(const int, const int, const int);

    void draw(cairo_t *, char);
    void draw_cursor(cairo_t *);
private:
    //переменные для области отрисовки
    int width;
    int height;
    
    //переменные для элементов сетки
    int font_size;
    int num_rows;
    int num_cols;

    std::vector<std::vector<char>> grid;

    void create_grid();
};

//functions
inline gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
    Grid *grid = static_cast<Grid*>(data);
    grid->draw_cursor(cr);
    return FALSE;
}

#endif // CREATETOOLS_H