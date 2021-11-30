#include "TitleScreen.h"

#include <SDL.h>

#include "GameTexture.h"
#include "Debugger.h"

extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;

void title_screen_main(GameManager* game_manager) {
	PlayerInfo player_info[2];
	player_info[0] = game_manager->player_info[0];
	player_info[1] = game_manager->player_info[1];

    TitleScreen title_screen;

	title_screen.looping = game_manager->looping;

    const Uint8 *keyboard_state;
    Debugger debugger;

	game_manager->set_menu_info(&title_screen);

	SDL_Texture* pScreenTexture = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);

    while (*game_manager->looping) {
		SDL_SetRenderTarget(g_renderer, pScreenTexture);
		SDL_RenderClear(g_renderer);
		frameTimeDelay();

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

		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(nullptr);

		for (int i = 0; i < BUTTON_DEBUG_MAX; i++) {
			bool old_button = debugger.button_info[i].button_on;
			debugger.button_info[i].button_on = keyboard_state[debugger.button_info[i].mapping];
			bool new_button = debugger.button_info[i].button_on;
			debugger.button_info[i].changed = (old_button != new_button);
		}
		if (debugger.check_button_trigger(BUTTON_DEBUG_FULLSCREEN)) {
			if (SDL_GetWindowFlags(g_window) & SDL_WINDOW_FULLSCREEN_DESKTOP) {
				SDL_SetWindowFullscreen(g_window, 0);
			}
			else {
				SDL_SetWindowFullscreen(g_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			}
		}

		game_manager->handle_menus();

		title_screen.render();

		SDL_SetRenderTarget(g_renderer, nullptr);
		SDL_RenderCopy(g_renderer, pScreenTexture, nullptr, nullptr);
		SDL_RenderPresent(g_renderer);
	}
	SDL_DestroyTexture(pScreenTexture);

	return game_manager->update(player_info, GAME_STATE_MENU);
}

TitleScreen::TitleScreen(){
    title_l1.init("resource\\ui\\title\\title-l1.png");
    title_l2.init("resource\\ui\\title\\title-l2.png");
    title_l3.init("resource\\ui\\title\\title-l3.png");
    title_l4.init("resource\\ui\\title\\title-l4.png");
    text.init("resource\\ui\\title\\Praeiudicium.png");

    title_l1.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);
    title_l2.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);
    title_l3.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);
    title_l4.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);
    text.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_CENTER);
    text.destRect.x = WINDOW_WIDTH/2;
    text.destRect.y = WINDOW_HEIGHT/2;
}

void TitleScreen::render(){
    title_l4.render();
    title_l3.render();
    title_l2.render();
    title_l1.render();
    text.render();
}

void TitleScreen::event_start_press(){
	*looping = false;
}