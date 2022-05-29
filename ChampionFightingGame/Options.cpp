#include "Options.h"
#include <glew/glew.h>
#include "RenderManager.h"

void controls_main() {
	GameManager* game_manager = GameManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();
	GameMenu* background_menu = game_manager->get_target();

	game_manager->layer++;
	PlayerInfo *player_info[2];
	player_info[0] = game_manager->player_info[0];
	player_info[1] = game_manager->player_info[1];

	const Uint8* keyboard_state;

	OptionsMenu options_menu(500, 300, "resource/ui/menu/options/overlay.png");
	options_menu.player_info[0] = player_info[0];
	options_menu.player_info[1] = player_info[1];

	game_manager->set_menu_info(&options_menu);

	while (*options_menu.looping) {
		wait_ms();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game_manager->handle_window_events();

		keyboard_state = SDL_GetKeyboardState(NULL);
		for (int i = 0; i < 2; i++) {
			player_info[i]->controller.check_controllers();
			player_info[i]->controller.poll_buttons(keyboard_state);
			if (player_info[i]->controller.check_button_trigger(BUTTON_MENU_BACK)) {
				*options_menu.looping = false;
			}
		}

		background_menu->process_background();
		options_menu.panel.render();

		SDL_GL_SwapWindow(render_manager->window);
	}

	options_menu.panel.destroy();
	game_manager->layer--;
}

OptionsMenu::OptionsMenu() {

}

OptionsMenu::OptionsMenu(int width, int height, std::string dir) {
	init(width, height, dir);
}

void OptionsMenu::init(int width, int height, std::string dir) {
	panel.init(dir);
	panel.set_width(width);
	panel.set_height(height);
	panel.set_orientation(GAME_TEXTURE_ORIENTATION_MIDDLE_LEFT);
}