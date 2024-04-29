#include <vector>
#include <gtk/gtk.h>

#ifndef CREATETOOLS_H
#define CREATETOOLS_H

class Grid {
public:
    Grid(const int, const int);

    void draw(cairo_t *);
private:
    int width;
    int height;
    
    int cell_width = 20;
    int cell_height = 30;
    int num_rows;
    int num_cols;
    int padding;

    std::vector<std::vector<char>> grid;

    void create_grid();
};

//functions
inline gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
    Grid *grid = static_cast<Grid*>(data);
    grid->draw(cr);
    return FALSE;
}

#endif // CREATETOOLS_H