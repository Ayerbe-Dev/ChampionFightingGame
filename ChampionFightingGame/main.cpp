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

bool running = true;

#include "game_main.cpp"

int main() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
	}

	SDL_Window* window = SDL_CreateWindow("Champions of the Ring", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);


	PlayerInfo player_info[2];

	PlayerInfo p1 {"eric", renderer};
	p1.setStateLikePlayer1();

	PlayerInfo p2{ "roy", renderer };
	p2.setStateLikePlayer2();

	player_info[0] = p1;
	player_info[1] = p2;

	

	/*PlayerInfo player_info[2];
	player_info[0].buttons[BUTTON_UP].mapping = SDL_SCANCODE_UP;
	player_info[0].buttons[BUTTON_DOWN].mapping = SDL_SCANCODE_DOWN;
	player_info[0].buttons[BUTTON_LEFT].mapping = SDL_SCANCODE_LEFT;
	player_info[0].buttons[BUTTON_RIGHT].mapping = SDL_SCANCODE_RIGHT;
	player_info[0].buttons[BUTTON_START].mapping = SDL_SCANCODE_RETURN;

	player_info[1].buttons[BUTTON_UP].mapping = SDL_SCANCODE_W;
	player_info[1].buttons[BUTTON_DOWN].mapping = SDL_SCANCODE_S;
	player_info[1].buttons[BUTTON_LEFT].mapping = SDL_SCANCODE_A;
	player_info[1].buttons[BUTTON_RIGHT].mapping = SDL_SCANCODE_D;
	player_info[1].buttons[BUTTON_START].mapping = SDL_SCANCODE_SPACE;
	player_info[0].width = 0.1;
	player_info[0].height = 0.1;
	player_info[0].id = 0;
	player_info[0].chara_kind = "roy";

	player_info[1].id = 1;
	player_info[1].chara_kind = "eric";
	player_info[1].width = 0.1;
	player_info[1].height = 0.1;*/

	const Uint8* keyboard_state;

	while (running) {
		SDL_Event event;

		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: {
					running = false;
				}
				break;
				/*case SDL_KEYUP:
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
				break;*/
				default: {}
				break;
			}
		}

		SDL_RenderClear(renderer);

		for (int i = 0; i < 2; i++) {
			game_main(&player_info[i], renderer, keyboard_state);

			SDL_Rect render_pos;
			render_pos.x = player_info[i].pos.getRenderCoodrinateX();
			render_pos.y = player_info[i].pos.getRenderCoodrinateY();
			render_pos.w = player_info[i].width;
			render_pos.h = player_info[i].height;

			//SDL_QueryTexture(player_info[i].texture_instance, NULL, NULL, &render_pos.w, &render_pos.h);
			SDL_RenderCopy(renderer, player_info[i].default_texture, NULL, &render_pos);
			//SDL_DestroyTexture(player_info[i].texture_instance);
		}

		SDL_RenderPresent(renderer); 

		SDL_Delay(1000 / 60);
	}

	SDL_DestroyRenderer(renderer);

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}