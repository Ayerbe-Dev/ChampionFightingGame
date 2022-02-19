#pragma once
#include "Fighter.h"
#include "utils.h"
#include <string>
#include <SDL.h>

class HealthBar {
public:
	GameTextureNew health_texture;
	GameTextureNew bar_texture;
	float* health;
	float max_health;

	HealthBar();
	void render();
};

class ExBar {
public:
	Fighter* fighter;
	GameTextureNew ex_texture;
	GameTextureNew ex_segment_texture;
	GameTextureNew bar_texture;
	float max_ex;
	int prev_segments = 0;

	ExBar();
	ExBar(Fighter* fighter);
};

class PlayerIndicator {
public:
	Fighter* fighter;
	SDL_Texture* texture;
	SDL_Rect indicator_rect;
	string nametag;

	PlayerIndicator();
	PlayerIndicator(Fighter *fighter, string nametag = "");
};

class GameTimer{
public:
    int uiDecaseconds;
    int uiSeconds;
	int uiDecaframes;
	int uiFrames;
	int ClockMode;
	
    SDL_Texture* pBigTypeface;
    SDL_Texture* pSmallTypeface;
	SDL_Texture* pClockFace;
    GameTimer();
    GameTimer(int time);

	void Tick();
	void Render();
};