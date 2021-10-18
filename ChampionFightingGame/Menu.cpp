#include "Menu.h"
#include "utils.h"
#include <SDL.h>
extern bool debug;
extern u32 frame_advance_ms;
extern u32 frame_advance_entry_ms;
extern u32 tick;
extern u32 tok;
extern int error_render;

int menu_main(SDL_Renderer* pRenderer) {
	bool menuing = true;
	
	const Uint8* keyboard_state;
	tick = SDL_GetTicks();

	while (menuing) {
		SDL_RenderClear(pRenderer);
		SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: {
					return GAME_STATE_CLOSE;
				} break;
			}
		}

		//Frame delay

		tok = SDL_GetTicks() - tick;
		if (tok < TICK_RATE_MS) {
			SDL_Delay(TICK_RATE_MS - tok);
		}
		tick = SDL_GetTicks();

		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);

		if (keyboard_state[SDL_SCANCODE_RETURN]) {
			menuing = false;
		}
		SDL_RenderPresent(pRenderer);
	}

	return GAME_STATE_CHARA_SELECT;
}

int chara_select_main(SDL_Renderer *pRenderer, PlayerChoice *p1_choice, PlayerChoice *p2_choice) {
	bool chara_selecting = true;
	
	const Uint8* keyboard_state;
	tick = SDL_GetTicks();

	int current_character = 1;

	while (chara_selecting) {
		SDL_RenderClear(pRenderer);
		SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: {
					return GAME_STATE_CLOSE;
				} break;
			}
		}

		//Frame delay

		tok = SDL_GetTicks() - tick;
		if (tok < TICK_RATE_MS) {
			SDL_Delay(TICK_RATE_MS - tok);
		}
		tick = SDL_GetTicks();

		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);

		if (keyboard_state[SDL_SCANCODE_1]) {
			current_character = 1;
		}
		if (keyboard_state[SDL_SCANCODE_2]) {
			current_character = 2;
		}

		if (keyboard_state[SDL_SCANCODE_R]) {
			if (current_character == 1) {
				p1_choice->chara_kind = CHARA_KIND_ROY;
			}
			else {
				p2_choice->chara_kind = CHARA_KIND_ROY;
			}
		}
		if (keyboard_state[SDL_SCANCODE_E]) {
			if (current_character == 1) {
				p1_choice->chara_kind = CHARA_KIND_ERIC;
			}
			else {
				p2_choice->chara_kind = CHARA_KIND_ERIC;
			}
		}

		if (keyboard_state[SDL_SCANCODE_SPACE]) {
			chara_selecting = false;
		}
		SDL_RenderPresent(pRenderer);
	}
	
	return GAME_STATE_GAME;
}