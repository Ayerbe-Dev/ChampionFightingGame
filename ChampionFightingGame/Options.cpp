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

	OptionsMenu* options_menu = new OptionsMenu;
	options_menu->load_game_menu();

	while (*options_menu->looping) {
		wait_ms();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game_manager->handle_window_events();

		keyboard_state = SDL_GetKeyboardState(NULL);
		for (int i = 0; i < 2; i++) {
			player_info[i]->controller.check_controllers();
			player_info[i]->controller.poll_buttons(keyboard_state);
		}

		game_manager->handle_menus();

		background_menu->process_background();
		options_menu->panel.render();

		SDL_GL_SwapWindow(render_manager->window);
	}

	delete options_menu;
	game_manager->layer--;
}

OptionsMenu::OptionsMenu() {

}

OptionsMenu::~OptionsMenu() {
	panel.destroy();
}

void OptionsMenu::load_game_menu() {
	GameManager* game_manager = GameManager::get_instance();

	game_manager->set_menu_info(this);

	player_info[0] = game_manager->player_info[0];
	player_info[1] = game_manager->player_info[1];

	panel.init("resource/ui/menu/options/overlay.png");
	panel.set_width(500);
	panel.set_height(300);
	panel.set_orientation(GAME_TEXTURE_ORIENTATION_MIDDLE_LEFT);
}

void OptionsMenu::event_back_press() {
	*looping = false;
}