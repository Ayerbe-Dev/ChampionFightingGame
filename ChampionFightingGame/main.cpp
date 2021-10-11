#include <windows.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>
#include "utils.h"
#include "Game.h"
#undef main
using namespace std;
bool running = true;
int error_render;

Uint32 tick;
Uint32 tok;

int main() {
	//init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
	}

	SDL_Window* window = SDL_CreateWindow("Champions of the Ring", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

	//init players
	PlayerInfo player_info[2];

	PlayerInfo p1 {"eric", renderer};
	p1.setStateLikePlayer1();
	p1.loadDefaultButtonMap();

	PlayerInfo p2{ "roy", renderer };
	p2.setStateLikePlayer2();
	p2.loadDefaultButtonMap();

	player_info[0] = p1;
	player_info[1] = p2;

	for (int i = 0; i < 60; i++) {
		loadAnimation(&ANIM_TABLE[i][0], renderer);
		loadAnimation(&ANIM_TABLE[i][1], renderer);
	}

	const Uint8* keyboard_state;
	tick = SDL_GetTicks();

	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT: {
				running = false;
			}
						 break;
			}
		}

		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);
		for (int i = 0; i < BUTTON_MAX; i++) {
			for (int o = 0; o < 2; o++) {
				bool old_button = player_info[o].button_info[i].button_on;
				player_info[o].button_info[i].button_on = keyboard_state[player_info[o].button_info[i].mapping];
				bool new_button = player_info[o].button_info[i].button_on;
				player_info[o].button_info[i].changed = (old_button != new_button);
			}
		}

		tok = SDL_GetTicks() - tick;
		//cout << "delta: " << tok << endl;
		if (tok < TICK_RATE_MS) {
			//cout << "delay: " << TICK_RATE_MS - tok << endl;
			SDL_Delay(TICK_RATE_MS - tok);
			//put code below in here
		}
		tick = SDL_GetTicks();

		SDL_RenderClear(renderer);
		for (int i = 0; i < 2; i++) {
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			if (player_info[i].situation_kind == CHARA_SITUATION_GROUND) {
				if (player_info[i].pos.x > player_info[!i].pos.x) {
					player_info[i].facing_dir = -1.0;
					player_info[i].facing_right = false;
					flip = SDL_FLIP_HORIZONTAL;
				}
				else {
					player_info[i].facing_dir = 1.0;
					player_info[i].facing_right = true;
				}
			}

			tickOnce(&player_info[i], renderer);

			SDL_Rect render_pos;
			render_pos.x = player_info[i].pos.getRenderCoodrinateX();
			render_pos.y = player_info[i].pos.getRenderCoodrinateY();
			render_pos.w = player_info[i].anim_kind->sprite_width;
			render_pos.h = player_info[i].anim_kind->sprite_height;
			const double angle = 0;
			error_render = SDL_RenderCopyEx(renderer, player_info[i].anim_kind->SPRITESHEET, &player_info[i].frame_rect, &render_pos, angle, NULL, flip);
			if (error_render != 0) {
				cout << "\n" << SDL_GetError();
			}
		}
		SDL_RenderPresent(renderer); 
	}

	SDL_DestroyRenderer(renderer);

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}