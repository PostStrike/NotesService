#pragma once

class Transformer {
public:
	virtual void move_forward(std::vector<void*>&, void*&, std::vector<int>) = 0;
};

template<class T1, class T2>
class CursorTransformer : public Transformer{
	void move_forward(std::vector<void*> &void_grid, void*& void_cursor, std::vector<int> additional_inf) {
		std::vector<T1*>& grid = reinterpret_cast<std::vector<T1*>&>(void_grid);
		T2& cursor = *static_cast<T2*>(void_cursor);

		int copy_x = cursor.x;
		int copy_y = cursor.y;

		cursor.x += cursor.space;
		if(cursor.x >= cursor.font_rectangles[cursor.font_size].first * cursor.num_cols) {
			cursor.y += cursor.font_rectangles[cursor.font_size].second;
			cursor.x = 0;
		}

		int i = copy_y;
		int copy_size = grid.size();
		int mx = 0;
		for(int ind = grid.size() - 1; ind >= 0; ind--) {
			if(grid[ind]->y != i) continue;
			if(grid[ind]->x >= copy_x) {
				grid[ind]->move_forward(grid, grid[ind]->x - copy_x, ind, cursor.x, cursor.y);
			}
		}
	}
};

template<class T1, class T2>
class LetterTransformer : public Transformer{
	void move_forward(std::vector<void*> &void_grid, void*& void_letter, std::vector<int> additional_inf) {
		std::vector<T1*>& grid = reinterpret_cast<std::vector<T1*>&>(void_grid);
		T2& letter = *static_cast<T2*>(void_letter);

		int dist = additional_inf[0]; 
		int ind = additional_inf[1];
		int cur_x = additional_inf[2];
		int cur_y = additional_inf[3];
 
		int copy_y = letter.y;
		int copy_x = letter.x;

		letter.x = cur_x + dist;
		letter.y = cur_y;

		if(letter.x >= letter.font_rectangles[letter.font_size].first * letter.num_cols || letter.y != copy_y) {
			letter.x = copy_x;
			letter.y = copy_y;
			std::vector<std::pair<T2*, int>> word = letter.get_word(grid, copy_y, letter.size, letter.x);
			
			int full_size = 0;
			for(int i = 0; i < word.size(); ++i) {
				std::cout << word[i].first->sym;
				full_size += word[i].first->size;
			}
			std::cout << std::endl;

			std::vector<std::pair<int, int>> arr;
			for(int i = 0; i < grid.size(); ++i) {
				if(grid[i]->y == copy_y + letter.font_rectangles[letter.font_size].second) {
					arr.push_back({grid[i]->x, i});
				}
			}

			sort(arr.begin(), arr.end());
			for(int i = arr.size() - 1; i >= 0; --i) {
				int ind = arr[i].second;
				if(grid[ind]->y != copy_y + letter.font_rectangles[letter.font_size].second) continue;
				grid[ind]->move_forward(grid, full_size + letter.space + grid[ind]->x, ind, 0, grid[ind]->y);
			}

			int new_x = 0;
			for(int i = 0; i < word.size(); ++i) {
				T2* temp = word[i].first;
				int ind = word[i].second;

				temp->x = new_x;
				temp->y = copy_y + letter.font_rectangles[letter.font_size].second;

				new_x += temp->size;
			}
		} 
	}
};

class Object {
public:
	GtkWidget* container = nullptr;
    GtkWidget* child = nullptr;

	Transformer* transformer = nullptr;

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
	char sym;

	Letter() {
		this->transformer = new LetterTransformer<Letter, Letter>();
	}

	~Letter() {
		delete this->transformer;
	}

	virtual void move() {
		gtk_fixed_move(GTK_FIXED(container), child, x, y);
    	gtk_widget_show(container);
	}

	void move_forward(std::vector<Letter*> &grid, int dist, int ind, int cur_x, int cur_y) {
		void* void_this = this;
		transformer->move_forward(reinterpret_cast<std::vector<void*>&>(grid), void_this, std::vector<int>{dist, ind, cur_x, cur_y});
	}

	static bool comp(const std::pair<Letter*, int> lhs, const std::pair<Letter*, int> rhs) {
		return lhs.first->x < rhs.first->x;
	}

	std::vector<std::pair<Letter*, int>> get_word(std::vector<Letter*> &grid, int i, int prev_size, int prev_x) {
		std::vector<std::pair<Letter*, int>> temp;
		for(int j = 0; j < grid.size(); ++j) {
			if(grid[j]->y != i) continue;
			temp.push_back({grid[j], j});
		}

		std::vector<std::pair<Letter*, int>> res;
		std::sort(temp.begin(), temp.end(), comp);
		for(int j = 0; j < temp.size(); ++j) {
			if(temp[j].first->x == prev_x) {
				std::cout << 200 << std::endl;
				int temp_x = prev_x;
				int k = j;
				while(k - 1 >= 0 && temp[k - 1].first->x + temp[k - 1].first->size == temp_x) {
					k -= 1;
					temp_x = temp[k].first->x;
				}

				for(k; k <= j; ++k) {
					res.push_back(temp[k]);
				}

				int temp_size = prev_size;
				temp_x = prev_x;
				k = j;
				while(k + 1 < temp.size() && temp[k + 1].first->x == temp_x + temp_size) {
					k += 1;
					temp_x = temp[k].first->x;
					temp_size = temp[k].first->size;
				}

				for(int l = j + 1; l <= k; ++l) {
					res.push_back(temp[l]);
				}

				return res;
			}
		}

		return res;
	}
};

class Cursor : public Object {
public:
	Cursor() {
		this->transformer = new CursorTransformer<Letter, Cursor>();
	}

	~Cursor() {
		delete this->transformer;
	}

	virtual void move() {
		gtk_fixed_move(GTK_FIXED(container), child,  x - font_rectangles[font_size].first / 2, y);
    	gtk_widget_show(container);
	}

	void move_forward(std::vector<Letter*> &grid) {
		void* void_this = this;
		transformer->move_forward(reinterpret_cast<std::vector<void*>&>(grid), void_this, std::vector<int>{});
	}

	void move_letters(std::vector<Letter*> &grid, int dist) {
		int copy_x = x;
		int copy_y = y;

		x += dist;
		if(x >= font_rectangles[font_size].first * num_cols) {
			y += font_rectangles[font_size].second;
			x = 0;
		}

		int i = copy_y;
		int copy_size = grid.size();
		int mx = 0;
		for(int ind = grid.size() - 1; ind >= 0; ind--) {
			if(grid[ind]->y != i) continue;
			if(grid[ind]->x >= copy_x) {
				grid[ind]->move_forward(grid, grid[ind]->x - copy_x, ind, x, y);
			}
		}

		x -= dist;
	}

	void enter() {
		y += font_rectangles[font_size].second;
    	x = 0;
	}

	void left_arrow(std::vector<Letter*> &grid) {
		int left = x - space;
		int right = x;

		Letter* obj = nullptr;
		int ind = 0;
		for(int i = 0; i < grid.size(); ++i) {
			if(grid[i]->y != y) continue;
			int a1 = grid[i]->x;
			int a2 = grid[i]->x + grid[i]->size;
			int b1 = left;
			int b2 = right;
			if((obj == nullptr || (b2 - a2 < b2 - obj->x - obj->size)) && b2 > a1 && b1 <= a1) {
				obj = grid[i];
				ind = i;
			}
		}

		if(obj == nullptr) {
			x -= space;
			if(x < 0) {
				x = (y == 0) ? 0 : num_cols * font_rectangles[font_size].first;
				y = std::max(y - font_rectangles[font_size].second, 0);
			}
			return;
		}

		x = obj->x; 
	}

	void down_arrow() {
		y += font_rectangles[font_size].second;
		y = std::min(y, font_rectangles[font_size].second * num_rows);
	}

	void up_arrow() {
		y -= font_rectangles[font_size].second;
		y = std::max(y, 0);
	}

	void right_arrow(std::vector<Letter*> &grid) {
		int copy_x = x;

		int left = x;
		int right = x + space;

		Letter* obj = nullptr;
		int ind = 0;
		for(int i = 0; i < grid.size(); ++i) {
			if(grid[i]->y != y) continue;
			int a1 = grid[i]->x;
			int a2 = grid[i]->x + grid[i]->size;
			int b1 = left;
			int b2 = right;
			if((obj == nullptr || (b2 - a2 < b2 - obj->x - obj->size)) && b2 > a1 && b1 <= a1) {
				obj = grid[i];
				ind = i;
			}
		}

		if(obj == nullptr) {
			x += space;
			if(x > num_cols * font_rectangles[font_size].first) {
				x = (y == num_rows * font_rectangles[font_size].second) ? x : 0;
				y = std::min(y + font_rectangles[font_size].second, num_rows * font_rectangles[font_size].second);
			}
			return;
		}

		x = (copy_x == obj->x) ? obj->x + obj->size : obj->x;
	}

	std::vector<std::pair<Letter*, int>> get_phrase(std::vector<Letter*> &grid, int i, int free_space) {
		std::vector<std::pair<Letter*, int>> temp;
		for(int j = 0; j < grid.size(); ++j) {
			if(grid[j]->y != i) continue;
			temp.push_back({grid[j], j});
		}
	
		std::vector<std::pair<Letter*, int>> res;

		if(temp.size() == 0) {
			for(int j = 0; j < grid.size(); ++j) {
				if(grid[j]->y > i) {
					grid[j]->y = grid[j]->y - font_rectangles[font_size].second;
				}
			}

			return res; 
		}

		std::sort(temp.begin(), temp.end(), grid.back()->comp);

		int max_i = 0;
		for(int j = 0; j < temp.size(); ++j) {
			if(temp[j].first->x > free_space) {
				break;
			}
			max_i = j;
		}

		while(max_i + 1 < temp.size() && max_i >= 0 && temp[max_i + 1].first->x - temp[max_i].first->x - temp[max_i].first->size == 0) {
			max_i -= 1;
		}

		for(int j = 0; j <= max_i; ++j) {
			res.push_back(temp[j]);
		}

		return res;
	}

	void move_back(std::vector<Letter*> &grid) {
		int left = -1;
		int right = x;

		Letter* obj = nullptr;
		int ind = 0;
		for(int i = 0; i < grid.size(); ++i) {
			if(grid[i]->y != y) continue;
			int a1 = grid[i]->x;
			int a2 = grid[i]->x + grid[i]->size;
			int b1 = left;
			int b2 = right;
			if((obj == nullptr || (b2 - a2 < b2 - obj->x - obj->size)) && b2 > a1 && b1 <= a1) {
				obj = grid[i];
				ind = i;
			}
		}

		if(obj == nullptr) {
			x = 0;
		}
		else {
			x = obj->x + obj->size;
		}

		int free_space = font_rectangles[font_size].first * num_cols - x;

		std::vector<std::pair<Letter*, int>> word = get_phrase(grid, y + font_rectangles[font_size].second, free_space);
		for(int i = 0; i < word.size(); ++i) {
			int ind = word[i].second;
			grid[ind]->y = y;
			grid[ind]->x = x + grid[ind]->x;
		}
	}

	void move_backward(std::vector<Letter*> &grid, std::vector<Object*> &objects) {
		int copy_y = y;
		int copy_x = x;
		int left = x - space;
		int right = x;

		Letter* obj = nullptr;
		int ind = 0;
		for(int i = 0; i < grid.size(); ++i) {
			if(grid[i]->y != y) continue;
			int a1 = grid[i]->x;
			int a2 = grid[i]->x + grid[i]->size;
			int b1 = left;
			int b2 = right;
			if((obj == nullptr || (b2 - a2 < b2 - obj->x - obj->size)) && b2 > a1 && b1 <= a1) {
				obj = grid[i];
				ind = i;
			}
		}

		if(obj == nullptr) {
			x -= space;
			if(x < 0) {
				x = (y == 0) ? 0 : num_cols * font_rectangles[font_size].first;
				y = std::max(y - font_rectangles[font_size].second, 0);
			}

			if(y == copy_y) {
				for(int ind = 0; ind < grid.size(); ind++) {
					if(grid[ind]->y == y && grid[ind]->x >= x) {
						grid[ind]->x -= std::abs(copy_x - x);
					}
				}
			}
			else {
				move_back(grid);
			}

			return;
		}

		x = obj->x;
		gtk_container_remove(GTK_CONTAINER(box), obj->container);
		delete grid[ind];
		grid.erase(grid.begin() + ind);
		objects.erase(objects.begin() + ind + 1);

		for(int ind = 0; ind < grid.size(); ind++) {
			if(grid[ind]->y == y && grid[ind]->x >= x) {
				grid[ind]->x -= std::abs(copy_x - x);
			}
		}
	}
};