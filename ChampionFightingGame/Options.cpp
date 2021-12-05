#include "Options.h"
extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;

void controls_main(GameManager* game_manager, SDL_Texture *background, GameMenu *background_menu) {
	PlayerInfo *player_info[2];
	player_info[0] = game_manager->player_info[0];
	player_info[1] = game_manager->player_info[1];

	SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);

	const Uint8* keyboard_state;
	SDL_Texture* pScreenTexture = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_SetTextureBlendMode(pScreenTexture, SDL_BLENDMODE_BLEND);

	OptionsOverlay options_overlay = OptionsOverlay(500, 300, "resource/ui/menu/options/overlay.png");
	options_overlay.player_info[0] = player_info[0];
	options_overlay.player_info[1] = player_info[1];
	
	bool substate_loop = true;
	while (substate_loop) {
		frameTimeDelay();
		SDL_SetRenderTarget(g_renderer, NULL);
		SDL_RenderClear(g_renderer);

		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
				{
					return game_manager->update(player_info, GAME_STATE_CLOSE);
				}
				break;
			}
		}
		for (int i = 0; i < 2; i++) {
			player_info[i]->check_controllers();
			player_info[i]->poll_buttons(keyboard_state);
			if (player_info[i]->check_button_trigger(BUTTON_MENU_BACK)) {
				substate_loop = false;
			}
		}
		background_menu->process_background(background);

		SDL_SetRenderTarget(g_renderer, pScreenTexture);
		SDL_RenderCopy(g_renderer, background, nullptr, nullptr);
		options_overlay.panel.render();
		SDL_SetRenderTarget(g_renderer, NULL);
		SDL_RenderCopy(g_renderer, pScreenTexture, NULL, NULL);
		SDL_RenderPresent(g_renderer);
	}

	SDL_DestroyTexture(pScreenTexture);
}

OptionsOverlay::OptionsOverlay() {

}

OptionsOverlay::OptionsOverlay(int width, int height, string dir) {
	panel.init(dir);
	panel.destRect.w = width;
	panel.destRect.h = height;
	panel.destRect.x = WINDOW_WIDTH / 2;
	panel.destRect.y = WINDOW_HEIGHT / 2;
}