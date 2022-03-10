#include "Options.h"
#include <glew/glew.h>
extern SDL_Window* g_window;

void controls_main(GameManager* game_manager) {
	GameMenu* background_menu = game_manager->get_target();

	game_manager->layer++;
	PlayerInfo *player_info[2];
	player_info[0] = game_manager->player_info[0];
	player_info[1] = game_manager->player_info[1];

	const Uint8* keyboard_state;

	OptionsOverlay options_overlay = OptionsOverlay(500, 300, "resource/ui/menu/options/overlay.png");
	options_overlay.player_info[0] = player_info[0];
	options_overlay.player_info[1] = player_info[1];

	game_manager->set_menu_info(&options_overlay);

	while (*game_manager->looping[game_manager->layer]) {
		frameTimeDelay();
		glClearColor(0.1, 0.1, 0.1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
				{
					game_manager->layer--;
					return game_manager->update_state(GAME_STATE_CLOSE);
				}
				break;
			}
		}
		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);
		for (int i = 0; i < 2; i++) {
			player_info[i]->check_controllers();
			player_info[i]->poll_buttons(keyboard_state);
			if (player_info[i]->check_button_trigger(BUTTON_MENU_BACK)) {
				*game_manager->looping[game_manager->layer] = false;
			}
		}

		background_menu->process_background();
		options_overlay.panel.render();

		SDL_GL_SwapWindow(g_window);
	}

	options_overlay.panel.destroy();
	game_manager->layer--;
}

OptionsOverlay::OptionsOverlay() {

}

OptionsOverlay::OptionsOverlay(int width, int height, std::string dir) {
	panel.init(dir);
	panel.set_width(width);
	panel.set_height(height);
	panel.set_orientation(GAME_TEXTURE_ORIENTATION_MIDDLE_LEFT);
}