#include "Opening.h"
extern SDL_Renderer* g_renderer;

void opening_main(PlayerInfo player_info[2]) {
	displayOpeningSplash(player_info);
}

void displayOpeningSplash(PlayerInfo player_info[2]) {
	const Uint8* keyboard_state;
	SDL_RenderClear(g_renderer);
	GameTexture titleSplash;
	titleSplash.init("resource/ui/menu/game-splash-background.png");
	titleSplash.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);

	GameTexture textSplash;
	textSplash.init("resource/ui/menu/game-splash-text.png");
	textSplash.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);


	Uint8 tmp_alpha = 0;
	for (int i = 0; i < 25; i++) {
		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);

		for (int i = 0; i < 2; i++) {
			player_info[i].update_buttons(keyboard_state);
			if (player_info[i].is_any_inputs()) {
				goto SKIP_INTRO;
			}
		}
		SDL_RenderClear(g_renderer);
		tmp_alpha += 10;
		titleSplash.setAlpha(tmp_alpha);
		titleSplash.render();
		SDL_RenderPresent(g_renderer);
		frameTimeDelay();
		frameTimeDelay(); //do it again
		frameTimeDelay(); //and again
		frameTimeDelay(); //YET AGAIN
	}

	titleSplash.setAlpha((Uint8)255);
	tmp_alpha = 0;
	for (int i = 0; i < 51; i++) {
		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);

		for (int i = 0; i < 2; i++) {
			player_info[i].update_buttons(keyboard_state);
			if (player_info[i].is_any_inputs()) {
				goto SKIP_INTRO;
			}
		}
		SDL_RenderClear(g_renderer);
		tmp_alpha += 5;
		textSplash.setAlpha(tmp_alpha);
		titleSplash.render();
		textSplash.render();
		SDL_RenderPresent(g_renderer);
		frameTimeDelay();
		frameTimeDelay(); //do it again
		frameTimeDelay(); //and again
		frameTimeDelay(); //YET AGAIN
	}
	for (int i = 0; i < 25; i++) {
		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);

		for (int i = 0; i < 2; i++) {
			player_info[i].update_buttons(keyboard_state);
			if (player_info[i].is_any_inputs()) {
				goto SKIP_INTRO;
			}
		}
		SDL_RenderClear(g_renderer);
		tmp_alpha -= 10;
		textSplash.setAlpha(tmp_alpha);
		titleSplash.setAlpha(tmp_alpha);
		titleSplash.render();
		textSplash.render();
		SDL_RenderPresent(g_renderer);
		frameTimeDelay();
		frameTimeDelay(); //do it again
		frameTimeDelay(); //and again
		frameTimeDelay(); //YET AGAIN
	}
	SKIP_INTRO:
	textSplash.clearTexture();
	titleSplash.clearTexture();
}