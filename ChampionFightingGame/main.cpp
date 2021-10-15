#include <windows.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>
#include "utils.h"
#include "Game.h"
#include "TestFighter.h"
#include "Animation.h"
#undef main
using namespace std;
int error_render;

Uint32 tick;
Uint32 tok;

SDL_Texture* pBG;

int main() {
	bool running = true;
	bool visualize_boxes = true;
	//init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
	}

	SDL_Window* window = SDL_CreateWindow("Champions of the Ring", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	//Background image
	pBG = loadTexture("resource/stage/training_room/background.png", renderer);

	//init players
	PlayerInfo player_info[2];

//	TestFighter p1{0};
//	TestFighter p2{1};
	//why doesnt the above work
	PlayerInfo p1{ 0, "roy", renderer}; //bro just use the characters that are already there lmao
	PlayerInfo p2{ 1, "eric", renderer}; //like you can just make the child classes for roy and eric

	player_info[0] = p1;
	player_info[1] = p2;

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
		SDL_RenderCopy(renderer, pBG, nullptr, nullptr);
		for (int i = 0; i < 2; i++) {
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			if (player_info[i].situation_kind == CHARA_SITUATION_GROUND && player_info[i].is_actionable()) {
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
			else if (!player_info[i].facing_right) {
				flip = SDL_FLIP_HORIZONTAL;
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
		check_attack_connections(&player_info[0], &player_info[1], renderer, visualize_boxes);
		SDL_RenderPresent(renderer); 
	}

	SDL_DestroyRenderer(renderer);

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}