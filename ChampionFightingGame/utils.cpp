using namespace std;
#include <iostream>
#include <sstream>
#include <algorithm> //std::min
#include "utils.h"
#include <SDL_image.h>
int clamp(int min, int value, int max) {
	if (min <= max)	{
		if (value < min) {
			value = min;
		}
		if (value > max) {
			value = max;
		}
	}
	return value;
}

float clampf(float min, float value, float max) {
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
	return ((ax1 >= bx0 && (ax0 <= bx0 || ax1 <= bx1)) || (bx1 >= ax0 && (bx0 <= ax0 || bx1 <= ax1))) && ((ay1 <= by0 && (ay0 >= by0 || ay1 >= by1)) || (by1 <= ay0 && (by0 >= ay0 || by1 >= ay1))) || ((ax1 >= bx1 && ax0 <= bx0 && ay1 >= by1 && ay0 <= by0) || (ax1 <= bx1 && ax0 >= bx0 && ay1 <= by1 && ay0 >= by0)) || (bx1 <= ax1 && bx0 >= ax0 && by1 >= ay1 && bx0 <= ay0);
}

SDL_Rect updateCamera(int player1X, int player1Y, int player2X, int player2Y)
{
	SDL_Rect cCamera;

	cCamera.w = std::min(
		std::max(
			std::abs(player1X - player2X) + 150,
			CAMERA_MAX_ZOOM_IN),
		CAMERA_MAX_ZOOM_OUT);

	//0.5625 = WINDOW_HEIGHT / WINDOW_WIDTH. its used to scale the camera for correct proportions
	cCamera.h = cCamera.w * WINDOW_FACTOR;

	//78 is the current average size of the character.
	cCamera.x = ((player1X + player2X) / 2) - (cCamera.w / 2) + (78 / 2);

	if (cCamera.x + cCamera.w > WINDOW_WIDTH)
	{
		cCamera.x = WINDOW_WIDTH - cCamera.w;
	}
	else if (cCamera.x < 0)
	{
		cCamera.x = 0;
	}

	//559 is the absolute y value of the floor
	//JUMP_FOLLOW_THRESHOLD is the jump line before the camera starts moving
	int iYdelta = (559 - std::min(player2Y, player1Y));
	if (iYdelta >= JUMP_FOLLOW_THRESHOLD)
	{
		cCamera.y = WINDOW_HEIGHT - cCamera.h - iYdelta + JUMP_FOLLOW_THRESHOLD - 50;
	}
	else
	{
		cCamera.y = WINDOW_HEIGHT - cCamera.h - 50;
	}

	return cCamera;
}

SDL_Texture* loadTexture(const char* file_path, SDL_Renderer* renderer) {
	SDL_Surface* image_surface = IMG_Load(file_path);
	if (image_surface == NULL) {
		cout << "Error loading image: " << IMG_GetError() << endl;
	}
	SDL_Texture* ret = SDL_CreateTextureFromSurface(renderer, image_surface);
	SDL_FreeSurface(image_surface);
	return ret;
}