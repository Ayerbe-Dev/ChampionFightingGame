#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
int debugMenu(int iLastMenu, SDL_Renderer *pRenderer);
SDL_Texture *newFontTexture(std::string text, SDL_Renderer *pRenderer, TTF_Font *font);