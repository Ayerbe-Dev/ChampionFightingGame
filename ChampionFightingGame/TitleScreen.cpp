#include "TitleScreen.h"
#include "GameTexture.h"
#include "Debugger.h"
#include <glew/glew.h>

extern SDL_Window* g_window;

void title_screen_main(GameManager* game_manager) {
	PlayerInfo *player_info[2];
	player_info[0] = game_manager->player_info[0];
	player_info[1] = game_manager->player_info[1];

    TitleScreen title_screen;

    const Uint8 *keyboard_state;
    Debugger debugger;

	game_manager->set_menu_info(&title_screen);

    while (game_manager->looping[game_manager->layer]) {
		wait_ms();

		glClearColor(0.0, 0.0, 0.0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
				{
					return game_manager->update_state(GAME_STATE_CLOSE);
				} break;
				case SDL_WINDOWEVENT:
				{
					switch (event.window.event) {
						case SDL_WINDOWEVENT_RESIZED:
						case SDL_WINDOWEVENT_SIZE_CHANGED:
						case SDL_WINDOWEVENT_MAXIMIZED:
						{
							int width;
							int height;
							SDL_GetWindowSize(g_window, &width, &height);
							glViewport(0, 0, width, height);
						} break;
					}
				} break;
			}
		}

		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(nullptr);
		for (int i = 0; i < 2; i++) {
			player_info[i]->check_controllers();
			player_info[i]->poll_buttons(keyboard_state);
		}

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

		SDL_GL_SwapWindow(g_window);
	}



	return game_manager->update_state(GAME_STATE_MENU);
}

TitleScreen::TitleScreen() {
    title_l1.init("resource/ui/title/title-l1.png");
    title_l2.init("resource/ui/title/title-l2.png");
    title_l3.init("resource/ui/title/title-l3.png");
    title_l4.init("resource/ui/title/title-l4.png");
    text.init("resource/ui/title/Praeiudicium.png");
}

TitleScreen::~TitleScreen() {
	title_l1.destroy();
	title_l2.destroy();
	title_l3.destroy();
	title_l4.destroy();
	text.destroy();
}

void TitleScreen::render(){
	title_l4.render();
	title_l3.render();
	title_l2.render();
	title_l1.render();
	text.render();
}

void TitleScreen::event_any_press(){
	*looping = false;
}