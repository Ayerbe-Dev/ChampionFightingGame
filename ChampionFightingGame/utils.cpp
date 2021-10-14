using namespace std;
#include <iostream>
#include <sstream>
#include "utils.h"
int clamp(int min, int value, int max) {
	if (min <= max) {
		if (value < min) {
			value = min;
		}
		if (value > max) {
			value = max;
		}
	}
	return value;
}

int ymlChopInt(string line) {
	return stoi(line.substr(line.find("=") + 1));
}

float ymlChopFloat(string line) {
	return stof(line.substr(line.find("=") + 1));
}

string ymlChopString(string line) {
	return line.substr(line.find("=") + 1);
}

bool is_collide(SDL_Rect RectA, SDL_Rect RectB) {
	int ax0 = RectA.x;
	int ax1 = RectA.x + RectA.w;
	int ay0 = RectA.y;
	int ay1 = RectA.y + RectA.h;
	int bx0 = RectB.x;
	int bx1 = RectB.x + RectB.w;
	int by0 = RectB.y;
	int by1 = RectB.y + RectB.h;
	bool horizontal_check = false;
	bool vertical_check = false;
	return ((ax1 >= bx0 && (ax0 <= bx0 || ax1 <= bx1)) || (bx1 >= ax0 && (bx0 <= ax0 || bx1 <= ax1))) && ((ay1 <= by0 && (ay0 >= by0 || ay1 >= by1)) || (by1 <= ay0 && (by0 >= ay0 || by1 >= ay1)));
}