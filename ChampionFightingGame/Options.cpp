#include "Options.h"
#include <glew/glew.h>
#include "RenderManager.h"
#include "TimeFuncs.h"

void controls_main() {
	GameManager* game_manager = GameManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();
	GameState* background_menu = game_manager->get_game_state();

	OptionsMenu* options_menu = new OptionsMenu;

	while (options_menu->looping) {
		wait_ms();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		render_manager->handle_window_events();
		options_menu->process_game_state();

		background_menu->process_background();
		options_menu->panel.render();

		render_manager->update_screen();
	}

	delete options_menu;
}

OptionsMenu::OptionsMenu() {
	GameManager* game_manager = GameManager::get_instance();

	panel.init("resource/game_state/menu/options/overlay.png");
	panel.set_width(500);
	panel.set_height(300);
	panel.set_orientation(SCREEN_TEXTURE_ORIENTATION_MIDDLE_LEFT);
}

OptionsMenu::~OptionsMenu() {
	panel.destroy();
}

void OptionsMenu::event_back_press() {
	looping = false;
}