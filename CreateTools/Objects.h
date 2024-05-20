#pragma once

class Object {
public:
	GtkWidget* container = nullptr;
    GtkWidget* child = nullptr;

	int x = 0;
	int y = 0;

	static int space;
	static int num_cols;
	static int num_rows;
	static int font_size;
	static std::map<int, std::pair<int, int>> font_rectangles;
	static GtkWidget* window;
    static GtkWidget* box;

	virtual void move() = 0;
};

class Letter : public Object {
public:
	int size = 0;

	void move_forward(std::map<int, std::vector<Letter>> &grid, int dist, int ind, int cur_x, int cur_y) {
		int copy_y = y;
		int copy_x = x;

		x = cur_x + dist;
		y = cur_y;
		if(x >= font_rectangles[font_size].first * num_cols) {
			int d = x / (font_rectangles[font_size].first * num_cols);
			x = x % (font_rectangles[font_size].first * num_cols);
			y += d * font_rectangles[font_size].second;
		}

		if(y != copy_y) {
			grid[y].push_back(std::move(grid[copy_y][ind]));
			grid[copy_y].erase(grid[copy_y].begin() + ind);
		} 

		move();
	}

	void move_backward(std::map<int, std::vector<Letter>> &grid) {
		
	}

	virtual void move() {
		gtk_fixed_move(GTK_FIXED(container), child, x, y);
    	gtk_widget_show(container);
	}
};

class Cursor : public Object {
public:
	void move_forward(std::map<int, std::vector<Letter>> &grid) {
		int copy_x = x;
		int copy_y = y;

		x += space;
		if(x >= font_rectangles[font_size].first * num_cols) {
			y += font_rectangles[font_size].second;
			x = 0;
		}

		for(int i = font_rectangles[font_size].second * num_rows; i >= copy_y; i -= font_rectangles[font_size].second) {
			for(int ind = grid[i].size() - 1; ind >= 0; ind--) {
				if(grid[i][ind].x >= copy_x || i > copy_y) {
					grid[i][ind].move_forward(grid, (grid[i][ind].x - copy_x) 
							+ (i - copy_y) / font_rectangles[font_size].second * font_rectangles[font_size].first * num_cols, ind, x, y);
					continue;
				}
			}
		}

		move();
	}

	void enter() {
		y += font_rectangles[font_size].second;
    	x = 0;

		move();
	}

	virtual void move() {
		gtk_fixed_move(GTK_FIXED(container), child,  x - font_rectangles[font_size].first / 2, y);
    	gtk_widget_show(container);
	}

	void move_backward(std::map<int, std::vector<Letter>> &grid) {
		int left = x - space;
		int right = x;

		Letter* obj = nullptr;
		int ind = 0;
		for(int i = 0; i < grid[y].size(); ++i) {
			int a1 = grid[y][i].x;
			int a2 = grid[y][i].x + grid[y][i].size;
			int b1 = left;
			int b2 = right;
			if((obj == nullptr || (b2 - a2 < b2 - obj->x - obj->size)) && b2 > a1 && b1 <= a1) {
				obj = &grid[y][i];
				ind = i;
			}
		}

		if(obj == nullptr) {
			x -= space;
			if(x < 0) {
				x = (y == 0) ? 0 : num_cols * font_rectangles[font_size].first;
				y = std::max(y - font_rectangles[font_size].second, 0);
			}
			move();
			return;
		}

		x = obj->x; move();
		gtk_container_remove(GTK_CONTAINER(box), obj->container);
		grid[y].erase(grid[y].begin() + ind);
	}
};