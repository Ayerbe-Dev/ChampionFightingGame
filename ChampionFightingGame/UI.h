#pragma once
#include "PlayerInfo.h"
#include "utils.h"
#include <string>
#include <SDL.h>

class HealthBar {
public:
	PlayerInfo* player_info;
	SDL_Texture* health_texture;
	SDL_Texture* bar_texture;
	SDL_Rect health_rect;
	SDL_Rect bar_rect;
	f32 max_health;

	HealthBar();
	HealthBar(SDL_Renderer* renderer, PlayerInfo* player_info);
};

class PlayerIndicator {
public:
	PlayerInfo* player_info;
	SDL_Texture* texture;
	SDL_Rect indicator_rect;
	string nametag;

	PlayerIndicator();
	PlayerIndicator(SDL_Renderer *renderer, PlayerInfo *player_info, string nametag = "");
};

class Timer {
public:
	int remaining;
	SDL_Texture* texture;
	SDL_Rect timer_rect;

	Timer();
	Timer(SDL_Renderer* renderer, int remaining);
};