using namespace std;
#include <iostream>
#include <sstream>
#include <algorithm> //std::min
#include "utils.h"
int clamp(int min, int value, int max)
{
	if (min <= max)
	{
		if (value < min)
		{
			value = min;
		}
		if (value > max)
		{
			value = max;
		}
	}
	return value;
}

int ymlChopInt(string line)
{
	return stoi(line.substr(line.find("=") + 1));
}

float ymlChopFloat(string line)
{
	return stof(line.substr(line.find("=") + 1));
}

string ymlChopString(string line)
{
	return line.substr(line.find("=") + 1);
}

bool is_collide(SDL_Rect RectA, SDL_Rect RectB)
{
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
	return ((ax1 >= bx0 && (ax0 <= bx0 || ax1 <= bx1)) || (bx1 >= ax0 && (bx0 <= ax0 || bx1 <= ax1))) && ((ay1 <= by0 && (ay0 >= by0 || ay1 >= by1)) || (by1 <= ay0 && (by0 >= ay0 || by1 >= ay1))) || ((ax1 >= bx1 && ax0 <= bx0 && ay1 >= by1 && ay0 <= by0) || (ax1 <= bx1 && ax0 >= bx0 && ay1 <= by1 && ay0 >= by0)) || (bx1 <= ax1 && bx0 >= ax0 && by1 >= ay1 && bx0 <= ay0);
}

SDL_Rect updateCamera(int player1X, int player1Y, int player2X, int player2Y)
{
	SDL_Rect cCamera;

	cCamera.w = std::min(
		std::max(player1X, player2X) + 300 - std::min(player1X, player2X),
		900);

	//0.5625 = WINDOW_HEIGHT / WINDOW_WIDTH. its used to scale the camera for correct proportions
	cCamera.h = cCamera.w * 0.5625;
	cCamera.x = std::min(player1X, player1Y) - 100;

	//559 is the absolute y value of the floor
	cCamera.y = WINDOW_HEIGHT - cCamera.h - (559 - std::min(player2Y, player1Y));

	return cCamera;
}