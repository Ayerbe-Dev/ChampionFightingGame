#pragma once
#include "FighterInstance.h"
#include "utils.h"
#include <string>
#include <SDL.h>

class HealthBar {
public:
	FighterInstance* fighter_instance;
	SDL_Texture* health_texture;
	SDL_Texture* bar_texture;
	SDL_Rect health_rect;
	SDL_Rect bar_rect;
	f32 max_health;

	HealthBar();
	HealthBar(SDL_Renderer* renderer, FighterInstance* fighter_instance);
};

class PlayerIndicator {
public:
	FighterInstance* fighter_instance;
	SDL_Texture* texture;
	SDL_Rect indicator_rect;
	string nametag;

	PlayerIndicator();
	PlayerIndicator(SDL_Renderer *renderer, FighterInstance *fighter_instance, string nametag = "");
};

class Timer {
public:
	int remaining;
	SDL_Texture* texture;
	SDL_Rect timer_rect;

	Timer();
	Timer(SDL_Renderer* renderer, int remaining);
};