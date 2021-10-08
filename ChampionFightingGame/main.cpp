#include <windows.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>
#include "utils.cpp"
#undef main
using namespace std;

global_variable bool running = true;

#include "game_main.cpp"

int main() {
	PlayerInfo player_info[2];

	player_info[0].buttons[BUTTON_UP].mapping = SDL_SCANCODE_UP;
	player_info[0].buttons[BUTTON_DOWN].mapping = SDL_SCANCODE_DOWN;
	player_info[0].buttons[BUTTON_LEFT].mapping = SDL_SCANCODE_LEFT;
	player_info[0].buttons[BUTTON_RIGHT].mapping = SDL_SCANCODE_RIGHT;
	player_info[0].buttons[BUTTON_START].mapping = SDL_SCANCODE_RETURN;
	player_info[0].id = 0;
	player_info[0].chara_kind = "roy";
	player_info[1].buttons[BUTTON_UP].mapping = SDL_SCANCODE_W;
	player_info[1].buttons[BUTTON_DOWN].mapping = SDL_SCANCODE_S;
	player_info[1].buttons[BUTTON_LEFT].mapping = SDL_SCANCODE_A;
	player_info[1].buttons[BUTTON_RIGHT].mapping = SDL_SCANCODE_D;
	player_info[1].buttons[BUTTON_START].mapping = SDL_SCANCODE_SPACE;
	player_info[1].id = 1;
	player_info[1].chara_kind = "eric";

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
	}

	SDL_Window* window = SDL_CreateWindow("Champions of the Ring", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

	while (running) {
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: {
					running = false;
				}
				break;
				case SDL_KEYUP:
				case SDL_KEYDOWN: {
					u32 key = (u32)event.key.keysym.scancode;
					bool pressed = (event.type == SDL_KEYDOWN);

					for (int i = 0; i < BUTTON_MAX; i++) {
						for (int o = 0; o < 2; o++) {
							if (player_info[o].buttons[i].mapping == key) {
								player_info[o].buttons[i].button_on = pressed;
								player_info[o].buttons[i].changed = true;
								break;
							}
						}
					}
				}
				break;
			}
		}

		SDL_RenderClear(renderer); 
		/*
			Clear BEFORE we run the player loop, since we don't want to clear it again between calling SDL_RenderCopy and actually rendering it
		*/

		for (int i = 0; i < 2; i++) {
			game_main(player_info[i], renderer);
			SDL_Rect render_pos;
			render_pos.x = player_info[i].pos_x;
			render_pos.y = player_info[i].pos_y;

			render_pos.w = 0.3;
			render_pos.h = 0.3;
			SDL_QueryTexture(player_info[i].texture_instance, NULL, NULL, &render_pos.w, &render_pos.h);

			SDL_RenderCopy(renderer, player_info[i].texture_instance, nullptr, &render_pos);
		}

		SDL_RenderPresent(renderer); 
		/*
			Since this part doesn't render anything, either I keep messing up the file directories for the sprites, or I'm missing something. Leaning
			towards the latter but I copied from multiple examples that all seemed to be exactly the same.
		*/

		SDL_Delay(1000 / 60);
	}

	for (int i = 0; i < 2; i++) {
		SDL_DestroyTexture(player_info[i].texture_instance);
	}

	SDL_DestroyRenderer(renderer);

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}