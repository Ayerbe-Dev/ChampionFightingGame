#pragma once
#include "SDL.h"
#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include "Button.h"
#include "PlayerInfo.h"

int menu_main(SDL_Renderer* pRenderer, PlayerInfo player_info[2]);
int chara_select_main(SDL_Renderer* pRenderer, PlayerInfo player_info[2]);
