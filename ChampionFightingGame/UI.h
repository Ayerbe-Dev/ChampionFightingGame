#pragma once
#include "Fighter.h"
#include "utils.h"
#include <string>
#include <SDL2/SDL.h>

class HealthBar {
public:
	Fighter* fighter;
	GameTexture health_texture;
	GameTexture bar_texture;
	float max_health;

	HealthBar();
	HealthBar(Fighter* fighter);
};

class ExBar {
public:
	Fighter* fighter;
	GameTexture ex_texture;
	GameTexture ex_segment_texture;
	GameTexture bar_texture;
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