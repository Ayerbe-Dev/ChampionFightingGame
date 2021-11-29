#include "Opening.h"
extern SDL_Renderer* g_renderer;

bool opening_main(GameManager* game_manager) {
	PlayerInfo player_info[2];
	player_info[0] = game_manager->player_info[0];
	player_info[1] = game_manager->player_info[1];

	return displayOpeningSplash(player_info);
}

bool displayOpeningSplash(PlayerInfo player_info[2]) {
	const Uint8* keyboard_state;
	SDL_RenderClear(g_renderer);
	GameTexture titleSplash;
	titleSplash.init("resource/ui/menu/opening/game-splash-background.png");
	titleSplash.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);

	GameTexture textSplash;
	textSplash.init("resource/ui/menu/opening/game-splash-text.png");
	textSplash.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);

	u8 title_alpha = 0;
	u8 text_alpha = 0;
	bool opening = true;
	int fade_count = 0;
	int fade_state = 0;

	while (opening) {
		frameTimeDelay();
		frameTimeDelay();
		frameTimeDelay();
		frameTimeDelay();

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
				{
					return false;
				}
				break;
			}
		}
		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);

		for (int i = 0; i < 2; i++) {
			player_info[i].update_controller();
			player_info[i].update_buttons(keyboard_state);
			if (player_info[i].is_any_inputs()) {
				goto SKIP_INTRO;
			}
		}

		SDL_RenderClear(g_renderer);

		if (fade_state == 0) {
			title_alpha += 10;
			fade_count++;
			if (fade_count == 24) {
				fade_state++;
				fade_count = 0;
			}
		}
		else if (fade_state == 1) {
			text_alpha += 5;
			fade_count++;
			if (fade_count == 50) {
				fade_state++;
				fade_count = 0;
			}
		}
		else {
			title_alpha -= 10;
			text_alpha -= 10;
			fade_count++;
			if (fade_count == 24) {
				opening = false;
			}
		}

		titleSplash.setAlpha(title_alpha);
		textSplash.setAlpha(text_alpha);
		titleSplash.render();
		textSplash.render();

		SDL_RenderPresent(g_renderer);
	}


	SKIP_INTRO:
	textSplash.clearTexture();
	titleSplash.clearTexture();
	return true;
}