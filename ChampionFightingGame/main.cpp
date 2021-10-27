#include <windows.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>
#include "utils.h"
#include "Game.h"
#include "Menu.h"
#include "Animation.h"
#include "Debugger.h"
#include "Stage.h"
#include "UI.h"
#include "DebugMenu.h"
#undef main
using namespace std;
int error_render;
int registered_controllers[4] = {-1, -1, -1, -1};

Uint32 tick;
Uint32 tok;
bool debug = false;

int main() {
	bool running = true;
	bool visualize_boxes = true;
	int game_state = GAME_STATE_DEBUG_MENU;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("Error initializing SDL: %s\n", SDL_GetError());
	}
	if (TTF_Init() < 0) {
		printf("Error initializing SDL_ttf: %s\n", TTF_GetError());
	}
	SDL_GameControllerEventState(SDL_ENABLE);

	SDL_Window* window = SDL_CreateWindow("Champions of the Ring", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);

	PlayerInfo player_info[2];
	player_info[0] = PlayerInfo(0);
	player_info[1] = PlayerInfo(1);

	while (running) {
		SDL_Renderer* pRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
		if (game_state == GAME_STATE_GAME) {
			game_state = game_main(pRenderer, player_info);
		}
		else if (game_state == GAME_STATE_MENU) {
			game_state = menu_main(pRenderer, player_info);
		}
		else if (game_state == GAME_STATE_CHARA_SELECT) {
			game_state = chara_select_main(pRenderer, player_info);
		}
		else if (game_state == GAME_STATE_CLOSE) {
			running = false;
		}
		else {
			game_state = debugMenu(pRenderer, player_info);
		}
		SDL_DestroyRenderer(pRenderer); 
	}

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}