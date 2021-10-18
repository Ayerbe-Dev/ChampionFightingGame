#include "Menu.h"
#include "utils.h"
#include "PlayerInfo.h"
#include <SDL.h>
extern bool debug;
extern u32 frame_advance_ms;
extern u32 frame_advance_entry_ms;
extern u32 tick;
extern u32 tok;
extern int error_render;

int menu_main(SDL_Renderer* pRenderer, PlayerInfo player_info[2]) {
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

		for (int i = 0; i < 2; i++) {
			(&player_info[i])->update_buttons(keyboard_state);
		}

		SDL_RenderPresent(pRenderer);
	}

	return GAME_STATE_CHARA_SELECT;
}

int chara_select_main(SDL_Renderer* pRenderer, PlayerInfo player_info[2]) {
	bool chara_selecting = true;
	
	const Uint8* keyboard_state;
	tick = SDL_GetTicks();

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
		for (int i = 0; i < 2; i++) {
			(&player_info[i])->update_buttons(keyboard_state);
		}

		SDL_RenderPresent(pRenderer);
	}
	
	return GAME_STATE_GAME;
}