#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "PlayerInfo.h"
int debugMenu(SDL_Renderer* pRenderer, PlayerInfo player_info[2]);
SDL_Texture *newFontTexture(std::string text, SDL_Renderer *pRenderer, TTF_Font *font);