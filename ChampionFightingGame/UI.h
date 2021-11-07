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
	SDL_Rect slice_rect;
	float max_health;
	int scale;
	float width;
	int height;

	HealthBar();
	HealthBar(FighterInstance* fighter_instance);

	void RenderAsP1();
	void RenderAsP2();
};

class PlayerIndicator {
public:
	FighterInstance* fighter_instance;
	SDL_Texture* texture;
	SDL_Rect indicator_rect;
	string nametag;

	PlayerIndicator();
	PlayerIndicator(FighterInstance *fighter_instance, string nametag = "");
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