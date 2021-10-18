#pragma once
#include "SDL.h"
#include <string>
#include <iostream>
#include <functional>
#include <vector>

struct PlayerChoice {
	int chara_kind;
	int color;
//	string controls;
};

int menu_main(SDL_Renderer* renderer);
int chara_select_main(SDL_Renderer* renderer, PlayerChoice *p1_choice, PlayerChoice *p2_choice);
