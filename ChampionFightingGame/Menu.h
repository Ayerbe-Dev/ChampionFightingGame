#pragma once
#include <SDL.h>
#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include "Button.h"
#include "PlayerInfo.h"

int menu_main(SDL_Renderer* pRenderer, SDL_Window *window, PlayerInfo player_info[2]);
int chara_select_main(SDL_Renderer* pRenderer, SDL_Window *window, PlayerInfo player_info[2]);

class MenuItem{
public:
    int destination;
    SDL_Texture* texture;
    SDL_Rect destRect;

    MenuItem();
    MenuItem(string texture_dir, SDL_Renderer *pRenderer);
};