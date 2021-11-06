#pragma once
#include <SDL.h>
#include <string>
#include <functional>
#include "utils.h"

struct Animation {
	SDL_Texture* SPRITESHEET;
	std::string name{ "default"};
	std::string path{ "default.png" };
	int length{ -1 };
	int faf{ -1 };
	int force_center{ 0 };
	int move_dir{ 0 };
	SDL_Rect anim_map[MAX_ANIM_LENGTH] = {};

	Animation();
};


SDL_Texture* loadTexture(const char* file_path, SDL_Renderer* renderer);
SDL_Rect getFrame(int frame, Animation* animation);
void loadAnimation(Animation* animation, SDL_Renderer* renderer);