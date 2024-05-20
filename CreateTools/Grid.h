#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <chrono>
#include <thread>
#include <queue>
#include <algorithm>

#include <gtk/gtk.h>

#include "Objects.h"

#ifndef CREATETOOLS_H
#define CREATETOOLS_H

class Grid {
public:
    Grid(GtkWidget*, GtkWidget*, const int, const int, const int);

    void draw(int);
    void draw_cursor();
    void delete_cursor();
    std::pair<int, int> nearest_cell(int, int);
    void to_new_row();
    void space();
    void backspace();
    void left_arrow();
    void right_arrow();
    void up_arrow();
    void down_arrow();
    void move_objects_back(int, int, int);
    void show_all();

    // Время существования сетки
    int t = 0;

    //курсор
    Cursor cursor;
    
    //область рисования
    GtkWidget* drawing_area;

    static std::map<int, std::pair<int, int>> font_rectangles;
    static std::map<std::string, std::pair<int, int>> spaces; 
private:
    GtkWidget* window;
    GtkWidget* box;

    // Переменные для области отрисовки
    int width;
    int height;
    
    // Переменные для элементов сетки
    int font_size;
    int num_rows;
    int num_cols;

    std::vector<Letter*> grid;
    std::vector<Object*> objects;

    void create_grid();
};

#endif // CREATETOOLS_H